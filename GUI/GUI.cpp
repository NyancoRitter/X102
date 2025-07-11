#include "framework.h"
#include <windowsx.h>
#include "GUI.h"

namespace GUI 
{
	//
	void DrawFrame( HDC hdc, const RECT &Rect, COLORREF Color, bool bFill )
	{
		HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
		SetDCPenColor( hdc, Color );
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( bFill ? BLACK_BRUSH : NULL_BRUSH ) );

		RoundRect( hdc, Rect.left, Rect.top, Rect.right, Rect.bottom, 10,10 );

		SelectPen( hdc, OldPen );
		SelectBrush( hdc, OldBrush );
	}
}
