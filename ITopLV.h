#pragma once
#include <string>

class PlayData;

/// <summary>
/// ITopLV::ChangeBGM()の引数に使える定数
/// </summary>
enum class BGMSpecifier
{
	Title,
	Town,
	Maze,
	Battle1, Battle2, Battle3,
	GameClear
};

/// <summary>
/// 各シーンに提供される．
/// シーン切り替え等の手段を提供．
/// </summary>
class ITopLV
{
public:
	virtual ~ITopLV() = default;

public:
	//プレイ状況データへのアクセス
	virtual PlayData &CurrPlayData() = 0;

public:	//シーン遷移

	virtual void ChangeToTownScene() = 0;


public:	//BGM
	virtual bool ChangeBGM( int BGM_index ) = 0;
	virtual void StopBMG() = 0;

public:	//Other

	//メッセージボックスのモーダル表示
	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const = 0;
};
