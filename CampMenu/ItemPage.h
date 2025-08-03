#pragma once

#include "CampMenu.h"

/// <summary>
/// キャンプ画面 - アイテムUI
/// </summary>
class CampMenu::ItemPage final : public IGUI
{
public:
	ItemPage( CampMenu &Outer ) : m_Outer( Outer ) {}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
	virtual ItemPage &TopLeft( const Vec2i &TL ) override {	m_TopLeft=TL;	return *this;	}
	virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装
protected:
	virtual void Paint_( HDC hdc ) const override;

private:

private:
	CampMenu &m_Outer;

	Vec2i m_TopLeft;
};
