#include "PlayData.h"
#include "Rnd.h"

using namespace GameContent;

PlayData::PlayData()
	: m_Chars{
		PartyChar(PartyCharID::AKINS), PartyChar(PartyCharID::FANA),
		PartyChar(PartyCharID::VISMATO), PartyChar(PartyCharID::LIBER),
		PartyChar(PartyCharID::IRY), PartyChar(PartyCharID::ASLAY),
		PartyChar(PartyCharID::ENA_MEA)
	}
	, m_CurrParty( 3 )	//パーティ最大人数
	, m_ItemStock( std::numeric_limits<size_t>::max() )
{
	//Test Code
	SetupParty( { PartyCharID::AKINS, PartyCharID::IRY, PartyCharID::ENA_MEA } );
	Char( PartyCharID::AKINS ).ChangeHP( -100 );
	Char( PartyCharID::ENA_MEA ).InfectPoison();
}

PlayData::~PlayData() = default;

//パーティを編成
void PlayData::SetupParty( const std::vector<GameContent::PartyCharID> &Chars )
{
	m_CurrParty.clear();
	for( auto ID : Chars )
	{
		if( !m_CurrParty.PushBack( ID ) )break;
	}
}

//宿宿泊時処理
std::vector< ActResult > PlayData::ProcOfINN()
{
	//回復処理
	std::vector< ActResult > Results;
	for( int iOrder=0; iOrder<(int)m_CurrParty.size(); ++iOrder )
	{
		auto &TgtChar = Char( m_CurrParty[iOrder] );

		//HPは最大HP分だけ回復
		const int RecovAmount = TgtChar.MaxHP();
		const int PrevHP = TgtChar.HP();
		TgtChar.ChangeHP( RecovAmount );
		Results.emplace_back( HPChanged( iOrder, RecovAmount, PrevHP, TgtChar.HP() ) );

		//MPは全回復
		TgtChar.FullRecoverMP();

		//毒の回復
		if( TgtChar.CurePoison() )
		{	Results.emplace_back( PoisonCured( iOrder ) );	}
	}

	//特殊処理
	//IRY がパーティにいて，宿泊結果として HP が 1以上であるとき，一定確率で HandMadePotion を得る
	if( m_CurrParty.Contains( PartyCharID::IRY ) )
	{
		auto &IRY = Char( PartyCharID::IRY );
		if( IRY.HP()>0  &&   RND().GetInt<int>( 1,100 )<=( IRY.Items().Contains( ItemID::HandMadePotion ) ? 20 : 60 ) )
		{	IRY.Items().PushBack( ItemID::HandMadePotion );	}
	}

	return Results;
}
