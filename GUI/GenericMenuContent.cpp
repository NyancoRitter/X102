#include "framework.h"
#include <windowsx.h>
#include "GenericMenuContent.h"
#include "Parts/CMonoBMP.h"
#include "Parts/Rect.h"
#include "DrawFuncs.h"
#include "Color.h"

namespace GUI::Menu
{
	GenericMenuItem::GenericMenuItem( std::initializer_list<PartFunc> Parts )
		: m_Parts( Parts )
		, m_Color( Color::White )
	{}

	void GenericMenuItem::Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const
	{
		if( IsAtCursorPos )
		{	DrawMenuCursor( hdc, ItemDrawReg, IsMenuFocused );	}

		Rect RestReg = ItemDrawReg;
		for( const auto &Part : m_Parts )
		{	RestReg.TopLeft()[0] += Part( hdc, RestReg, m_Color );	}
	}

	//------------------------------------
	//Text

	int Text::operator()( HDC hdc, const Rect &RestReg, COLORREF Color ) const
	{
		SetTextColor( hdc, Color );
		SetBkMode( hdc, TRANSPARENT );

		Rect ThisReg = RestReg;
		if( m_XSize>0  &&  ThisReg.Width() > m_XSize ){	ThisReg.SetWidth( m_XSize );	}

		auto TextRect = ThisReg.AsRECT();
		DrawTextW(
			hdc, m_TextU16.c_str(), -1, &TextRect,
			/*DT_NOCLIP*/DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | m_XPosFormat
		);

		return m_XSize;
	}

	//------------------------------------
	//Icon

	Icon::Icon( const CMonoBMP &rIcon, int W )
		: m_rIcon(rIcon)
		, m_XSize( W>=0  ?  W  :  rIcon.GetW() )
	{}

	int Icon::operator()( HDC hdc, const Rect &Reg, COLORREF Color ) const
	{
		Vec2i TL = Reg.TopLeft() + Vec2i{ m_XPosOffset, ( Reg.Height() - m_rIcon.GetH() )/2 };
		if( m_TransBit0Part )
		{	m_rIcon.BitBlt_TransBit0Part( hdc, TL[0],TL[1], Color );	}
		else
		{	m_rIcon.BitBlt( hdc, TL[0],TL[1], Color );	}

		return m_XSize;
	}

	Icon &Icon::AlignCenter()
	{
		m_XPosOffset = ( m_XSize - m_rIcon.GetW() ) / 2;
		return *this;
	}

	Icon &Icon::AlignRight()
	{
		m_XPosOffset = m_XSize - m_rIcon.GetW();
		return *this;
	}

}
