#include "Parts/CMonoBMP.h"
#include "MonoBmpHolder.h"
#include <stdexcept>

namespace ResManage
{
	std::unique_ptr<CMonoBMP> LoadMonoBMP_or_Dummy( const std::wstring &FilePathNameU16 )
	{
		{//ロードを試み，成功したならそれを返す．
			auto LoadResult = CMonoBMP::LoadBMPFile( FilePathNameU16 );
			if( LoadResult )return LoadResult;
		}

		//ロード失敗時
		//Create 16x16 Dummy Monochrome BMP
		UINT16 Ptn[16] = {
			0b0000'0000'0000'0000,
			0b0000'0000'0000'0000,
			0b0000'0011'1100'0000,
			0b0000'1111'1111'0000,
			0b0001'1100'0011'1000,
			0b0001'1000'0001'1000,
			0b0001'1000'0001'1000,
			0b0000'0000'0111'1000,
			0b0000'0001'1111'0000,
			0b0000'0001'1000'0000,
			0b0000'0001'1000'0000,
			0b0000'0000'0000'0000,
			0b0000'0001'1000'0000,
			0b0000'0001'1000'0000,
			0b0000'0000'0000'0000,
			0b0000'0000'0000'0000,
		};
		for( auto &L : Ptn ){	L = (L & 0xFF00)>>8  |  (L & 0x00FF)<<8;	}	//エンディアン的に必要っぽい
		HBITMAP hDummyBMP = ::CreateBitmap( 16,16,1,1, Ptn );
		return std::make_unique<CMonoBMP>( hDummyBMP );
	}

	//----------------------------------------------
	//MonoBmpBank

	MonoBmpHolder MonoBmpHolder::Create(
		const std::wstring &ImgDirU16,
		std::initializer_list<std::wstring> ImgFileNamesU16
	)
	{	return MonoBmpHolder( ImgDirU16, ImgFileNamesU16.begin(), ImgFileNamesU16.end() );	}

	MonoBmpHolder::~MonoBmpHolder() = default;

	const CMonoBMP& MonoBmpHolder::operator[]( int index )
	{
		if( index<0 || index>=(int)m_Bank.size() )throw std::out_of_range( "index out of range" );
		auto &Ret = m_Bank[index];
		if( !Ret )
		{//初回にロード
			Ret = LoadMonoBMP_or_Dummy( m_ImgDirU16 + m_ImgFileNamesU16[index] );
		}
		return *Ret;
	}
}
