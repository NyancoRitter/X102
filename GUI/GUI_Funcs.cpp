#include "framework.h"
#include <windowsx.h>
#include "GUI_Funcs.h"
#include "Color.h"

namespace GUI 
{
	//
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, bool bFill, int RoundedCornerRadius )
	{
		HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
		SetDCPenColor( hdc, Color );
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( bFill ? BLACK_BRUSH : NULL_BRUSH ) );

		RoundRect( hdc, Rect.TopLeft()[0], Rect.TopLeft()[1], Rect.RightBottom()[0], Rect.RightBottom()[1], RoundedCornerRadius*2, RoundedCornerRadius*2 );

		SelectPen( hdc, OldPen );
		SelectBrush( hdc, OldBrush );
	}

	//
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused )
	{
		HBRUSH hbr = CreateSolidBrush( Color::MenuCursor(IsMenuFocused) );
		auto r = ItemDrawReg.AsRECT();
		FillRect( hdc, &r, hbr );
		DeleteBrush( hbr );
	}
}
