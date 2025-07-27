#include "framework.h"
#include "TextLinePainter.h"
#include "Color.h"

namespace GUI
{
	void TextLinePainter::Paint_( HDC hdc ) const
	{
		if( m_TextLinesU16.empty() )return;

		SetTextColor( hdc, Color::White );
		SetBkMode( hdc, TRANSPARENT );

		const UINT Format = DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|m_HorizontalFormat;

		RECT TextLineRect;
		TextLineRect.left = m_TopLeft[0];
		TextLineRect.right = TextLineRect.left + m_Size[0];
		TextLineRect.top = m_TopLeft[1];
		TextLineRect.bottom = TextLineRect.top + m_LineH;
		for( const auto &rLine : m_TextLinesU16 )
		{
			DrawText( hdc, rLine.c_str(), -1, &TextLineRect, Format );
			OffsetRect( &TextLineRect, 0, m_LineH );
		}
	}

	TextLinePainter &TextLinePainter::AlignCenter(){	m_HorizontalFormat=DT_CENTER;	return *this;	}
	TextLinePainter &TextLinePainter::AlignLeft(){	m_HorizontalFormat=DT_LEFT;	return *this;	}
	TextLinePainter &TextLinePainter::AlignRight(){	m_HorizontalFormat=DT_RIGHT;	return *this;	}
}
