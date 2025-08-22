#include "framework.h"
#include "MazeTitleEffect.h"
#include "GlobalConst.h"
#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"
#include "Common/PainterSeq.h"

namespace Maze
{
	Flags<CmdResult> TitleEffect::operator()()
	{
		if( m_Counter >= m_Period )
		{
			return ( m_PStoJoin.Remove( this )  ?  CmdResult::Finished|CmdResult::ReqRedraw  :  CmdResult::Finished );
		}

		++m_Counter;
		if( m_Counter == 1 ){	m_PStoJoin.PushBack( this );	}

		return CmdResult::None;
	}

	void TitleEffect::Paint_( HDC hdc ) const
	{
		constexpr int Margin = 4;
		constexpr int H = 48;
		Rect R( Vec2i(Margin,Margin), GlobalConst::GC_W - Margin*2,  H );

		GUI::DrawFilledFrame( hdc, R, GUI::Color::White, RGB(0,0,0) );
		SetTextColor( hdc, GUI::Color::White );
		SetBkMode( hdc, TRANSPARENT );

		auto TextRect = R.AsRECT();
		DrawText( hdc, L"=== ENTERING THE MAZE OF PALMETINA ===", -1, &TextRect, DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_CENTER );
	}
}
