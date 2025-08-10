#pragma once

#include "ItemID.h"
#include "ActProc/TgtRange.h"
#include "ActProc/ActEfficacy.h"
#include <string>
#include "ResManage/BmpBank.h"

namespace GameContent
{
	/// <summary>
	/// �A�C�e���̒�`
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
		/// <summary>�A�C�e����`���Q��</summary>
		/// <param name="ID"></param>
		/// <returns>�s����ID�l���w�肵���ꍇ�ɂ�nullptr���Ԃ����</returns>
		static const Item *Definition( int ID );

		/// <summary>���̂���A�C�e����ID�l�𓾂�</summary>
		/// <param name="Name"></param>
		/// <returns>ID�l�D�������w�薼�̂̃A�C�e�������݂��Ȃ��ꍇ�ɂ͕��̒l</returns>
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

		//�l�i
		int Price() const {	return m_Price;	}
		Item &Price( int price ){	m_Price=price;	return *this;	}

		//���Օi���ۂ�
		bool CanUseOnlyOnce() const {	return m_CanUseOnlyOnce;	}
		Item &CanUseOnlyOnce( bool OnceOnly ){	m_CanUseOnlyOnce=OnceOnly;	return *this;	}

		//�s�����x[%]
		int ActSpdPercentage() const {	return m_ActSpdPer;	}
		Item &ActSpdPercentage( int Per ){	m_ActSpdPer=Per;	return *this;	}

		//�摜
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
