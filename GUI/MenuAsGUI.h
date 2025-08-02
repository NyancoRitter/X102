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
		using InputHandler = std::function< Flags<GUIResult>( const IController & ) >;

	public:
		/// <summary>
		/// ctor.
		/// メニューと，Update()で呼ばれることになるメニューの入力処理を渡す
		/// </summary>
		/// <param name="rMenu">メニュー</param>
		/// <param name="Handler">入力処理．引数や戻り値の仕様は IGUI::Update() と同一．</param>
		MenuAsGUI( Menu &rMenu, const InputHandler &Handler )
			: m_rMenu( rMenu )
			, m_InputHandler( Handler )
		{}

	public:	// IGUI Impl
		virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override
		{	return m_InputHandler( Controller );	}

		virtual void OnGotFocus() override {	m_rMenu.IsFocused(true);	}
		virtual void OnLostFocus() override {	m_rMenu.IsFocused(false);	}

		virtual Vec2i TopLeft() const override {	return m_rMenu.TopLeft();	}
		virtual MenuAsGUI &TopLeft( const Vec2i &TL ) override {	m_rMenu.TopLeft(TL);	return *this;	}
		virtual Vec2i Size() const override {	return m_rMenu.Size();	}

	protected:
		virtual void Paint_( HDC hdc ) const override {	m_rMenu.Paint( hdc );	}

	private:
		Menu &m_rMenu;
		InputHandler m_InputHandler;
	};

}
