#pragma once

#include "ActResult.h"
#include "ActContext.h"
#include "EfficacySource.h"
#include "GameContent/AttackElement.h"

#include <functional>
#include <vector>

/// <summary>
/// �A�C�e���▂�@�ɂ�萶������ʂ̏���
/// </summary>
using ActEfficacy = 
std::function<
	std::vector< ActResult >(
		CharSpecifier Actor,	//���ʂ𔭐�������s���̎��{��
		std::vector< CharSpecifier > Tgts,	//���ʂ�^����Ώ�
		const ActContext &Context	//�����ɕK�v�ȕ���
	)
>;

//HP�񕜏���
class RecoverHP
{
public:	//��`�p

	//�񕜌��ʗʌ����i�D�����͉񕜍s�����s��
	using CalcRawAmountFunc = std::function< double( const GameContent::ICharacter & ) >;

	RecoverHP( const CalcRawAmountFunc &CalcAmount, EfficacySource RecovSource=EfficacySource::Other )
		: m_CalcRawAmount(CalcAmount), m_RecovSource(RecovSource)
	{}

public:
	std::vector< ActResult > operator()(
		CharSpecifier Actor,
		std::vector< CharSpecifier > Tgts,
		const ActContext &Context
	) const;
private:
	CalcRawAmountFunc m_CalcRawAmount;
	EfficacySource m_RecovSource;
};

//�ŉ񕜏���
class CurePoison
{
public:
	std::vector< ActResult > operator()(
		CharSpecifier Actor,
		std::vector< CharSpecifier > Tgts,
		const ActContext &Context
	) const;
};

//�U������
class Attack
{
public:	//��`�p
	//�U���З͌����i�D�����͍U���s�����s�ҁD
	//���i�U���̏ꍇ�ɂ͕����v�f��Ԃ��D
	using CalcRawPowerFunc = std::function< std::vector<double>(const GameContent::ICharacter &) >;

	Attack( const CalcRawPowerFunc &CalcPower, EfficacySource AtkSource=EfficacySource::Other )
		: m_CalcRawPower( CalcPower ), m_AtkSource(AtkSource)
	{}

	//�U�������D�w�肵�Ȃ��ꍇ�̃f�t�H���g�͕����D
	Attack &Elem( GameContent::AttackElement AtkElem ){	m_AtkElem=AtkElem;	return *this;	}
	//HP�z���U���ɂ���
	Attack &Suction(){	m_IsSuction=true;	return *this;	}
	//�ŕt�^���ʂ���������
	Attack &WithPoison(){	m_WithPoison=true;	return *this;	}

public:
	GameContent::AttackElement Element() const {	return m_AtkElem;	}

	std::vector< ActResult > operator()(
		CharSpecifier Actor,
		std::vector< CharSpecifier > Tgts,
		const ActContext &Context
	) const;

private:
	CalcRawPowerFunc m_CalcRawPower;
	EfficacySource m_AtkSource;
	GameContent::AttackElement m_AtkElem = GameContent::AttackElement::Physical;
	bool m_IsSuction = false;
	bool m_WithPoison = false;
};
