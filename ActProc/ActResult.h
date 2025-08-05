#pragma once

#include <variant>
#include "CharSpecifier.h"

//
//�A�C�e���▂�@���g�����C���炩�̍s�����s�������ʁC������������
//

/// <summary>
/// ��������(��or�_���[�W)�̏��D
/// �i���������ۂɂ�HP���ω����Ȃ������ꍇ�����蓾��j
/// </summary>
struct HPChanged
{
	HPChanged( CharSpecifier TgtChar, int Amount, int PrevHP, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), PrevHP( PrevHP ), AfterHP( AfterHP )
	{}

	/// <summary>�ΏۃL�����N�^</summary>
	CharSpecifier TgtChar;

	/// <summary>
	/// ���ʗʁi�ω������悤�Ƃ����l�j
	/// �Ⴆ�΃A�C�e���▂�@�ɒ�`���ꂽ�񕜌��ʗʂ̂悤�Ȓl�D
	/// �K���������ۂɂ��̒l����HP���ω������킯�ł͂Ȃ��D
	/// 
	/// * �񕜂̏ꍇ�ɂ� 1�ȏ�̒l�C�_���[�W�̏ꍇ�ɂ� 0�ȉ��̒l�D
	///   �i0�_���[�W�͂��蓾�邪�C0�񕜂͖����C�Ƃ����z��j
	/// </summary>
	int Amount;

	/// <summary>���ʔ��������O�̑Ώۂ�HP</summary>
	int PrevHP;

	/// <summary>���ʔ���������̑Ώۂ�HP</summary>
	int AfterHP;
};

/// <summary>�L�����N�^���ŏ�ԂɂȂ����|���������</summary>
struct PoisonInfected
{
	PoisonInfected( CharSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	CharSpecifier TgtChar;
};

/// <summary>�L�����N�^�̓ŏ�Ԃ��������ꂽ�|���������</summary>
struct PoisonCured
{
	PoisonCured( CharSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	CharSpecifier TgtChar;
};

//------------------------------------
	
using ActResult = std::variant<
	HPChanged,
	PoisonInfected,
	PoisonCured
>;
