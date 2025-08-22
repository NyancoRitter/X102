#include "framework.h"
#include "TownCenter_UI.h"

#include "IController.h"
#include "GlobalConst.h"

#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"
#include "GUI/MenuAsGUI.h"
#include "GUI/MsgWnd.h"
#include "PlayData/PlayData.h"
#include "ResManage/BmpBank.h"
#include <algorithm>

using namespace GUI;

namespace Town
{
	TownScene::TownCenter_UI::TownCenter_UI( TownScene &Outer )
		: m_Outer( Outer )
		, m_MenuContent( Vec2i{96,32} )
	{
		using namespace ResManage;

		//表示物セットアップ
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"＝ 古都 ヴィオリスケス ＝" } );
			m_Header.LineWidth( GlobalConst::GC_W - 2*Padding ).LineHeight( 40 );
			m_Header.TopLeft( { Padding, Padding } );
		}
		{//メニュー
			using namespace GUI::Menu;

			m_MenuContent.Reserve( 4 );
			m_MenuContent.Add( { Icon( BMP( TownImg::Inn ), 16+6 ), Text( L"宿" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Pub ), 16+6 ), Text( L"酒場" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Shop ), 16+6 ), Text( L"店" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Maze ), 16+6 ), Text( L"迷宮" ) } );
			m_MenuContent.CursorPos( 0 );

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );

			m_GUIStack.Push(
				std::make_unique<GUI::Menu::MenuAsGUI>(
					m_Menu,
					[this]( HandleInputResult MenuResult )->Flags<GUIResult>{	return MenuHandler(MenuResult);	}
				)
			);
		}
		{//背景画像
			const int MenuR = m_Menu.BoundingRect().Right();
			const int HeaderB = m_Header.BoundingRect().Bottom();

			m_ImgPainter.SetImg( &BMP( TownImg::TownCenter ) )
				.TransBit0Part( false )
				.XCenter( GlobalConst::GC_W-MenuR, MenuR )
				.YCenter( GlobalConst::GC_H-HeaderB, HeaderB );
		}
	}

	TownScene::TownCenter_UI::~TownCenter_UI() = default;

	void TownScene::TownCenter_UI::SetImgToOutskirts()
	{	m_ImgPainter.SetImg( &ResManage::BMP( ResManage::TownImg::Outskirts ) );	}

	//描画
	void TownScene::TownCenter_UI::Paint_( HDC hdc ) const
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		//ヘッダ
		DrawFrame( hdc, m_Header.BoundingRect(), Color::White );
		m_Header.Paint( hdc );

		//背景画像
		DrawFrame( hdc, Rect::Dilation( m_ImgPainter.BoundingRect(), 8,8 ), Color::White );
		m_ImgPainter.Paint( hdc );
		
		//GUI
		m_GUIStack.Paint( hdc );
	}

	//更新
	Flags<GUIResult> TownScene::TownCenter_UI::Update( const IController &Controller )
	{
		if( m_Menu.IsFocused() )
		{
			if( Controller.OpenCampMenu() )
			{	m_Outer.Push_CampMenu_UI( nullptr );	return GUIResult::ReqRedraw;	}
		}

		if( m_GUIStack.Update( Controller ) )
		{	return GUIResult::ReqRedraw;	}

		return GUIResult::None;
	}

	//メニュー操作時処理
	Flags<GUI::GUIResult> TownScene::TownCenter_UI::MenuHandler( GUI::Menu::HandleInputResult MenuResult )
	{
		using namespace GUI::Menu;

		switch( MenuResult )
		{
		case HandleInputResult::CursorMoved:
			return GUIResult::ReqRedraw;
			break;

		case HandleInputResult::Selected:
			{
				m_ImgPainter.SetImg( &ResManage::BMP( ResManage::TownImg::TownCenter ) );	//このタイミングで背景画像をリセット

				switch( m_MenuContent.CursorPos() )
				{
				case 0:	m_Outer.Push_Inn_UI();	return GUIResult::ReqRedraw;	break;
				case 1:	m_Outer.Push_Pub_UI();	return GUIResult::ReqRedraw;	break;
				case 2:	m_Outer.Push_Shop_UI();	return GUIResult::ReqRedraw;	break;
				case 3:	OnMazeSelected();	return GUIResult::ReqRedraw;	break;
				default:	break;
				}
			}
			break;

		default:
			break;
		}
		return GUIResult::None;
	}

	//メニューで「迷宮」が選択された際の処理
	void TownScene::TownCenter_UI::OnMazeSelected()
	{
		{//パーティ未編成あるいは全滅状態ならば棄却する
			const auto &PD = m_Outer.CurrPlayData();
			const auto &Party = PD.CurrParty();

			if( Party.empty() )
			{
				auto upMsgWnd = std::make_unique<GUI::MsgWnd>();
				upMsgWnd->AlignLeft().TextLines( { L"誰が迷宮に挑むのか？", L"まずパーティを編成する必要がある" } );
				m_GUIStack.Push( std::move(upMsgWnd) );
				return;
			}

			if( std::none_of(
				Party.begin(), Party.end(),
				[&PD](auto ID)->bool{	return ( PD.Char(ID).HP() >= 0 );	}
			) )
			{
				auto upMsgWnd = std::make_unique<GUI::MsgWnd>();
				upMsgWnd->AlignLeft().TextLines( { L"その状態で迷宮に行くのは自殺行為だ！", L"今はまず体を休めることを考えよ！" } );
				m_GUIStack.Push( std::move(upMsgWnd) );
				return;
			}
		}

		//
		m_Outer.GoTo_Maze();
	}
}
