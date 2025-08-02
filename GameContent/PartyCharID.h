#pragma once
#include <string>

namespace GameContent
{
	/// <summary>
	/// �p�[�e�B�L�����N�^�ŗLID�D
	/// �l�� 0 ����̘A�ԂƂ���D
	/// </summary>
	enum class PartyCharID
	{
		AKINS,
		FANA,
		VISMATO,
		LIBER,
		IRY,
		ASLAY,
		ENA_MEA
	};

	/// <summary>��`����Ă���p�[�e�B�L�����N�^�̐l��</summary>
	constexpr int N_PARTY_CHAR_DEF = 7;

	/// <summary>�L�����N�^�̖��O</summary>
	/// <param name="ID">�L�����N�^ID</param>
	/// <returns>���O</returns>
	inline const std::wstring &NameStr( PartyCharID ID )
	{
		static const std::wstring Names[N_PARTY_CHAR_DEF] = {
			L"�G�C�L���X",
			L"�t�@�i",
			L"���B�X�}�g",
			L"���[�x��",
			L"�C���B",
			L"�A�X���C",
			L"�G�i�E���A"
		};

		return Names[ (int)ID ];
	}
}
