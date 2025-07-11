#pragma once
#include "Toyger_IWnd.h"

namespace Toyger
{
	class IWndInputListener;
	class ScrollBarHandling;

	/// <summary>ウィンドウ実装</summary>
	class GameWnd : public IWnd
	{
	public:
		/// <summary>ctor.</summary>
		/// <param name="rInputListener">
		/// 入力リスナをここで指定する
		///		<remarks>※型から推測できる通り，このリスナの寿命はウィンドウよりも長い必要がある</remarks>
		/// </param>
		GameWnd( IWndInputListener &rInputListener );

		/// <summary>dtor. DstroyWnd()が呼ばれる．</summary>
		virtual ~GameWnd();

	public:
		/// <summary>
		/// ウィンドウ生成
		///		<remarks>既にウィンドウが生成されいる場合には何もせずにtrueを返す</remarks>
		/// </summary>
		/// <param name="pCaptionStr">タイトルテキスト</param>
		/// <param name="ResizableBorder">サイズ変更可能なウィンドウとするか否か</param>
		/// <param name="left">場所</param>
		/// <param name="top">場所</param>
		/// <param name="width">大きさ</param>
		/// <param name="height">大きさ</param>
		/// <returns>成否</returns>
		bool CreateWnd(
			LPCTSTR pCaptionStr,
			bool Resizable,
			int left = CW_USEDEFAULT,
			int top = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
			);

		/// <summary>ウィンドウ破棄</summary>
		/// <remarks>ウィンドウが生成されていない場合には何もせずにtrueを返す</remarks>
		/// <returns>成否</returns>
		bool DestroyWnd();

		/// <summary>ウィンドウ生成されているか</summary>
		bool IsWndCreated() const {	return (m_hWnd != NULL);	}

		/// <summary>ウィンドウが表示状態か</summary>
		/// <returns>
		/// 表示状態であればtrue．
		/// ただし，ウィンドウが生成されていない場合にはfalseを返す．
		/// </returns>
		bool IsWndVisible() const;

		/// <summary>ウィンドウ表示状態の変更</summary>
		/// <param name="bShow">trueを指定すると表示状態に，falseを指定すると非表示状態にする</param>
		/// <remarks>ウィンドウが生成されていない場合には何もしない</remarks>
		void ShowWnd( bool bShow );

	public:	//IWnd実装

		virtual bool SetViewContentSize( int W, int H ) override;
		virtual bool SetViewMagRate( int Rate ) override;
		virtual int GetViewMagRate() const override {	return m_ViewMagRate;	}
		virtual bool ResizeToFit() override;
		virtual bool UpdateViewContent( IContentPainter &rPainter ) override;
		//virtual HBITMAP CloneCurrentViewContent() const override;

		virtual bool ScrollTo( int cx, int cy ) override ;
		virtual int ToClientAreaX( int ContentX ) const override;
		virtual int ToClientAreaY( int ContentY ) const override;
		virtual int ToContentX( int ClientAreaX ) const override;
		virtual int ToContentY( int ClientAreaY ) const override;

		virtual bool SetCaption( LPCTSTR pStr ) override;
		virtual bool MoveTo( int x, int y ) override;
		virtual int MsgBox( LPCTSTR Msg, LPCTSTR Caption, UINT MsgBoxType ) override;
		virtual SIZE CalcTextDrawSize( LPCWSTR pStrU16, HFONT hFont ) const override;
		virtual bool ChangeIcon( HANDLE IconHandle, bool ForSmall ) override;

		virtual IBGMPlayer* BGMPlayer() override;

	private:	//private methods
		GameWnd( const GameWnd & ) = delete;	//複製の禁止
		GameWnd &operator =( const GameWnd & ) = delete;	//複製の禁止
		LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam );
		bool SetClientAreaSize( int W, int H );

	private:	//data
		HWND m_hWnd = NULL;	//ウィンドウハンドル or NULL
		IWndInputListener &m_rInputListener;	//ctorで指定された入力イベントリスナ
		std::unique_ptr<ScrollBarHandling> m_upSBHandler;	//スクロール関連処理用

		HBITMAP m_hViewContent = NULL;	//クライアント領域への表示内容
		int m_ViewContentW = 320;	//m_hViewContentのサイズ
		int m_ViewContentH = 240;	//m_hViewContentのサイズ
		int m_ViewMagRate = 1;	//表示拡大倍率

	private:	//static
		static bool RegisterWndClass();
		static LRESULT CALLBACK WndProc_s( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		static HINSTANCE GetHInstance(){	return ::GetModuleHandle( NULL );	}
		
		static bool ms_bRegisterClassFlag;	//RegisterClassしたことを覚えておくフラグ
		static LPCTSTR ms_WndClassName;	//ウィンドウクラス名

	private:
		static constexpr UINT MCITHREAD_ERR_MSG = (WM_APP+1);

		class MCI_BGM_Player;
		std::unique_ptr<MCI_BGM_Player> m_upMCIPlayer;
	};

}

