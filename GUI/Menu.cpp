#include "framework.h"
#include "GUI.h"
#include "Menu.h"
#include "Color.h"
#include "IController.h"

namespace GUI::Menu
{
	Vec2i MenuPainter::TotalSize( const IMenuContent &Content ) const
	{
		Vec2i Total = m_OuterMargin * 2;
		const int n = Content.nItems();
		const auto ItemSize = Content.ItemSize();
		if( Content.IsVerticalMenu() )
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
	Rect MenuPainter::ItemDrawRECT( const IMenuContent &Content, int index ) const
	{
		Vec2i TL = m_TopLeft + m_OuterMargin;
		int iOffsetCoord = ( Content.IsVerticalMenu() ? 1 : 0 );
		const auto ItemSize = Content.ItemSize();
		TL[ iOffsetCoord ] += index * ( ItemSize[ iOffsetCoord ] + m_ItemSpacing );
		return Rect( TL, ItemSize[0], ItemSize[1] );
	}

	//
	void MenuPainter::Draw( HDC hdc, const IMenuContent &Content ) const
	{
		{//òg
			auto Size = TotalSize( Content );
			DrawFrame( hdc, Rect( m_TopLeft, Size[0], Size[1] ).AsRECT(), Color::White );
		}
		//çÄñ⁄åQ
		const int iCursorPos = Content.CursorPos();
		for( int i=0; i<Content.nItems(); ++i )
		{
			Content.Item( i ).Draw( hdc, ItemDrawRECT( Content, i ), (i==iCursorPos), m_bFocused );
		}
	}

	//
	HandleInputResult HandleInput( IMenuContent &Content, const IController &Controller )
	{
		if( Content.nItems() >= 1 )
		{//ÉJÅ[É\Éãà⁄ìÆ
			int NewCursorPos = Content.CursorPos();
			if( Content.IsVerticalMenu() ? Controller.CursorUp() : Controller.CursorLeft() )
			{
				--NewCursorPos;
				if( NewCursorPos<0 )NewCursorPos = Content.nItems() - 1;
			}
			else if( Content.IsVerticalMenu() ? Controller.CursorDown() : Controller.CursorRight() )
			{
				++NewCursorPos;
				if( NewCursorPos<0 )NewCursorPos = 0;
				if( NewCursorPos>=Content.nItems() )NewCursorPos = 0;
			}

			if( Content.CursorPos() != NewCursorPos )
			{
				Content.CursorPos( NewCursorPos );
				return HandleInputResult::CursorMoved;
			}
		}

		if( Controller.Select()  &&  Content.CursorPos()>=0 )
		{	return HandleInputResult::Selected;	}
		
		if( Controller.Cancel() )
		{	return HandleInputResult::Canceled;	}

		return HandleInputResult::None;
	}
}
