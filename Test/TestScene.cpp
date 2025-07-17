#include "framework.h"
#include "TestScene.h"
//#include "Parts/CMonoBMP.h"
//#include "ResManage/ImgBank.h"
#include "GlobalConst.h"
//#include "GUI/Menu.h"
#include <vector>

//#include "GUI/MenuContent.h"
#include "GUI/SimpleSelection.h"

//=====================================

TestScene::TestScene( ITopLV &rTopLV )
	: m_rTopLV( rTopLV )
{
	//m_upMenuIcon = ResManage::LoadMonoBMP_or_Dummy( L"Dummy16.bmp" );

	using namespace GUI::Menu;
	auto SS = std::make_unique< SimpleSelection >( L"Test", Vec2i{224,24} );
	SS->Add( L"NOP1" ).Add( L"NOP2" ).Add( L"Quit APP" )
		.CursorPos( 0 )
		.Cancelable( false )
		.OnSelect(
			[]( int iSel )->bool
			{
				return iSel==2;
			}
		)
		.XCenter( GlobalConst::GC_W )
		.YCenter( GlobalConst::GC_H );
	
	m_Stack.Push( std::move(SS) );
}

TestScene::~TestScene() = default;

void TestScene::OnEnter(){}

Flags<SceneUpdateResult> TestScene::Update( const IController &Controller )
{
	if( m_Stack.empty() )
	{	return SceneUpdateResult::ReqAppQuit;	}

	if( m_Stack.Update( Controller ) )
	{
		return SceneUpdateResult::ReqRedraw;
	}
}

void TestScene::Draw( HDC hdc )
{
	m_Stack.Paint( hdc );
}
