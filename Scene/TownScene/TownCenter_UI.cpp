#include "framework.h"
#include "TownCenter_UI.h"

#include "IController.h"
#include "GlobalConst.h"

#include "GUI/DrawFuncs.h"
#include "GUI/Color.h"

#include "ResManage/BmpBank.h"

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
			m_MenuContent
				.Add( { Icon( BMP( TownImg::Inn ), 16+6 ), Text( L"�h" ) } )
				.Add( { Icon( BMP( TownImg::Pub ), 16+6 ), Text( L"����" ) } )
				.Add( { Icon( BMP( TownImg::Shop ), 16+6 ), Text( L"�X" ) } )
				.Add( { Icon( BMP( TownImg::Maze ), 16+6 ), Text( L"���{" ) } )
				.CursorPos( 0 );

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );
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
		
		//���j���[
		m_Menu.Paint( hdc );
	}

	//�X�V
	Flags<GUIResult> TownScene::TownCenter_UI::Update( const IController &Controller )
	{
		if( Controller.OpenPartyMenu() )
		{	m_Outer.Push_CampMenu_UI();	return GUIResult::ReqRedraw;	}

		using namespace GUI::Menu;

		switch( m_Menu.HandleInput( Controller ) )
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
				case 3:	m_Outer.GoTo_Maze();	return GUIResult::ReqRedraw;	break;
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
