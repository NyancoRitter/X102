#pragma once

#include "ActResult.h"
#include "GameContent/ICharacter.h"
#include <functional>
#include <vector>

using IChar = GameContent::ICharacter;

/// <summary>
/// アイテムや魔法等の効果の処理．
/// 
/// 引数:
/// * 効果を発生する行動の実行者
/// * 効果を受ける対象
/// 戻り値:
/// 起きた結果を示すデータ群
/// </summary>
/// <typeparam name="TgtSpecifier">実行者や対象を示すための型</typeparam>
template< class TgtSpecifier >
using ActEfficacy = std::function<   std::vector< ActResult<TgtSpecifier> >( TgtSpecifier, std::vector<TgtSpecifier> )   >;

/// <summary>
/// TgtSpecifier からキャラクタを解決する手段
/// </summary>
/// <typeparam name="TgtSpecifier"></typeparam>
template< class TgtSpecifier >
using ToCharacter = std::function< IChar &(TgtSpecifier) >;


//HP回復効果処理
template< class TgtSpecifier >
class RecoverHP
{
public:
	//回復量決定手段．引数は行動実行者
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

//毒回復処理
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
