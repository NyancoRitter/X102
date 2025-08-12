#pragma once

#include "IPage.h"
#include "Common/MagicSelUI.h"
#include "GUI/TextLinePainter.h"

/// <summary>
/// �L�����v��� - ���@UI
/// * ����ɉ������f�[�^�X�V���K�v�ȏꍇ�C�Ή����� CampMenu �̃��\�b�h���Ăԁi���̃N���X�����ړI�Ƀf�[�^���X�V���邱�Ƃ͂��Ȃ��j
/// </summary>
class CampMenu::MagicPage final : public CampMenu::IPage
{
public:
	MagicPage( CampMenu &Outer );

	//���@���g�p���ꂽ�ۂ�UI�X�V
	void UpdateOnMagicUsed();

public:	// IPage Impl
	virtual void OnSelectedCharChanged() override;
	virtual bool CanEnter() const override {	return !m_UI.NoAvailableMagic();	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnPushed() override;
	virtual void OnPrePopped() override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	Flags<GUI::GUIResult> OnIndicatedMagicChanged();
	Flags<GUI::GUIResult> OnMagicSelected();

	void UpdateDescView( const GameContent::Magic &TgtMagic );

private:
	CampMenu &m_Outer;

	MagicSelUI m_UI;
	GUI::TextLinePainter m_DescView;

	inline static constexpr int DescViewXMargin = 6;
};
