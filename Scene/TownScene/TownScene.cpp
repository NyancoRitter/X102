#include "framework.h"
#include "TownScene.h"
#include "TownCenter_UI.h"
#include "Inn_UI.h"
#include "Pub_UI.h"
#include "Shop_UI.h"

#include "ITopLV.h"
#include "CampMenu/CampMenu.h"

namespace Town
{

	TownScene::TownScene( ITopLV &rTopLV ) : m_rTopLV( rTopLV )
	{
		m_upTownCenterUI = std::make_unique<TownCenter_UI>( *this );
	}

	TownScene::~TownScene() = default;

	void TownScene::OnEnter()
	{	m_Stack.Push( std::make_unique<GUI::RefWrapper>( *m_upTownCenterUI ) );	}
	
	void TownScene::OnLeave()
	{	m_Stack.clear( false );	}

	Flags<SceneUpdateResult> TownScene::Update( const IController &Controller )
	{
		if( m_Stack.Update( Controller ) )
		{
			return SceneUpdateResult::ReqRedraw;
		}

		return SceneUpdateResult::None;
	}

	void TownScene::Draw( HDC hdc )
	{
		m_Stack.Paint( hdc );
	}

	void TownScene::Push_Inn_UI(){	m_Stack.Push( std::make_unique<Inn_UI>( *this ) );	}
	void TownScene::Push_Pub_UI(){	m_Stack.Push( std::make_unique<Pub_UI>( *this ) );	}
	void TownScene::Push_Shop_UI(){	m_Stack.Push( std::make_unique<Shop_UI>( *this ) );	}
	void TownScene::Push_CampMenu_UI( GUI::GUIStack *pStack )
	{
		auto &TgtStack = ( pStack ? *pStack : m_Stack );
		TgtStack.Push( std::make_unique<CampMenu>( CurrPlayData() ) );
	}

	void TownScene::GoTo_Maze()
	{
	}

	PlayData &TownScene::CurrPlayData(){	return m_rTopLV.CurrPlayData();	}
}
