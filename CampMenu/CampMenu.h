#pragma once

#include "GUI/IGUI.h"
//#include "GUI/Menu.h"
//#include "GUI/MenuContent.h"
//#include "GUI/TextLinePainter.h"
//#include "GUI/ImgPainter.h"
#include "GUI/Menu.h"
#include "Common/MoneyView.h"
#include "Common/CharSelMenuContent.h"

//class CMonoBMP;

class PlayData;

class CampMenu final : public GUI::IGUI
{
public:
	CampMenu( PlayData &rPlayData );
	~CampMenu();

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	//virtual void OnGotFocus() override;

	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//Å¶égÇÌÇ»Ç¢ÇÃÇ≈Dummyé¿ëï
	virtual CampMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//Å¶égÇÌÇ»Ç¢ÇÃÇ≈Dummyé¿ëï
	virtual Vec2i Size() const override {	return Vec2i();	}	//Å¶égÇÌÇ»Ç¢ÇÃÇ≈Dummyé¿ëï
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	PlayData &m_rPlayData;

	MoneyView m_MoneyView;

	GUI::Menu::Menu m_CharSelMenu;
	GUI::Menu::CharSelMenuContent<true> m_CharSelMenuContent;
};

