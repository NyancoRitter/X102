#include "framework.h"
#include "Inn_UI.h"

#include "IController.h"
#include "GlobalConst.h"

#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"
#include "GUI/SimpleSelection.h"
#include "GUI/MenuAsGUI.h"

#include "PlayData/PlayData.h"

#include "ResManage/BmpBank.h"

using namespace GUI;

namespace Town
{
	TownScene::Inn_UI::Inn_UI( TownScene &Outer )
		: m_Outer( Outer )
		, m_MenuContent( Vec2i{96,32} )
	{
		//表示物セットアップ
		const PlayData &PD = m_Outer.CurrPlayData();
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"＝ 宿屋 ロングコートブラックキャッツ ＝" } );
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
					.Add( { Text( L"泊まる" ) } )
					.Add( { Text( L"Save" ) } )
					.Add( { Text( L"1F(酒場)へ" ) } )
					.Add( { Text( L"出る" ) } )
					.CursorPos( 0 );

				//パーティメンバーがいない状態では「泊まる」をグレー表示
				if( PD.CurrParty().empty() )
				{	m_MenuContent.Item(0).DrawColor( GUI::Color::GrayOut );	}
			}

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );
		}
		{//店員
			m_StaffImgPainter.SetImg( &ResManage::BMP( ResManage::NPC::InnStaff ) );
			const auto size = m_StaffImgPainter.Size();
			m_StaffImgPainter.TopLeft( Vec2i{ GlobalConst::GC_W - size[0] - 8, GlobalConst::GC_H - size[1] - 8 } );

			m_StaffText = L"休んでいくニャ？";
		}

		m_LocalStack.Push(
			std::make_unique< GUI::Menu::MenuAsGUI >(
				m_Menu,
				[this]( const IController &Controller )->auto{	return HandleTopMenuInput(Controller);	}
			) 
		);
	}

	TownScene::Inn_UI::~Inn_UI() = default;

	void TownScene::Inn_UI::Paint_( HDC hdc ) const
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
	Flags<GUIResult> TownScene::Inn_UI::Update( const IController &Controller )
	{
		if( Controller.OpenPartyMenu() )
		{	m_Outer.Push_CampMenu_UI();	return GUIResult::ReqRedraw;	}

		Flags<GUIResult> Ret = ( m_LocalStack.Update( Controller )  ?  GUIResult::ReqRedraw  :  GUIResult::None );
		if( m_LocalStack.empty() ){	Ret |= GUIResult::Finished;	}
		return Ret;
	}

	//トップメニューの入力
	Flags<GUIResult> TownScene::Inn_UI::HandleTopMenuInput( const IController &Controller )
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
				case 0:	return OnStaySelected();	break;
				case 1:	return OnSaveSelected();	break;
				case 2:	m_Outer.Push_Pub_UI();	return ( GUIResult::ReqRedraw | GUIResult::Finished );	break;
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

	//"泊まる" 選択時
	Flags<GUI::GUIResult> TownScene::Inn_UI::OnStaySelected()
	{
		//パーティーメンバーが空の状態では何もしない
		if( m_Outer.CurrPlayData().CurrParty().empty() )
		{	return GUI::GUIResult::None;	}

		//
		const int Price = DecidePrice();
		auto upConfirmUI = std::make_unique< GUI::Menu::SimpleSelection >(
			L"「 宿泊料は " + std::to_wstring(Price) + L" G になります 」",
			Vec2i{160,32}
		);
		upConfirmUI->Add( L"泊まる" ).Add( L"やめる" )
			.OnSelect(
				[this, Price](int iSel)->bool
				{
					if( iSel==0 ){	Stay( Price );	}
					return true;
				}
			)
			.CursorPos( -1 )
			.XCenter( GlobalConst::GC_W )
			.YCenter( GlobalConst::GC_H );

		m_LocalStack.Push( std::move(upConfirmUI) );
		return GUIResult::ReqRedraw;
	}

	//"Save" 選択時
	Flags<GUI::GUIResult> TownScene::Inn_UI::OnSaveSelected()
	{
		auto upConfirmUI = std::make_unique< GUI::Menu::SimpleSelection >(
			L"※ 既存セーブデータがある場合には 上書きされます ※",
			Vec2i{160,32}
		);
		upConfirmUI->Add( L"セーブする" ).Add( L"やめる" )
			.OnSelect(
				[this](int iSel)->bool
				{
					if( iSel==0 ){	Save();	}
					return true;
				}
			)
			.CursorPos( -1 )
			.XCenter( GlobalConst::GC_W )
			.YCenter( GlobalConst::GC_H );

		m_LocalStack.Push( std::move(upConfirmUI) );
		return GUIResult::ReqRedraw;
	}

	//データセーブ処理実施
	void TownScene::Inn_UI::Save()
	{
		//TODO : セーブ処理，セーブした旨の表示
	}

	/// <summary>
	/// 宿に泊まる処理の実施
	/// </summary>
	/// <param name="Price">宿泊料．所持金から減らすべき値．</param>
	void TownScene::Inn_UI::Stay( int Price )
	{
		//TODO : 回復処理

		//所持金を減らす
		auto &PD = m_Outer.CurrPlayData();
		PD.AddMoney( -Price );
		const int Money = PD.Money();
		m_MoneyView.SetMoney( Money );

		//
		if( Money < 0 )
		{	m_StaffText = L"ツケにしとくニャ…";	}
	}

	//宿泊料の決定
	int TownScene::Inn_UI::DecidePrice() const
	{	//TODO : 実装
		return 77;
	}
}
