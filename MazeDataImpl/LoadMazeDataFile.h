#pragma once

#include <vector>
#include "FloorData.h"

namespace MazeDataImpl
{
	/// <summary>
	/// 迷路のデータをファイルから読込む．
	/// 
	/// エラー時には UTF-8 文字列でエラーメッセージを持つ std::runtime_error を送出．
	/// なお，その際には結果受取用引数の内容は保証されない．
	/// </summary>
	/// <param name="MazeFilePathNameU8">ファイルパス(UTF8)</param>
	/// <param name="StartPos">スタート地点座標受取</param>
	/// <param name="StartDir">スタート時の向き受取</param>
	/// <returns>
	/// 読込んだ迷路マップ情報．
	/// 記述されていた階層分．
	/// </returns>
	std::vector< FloorData > LoadMazeMapFile( const std::string &MazeFilePathNameU8, Vec2i &StartPos, Direction &StartDir );
}
