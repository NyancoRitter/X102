#pragma once

namespace GameContent{	enum class AttackElement;	}

/// <summary>
/// �L�����N�^
/// �񕜂�U�����̑Ώ�
/// </summary>
class ICharacter
{
public:
	virtual ~ICharacter() = default;

public:
	//����LV
	virtual int LV() const = 0;
	//����HP
	virtual int HP() const = 0;

	//�ŕt�^��Ԃ�
	virtual bool PoisonInfected() const = 0;

	//�e��X�e�[�^�X�l
	virtual int MaxHP() const = 0;
	virtual int STR() const = 0;
	virtual int MAG() const = 0;
	virtual int AGI() const = 0;

	//�_���[�W�y����[%]
	virtual int DmgReducePercentage( GameContent::AttackElement Elem ) const = 0;
	//�ŕt�^���[%]
	virtual int PoisonResistPercentage() const = 0;

	/// <summary>
	/// HP�̑���(��/�_���[�W)�D
	/// ���������ʂ� ���/���� �𒴂��Ȃ��l�ƂȂ�D
	/// </summary>
	/// <param name="dHP">���]�̕ω��ʁD���Ȃ�Ή񕜁C���Ȃ�_���[�W</param>
	virtual void ChangeHP( int dHP ) = 0;

	/// <summary>�ł�t�^</summary>
	/// <returns>�V���ɕt�^�������ۂ��D�ŏ�Ԃł������ꍇ�ɂ�false�D</returns>
	virtual bool InfectPoison() = 0;

	/// <summary>�ł���</summary>
	/// <returns>�񕜂������ۂ��D�ŏ�ԂłȂ������ꍇ�ɂ�false�D</returns>
	virtual bool CurePoison() = 0;
};

