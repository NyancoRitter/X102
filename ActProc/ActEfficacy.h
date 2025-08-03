#pragma once

#include "ActResult.h"
#include "ICharacter.h"
#include <functional>
#include <vector>

/// <summary>
/// TgtSpecifier ����L�����N�^�����������i
/// </summary>
using ToCharacter = std::function< ICharacter &(TgtSpecifier) >;

/// <summary>
/// �A�C�e���▂�@���̌��ʂ̏����D
/// 
/// ����:
/// * ���ʂ𔭐�����s���̎��s��
/// * ���ʂ��󂯂�Ώ�
/// * TgtSpecifer �ɑΉ�����L�����N�^�f�[�^�𓾂��i
/// �߂�l:
/// �N�������ʂ������f�[�^�Q
/// </summary>
using ActEfficacy = 
std::function<
	std::vector< ActResult >(
		TgtSpecifier,
		std::vector<TgtSpecifier>,
		ToCharacter&
	)
>;


//HP�񕜌��ʏ���
class RecoverHP
{
public:
	//�񕜗ʌ����i�D�����͍s�����s��
	using CalcAmountFunc = std::function<int(ICharacter &)>;

	RecoverHP( const CalcAmountFunc &CalcAmount ) : m_CalcAmount(CalcAmount) {}
public:
	std::vector< ActResult > operator()(
		TgtSpecifier Actor,
		std::vector<TgtSpecifier> Tgts,
		ToCharacter &ToChar
	) const
	{
		const int RecovAmount = m_CalcAmount( ToChar(Actor) );

		std::vector< ActResult > Results;
		for( auto Specifier : Tgts )
		{
			auto &TgtChar = ToChar( Specifier );
			const int PrevHP = TgtChar.HP();
			TgtChar.ChangeHP( RecovAmount );
			Results.emplace_back( HPChanged( Specifier, RecovAmount, PrevHP, TgtChar.HP() ) );
		}
		return Results;
	}
private:
	CalcAmountFunc m_CalcAmount;
};

//�ŉ񕜏���
class CurePoison
{
public:
	std::vector< ActResult > operator()(
		TgtSpecifier Actor,
		std::vector<TgtSpecifier> Tgts,
		ToCharacter &ToChar
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

//
