#include "PlayData.h"

//セーブ
bool PlayData::Save( const std::string &FilePathU8 ) const
{
	return false;
}

//ロード
std::unique_ptr<PlayData> PlayData::Load( const std::string &FilePathU8 )
{
	return nullptr;
}
