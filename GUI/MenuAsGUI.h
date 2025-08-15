#pragma once

#include <functional>
#include "IGUI.h"
#include "Menu.h"

namespace GUI::Menu
{
	/// <summary>
	/// Menu を IGUI として扱うためのアダプタ
	/// </summary>
	class MenuAsGUI : public IGUI
	{
	public:
		using ProcFunc = std::function< Flags<GUIResult>( HandleInputResult ) >;

	public:
		/// <summary>
		/// ctor.
		/// メニューと，Update()で呼ばれることになるメニューの入力処理を渡す
		/// </summary>
		/// <param name="rMenu">メニュー</param>
		/// <param name="Handler">
		/// メニュー入力処理結果に応じた処理実装．
		/// 引数には rMenu.HandleInput() が返した値が与えられる．
		/// 戻り値は Update() の戻り値となる．
		/// </param>
		MenuAsGUI( Menu &rMenu, const ProcFunc &Handler )
			: m_rMenu( rMenu )
			, m_ProcFunc( Handler )
		{}

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override
		{	return m_ProcFunc( m_rMenu.HandleInput(Controller) );	}

		virtual void OnGotFocus() override {	m_rMenu.IsFocused(true);	}
		virtual void OnLostFocus() override {	m_rMenu.IsFocused(false);	}
	protected:
		virtual void Paint_( HDC hdc ) const override {	m_rMenu.Paint( hdc );	}

	private:
		Menu &m_rMenu;
		ProcFunc m_ProcFunc;
	};

}
