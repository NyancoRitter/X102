#include "framework.h"
#include "TownScene.h"
#include "TownCenter_UI.h"

namespace Town
{
	TownScene::TownScene( ITopLV &rTopLV ) : m_rTopLV( rTopLV ) {}
	TownScene::~TownScene() = default;

	void TownScene::OnEnter()
	{
		GoTo_TownCenter();
	}
	
	void TownScene::OnLeave()
	{
		m_Stack.clear();
	}

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

	void TownScene::GoTo_TownCenter()
	{
		m_Stack.clear();
		m_Stack.Push( std::make_unique<TownCenter_UI>( *this ) );
	}

	void TownScene::GoTo_Inn()
	{
		//m_Stack.clear();
		//m_Stack.Push( std::make_unique<TownCenter_UI>( *this ) );
	}

	void TownScene::GoTo_Pub()
	{
		//m_Stack.clear();
		//m_Stack.Push( std::make_unique<TownCenter_UI>( *this ) );
	}

	void TownScene::GoTo_Shop()
	{
		//m_Stack.clear();
		//m_Stack.Push( std::make_unique<TownCenter_UI>( *this ) );
	}

	void TownScene::GoTo_Maze()
	{
	}
}
