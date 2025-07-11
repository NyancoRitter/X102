#include "framework.h"
#include "TestScene.h"
#include "Parts/CMonoBMP.h"
#include "ResManage/ImgBank.h"
#include "GUI/Menu.h"
#include <vector>

namespace TestSceneImpl
{
	using namespace GUI::Menu;

	class TestMenuContent : public IMenuContent
	{
	public:
		TestMenuContent( CMonoBMP &IconBMP )
		{
			m_Items.emplace_back( Icon(IconBMP, 24).AlignCenter(), Text( L"NOP1", 200 ) );
			m_Items.emplace_back( Icon(IconBMP, 24).AlignCenter(), Text( L"NOP2", 200 ) );
			m_Items.emplace_back( Icon(IconBMP, 24).AlignCenter(), Text( L"Quit APP", 200 ) );
		}

	public:
		virtual bool IsVerticalMenu() const override {	return true;	}
		virtual int nItems() const override {	return (int)m_Items.size();	}
		virtual int CursorPos() const override {	return m_CursorPos;	}
		virtual void CursorPos( int pos ){	m_CursorPos = ( pos<nItems()  ?  pos  :  -1 );	}
		virtual Vec2i ItemSize() const override {	return Vec2i{ 224, 24 };	}

		//項目への参照
		//indexが範囲外の場合は例外送出される
		virtual const IMenuItem &Item( int index ) const override
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	private:
		std::vector< MenuItem<Icon,Text> > m_Items;
		int m_CursorPos = -1;
	};
}

//=====================================

TestScene::TestScene( ITopLV &rTopLV )
	: m_rTopLV( rTopLV )
{
	m_upMenuIcon = ResManage::LoadMonoBMP_or_Dummy( L"Dummy16.bmp" );
	if( m_upMenuIcon )
	{	m_upMenuContent = std::make_unique<TestSceneImpl::TestMenuContent>( *m_upMenuIcon );	}
}

TestScene::~TestScene() = default;

void TestScene::OnEnter(){	m_upMenuContent->CursorPos( -1 );	}

Flags<SceneUpdateResult> TestScene::Update( const IController &Controller )
{
	if( !m_upMenuContent )return SceneUpdateResult::None;

	using GUI::Menu::HandleInputResult;
	switch( auto MHR = GUI::Menu::HandleInput( *m_upMenuContent, Controller ) )
	{
	case HandleInputResult::CursorMoved:
		return SceneUpdateResult::ReqRedraw;	break;
	case HandleInputResult::Selected:
		{
			if( m_upMenuContent->CursorPos() == 2 )
			{	return SceneUpdateResult::ReqAppQuit;	}
		}
		break;
	default:
		break;
	}

	return SceneUpdateResult::None;
}

void TestScene::Draw( HDC hdc )
{
	if( m_upMenuIcon )
	{
		GUI::Menu::MenuPainter()
			.OuterMargin( { 8, 12 } )
			.WithFrame( true )
			.TopLeft( { 20, 10 } )
			.IsFocused( true )
			.Draw( hdc, *m_upMenuContent );
	}
}