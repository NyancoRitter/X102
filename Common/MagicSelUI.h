#pragma once

#include <vector>
#include <functional>
#include "GameContent/Spell.h"
#include "GUI/IGUI.h"
#include "GUI/IRectReg.h"

#include "GUI/Menu.h"
#include "GUI/GenericMenuContent.h"

namespace GameContent
{
	class PartyChar;
	class Magic;
}

/// <summary>
/// 魔法選択用 UI
/// </summary>
class MagicSelUI final : public GUI::IGUI, public GUI::IRectReg
{
public:
	using OnIndicatedMagicChanged = std::function< Flags<GUI::GUIResult>() >;
	using OnMagicSelected = std::function< Flags<GUI::GUIResult>() >;

public:
	MagicSelUI( const Vec2i &ItemDrawSize );

	/// <summary>
	/// メニュー項目の更新
	/// </summary>
	/// <param name="MagicUser">このキャラクタの現状に合わせて更新される</param>
	/// <param name="ResetCursorPos">カーソル位置をリセットするか否か</param>
	void UpdateContent( const GameContent::PartyChar &MagicUser, bool ResetCursorPos );

	/// <summary>
	/// カーソル表示の有無
	/// </summary>
	/// <param name="Visible">表示するか否か</param>
	void CursorVisiblity( bool Visible );

	/// <summary>
	/// メニューで選択可能な魔法が無い状態か否か
	/// </summary>
	/// <returns></returns>
	bool NoAvailableMagic() const {	return m_Spell1st.empty() || m_Spell2nd.empty();	}

	/// <summary>
	/// 現在のメニューカーソルに対応する魔法の情報を取得．
	/// </summary>
	/// <returns>魔法情報．ただしメニューが有効な魔法を指さない状況ではnullptrを返す</returns>
	const GameContent::Magic *CurrIndicatedMagic();

public:	//コールバックの設定

	/// <summary>
	/// Update() 内でカーソルが指す対象が変化したときの処理を設定．
	/// 設定した処理が返した値が Update() の戻り値となる．
	/// </summary>
	/// <param name="Func">処理</param>
	/// <returns>*this</returns>
	MagicSelUI &Set_OnIndicatedMagicChanged_Proc( const OnIndicatedMagicChanged &Func ){	m_OnIndicatedMagicChanged=Func;	return *this;	}

	/// <summary>
	/// Update() 内で選択操作が発生した際に呼ばれる．
	/// 設定した処理が返した値が Update() の戻り値となる．
	/// </summary>
	/// <param name="Func">処理</param>
	/// <returns>*this</returns>
	MagicSelUI &Set_OnMagicSelected_Proc( const OnMagicSelected &Func ){	m_OnMagicSelected=Func;	return *this;	}

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
protected:
	virtual void Paint_( HDC hdc ) const override;
public:	// IRectReg Impl
	virtual Vec2i TopLeft() const override {	return m_Menu[0].TopLeft();	}
	virtual MagicSelUI &TopLeft( const Vec2i &TL ) override;
	virtual Vec2i Size() const override;

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
