#include "framework.h"
#include "CampMenu.h"
#include "PlayData/PlayData.h"
#include "GameContent/Magic.h"
#include "GameContent/PartyChar.h"

using namespace GameContent;


//PartyChar &CampMenu::ToChar( const TgtSpecifier &tgt )
//{
//	const auto Party = m_rPlayData.CurrParty();
//	return m_rPlayData.Char( Party[m_iCurrChar] );
//}

void CampMenu::UseMagic( const GameContent::Magic &Magic, int iTgtOrder )
{
	if( m_iCurrChar<0 )return;

	//�����ł͖�����ΏۂƂ��閂�@�����g���Ȃ�
	if( !IsForFriend( Magic.Range() ) )return;

	const auto Party = m_rPlayData.CurrParty();
	auto &User = m_rPlayData.Char( Party[m_iCurrChar] );
	const auto Spell = Magic.Spell();

	//MP�c�ʃ`�F�b�N
	if( User.MP( Spell.first )<=0  ||  User.MP( Spell.second ) )return;

	//
	const auto &Efficacy = Magic.Efficacy();

	//...

	
}
