#pragma once

class CMonoBMP;

namespace GameContent
{	
	enum class PartyCharID;
	enum class AttackElement;
	enum class FirstSpell;
	enum class SecondSpell;
}

namespace ResManage
{
	/// <summary>街の画面で使う画像ID．値は0から始まる連番とする</summary>
	enum class TownImg
	{
		Inn, Pub, Shop, Maze,
		TownCenter, Outskirts
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

	const CMonoBMP &BMP( TownImg ID );
	const CMonoBMP &BMP( NPC ID );
	const CMonoBMP &BMP( GameContent::AttackElement Elem );
	const CMonoBMP &BMP( GameContent::FirstSpell Spell );
	const CMonoBMP &BMP( GameContent::SecondSpell Spell );

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID );
}
