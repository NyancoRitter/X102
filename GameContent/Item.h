#pragma once

#include "ItemID.h"
#include "ActProc/TgtRange.h"
#include "ActProc/ActEfficacy.h"
#include <string>
#include "ResManage/BmpBank.h"

namespace GameContent
{
	/// <summary>
	/// アイテムの定義
	/// </summary>
	class Item
	{
	public:
		Item() = default;

		explicit Item(
			const std::wstring &Name,
			TgtRange Range,
			ActEfficacy Efficacy
		)
			: m_Name( Name )
			, m_Range( Range )
			, m_Efficacy( Efficacy )
		{}

	public:
		/// <summary>アイテム定義を参照</summary>
		/// <param name="ID"></param>
		/// <returns>不正なID値を指定した場合にはnullptrが返される</returns>
		static const Item *Definition( int ID );

		/// <summary>名称からアイテムのID値を得る</summary>
		/// <param name="Name"></param>
		/// <returns>ID値．ただし指定名称のアイテムが存在しない場合には負の値</returns>
		static int ID_from_Name( const std::wstring &Name );

	public:
		//ItemID ID() const {	return m_ID;	}
		const std::wstring &Name() const {	return m_Name;	}
		
		TgtRange Range() const {	return m_Range;	}
		const ActEfficacy &Efficacy() const {	return m_Efficacy;	}
		const std::wstring &DescText() const {	return m_DescText;	}
		const std::wstring &FlavorText() const {	return m_FlavorText;	}

		Item &Description( const std::wstring &Desc, const std::wstring &Flavor )
		{	m_DescText=Desc;	m_FlavorText=Flavor;	return *this;	}

		//値段
		int Price() const {	return m_Price;	}
		Item &Price( int price ){	m_Price=price;	return *this;	}

		//消耗品か否か
		bool CanUseOnlyOnce() const {	return m_CanUseOnlyOnce;	}
		Item &CanUseOnlyOnce( bool OnceOnly ){	m_CanUseOnlyOnce=OnceOnly;	return *this;	}

		//行動速度[%]
		int ActSpdPercentage() const {	return m_ActSpdPer;	}
		Item &ActSpdPercentage( int Per ){	m_ActSpdPer=Per;	return *this;	}

		//画像
		const CMonoBMP &Img() const {	return ResManage::BMP( m_ImgID );	}
		Item &ImgID( ResManage::ItemImg ID ){	m_ImgID=ID;	return *this;	}

	private:
		std::wstring m_Name;
		//ItemID m_ID;
		TgtRange m_Range;
		ActEfficacy m_Efficacy;
		std::wstring m_DescText;
		std::wstring m_FlavorText;

		int m_Price = 0;
		int m_ActSpdPer = 100;
		bool m_CanUseOnlyOnce = false;

		ResManage::ItemImg m_ImgID = ResManage::ItemImg::Potion;
	};
}
