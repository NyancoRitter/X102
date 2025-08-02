#pragma once

class CMonoBMP;

namespace GameContent{	enum class PartyCharID;	}

namespace ResManage
{
	/// <summary>16x16[pixel]�A�C�R���摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class Icon16
	{
		Inn,
		Pub,
		Shop,
		Maze,

	};

	/// <summary>�X�̔w�i�摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class TownBkgnd
	{
		TownCenter,
		Outskirts
	};

	/// <summary>NPC�摜ID�D�l��0����n�܂�A�ԂƂ���</summary>
	enum class NPC
	{
		InnStaff,
		PubStaff,
		ShopStaff
	};

	//------------------------------------
	//���m�N��BMP�ւ̎Q�Ƃ��擾

	const CMonoBMP &BMP( Icon16 ID );
	const CMonoBMP &BMP( TownBkgnd ID );
	const CMonoBMP &BMP( NPC ID );

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID );
}
