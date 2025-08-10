#include "framework.h"
#include "MagicPage.h"
#include "IController.h"
#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"
#include "GameContent/Magic.h"
#include "GameContent/ICharacter.h"
#include "PlayData/PlayData.h"

using namespace GUI;

CampMenu::MagicPage::MagicPage( CampMenu &Outer )
	: m_Outer( Outer )
	, m_UI( Vec2i{ 200, 24 } )
{
	{
		constexpr int LineH = 24;
		constexpr int Margin = 4;
		m_DescView.AlignLeft();
		m_DescView.LineHeight( LineH );
		m_DescView.LineWidth( CampMenu::MainAreaRect.Width() - (DescViewXMargin+Margin)*2 );
		m_DescView.TopLeft( { CampMenu::MainAreaRect.Left() + DescViewXMargin+Margin, CampMenu::MainAreaRect.Bottom() - LineH*2 - Margin } );
		m_DescView.Visible( false );
	}

	m_UI.Set_OnIndicatedMagicChanged_Proc( [this]()->auto{	return OnIndicatedMagicChanged();	} );
	m_UI.Set_OnMagicSelected_Proc( [this]()->auto{	return OnMagicSelected();	} );
	m_UI.CursorVisiblity( false );
	m_UI.TopLeft( CampMenu::MainAreaRect.TopLeft() + Vec2i{ 16, 32 } );
}

void CampMenu::MagicPage::OnGotFocus()
{
	const auto *pMagic = m_UI.CurrIndicatedMagic();
	if( pMagic != nullptr )
	{
		UpdateDescView( *pMagic );
		m_DescView.Visible( true );
	}
}

void CampMenu::MagicPage::OnPushed()
{
	m_UI.CursorVisiblity( true );
	OnGotFocus();
}

void CampMenu::MagicPage::OnPrePopped()
{
	m_UI.CursorVisiblity( false );
	m_DescView.Visible( false );
}

void CampMenu::MagicPage::Paint_( HDC hdc ) const
{
	m_UI.Paint( hdc );

	if( m_DescView.Visible() )
	{	DrawFilledFrame( hdc, Rect::Dilation( m_DescView.BoundingRect(), DescViewXMargin, 0 ), Color::White, RGB(0,0,0) );	}

	m_DescView.Paint( hdc );
}

Flags<GUI::GUIResult> CampMenu::MagicPage::Update( const IController &Controller )
{
	return m_UI.Update( Controller );
}

void CampMenu::MagicPage::OnSelectedCharChanged( int iCharOrder )
{
	const auto &PD = m_Outer.m_rPlayData;
	m_UI.UpdateContent( PD.Char( PD.CurrParty()[ iCharOrder ] ), true );
}

//魔法が使用された際の更新
void CampMenu::MagicPage::UpdateOnMagicUsed( int iCurrCharOrder )
{
	const auto &PD = m_Outer.m_rPlayData;
	m_UI.UpdateContent( PD.Char( PD.CurrParty()[ iCurrCharOrder ] ), false );
}

Flags<GUI::GUIResult> CampMenu::MagicPage::OnIndicatedMagicChanged()
{
	const auto *pMagic = m_UI.CurrIndicatedMagic();
	if( pMagic == nullptr )return GUIResult::ReqRedraw;

	UpdateDescView( *pMagic );
	return GUIResult::ReqRedraw;
}

void CampMenu::MagicPage::UpdateDescView( const GameContent::Magic &TgtMagic )
{	m_DescView.TextLines( { TgtMagic.DescText(), TgtMagic.FlavorText() } );	}

//使用魔法選択時
Flags<GUI::GUIResult> CampMenu::MagicPage::OnMagicSelected()
{
	const auto *pMagic = m_UI.CurrIndicatedMagic();
	if( pMagic == nullptr )return GUIResult::None;

	m_Outer.OnMagicSelected( *pMagic );
	return GUIResult::ReqRedraw;
}

