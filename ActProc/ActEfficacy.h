#pragma once

#include "ActResult.h"
#include "GameContent/ICharacter.h"
#include <functional>
#include <vector>

using IChar = GameContent::ICharacter;

/// <summary>
/// �A�C�e���▂�@���̌��ʂ̏����D
/// 
/// ����:
/// * ���ʂ𔭐�����s���̎��s��
/// * ���ʂ��󂯂�Ώ�
/// �߂�l:
/// �N�������ʂ������f�[�^�Q
/// </summary>
/// <typeparam name="TgtSpecifier">���s�҂�Ώۂ��������߂̌^</typeparam>
template< class TgtSpecifier >
using ActEfficacy = std::function<   std::vector< ActResult<TgtSpecifier> >( TgtSpecifier, std::vector<TgtSpecifier> )   >;

/// <summary>
/// TgtSpecifier ����L�����N�^�����������i
/// </summary>
/// <typeparam name="TgtSpecifier"></typeparam>
template< class TgtSpecifier >
using ToCharacter = std::function< IChar &(TgtSpecifier) >;


//HP�񕜌��ʏ���
template< class TgtSpecifier >
class RecoverHP
{
public:
	//�񕜗ʌ����i�D�����͍s�����s��
	using CalcAmountFunc = std::function<int(IChar &)>;

	RecoverHP(
		const ToCharacter<TgtSpecifier> &ToChar,
		const CalcAmountFunc &CalcAmount
	) : m_ToChar(ToChar), m_CalcAmount(CalcAmount)
	{}

public:
	std::vector< ActResult<TgtSpecifier> > operator()( TgtSpecifier Actor, std::vector<TgtSpecifier> Tgts )
	{
		const int RecovAmount = m_CalcAmount( m_ToChar(Actor) );

		std::vector< ActResult<TgtSpecifier> > Results;
		for( auto Specifier : Tgts )
		{
			auto &TgtChar = m_ToChar( Specifier );
			TgtChar.ChangeHP( RecovAmount );
			Results.emplace_back( HPChanged( Specifier, RecovAmount, TgtChar.HP() ) );
		}
		return Results;
	}
private:
	ToCharacter<TgtSpecifier> m_ToChar;
	CalcAmountFunc m_CalcAmount;
};

//�ŉ񕜏���
template< class TgtSpecifier >
class CurePoison
{
public:
	CurePoison( const ToCharacter<TgtSpecifier> &ToChar ) : m_ToChar(ToChar) {}

public:
	std::vector< ActResult<TgtSpecifier> > operator()( TgtSpecifier, std::vector<TgtSpecifier> Tgts )
	{
		std::vector< ActResult<TgtSpecifier> > Results;
		for( auto Specifier : Tgts )
		{
			if( m_ToChar( Specifier ).CurePoison() )
			{	Results.emplace_back( PoisonCured( Specifier ) );	}
		}
		return Results;
	}
private:
	ToCharacter<TgtSpecifier> m_ToChar;
};
