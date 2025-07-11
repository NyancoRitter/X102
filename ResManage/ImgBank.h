#pragma once

#include <string>
#include <memory>
class CMonoBMP;

namespace ResManage
{
	/// <summary>
	/// モノクロBMPをbitmapファイルからロードする．
	/// ただしファイルロードが失敗した場合でも何らかのダミー画像を返す．
	/// </summary>
	/// <param name="FilePathNameU16">ファイルパス</param>
	/// <returns></returns>
	std::unique_ptr<CMonoBMP> LoadMonoBMP_or_Dummy( const std::wstring &FilePathNameU16 );
}
