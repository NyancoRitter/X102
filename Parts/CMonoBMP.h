//[CMonoBMP.h]

#pragma once
#include "framework.h"
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>

/// <summary>
/// モノクロ(1bit)BMPの ファイル読込み/描画の実装
/// </summary>
class CMonoBMP
{
public:
	/// <summary>
	/// 引数指定のモノクロBMPの管理者として生成．
	/// ※引数で渡したハンドルに対して DeleteObject() がデストラクタで実施される．
	/// </summary>
	explicit CMonoBMP( HBITMAP hBMP )
	{
		m_hBMP = hBMP;

		BITMAP bmp;
		::GetObject( m_hBMP, sizeof(BITMAP), &bmp );

		if( bmp.bmBitsPixel != 1 )
		{	throw new std::invalid_argument("Invalid Format");	}

		m_Width = bmp.bmWidth;
		m_Height = bmp.bmHeight;
	}

public:
	CMonoBMP( const CMonoBMP & ) = delete;
	CMonoBMP &operator=( const CMonoBMP & ) = delete;
	~CMonoBMP(){	::DeleteObject( m_hBMP );	}

public:
	/// <summary>
	/// BMPファイルをロードして生成
	/// </summary>
	/// <param name="FilePathName">ファイルパス</param>
	/// <returns>失敗時はnullptr</returns>
	static std::unique_ptr<CMonoBMP> LoadBMPFile( const std::wstring &FilePathNameU16 )
	{
		auto hBMP = (HBITMAP)::LoadImage( NULL, FilePathNameU16.c_str(), IMAGE_BITMAP, 0,0, LR_LOADFROMFILE|LR_MONOCHROME );
		if( hBMP==NULL )return nullptr;

		try
		{	return std::unique_ptr< CMonoBMP >( new CMonoBMP( hBMP ) );	}
		catch( std::invalid_argument & )
		{	return nullptr;	}
	}

	//BitBlt : 単純描画
	//	hdc, DestLeft, DestTop : 描画先のDCと位置
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIBのうち描画する矩形範囲
	//	Bit1Color : 画像データのbit値が1の箇所に対する描画色
	//	Bit0Color : 画像データのbit値が0の箇所に対する描画色
	bool BitBlt(
		HDC hdc,
		int DestLeft, int DestTop,
		int SrcLeft, int SrcTop,
		int SrcWidth, int SrcHeight,
		COLORREF Bit1Color = RGB(255,255,255),
		COLORREF Bit0Color = RGB(0,0,0)
		) const
	{
		HDC hmemdc = ::CreateCompatibleDC( hdc );
		if( hmemdc == NULL )return false;
		HBITMAP holdbmp = (HBITMAP)SelectObject( hmemdc, m_hBMP );

		::SetTextColor( hdc, Bit0Color );
		::SetBkColor( hdc, Bit1Color );
		bool ret = ( ::BitBlt( hdc, DestLeft,DestTop, SrcWidth,SrcHeight, hmemdc, SrcLeft,SrcTop, SRCCOPY ) != 0 );

		SelectObject( hmemdc, holdbmp );
		DeleteDC( hmemdc );
		return ret;
	}

	//画像全体を伸縮して描画
	bool StretchBlt(
		HDC hdc,
		int DestLeft, int DestTop,
		float XSizeRate, float YSizeRate,
		COLORREF Bit1Color = RGB(255,255,255),
		COLORREF Bit0Color = RGB(0,0,0)
		) const
	{
		HDC hmemdc = ::CreateCompatibleDC( hdc );
		if( hmemdc == NULL )return false;
		HBITMAP holdbmp = (HBITMAP)SelectObject( hmemdc, m_hBMP );

		::SetTextColor( hdc, Bit0Color );
		::SetBkColor( hdc, Bit1Color );
		const int W = GetW();
		const int H = GetH();
		const int DestW = int( std::round( W*XSizeRate ) );
		const int DestH = int( std::round( H*YSizeRate ) );
		bool ret = ( ::StretchBlt( hdc, DestLeft, DestTop, DestW,DestH, hmemdc, 0,0,W,H, SRCCOPY ) != 0 );

		SelectObject( hmemdc, holdbmp );
		DeleteDC( hmemdc );
		return ret;
	}

	//画像データの指定範囲を，bit値が0の箇所を透過する形で伸縮して描画
	//	hdc, DestLeft, DestTop : 描画先のDCと位置
	//	XSizeRate, YSizeRate : 描画倍率
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIBのうち描画する矩形範囲
	//	Bit1Color : 画像データのbit値が1の箇所に対する描画色
	bool StretchBlt_TransBit0Part(
		HDC hdc,
		int DestLeft, int DestTop,
		float XSizeRate, float YSizeRate,
		int SrcLeft, int SrcTop,
		int SrcWidth, int SrcHeight,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{
		HDC hmemdc = ::CreateCompatibleDC( hdc );
		if( hmemdc == NULL )return false;
		HBITMAP holdbmp = (HBITMAP)SelectObject( hmemdc, m_hBMP );

		const int DestW = int( std::round( SrcWidth*XSizeRate ) );
		const int DestH = int( std::round( SrcHeight*YSizeRate ) );

		::SetTextColor( hdc, RGB(255,255,255) );
		::SetBkColor( hdc, RGB(0,0,0) );
		bool ret = ( ::StretchBlt( hdc, DestLeft, DestTop, DestW,DestH, hmemdc, SrcLeft,SrcTop,SrcWidth,SrcHeight, SRCAND ) != 0 );

		::SetTextColor( hdc, RGB(0,0,0) );
		::SetBkColor( hdc, Bit1Color );
		ret &= ( ::StretchBlt( hdc, DestLeft, DestTop, DestW,DestH, hmemdc, SrcLeft,SrcTop,SrcWidth,SrcHeight, SRCPAINT ) != 0 );	//"SRCPAINT" is OR

		SelectObject( hmemdc, holdbmp );
		DeleteDC( hmemdc );
		return ret;
	}

	//画像全体を，データのbit値が0の箇所を透過する形で伸縮して描画
	bool StretchBlt_TransBit0Part(
		HDC hdc,
		int DestLeft, int DestTop,
		float XSizeRate, float YSizeRate,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{
		return StretchBlt_TransBit0Part( hdc, DestLeft, DestTop, XSizeRate, YSizeRate, 0,0, GetW(),GetH(), Bit1Color );
	}

	//画像データのbit値が0の箇所を透過する形で描画
	//	hdc, DestLeft, DestTop : 描画先のDCと位置
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIBのうち描画する矩形範囲
	//	Bit1Color : 画像データのbit値が1の箇所に対する描画色
	bool BitBlt_TransBit0Part(
		HDC hdc,
		int DestLeft, int DestTop,
		int SrcLeft, int SrcTop,
		int SrcWidth, int SrcHeight,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{
		HDC hmemdc = ::CreateCompatibleDC( hdc );
		if( hmemdc == NULL )return false;
		HBITMAP holdbmp = (HBITMAP)SelectObject( hmemdc, m_hBMP );

		::SetTextColor( hdc, RGB(255,255,255) );
		::SetBkColor( hdc, RGB(0,0,0) );
		bool ret = ( ::BitBlt( hdc, DestLeft,DestTop, SrcWidth,SrcHeight, hmemdc, SrcLeft,SrcTop, SRCAND ) != 0 );

		::SetTextColor( hdc, RGB(0,0,0) );
		::SetBkColor( hdc, Bit1Color );
		ret &= ( ::BitBlt( hdc, DestLeft,DestTop, SrcWidth,SrcHeight, hmemdc, SrcLeft,SrcTop, SRCPAINT ) != 0 );	//"SRCPAINT" is OR

		SelectObject( hmemdc, holdbmp );
		DeleteDC( hmemdc );
		return ret;
	}

	//全体を描画する
	bool BitBlt(
		HDC hdc, int DestLeft, int DestTop,
		COLORREF Bit1Color = RGB(255,255,255), COLORREF Bit0Color = RGB(0,0,0)
	) const
	{	return BitBlt( hdc, DestLeft,DestTop, 0,0,GetW(),GetH(), Bit1Color, Bit0Color );	}

	//全体を描画する（画像データのbit値が0の箇所を透過）
	bool BitBlt_TransBit0Part(
		HDC hdc, int DestLeft, int DestTop,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{	return BitBlt_TransBit0Part( hdc, DestLeft,DestTop, 0,0,GetW(),GetH(), Bit1Color );	}

public:
	//画像サイズ[pixel]
	int GetW() const {	return m_Width;	}
	int GetH() const {	return m_Height;	}
private:
	HBITMAP m_hBMP = NULL;
	int m_Width = 0;
	int m_Height = 0;
};

//[EOF]