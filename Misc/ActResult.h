#pragma once

#include <variant>

//
//�A�C�e���▂�@���g�����C���炩�̍s�����s�������ʁC������������
//

/// <summary>HP�̕ύX(��or�_���[�W)</summary>
/// <typeparam name="TgtSpecifer">���ʔ����Ώۂ������^</typeparam>
template< class TgtSpecifer >
struct ChangeHP
{
	ChangeHP( TgtSpecifer TgtChar, int Amount, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), AfterHP( AfterHP )
	{}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;

	//���ʗʁi�ω������悤�Ƃ����l�j�D
	//�񕜂̏ꍇ�ɂ� 1�ȏ�̒l�C�_���[�W�̏ꍇ�ɂ� 0�ȉ��̒l�D
	//�i0�_���[�W�͂��蓾�邪�C0�񕜂͖����C�Ƃ����z��j
	//
	//�Ⴆ�΃A�C�e���▂�@�ɒ�`���ꂽ�񕜌��ʗʂ̂悤�Ȓl�ł���C�g�p���̎�ɕ\���ɗp����z��̂��́D
	//�K���������ۂɂ��̒l����HP���ω������킯�ł͂Ȃ��D
	//�i�Ⴆ�΁CHP��100�񕜂�����A�C�e�����g�p�����Ƃ��Ă��CHP�͍ő�HP��30�ɂ����Ȃ�Ȃ�������������Ȃ��j

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
struct PoionInfected
{
	PoionInfected( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;
};

/// <summary>�ŏ�Ԃ��������ꂽ</summary>
/// <typeparam name="TgtSpecifer">���ʔ����Ώۂ������^</typeparam>
template< class TgtSpecifer >
struct PoionCured
{
	PoionCured( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>�ΏۃL�����N�^</summary>
	TgtSpecifer TgtChar;
};

//------------------------------------
	
/// <summary>
/// ��퓬���p�D
/// 
/// ���ʑΏۂ̓p�[�e�B�����o�[�݂̂Ȃ̂ŁC
/// �e�^�� TgtSpecifer �́C�p�[�e�B���ł̑ΏۃL�����N�^�̈ʒu(���я�, 0-based) ������ int �D
/// </summary>
using PartyActResult = std::variant<
	ChangeHP<int>,
	PoionCured<int>
>;
