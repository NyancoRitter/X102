#include "framework.h"
#include "CampMenu.h"
#include "PlayData/PlayData.h"
#include "GameContent/Magic.h"
#include "GameContent/PartyChar.h"

#include "TopLVMenu.h"
#include "MagicPage.h"

#include "ActProc/ActEfficacyImpl.h"
#include "ActProc/ActContext.h"

using namespace GameContent;

namespace
{
	//�p�[�e�B���j���[�ł̃A�C�e���▂�@�̎g�p�����p
	class UseCtxt : public ActContext
	{
	private:
		PlayData &m_rPD;
	public:
		UseCtxt( PlayData &PD ) : m_rPD(PD) {} 

		virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const override
		{	return m_rPD.Char( m_rPD.CurrParty()[ Tgt.m_Order ] );	}	//Tgt�͕K�������L�����N�^���w���Ƃ����O��ł̎���
	};
}

//
bool CampMenu::Affect( const ActEfficacy &Efficacy, TgtRange Range, int iTgtOrder )
{
	//���ʏ���
	std::vector< ActResult > Results;
	{	
		std::vector< CharSpecifier > Tgts;
		if( IsForSingle( Range ) )
		{	Tgts.emplace_back( iTgtOrder );	}
		else
		{
			for( int i=0; i<(int)m_rPlayData.CurrParty().size(); ++i )
			{	Tgts.emplace_back( i );	}
		}

		Results = Efficacy( CharSpecifier(m_iCurrChar), std::move(Tgts), UseCtxt{m_rPlayData} );
	}

	{//�L���Ȍ��ʂ��P�������Ă��Ȃ��ꍇ�́u�g�p�����p�i�����������Ƃɂ���j�v���߂̃`�F�b�N�D
		bool AnyValid = false;
		for( const auto &R : Results )
		{
			if( const auto *p=std::get_if<HPChanged>( &R ); p )
			{
				if( p->PrevHP < p->AfterHP )
				{	AnyValid = true;	break;	}
			}
			else if( const auto *p=std::get_if<PoisonCured>( &R ); p )
			{	AnyValid = true;	break;	}
		}
		if( !AnyValid )return false;
	}

	//�L�����N�^�I�����j���[�̕\���X�V
	m_upTopLVMenu->UpdateCharSelMenuContent();

	//���ʕ\���G�t�F�N�g����

	return true;
}

//
void CampMenu::OnMagicSelected( const GameContent::Magic &Magic )
{
	if( m_iCurrChar<0 )return;

	//�����ł͖�����ΏۂƂ��閂�@�����g���Ȃ�
	if( !IsForFriend( Magic.Range() ) )return;

	{	//MP�c�ʃ`�F�b�N
		auto &User = m_rPlayData.Char( m_rPlayData.CurrParty()[m_iCurrChar] );
		const auto Spell = Magic.Spell();
		if( User.MP( Spell.first )<=0  ||  User.MP( Spell.second )<=0 )return;
	}

	PushTgtCharSelector(
		IsForAll( Magic.Range() ),
		[this, &Magic]( bool Selected, int iTgt )->Flags<GUI::GUIResult>
		{
			if( !Selected )return GUI::GUIResult::Finished;

			if( Affect( Magic.Efficacy(), Magic.Range(), iTgt ) )
			{
				//MP����
				const auto Spell = Magic.Spell();
				m_rPlayData.Char( m_rPlayData.CurrParty()[m_iCurrChar] ).DecMP( Spell.first, Spell.second );

				//MagicPage�\���X�V
				m_upMagicPage->UpdateOnMagicUsed( m_iCurrChar );
			}

			//���������@��A�����Ďg����悤�ɁC�ΏۑI�������𑱍s
			return GUI::GUIResult::ReqRedraw;
		}
	);
}
