#pragma once
#include "Spell.h"
#include "ActProc/TgtRange.h"
#include "ActProc/ActEfficacy.h"

namespace GameContent
{
	/// <summary>
	/// 魔法の定義
	/// </summary>
	class Magic
	{
	public:
		Magic() = default;

		explicit Magic(
			FirstSpell Spell1st,
			SecondSpell Spell2nd,
			TgtRange Range,
			ActEfficacy Efficacy
		)
			: m_Spell{ Spell1st, Spell2nd }
			, m_Range( Range )
			, m_Name( NameStr(Spell1st) + L"・" + NameStr(Spell2nd) )
			, m_Efficacy( Efficacy )
		{}

	public:
		/// <summary>
		/// スペルの組み合わせに対応する魔法定義を参照
		/// </summary>
		/// <param name="Spell1st"></param>
		/// <param name="Spell2nd"></param>
		/// <returns></returns>
		static const Magic &Definition( FirstSpell Spell1st, SecondSpell Spell2nd );

	public:
		const std::wstring &Name() const {	return m_Name;	}
		std::pair< FirstSpell, SecondSpell > Spell() const {	return m_Spell; }
		TgtRange Range() const {	return m_Range;	}
		const ActEfficacy &Efficacy() const {	return m_Efficacy;	}
		const std::wstring &DescText() const {	return m_DescText;	}
		const std::wstring &FlavorText() const {	return m_FlavorText;	}

		Magic &Description( const std::wstring &Desc, const std::wstring &Flavor )
		{	m_DescText=Desc;	m_FlavorText=Flavor;	return *this;	}

	private:
		std::wstring m_Name;
		std::pair< FirstSpell, SecondSpell > m_Spell;
		TgtRange m_Range;
		ActEfficacy m_Efficacy;
		std::wstring m_DescText;
		std::wstring m_FlavorText;
	};
}
