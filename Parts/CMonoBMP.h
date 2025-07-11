//[CMonoBMP.h]

#pragma once
#include "framework.h"
#include <string>
#include <memory>
#include <stdexcept>
#include <cmath>

/// <summary>
/// ���m�N��(1bit)BMP�� �t�@�C���Ǎ���/�`��̎���
/// </summary>
class CMonoBMP
{
public:
	/// <summary>
	/// �����w��̃��m�N��BMP�̊Ǘ��҂Ƃ��Đ����D
	/// �������œn�����n���h���ɑ΂��� DeleteObject() ���f�X�g���N�^�Ŏ��{�����D
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
	/// BMP�t�@�C�������[�h���Đ���
	/// </summary>
	/// <param name="FilePathName">�t�@�C���p�X</param>
	/// <returns>���s����nullptr</returns>
	static std::unique_ptr<CMonoBMP> LoadBMPFile( const std::wstring &FilePathNameU16 )
	{
		auto hBMP = (HBITMAP)::LoadImage( NULL, FilePathNameU16.c_str(), IMAGE_BITMAP, 0,0, LR_LOADFROMFILE|LR_MONOCHROME );
		if( hBMP==NULL )return nullptr;

		try
		{	return std::unique_ptr< CMonoBMP >( new CMonoBMP( hBMP ) );	}
		catch( std::invalid_argument & )
		{	return nullptr;	}
	}

	//BitBlt : �P���`��
	//	hdc, DestLeft, DestTop : �`����DC�ƈʒu
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIB�̂����`�悷���`�͈�
	//	Bit1Color : �摜�f�[�^��bit�l��1�̉ӏ��ɑ΂���`��F
	//	Bit0Color : �摜�f�[�^��bit�l��0�̉ӏ��ɑ΂���`��F
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

	//�摜�S�̂�L�k���ĕ`��
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

	//�摜�f�[�^�̎w��͈͂��Cbit�l��0�̉ӏ��𓧉߂���`�ŐL�k���ĕ`��
	//	hdc, DestLeft, DestTop : �`����DC�ƈʒu
	//	XSizeRate, YSizeRate : �`��{��
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIB�̂����`�悷���`�͈�
	//	Bit1Color : �摜�f�[�^��bit�l��1�̉ӏ��ɑ΂���`��F
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

	//�摜�S�̂��C�f�[�^��bit�l��0�̉ӏ��𓧉߂���`�ŐL�k���ĕ`��
	bool StretchBlt_TransBit0Part(
		HDC hdc,
		int DestLeft, int DestTop,
		float XSizeRate, float YSizeRate,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{
		return StretchBlt_TransBit0Part( hdc, DestLeft, DestTop, XSizeRate, YSizeRate, 0,0, GetW(),GetH(), Bit1Color );
	}

	//�摜�f�[�^��bit�l��0�̉ӏ��𓧉߂���`�ŕ`��
	//	hdc, DestLeft, DestTop : �`����DC�ƈʒu
	//	SrcLeft,SrcTop,SrcWidth,SrcHeight : DIB�̂����`�悷���`�͈�
	//	Bit1Color : �摜�f�[�^��bit�l��1�̉ӏ��ɑ΂���`��F
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

	//�S�̂�`�悷��
	bool BitBlt(
		HDC hdc, int DestLeft, int DestTop,
		COLORREF Bit1Color = RGB(255,255,255), COLORREF Bit0Color = RGB(0,0,0)
	) const
	{	return BitBlt( hdc, DestLeft,DestTop, 0,0,GetW(),GetH(), Bit1Color, Bit0Color );	}

	//�S�̂�`�悷��i�摜�f�[�^��bit�l��0�̉ӏ��𓧉߁j
	bool BitBlt_TransBit0Part(
		HDC hdc, int DestLeft, int DestTop,
		COLORREF Bit1Color = RGB(255,255,255)
	) const
	{	return BitBlt_TransBit0Part( hdc, DestLeft,DestTop, 0,0,GetW(),GetH(), Bit1Color );	}

public:
	//�摜�T�C�Y[pixel]
	int GetW() const {	return m_Width;	}
	int GetH() const {	return m_Height;	}
private:
	HBITMAP m_hBMP = NULL;
	int m_Width = 0;
	int m_Height = 0;
};

//[EOF]