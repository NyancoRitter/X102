#include "BmpBank.h"
#include "DataPath.h"
#include "MonoBmpHolder.h"

#include "GameContent/PartyCharID.h"

namespace ResManage
{
	const CMonoBMP &BMP( Icon16 ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			ImgDirU16(),
			{
				L"Location_Inn.bmp", L"Location_Pub.bmp", L"Location_Shop.bmp", L"Location_Maze.bmp"
			} 
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &BMP( TownBkgnd ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			ImgDirU16(),
			{ L"Town1.bmp", L"Town2.bmp" }
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &BMP( NPC ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			ImgDirU16(),
			{
				L"Staff1.bmp", L"Staff2.bmp", L"Staff3.bmp"
			} 
		);
		return Imgs[ (int)ID ];
	}

	const CMonoBMP &FaceBMP( GameContent::PartyCharID ID )
	{
		static auto Imgs = MonoBmpHolder::Create(
			ImgDirU16(),
			{	L"Face1.bmp", L"Face2.bmp", L"Face3.bmp", L"Face4.bmp", L"Face5.bmp", L"Face6.bmp"	} 
		);
		return Imgs[ (int)ID ];
	}
}
