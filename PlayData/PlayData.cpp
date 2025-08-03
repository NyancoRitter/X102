#include "PlayData.h"
#include "Rnd.h"

#include "ActProc/ActEfficacy.h"

using namespace GameContent;

PlayData::PlayData()
	: m_Chars{
		PartyChar(PartyCharID::AKINS), PartyChar(PartyCharID::FANA),
		PartyChar(PartyCharID::VISMATO), PartyChar(PartyCharID::LIBER),
		PartyChar(PartyCharID::IRY), PartyChar(PartyCharID::ASLAY),
		PartyChar(PartyCharID::ENA_MEA)
	}
	, m_CurrParty( 3 )	//�p�[�e�B�ő�l��
	, m_ItemStock( std::numeric_limits<size_t>::max() )
{
	//Test Code
	SetupParty( { PartyCharID::AKINS, PartyCharID::IRY, PartyCharID::ENA_MEA } );
}

PlayData::~PlayData() = default;

//�p�[�e�B��Ґ�
void PlayData::SetupParty( const std::vector<GameContent::PartyCharID> &Chars )
{
	m_CurrParty.clear();
	for( auto ID : Chars )
	{
		if( !m_CurrParty.PushBack( ID ) )break;
	}
}

//�h�h��������
std::vector< ActResult<int> > PlayData::ProcOfINN()
{
	//�񕜏���
	std::vector< ActResult<int> > Results;
	for( int iOrder=0; iOrder<(int)m_CurrParty.size(); ++iOrder )
	{
		auto &TgtChar = Char( m_CurrParty[iOrder] );

		//HP�͍ő�HP��������
		const int RecovAmount = TgtChar.MaxHP();
		TgtChar.ChangeHP( RecovAmount );
		Results.emplace_back( HPChanged<int>( iOrder, RecovAmount, TgtChar.HP() ) );

		//MP�͑S��
		TgtChar.FullRecoverMP();

		//�ł̉�
		if( TgtChar.CurePoison() )
		{	Results.emplace_back( PoisonCured<int>( iOrder ) );	}
	}

	//���ꏈ��
	//IRY ���p�[�e�B�ɂ��āC�h�����ʂƂ��� HP �� 1�ȏ�ł���Ƃ��C���m���� HandMadePotion �𓾂�
	if( m_CurrParty.Contains( PartyCharID::IRY ) )
	{
		auto &IRY = Char( PartyCharID::IRY );
		if( IRY.HP()>0  &&   RND().GetInt<int>( 1,100 )<=( IRY.Items().Contains( ItemID::HandMadePotion ) ? 20 : 60 ) )
		{	IRY.Items().PushBack( ItemID::HandMadePotion );	}
	}

	return Results;
}
