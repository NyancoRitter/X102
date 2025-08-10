#pragma once
#include "GUI/IMenuItem.h"
#include "GUI/IMenuContent.h"
#include "GameContent/PartyCharID.h"
#include <vector>

namespace GUI::Menu
{

	/// <summary>
	/// �L�����N�^�̊�摜��HP��Ԃ�\�����郁�j���[����
	/// </summary>
	class CharSelMenuItem : public IMenuItem
	{
	public:
		CharSelMenuItem( GameContent::PartyCharID TgtChar, int HP=0, int MaxHP=0 );
		CharSelMenuItem &HPInfo( int HP, int MaxHP ){	m_HP=HP;	m_MaxHP=MaxHP;	return *this;	}

	public:
		/// <summary>���̍��ڂ��J�[�\���ʒu�ɂ���ꍇ�ɁC�`��ʒu�� Draw() �Ŏw�肳�ꂽ������I�t�Z�b�g�����</summary>
		/// <param name="Offset">�I�t�Z�b�g��</param>
		/// <returns>*this</returns>
		CharSelMenuItem &OffsetWhenCursorPos( const Vec2i &Offset ){	m_OffsetWhenCursorPos=Offset;	return *this;	}

	public:	// IMenuItem Impl
		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual CharSelMenuItem &DrawColor( COLORREF Color ) override {	m_Color=Color;	return *this;	}
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused, bool ShouldDrawCursor ) const override;

	private:
		GameContent::PartyCharID m_TgtChar;
		COLORREF m_Color;
		int m_HP;
		int m_MaxHP;
		Vec2i m_OffsetWhenCursorPos;
	};

	/// <summary>
	/// CharSelMenuItem �^�����ڃf�[�^�^�Ƃ��� IMenuContent �����D
	/// ���ڕ��ѕ����� template �Ŏw��D
	/// </summary>
	template< bool IsVertical >
	class CharSelMenuContent : public MenuContent<IsVertical,CharSelMenuItem>
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="CursorPosItemOffset">�J�[�\���ʒu�ɂ��鍀�ڂ̕`��ʒu���I�t�Z�b�g�����</param>
		/// <param name="ItemDrawSize">���ڂ̕`��T�C�Y���w��</param>
		CharSelMenuContent(
			const Vec2i &CursorPosItemOffset,
			const Vec2i &ItemDrawSize = Vec2i{ 160, 64 }
		)
			: MenuContent<IsVertical,CharSelMenuItem>(ItemDrawSize)
			, m_CursorPosItemOffset( CursorPosItemOffset )
		{}

	public:
		/// <summary>���ڂ�ǉ�</summary>
		/// <param name="TgtChar">�L�����N�^ID</param>
		/// <returns>�ǉ����ꂽ���ڂւ̎Q�Ƃ�Ԃ�</returns>
		CharSelMenuItem &Add( GameContent::PartyCharID TgtChar )
		{
			this->m_Items.emplace_back( TgtChar );
			return this->m_Items.back().OffsetWhenCursorPos( m_CursorPosItemOffset );
		}

	private:
		Vec2i m_CursorPosItemOffset;
	};

}
