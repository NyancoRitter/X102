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

	/// <summary>アイテム用アイコン画像ID．値は0から始まる連番とする</summary>
	enum class ItemImg
	{
		Potion, Antidote, Crystal,
		Sword, SwordL, Mace, Axe, Wand
	};

	//------------------------------------
	//モノクロBMPへの参照を取得

	const CMonoBMP &BMP( TownImg ID );
	const CMonoBMP &BMP( NPC ID );
	const CMonoBMP &BMP( GameContent::AttackElement Elem );
	const CMonoBMP &BMP( GameContent::FirstSpell Spell );
	const CMonoBMP &BMP( GameContent::SecondSpell Spell );
	const CMonoBMP &BMP( ItemImg ID );

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID );
}
