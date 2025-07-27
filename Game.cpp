#include "framework.h"
#include <windowsx.h>
#include "Game.h"
#include "GlobalConst.h"
#include "Toyger/Toyger_IWnd.h"
#include "IController.h"

#include "Scene/TownScene/TownScene.h"

//===============================================

namespace
{
	//IController 実装
	class Controller : public IController
	{
	private:
		const InputState &m_rInput;
	public:
		Controller( const InputState &rInput ) : m_rInput(rInput) {}
	public:	// IController Impl
		//カーソルの上下左右移動
		virtual bool CursorUp() const override {	return Toyger::PosEdge( m_rInput, VK_UP );	}
		virtual bool CursorDown() const override {	return Toyger::PosEdge( m_rInput, VK_DOWN );	}
		virtual bool CursorLeft() const override {	return Toyger::PosEdge( m_rInput, VK_LEFT );	}
		virtual bool CursorRight() const override {	return Toyger::PosEdge( m_rInput, VK_RIGHT );	}
		//選択，キャンセル
		virtual bool Select() const override {	return Toyger::PosEdge( m_rInput, 'Z' );	}
		virtual bool Cancel() const override {	return Toyger::PosEdge( m_rInput, 'X' );	}
	};
}

//===============================================

Game::Game( Toyger::IWnd &Wnd )
	: m_rWnd( Wnd )
{}

Game::~Game()
{
	if( m_hDefaultFont != NULL ){	DeleteObject( m_hDefaultFont );	}
}

//初期処理
bool Game::Initialize()
{
	//デフォルトフォント生成
	m_hDefaultFont = CreateFontW(
		16,	//フォント高さ
		0,	//幅：0で自動
		0,0,	//回転角度関係
		FW_NORMAL,	//太さ
		0, 0, 0,	//イタリック，下線，取り消し線
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		L"ＭＳ ゴシック"
	);
	if( m_hDefaultFont==NULL )
	{	//※一応動作はできるのでメッセージだけ表示して処理は続行
		m_rWnd.MsgBox( L"デフォルトフォントの生成に失敗", L"ERR", MB_OK );
	}

	//ウィンドウ設定
	m_rWnd.SetCaption( L"Tiny DRPG Remake" );
	m_rWnd.SetViewContentSize( GlobalConst::GC_W, GlobalConst::GC_H );
	m_rWnd.ResizeToFit();

	//初期ステート設定
	m_upTownScene = std::make_unique< Town::TownScene >( *this );
	ChangeCurrSceneTo( m_upTownScene.get() );

	//初期状態描画
	m_rWnd.UpdateViewContent( *this );
	return true;
}

//描画
void Game::Paint( HDC hdc, int W, int H )
{
	//デフォルトのフォントをhdcに設定
	HFONT hOldFont = NULL;
	{
		if( m_hDefaultFont!=NULL )
		{	hOldFont = SelectFont( hdc, m_hDefaultFont );	}
	}
	
	//描画
	if( m_pCurrScene )m_pCurrScene->Draw( hdc );

	//フォント戻し
	if( hOldFont!=NULL )
	{	SelectFont( hdc, hOldFont );	}
}

//更新
bool Game::Update( const InputState &Input )
{
	//終了操作への対処
	if( Input.IsWndCloseOperationOccur() )
	{
		if( m_rWnd.MsgBox( L"終了しますか？", L"確認", MB_YESNO ) == IDYES )
		{	return false;	}
	}

	//
	auto Ret = m_pCurrScene->Update( Controller(Input) );
	if( Ret.Has( SceneUpdateResult::ReqAppQuit ) )return false;
	if( Ret.Has( SceneUpdateResult::ReqRedraw ) ){	m_rWnd.UpdateViewContent( *this );	}

	return true;
}

//シーン切り替え処理
void Game::ChangeCurrSceneTo( IScene *pScene )
{
	if( !pScene  ||  m_pCurrScene==pScene )return;

	if( m_pCurrScene )m_pCurrScene->OnLeave();
	m_pCurrScene = pScene;
	m_pCurrScene->OnEnter();
}

//メッセージボックス表示
void Game::ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const
{	m_rWnd.MsgBox( Msg.c_str(), Title.c_str(), MB_OK );	}
