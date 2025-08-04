#include "framework.h"
#include <windowsx.h>
#include "TgtSelection.h"
#include "IController.h"
#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"

namespace GUI
{

	void TgtSelection::Paint_( HDC hdc ) const
	{
		HPEN OldPen = SelectPen( hdc, GetStockPen( BLACK_PEN ) );
		HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( DC_BRUSH ) );
		SetDCBrushColor( hdc, Color::White );

		if( m_IsForAll )
		{
			for( const auto &ItemRect : m_ItemRects )
			{	DrawCursorFor( hdc, ItemRect );	}
		}
		else
		{	DrawCursorFor( hdc, m_ItemRects[ m_CursorPos ] );	}

		SelectBrush( hdc, OldBrush );
		SelectPen( hdc, OldPen );
	}

	void TgtSelection::DrawCursorFor( HDC hdc, const Rect &ItemRect ) const
	{
		constexpr int EdgeLength = 16;
		const int hH = (int)std::round( 0.25 * EdgeLength * sqrt(3.0) );
		const int AnimOffset = ( (m_AnimCounter & ms_AnimBit) ? 3 : 0 );

		if( m_IsVertical )
		{
			Vec2i Center{ ItemRect.Left() + hH + AnimOffset, ( ItemRect.Top()+ItemRect.Bottom() )/2 };
			DrawTriangle( hdc, Center, EdgeLength, 1 );
		}
		else
		{
			Vec2i Center{ ( ItemRect.Left() + ItemRect.Bottom() )/2, ItemRect.Top() + hH + AnimOffset };
			DrawTriangle( hdc, Center, EdgeLength, 2 );
		}
	}

	Flags<GUIResult> TgtSelection::Update( const IController &Controller  )
	{
		if( !m_IsForAll )
		{//カーソル移動
			if( m_IsVertical  ?  Controller.CursorUp()  :  Controller.CursorLeft() )
			{
				m_CursorPos = ( m_CursorPos>0  ?  m_CursorPos-1  :  (int)m_ItemRects.size()-1 );
				return GUIResult::ReqRedraw;
			}

			if( m_IsVertical  ?  Controller.CursorDown()  :  Controller.CursorRight() )
			{
				m_CursorPos = ( m_CursorPos+1<(int)m_ItemRects.size()  ?  m_CursorPos+1  :  0 );
				return GUIResult::ReqRedraw;
			}
		}
		
		if( Controller.Cancel() )
		{	return m_OnDecided( false, ( m_IsForAll ? -1 : m_CursorPos ) );	}

		if( Controller.Select() )
		{	return m_OnDecided( true, ( m_IsForAll ? -1 : m_CursorPos ) );	}

		{//カーソルアニメーション更新
			int Pre = m_AnimCounter;
			++m_AnimCounter;
			if( (Pre & ms_AnimBit) != (m_AnimCounter & ms_AnimBit) )
			{	return GUIResult::ReqRedraw;	}
		}

		return GUIResult::None;
	}
}