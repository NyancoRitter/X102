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

		//�\�����Z�b�g�A�b�v
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"�� �Ós ���B�I���X�P�X ��" } );
			m_Header.LineWidth( GlobalConst::GC_W - 2*Padding ).LineHeight( 40 );
			m_Header.TopLeft( { Padding, Padding } );
		}
		{//���j���[
			using namespace GUI::Menu;

			m_MenuContent.Reserve( 4 );
			m_MenuContent.Add( { Icon( BMP( TownImg::Inn ), 16+6 ), Text( L"�h" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Pub ), 16+6 ), Text( L"����" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Shop ), 16+6 ), Text( L"�X" ) } );
			m_MenuContent.Add( { Icon( BMP( TownImg::Maze ), 16+6 ), Text( L"���{" ) } );
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
		{//�w�i�摜
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

	//�`��
	void TownScene::TownCenter_UI::Paint_( HDC hdc ) const
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		//�w�b�_
		DrawFrame( hdc, m_Header.BoundingRect(), Color::White );
		m_Header.Paint( hdc );

		//�w�i�摜
		DrawFrame( hdc, Rect::Dilation( m_ImgPainter.BoundingRect(), 8,8 ), Color::White );
		m_ImgPainter.Paint( hdc );
		
		//GUI
		m_GUIStack.Paint( hdc );
	}

	//�X�V
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

	//���j���[���쎞����
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
				m_ImgPainter.SetImg( &ResManage::BMP( ResManage::TownImg::TownCenter ) );	//���̃^�C�~���O�Ŕw�i�摜�����Z�b�g

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

	//���j���[�Łu���{�v���I�����ꂽ�ۂ̏���
	void TownScene::TownCenter_UI::OnMazeSelected()
	{
		{//�p�[�e�B���Ґ����邢�͑S�ŏ�ԂȂ�Ί��p����
			const auto &PD = m_Outer.CurrPlayData();
			const auto &Party = PD.CurrParty();

			if( Party.empty() )
			{
				auto upMsgWnd = std::make_unique<GUI::MsgWnd>();
				upMsgWnd->AlignLeft().TextLines( { L"�N�����{�ɒ��ނ̂��H", L"�܂��p�[�e�B��Ґ�����K�v������" } );
				m_GUIStack.Push( std::move(upMsgWnd) );
				return;
			}

			if( std::none_of(
				Party.begin(), Party.end(),
				[&PD](auto ID)->bool{	return ( PD.Char(ID).HP() >= 0 );	}
			) )
			{
				auto upMsgWnd = std::make_unique<GUI::MsgWnd>();
				upMsgWnd->AlignLeft().TextLines( { L"���̏�ԂŖ��{�ɍs���͎̂��E�s�ׂ��I", L"���͂܂��̂��x�߂邱�Ƃ��l����I" } );
				m_GUIStack.Push( std::move(upMsgWnd) );
				return;
			}
		}

		//
		m_Outer.GoTo_Maze();
	}
}
