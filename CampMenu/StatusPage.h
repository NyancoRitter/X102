#pragma once

#include "IPage.h"

/// <summary>
/// キャンプ画面 - ステータス表示
/// </summary>
class CampMenu::StatusPage final : public CampMenu::IPage
{
public:
	StatusPage( CampMenu &Outer ) : m_Outer( Outer ) {}

public:
	virtual void OnSelectedCharChanged( int iCharOrder ) override;
	virtual bool CanEnter() const override {	return false;	}
public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override {	return GUI::GUIResult::None;	}
	//virtual void OnGotFocus() override;
	//virtual void OnLostFocus() override;
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	CampMenu &m_Outer;
	const GameContent::PartyChar *m_pChar = nullptr;
};
