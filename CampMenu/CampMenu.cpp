#include "framework.h"
#include "CampMenu.h"

#include "IController.h"
#include "GlobalConst.h"
#include "GUI/DrawFuncs.h"
//#include "GUI/Color.h"
//
//#include "GUI/MenuAsGUI.h"
//
#include "PlayData/PlayData.h"
//
//#include "ResManage/ImgBank.h"
//#include "DataPath.h"

using namespace GUI;

CampMenu::CampMenu( PlayData &rPlayData )
	: m_rPlayData( rPlayData )
	, m_CharSelMenuContent( Vec2i{ 16, 0 } )
{
	{//èäéùã‡ï\é¶
		m_MoneyView.TopLeft( Vec2i{ 10, 400 } );
		m_MoneyView.SetMoney( m_rPlayData.Money() );
	}

	//TestCode
	m_CharSelMenuContent
		.Add( GameContent::PartyCharID::AKINS )
		.Add( GameContent::PartyCharID::FANA )
		.Add( GameContent::PartyCharID::IRY )
		.CursorPos( 1 );
	m_CharSelMenu.SetContent( &m_CharSelMenuContent ).TopLeft( Vec2i{ 100, 6 } ).ItemSpacing(4);
	m_CharSelMenu.IsFocused( false );
}

CampMenu::~CampMenu() = default;


void CampMenu::Paint_( HDC hdc ) const
{
	FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

	//èäéùã‡
	m_MoneyView.Paint( hdc );

	m_CharSelMenu.Paint( hdc );
}

//çXêV
Flags<GUIResult> CampMenu::Update( const IController &Controller )
{
	if( Controller.Cancel() )
	{	return ( GUIResult::Finished | GUIResult::ReqRedraw );	}
	
	return GUIResult::None;
}


