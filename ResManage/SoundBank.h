#pragma once
#include <array>
#include <string>

namespace ResManage
{
	//���ʉ��DPlaySE()�̈����p
	enum class SE
	{	Dmg1, Dmg2, Cure, LVUP	};

	//SE�̌�
	constexpr int N_SE = 4;

	/// <summary>
	/// SE�t�@�C�����̎w��i���������j
	/// PlaySE(se_index) �����{��������O��SE�t�@�C���Q�̃t�@�C�������w�肵�Ă����K�v������D
	/// </summary>
	/// <param name="FileNames">�t�@�C�����Q</param>
	void SetupSE( const std::array< std::wstring, N_SE > &FileNames );

	//���ʉ��̍Đ�
	void PlaySE( SE index );
}
