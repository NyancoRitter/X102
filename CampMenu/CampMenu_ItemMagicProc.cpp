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
	//パーティメニューでのアイテムや魔法の使用処理用 ActContext 実装
	class UseCtxt : public ActContext
	{
	private:
		PlayData &m_rPD;
	public:
		UseCtxt( PlayData &PD ) : m_rPD(PD) {} 

		virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const override
		{	return m_rPD.Char( m_rPD.CurrParty()[ Tgt.m_Order ] );	}	//Tgtは必ず味方キャラクタを指すという前提での実装
	};

	/// <summary>
	/// キャラクタ所持品←→Stock アイテム移動処理用
	/// From内の iOrder で指定された要素を To の末尾に移動させる，
	/// </summary>
	/// <param name="From"></param>
	/// <param name="To"></param>
	/// <param name="iOrder">From内での対象要素index</param>
	/// <returns>成否</returns>
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
	//効果処理
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

	{//有効な効果が１つも生じていない場合は「使用を棄却（無かったことにする）」
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

	{//効果エフェクト
		//※現実装では，決め打ちで回復時用効果音を鳴らす
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

	//キャラクタ選択メニューの表示更新
	m_upTopLVMenu->UpdateCharSelMenuContent();
	return true;
}

//
void CampMenu::OnMagicSelected( const GameContent::Magic &Magic )
{
	const auto *pUser = CurrSelChar();
	if( !pUser )return;

	//使用者がHP的に行動不能でないかをチェック
	if( pUser->HP()<=0 )return;

	//ここでは味方を対象とする魔法しか使えない
	if( !IsForFriend( Magic.Range() ) )return;

	{//MP残量チェック
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

			//MP消費
			auto &User = *CurrSelChar();
			const auto Spell = Magic.Spell();
			User.DecMP( Spell.first, Spell.second );

			//MagicPage表示更新
			m_upMagicPage->UpdateOnMagicUsed();

			//※MP残量がある場合には同じ魔法を連続して使えるように対象選択処理を続行
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

	//ここでは味方を対象とするアイテムしか使えない
	if( !IsForFriend( ItemDef.Range() ) )return;

	PushTgtCharSelector(
		IsForAll( ItemDef.Range() ),
		[this, &ItemDef, iOrder]( bool Selected, int iTgt )->Flags<GUI::GUIResult>
		{
			if( !Selected )return GUI::GUIResult::Finished;

			if( !Affect( ItemDef.Efficacy(), ItemDef.Range(), iTgt ) )
			{	return GUI::GUIResult::ReqRedraw;	}

			//アイテム消費
			if( ItemDef.CanUseOnlyOnce() )
			{
				CurrSelChar()->Items().RemoveAt( iOrder );
				m_upItemPage->UpdateOnItemUsed();	//ItemPage表示更新
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
