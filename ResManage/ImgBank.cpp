#include "framework.h"
#include "Parts/CMonoBMP.h"

namespace ResManage
{
	std::unique_ptr<CMonoBMP> LoadMonoBMP_or_Dummy( const std::wstring &FilePathNameU16 )
	{
		{//���[�h�����݁C���������Ȃ炻���Ԃ��D
			auto LoadResult = CMonoBMP::LoadBMPFile( FilePathNameU16 );
			if( LoadResult )return LoadResult;
		}

		//���[�h���s��
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
		for( auto &L : Ptn ){	L = (L & 0xFF00)>>8  |  (L & 0x00FF)<<8;	}	//�G���f�B�A���I�ɕK�v���ۂ�
		HBITMAP hDummyBMP = ::CreateBitmap( 16,16,1,1, Ptn );
		return std::make_unique<CMonoBMP>( hDummyBMP );
	}
}
