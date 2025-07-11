#include "framework.h"
#include "Parts/CMonoBMP.h"

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
}
