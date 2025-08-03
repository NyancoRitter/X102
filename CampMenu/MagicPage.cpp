#include "framework.h"
#include "MagicPage.h"
#include "IController.h"

using namespace GUI;

void CampMenu::MagicPage::OnGotFocus()
{
}

void CampMenu::MagicPage::OnLostFocus()
{
}

void CampMenu::MagicPage::Paint_( HDC hdc ) const
{
}

Flags<GUI::GUIResult> CampMenu::MagicPage::Update( const IController &Controller )
{
	if( Controller.Cancel() )return GUIResult::Finished;
	return GUIResult::None;
}
