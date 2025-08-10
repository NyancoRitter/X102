#include "framework.h"
#include <windowsx.h>
#include "Menu.h"
#include "IMenuItem.h"
#include "IMenuContent.h"
#include "DrawFuncs.h"
#include "Color.h"
#include "IController.h"

namespace GUI::Menu
{

	//
	int Menu::ActualItemAreaSpace() const
	{
		if( m_ItemAreaSpace <= 0 )
		{	return ( m_pContent  ?  m_pContent->nItems()  :  0 );	}
		else
		{	return m_ItemAreaSpace;	}
	}

	Menu &Menu::UpdateScrollState()
	{
		const int nSpc = ActualItemAreaSpace();
		if( !m_pContent  ||  m_pContent->nItems()<=nSpc )
		{	m_iDrawBegin = 0;	return *this;	}

		int iCursorPos = m_pContent->CursorPos();
		if( iCursorPos < 0 )
		{	m_iDrawBegin = 0;	}
		else if( iCursorPos<m_iDrawBegin )
		{	m_iDrawBegin = iCursorPos;	}
		else if( m_iDrawBegin+nSpc <= iCursorPos )
		{	m_iDrawBegin = iCursorPos - (nSpc-1);	}

		return *this;
	}

	//
	Vec2i Menu::Size() const
	{
		if( !m_pContent )return Vec2i{0,0};

		Vec2i Total = m_OuterMargin * 2;
		const int n = ActualItemAreaSpace();
		const auto ItemSize = m_pContent->ItemDrawSize();
		if( m_pContent->IsVerticalMenu() )
		{
			Total[0] += ItemSize[0];
			Total[1] += n * ItemSize[1];
			Total[1] += (n-1) * m_ItemSpacing;
		}
		else
		{
			Total[0] += n * ItemSize[0];
			Total[0] += (n-1) * m_ItemSpacing;
			Total[1] += ItemSize[1];
		}
		return Total;
	}

	//
	Rect Menu::ItemDrawRect( int index ) const
	{
		if( !m_pContent )return Rect();

		Vec2i TL = m_TopLeft + m_OuterMargin;
		int iOffsetCoord = ( m_pContent->IsVerticalMenu() ? 1 : 0 );
		const auto ItemSize = m_pContent->ItemDrawSize();

		TL[ iOffsetCoord ] += (index-m_iDrawBegin) * ( ItemSize[ iOffsetCoord ] + m_ItemSpacing );
		return Rect( TL, ItemSize[0], ItemSize[1] );
	}

	//
	void Menu::Paint_( HDC hdc ) const
	{
		if( !m_pContent )return;

		const auto BR = this->BoundingRect();
		const int nSpc = ActualItemAreaSpace();
		const int iCursorPos = m_pContent->CursorPos();

		//枠
		if( m_bDrawFrame )
		{	DrawFilledFrame( hdc, BR, Color::White, RGB(0,0,0) );	}

		//項目群
		const auto ItemSize = m_pContent->ItemDrawSize();
		const Vec2i Offset =
			m_pContent->IsVerticalMenu() ?
			Vec2i{ 0, ItemSize[1]+m_ItemSpacing } :
			Vec2i{ ItemSize[0]+m_ItemSpacing, 0 };

		Rect ItemRect( m_TopLeft + m_OuterMargin, ItemSize[0], ItemSize[1] );
		for( int i=0; i<nSpc; ++i )
		{
			int iItem = m_iDrawBegin + i;
			if( iItem >= m_pContent->nItems() )break;

			m_pContent->Item( iItem ).Draw( hdc, ItemRect, (iItem==iCursorPos), m_bFocused, m_bDrawCursor );
			ItemRect.Offset( Offset );
		}

		{//スクロールマーク
			const bool CanScrollToDecDir = ( m_iDrawBegin > 0 );
			const bool CanScrollToIncDir = ( m_iDrawBegin+nSpc < m_pContent->nItems() );
			if( CanScrollToDecDir || CanScrollToIncDir )
			{
				HPEN OldPen = SelectPen( hdc, GetStockPen( BLACK_PEN ) );
				HBRUSH OldBrush = SelectBrush( hdc, GetStockBrush( DC_BRUSH ) );
				SetDCPenColor( hdc, Color::White );

				constexpr int EdgeLength = 12;
				if( CanScrollToDecDir )
				{
					if( m_pContent->IsVerticalMenu() )
					{	DrawTriangle( hdc, Vec2i{ ( BR.Left() + BR.Right() )/2, BR.Top() }, EdgeLength );	}
					else
					{	DrawTriangle( hdc, Vec2i{ BR.Left(), ( BR.Top() + BR.Bottom() )/2 }, EdgeLength, 3u );	}
				}
				if( CanScrollToIncDir )
				{
					if( m_pContent->IsVerticalMenu() )
					{	DrawTriangle( hdc, Vec2i{ ( BR.Left() + BR.Right() )/2, BR.Bottom() }, EdgeLength, 2u );	}
					else
					{	DrawTriangle( hdc, Vec2i{ BR.Right(), ( BR.Top() + BR.Bottom() )/2 }, EdgeLength, 1u );	}
				}

				SelectBrush( hdc, OldBrush );
				SelectPen( hdc, OldPen );
			}
		}
	}

	//
	HandleInputResult Menu::HandleInput( const IController &Controller )
	{
		if( !m_pContent )return HandleInputResult::None;

		auto Ret = GUI::Menu::HandleInput( *m_pContent, Controller );

		if( Ret == HandleInputResult::CursorMoved )
		{	UpdateScrollState();	}

		return Ret;
	}

}
