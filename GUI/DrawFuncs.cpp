#include "framework.h"
#include <windowsx.h>
#include "DrawFuncs.h"
#include "Color.h"

namespace GUI 
{
	//
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, int RoundedCornerRadius )
	{
		HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
		SetDCPenColor( hdc, Color );
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( NULL_BRUSH ) );

		RoundRect( hdc, Rect.TopLeft()[0], Rect.TopLeft()[1], Rect.RightBottom()[0], Rect.RightBottom()[1], RoundedCornerRadius*2, RoundedCornerRadius*2 );

		SelectPen( hdc, OldPen );
		SelectBrush( hdc, OldBrush );
	}

	void DrawFilledFrame( HDC hdc, const Rect &Rect, COLORREF LineColor, COLORREF FillColor, int RoundedCornerRadius )
	{
		HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
		SetDCPenColor( hdc, LineColor );
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( DC_BRUSH ) );
		SetDCBrushColor( hdc, FillColor );

		RoundRect( hdc, Rect.TopLeft()[0], Rect.TopLeft()[1], Rect.RightBottom()[0], Rect.RightBottom()[1], RoundedCornerRadius*2, RoundedCornerRadius*2 );

		SelectPen( hdc, OldPen );
		SelectBrush( hdc, OldBrush );
	}

	//
	void FillRectReg( HDC hdc, const Rect &Rect, COLORREF Color )
	{
		HBRUSH hbr = CreateSolidBrush( Color );
		auto r = Rect.AsRECT();
		FillRect( hdc, &r, hbr );
		DeleteBrush( hbr );
	}

	/// <summary>
	/// 90ìxíPà âÒì]çÏã∆
	/// </summary>
	/// <param name="P"></param>
	/// <param name="Rot"></param>
	/// <returns></returns>
	static inline POINT Rotate( const POINT &P, unsigned int Rot )
	{
		constexpr int C[] = { 1, 0, -1, 0 };
		constexpr int S[] = { 0, 1, 0, -1 };

		return POINT{
			P.x*C[Rot] - P.y*S[Rot],
			P.x*S[Rot] + P.y*C[Rot]
		};
	}

	//
	void DrawTriangle( HDC hdc, const Vec2i &Center, int EdgeLength, unsigned int Rot )
	{
		const int hEL = EdgeLength/2;
		const int hH = (int)std::round( 0.25 * EdgeLength * sqrt(3.0) );
		POINT P[3];

		P[0].x = 0;
		P[0].y = - hH;
		P[1].x = - hEL;
		P[2].x = hEL;
		P[1].y = P[2].y = hH;

		Rot = Rot & 0b011;
		if( Rot != 0 )
		{
			for( auto &p : P )
			{	p = Rotate(p,Rot);	}
		}

		for( auto &p : P )
		{	p.x += Center[0];	p.y += Center[1];	}
		
		Polygon( hdc, P, 3 );
	}

	//
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused )
	{	FillRectReg( hdc, ItemDrawReg, Color::MenuCursor(IsMenuFocused) );	}

}
