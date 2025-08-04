#pragma once

#include "ActResult.h"
#include "ICharacter.h"
#include <functional>
#include <vector>
#include "GameContent/AttackElement.h"
#include "Rnd.h"

/// <summary>
/// TgtSpecifier からキャラクタを解決する手段
/// </summary>
using ToCharacter = std::function< ICharacter &( const TgtSpecifier &) >;

/// <summary>
/// 回復/ダメージ 効果量の変更処理
/// * 引数は生の(定義上の)効果量，効果を生む行動の実行者, 効果を受ける側
/// * 戻り値は変更結果効果量（複数値を返すことで，連続複数回を表現可能）
/// </summary>
using AmountChanger = std::function< std::vector<double>( double, const ICharacter &, const ICharacter & ) >;


//HP回復効果処理
class RecoverHP
{
public:	//定義用

	//回復効果量決定手段．引数は回復行動実行者
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
				const int dHP = std::max( 1, (int)std::round( d ) );	//※最低回復量 1 を保障
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

//毒回復処理
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

//攻撃処理
class Attack
{
public:	//定義用
	//攻撃威力決定手段．
	//引数は攻撃行動実行者．
	//多段攻撃の場合には複数要素を返す．
	using CalcRawPowerFunc = std::function< std::vector<double>(const ICharacter &) >;

	Attack( const CalcRawPowerFunc &CalcPower ) : m_CalcRawPower( CalcPower ) {}

	//攻撃属性．指定しない場合のデフォルトは物理．
	Attack &Elem( GameContent::AttackElement AtkElem ){	m_AtkElem=AtkElem;	return *this;	}
	//HP吸収攻撃にする
	Attack &Suction(){	m_IsSuction=true;	return *this;	}
	//毒付与効果を持たせる
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

			const auto RawPowers = m_CalcRawPower( ActChar );	//定義上の生の威力
			for( double RawPow : RawPowers )
			{
				//スキル効果などの影響の加味
				const auto ActualRawPowers = Changer( RawPow, ActChar, TgtChar );
				for( double Dmg : ActualRawPowers )
				{
					//被攻撃者の耐性による軽減
					Dmg *= ( ( 100 - TgtChar.DmgReducePercentage(m_AtkElem) ) / 100.0 );

					//乱数
					if( Dmg<=10.0 )
					{	Dmg += RND().GetInt<int>( -1, 1 );	}
					else
					{	Dmg *= ( RND().GetInt<int>( 90, 110 ) / 100.0f );	}

					//整数化
					int IntDmg = (int)std::round( Dmg );

					//全くダメージが通せないということにならないように
					//ダメージ計算結果が0になった場合，一定確率で 1 に修正する
					if( IntDmg==0 ){	IntDmg = RND().GetInt<int>( 0,1 );	}

					//
					if( IntDmg > 0 ){	SuctionAmount += IntDmg;	}

					//ダメージ処理
					const int PrevHP = TgtChar.HP();
					TgtChar.ChangeHP( IntDmg );
					Results.emplace_back( HPChanged( Specifier, IntDmg, PrevHP, TgtChar.HP() ) );

					if( m_WithPoison  &&  RND().GetInt<int>( 1, 100 ) > TgtChar.PoisonResistPercentage() )
					{//毒付与処理
						if( TgtChar.InfectPoison() )
						{	Results.emplace_back( PoisonInfected(Specifier) );	}
					}
				}
			}
		}

		if( m_IsSuction  &&  SuctionAmount>0 )
		{//吸収処理
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
/// アイテムや魔法等の効果
/// </summary>
using ActEfficacy = std::variant<
	CurePoison,	//※実装MEMO : デフォルト構築できる型を先頭に書いておく
	RecoverHP,
	Attack
>;
