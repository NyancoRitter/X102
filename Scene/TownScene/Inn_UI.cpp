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
#include "Common/CharDrawColor.h"
#include "Common/EffectImpls.h"
#include "ResManage/SoundBank.h"

using namespace GUI;

namespace Town
{
	TownScene::Inn_UI::Inn_UI( TownScene &Outer )
		: m_Outer( Outer )
		, m_MenuContent( Vec2i{96,32} )
		, m_PartyViewCont( Vec2i() )
	{
		//�\�����Z�b�g�A�b�v
		const PlayData &PD = m_Outer.CurrPlayData();
		constexpr int Padding = 4;
		{//Header
			m_Header.TextLines( { L"�� �h�� �����O�R�[�g�u���b�N�L���b�c ��" } );
			m_Header.LineWidth( GlobalConst::GC_W - 2*Padding ).LineHeight( 40 );
			m_Header.TopLeft( { Padding, Padding } );
		}
		{//�������\��
			m_MoneyView.TopLeft( Vec2i{ GlobalConst::GC_W-(Padding+m_MoneyView.Size()[0]), m_Header.BoundingRect().Bottom()+Padding } );
			m_MoneyView.SetMoney( PD.Money() );
		}
		{//�L�����N�^�\��
			UpdateCharViewContent();

			m_PartyView.SetContent( &m_PartyViewCont )
				.ItemSpacing( 8 )
				.TopLeft( { 0, GlobalConst::GC_H - m_PartyView.Size()[1] - Padding } )
				.XCenter( GlobalConst::GC_W );
		}
		{//���j���[
			using namespace GUI::Menu;
			{
				m_MenuContent.Reserve( 4 );
				m_MenuContent.Add( { Text( L"���܂�" ) } );	
				m_MenuContent.Add( { Text( L"Save" ) } );
				m_MenuContent.Add( { Text( L"1F(����)��" ) } );
				m_MenuContent.Add( { Text( L"�o��" ) } );
				m_MenuContent.CursorPos( 0 );

				//�p�[�e�B�����o�[�����Ȃ���Ԃł́u���܂�v���O���[�\��
				if( PD.CurrParty().empty() )
				{	m_MenuContent.Item(0).DrawColor( GUI::Color::GrayOut );	}
			}

			m_Menu.SetContent( &m_MenuContent )
				.WithFrame( true )
				.TopLeft( { Padding, m_Header.BoundingRect().Bottom() + Padding } )
				.OuterMargin( Vec2i{12,4} )
				.IsFocused( true );

			m_LocalStack.Push(
				std::make_unique< MenuAsGUI >( m_Menu, [this]( HandleInputResult HIR ){	return TopMenuProc(HIR);	} ) 
			);
		}
		{//�X��
			m_StaffImgPainter.SetImg( &ResManage::BMP( ResManage::NPC::InnStaff ) );
			const auto size = m_StaffImgPainter.Size();
			m_StaffImgPainter.TopLeft( Vec2i{ GlobalConst::GC_W - size[0] - 8, GlobalConst::GC_H - size[1] - 8 } );

			m_StaffText = L"�x��ł����j���H";
		}
	}

	TownScene::Inn_UI::~Inn_UI() = default;

	//
	void TownScene::Inn_UI::UpdateCharViewContent()
	{
		const PlayData &PD = m_Outer.CurrPlayData();
		m_PartyViewCont.Clear();

		for( auto CharID : PD.CurrParty() )
		{
			const auto &Char = PD.Char( CharID );
			m_PartyViewCont.Add( CharID )
				.HPInfo( Char.HP(), Char.MaxHP() )
				.DrawColor( CharDrawColor( Char.HP(), Char.PoisonInfected() ) );
		}
	}

	//
	void TownScene::Inn_UI::Paint_( HDC hdc ) const
	{
		FillRectReg( hdc, Rect( 0,0, GlobalConst::GC_W, GlobalConst::GC_H ), RGB(0,0,0) );

		//�X��
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

		//�w�b�_
		DrawFrame( hdc, m_Header.BoundingRect(), Color::White );
		m_Header.Paint( hdc );

		//������, �p�[�e�B�\��
		m_MoneyView.Paint( hdc );
		m_PartyView.Paint( hdc );
		
		//UI
		m_LocalStack.Paint( hdc );

		//�G�t�F�N�g
		m_EffectList.Paint( hdc );
	}

	//�X�V
	Flags<GUIResult> TownScene::Inn_UI::Update( const IController &Controller )
	{
		if( !m_EffectList.empty() )
		{
			m_EffectList.Update();
			return GUIResult::ReqRedraw;
		}

		if( Controller.OpenPartyMenu() )
		{	m_Outer.Push_CampMenu_UI( nullptr );	return GUIResult::ReqRedraw;	}

		Flags<GUIResult> Ret = ( m_LocalStack.Update( Controller )  ?  GUIResult::ReqRedraw  :  GUIResult::None );
		if( m_LocalStack.empty() ){	Ret |= GUIResult::Finished;	}
		return Ret;
	}

	//�g�b�v���j���[�̓���
	Flags<GUIResult> TownScene::Inn_UI::TopMenuProc( GUI::Menu::HandleInputResult HIR )
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

	//"���܂�" �I����
	Flags<GUI::GUIResult> TownScene::Inn_UI::OnStaySelected()
	{
		//�p�[�e�B�[�����o�[����̏�Ԃł͉������Ȃ�
		if( m_Outer.CurrPlayData().CurrParty().empty() )
		{	return GUI::GUIResult::None;	}

		//
		const int Price = DecidePrice();
		auto upConfirmUI = std::make_unique< GUI::Menu::SimpleSelection >(
			L"�u �h������ " + std::to_wstring(Price) + L" G �ɂȂ�܂� �v",
			Vec2i{160,32}
		);
		upConfirmUI->Add( L"���܂�" ).Add( L"��߂�" )
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

	//"Save" �I����
	Flags<GUI::GUIResult> TownScene::Inn_UI::OnSaveSelected()
	{
		auto upConfirmUI = std::make_unique< GUI::Menu::SimpleSelection >(
			L"�� �����Z�[�u�f�[�^������ꍇ�ɂ� �㏑������܂� ��",
			Vec2i{160,32}
		);
		upConfirmUI->Add( L"�Z�[�u����" ).Add( L"��߂�" )
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

	//�f�[�^�Z�[�u�������{
	void TownScene::Inn_UI::Save()
	{
		//TODO : �Z�[�u�����C�Z�[�u�����|�̕\��
	}

	/// <summary>
	/// �h�ɔ��܂鏈���̎��{
	/// </summary>
	/// <param name="Price">�h�����D���������猸�炷�ׂ��l�D</param>
	void TownScene::Inn_UI::Stay( int Price )
	{
		auto &PD = m_Outer.CurrPlayData();

		ResManage::PlaySE( ResManage::SE::Cure );

		//�񕜏����ƕ\���X�V
		auto Results = PD.ProcOfINN();
		for( const auto &result : Results )
		{
			if( const auto *p=std::get_if<HPChanged>( &result );	(p && p->Amount>0) )
			{
				auto BB = m_PartyView.ItemDrawRect( p->TgtChar.m_Order );
				m_EffectList.PushBack( CreateHPRecovEffect( p->Amount, ( BB.TopLeft()+BB.RightBottom() )/2, 0 ) );
			}
		}
		UpdateCharViewContent();

		//�����������炷
		PD.AddMoney( -Price );
		const int Money = PD.Money();
		m_MoneyView.SetMoney( Money );

		//
		if( Money < 0 )
		{	m_StaffText = L"�c�P�ɂ��Ƃ��j���c";	}
	}

	//�h�����̌���
	int TownScene::Inn_UI::DecidePrice() const
	{
		const auto &PD = m_Outer.CurrPlayData();

		int SumLV = 0;
		for( auto CharID : PD.CurrParty() )
		{	SumLV += PD.Char( CharID ).LV();	}

		return std::max( (int)PD.CurrParty().size(), SumLV/2 );
	}
}
