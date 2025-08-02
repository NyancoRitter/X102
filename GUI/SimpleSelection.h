#pragma once

#include <functional>
#include <string>
#include "IGUI.h"
#include "GenericMenuContent.h"
#include "Menu.h"

namespace GUI::Menu
{

	/// <summary>
	/// �P���ȑI����UI
	/// 
	/// 1�s�̃e�L�X�g�̉��Ɂi����������Łj�P���ȑI�����j���[��\������D
	///		<remarks>������Ƃ��� Yes/No �݂����Ȃ̂��o���p</remarks>
	/// </summary>
	class SimpleSelection : public IGUI
	{
	public:
		SimpleSelection( const std::wstring &MsgText, const Vec2i &ItemDrawSize )
			: m_Content( ItemDrawSize ), m_MsgText(MsgText)
		{
			m_Menu.SetContent( &m_Content ).WithFrame(false);
			UpdateMenuPos();
		}

	public:
		SimpleSelection &Reserve( size_t n ){	m_Content.Reserve(n);	return *this;	};

		/// <summary>�I�����̒ǉ�</summary>
		/// <param name="ItemText">�I����������</param>
		/// <returns>*this</returns>
		SimpleSelection &Add( const std::wstring &ItemText );

		/// <summary>�J�[�\���ʒu�ݒ�</summary>
		/// <param name="pos">�J�[�\���ʒu</param>
		/// <returns>*this</returns>
		SimpleSelection &CursorPos( int pos ){	m_Content.CursorPos(pos);	return *this;	}

		/// <summary>
		/// �I�𑀍삪�����ꂽ�Ƃ��̃R�[���o�b�N��ݒ�D
		/// ������w�肵�Ȃ��ꍇ�C�I�𑀍�����Ă������N���Ȃ��D
		/// </summary>
		/// <param name="Callback">
		/// * �����͑I�����ꂽ���ڂ�index�D
		///   �K�������ꂩ�̑I�������w�������i�F�J�[�\�����I�������w�������Ȃ���Ԃł͂��̃R�[���o�b�N�͌Ă΂�Ȃ��j�D
		/// * �߂�l�͂��̑I����UI��p���������������������ۂ��D
		/// </param>
		/// <returns>*this</returns>
		SimpleSelection &OnSelect(
			std::function<bool(int)> Callback
		)
		{	m_OnSelect=Callback;	return *this;	}

		/// <summary>
		/// �S�̂̕`�敝���w��D
		/// �w�肵�Ȃ��ꍇ�C���炩�̃f�t�H���g�T�C�Y�ƂȂ�D
		/// </summary>
		/// <param name="w">��</param>
		/// <returns>*this</returns>
		SimpleSelection &Width( int w ){	m_W=w;	UpdateMenuPos();	return *this;	}

		/// <summary>
		/// �L�����Z�����삪�\���H�@�f�t�H���g��true�D
		/// </summary>
		/// <param name="val">�L�����Z���\�Ƃ���Ȃ�true</param>
		/// <returns>*this</returns>
		SimpleSelection &Cancelable( bool val ){	m_bCancelable=val;	return *this;	}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
		virtual void OnGotFocus() override;
		virtual void OnLostFocus() override;
	
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual SimpleSelection &TopLeft( const Vec2i &TL ) override {	m_TopLeft=TL;	UpdateMenuPos();	return *this;	}
		virtual Vec2i Size() const override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void UpdateMenuPos();

	private:
		GenericMenuContent<true> m_Content;
		Menu m_Menu;
		std::wstring m_MsgText;
		Vec2i m_TopLeft;
		int m_W = 480;
		bool m_bCancelable = true;
		std::function<bool(int)> m_OnSelect;
	};

}