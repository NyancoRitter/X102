#include "framework.h"
#include "CampMenu.h"
#include "PlayData/PlayData.h"
#include "GameContent/Magic.h"
#include "GameContent/Item.h"
#include "GameContent/PartyChar.h"

#include "TopLVMenu.h"
#include "MagicPage.h"
#include "ItemPage.h"

#include "ActProc/ActEfficacyImpl.h"
#include "ActProc/ActContext.h"
#include "Common/EffectImpls.h"
#include "ResManage/SoundBank.h"

using namespace GameContent;

namespace
{
	//�p�[�e�B���j���[�ł̃A�C�e���▂�@�̎g�p�����p ActContext ����
	class UseCtxt : public ActContext
	{
	private:
		PlayData &m_rPD;
	public:
		UseCtxt( PlayData &PD ) : m_rPD(PD) {} 

		virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const override
		{	return m_rPD.Char( m_rPD.CurrParty()[ Tgt.m_Order ] );	}	//Tgt�͕K�������L�����N�^���w���Ƃ����O��ł̎���
	};

	/// <summary>
	/// �L�����N�^�����i����Stock �A�C�e���ړ������p
	/// From���� iOrder �Ŏw�肳�ꂽ�v�f�� To �̖����Ɉړ�������C
	/// </summary>
	/// <param name="From"></param>
	/// <param name="To"></param>
	/// <param name="iOrder">From���ł̑Ώۗv�findex</param>
	/// <returns>����</returns>
	bool MoveItem( DataVec<ItemID> &From, DataVec<ItemID> &To, int iOrder )
	{
		if( iOrder<0 || iOrder>=(int)From.size() )return false;

		auto ID = From[iOrder];
		if( !To.PushBack( ID ) )return false;
		From.RemoveAt( iOrder );
		return true;
	}
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

	{//�L���Ȍ��ʂ��P�������Ă��Ȃ��ꍇ�́u�g�p�����p�i�����������Ƃɂ���j�v
		bool AnyValid = false;
		for( const auto &R : Results )
		{
			if( const auto *p=std::get_if<HPChanged>( &R ); p )
			{
				if( ( p->PrevHP < p->AfterHP ) )
				{	AnyValid = true;	break;	}
			}
			else if( const auto *p=std::get_if<PoisonCured>( &R ); p )
			{	AnyValid = true;	break;	}
		}
		if( !AnyValid )return false;
	}

	{//���ʃG�t�F�N�g
		//���������ł́C���ߑł��ŉ񕜎��p���ʉ���炷
		ResManage::PlaySE( ResManage::SE::Cure );

		for( const auto &R : Results )
		{
			if( const auto *p=std::get_if<HPChanged>( &R ); p )
			{
				const auto ViewRect = m_upTopLVMenu->CharViewRect( p->TgtChar.m_Order );
				m_EffectList.PushBack( CreateHPRecovEffect( p->Amount, (ViewRect.TopLeft()+ViewRect.RightBottom())/2, 4 ) );
			}
		}
	}

	//�L�����N�^�I�����j���[�̕\���X�V
	m_upTopLVMenu->UpdateCharSelMenuContent();
	return true;
}

//
void CampMenu::OnMagicSelected( const GameContent::Magic &Magic )
{
	const auto *pUser = CurrSelChar();
	if( !pUser )return;

	//�g�p�҂�HP�I�ɍs���s�\�łȂ������`�F�b�N
	if( pUser->HP()<=0 )return;

	//�����ł͖�����ΏۂƂ��閂�@�����g���Ȃ�
	if( !IsForFriend( Magic.Range() ) )return;

	{//MP�c�ʃ`�F�b�N
		const auto Spell = Magic.Spell();
		if( pUser->MP( Spell.first )<=0  ||  pUser->MP( Spell.second )<=0 )return;
	}

	PushTgtCharSelector(
		IsForAll( Magic.Range() ),
		[this, &Magic]( bool Selected, int iTgt )->Flags<GUI::GUIResult>
		{
			if( !Selected )return GUI::GUIResult::Finished;

			if( !Affect( Magic.Efficacy(), Magic.Range(), iTgt ) )
			{	return GUI::GUIResult::ReqRedraw;	}

			//MP����
			auto &User = *CurrSelChar();
			const auto Spell = Magic.Spell();
			User.DecMP( Spell.first, Spell.second );

			//MagicPage�\���X�V
			m_upMagicPage->UpdateOnMagicUsed();

			//��MP�c�ʂ�����ꍇ�ɂ͓������@��A�����Ďg����悤�ɑΏۑI�������𑱍s
			if( User.MP( Spell.first )<=0  ||  User.MP( Spell.second )<=0 )
			{	return GUI::GUIResult::Finished;	}
			else
			{	return GUI::GUIResult::ReqRedraw;	}
		}
	);
}

//
void CampMenu::OnItemSelected( int iOrder )
{
	if( iOrder<0 )return;

	const auto *pUser = CurrSelChar();
	if( !pUser )return;

	const auto &CharItems = pUser->Items();
	if( iOrder >= (int)CharItems.size() )return;
	const auto &ItemDef = Item::Definition( CharItems[ iOrder ] );

	//�����ł͖�����ΏۂƂ���A�C�e�������g���Ȃ�
	if( !IsForFriend( ItemDef.Range() ) )return;

	PushTgtCharSelector(
		IsForAll( ItemDef.Range() ),
		[this, &ItemDef, iOrder]( bool Selected, int iTgt )->Flags<GUI::GUIResult>
		{
			if( !Selected )return GUI::GUIResult::Finished;

			if( !Affect( ItemDef.Efficacy(), ItemDef.Range(), iTgt ) )
			{	return GUI::GUIResult::ReqRedraw;	}

			//�A�C�e������
			if( ItemDef.CanUseOnlyOnce() )
			{
				CurrSelChar()->Items().RemoveAt( iOrder );
				m_upItemPage->UpdateOnItemUsed();	//ItemPage�\���X�V
			}

			return GUI::GUIResult::Finished;
		}
	);
}

bool CampMenu::MoveBackCharItem( int iOrder )
{
	if( auto *pChar = CurrSelChar();	pChar )
	{	return pChar->Items().MoveItemToBack( iOrder );	}
	else{	return false;	}
}

bool CampMenu::MoveBackStockItem( int iOrder )
{	return m_rPlayData.ItemStock().MoveItemToBack( iOrder );	}

bool CampMenu::MoveItemFromCharToStock( int iOrder )
{
	if( auto *pChar=CurrSelChar();	pChar )
	{	return MoveItem( pChar->Items(), m_rPlayData.ItemStock(), iOrder );	}
	else{	return false;	}
}

bool CampMenu::MoveItemFromStockToChar( int iOrder )
{
	if( auto *pChar=CurrSelChar();	pChar )
	{	return MoveItem( m_rPlayData.ItemStock(), pChar->Items(), iOrder );	}
	else{	return false;	}
}
