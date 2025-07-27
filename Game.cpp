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
	//IController ����
	class Controller : public IController
	{
	private:
		const InputState &m_rInput;
	public:
		Controller( const InputState &rInput ) : m_rInput(rInput) {}
	public:	// IController Impl
		//�J�[�\���̏㉺���E�ړ�
		virtual bool CursorUp() const override {	return Toyger::PosEdge( m_rInput, VK_UP );	}
		virtual bool CursorDown() const override {	return Toyger::PosEdge( m_rInput, VK_DOWN );	}
		virtual bool CursorLeft() const override {	return Toyger::PosEdge( m_rInput, VK_LEFT );	}
		virtual bool CursorRight() const override {	return Toyger::PosEdge( m_rInput, VK_RIGHT );	}
		//�I���C�L�����Z��
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
	m_rWnd.ResizeToFit();

	//�����X�e�[�g�ݒ�
	m_upTownScene = std::make_unique< Town::TownScene >( *this );
	ChangeCurrSceneTo( m_upTownScene.get() );

	//������ԕ`��
	m_rWnd.UpdateViewContent( *this );
	return true;
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

	//
	auto Ret = m_pCurrScene->Update( Controller(Input) );
	if( Ret.Has( SceneUpdateResult::ReqAppQuit ) )return false;
	if( Ret.Has( SceneUpdateResult::ReqRedraw ) ){	m_rWnd.UpdateViewContent( *this );	}

	return true;
}

//�V�[���؂�ւ�����
void Game::ChangeCurrSceneTo( IScene *pScene )
{
	if( !pScene  ||  m_pCurrScene==pScene )return;

	if( m_pCurrScene )m_pCurrScene->OnLeave();
	m_pCurrScene = pScene;
	m_pCurrScene->OnEnter();
}

//���b�Z�[�W�{�b�N�X�\��
void Game::ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const
{	m_rWnd.MsgBox( Msg.c_str(), Title.c_str(), MB_OK );	}
