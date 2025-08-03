#pragma once

#include <string>

namespace GameContent
{
	/// <summary>
	/// UŒ‚‘®«D
	/// ’l‚Í 0 ‚©‚ç‚Ì˜A”Ô‚Æ‚·‚éD
	/// </summary>
	enum class AttackElement
	{
		Physical = 0,	//•¨—
		Fire = 1,	//‰Î
		Ice = 2,	//•X
		Thunder = 3	//—‹
	};

	//‘®«–¼Ì
	inline const std::wstring &NameStr( AttackElement elem )
	{
		static const std::wstring Strs[] = { L"•¨", L"‰Î", L"•X", L"—‹" };
		return Strs[ (int)elem ];
	}
};
