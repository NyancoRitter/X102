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
		m_Pages[0] = m_upStatusPage.get();

		m_upItemPage = std::make_unique<ItemPage>( *this );
		m_Pages[1] = m_upItemPage.get();

		m_upMagicPage = std::make_unique<MagicPage>( *this );
		m_Pages[2] = m_upMagicPage.get();

		const auto TL = MainAreaRect.TopLeft() + Vec2i{ 16, 32 };
		for( auto &upPage : m_Pages )
		{	upPage->TopLeft( TL );	}
	}

	m_upTopLVMenu = std::make_unique<TopLVMenu>(
		*this,
		std::vector<std::wstring>{ L"Status", L"Item", L"Magic" }
	);
	m_LocalStack.Push( std::make_unique< RefWrapper >( *m_upTopLVMenu )	);

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

	//LocalStackに積まれていない状況でもページを描画する
	if( m_LocalStack.size() == 1 )
	{
		for( const auto &upPage : m_Pages )
		{	upPage->Paint( hdc );	}
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
		for( auto &upPage : m_Pages )
		{	upPage->Visible( false );	}

		return;
	}

	if( m_iCurrChar != CharOrder )
	{
		m_iCurrChar = CharOrder;

		for( auto &upPage : m_Pages )
		{	upPage->OnSelectedCharChanged( m_iCurrChar );	}
	}

	if( m_iCurrPage != CmdOrder )
	{
		m_iCurrPage = CmdOrder;
		for( int i=0; i<3; ++i )
		{	m_Pages[i]->Visible( i==m_iCurrPage );	}
	}
}

void CampMenu::OnTopLVMenuSelected( int CharOrder, int CmdOrder )
{
	if( m_Pages[CmdOrder]->CanEnter() )
	{
		m_LocalStack.Push( std::make_unique<GUI::RefWrapper>( *m_Pages[CmdOrder] ) );
	}
}

void CampMenu::PushTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback )
{
	m_LocalStack.Push( m_upTopLVMenu->CreateTgtCharSelector( ForAll, Callback ) );
}

