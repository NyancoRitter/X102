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
		using ProcFunc = std::function< Flags<GUIResult>( HandleInputResult ) >;

	public:
		/// <summary>
		/// ctor.
		/// ���j���[�ƁCUpdate()�ŌĂ΂�邱�ƂɂȂ郁�j���[�̓��͏�����n��
		/// </summary>
		/// <param name="rMenu">���j���[</param>
		/// <param name="Handler">
		/// ���j���[���͏������ʂɉ��������������D
		/// �����ɂ� rMenu.HandleInput() ���Ԃ����l���^������D
		/// �߂�l�� Update() �̖߂�l�ƂȂ�D
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
