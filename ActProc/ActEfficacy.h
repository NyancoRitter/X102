#pragma once

#include "ActResult.h"
#include "ICharacter.h"
#include <functional>
#include <vector>
#include "GameContent/AttackElement.h"
#include "Rnd.h"

/// <summary>
/// TgtSpecifier ����L�����N�^�����������i
/// </summary>
using ToCharacter = std::function< ICharacter &( const TgtSpecifier &) >;

/// <summary>
/// ��/�_���[�W ���ʗʂ̕ύX����
/// * �����͐���(��`���)���ʗʁC���ʂ𐶂ލs���̎��s��, ���ʂ��󂯂鑤
/// * �߂�l�͕ύX���ʌ��ʗʁi�����l��Ԃ����ƂŁC�A���������\���\�j
/// </summary>
using AmountChanger = std::function< std::vector<double>( double, const ICharacter &, const ICharacter & ) >;


//HP�񕜌��ʏ���
class RecoverHP
{
public:	//��`�p

	//�񕜌��ʗʌ����i�D�����͉񕜍s�����s��
	using CalcRawAmountFunc = std::function<double(const ICharacter &)>;

	RecoverHP( const CalcRawAmountFunc &CalcAmount ) : m_CalcRawAmount(CalcAmount) {}

public:
	std::vector< ActResult > operator()(
		TgtSpecifier Actor,
		std::vector<TgtSpecifier> Tgts,
		const ToCharacter &ToChar,
		const AmountChanger &Changer
	) const
	{
		std::vector< ActResult > Results;
		const auto &ActChar = ToChar( Actor );
		for( auto Specifier : Tgts )
		{
			auto &TgtChar = ToChar( Specifier );

			const auto ActualAmounts = Changer( m_CalcRawAmount(ActChar), ActChar, TgtChar );
			for( double d : ActualAmounts )
			{
				const int dHP = std::max( 1, (int)std::round( d ) );	//���Œ�񕜗� 1 ��ۏ�
				const int PrevHP = TgtChar.HP();
				TgtChar.ChangeHP( dHP );
				Results.emplace_back( HPChanged( Specifier, dHP, PrevHP, TgtChar.HP() ) );
			}
		}
		return Results;
	}
private:
	CalcRawAmountFunc m_CalcRawAmount;
};

//�ŉ񕜏���
class CurePoison
{
public:
	std::vector< ActResult > operator()(
		TgtSpecifier Actor,
		std::vector<TgtSpecifier> Tgts,
		const ToCharacter &ToChar
	) const
	{
		std::vector< ActResult > Results;
		for( auto Specifier : Tgts )
		{
			if( ToChar( Specifier ).CurePoison() )
			{	Results.emplace_back( PoisonCured( Specifier ) );	}
		}
		return Results;
	}
};

//�U������
class Attack
{
public:	//��`�p
	//�U���З͌����i�D
	//�����͍U���s�����s�ҁD
	//���i�U���̏ꍇ�ɂ͕����v�f��Ԃ��D
	using CalcRawPowerFunc = std::function< std::vector<double>(const ICharacter &) >;

	Attack( const CalcRawPowerFunc &CalcPower ) : m_CalcRawPower( CalcPower ) {}

	//�U�������D�w�肵�Ȃ��ꍇ�̃f�t�H���g�͕����D
	Attack &Elem( GameContent::AttackElement AtkElem ){	m_AtkElem=AtkElem;	return *this;	}
	//HP�z���U���ɂ���
	Attack &Suction(){	m_IsSuction=true;	return *this;	}
	//�ŕt�^���ʂ���������
	Attack &WithPoison(){	m_WithPoison=true;	return *this;	}

public:
	GameContent::AttackElement Element() const {	return m_AtkElem;	}

public:
	std::vector< ActResult > operator()(
		TgtSpecifier Actor,
		std::vector<TgtSpecifier> Tgts,
		const ToCharacter &ToChar,
		const AmountChanger &Changer
	) const
	{
		std::vector< ActResult > Results;

		auto &ActChar = ToChar( Actor );
		int SuctionAmount = 0;

		for( auto Specifier : Tgts )
		{
			auto &TgtChar = ToChar( Specifier );

			const auto RawPowers = m_CalcRawPower( ActChar );	//��`��̐��̈З�
			for( double RawPow : RawPowers )
			{
				//�X�L�����ʂȂǂ̉e���̉���
				const auto ActualRawPowers = Changer( RawPow, ActChar, TgtChar );
				for( double Dmg : ActualRawPowers )
				{
					//��U���҂̑ϐ��ɂ��y��
					Dmg *= ( ( 100 - TgtChar.DmgReducePercentage(m_AtkElem) ) / 100.0 );

					//����
					if( Dmg<=10.0 )
					{	Dmg += RND().GetInt<int>( -1, 1 );	}
					else
					{	Dmg *= ( RND().GetInt<int>( 90, 110 ) / 100.0f );	}

					//������
					int IntDmg = (int)std::round( Dmg );

					//�S���_���[�W���ʂ��Ȃ��Ƃ������ƂɂȂ�Ȃ��悤��
					//�_���[�W�v�Z���ʂ�0�ɂȂ����ꍇ�C���m���� 1 �ɏC������
					if( IntDmg==0 ){	IntDmg = RND().GetInt<int>( 0,1 );	}

					//
					if( IntDmg > 0 ){	SuctionAmount += IntDmg;	}

					//�_���[�W����
					const int PrevHP = TgtChar.HP();
					TgtChar.ChangeHP( IntDmg );
					Results.emplace_back( HPChanged( Specifier, IntDmg, PrevHP, TgtChar.HP() ) );

					if( m_WithPoison  &&  RND().GetInt<int>( 1, 100 ) > TgtChar.PoisonResistPercentage() )
					{//�ŕt�^����
						if( TgtChar.InfectPoison() )
						{	Results.emplace_back( PoisonInfected(Specifier) );	}
					}
				}
			}
		}

		if( m_IsSuction  &&  SuctionAmount>0 )
		{//�z������
			const int PrevHP = ActChar.HP();
			ActChar.ChangeHP( SuctionAmount );
			Results.emplace_back( HPChanged( Actor, SuctionAmount, PrevHP, ActChar.HP() ) );
		}

		return Results;
	}

private:
	GameContent::AttackElement m_AtkElem = GameContent::AttackElement::Physical;
	CalcRawPowerFunc m_CalcRawPower;

	bool m_IsSuction = false;
	bool m_WithPoison = false;
};


/// <summary>
/// �A�C�e���▂�@���̌���
/// </summary>
using ActEfficacy = std::variant<
	CurePoison,	//������MEMO : �f�t�H���g�\�z�ł���^��擪�ɏ����Ă���
	RecoverHP,
	Attack
>;
