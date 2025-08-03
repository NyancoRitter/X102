#include "framework.h"
#include "MagicSelUI.h"
#include "GameContent/PartyChar.h"
#include "GUI/Color.h"
#include "IController.h"
#include "ResManage/BmpBank.h"

using namespace GUI;
using namespace GUI::Menu;

namespace
{
	using namespace GameContent;

	/// <summary>
	/// キャラクタの MP 状態に応じて Spells と TgtCont の内容を更新する作業
	/// </summary>
	/// <typeparam name="Spell_t">FirstSpell or SecondSpell</typeparam>
	/// <typeparam name="N">Spell_t のメンバ個数</typeparam>
	/// <param name="Spells">更新対象．最大MPが1以上であるスペルだけが格納される</param>
	/// <param name="TgtCont">
	/// 更新対象．項目の順序は Spells に対応．
	/// ただし１つもスペルが無い場合にはその旨を表示するための項目を１つ持つ形となる．
	/// </param>
	/// <param name="MagicUser">キャラクタ</param>
	template< class Spell_t, int N >
	void UpdateCont( std::vector<Spell_t> &Spells, GenericMenuContent<true> &TgtCont, const PartyChar &MagicUser )
	{
		const int PrevCursorPos = TgtCont.CursorPos();

		Spells.clear();
		TgtCont.Clear();
		for( int i=0; i<N; ++i )
		{
			auto spell = (Spell_t)i;

			const int MaxMP = MagicUser.MaxMP( spell );
			if( MaxMP <= 0 )continue;

			Spells.push_back( spell );

			const int MP = MagicUser.MP( spell );
			TgtCont.Add(
				{
					Icon( ResManage::BMP(spell), 16+6 ),
					Text( NameStr(spell) ),
					Text( std::to_wstring(MP) + L"/" + std::to_wstring(MaxMP) ).AlignRight()
				} 
			).DrawColor( MP>=0  ?  Color::White  :  Color::GrayOut );
		}
		TgtCont.CursorPos( PrevCursorPos );

		if( TgtCont.nItems()==0 )
		{
			TgtCont.Add( { Text( L"(None)" ).AlignCenter() } ).DrawColor( Color::GrayOut );
		}
	}
}

//---

MagicSelUI::MagicSelUI( const Vec2i &ItemDrawSize )
	: m_Content{	GenericMenuContent<true>( ItemDrawSize ), GenericMenuContent<true>( ItemDrawSize )	}
{
	for( int i=0; i<2; ++i )
	{
		m_Menu[i].SetContent( &m_Content[i] )
			.OuterMargin( {12,12 } )
			.WithFrame( true );
	}

	TopLeft( {0,0} );
	SwitchFocusTo( 0 );
}

void MagicSelUI::UpdateContent( const GameContent::PartyChar &MagicUser )
{
	using namespace GameContent;

	UpdateCont< FirstSpell, N_FirstSpell >( m_Spell1st, m_Content[0], MagicUser );
	UpdateCont< SecondSpell, N_SecondSpell >( m_Spell2nd, m_Content[1], MagicUser );
}

void MagicSelUI::SwitchFocusTo( int iMenu )
{
	m_iFocusedMenu = iMenu;
	for( int i=0; i<2; ++i )
	{
		m_Menu[i].IsFocused( i==iMenu );
	}
}

MagicSelUI &MagicSelUI::TopLeft( const Vec2i &TL )
{
	m_Menu[0].TopLeft( TL );
	m_Menu[1].TopLeft( TL + Vec2i{ m_Menu[0].Size()[0] - 8, 0 } );	//2つのメニューを少しだけ重ねる
	return *this;
}

void MagicSelUI::OnGotFocus()
{
	for( auto &Cont : m_Content )Cont.CursorPos( 0 );
	SwitchFocusTo( 0 );
}

void MagicSelUI::OnLostFocus()
{
	for( auto &Cont : m_Content )Cont.CursorPos( -1 );
}

Vec2i MagicSelUI::Size() const
{
	const Rect BR[2] = {	m_Menu[0].BoundingRect(), m_Menu[1].BoundingRect()	};
	return Vec2i{
		BR[1].Right() - BR[0].Left(),
		std::max( BR[0].Height(), BR[1].Height() )
	};
}

Flags<GUIResult> MagicSelUI::Update( const IController &Controller )
{
	HandleInputResult HIR = HandleInputResult::None;

	if( m_iFocusedMenu == 0 )
	{
		if( Controller.CursorRight() )
		{
			SwitchFocusTo( 1 );
			return GUIResult::ReqRedraw;
		}

		HIR = m_Menu[0].HandleInput( Controller );
	}
	else
	{
		if( Controller.CursorLeft() )
		{
			SwitchFocusTo( 0 );
			return GUIResult::ReqRedraw;
		}

		HIR = m_Menu[1].HandleInput( Controller );
	}

	switch( HIR )
	{
	case HandleInputResult::CursorMoved:
		{
			if( !m_OnIndicatedMagicChanged || m_Spell1st.empty() || m_Spell2nd.empty() )
			{	return GUIResult::ReqRedraw;	}

			return m_OnIndicatedMagicChanged( m_Spell1st[ m_Content[0].CursorPos() ], m_Spell2nd[ m_Content[1].CursorPos() ] );
		}
		break;
	case HandleInputResult::Selected:
		{
			if( !m_OnMagicSelected || m_Spell1st.empty() || m_Spell2nd.empty() )
			{	return GUIResult::None;	}

			return m_OnMagicSelected( m_Spell1st[ m_Content[0].CursorPos() ], m_Spell2nd[ m_Content[1].CursorPos() ] );
		}
		break;
	case HandleInputResult::Canceled:
		return GUIResult::Finished;	break;
	}

	return GUIResult::None;
}


void MagicSelUI::Paint_( HDC hdc ) const
{
	if( m_iFocusedMenu == 0 )
	{
		m_Menu[1].Paint( hdc );
		m_Menu[0].Paint( hdc );
	}
	else
	{
		m_Menu[0].Paint( hdc );
		m_Menu[1].Paint( hdc );
	}
}