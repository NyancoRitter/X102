#pragma once

#include <string>

namespace TestGame::GameContent
{
	/// <summary>
	/// �U�������D
	/// �l�� 0 ����̘A�ԂƂ���D
	/// </summary>
	enum class AttackElement
	{
		Physical = 0,	//����
		Fire = 1,	//��
		Freeze = 2,	//�X
		Thunder = 3	//��
	};

	//��������
	inline const std::wstring &NameStr( AttackElement elem )
	{
		static const std::wstring Strs[] = { L"��", L"��", L"�X", L"��" };
		return Strs[ (int)elem ];
	}
};
