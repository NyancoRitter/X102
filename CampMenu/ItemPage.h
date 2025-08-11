#pragma once

#include "IPage.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "GUI/TextLinePainter.h"
#include "GameContent/ItemID.h"

/// <summary>
/// キャンプ画面 - アイテムUI
/// </summary>
class CampMenu::ItemPage final : public CampMenu::IPage
{
public:
	ItemPage( CampMenu &Outer );

	//アイテムが使用された時の更新処理
	void UpdateOnItemUsed( int iCurrCharOrder );

public:	// IPage Impl
	virtual void OnSelectedCharChanged( int iCharOrder ) override;
	virtual bool CanEnter() const override {	return true;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnPushed() override;
	virtual void OnPrePopped() override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	void SwitchFocusTo( int iMenu );
	void UpdateCharItemContent( int iCurrCharOrder );
	void UpdateStockContent();
	void UpdateDescView();
private:
	CampMenu &m_Outer;
	bool m_ShowInfos = false;
	GUI::TextLinePainter m_DescView;

	//[0]:キャラクタ保持物, [1]:Stock
	std::vector< GameContent::ItemID > m_ItemIDs[2];
	GUI::Menu::Menu m_Menu[2];
	GUI::Menu::GenericMenuContent<true> m_Content[2];
	int m_iFocusedMenu = 0;

	inline static constexpr int DescViewXMargin = 6;
};
