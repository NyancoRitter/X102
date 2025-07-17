#include "framework.h"
#include "Menu.h"
#include "IMenuContent.h"
#include "GUI_Funcs.h"
#include "Color.h"
#include "IController.h"

namespace GUI::Menu
{
	Vec2i Menu::Size() const
	{
		if( !m_pContent )return Vec2i{0,0};

		Vec2i Total = m_OuterMargin * 2;
		const int n = m_pContent->nItems();
		const auto ItemSize = m_pContent->ItemSize();
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
	Rect Menu::ItemDrawRECT( int index ) const
	{
		if( !m_pContent )return Rect();

		Vec2i TL = m_TopLeft + m_OuterMargin;
		int iOffsetCoord = ( m_pContent->IsVerticalMenu() ? 1 : 0 );
		const auto ItemSize = m_pContent->ItemSize();
		TL[ iOffsetCoord ] += index * ( ItemSize[ iOffsetCoord ] + m_ItemSpacing );
		return Rect( TL, ItemSize[0], ItemSize[1] );
	}

	//
	void Menu::Paint( HDC hdc ) const
	{
		if( !m_pContent )return;

		{//˜g
			auto size = Size();
			DrawFrame( hdc, Rect( m_TopLeft, size[0], size[1] ), Color::White );
		}

		//€–ÚŒQ
		const int iCursorPos = m_pContent->CursorPos();
		const auto ItemSize = m_pContent->ItemSize();
		const Vec2i Offset =
			m_pContent->IsVerticalMenu() ?
			Vec2i{ 0, ItemSize[1]+m_ItemSpacing } :
			Vec2i{ ItemSize[0]+m_ItemSpacing, 0 };

		Rect ItemRect( m_TopLeft + m_OuterMargin, ItemSize[0], ItemSize[1] );
		for( int i=0; i<m_pContent->nItems(); ++i )
		{
			m_pContent->Item( i ).Draw( hdc, ItemRect, (i==iCursorPos), m_bFocused );
			ItemRect.Offset( Offset );
		}
	}

	//
	HandleInputResult Menu::HandleInput( const IController &Controller )
	{
		if( !m_pContent )return HandleInputResult::None;

		if( m_pContent->nItems() >= 1 )
		{//ƒJ[ƒ\ƒ‹ˆÚ“®
			int NewCursorPos = m_pContent->CursorPos();
			if( m_pContent->IsVerticalMenu() ? Controller.CursorUp() : Controller.CursorLeft() )
			{
				--NewCursorPos;
				if( NewCursorPos < 0 )NewCursorPos = m_pContent->nItems() - 1;
			}
			else if( m_pContent->IsVerticalMenu() ? Controller.CursorDown() : Controller.CursorRight() )
			{
				++NewCursorPos;
				if( NewCursorPos < 0 )NewCursorPos = 0;
				if( NewCursorPos >= m_pContent->nItems() )NewCursorPos = 0;
			}

			if( m_pContent->CursorPos() != NewCursorPos )
			{
				m_pContent->CursorPos( NewCursorPos );
				return HandleInputResult::CursorMoved;
			}
		}

		if( Controller.Select()  &&  m_pContent->CursorPos() >= 0 )
		{	return HandleInputResult::Selected;	}

		if( Controller.Cancel() )
		{	return HandleInputResult::Canceled;	}

		return HandleInputResult::None;
	}
}
