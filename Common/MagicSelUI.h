#pragma once

#include <vector>
#include <functional>
#include "GameContent/Spell.h"
#include "GUI/IGUI.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"

namespace GameContent{	class PartyChar;	}

/// <summary>
/// –‚–@‘I‘ð—p UI
/// </summary>
class MagicSelUI final : public GUI::IGUI
{
public:
	using OnIndicatedMagicChanged = std::function< Flags<GUI::GUIResult>(GameContent::FirstSpell,GameContent::SecondSpell) >;
	using OnMagicSelected = std::function< Flags<GUI::GUIResult>(GameContent::FirstSpell,GameContent::SecondSpell) >;

public:
	MagicSelUI( const Vec2i &ItemDrawSize );
	void UpdateContent( const GameContent::PartyChar &MagicUser );

	bool NoAvailableMagic() const {	return m_Spell1st.empty() || m_Spell2nd.empty();	}

	MagicSelUI &Set_OnIndicatedMagicChanged_Proc( OnIndicatedMagicChanged &Func ){	m_OnIndicatedMagicChanged=Func;	return *this;	}
	MagicSelUI &Set_OnMagicSelected_Proc( OnMagicSelected &Func ){	m_OnMagicSelected=Func;	return *this;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_Menu[0].TopLeft();	}
	virtual MagicSelUI &TopLeft( const Vec2i &TL ) override;
	virtual Vec2i Size() const override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	void SwitchFocusTo( int iMenu );
private:
	Vec2i m_TopLeft;
	OnIndicatedMagicChanged m_OnIndicatedMagicChanged;
	OnMagicSelected m_OnMagicSelected;

	std::vector< GameContent::FirstSpell > m_Spell1st;
	std::vector< GameContent::SecondSpell > m_Spell2nd;

	GUI::Menu::GenericMenuContent<true> m_Content[2];
	GUI::Menu::Menu m_Menu[2];
	int m_iFocusedMenu = 0;
};
