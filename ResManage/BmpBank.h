#pragma once

class CMonoBMP;

namespace GameContent{	enum class PartyCharID;	}

namespace ResManage
{
	/// <summary>16x16[pixel]アイコン画像ID．値は0から始まる連番とする</summary>
	enum class Icon16
	{
		Inn,
		Pub,
		Shop,
		Maze,

	};

	/// <summary>街の背景画像ID．値は0から始まる連番とする</summary>
	enum class TownBkgnd
	{
		TownCenter,
		Outskirts
	};

	/// <summary>NPC画像ID．値は0から始まる連番とする</summary>
	enum class NPC
	{
		InnStaff,
		PubStaff,
		ShopStaff
	};

	//------------------------------------
	//モノクロBMPへの参照を取得

	const CMonoBMP &BMP( Icon16 ID );
	const CMonoBMP &BMP( TownBkgnd ID );
	const CMonoBMP &BMP( NPC ID );

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID );
}
