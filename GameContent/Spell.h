#pragma once
#include <string>
#include "AttackElement.h"

namespace GameContent
{
	/// <summary>
	/// First Spell�D
	/// �l�� 0 ����̘A�ԂƂ���D
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
	/// Second Spell�D
	/// �l�� 0 ����̘A�ԂƂ���C���C{Fire, Ice, Thunder} �̒l�� AttackElement �ƈ�v������D
	/// </summary>
	enum class SecondSpell
	{
		Recover=0,
		Fire = AttackElement::Fire,
		Ice = AttackElement::Ice,
		Thunder = AttackElement::Thunder
	};
	constexpr int N_SecondSpell = 4;

	//�X�y����
	inline const std::wstring &NameStr( FirstSpell index )
	{
		static const std::wstring FirstSpellName[N_FirstSpell] = {
			L"�N�I�g", L"�~���g", L"�G���h",
			L"���g��", L"�o���t",
			L"�s�]�i"
		};
		return FirstSpellName[ (int)index ];
	}

	//�X�y����
	inline const std::wstring &NameStr( SecondSpell index )
	{
		static const std::wstring SecondSpellName[N_SecondSpell] = {
			L"�L�A",
			L"�J�m", L"���Y", L"���g"
		};
		return SecondSpellName[ (int)index ];
	}
}