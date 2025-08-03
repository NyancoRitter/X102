#pragma once

#include <variant>

//
//�A�C�e���▂�@���g�����C���炩�̍s�����s�������ʁC������������
//

/// <summary>HP�̕ύX(��or�_���[�W)</summary>
/// <typeparam name="TgtSpecifer">���ʔ����Ώۂ������^</typeparam>
template< class TgtSpecifer >
struct HPChanged
{
	HPChanged( TgtSpecifer TgtChar, int Amount, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), AfterHP( AfterHP )
	{}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;

	/// <summary>
	/// ���ʗʁi�ω������悤�Ƃ����l�j
	/// �Ⴆ�΃A�C�e���▂�@�ɒ�`���ꂽ�񕜌��ʗʂ̂悤�Ȓl�D
	/// �K���������ۂɂ��̒l����HP���ω������킯�ł͂Ȃ��D
	/// 
	/// * �񕜂̏ꍇ�ɂ� 1�ȏ�̒l�C�_���[�W�̏ꍇ�ɂ� 0�ȉ��̒l�D
	///   �i0�_���[�W�͂��蓾�邪�C0�񕜂͖����C�Ƃ����z��j
	/// </summary>
	int Amount;

	/// <summary>���ʔ���������̑Ώۂ�HP</summary>
	int AfterHP;
};

/// <summary>�ŏ�ԂɂȂ���</summary>
/// <typeparam name="TgtSpecifer">���ʔ����Ώۂ������^</typeparam>
template< class TgtSpecifer >
struct PoisonInfected
{
	PoisonInfected( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;
};

/// <summary>�ŏ�Ԃ��������ꂽ</summary>
/// <typeparam name="TgtSpecifer">���ʔ����Ώۂ������^</typeparam>
template< class TgtSpecifer >
struct PoisonCured
{
	PoisonCured( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;
};

//------------------------------------
	
template< class TgtSpecifer >
using ActResult = std::variant<
	HPChanged<TgtSpecifer>,
	PoisonInfected<TgtSpecifer>,
	PoisonCured<TgtSpecifer>
>;
