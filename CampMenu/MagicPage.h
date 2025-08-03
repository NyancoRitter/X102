#pragma once

#include "CampMenu.h"


/// <summary>
/// �L�����v��� - ���@UI
/// </summary>
class CampMenu::MagicPage final : public IGUI
{
public:
	MagicPage( CampMenu &Outer ) : m_Outer( Outer ) {}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
	virtual MagicPage &TopLeft( const Vec2i &TL ) override {	m_TopLeft=TL;	return *this;	}
	virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
protected:
	virtual void Paint_( HDC hdc ) const override;

private:

private:
	CampMenu &m_Outer;

	Vec2i m_TopLeft;
};
