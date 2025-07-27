#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"

namespace ResManage{	class MonoBmpBank;	}


namespace Town
{
	/// <summary>
	/// �X�̒��SUI
	/// * �ǂ��ɍs���̂������j���[�őI�������i
	/// </summary>
	class TownScene::TownCenter_UI final : public GUI::IGUI
	{
	public:
		TownCenter_UI( TownScene &Outer );
		~TownCenter_UI();

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( GUI::IGUIStack &Stack, const IController &Controller ) override;

		//virtual void OnGotFocus() override {}
		//virtual void OnLostFocus() override {}

		virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
		virtual TownCenter_UI &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
		virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����

	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		TownScene &m_Outer;
		//���j���[
		GUI::Menu::Menu m_Menu;
		std::unique_ptr< GUI::Menu::IMenuContent > m_upMenuContent;

		//���j���[�p�A�C�R��
		static std::unique_ptr<ResManage::MonoBmpBank> ms_MenuIcons;
	};
}
