#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"

namespace ResManage{	class MonoBmpBank;	}


namespace Town
{
	/// <summary>
	/// 街の中心UI
	/// * どこに行くのかをメニューで選択する手段
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

		virtual Vec2i TopLeft() const override {	return Vec2i();	}	//※使わないのでDummy実装
		virtual TownCenter_UI &TopLeft( const Vec2i &TL ) override {	return *this;	}	//※使わないのでDummy実装
		virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装

	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		TownScene &m_Outer;
		//メニュー
		GUI::Menu::Menu m_Menu;
		std::unique_ptr< GUI::Menu::IMenuContent > m_upMenuContent;

		//メニュー用アイコン
		static std::unique_ptr<ResManage::MonoBmpBank> ms_MenuIcons;
	};
}
