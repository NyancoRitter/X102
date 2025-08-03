#include "framework.h"
#include "CampMenu.h"

#include "IController.h"
#include "GlobalConst.h"
#include "GUI/DrawFuncs.h"
#include "GUI/MenuAsGUI.h"
#include "GUI/Color.h"

#include "PlayData/PlayData.h"

#include "TopLVMenu.h"
#include "StatusPage.h"
#include "ItemPage.h"
#include "MagicPage.h"

using namespace GUI;

const Rect CampMenu::MainAreaRect{ Vec2i{168, 24}, Vec2i{GlobalConst::GC_W-2, GlobalConst::GC_H-2} };

CampMenu::CampMenu( PlayData &rPlayData )
	: m_rPlayData( rPlayData )
{
	{//所持金表示
		m_MoneyView.TopLeft( Vec2i{ 4, GlobalConst::GC_H-m_MoneyView.Size()[1]-2 } );
		m_MoneyView.SetMoney( m_rPlayData.Money() );
	}

	{//ページ
		m_upStatusPage = std::make_unique<StatusPage>( *this );
		m_upItemPage = std::make_unique<ItemPage>( *this );
		m_upMagicPage = std::make_unique<MagicPage>( *this );

		const auto TL = MainAreaRect.TopLeft() + Vec2i{ 16, 32 };
		m_upStatusPage->TopLeft( TL );
		m_upItemPage->TopLeft( TL );
		m_upMagicPage->TopLeft( TL );
	}

	m_LocalStack.Push(
		std::make_unique<TopLVMenu>(
			*this,
			std::vector<std::wstring>{ L"Status", L"Item", L"Magic" }
		)
	);

	OnTopLVMenuCursorMoved( 0,0 );
}

CampMenu::~CampMenu() = default;

//
void CampMenu::Paint_( HDC hdc ) const
{
	FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );
	DrawFrame( hdc, MainAreaRect, GUI::Color::White );
	m_MoneyView.Paint( hdc );
	m_LocalStack.Paint( hdc );

	//LocalStackに積まれていない状況でも描画する
	m_upStatusPage->Paint( hdc );
	if( m_LocalStack.size() == 1 )
	{
		m_upItemPage->Paint( hdc );
		m_upMagicPage->Paint( hdc );
	}
}

//更新
Flags<GUIResult> CampMenu::Update( const IController &Controller )
{
	Flags<GUIResult> Ret;

	if( m_LocalStack.Update( Controller ) )
	{	Ret |= GUIResult::ReqRedraw;	}
	
	if( m_LocalStack.empty() )
	{	Ret |= GUIResult::Finished;	}

	return Ret;
}

void CampMenu::OnTopLVMenuCursorMoved( int CharOrder, int CmdOrder )
{
	const auto Party = m_rPlayData.CurrParty();
	if( CmdOrder<0 || CharOrder<0 || Party.empty() )
	{
		m_upStatusPage->Visible( false );
		m_upItemPage->Visible( false );
		m_upMagicPage->Visible( false );
		return;
	}

	if( m_iCurrChar != CharOrder )
	{
		m_iCurrChar = CharOrder;
		m_upStatusPage->SetDispTgt( &m_rPlayData.Char( Party[CharOrder] ) );
	}

	m_upStatusPage->Visible( CmdOrder==0 );
	m_upItemPage->Visible( CmdOrder==1 );
	m_upMagicPage->Visible( CmdOrder==2 );
}

void CampMenu::OnTopLVMenuSelected( int CharOrder, int CmdOrder )
{
	switch( CmdOrder )
	{
	case 1:
		m_LocalStack.Push( std::make_unique<GUI::RefWrapper>( *m_upItemPage ) );
		break;
	case 2:
		m_LocalStack.Push( std::make_unique<GUI::RefWrapper>( *m_upMagicPage ) );
		break;
	default:
		break;
	}
}

