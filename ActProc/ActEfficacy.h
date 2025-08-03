#pragma once

#include "ActResult.h"
#include "ICharacter.h"
#include <functional>
#include <vector>

/// <summary>
/// TgtSpecifier からキャラクタを解決する手段
/// </summary>
using ToCharacter = std::function< ICharacter &(TgtSpecifier) >;

/// <summary>
/// アイテムや魔法等の効果の処理．
/// 
/// 引数:
/// * 効果を発生する行動の実行者
/// * 効果を受ける対象
/// * TgtSpecifer に対応するキャラクタデータを得る手段
/// 戻り値:
/// 起きた結果を示すデータ群
/// </summary>
using ActEfficacy = 
std::function<
	std::vector< ActResult >(
		TgtSpecifier,
		std::vector<TgtSpecifier>,
		ToCharacter&
	)
>;


//HP回復効果処理
class RecoverHP
{
public:
	//回復量決定手段．引数は行動実行者
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

//毒回復処理
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
