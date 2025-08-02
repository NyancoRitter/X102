#include "framework.h"
#include "CharSelMenuContent.h"
#include "DrawCharPanel.h"
#include "Parts/Rect.h"
#include "ResManage/BmpBank.h"
#include "GUI/Color.h"

namespace GUI::Menu
{
	//------------------------------------
	// CharSelMenuItem

	CharSelMenuItem::CharSelMenuItem( GameContent::PartyCharID TgtChar, int HP, int MaxHP )
		: m_TgtChar( TgtChar )
		, m_HP(HP), m_MaxHP(MaxHP)
		, m_Color( Color::White )
	{}

	void CharSelMenuItem::Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const
	{
		RECT Reg = ItemDrawReg.AsRECT();
		if( IsAtCursorPos )
		{	OffsetRect( &Reg, m_OffsetWhenCursorPos[0], m_OffsetWhenCursorPos[1] );	}

		DrawCharPanel(
			hdc,
			Reg,
			GameContent::NameStr(m_TgtChar),
			ResManage::FaceBMP(m_TgtChar),
			m_HP, m_MaxHP,
			m_Color,
			( IsAtCursorPos  ?  Color::MenuCursor( IsMenuFocused )  :  RGB(0,0,0) )
		);
	}

	//------------------------------------
	// CharSelMenuContent
}