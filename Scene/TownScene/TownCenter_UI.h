#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GUI/ImgPainter.h"

namespace ResManage{	class MonoBmpHolder;	}

namespace Town
{
	/// <summary>
	/// 街の中心UI
	/// 
	/// 行先をメニューで選択するだけの画面
	/// </summary>
	class TownScene::TownCenter_UI final : public GUI::IGUI
	{
	public:
		TownCenter_UI( TownScene &Outer );
		~TownCenter_UI();

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;

		virtual void OnGotFocus() override {	this->Visible(true);	}
		virtual void OnLostFocus() override {	this->Visible(false);	}

		virtual Vec2i TopLeft() const override {	return Vec2i();	}	//※使わないのでDummy実装
		virtual TownCenter_UI &TopLeft( const Vec2i &TL ) override {	return *this;	}	//※使わないのでDummy実装
		virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装

	protected:
		virtual void Paint_( HDC hdc ) const override;

	public:
		/// <summary>
		/// 背景画像を，通常の街の絵ではなく「町はずれ」にする．
		/// ただし，この変更は施設（店など）への移動が発生した時点で自動的にリセットされる．
		///		<remarks>
		///		※パーティが戦闘敗北した後の表示用
		///		</remarks>
		/// </summary>
		void SetImgToOutskirts();

	private:
		TownScene &m_Outer;

		//表示物
		GUI::TextLinePainter m_Header;
		GUI::MonoImgPainter m_ImgPainter;
		//メニュー
		GUI::Menu::Menu m_Menu;
		GUI::Menu::GenericMenuContent<true> m_MenuContent;
	};
}
