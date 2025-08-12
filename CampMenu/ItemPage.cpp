#include "framework.h"
#include <algorithm>
#include "ItemPage.h"
#include "IController.h"
#include "PlayData/PlayData.h"
#include "GameContent/PartyChar.h"
#include "GlobalConst.h"
#include "GameContent/Item.h"
#include "GUI/Color.h"
#include "GUI/DrawFuncs.h"

using namespace GUI;
using namespace GUI::Menu;
using namespace GameContent;

CampMenu::ItemPage::ItemPage( CampMenu &Outer )
	: m_Outer( Outer )
	, m_Content{ GenericMenuContent<true>( Vec2i{ 180, 24 } ), GenericMenuContent<true>( Vec2i{ 180, 24 } ) }
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

	for( int i=0; i<2; ++i )
	{
		m_Menu[i].SetContent( &m_Content[i] )
			.OuterMargin( Vec2i{ 12, 12 } )
			.WithFrame( true );
	}

	m_Menu[0].TopLeft( CampMenu::MainAreaRect.TopLeft() + Vec2i{ 16, 32+24 } );
	m_Menu[1].TopLeft( Vec2i{ CampMenu::MainAreaRect.Right() - m_Menu[1].Size()[0] - 16 , m_Menu[0].TopLeft()[1] } );

	for( auto &M : m_Menu ){	M.CursorVisible( false );	}
	UpdateStockContent();
}

void CampMenu::ItemPage::OnSelectedCharChanged()
{
	UpdateCharItemContent();
	m_Content[0].CursorPos( 0 );
	SwitchFocusTo( m_Content[0].nItems()>0  ?  0  :  1 );
}

//キャラクタ保持物側メニュー内容更新
void CampMenu::ItemPage::UpdateCharItemContent()
{
	const auto *pChar = m_Outer.CurrSelChar();

	auto &Cont = m_Content[0];
	int PrevCursorPos = Cont.CursorPos();
	Cont.Clear();

	if( !pChar )return;

	auto IDs = pChar->Items().DataClone();
	Cont.Reserve( IDs.size() );
	for( auto ID : IDs )
	{
		const auto &Def = Item::Definition( ID );
		Cont.Add( { Icon( Def.Img(), 16+2 ).AlignLeft(), Text( Def.Name() ) } );
	}
	Cont.CursorPos( std::max( 0, std::min( PrevCursorPos, (int)IDs.size()-1 ) ) );
	m_Menu[0].ItemAreaSpace( (int)pChar->Items().MaxSize() );
}

//アイテムが使用された際の更新処理
void CampMenu::ItemPage::UpdateOnItemUsed()
{
	UpdateCharItemContent();
	UpdateDescView();
}

//Stock側メニュー内容更新
void CampMenu::ItemPage::UpdateStockContent()
{
	auto IDs = m_Outer.m_rPlayData.ItemStock().DataClone();

	auto &Cont = m_Content[1];
	int PrevCursorPos = Cont.CursorPos();
	Cont.Clear();
	if( IDs.empty() )
	{
		Cont.Add( { Text( L"(NO ITEM)" ).AlignCenter() } ).DrawColor( Color::GrayOut );
		Cont.CursorPos( 0 );
	}
	else
	{
		Cont.Reserve( IDs.size() );
		for( auto ID : IDs )
		{
			const auto &Def = Item::Definition( ID );
			Cont.Add( { Icon( Def.Img(), 16+2 ).AlignLeft(), Text( Def.Name() ) } );
		}
	}
	Cont.CursorPos( std::max( 0, std::min( PrevCursorPos, Cont.nItems()-1 ) ) );
	m_Menu[1].ItemAreaSpace( std::min( Cont.nItems(), 12 ) );
}

//
void CampMenu::ItemPage::SwitchFocusTo( int iMenu )
{
	m_iFocusedMenu = iMenu;
	for( int i=0; i<2; ++i )
	{	m_Menu[i].IsFocused( i==iMenu );	}

	UpdateDescView();
}

void CampMenu::ItemPage::OnPushed()
{
	for( auto &M : m_Menu )M.CursorVisible( true );
	m_ShowInfos = true;
	UpdateDescView();
}

void CampMenu::ItemPage::OnPrePopped()
{
	m_ShowInfos = false;
	m_DescView.Visible( false );
	for( auto &M : m_Menu )M.CursorVisible( false );
}

void CampMenu::ItemPage::UpdateDescView()
{
	int CursorPos = m_Content[ m_iFocusedMenu ].CursorPos();
	if( CursorPos<0 ){	m_DescView.Visible( false );	return;	}

	const DataVec<ItemID> *pItems = nullptr;

	if( m_iFocusedMenu==0 )
	{
		if( const auto *pCurrChar = m_Outer.CurrSelChar();	pCurrChar )
		{	pItems = &pCurrChar->Items();	}
	}
	else
	{	pItems = &m_Outer.m_rPlayData.ItemStock();	}

	if( !pItems  ||  CursorPos>=(int)pItems->size() )
	{	m_DescView.Visible( false );	return;	}

	const auto &ItemDef = Item::Definition( (*pItems)[CursorPos] );
	m_DescView.TextLines( { ItemDef.DescText(), ItemDef.FlavorText() } );
	m_DescView.Visible( true );
}

//
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
		DrawTextW( hdc, L"←(S)→", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_CENTER|DT_TOP );
		DrawTextW( hdc, L"[Stock]", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_RIGHT|DT_TOP );

		if( m_ShowInfos )
		{
			rect.top = m_Menu[0].BoundingRect().Bottom() + 8;
			DrawTextW( hdc, L"※(B):末尾に移動", -1, &rect, DT_NOCLIP|DT_SINGLELINE|DT_LEFT|DT_TOP );
		}
	}

	for( int i=0; i<2; ++i )
	{	m_Menu[i].Paint( hdc );	}

	if( m_DescView.Visible() )
	{	DrawFilledFrame( hdc, Rect::Dilation( m_DescView.BoundingRect(), DescViewXMargin, 0 ), Color::White, RGB(0,0,0) );	}

	m_DescView.Paint( hdc );
}

//更新
Flags<GUI::GUIResult> CampMenu::ItemPage::Update( const IController &Controller )
{
	//メニュー間カーソル移動
	if( m_iFocusedMenu==0 && Controller.CursorRight() )
	{	SwitchFocusTo( 1 );	return GUIResult::ReqRedraw;	}

	if( m_iFocusedMenu==1 && Controller.CursorLeft() )
	{
		const auto *pChar = m_Outer.CurrSelChar();
		if( pChar!=nullptr  &&  !pChar->Items().empty() )
		{	SwitchFocusTo( 0 );	return GUIResult::ReqRedraw;	}
	}

	//アイテム整理
	if( Controller.MoveItemToBack() )
	{
		if( m_iFocusedMenu==0 )
		{
			if( m_Outer.MoveBackCharItem( m_Content[0].CursorPos() ) )
			{
				UpdateCharItemContent();
				UpdateDescView();
				return GUIResult::ReqRedraw;
			}
		}
		else
		{
			if( m_Outer.MoveBackStockItem( m_Content[1].CursorPos() ) )
			{
				UpdateStockContent();
				UpdateDescView();
				return GUIResult::ReqRedraw;
			}
		}
	}

	//メニュー間のアイテム移動
	if( Controller.MoveItem() )
	{
		bool Done = m_iFocusedMenu==0 ?
			m_Outer.MoveItemFromCharToStock( m_Content[0].CursorPos() ) :
			m_Outer.MoveItemFromStockToChar( m_Content[1].CursorPos() );

		if( Done )
		{
			UpdateCharItemContent();
			UpdateStockContent();

			if( m_Outer.CurrSelChar()->Items().empty() )
			{	SwitchFocusTo( 1 );	}

			UpdateDescView();
			return GUIResult::ReqRedraw;
		}
	}

	//メニュー操作
	switch( m_Menu[ m_iFocusedMenu ].HandleInput( Controller ) )
	{
	case HandleInputResult::CursorMoved:
		UpdateDescView();
		return GUIResult::ReqRedraw;
		break;

	case HandleInputResult::Canceled:
		return GUIResult::Finished;
		break;

	case HandleInputResult::Selected:
		if( m_iFocusedMenu==0 )	//アイテムはStockからは使えない
		{
			m_Outer.OnItemSelected( m_Content[0].CursorPos() );
			return GUIResult::ReqRedraw;
		}
		break;

	default:
		break;
	}

	return GUIResult::None;
}
