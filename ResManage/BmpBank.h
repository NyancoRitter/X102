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
	/// <summary>�X�̉�ʂŎg���摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class TownImg
	{
		Inn, Pub, Shop, Maze,
		TownCenter, Outskirts
	};

	/// <summary>NPC�摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class NPC
	{
		InnStaff,
		PubStaff,
		ShopStaff
	};

	/// <summary>�A�C�e���p�A�C�R���摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class ItemImg
	{
		Potion, Antidote, Crystal,
		Sword, SwordL, Mace, Axe, Wand
	};

	//------------------------------------
	//���m�N��BMP�ւ̎Q�Ƃ��擾

	const CMonoBMP &BMP( TownImg ID );
	const CMonoBMP &BMP( NPC ID );
	const CMonoBMP &BMP( GameContent::AttackElement Elem );
	const CMonoBMP &BMP( GameContent::FirstSpell Spell );
	const CMonoBMP &BMP( GameContent::SecondSpell Spell );
	const CMonoBMP &BMP( ItemImg ID );

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID );
}
