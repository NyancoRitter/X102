#include "ActEfficacy.h"
#include "GameContent/ICharacter.h"
#include "Rnd.h"

std::vector< ActResult > RecoverHP::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;
	const auto &ActChar = Context.CharAt( Actor );
	for( auto Specifier : Tgts )
	{
		auto &TgtChar = Context.CharAt( Specifier );

		const auto ActualAmounts = ActChar.ChangeRecovHPAmount( m_RecovSource, m_CalcRawAmount(ActChar), Context );
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


std::vector< ActResult > CurePoison::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;
	for( auto Specifier : Tgts )
	{
		if( Context.CharAt( Specifier ).CurePoison() )
		{	Results.emplace_back( PoisonCured( Specifier ) );	}
	}
	return Results;
}



std::vector< ActResult > Attack::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;

	auto &ActChar = Context.CharAt( Actor );
	int SuctionAmount = 0;

	for( auto Specifier : Tgts )
	{
		auto &TgtChar = Context.CharAt( Specifier );

		const auto RawPowers = m_CalcRawPower( ActChar );	//定義上の生の威力
		for( double RawPow : RawPowers )
		{
			//スキル効果などの影響の加味
			const auto ActualRawPowers = ActChar.ChangeAtkPow( m_AtkSource, m_AtkElem, RawPow, TgtChar, Context );
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
