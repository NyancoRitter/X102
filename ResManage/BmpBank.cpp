#include "BmpBank.h"
#include "DataPath.h"
#include "MonoBmpHolder.h"

#include "Parts/CMonoBMP.h"
#include "GameContent/PartyCharID.h"
#include "GameContent/AttackElement.h"
#include "GameContent/Spell.h"

namespace ResManage
{
	const CMonoBMP &BMP( TownImg ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{
				L"Location_Inn.bmp", L"Location_Pub.bmp", L"Location_Shop.bmp", L"Location_Maze.bmp",
				L"Town1.bmp", L"Town2.bmp"
			} 
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &BMP( NPC ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{
				L"Staff1.bmp", L"Staff2.bmp", L"Staff3.bmp"
			} 
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &BMP( ItemImg ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{
				L"Potion.bmp", L"Potion2.bmp", L"Crystal.bmp",
				L"Sword.bmp", L"SwordL.bmp", L"Mace.bmp", L"Axe.bmp", L"Wand.bmp"
			} 
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{	L"Face1.bmp", L"Face2.bmp", L"Face3.bmp", L"Face4.bmp", L"Face5.bmp", L"Face6.bmp", L"Face7.bmp"	} 
		);
		return Imgs[ (int)ID ];
	}

	//
	const CMonoBMP &BMP( GameContent::AttackElement Elem )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{	L"Sword.bmp", L"Fire.bmp", L"Ice.bmp", L"Thunder.bmp"	} 
		);
		return Imgs[ (int)Elem ];
	}

	const CMonoBMP &BMP( GameContent::FirstSpell Spell )
	{
		static auto Imgs = MonoBmpHolder::Create(
			DataPath::ImgDirU16(),
			{	L"Single1.bmp", L"Single2.bmp", L"Single3.bmp", L"All1.bmp", L"All2.bmp", L"Poison.bmp"	} 
		);
		return Imgs[ (int)Spell ];
	}

	const CMonoBMP &BMP( GameContent::SecondSpell Spell )
	{
		static const auto upCureImg = LoadMonoBMP_or_Dummy( DataPath::ImgDirU16() + L"Cure.bmp" );

		if( Spell == GameContent::SecondSpell::Recover )return *upCureImg;

		return BMP( (GameContent::AttackElement)(int)Spell );
	}
}
