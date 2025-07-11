#pragma once
#include "framework.h"
#include <string>

namespace Toyger
{
	//------------------------------------

	/// <summary>BGM演奏手段</summary>
	class IBGMPlayer
	{
	public:
		~IBGMPlayer() = default;

		/// <summary>
		/// BGMファイルを登録する
		/// </summary>
		/// <param name="specifier">登録番号．ChangeBMG()での引数に使用する</param>
		/// <param name="rBGMFilePath">BGMファイルパス</param>
		/// <param name="ShouldKeepLong">
		/// このBGMファイルを読み込んだ内容を長期保持すべきか否か．
		/// falseの場合，他のBGMの再生を始めた際に破棄される．
		/// </param>
		/// <returns>
		/// 成否．指定の番号が既に登録されている場合には失敗となる．
		/// </returns>
		virtual bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong ) = 0;

		//再生BGMの変更
		virtual bool ChangeBGM( int specifier ) = 0;
		//演奏を止める
		virtual bool StopBGM() = 0;
	};
}