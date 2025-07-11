#include "framework.h"	//VC

#include <windowsx.h>
#include <map>
#include <mmsystem.h>	//MCI関連のイベント識別子用

#include "Toyger_GameWnd.h"
#include "Toyger_IWndInputListener.h"
#include "Toyger_IContentPainter.h"
#include "Toyger_ScrollBarHandling.h"
#include "Toyger_IBMGPlayer.h"
#include "Parts/MCIThread.h"

namespace Toyger
{
	class GameWnd::MCI_BGM_Player : public Toyger::IBGMPlayer
	{
	private:
		CMCIThread m_MCIThread;

	private:
		MCI_BGM_Player( const MCI_BGM_Player & ) = delete;
		MCI_BGM_Player &operator=( const MCI_BGM_Player & ) = delete;
	public:
		MCI_BGM_Player( HWND hWnd )
		{	m_MCIThread.BeginThread( hWnd, MCITHREAD_ERR_MSG );	}

		~MCI_BGM_Player(){	m_MCIThread.EndThread();	}

		void RestartBGM(){	m_MCIThread.RestartBGM();	}
		std::wstring GetLastMCIErrMsg(){	return m_MCIThread.GetLastMCIErrMsg();	}

		// IBGMPlayer を介して継承されました
		bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong ) override
		{	return m_MCIThread.RegisterBMG( specifier, rBGMFilePath, ShouldKeepLong );	}

		bool StopBGM() override {	return m_MCIThread.StopBGM();	}
		bool ChangeBGM( int specifier ) override {	return m_MCIThread.ChangeBGM( specifier );	}
	};


	//--------------------------------------------------------

	//static
	bool GameWnd::ms_bRegisterClassFlag = false;
	LPCTSTR GameWnd::ms_WndClassName = L"Toyger_GameWnd";

	//ctor
	GameWnd::GameWnd( IWndInputListener &rInputListener )
		: m_rInputListener( rInputListener )
	{}

	//dtor
	GameWnd::~GameWnd(){	DestroyWnd();	}

	//ウィンドウクラス登録
	bool GameWnd::RegisterWndClass()
	{
		if( ms_bRegisterClassFlag )return true;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)( GameWnd::WndProc_s );
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= GetHInstance();
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL,
		wcex.lpszClassName	= ms_WndClassName;
		wcex.hIconSm		= NULL;

		ms_bRegisterClassFlag = ( RegisterClassEx(&wcex) != 0 );
		return ms_bRegisterClassFlag;
	}

	//ウィンドウ生成
	bool GameWnd::CreateWnd(
		LPCTSTR pCaptionStr,
		bool Resizable,
		int left, int top,
		int width, int height
	)
	{
		if( IsWndCreated() )return true;
		if( !RegisterWndClass() )return false;

		DWORD Style = ( WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX )  |  ( WS_VSCROLL | WS_HSCROLL );
		if( !Resizable ){	Style &= ~WS_THICKFRAME;	}

		m_hWnd = CreateWindowEx(
			0, ms_WndClassName, pCaptionStr, Style,
			left, top, width, height,
			NULL, NULL, GetHInstance(),
			this
		);
		if( m_hWnd == NULL )return false;

		m_upSBHandler = std::make_unique<ScrollBarHandling>( m_hWnd );
		m_upSBHandler->ChangeContentSize( m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate );

		ShowWindow( m_hWnd, SW_SHOW );
		UpdateWindow( m_hWnd );
		return true;
	}

	//ウィンドウ破棄
	bool GameWnd::DestroyWnd()
	{
		if( !IsWndCreated() )return true;
		return ( ::DestroyWindow( m_hWnd ) != 0 );
	}

	//ウィンドウの 表示/非表示 状態
	bool GameWnd::IsWndVisible() const
	{
		if( !IsWndCreated() )return false;
		return ( ::IsWindowVisible( m_hWnd ) != 0 );
	}

	//ウィンドウの 表示/非表示
	void GameWnd::ShowWnd( bool bShow )
	{
		if( IsWndCreated() )
		{	::ShowWindow( m_hWnd, ( bShow ? SW_SHOW : SW_HIDE ) );	}
	}

	//ウィンドウプロシージャ（static）
	LRESULT CALLBACK GameWnd::WndProc_s(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static std::map< HWND, GameWnd* > HWND_2_pInst;

		GameWnd *pInst = nullptr;
		if( message == WM_CREATE )
		{
			pInst = (GameWnd*)(   ( (CREATESTRUCT*)lParam )->lpCreateParams   );
			HWND_2_pInst[hWnd] = pInst;
		}
		else
		{
			auto i = HWND_2_pInst.find( hWnd );
			if( i == HWND_2_pInst.end() )
			{	return ::DefWindowProc( hWnd, message, wParam, lParam );	}

			pInst = i->second;
		}

		if( message == WM_DESTROY )
		{	HWND_2_pInst.erase( hWnd );	}

		return pInst->WndProc( message, wParam, lParam );
	}

	//ウィンドウプロシージャ
	LRESULT GameWnd::WndProc( UINT message, WPARAM wParam, LPARAM lParam )
	{
		LRESULT Ret = 0;

		switch (message)
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			if( m_hViewContent ){	::DeleteObject( m_hViewContent );	m_hViewContent = NULL;	}
			if( m_upMCIPlayer ){	m_upMCIPlayer = nullptr;	}
			if( m_upSBHandler ){	m_upSBHandler = nullptr;	}
			m_hWnd = NULL;
			break;

		case WM_HSCROLL:
			if( m_upSBHandler )
			{
				m_upSBHandler->HandleScrollEvent( true, LOWORD(wParam), HIWORD(wParam) );
				InvalidateRect( m_hWnd, NULL, FALSE );
			}
			break;

		case WM_VSCROLL:
			if( m_upSBHandler )
			{
				m_upSBHandler->HandleScrollEvent( false, LOWORD(wParam), HIWORD(wParam) );
				InvalidateRect( m_hWnd, NULL, FALSE );
			}
			break;

		case MM_MCINOTIFY:
			if( wParam == MCI_NOTIFY_SUCCESSFUL )
			{	m_upMCIPlayer->RestartBGM();	}
			break;

		case MCITHREAD_ERR_MSG:
			this->MsgBox( m_upMCIPlayer->GetLastMCIErrMsg().c_str(), L"MCI ERR", MB_OK );
			break;

		case WM_SIZE:
			{
				int W = LOWORD(lParam);
				int H = HIWORD(lParam);
				m_upSBHandler->ChangeViewSize( W,H );
			}
			break;

		case WM_ERASEBKGND:
			{//コンテンツを表示しない箇所だけを塗りつぶすことで，コンテンツ表示領域のちらつきを抑える
				RECT ClientRect;
				::GetClientRect( m_hWnd, &ClientRect );
				HDC hdc = (HDC)(wParam);
				HBRUSH hbr = (HBRUSH)(GetStockObject(DKGRAY_BRUSH));

				if( ToClientAreaX( m_ViewContentW ) < ClientRect.right )
				{
					RECT rect = ClientRect;
					rect.left = ToClientAreaX( m_ViewContentW );
					::FillRect( hdc, &rect, hbr );
				}
				if( ToClientAreaY( m_ViewContentH ) < ClientRect.bottom )
				{
					RECT rect = ClientRect;
					rect.top = ToClientAreaY( m_ViewContentH );
					::FillRect( hdc, &rect, hbr );
				}

				Ret = 1;
			}
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint( m_hWnd, &ps );
				if( hdc!=NULL  &&  m_hViewContent!=NULL )
				{
					HDC memDC = ::CreateCompatibleDC( hdc );
					if( memDC )
					{
						int sx = 0, sy=0;
						if( m_upSBHandler )
						{
							sx = m_upSBHandler->GetScrollPos( true );
							sy = m_upSBHandler->GetScrollPos( false );
						}

						HBITMAP hOldBMP = SelectBitmap( memDC, m_hViewContent );
						if( m_ViewMagRate==1 )
						{	::BitBlt( hdc, -sx,-sy, m_ViewContentW, m_ViewContentH, memDC, 0,0, SRCCOPY );	}
						else
						{
							::StretchBlt(
								hdc, -sx, -sy, m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate,
								memDC, 0,0, m_ViewContentW, m_ViewContentH,
								SRCCOPY
							);
						}
						SelectBitmap( memDC, hOldBMP );
						::DeleteDC( memDC );
					}
				}
				EndPaint( m_hWnd, &ps );
			}
			break;

		case WM_KEYDOWN:	m_rInputListener.OnPress( wParam & 0xFF );	break;
		case WM_KEYUP:	m_rInputListener.OnRelease( wParam & 0xFF );	break;

		case WM_LBUTTONDOWN:	m_rInputListener.OnPress( VK_LBUTTON );	break;
		case WM_LBUTTONUP:	m_rInputListener.OnRelease( VK_LBUTTON );	break;
		
		case WM_MBUTTONDOWN:	m_rInputListener.OnPress( VK_MBUTTON );	break;
		case WM_MBUTTONUP:	m_rInputListener.OnRelease( VK_MBUTTON );	break;

		case WM_RBUTTONDOWN:	m_rInputListener.OnPress( VK_RBUTTON );	break;
		case WM_RBUTTONUP:	m_rInputListener.OnRelease( VK_RBUTTON );	break;

		case WM_MOUSEMOVE:	m_rInputListener.OnMouseMove( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );	break;

		case WM_CLOSE:
			if( m_rInputListener.OnWndCloseOP() )
			{	Ret = ::DefWindowProc( m_hWnd, message, wParam, lParam );	}
			break;

		default:
			Ret = ::DefWindowProc( m_hWnd, message, wParam, lParam );
			break;
		}

		return Ret;
	}

	//ウィンドウキャプション文字列
	bool GameWnd::SetCaption( LPCTSTR pStr )
	{
		if( pStr  &&  IsWndCreated() )
		{	return ( ::SetWindowText( m_hWnd, pStr ) != 0 );	}
		else
		{	return false;	}
	}
	
	//表示物サイズ設定
	bool GameWnd::SetViewContentSize( int W, int H )
	{
		if( !IsWndCreated() )return false;
		if( m_ViewContentW==W && m_ViewContentH==H )return true;

		HDC hdc = ::GetDC( m_hWnd );
		if( hdc )
		{
			m_ViewContentW = W;
			m_ViewContentH = H;
			if( m_hViewContent ){	::DeleteObject( m_hViewContent );	m_hViewContent = NULL;	}

			m_hViewContent = ::CreateCompatibleBitmap( hdc, W, H );
			::ReleaseDC( m_hWnd, hdc );
		}
		else{	return false;	}

		m_upSBHandler->ChangeContentSize( m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate );
		::InvalidateRect( m_hWnd, NULL, TRUE );
		return true;
	}

	//ResizeToFit
	bool GameWnd::ResizeToFit()
	{	return SetClientAreaSize( m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate );	}

	//表示拡大倍率変更
	bool GameWnd::SetViewMagRate( int Rate )
	{
		if( !IsWndCreated() )return false;

		if( Rate<1 )Rate = 1;
		if( m_ViewMagRate == Rate )return true;
		m_ViewMagRate = Rate;

		m_upSBHandler->ChangeContentSize( m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate );
		::InvalidateRect( m_hWnd, NULL, TRUE );
		return true;
	}

	//クライアント領域サイズ変更
	bool GameWnd::SetClientAreaSize( int W, int H )
	{
		if( !IsWndCreated() )return false;

		//※ SetWindowPos() によるウィンドウサイズ変更によってメニューの行数が変わったりする場合があるため，
		//　１回の SetWindowPos() では無理な場合がある．そのためてきとーな回数ループさせている
		for( int i=0; i<5; i++ )	//※最大でも２回で十分？？
		{
			RECT rect;
			if( ::GetClientRect( m_hWnd, &rect ) == 0 )return false;
			int dx = (int)W - rect.right;
			int dy = (int)H - rect.bottom;
			if( dx==0 && dy==0 )break;
			if( ::GetWindowRect( m_hWnd, &rect ) == 0 )return false;
			if( ::SetWindowPos( m_hWnd, NULL, 0,0, rect.right+dx-rect.left ,rect.bottom+dy-rect.top, SWP_NOMOVE|SWP_NOZORDER ) == 0 )return false;
		}
		return true;
	}

	//移動
	bool GameWnd::MoveTo( int x, int y )
	{
		if( !IsWndCreated() )return false;
		return ( ::SetWindowPos( m_hWnd, NULL, x,y, 0,0, SWP_NOSIZE|SWP_NOZORDER ) != 0 );
	}

	//描画
	bool GameWnd::UpdateViewContent( IContentPainter &rPainter )
	{
		if( !IsWndCreated() || m_hViewContent==NULL )return false;

		bool Ret = false;
		HDC hClientDC = ::GetDC( m_hWnd );
		if( hClientDC )
		{
			HDC memDC = ::CreateCompatibleDC( hClientDC );
			if( memDC )
			{
				HBITMAP hOldBMP = SelectBitmap( memDC, m_hViewContent );
				rPainter.Paint( memDC, m_ViewContentW, m_ViewContentH );

				int sx = 0, sy=0;
				if( m_upSBHandler )
				{
					sx = m_upSBHandler->GetScrollPos( true );
					sy = m_upSBHandler->GetScrollPos( false );
				}

				if( m_ViewMagRate==1 )
				{	::BitBlt( hClientDC, -sx,-sy, m_ViewContentW, m_ViewContentH, memDC, 0,0, SRCCOPY );	}
				else
				{
					::StretchBlt(
						hClientDC, -sx, -sy, m_ViewContentW*m_ViewMagRate, m_ViewContentH*m_ViewMagRate,
						memDC, 0,0, m_ViewContentW, m_ViewContentH,
						SRCCOPY
					);
				}

				SelectBitmap( memDC, hOldBMP );
				::DeleteDC( memDC );
				Ret = true;
			}
			::ReleaseDC( m_hWnd, hClientDC );
		}
		return Ret;
	}

	////表示内容のクローン
	//HBITMAP GameWnd::CloneCurrentViewContent() const
	//{
	//	if( m_hViewContent==NULL )return NULL;
	//	return (HBITMAP)::CopyImage( m_hViewContent, IMAGE_BITMAP, 0,0, 0 );
	//}

	//MsgBox
	int GameWnd::MsgBox( LPCTSTR Msg, LPCTSTR Caption, UINT MsgBoxType )
	{	return ::MessageBox( m_hWnd, Msg, Caption, MsgBoxType );	}

	//
	SIZE GameWnd::CalcTextDrawSize( LPCWSTR pStrU16, HFONT hFont ) const
	{
		SIZE Ret{ 0,0 };
		if( !IsWndCreated() )return Ret;

		HDC hdc = ::GetDC( m_hWnd );
		if( !hdc )return Ret;

		HFONT hOldFont = NULL;
		if( hFont!=NULL )
		{	hOldFont = SelectFont( hdc, hFont );	}

		GetTextExtentPoint32W( hdc, pStrU16, (int)wcslen( pStrU16 ), &Ret );
		
		if( hOldFont!=NULL )
		{	SelectFont( hdc, hOldFont );	}

		::ReleaseDC( m_hWnd, hdc );
		return Ret;
	}

	//
	bool GameWnd::ChangeIcon( HANDLE IconHandle, bool ForSmall )
	{
		if( !IsWndCreated() )return false;
		return ( ::PostMessage( m_hWnd, WM_SETICON, ( ForSmall ? ICON_SMALL : ICON_BIG ), LPARAM(IconHandle) ) != 0 );
	}

	//
	bool GameWnd::ScrollTo( int cx, int cy )
	{
		if( !IsWndCreated() )return false;
		m_upSBHandler->ScrollTo( cx*m_ViewMagRate, cy*m_ViewMagRate );
		::InvalidateRect( m_hWnd, NULL, TRUE );
		return true;
	}

	//
	int GameWnd::ToClientAreaX( int ContentX ) const {	return ContentX*m_ViewMagRate - ( m_upSBHandler ? m_upSBHandler->GetScrollPos(true) : 0 );	}
	int GameWnd::ToClientAreaY( int ContentY ) const {	return ContentY*m_ViewMagRate - ( m_upSBHandler ? m_upSBHandler->GetScrollPos(false) : 0 );	}
	int GameWnd::ToContentX( int ClientAreaX ) const {	return ( ClientAreaX + ( m_upSBHandler ? m_upSBHandler->GetScrollPos(true) : 0 ) )/m_ViewMagRate;	}
	int GameWnd::ToContentY( int ClientAreaY ) const {	return ( ClientAreaY + ( m_upSBHandler ? m_upSBHandler->GetScrollPos(false) : 0 ) )/m_ViewMagRate;	}

	//
	IBGMPlayer *GameWnd::BGMPlayer()
	{
		if( !IsWndCreated() )return nullptr;
		if( !m_upMCIPlayer )
		{	m_upMCIPlayer = std::make_unique<MCI_BGM_Player>( m_hWnd );	}

		return m_upMCIPlayer.get();
	}
}
