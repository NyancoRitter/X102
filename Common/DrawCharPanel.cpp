#include "framework.h"
#include <windowsx.h>
#include "Parts/CMonoBMP.h"
#include "DrawCharPanel.h"

void DrawCharPanel(
	HDC hdc,
	const RECT &DrawRegion,
	const std::wstring &CharNameU16,
	const CMonoBMP &Icon,
	int HP,
	int MaxHP,
	COLORREF ForeColor,
	COLORREF BkColor
)
{
	HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
	SetDCPenColor( hdc, ForeColor );

	{//˜g
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( DC_BRUSH ) );
		SetDCBrushColor( hdc, BkColor );

		RoundRect( hdc, DrawRegion.left, DrawRegion.top, DrawRegion.right, DrawRegion.bottom, 10,10 );
		SelectBrush( hdc, OldBrush );
	}
	{//˜g“à•\Ž¦•¨
		constexpr int Margin = 8;
		Icon.BitBlt( hdc, DrawRegion.left+Margin, ( DrawRegion.top+DrawRegion.bottom-Icon.GetH() )/2, ForeColor );

		const int nViewContent = 2;
		const int ContentH = ( DrawRegion.bottom - DrawRegion.top - Margin*2 ) / nViewContent;

		RECT ContentRect;
		ContentRect.left = DrawRegion.left + Margin*2 + Icon.GetW();
		ContentRect.right = DrawRegion.right - Margin;
		ContentRect.top = DrawRegion.top + Margin;
		ContentRect.bottom = ContentRect.top + ContentH;

		SetTextColor( hdc, ForeColor );
		SetBkMode( hdc, TRANSPARENT );
		DrawText( hdc, CharNameU16.c_str(), -1, &ContentRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_LEFT );

		OffsetRect( &ContentRect, 0, ContentH );
		{
			std::wstring HPInfoText = std::to_wstring(HP) + L" / " + std::to_wstring(MaxHP);
			DrawText( hdc, HPInfoText.c_str(), -1, &ContentRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_RIGHT );	
		}
	}

	SelectPen( hdc, OldPen );
}
