#include "framework.h"
#include "ItemPage.h"
#include "IController.h"

using namespace GUI;

void CampMenu::ItemPage::OnGotFocus()
{
}

void CampMenu::ItemPage::OnLostFocus()
{
}

void CampMenu::ItemPage::Paint_( HDC hdc ) const
{
}

Flags<GUI::GUIResult> CampMenu::ItemPage::Update( const IController &Controller )
{
	if( Controller.Cancel() )return GUIResult::Finished;
	return GUIResult::None;
}
