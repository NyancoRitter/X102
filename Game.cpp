#include "framework.h"
#include <windowsx.h>
#include "Game.h"
#include "GlobalConst.h"
#include "Toyger/Toyger_IWnd.h"
#include "IController.h"

#include "Scene/TownScene/TownScene.h"
#include "PlayData/PlayData.h"

//===============================================

namespace
{
	//IController ����
	class Controller : public IController
	{
	private:
		const InputState &m_rInput;
	public:
		Controller( const InputState &rInput ) : m_rInput(rInput) {}
	public:	// IController Impl
		//Test (�J�����e�X�g�����p)
		virtual bool TestKey() const {	return Toyger::PosEdge( m_rInput, 'T' );	}

		//�J�[�\���̏㉺���E�ړ�
		virtual bool CursorUp() const override {	return Toyger::PosEdge( m_rInput, VK_UP );	}
		virtual bool CursorDown() const override {	return Toyger::PosEdge( m_rInput, VK_DOWN );	}
		virtual bool CursorLeft() const override {	return Toyger::PosEdge( m_rInput, VK_LEFT );	}
		virtual bool CursorRight() const override {	return Toyger::PosEdge( m_rInput, VK_RIGHT );	}
		//�I���C�L�����Z��
		virtual bool Select() const override {	return Toyger::PosEdge( m_rInput, 'Z' );	}
		virtual bool Cancel() const override {	return Toyger::PosEdge( m_rInput, 'X' );	}
		//�p�[�e�B���j���[���J��
		virtual bool OpenPartyMenu() const override {	return Toyger::PosEdge( m_rInput, 'C' ) || Toyger::PosEdge( m_rInput, 'M' );	}

		//�A�C�e���́@�L�����N�^�����X�g�b�N�@�ړ�
		virtual bool MoveItem() const override {	return Toyger::PosEdge( m_rInput, 'S' );	}
		//�A�C�e�����̐����i��Ԍ��Ɉړ�������j
		virtual bool MoveItemToBack() const override {	return Toyger::PosEdge( m_rInput, 'B' );	}

		//�e�L�X�g����
		virtual bool ToNextText() const override {	return Select() || Cancel();	}
		//���H���ړ�
		virtual bool MoveFront() const override {	return Toyger::Pressed( m_rInput, VK_UP );	}
		virtual bool MoveBack() const override {	return Toyger::Pressed( m_rInput, VK_DOWN );	}
		virtual bool TurnLeft() const override {	return Toyger::Pressed( m_rInput, VK_LEFT );	}
		virtual bool TurnRight() const override {	return Toyger::Pressed( m_rInput, VK_RIGHT );	}
		//���H���Œ��ׂ�
		virtual bool Inspect() const override {	return Select();	}
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

//��������
bool Game::Initialize()
{
	//�f�t�H���g�t�H���g����
	m_hDefaultFont = CreateFontW(
		16,	//�t�H���g����
		0,	//���F0�Ŏ���
		0,0,	//��]�p�x�֌W
		FW_NORMAL,	//����
		0, 0, 0,	//�C�^���b�N�C�����C��������
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		L"�l�r �S�V�b�N"
	);
	if( m_hDefaultFont==NULL )
	{	//���ꉞ����͂ł���̂Ń��b�Z�[�W�����\�����ď����͑��s
		m_rWnd.MsgBox( L"�f�t�H���g�t�H���g�̐����Ɏ��s", L"ERR", MB_OK );
	}

	//�E�B���h�E�ݒ�
	m_rWnd.SetCaption( L"Tiny DRPG Remake" );
	m_rWnd.SetViewContentSize( GlobalConst::GC_W, GlobalConst::GC_H );
	ChangeViewMagRate( 1 );

	//(��)
	m_upPlayData = std::make_unique<PlayData>();

	//�����X�e�[�g�ݒ�
	ChangeToTownScene();

	//������ԕ`��
	m_rWnd.UpdateViewContent( *this );
	return true;
}

//�\���{���ύX
void Game::ChangeViewMagRate( int Rate )
{
	m_rWnd.SetViewMagRate( Rate );
	m_rWnd.ResizeToFit();
}

//�`��
void Game::Paint( HDC hdc, int W, int H )
{
	//�f�t�H���g�̃t�H���g��hdc�ɐݒ�
	HFONT hOldFont = NULL;
	{
		if( m_hDefaultFont!=NULL )
		{	hOldFont = SelectFont( hdc, m_hDefaultFont );	}
	}
	
	//�`��
	if( m_pCurrScene )m_pCurrScene->Draw( hdc );

	//�t�H���g�߂�
	if( hOldFont!=NULL )
	{	SelectFont( hdc, hOldFont );	}
}

//�X�V
bool Game::Update( const InputState &Input )
{
	//�I������ւ̑Ώ�
	if( Input.IsWndCloseOperationOccur() )
	{
		if( m_rWnd.MsgBox( L"�I�����܂����H", L"�m�F", MB_YESNO ) == IDYES )
		{	return false;	}
	}

	//�\���{��
	if( Toyger::PosEdge( Input, '1' ) ){	ChangeViewMagRate( 1 );	}
	if( Toyger::PosEdge( Input, '2' ) ){	ChangeViewMagRate( 2 );	}

	//�X�V
	auto Ret = m_pCurrScene->Update( Controller(Input) );
	if( Ret.Has( SceneUpdateResult::ReqAppQuit ) )return false;
	if( Ret.Has( SceneUpdateResult::ReqRedraw ) ){	m_rWnd.UpdateViewContent( *this );	}

	return true;
}

//
PlayData &Game::CurrPlayData()
{
	if( !m_upPlayData )throw std::exception( "No PlayData" );
	return *m_upPlayData;
}

//�V�[���؂�ւ�����
void Game::ChangeCurrSceneTo( IScene *pScene )
{
	if( !pScene  ||  m_pCurrScene==pScene )return;

	if( m_pCurrScene )m_pCurrScene->OnLeave();
	m_pCurrScene = pScene;
	m_pCurrScene->OnEnter();
}

void Game::ChangeToTownScene()
{
	if( !m_upTownScene )
	{	m_upTownScene = std::make_unique< Town::TownScene >( *this );	}

	ChangeCurrSceneTo( m_upTownScene.get() );
}

//���b�Z�[�W�{�b�N�X�\��
void Game::ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const
{	m_rWnd.MsgBox( Msg.c_str(), Title.c_str(), MB_OK );	}
