#pragma once

#include "IPage.h"
#include "Common/MagicSelUI.h"

/// <summary>
/// ƒLƒƒƒ“ƒv‰æ–Ê - –‚–@UI
/// </summary>
class CampMenu::MagicPage final : public CampMenu::IPage
{
public:
	MagicPage( CampMenu &Outer );

public:
	virtual void OnSelectedCharChanged( const GameContent::PartyChar &Char ){	m_UI.UpdateContent( Char );	}
	virtual bool CanEnter() const override {	return !m_UI.NoAvailableMagic();	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_UI.TopLeft();	}
	virtual MagicPage &TopLeft( const Vec2i &TL ) override {	m_UI.TopLeft(TL);	return *this;	}
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	CampMenu &m_Outer;

	Vec2i m_TopLeft;
	MagicSelUI m_UI;
};
