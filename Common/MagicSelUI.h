#pragma once

#include <vector>
#include <functional>
#include "GameContent/Spell.h"
#include "GUI/IGUI.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"

namespace GameContent
{
	class PartyChar;
	class Magic;
}

/// <summary>
/// ���@�I��p UI
/// </summary>
class MagicSelUI final : public GUI::IGUI
{
public:
	using OnIndicatedMagicChanged = std::function< Flags<GUI::GUIResult>() >;
	using OnMagicSelected = std::function< Flags<GUI::GUIResult>() >;

public:
	MagicSelUI( const Vec2i &ItemDrawSize );

	/// <summary>
	/// ���j���[���ڂ̍X�V
	/// </summary>
	/// <param name="MagicUser">���̃L�����N�^�̌���ɍ��킹�čX�V�����</param>
	void UpdateContent( const GameContent::PartyChar &MagicUser );

	/// <summary>
	/// ���j���[�őI���\�Ȗ��@��������Ԃ��ۂ�
	/// </summary>
	/// <returns></returns>
	bool NoAvailableMagic() const {	return m_Spell1st.empty() || m_Spell2nd.empty();	}

	/// <summary>
	/// ���݂̃��j���[�J�[�\���ɑΉ����閂�@�̏����擾�D
	/// </summary>
	/// <returns>���@���D���������j���[���L���Ȗ��@���w���Ȃ��󋵂ł�nullptr��Ԃ�</returns>
	const GameContent::Magic *CurrIndicatedMagic();

public:	//�R�[���o�b�N�̐ݒ�

	/// <summary>
	/// Update() ���őI�J�[�\�����w���Ώۂ��ω������Ƃ��̏�����ݒ�D
	/// �ݒ肵���������Ԃ����l�� Update() �̖߂�l�ƂȂ�D
	/// </summary>
	/// <param name="Func">����</param>
	/// <returns>*this</returns>
	MagicSelUI &Set_OnIndicatedMagicChanged_Proc( const OnIndicatedMagicChanged &Func ){	m_OnIndicatedMagicChanged=Func;	return *this;	}

	/// <summary>
	/// Update() ���őI�𑀍삪���������ۂɌĂ΂��D
	/// �ݒ肵���������Ԃ����l�� Update() �̖߂�l�ƂȂ�D
	/// </summary>
	/// <param name="Func">����</param>
	/// <returns>*this</returns>
	MagicSelUI &Set_OnMagicSelected_Proc( const OnMagicSelected &Func ){	m_OnMagicSelected=Func;	return *this;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_Menu[0].TopLeft();	}
	virtual MagicSelUI &TopLeft( const Vec2i &TL ) override;
	virtual Vec2i Size() const override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	void SwitchFocusTo( int iMenu );
private:
	Vec2i m_TopLeft;
	OnIndicatedMagicChanged m_OnIndicatedMagicChanged;
	OnMagicSelected m_OnMagicSelected;

	std::vector< GameContent::FirstSpell > m_Spell1st;
	std::vector< GameContent::SecondSpell > m_Spell2nd;

	GUI::Menu::GenericMenuContent<true> m_Content[2];
	GUI::Menu::Menu m_Menu[2];
	int m_iFocusedMenu = 0;
};
