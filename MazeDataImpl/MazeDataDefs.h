#pragma once

namespace MazeDataImpl
{
	//迷路エッジ属性
	enum class EdgeAttr
	{
		NONE = 0,	//何も無い：通れる場所
		WALL = 1,	//壁：通れない場所
		DOOR = 2,	//この値以降はドア
	};

	//迷路内オブジェクト種類
	enum class ObjType : unsigned char
	{
		UP_STAIR = 0,	//登り階段
		DOWN_STAIR,	//下り階段
		CHAR,	//人物
		MAGIC_CIRCLE,	//魔法陣

		TREASURE_BOX_E,	//宝箱
		TREASURE_BOX_S,
		TREASURE_BOX_W,
		TREASURE_BOX_N
	};

	inline bool IsTreasureBox( ObjType t ){	return ( ObjType::TREASURE_BOX_E<=t && t<=ObjType::TREASURE_BOX_N );	}

	//FloorData 等で扱うオブジェクト情報
	struct ObjInfo
	{
		ObjType Type = ObjType::UP_STAIR;	//オブジェクト種類
		unsigned char Brightness = 0;	//描画時明るさ（表示用）
	};
}
