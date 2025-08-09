#pragma once

#include "CampMenu.h"
#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"
#include "Common/CharSelMenuContent.h"
#include <functional>

/// <summary>
/// キャンプ画面 - キャラクタとコマンドを選ぶUI
/// </summary>
class CampMenu::TopLVMenu final : public IGUI
{
public:
	TopLVMenu( CampMenu &Outer, std::vector<std::wstring> CmdMenuItemStrs );

	/// <summary>
	/// アイテムや魔法の使用対象キャラクタ選択用UIを生成して返す
	/// </summary>
	/// <param name="ForAll">対象が全員のものか否か</param>
	/// <param name="Callback">選択用UIに渡すCallback（仕様は TgtSelection を参照 ）</param>
	/// <returns></returns>
	std::unique_ptr<IGUI> CreateTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback );

	/// <summary>
	/// キャラクタ選択メニューの表示を現パーティのデータに合わせて更新
	/// </summary>
	void UpdateCharSelMenuContent();

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	virtual void OnGotFocus() override;
	virtual void OnLostFocus() override;
	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//※使わないのでDummy実装
	virtual TopLVMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//※使わないのでDummy実装
	virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	CampMenu &m_Outer;

	GUI::Menu::Menu m_CharSelMenu;
	GUI::Menu::CharSelMenuContent<true> m_CharSelMenuContent;

	GUI::Menu::Menu m_CmdMenu;
	GUI::Menu::GenericMenuContent<false> m_CmdMenuContent;
};
