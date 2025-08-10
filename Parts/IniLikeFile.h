//[IniLikeFile.h]

#pragma once

#include "SectionFileRead.h"
#include <vector>

//INIっぽいセクション区切りファイルの読み込み．
//
// * セクション内は
//	Key = Value
//  という '=' を用いた記述．
//

namespace IniLikeFile
{
	//セクションの読み込み結果データ
	//  map< Key, Value >
	using SectionData = std::map< std::string, std::string >;
	//ファイル読み込み結果データ
	//  map< Section名, SectionData >
	using FileData = std::map< std::string, SectionData >;

	//ファイルローダ
	class FileLoader : public SectionFile::FileLoader<SectionData>
	{
	protected:
		virtual bool ParseLine( const std::string &LineStr, SectionData &SecData, int LineNo ) override;
	};

	/// <summary>
	/// ヘルパ関数．
	/// セクションデータ SecData の Key で指定した要素の Val をカンマで分割する
	/// SecData にKeyが含まれていない場合，std::runtime_error を送出．
	/// </summary>
	/// <param name="SecData">セクションデータ</param>
	/// <param name="Key">キー</param>
	/// <param name="delimiters">デリミタ</param>
	/// <returns>
	/// 分割結果．
	/// 単純にデリミタで切り分けた結果（：個々の要素の前後のスペース除去等はなされない）
	/// </returns>
	std::vector< std::string > SplitVal(
		const SectionData &SecData,
		const std::string &Key,
		const std::string &delimiters=","
	);
}
