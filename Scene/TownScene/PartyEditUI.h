#pragma once

#include "GUI/IGUI.h"
#include "GUI/Menu.h"
#include "Common/CharSelMenuContent.h"

class PlayData;

namespace GameContent{	enum class PartyCharID;	}

namespace Town
{
	/// <summary>
	/// �p�[�e�B�Ґ�UI
	/// 
	/// �g���񂳂�Ȃ����Ƃ��O��̎����ɂȂ��Ă���
	/// </summary>
	class PartyEditUI : public GUI::IGUI
	{
	public:
		PartyEditUI( PlayData &rPlayData );

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual PartyEditUI &TopLeft( const Vec2i &TL ) override;
		virtual Vec2i Size() const override;

		virtual void OnGotFocus() override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void SwitchFocusTo( int iMenu );
		void UpdateContent();
		bool MoveFromPartyToStandby( int iOrder );
		bool MoveFromStandbyToParty( int iOrder );
	private:
		PlayData &m_rPlayData;

		Vec2i m_TopLeft;

		std::vector< GameContent::PartyCharID > m_StandbyCharIDs;
		GUI::Menu::CharSelMenuContent<true> m_Content[2];
		GUI::Menu::Menu m_Menu[2];
		int m_iFocusedMenu = 0;

		static constexpr int XPadding = 16;	//�g���]���i���E�j
		static constexpr int YPadding = 8;	//�g���]���i�㉺�j
		static constexpr int LabelH = 24;	//���j���[�̏�ɕ\������e�L�X�g�̍���
		static constexpr int ContentMaxSize = 5;
	};
}
