#pragma once

#include "CampMenu.h"

namespace GameContent{	class PartyChar;	}

/// <summary>
/// キャンプ画面 - ステータス表示
/// </summary>
class CampMenu::StatusPage final : public IGUI
{
public:
	StatusPage( CampMenu &Outer ) : m_Outer( Outer ) {}

public:
	void SetDispTgt( const GameContent::PartyChar *pChar ){	m_pChar = pChar;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override {	return GUI::GUIResult::None;	}
	//virtual void OnGotFocus() override;
	//virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
	virtual StatusPage &TopLeft( const Vec2i &TL ) override {	m_TopLeft=TL;	return *this;	}
	virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装
protected:
	virtual void Paint_( HDC hdc ) const override;

private:

private:
	CampMenu &m_Outer;

	Vec2i m_TopLeft;
	const GameContent::PartyChar *m_pChar = nullptr;
};
