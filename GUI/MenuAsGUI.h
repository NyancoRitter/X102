#pragma once

#include <functional>
#include "IGUI.h"
#include "Menu.h"

namespace GUI::Menu
{
	/// <summary>
	/// Menu �� IGUI �Ƃ��Ĉ������߂̃A�_�v�^
	/// </summary>
	class MenuAsGUI : public IGUI
	{
	public:
		using InputHandler = std::function< Flags<GUIResult>( const IController & ) >;

	public:
		/// <summary>
		/// ctor.
		/// ���j���[�ƁCUpdate()�ŌĂ΂�邱�ƂɂȂ郁�j���[�̓��͏�����n��
		/// </summary>
		/// <param name="rMenu">���j���[</param>
		/// <param name="Handler">���͏����D������߂�l�̎d�l�� IGUI::Update() �Ɠ���D</param>
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
