#include "framework.h"
#include "TopLVMenu.h"
#include "IController.h"

using namespace GUI;
using namespace GUI::Menu;

CampMenu::TopLVMenu::TopLVMenu( CampMenu &Outer )
	: m_Outer( Outer )
	, m_CharSelMenuContent( Vec2i{ 16, 0 } )
	, m_CmdMenuContent( Vec2i{96,32} )
{
	m_CmdMenuContent
		.Add( { Text( L"Status" ).AlignCenter() } )
		.Add( { Text( L"Item" ).AlignCenter() } )
		.Add( { Text( L"Magic" ).AlignCenter() } )
		.CursorPos( 0 );
	m_CmdMenu.SetContent( &m_CmdMenuContent )
		.TopLeft( {168,2} )
		.OuterMargin( {12,4} )
		.WithFrame( true );

	//TestCode
	m_CharSelMenuContent
		.Add( GameContent::PartyCharID::AKINS )
		.Add( GameContent::PartyCharID::FANA )
		.Add( GameContent::PartyCharID::ENA_MEA )
		.CursorPos( 0 );
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

void CampMenu::TopLVMenu::Paint_( HDC hdc ) const
{
	m_CmdMenu.Paint( hdc );
	m_CharSelMenu.Paint( hdc );
}

Flags<GUI::GUIResult> CampMenu::TopLVMenu::Update( const IController &Controller )
{
	if( Controller.Cancel() ){	return GUIResult::Finished;	}

	HandleInputResult HIR = HandleInputResult::None;

	if( Controller.CursorUp() || Controller.CursorDown() )
	{	HIR = m_CharSelMenu.HandleInput( Controller );	}
	else if( Controller.CursorLeft() || Controller.CursorRight() )
	{	HIR = m_CmdMenu.HandleInput( Controller );	}

	return ( HIR==HandleInputResult::CursorMoved  ?  GUIResult::ReqRedraw  :  GUIResult::None );
}


