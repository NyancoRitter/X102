#include "framework.h"
#include <windowsx.h>
#include "MenuItem.h"
#include "Parts/CMonoBMP.h"
#include "Color.h"

namespace GUI::Menu
{
	void DrawCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused )
	{
		HBRUSH hbr = CreateSolidBrush( Color::MenuCursor(IsMenuFocused) );
		auto r = ItemDrawReg.AsRECT();
		FillRect( hdc, &r, hbr );
		DeleteBrush( hbr );
	}

	//------------------------------------
	//Text

	void Text::Draw( HDC hdc, const Rect &RestReg, COLORREF Color ) const
	{
		SetTextColor( hdc, Color );
		SetBkMode( hdc, TRANSPARENT );

		Rect ThisReg = RestReg;
		if( ThisReg.Width() > XSize() ){	ThisReg.SetWidth( XSize() );	}

		auto TextRect = ThisReg.AsRECT();
		DrawTextW(
			hdc, m_TextU16.c_str(), -1, &TextRect,
			/*DT_NOCLIP*/DT_WORD_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | m_XPosFormat
		);
	}

	//------------------------------------
	//Icon

	Icon::Icon( const CMonoBMP &rIcon, int W )
		: m_rIcon(rIcon)
		, m_XSize( W>=0  ?  W  :  rIcon.GetW() )
	{}

	void Icon::Draw( HDC hdc, const Rect &Reg, COLORREF Color ) const
	{
		Vec2i TL = Reg.TopLeft() + Vec2i{ m_XPosOffset, ( Reg.Height() - m_rIcon.GetH() )/2 };
		if( m_TransBit0Part )
		{	m_rIcon.BitBlt_TransBit0Part( hdc, TL[0],TL[1], Color );	}
		else
		{	m_rIcon.BitBlt( hdc, TL[0],TL[1], Color );	}
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
