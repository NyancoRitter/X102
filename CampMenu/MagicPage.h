#pragma once

#include "IPage.h"
#include "Common/MagicSelUI.h"
#include "GUI/TextLinePainter.h"

/// <summary>
/// キャンプ画面 - 魔法UI
/// </summary>
class CampMenu::MagicPage final : public CampMenu::IPage
{
public:
	MagicPage( CampMenu &Outer );

	//魔法が使用された際の更新
	void UpdateOnMagicUsed( int iCurrCharOrder );

public:	// IPage Impl
	virtual void OnSelectedCharChanged( int iCharOrder ) override;
	virtual bool CanEnter() const override {	return !m_UI.NoAvailableMagic();	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnPushed() override;
	virtual void OnPrePopped() override;
	virtual Vec2i TopLeft() const override {	return m_UI.TopLeft();	}
	virtual MagicPage &TopLeft( const Vec2i &TL ) override {	m_UI.TopLeft(TL);	return *this;	}
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	Flags<GUI::GUIResult> OnIndicatedMagicChanged();
	Flags<GUI::GUIResult> OnMagicSelected();

	void UpdateDescView( const GameContent::Magic &TgtMagic );

private:
	CampMenu &m_Outer;

	Vec2i m_TopLeft;
	MagicSelUI m_UI;
	GUI::TextLinePainter m_DescView;

	inline static constexpr int DescViewXMargin = 6;
};
