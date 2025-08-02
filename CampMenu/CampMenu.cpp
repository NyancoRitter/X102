#include "framework.h"
#include "CampMenu.h"

#include "IController.h"
#include "GlobalConst.h"
#include "GUI/DrawFuncs.h"
#include "GUI/MenuAsGUI.h"

#include "GUI/Color.h"
//
//#include "GUI/MenuAsGUI.h"
//
#include "PlayData/PlayData.h"

#include "TopLVMenu.h"

using namespace GUI;

const Rect CampMenu::MainAreaRect{ Vec2i{168, 24}, Vec2i{GlobalConst::GC_W-2, GlobalConst::GC_H-2} };

CampMenu::CampMenu( PlayData &rPlayData )
	: m_rPlayData( rPlayData )
{
	{//èäéùã‡ï\é¶
		m_MoneyView.TopLeft( Vec2i{ 2, GlobalConst::GC_H-m_MoneyView.Size()[1]-2 } );
		m_MoneyView.SetMoney( m_rPlayData.Money() );
	}

	m_LocalStack.Push( std::make_unique<TopLVMenu>( *this ) );
}

CampMenu::~CampMenu() = default;

//
void CampMenu::Paint_( HDC hdc ) const
{
	FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );
	DrawFrame( hdc, MainAreaRect, GUI::Color::White );
	m_MoneyView.Paint( hdc );
	m_LocalStack.Paint( hdc );
}

//çXêV
Flags<GUIResult> CampMenu::Update( const IController &Controller )
{
	Flags<GUIResult> Ret;

	if( m_LocalStack.Update( Controller ) )
	{	Ret |= GUIResult::ReqRedraw;	}
	
	if( m_LocalStack.empty() )
	{	Ret |= GUIResult::Finished;	}

	return Ret;
}


