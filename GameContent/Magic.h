#pragma once
#include "Spell.h"
#include "ActProc/TgtRange.h"
#include "ActProc/ActEfficacy.h"

namespace GameContent
{
	class Magic
	{
	public:
		Magic(
			FirstSpell Spell1st,
			SecondSpell Spell2nd,
			TgtRange Range,
			std::initializer_list< ActEfficacy > Efficacies
		)
			: m_Spell{ Spell1st, Spell2nd }
			, m_Range( Range )
			, m_Name( NameStr(Spell1st) + L"ÅE" + NameStr(Spell2nd) )
			, m_Efficacies( Efficacies )
		{}

	public:
		const std::wstring &Name() const {	return m_Name;	}
		std::pair< FirstSpell, SecondSpell > Spell() const {	return m_Spell; }
		TgtRange Range() const {	return m_Range;	}
		const std::vector< ActEfficacy > &Efficacies() const {	return m_Efficacies;	}
		const std::wstring &DescText() const {	return m_DescText;	}
		const std::wstring &FlavorText() const {	return m_FlavorText;	}

		Magic &Description( const std::wstring &DescU8, const std::wstring &FlavorU8 )
		{	m_DescText=DescU8;	m_FlavorText=FlavorU8;	return *this;	}

	private:
		std::wstring m_Name;
		std::pair< FirstSpell, SecondSpell > m_Spell;
		TgtRange m_Range;
		std::vector< ActEfficacy > m_Efficacies;
		std::wstring m_DescText;
		std::wstring m_FlavorText;
	};
}
