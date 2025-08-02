#include "framework.h"
#include "Pub_UI.h"

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
	TownScene::Pub_UI::Pub_UI( TownScene &Outer )
		: m_Outer( Outer )
		, m_MenuContent( Vec2i{96,32} )
	{
		//表示物セットアップ
		const PlayData &PD = m_Outer.CurrPlayData();
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"＝ 酒場 ロングコートブラックキャッツ ＝" } );
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
				m_MenuContent.Reserve( 4 );
				m_MenuContent
					.Add( { Text(L"パーティ編成") } )
					.Add( { Text(L"LV UP") } )
					.Add( { Text(L"2F(宿)へ") } )
					.Add( { Text(L"出る") } )
					.CursorPos( 0 );
			}

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );
		}
		{//店員
			m_StaffImgPainter.SetImg( &ResManage::BMP( ResManage::NPC::PubStaff ) );
			const auto size = m_StaffImgPainter.Size();
			m_StaffImgPainter.TopLeft( Vec2i{ GlobalConst::GC_W - size[0] - 2, GlobalConst::GC_H - size[1] - 4 } );

			m_StaffText = L"あら　いらっしゃい！";
		}

		m_LocalStack.Push(
			std::make_unique< GUI::Menu::MenuAsGUI >(
				m_Menu,
				[this]( const IController &Controller )->auto{	return HandleTopMenuInput(Controller);	}
			) 
		);
	}

	TownScene::Pub_UI::~Pub_UI() = default;

	void TownScene::Pub_UI::Paint_( HDC hdc ) const
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
	Flags<GUIResult> TownScene::Pub_UI::Update( const IController &Controller )
	{
		if( Controller.OpenPartyMenu() )
		{	m_Outer.Push_CampMenu_UI();	return GUIResult::ReqRedraw;	}

		Flags<GUIResult> Ret = ( m_LocalStack.Update( Controller )  ?  GUIResult::ReqRedraw  :  GUIResult::None );
		if( m_LocalStack.empty() ){	Ret |= GUIResult::Finished;	}
		return Ret;
	}

	//トップメニューの入力
	Flags<GUIResult> TownScene::Pub_UI::HandleTopMenuInput( const IController &Controller )
	{
		using namespace GUI::Menu;

		switch( m_Menu.HandleInput( Controller ) )
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
				case 0:		break;
				case 1:		break;
				case 2:	m_Outer.Push_Inn_UI();	return ( GUIResult::ReqRedraw | GUIResult::Finished );	break;
				case 3:	return ( GUIResult::ReqRedraw | GUIResult::Finished );	break;
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
