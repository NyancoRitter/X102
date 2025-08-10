#include "framework.h"
#include "ValueImgPainter.h"

#include "DataPath.h"
#include "ResManage/MonoBmpHolder.h"
#include "Parts/CMonoBmp.h"

std::unique_ptr<CMonoBMP> ValueImgPainter::ms_upValImg;
std::unique_ptr<CMonoBMP> ValueImgPainter::ms_upValFrameImg;

ValueImgPainter::ValueImgPainter( unsigned int val )
	: m_Color( RGB(255,255,255) )
{
	if( !ms_upValImg )
	{	ms_upValImg = ResManage::LoadMonoBMP_or_Dummy( DataPath::ImgDirU16() + L"Nums.bmp" );	}
	if( !ms_upValFrameImg )
	{	ms_upValFrameImg = ResManage::LoadMonoBMP_or_Dummy( DataPath::ImgDirU16() + L"NumFrames.bmp" );	}

	//
	std::string S = std::to_string( val );
	m_Vals.resize( S.length() );
	for( size_t i=0; i<S.length(); ++i )
	{	m_Vals[i] = S[i] - '0';	}
}

Vec2i ValueImgPainter::Size() const
{	return m_MagRate * Vec2i( (int)m_Vals.size()*ValImgW, ValImgH );	}

void ValueImgPainter::Paint_( HDC hdc ) const
{
	int x = m_TopLeft[0];
	const int y = m_TopLeft[1];

	if( m_MagRate==1 )
	{
		for( int v : m_Vals )
		{
			int ImgTop = v*ValImgH;
			ms_upValFrameImg->BitBlt_TransBit0Part( hdc, x,y, 0,ImgTop, ValImgW,ValImgH, RGB(0,0,0) );
			ms_upValImg->BitBlt_TransBit0Part( hdc, x,y, 0,ImgTop, ValImgW,ValImgH, m_Color );
			x += ValImgW;
		}
	}
	else
	{
		for( int v : m_Vals )
		{
			int ImgTop = v*ValImgH;
			ms_upValFrameImg->StretchBlt_TransBit0Part( hdc, x,y, (float)m_MagRate,(float)m_MagRate, 0,ImgTop, ValImgW,ValImgH, RGB(0,0,0) );
			ms_upValImg->StretchBlt_TransBit0Part( hdc, x,y, (float)m_MagRate,(float)m_MagRate, 0,ImgTop, ValImgW,ValImgH, m_Color );
			x += m_MagRate * ValImgW;
		}
	}
}

