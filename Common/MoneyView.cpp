#include "framework.h"
#include "MoneyView.h"
#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"

MoneyView::MoneyView()
{
	m_TextView.LineWidth( 160 ).LineHeight( 40 ).AlignRight();
	SetMoney(0);
}

MoneyView &MoneyView::SetMoney( int money )
{
	m_TextView.TextLines( { std::to_wstring( money ) + L" G" } );
	return *this;
}

void MoneyView::Paint_( HDC hdc ) const
{
	auto FrameRect = m_TextView.BoundingRect();
	FrameRect.SetWidth( FrameRect.Width() + ms_RPadding );
	GUI::DrawFilledFrame( hdc, FrameRect, GUI::Color::White, RGB(0,0,0) );

	m_TextView.Paint( hdc );
}
