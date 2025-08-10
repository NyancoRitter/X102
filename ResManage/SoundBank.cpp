#include "framework.h"
#include "SoundBank.h"
#include "Parts/Sound.h"
#include "DataPath.h"
#include <bitset>

namespace ResManage
{
	static std::array< std::wstring, N_SE > SE_FileNames;

	void SetupSE( const std::array< std::wstring, N_SE > &FileNames )
	{	SE_FileNames = FileNames;	}

	void PlaySE( SE se )
	{//※実装メモ：読めないファイルを何度もLoadしに行かないようにここでフラグ管理
		static Sound<N_SE> SEs;
		static std::bitset<N_SE> LoadErrFlags;	//ロード失敗フラグ

		const int index = (int)se;
		if( LoadErrFlags[index] )return;

		if( !SEs.Loaded( index ) )
		{
			if( !SEs.Load( index, DataPath::SoundDirU16()+SE_FileNames[index] ) )
			{
				LoadErrFlags.set(index);
				return;
			}
		}
		SEs.Play( index );
	}
}
