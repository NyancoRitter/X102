#pragma once
#include "AttackElement.h"
#include "ActProc/EfficacySource.h"
#include "ActProc/ActContext.h"
#include <vector>

namespace GameContent
{
	/// <summary>
	/// �L�����N�^
	/// �񕜂�U������ ���s��/�Ώ� �� I/F
	/// </summary>
	class ICharacter
	{
	public:
		virtual ~ICharacter() = default;

	public:	//�e��X�e�[�^�X�l
		//�����s�l��Ԃ��F���炩�̌��ʂő������Ă���Ȃ炻�̒l��Ԃ�

		virtual int MaxHP() const = 0;
		virtual int STR() const = 0;
		virtual int MAG() const = 0;
		virtual int AGI() const = 0;

		//�_���[�W�y����[%]
		virtual int DmgReducePercentage( GameContent::AttackElement Elem ) const = 0;
		//�ŕt�^���[%]
		virtual int PoisonResistPercentage() const = 0;

	public:	//HP, ��Ԉُ�֘A
		//����HP
		virtual int HP() const = 0;

		/// <summary>
		/// HP�̑���(��/�_���[�W)�D
		/// ���������ʂ� ���/���� �𒴂��Ȃ��l�ƂȂ�D
		/// </summary>
		/// <param name="dHP">���]�̕ω��ʁD���Ȃ�Ή񕜁C���Ȃ�_���[�W</param>
		virtual void ChangeHP( int dHP ) = 0;

		//�ŕt�^��Ԃ�
		virtual bool PoisonInfected() const = 0;

		/// <summary>�ł�t�^</summary>
		/// <returns>�V���ɕt�^�������ۂ��D�ŏ�Ԃł������ꍇ�ɂ�false�D</returns>
		virtual bool InfectPoison() = 0;

		/// <summary>�ł���</summary>
		/// <returns>�񕜂������ۂ��D�ŏ�ԂłȂ������ꍇ�ɂ�false�D</returns>
		virtual bool CurePoison() = 0;

	public:	//�ێ��X�L�����ʊ֘A

		/// <summary>
		/// ���g�̍s���ɂ��N����HP�񕜌��ʗʂւ̕␳����
		/// </summary>
		/// <param name="Source">���ʔ��������</param>
		/// <param name="RawAmount">���␳�̌��ʗ�</param>
		/// <param name="Context">�s���������</param>
		/// <returns>
		/// �␳���HP�񕜗ʁD
		/// �����l��Ԃ����ꍇ�C�A���I�Ɍ��ʂ𐶂���D
		/// </returns>
		virtual std::vector<double> ChangeRecovHPAmount(
			EfficacySource Source,
			double RawAmount,
			const ActContext &Context
		) const
		{	return {	RawAmount	};	}

		/// <summary>
		/// ���g�̍s���ɂ��N����U���̈З͂ւ̕␳����
		/// </summary>
		/// <param name="Source">���ʔ��������</param>
		/// <param name="Elem">�U������</param>
		/// <param name="RawPow">���␳�̍U���З�</param>
		/// <param name="TgtChar">�U���ΏۃL�����N�^���</param>
		/// <param name="Context">�s���������</param>
		/// <returns>�␳��̍U���З́D
		/// �����l��Ԃ����ꍇ�C���i�U���ƂȂ�D</returns>
		virtual std::vector<double> ChangeAtkPow(
			EfficacySource Source,
			AttackElement Elem,
			double RawPow,
			const ICharacter &TgtChar,
			const ActContext &Context
		) const
		{	return { RawPow };	}
	};

}