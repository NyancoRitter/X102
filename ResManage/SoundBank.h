#pragma once
#include <array>
#include <string>

namespace ResManage
{
	//効果音．PlaySE()の引数用
	enum class SE
	{	Dmg1, Dmg2, Cure, LVUP	};

	//SEの個数
	constexpr int N_SE = 4;

	/// <summary>
	/// SEファイル名の指定（初期処理）
	/// PlaySE(se_index) を実施するよりも前にSEファイル群のファイル名を指定しておく必要がある．
	/// </summary>
	/// <param name="FileNames">ファイル名群</param>
	void SetupSE( const std::array< std::wstring, N_SE > &FileNames );

	//効果音の再生
	void PlaySE( SE index );
}
