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
	virtual void OnSelectedCharChanged() override {}
	virtual bool CanEnter() const override {	return false;	}
public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override {	return GUI::GUIResult::None;	}
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	CampMenu &m_Outer;
};
