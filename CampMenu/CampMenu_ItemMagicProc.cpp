#include "framework.h"
#include "CampMenu.h"
#include "PlayData/PlayData.h"
#include "GameContent/Magic.h"
#include "GameContent/PartyChar.h"

#include "TopLVMenu.h"
#include "MagicPage.h"

#include "ActProc/ActEfficacyImpl.h"
#include "ActProc/ActContext.h"
#include "Common/EffectImpls.h"
#include "ResManage/SoundBank.h"

using namespace GameContent;

namespace
{
	//パーティメニューでのアイテムや魔法の使用処理用
	class UseCtxt : public ActContext
	{
	private:
		PlayData &m_rPD;
	public:
		UseCtxt( PlayData &PD ) : m_rPD(PD) {} 

		virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const override
		{	return m_rPD.Char( m_rPD.CurrParty()[ Tgt.m_Order ] );	}	//Tgtは必ず味方キャラクタを指すという前提での実装
	};
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

	{//効果エフェクト
		bool AnyValid = false;
		for( const auto &R : Results )
		{
			if( const auto *p=std::get_if<HPChanged>( &R ); p )
			{
				if( AnyValid  ||  ( p->PrevHP < p->AfterHP ) )
				{
					AnyValid = true;
					const auto ViewRect = m_upTopLVMenu->CharViewRect( p->TgtChar.m_Order );
					m_EffectList.PushBack( CreateHPRecovEffect( p->Amount, (ViewRect.TopLeft()+ViewRect.RightBottom())/2, 4 ) );
				}
			}
			else if( const auto *p=std::get_if<PoisonCured>( &R ); p )
			{
				AnyValid = true;
				m_EffectList.PushBack( CreatePoisonCuredEffect() );
			}
		}

		//有効な効果が１つも生じていない場合は「使用を棄却（無かったことにする）」
		if( !AnyValid )return false;

		//※現実装では，決め打ちで効果音を鳴らす
		ResManage::PlaySE( ResManage::SE::Cure );
	}

	//キャラクタ選択メニューの表示更新
	m_upTopLVMenu->UpdateCharSelMenuContent();
	return true;
}

//
void CampMenu::OnMagicSelected( const GameContent::Magic &Magic )
{
	if( m_iCurrChar<0 )return;

	//ここでは味方を対象とする魔法しか使えない
	if( !IsForFriend( Magic.Range() ) )return;

	{	//MP残量チェック
		auto &User = m_rPlayData.Char( m_rPlayData.CurrParty()[m_iCurrChar] );
		const auto Spell = Magic.Spell();
		if( User.MP( Spell.first )<=0  ||  User.MP( Spell.second )<=0 )return;
	}

	PushTgtCharSelector(
		IsForAll( Magic.Range() ),
		[this, &Magic]( bool Selected, int iTgt )->Flags<GUI::GUIResult>
		{
			if( !Selected )return GUI::GUIResult::Finished;

			if( !Affect( Magic.Efficacy(), Magic.Range(), iTgt ) )
			{	return GUI::GUIResult::ReqRedraw;	}

			//MP消費
			const auto Spell = Magic.Spell();
			auto &User = m_rPlayData.Char( m_rPlayData.CurrParty()[m_iCurrChar] );
			User.DecMP( Spell.first, Spell.second );

			//MagicPage表示更新
			m_upMagicPage->UpdateOnMagicUsed( m_iCurrChar );

			//※MP残量がある場合には同じ魔法を連続して使えるように対象選択処理を続行
			if( User.MP( Spell.first )<=0  ||  User.MP( Spell.second )<=0 )
			{	return GUI::GUIResult::Finished;	}
			else
			{	return GUI::GUIResult::ReqRedraw;	}
		}
	);
}
