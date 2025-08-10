#pragma once

#include "IPage.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"

/// <summary>
/// キャンプ画面 - アイテムUI
/// </summary>
class CampMenu::ItemPage final : public CampMenu::IPage
{
public:
	ItemPage( CampMenu &Outer );

public:
	virtual void OnSelectedCharChanged( int iCharOrder ) override;
	virtual bool CanEnter() const override {	return true;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual void OnPrePopped() override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:

private:
	CampMenu &m_Outer;
	bool m_FocusState = false;

	std::vector< int > m_CharItems;
	std::vector< int > m_StockItems;

	GUI::Menu::Menu m_Menu[2];
	GUI::Menu::GenericMenuContent<true> m_Content[2];
};
