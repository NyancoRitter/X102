#pragma once
#include <string>
#include "AttackElement.h"

namespace GameContent
{
	/// <summary>
	/// First Spell．
	/// 値は 0 からの連番とする．
	/// </summary>
	enum class FirstSpell
	{
		Single_LV1=0,
		Single_LV2,
		Single_LV3,
		All_LV1,
		All_LV2,
		Poison
	};
	constexpr int N_FirstSpell = 6;
	
	/// <summary>
	/// Second Spell．
	/// 値は 0 からの連番とする，且つ，{Fire, Ice, Thunder} の値は AttackElement と一致させる．
	/// </summary>
	enum class SecondSpell
	{
		Recover=0,
		Fire = AttackElement::Fire,
		Ice = AttackElement::Ice,
		Thunder = AttackElement::Thunder
	};
	constexpr int N_SecondSpell = 4;

	//スペル名
	inline const std::wstring &NameStr( FirstSpell index )
	{
		static const std::wstring FirstSpellName[N_FirstSpell] = {
			L"クオト", L"ミルト", L"エルド",
			L"ラトラ", L"バルフ",
			L"ピゾナ"
		};
		return FirstSpellName[ (int)index ];
	}

	//スペル名
	inline const std::wstring &NameStr( SecondSpell index )
	{
		static const std::wstring SecondSpellName[N_SecondSpell] = {
			L"キア",
			L"カノ", L"リズ", L"ルト"
		};
		return SecondSpellName[ (int)index ];
	}
}