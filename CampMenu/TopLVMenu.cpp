#include "framework.h"
#include "TopLVMenu.h"
#include "IController.h"
#include "GlobalConst.h"
#include "PlayData/PlayData.h"
#include "Common/CharDrawColor.h"

using namespace GUI;
using namespace GUI::Menu;

CampMenu::TopLVMenu::TopLVMenu( CampMenu &Outer, std::vector<std::wstring> CmdMenuItemStrs )
	: m_Outer( Outer )
	, m_CharSelMenuContent( Vec2i{ 16, 0 } )
	, m_CmdMenuContent( Vec2i{96,32} )
{

	for( const auto &ItemStr : CmdMenuItemStrs )
	{
		m_CmdMenuContent.Add( { Text( ItemStr ).AlignCenter() } );
	}
	m_CmdMenuContent.CursorPos( 0 );
	m_CmdMenu.SetContent( &m_CmdMenuContent )
		.TopLeft( {168,2} )
		.OuterMargin( {12,4} )
		.WithFrame( true );

	UpdateCharSelMenuContent();
	m_CharSelMenuContent.CursorPos( 0 );
	m_CharSelMenu.SetContent( &m_CharSelMenuContent )
		.TopLeft( Vec2i{ 4, 64 } ).ItemSpacing(6);
}

void CampMenu::TopLVMenu::OnGotFocus()
{
	m_CmdMenu.IsFocused( true );
	m_CharSelMenu.IsFocused( true );
}

void CampMenu::TopLVMenu::OnLostFocus()
{
	m_CmdMenu.IsFocused( false );
	m_CharSelMenu.IsFocused( false );
}

//描画
void CampMenu::TopLVMenu::Paint_( HDC hdc ) const
{
	m_CmdMenu.Paint( hdc );
	m_CharSelMenu.Paint( hdc );

	if( m_CharSelMenu.IsFocused()  &&  m_CharSelMenuContent.nItems()>=2 )
	{
		auto MenuBB = m_CharSelMenu.BoundingRect();

		RECT TextRect;
		TextRect.left = MenuBB.Left();
		TextRect.right = MenuBB.Right();
		TextRect.top = MenuBB.Bottom() + 16;
		TextRect.bottom = GlobalConst::GC_H;
			
		SetTextColor( hdc, RGB(255,255,255) );
		SetBkMode( hdc, TRANSPARENT );
		DrawTextW( hdc, L"※(B):最後尾へ", -1, &TextRect, DT_NOCLIP|DT_SINGLELINE|DT_CENTER|DT_TOP );
	}
}

//更新
Flags<GUI::GUIResult> CampMenu::TopLVMenu::Update( const IController &Controller )
{
	//パーティ並び順変更
	if( int Tgt = m_CharSelMenuContent.CursorPos();	( Tgt>=0 && Controller.MoveItemToBack() ) )
	{
		auto &PD = m_Outer.m_rPlayData;
		auto Party = PD.CurrParty();

		if( Party.MoveItemToBack( Tgt ) )
		{
			PD.SetupParty( Party.DataClone() );
			UpdateCharSelMenuContent();
			m_Outer.OnTopLVMenuCursorMoved( m_CharSelMenuContent.CursorPos(), m_CmdMenuContent.CursorPos() );
			return GUIResult::ReqRedraw;
		}
	}

	//
	const HandleInputResult HIR = 
		( Controller.CursorUp() || Controller.CursorDown() ) ?
		m_CharSelMenu.HandleInput( Controller ) :
		m_CmdMenu.HandleInput( Controller );

	switch( HIR )
	{
	case HandleInputResult::Canceled:
		return GUIResult::Finished;	break;
	case HandleInputResult::CursorMoved:
		m_Outer.OnTopLVMenuCursorMoved( m_CharSelMenuContent.CursorPos(), m_CmdMenuContent.CursorPos() );
		return GUIResult::ReqRedraw;
		break;
	case HandleInputResult::Selected:
		m_Outer.OnTopLVMenuSelected( m_CharSelMenuContent.CursorPos(), m_CmdMenuContent.CursorPos() );
		return GUIResult::ReqRedraw;
		break;
	default:
		break;
	}

	return GUIResult::None;
}

//m_CharSelMenuContentの内容を現在のデータに合わせる
void CampMenu::TopLVMenu::UpdateCharSelMenuContent()
{
	const auto &PD = m_Outer.m_rPlayData;

	int CursorPos = m_CharSelMenuContent.CursorPos();
	m_CharSelMenuContent.Clear();

	for( auto CharID : PD.CurrParty() )
	{
		const auto &Char = PD.Char( CharID );

		m_CharSelMenuContent.Add( CharID )
			.HPInfo( Char.HP(), Char.MaxHP() )
			.DrawColor( CharDrawColor( Char.HP(), Char.PoisonInfected() ) );
	}

	m_CharSelMenuContent.CursorPos( CursorPos );
}
