#pragma once

#include "IPage.h"
#include "Common/MagicSelUI.h"
#include "GUI/TextLinePainter.h"

/// <summary>
/// キャンプ画面 - 魔法UI
/// * 操作に応じたデータ更新が必要な場合，対応する CampMenu のメソッドを呼ぶ（このクラスが直接的にデータを更新することはしない）
/// </summary>
class CampMenu::MagicPage final : public CampMenu::IPage
{
public:
	MagicPage( CampMenu &Outer );

	//魔法が使用された際のUI更新
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
