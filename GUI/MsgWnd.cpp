#include "framework.h"
#include <windowsx.h>
#include "MsgWnd.h"
#include "TextLinePainter.h"
#include "GlobalConst.h"
#include "DrawFuncs.h"
#include "Color.h"
#include "IController.h"

namespace GUI
{
	MsgWnd::MsgWnd(
		int nLinesPerPage,
		VerticalLocation VerticalLoc,
		int Width
	)
	{
		constexpr int TBMargin = 4;	//�z�u�� TOP or BOTTOM �̍ۂ̔z�u�]���i��ʒ[���班�������j
		const Vec2i Padding{ 12,6 };	//�t���[�������ɐ݂���]���i�t���[���ƃe�L�X�g�Ƃ̊ԁj

		m_nLinesPerPage = std::max( 1, nLinesPerPage );
		if( Width<=0 ){	Width = GlobalConst::GC_W - 4*2;	}
		const int TextW = Width - Padding[0]*2;

		m_upTextPainter = std::make_unique< TextLinePainter >( TextW, 24 );
		{
			//���T�C�Y�v�Z���\�Ƃ��邽�߂�nLinesPerPage���̋󕶎����^����
			m_upTextPainter->TextLines( std::vector<std::wstring>( m_nLinesPerPage, std::wstring() ) );
			
			//
			int TPTop = TBMargin + Padding[1];
			if( VerticalLoc == VerticalLocation::CENTER )
			{	TPTop = ( GlobalConst::GC_H - m_upTextPainter->Size()[1] ) / 2;	}
			else if( VerticalLoc == VerticalLocation::BOTTOM )
			{	TPTop = GlobalConst::GC_H - m_upTextPainter->Size()[1] - TBMargin - Padding[1];	}

			m_upTextPainter->TopLeft( Vec2i{ 0, TPTop } ).XCenter( GlobalConst::GC_W );
		}

		m_FrameRect = Rect::Dilation( m_upTextPainter->BoundingRect(), Padding[0], Padding[1] );
	}

	MsgWnd::~MsgWnd() = default;

	MsgWnd &MsgWnd::AlignCenter(){	m_upTextPainter->AlignCenter();	return *this;	}
	MsgWnd &MsgWnd::AlignLeft(){	m_upTextPainter->AlignLeft();	return *this;	}
	MsgWnd &MsgWnd::AlignRight(){	m_upTextPainter->AlignRight();	return *this;	}

	void MsgWnd::ToNextPage()
	{
		std::vector<std::wstring> PageLines;
		while( !m_RestLines.empty()  &&  (int)PageLines.size()<m_nLinesPerPage )
		{
			PageLines.emplace_back( m_RestLines.front() );
			m_RestLines.pop();
		}
		if( int nEmptyLines=m_nLinesPerPage-(int)PageLines.size();	nEmptyLines>0 )
		{	PageLines.insert( PageLines.end(), nEmptyLines, std::wstring() );	}

		m_upTextPainter->TextLines( std::move(PageLines) );
		m_NextPageIndicatorAnimCounter = 0;
		m_bDispNextPageIndicator = !m_RestLines.empty();
	}

	Flags<GUIResult> MsgWnd::Update( const IController &Controller )
	{
		if( Controller.ToNextText() )
		{
			if( m_RestLines.empty() )
			{	return GUIResult::Finished;	}
			else
			{
				ToNextPage();
				return GUIResult::ReqRedraw;
			}
		}

		//�e�L�X�g�ɑ��������邱�Ƃ������}�[�N�̕\���A�j���[�V�����X�V�����D
		if( !m_RestLines.empty() )
		{
			++m_NextPageIndicatorAnimCounter;
			if( m_NextPageIndicatorAnimCounter >= ms_NextPageIndicatorAnimInterval )
			{
				m_NextPageIndicatorAnimCounter = 0;
				m_bDispNextPageIndicator = !m_bDispNextPageIndicator;
				return GUIResult::ReqRedraw;
			}
		}
		return GUIResult::None;
	}

	void MsgWnd::Paint_( HDC hdc ) const
	{
		DrawFilledFrame( hdc, m_FrameRect, Color::White, RGB(0,0,0) );
		m_upTextPainter->Paint( hdc );

		if( m_bDispNextPageIndicator  &&  !m_RestLines.empty() )
		{
			HPEN hOldPen = SelectPen( hdc, GetStockPen( DC_PEN ) );
			HBRUSH hOldBrush = SelectBrush( hdc, GetStockBrush( DC_BRUSH ) );
			SetDCPenColor( hdc, RGB(0,0,0) );
			SetDCBrushColor( hdc, Color::White );
			
			DrawTriangle( hdc, m_upTextPainter->BoundingRect().RightBottom(), 12, 2 );

			SelectBrush( hdc, hOldBrush );
			SelectPen( hdc, hOldPen );
		}
	}
}
