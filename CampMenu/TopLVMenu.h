#pragma once

#include "CampMenu.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "Common/CharSelMenuContent.h"

/// <summary>
/// �L�����v��� - �L�����N�^�ƃR�}���h��I��UI
/// </summary>
class CampMenu::TopLVMenu final : public IGUI
{
public:
	TopLVMenu( CampMenu &Outer, std::vector<std::wstring> CmdMenuItemStrs );

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
	virtual TopLVMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
	virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	void UpdateCharSelMenuContent();
private:
	CampMenu &m_Outer;

	GUI::Menu::Menu m_CharSelMenu;
	GUI::Menu::CharSelMenuContent<true> m_CharSelMenuContent;

	GUI::Menu::Menu m_CmdMenu;
	GUI::Menu::GenericMenuContent<false> m_CmdMenuContent;
};
