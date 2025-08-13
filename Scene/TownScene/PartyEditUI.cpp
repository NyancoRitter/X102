#include "framework.h"
#include "PartyEditUI.h"
#include "PlayData/PlayData.h"
#include "GameContent/PartyChar.h"
#include "Common/CharDrawColor.h"
#include "GUI/Color.h"
#include "GUI/DrawFuncs.h"
#include "IController.h"

namespace Town
{
	using namespace GameContent;
	using namespace GUI;
	using namespace GUI::Menu;

	PartyEditUI::PartyEditUI( PlayData &rPlayData )
		: m_rPlayData( rPlayData )
		, m_Content{	CharSelMenuContent<true>( Vec2i() ), CharSelMenuContent<true>( Vec2i() ) }
	{
		UpdateContent();

		m_Menu[0].SetContent( &m_Content[0] )
			.ItemSpacing(4);

		m_Menu[1].SetContent( &m_Content[1] )
			.ItemSpacing(4)
			.ItemAreaSpace( std::min( ContentMaxSize, m_Content[1].nItems() ) );

		TopLeft( Vec2i() );
		SwitchFocusTo( m_Content[0].nItems()>0  ?  0  :  1 );
	}

	void PartyEditUI::UpdateContent()
	{
		const auto CurrParty = m_rPlayData.CurrParty();
		
		{
			auto &Cont = m_Content[0];
			int PrevCursorPos = Cont.CursorPos();
			Cont.Clear();
			for( auto ID : CurrParty )
			{
				const auto &Char = m_rPlayData.Char( ID );
				Cont.Add( ID )
					.HPInfo( Char.HP(), Char.MaxHP() )
					.DrawColor( CharDrawColor( Char.HP(), Char.PoisonInfected() ) );
			}
			Cont.CursorPos( std::max( 0, std::min( PrevCursorPos, Cont.nItems()-1 ) ) );
		}
		{
			auto &Cont = m_Content[1];
			int PrevCursorPos = Cont.CursorPos();
			Cont.Clear();
			m_StandbyCharIDs.clear();
			for( int i=0; i<N_PARTY_CHAR_DEF;	++i )
			{
				auto ID = (PartyCharID)i;
				if( CurrParty.Contains( ID ) )continue;

				m_StandbyCharIDs.push_back( ID );

				const auto &Char = m_rPlayData.Char( ID );
				Cont.Add( ID )
					.HPInfo( Char.HP(), Char.MaxHP() )
					.DrawColor( CharDrawColor( Char.HP(), Char.PoisonInfected() ) );
			}
			Cont.CursorPos( std::max( 0, std::min( PrevCursorPos, Cont.nItems()-1 ) ) );
		}
		m_Menu[1].ItemAreaSpace( std::min( ContentMaxSize, m_Content[1].nItems() ) );
		m_Menu[1].UpdateScrollState();
	}

	PartyEditUI &PartyEditUI::TopLeft( const Vec2i &TL )
	{
		m_TopLeft = TL;

		m_Menu[0].TopLeft( TL + Vec2i{ XPadding, YPadding + LabelH } );
		m_Menu[1].TopLeft( m_Menu[0].TopLeft() + Vec2i{ m_Menu[0].Size()[0]+XPadding*2, 0 } );

		return *this;
	}

	Vec2i PartyEditUI::Size() const
	{
		return Vec2i
		{
			m_Menu[0].Size()[0] + m_Menu[1].Size()[0] + XPadding*4,
			YPadding*3 + LabelH + m_Content[1].ItemDrawSize()[1]*ContentMaxSize + m_Menu[1].ItemSpacing()*(ContentMaxSize-1)
		};
	}

	void PartyEditUI::SwitchFocusTo( int iMenu )
	{
		m_iFocusedMenu = iMenu;
		for( int i=0; i<2; ++i )
		{
			m_Menu[i].IsFocused( i==iMenu );
			m_Menu[i].CursorVisible( i==iMenu );
		}
	}

	void PartyEditUI::OnGotFocus()
	{
		//※編成中にパーティメニューを開いて何かデータ変化を起こす操作をしたかもしれないので
		UpdateContent();
	}

	//
	void PartyEditUI::Paint_( HDC hdc ) const
	{
		{//外枠
			const auto size = Size();
			Rect FrameRect( m_TopLeft, size[0], size[1] );
			DrawFilledFrame( hdc, FrameRect, Color::White, RGB(0,0,0) );
		}
		//メニュー
		m_Menu[0].Paint( hdc );
		m_Menu[1].Paint( hdc );

		{//メニューの上の文字
			SetTextColor( hdc, RGB(255,255,255) );
			SetBkMode( hdc, TRANSPARENT );

			RECT rect;
			rect.top = m_TopLeft[1] + YPadding;
			rect.bottom = rect.top + LabelH;
			rect.left = m_Menu[0].TopLeft()[0];
			rect.right = rect.left + m_Menu[0].Size()[0];
			DrawTextW( hdc, L"Party", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_CENTER|DT_TOP );

			rect.left = m_Menu[1].TopLeft()[0];
			rect.right = rect.left + m_Menu[1].Size()[0];
			DrawTextW( hdc, L"Standby", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_CENTER|DT_TOP );
		}
	}

	//
	Flags<GUI::GUIResult> PartyEditUI::Update( const IController &Controller )
	{
		//メニュー間カーソル移動
		if( m_iFocusedMenu==0 && Controller.CursorRight() && m_Content[1].nItems()>0 )
		{	SwitchFocusTo( 1 );	return GUIResult::ReqRedraw;	}

		if( m_iFocusedMenu==1 && Controller.CursorLeft() && m_Content[0].nItems()>0 )
		{	SwitchFocusTo( 0 );	return GUIResult::ReqRedraw;	}

		switch( m_Menu[m_iFocusedMenu].HandleInput( Controller ) )
		{
		case HandleInputResult::CursorMoved:
			return GUIResult::ReqRedraw;	break;
		case HandleInputResult::Canceled:
			return GUIResult::Finished;	break;
		case HandleInputResult::Selected:
			if( m_iFocusedMenu==0 )
			{
				if( MoveFromPartyToStandby( m_Content[0].CursorPos() ) )
				{	return GUIResult::ReqRedraw;	}
			}
			else
			{
				if( MoveFromStandbyToParty( m_Content[1].CursorPos() ) )
				{	return GUIResult::ReqRedraw;	}
			}
			break;
		default:
			break;
		}

		return GUIResult::None;
	}

	//
	bool PartyEditUI::MoveFromPartyToStandby( int iOrder )
	{
		if( iOrder<0 )return false;

		auto Party = m_rPlayData.CurrParty();
		Party.RemoveAt( iOrder );
		m_rPlayData.SetupParty( Party.DataClone() );

		UpdateContent();
		if( Party.empty() )
		{	SwitchFocusTo( 1 );	}

		return true;
	}

	//
	bool PartyEditUI::MoveFromStandbyToParty( int iOrder )
	{
		if( iOrder<0 )return false;

		auto Party = m_rPlayData.CurrParty();
		if( Party.full() )return false;

		Party.PushBack( m_StandbyCharIDs[iOrder] );
		m_rPlayData.SetupParty( Party.DataClone() );

		UpdateContent();
		if( m_StandbyCharIDs.empty() )
		{	SwitchFocusTo( 0 );	}

		return true;
	}
}
