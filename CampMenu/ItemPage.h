#pragma once

#include "IPage.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GameContent/ItemID.h"

/// <summary>
/// �L�����v��� - �A�C�e��UI
/// * ����ɉ������f�[�^�X�V���K�v�ȏꍇ�C�Ή����� CampMenu �̃��\�b�h���Ăԁi���̃N���X�����ړI�Ƀf�[�^���X�V���邱�Ƃ͂��Ȃ��j
/// </summary>
class CampMenu::ItemPage final : public CampMenu::IPage
{
public:
	ItemPage( CampMenu &Outer );

	//�A�C�e�����g�p���ꂽ����UI�X�V
	void UpdateOnItemUsed();

public:	// IPage Impl
	virtual void OnSelectedCharChanged() override;
	virtual bool CanEnter() const override {	return true;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnPushed() override;
	virtual void OnPrePopped() override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	void SwitchFocusTo( int iMenu );
	void UpdateCharItemContent();
	void UpdateStockContent();
	void UpdateDescView();

private:
	CampMenu &m_Outer;
	bool m_ShowInfos = false;
	GUI::TextLinePainter m_DescView;

	//[0]:�L�����N�^�ێ���, [1]:Stock
	GUI::Menu::Menu m_Menu[2];
	GUI::Menu::GenericMenuContent<true> m_Content[2];
	int m_iFocusedMenu = 0;

	inline static constexpr int DescViewXMargin = 6;
};
