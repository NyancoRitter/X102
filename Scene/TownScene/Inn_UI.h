#pragma once
#include "TownScene.h"
#include "GUI/IGUI.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GUI/ImgPainter.h"
#include "Common/MoneyView.h"
#include "Common/CharSelMenuContent.h"
#include "Common/PainterSeq.h"
#include "Misc/Cmd.h"

namespace Town
{
	/// <summary>
	/// 宿屋UI
	/// 
	/// 使い回されない（：宿に入るタイミングで生成され→出るときに破棄される）ことが前提の実装になっている
	/// </summary>
	class TownScene::Inn_UI final : public GUI::IGUI
	{
	public:
		Inn_UI( TownScene &Outer );
		~Inn_UI();

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
		virtual void OnGotFocus() override {	this->Visible(true);	}
		virtual void OnLostFocus() override {	this->Visible(false);	}
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		Flags<GUI::GUIResult> TopMenuProc( GUI::Menu::HandleInputResult HIR );
		Flags<GUI::GUIResult> OnStaySelected();
		Flags<GUI::GUIResult> OnSaveSelected();

		int DecidePrice() const;
		void Save();
		void Stay( int Price );

		void UpdateCharViewContent();

	private:
		TownScene &m_Outer;

		//
		GUI::GUIStack m_LocalStack;
		CmdSequence<> m_Effects;
		PainterSeq m_EffectsPainter;

		//表示物
		GUI::TextLinePainter m_Header;
		MoneyView m_MoneyView;
		GUI::MonoImgPainter m_StaffImgPainter;
		std::wstring m_StaffText;
		//メニュー
		GUI::Menu::Menu m_Menu;
		GUI::Menu::GenericMenuContent<true> m_MenuContent;

		//※キャラクタの表示用に Menu を流用
		GUI::Menu::Menu m_PartyView;
		GUI::Menu::CharSelMenuContent<false> m_PartyViewCont;
	};
}
