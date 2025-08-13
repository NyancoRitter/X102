#include "framework.h"
#include "Shop_UI.h"

#include "IController.h"
#include "GlobalConst.h"

#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"
#include "GUI/MenuAsGUI.h"

#include "PlayData/PlayData.h"

#include "ResManage/BmpBank.h"


using namespace GUI;

namespace Town
{
	TownScene::Shop_UI::Shop_UI( TownScene &Outer )
		: m_Outer( Outer )
		, m_MenuContent( Vec2i{96,32} )
	{
		//表示物セットアップ
		const PlayData &PD = m_Outer.CurrPlayData();
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"＝ 冒険者の店 トレジャーボックス ＝" } );
			m_Header.LineWidth( GlobalConst::GC_W - 2*Padding ).LineHeight( 40 );
			m_Header.TopLeft( { Padding, Padding } );
		}
		{//所持金表示
			m_MoneyView.TopLeft( Vec2i{ GlobalConst::GC_W-(Padding+m_MoneyView.Size()[0]), m_Header.BoundingRect().Bottom()+Padding } );
			m_MoneyView.SetMoney( PD.Money() );
		}
		{//メニュー
			using namespace GUI::Menu;
			{
				m_MenuContent.Reserve( 3 );
				m_MenuContent.Add( { Text( L"買う" ) } );
				m_MenuContent.Add( { Text( L"売る" ) } );
				m_MenuContent.Add( { Text( L"出る" ) } );
				m_MenuContent.CursorPos( 0 );
			}

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );

			m_LocalStack.Push(
				std::make_unique< GUI::Menu::MenuAsGUI >( m_Menu, [this]( HandleInputResult HIR ){	return TopMenuProc(HIR);	} ) 
			);
		}
		{//店員
			m_StaffImgPainter.SetImg( &ResManage::BMP( ResManage::NPC::ShopStaff ) );
			const auto size = m_StaffImgPainter.Size();
			m_StaffImgPainter.TopLeft( Vec2i{ GlobalConst::GC_W - size[0] - 8, GlobalConst::GC_H - size[1] - 8 } );

			m_StaffText = L"いらっしゃいませ";
		}
	}

	TownScene::Shop_UI::~Shop_UI() = default;

	void TownScene::Shop_UI::Paint_( HDC hdc ) const
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		//店員
		m_StaffImgPainter.Paint( hdc );

		if( m_Menu.IsFocused() )
		{
			RECT TextRect;
			TextRect.left = 0;
			TextRect.right = GlobalConst::GC_W - 20;
			TextRect.bottom = m_StaffImgPainter.TopLeft()[1] - 8;
			TextRect.top = 0;
			SetTextColor( hdc, Color::White );
			SetBkMode( hdc, TRANSPARENT );
			DrawTextW( hdc, m_StaffText.c_str(), -1, &TextRect, DT_NOCLIP|DT_SINGLELINE|DT_RIGHT|DT_BOTTOM );
		}

		//ヘッダ
		DrawFrame( hdc, m_Header.BoundingRect(), Color::White );
		m_Header.Paint( hdc );

		//所持金
		m_MoneyView.Paint( hdc );
		
		//UI
		m_LocalStack.Paint( hdc );
	}

	//更新
	Flags<GUIResult> TownScene::Shop_UI::Update( const IController &Controller )
	{
		if( Controller.OpenCampMenu() )
		{	m_Outer.Push_CampMenu_UI( nullptr );	return GUIResult::ReqRedraw;	}

		Flags<GUIResult> Ret = ( m_LocalStack.Update( Controller )  ?  GUIResult::ReqRedraw  :  GUIResult::None );
		if( m_LocalStack.empty() ){	Ret |= GUIResult::Finished;	}
		return Ret;
	}

	//トップメニューの入力
	Flags<GUIResult> TownScene::Shop_UI::TopMenuProc( GUI::Menu::HandleInputResult HIR )
	{
		using namespace GUI::Menu;

		switch( HIR )
		{
		case HandleInputResult::CursorMoved:
			return GUIResult::ReqRedraw;
			break;

		case HandleInputResult::Canceled:
			return ( GUIResult::ReqRedraw | GUIResult::Finished );
			break;

		case HandleInputResult::Selected:
			{
				switch( m_MenuContent.CursorPos() )
				{
				case 0:	break;
				case 1:	break;
				case 2:	return ( GUIResult::ReqRedraw | GUIResult::Finished );	break;
				default:	break;
				}
			}
			break;

		default:
			break;
		}

		return GUIResult::None;
	}

}
