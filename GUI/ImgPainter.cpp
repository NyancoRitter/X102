#include "framework.h"
#include "ImgPainter.h"
#include "Parts/CMonoBMP.h"
#include "Color.h"

namespace GUI
{
	MonoImgPainter::MonoImgPainter( const CMonoBMP *pBMP )
		: m_Color( Color::White )
	{	SetImg( pBMP );	}

	void MonoImgPainter::Paint_( HDC hdc ) const
	{
		if( !m_pBMP )return;

		if( m_TransBit0Part )
		{	m_pBMP->BitBlt_TransBit0Part( hdc, m_TopLeft[0],m_TopLeft[1], m_Color );	}
		else
		{	m_pBMP->BitBlt( hdc, m_TopLeft[0],m_TopLeft[1], m_Color );	}
	}

	Vec2i MonoImgPainter::Size() const
	{
		if( !m_pBMP )return Vec2i{0,0};
		return Vec2i{ m_pBMP->GetW(), m_pBMP->GetH() };
	}
}
