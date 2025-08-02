#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GUI/ImgPainter.h"
#include "Common/MoneyView.h"

namespace Town
{
	/// <summary>
	/// ����UI
	/// 
	/// �g���񂳂�Ȃ��i�F����ɓ���^�C�~���O�Ő������ꁨ�o��Ƃ��ɔj�������j���Ƃ��O��̎����ɂȂ��Ă���
	/// </summary>
	class TownScene::Pub_UI final : public GUI::IGUI
	{
	public:
		Pub_UI( TownScene &Outer );
		~Pub_UI();

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
		virtual void OnGotFocus() override {	this->Visible(true);	}
		virtual void OnLostFocus() override {	this->Visible(false);	}
		virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
		virtual Pub_UI &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
		virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		Flags<GUI::GUIResult> TopMenuProc( GUI::Menu::HandleInputResult HIR );

	private:
		TownScene &m_Outer;

		//
		GUI::GUIStack m_LocalStack;

		//�\����
		GUI::TextLinePainter m_Header;
		MoneyView m_MoneyView;
		GUI::MonoImgPainter m_StaffImgPainter;
		std::wstring m_StaffText;
		//���j���[
		GUI::Menu::Menu m_Menu;
		GUI::Menu::GenericMenuContent<true> m_MenuContent;
	};
}
