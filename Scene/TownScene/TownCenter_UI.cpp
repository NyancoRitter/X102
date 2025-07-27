#include "framework.h"
#include "TownCenter_UI.h"

#include "Parts/CMonoBmp.h"
#include "ResManage/ImgBank.h"
#include "DataPath.h"
#include "GUI/MenuContent.h"

#include "GlobalConst.h"
#include "GUI/GUI_Funcs.h"

using namespace GUI;

namespace Town
{
	std::unique_ptr<ResManage::MonoBmpBank> TownScene::TownCenter_UI::ms_MenuIcons;

	TownScene::TownCenter_UI::TownCenter_UI( TownScene &Outer )
		: m_Outer( Outer )
	{
		//メニュー用アイコン
		if( !ms_MenuIcons )
		{
			ms_MenuIcons = ResManage::MonoBmpBank::Create(
				ImgDirU16(),
				{ L"Location_Inn.bmp", L"Location_Pub.bmp", L"Location_Shop.bmp", L"Location_Maze.bmp" }
			);
		}

		//メニューセットアップ
		using namespace GUI::Menu;
		{
			const std::wstring ItemTexts[4] = { L"宿", L"酒場", L"店", L"迷宮" };

			auto upContent = std::make_unique< VMenuContent< Icon, Text > >( Vec2i{96,32} );
			upContent->Reserve( 4 );
			for( int i=0; i<4; ++i )
			{	upContent->Add( Icon( *(*ms_MenuIcons)[i], 16+6 ), Text( ItemTexts[i] ) );	}

			m_upMenuContent = std::move(upContent);
			m_upMenuContent->CursorPos( 0 );
		}
		m_Menu.SetContent( m_upMenuContent.get() )
			.WithFrame( true )
			.OuterMargin( Vec2i{12,4} )
			.IsFocused( true );
	}

	TownScene::TownCenter_UI::~TownCenter_UI() = default;

	void TownScene::TownCenter_UI::Paint_( HDC hdc ) const
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );
		m_Menu.Paint( hdc );
	}

	Flags<GUIResult> TownScene::TownCenter_UI::Update( IGUIStack &Stack, const IController &Controller )
	{
		using namespace GUI::Menu;

		switch( m_Menu.HandleInput( Controller ) )
		{
		case HandleInputResult::CursorMoved:
			return GUIResult::ReqRedraw;
			break;

		case HandleInputResult::Selected:
			{
				switch( m_upMenuContent->CursorPos() )
				{
				case 0:	m_Outer.GoTo_Inn();	return GUIResult::ReqRedraw;	break;
				case 1:	m_Outer.GoTo_Pub();	return GUIResult::ReqRedraw;	break;
				case 2:	m_Outer.GoTo_Shop();	return GUIResult::ReqRedraw;	break;
				case 3:	m_Outer.GoTo_Maze();	return GUIResult::ReqRedraw;	break;
				default:	break;
				}
			}
			break;
		}

		return GUIResult::None;
	}
}
