#include "framework.h"
#include "ItemPage.h"
#include "IController.h"
#include "PlayData/PlayData.h"
#include "GameContent/PartyChar.h"
#include "GlobalConst.h"

using namespace GUI;
using namespace GUI::Menu;

CampMenu::ItemPage::ItemPage( CampMenu &Outer )
	: m_Outer( Outer )
	, m_Content{ GenericMenuContent<true>( Vec2i{ 180, 24 } ), GenericMenuContent<true>( Vec2i{ 180, 24 } ) }
{
	for( int i=0; i<2; ++i )
	{
		m_Menu[i].SetContent( &m_Content[i] )
			.OuterMargin( Vec2i{ 12, 12 } )
			.WithFrame( true );
	}

	m_Menu[0].TopLeft( CampMenu::MainAreaRect.TopLeft() + Vec2i{ 16, 32+24 } );
	m_Menu[1].TopLeft( Vec2i{ CampMenu::MainAreaRect.Right() - m_Menu[1].Size()[0] - 16 , m_Menu[0].TopLeft()[1] } );
}

void CampMenu::ItemPage::OnSelectedCharChanged( int iCharOrder )
{
	const auto &PD = m_Outer.m_rPlayData;
	const auto &Char = PD.Char( PD.CurrParty()[ iCharOrder ] );


}


void CampMenu::ItemPage::OnGotFocus()
{
	m_FocusState = true;
}

void CampMenu::ItemPage::OnLostFocus()
{
	m_FocusState = false;
}

void CampMenu::ItemPage::OnPrePopped()
{
	m_FocusState = false;
}

void CampMenu::ItemPage::Paint_( HDC hdc ) const
{
	{
		SetTextColor( hdc, RGB(255,255,255) );
		SetBkMode( hdc, TRANSPARENT );
		RECT rect;
		rect.left = m_Menu[0].TopLeft()[0] + 4;
		rect.right = m_Menu[1].BoundingRect().Right() - 4;
		rect.top = m_Menu[0].TopLeft()[1] - 24;
		rect.bottom = GlobalConst::GC_H;
		DrawTextW( hdc, L"[Equipment]", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_LEFT|DT_TOP );
		DrawTextW( hdc, L"©(S)¨", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_CENTER|DT_TOP );
		DrawTextW( hdc, L"[Stock]", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_RIGHT|DT_TOP );

		if( m_FocusState )
		{
			rect.top = m_Menu[0].BoundingRect().Bottom() + 8;
			DrawTextW( hdc, L"¦(B):––”ö‚ÉˆÚ“®", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_LEFT|DT_TOP );
		}
	}

	for( int i=0; i<2; ++i )
	{
		m_Menu[i].Paint( hdc );
	}
}

Flags<GUI::GUIResult> CampMenu::ItemPage::Update( const IController &Controller )
{
	if( Controller.Cancel() )return GUIResult::Finished;
	return GUIResult::None;
}
