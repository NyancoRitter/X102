#include "framework.h"

#include <windowsx.h>
#include "SimpleSelection.h"
#include "DrawFuncs.h"
#include "Color.h"

namespace
{
	constexpr int TB_Margin = 16;	//｛外枠，テキスト，横線，メニュー｝の間の余白
	constexpr int TextLineHeight = 24;
}

namespace GUI::Menu
{
	//項目追加
	SimpleSelection &SimpleSelection::Add( const std::wstring &ItemText )
	{
		m_Content.Add( { Text( ItemText ).AlignCenter() } );
		return *this;
	}

	void SimpleSelection::UpdateMenuPos()
	{
		m_Menu.XCenter( m_W ).Offset( m_TopLeft[0] );

		int Left = m_TopLeft[0] + ( m_W - m_Menu.Size()[0] )/2;
		int Top = m_TopLeft[1] + ( TB_Margin*3 + TextLineHeight );
		m_Menu.TopLeft( { Left,Top } );
	}

	Vec2i SimpleSelection::Size() const
	{
		return Vec2i(
			m_W,
			TB_Margin*4 + TextLineHeight + m_Menu.Size()[1]
		);
	}

	//Paint
	void SimpleSelection::Paint_( HDC hdc ) const
	{
		//外枠
		Rect BRect = BoundingRect();
		DrawFilledFrame( hdc, BRect, Color::White, RGB(0,0,0) );

		//テキスト
		::SetTextColor( hdc, Color::White );
		::SetBkMode( hdc, TRANSPARENT );
		RECT rect = BRect.AsRECT();
		rect.top += TB_Margin;
		rect.bottom = rect.top + TextLineHeight;
		DrawTextW( hdc, m_MsgText.c_str(), -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_CENTER );

		{//横線
			HPEN OldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
			SetDCPenColor( hdc, Color::White );
			int y = rect.bottom + TB_Margin;
			::MoveToEx( hdc, rect.left+16, y, NULL );
			::LineTo( hdc, rect.right-16, y );
			SelectPen( hdc, OldPen );
		}

		//メニュー
		m_Menu.Paint( hdc );
	}

	//Update
	Flags<GUIResult> SimpleSelection::Update( const IController &Controller )
	{
		switch( m_Menu.HandleInput(Controller) )
		{
		case HandleInputResult::CursorMoved:
			return GUIResult::ReqRedraw;
			break;

		case HandleInputResult::Selected:
			if( !m_OnSelect )return GUIResult::None;
			if( int iCur=m_Content.CursorPos(); 0<=iCur )
			{
				if( m_OnSelect( iCur ) )
				{	return ( GUIResult::Finished | GUIResult::ReqRedraw );	}
			}
			break;

		case HandleInputResult::Canceled:
			if( m_bCancelable )
			{	return ( GUIResult::Finished | GUIResult::ReqRedraw );	}
			break;

		default:
			break;
		}
		return GUIResult::None;
	}

	void SimpleSelection::OnGotFocus(){	m_Menu.IsFocused(true);	}
	void SimpleSelection::OnLostFocus(){	m_Menu.IsFocused(false);	}
}

