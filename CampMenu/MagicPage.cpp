#include "framework.h"
#include "MagicPage.h"
#include "IController.h"

using namespace GUI;

CampMenu::MagicPage::MagicPage( CampMenu &Outer )
	: m_Outer( Outer )
	, m_UI( Vec2i{ 200, 24 } )
{}

void CampMenu::MagicPage::OnGotFocus()
{
	m_UI.OnGotFocus();
}

void CampMenu::MagicPage::OnLostFocus()
{
	m_UI.OnLostFocus();
}

void CampMenu::MagicPage::Paint_( HDC hdc ) const
{
	m_UI.Paint( hdc );
}

Flags<GUI::GUIResult> CampMenu::MagicPage::Update( const IController &Controller )
{
	auto Ret = m_UI.Update( Controller );
	
	if( Ret.Has( GUIResult::Finished ) )
	{	m_UI.OnLostFocus(); }
	
	return Ret;
}
