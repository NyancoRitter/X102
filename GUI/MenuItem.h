#pragma once

#include "Parts/Vec2D.h"
#include "Parts/TupleForEach.h"
#include <string>
#include "GUI_Funcs.h"

class CMonoBMP;

namespace GUI::Menu
{
	/// <summary>
	/// ���j���[���̂P���ڂ̕`���i
	/// 
	/// * �ݒ�Ƃ��ĕ`��F��ێ�
	/// </summary>
	class IMenuItem
	{
	public:
		virtual ~IMenuItem() = default;

		/// <summary>���ڂ̕`��F</summary>
		/// <returns>�`��F</returns>
		virtual COLORREF DrawColor() const = 0;

		/// <summary>���ڂ̕`��F��ύX</summary>
		/// <returns>*this</returns>
		virtual IMenuItem &DrawColor( COLORREF Color ) = 0;

		/// <summary>
		/// ���j���[���ڂ̕`��
		/// </summary>
		/// <param name="hdc">�`��Ώ�hdc</param>
		/// <param name="ItemDrawReg">���ڕ`��͈�</param>
		/// <param name="IsAtCursorPos">���̍��ڂ��J�[�\���ʒu�ɂ��邩�ۂ�</param>
		/// <param name="IsMenuFocused">�������j���[���t�H�[�J�X��Ԃ��ۂ�</param>
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const = 0;
	};

	/// <summary>
	/// �ėp�D
	/// �������̕`��p�[�c�ō\�����ꂽ���j���[���ځD
	/// 
	/// template�Ŏw�肵���p�[�c�Q���������牡���тɕ`�悷��D
	/// �e�p�[�c�̌^�͈ȉ��̃��\�b�h�����K�v������D
	/// * int XSize() const;
	///		�Ӗ��I�ɂ͂��̃p�[�c���������ɐ�߂�T�C�Y[pixel]��Ԃ����́D
	///		�p�[�c�͍����珇�ɕ`�悳��Ă������C���̉ߒ��Ŏ��̃p�[�c�̕`��͈͂����߂�̂ɗp������D
	///		��̓I�ɂ͉E�ׂ�̃p�[�c�̕`��͈́i��left�j�͂��̕������E�ɂ��炳���D
	///		�i�̂ɁC�ł��E�[�̃p�[�c���Ԃ��l�͕`��ɉe����^���Ȃ��j
	/// * void Draw( HDC hdc, const Rect &Reg, COLORREF Color ) const;
	///		�`�揈���D
	///		Reg �̓��j���[���ڂ̕`��͈͂��炱�̃p�[�c���������̃p�[�c�Q����߂�͈͂����������́D
	///		Color�͍��ڂ̕`��F�ł���D
	/// </summary>
	/// <typeparam name="...DrawParts">���ڂ��\������p�[�c�i�`��v�f�j�Q</typeparam>
	template< class ...DrawParts >
	class MenuItem : public IMenuItem
	{
	public:
		using Parts_t = std::tuple< DrawParts... >;

	public:
		MenuItem( const DrawParts& ...Parts )
			: m_Parts( Parts... )
		{}

		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const override
		{
			Rect RestReg = ItemDrawReg;
			TupleForEach(
				m_Parts,
				[hdc, &RestReg, Color=m_Color]( const auto &Part )
				{
					Part.Draw( hdc, RestReg, Color );
					RestReg.TopLeft()[0] += Part.XSize();
				}
			);
		}

		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual MenuItem &DrawColor( COLORREF Color ) override {	m_Color = Color;	return *this;	}

	private:
		Parts_t m_Parts;
		COLORREF m_Color = 0xFFFFFF;
	};

	//--------------------------
	//MenuItem<> �p�̃��j���[���ڃp�[�c����

	/// <summary>���j���[���ڃp�[�c�FText��`�悷��</summary>
	class Text
	{
	public:
		/// <summary>ctor </summary>
		/// <param name="TextU16">�`��Text</param>
		/// <param name="W">�`��͈͂̕�</param>
		Text( const std::wstring &TextU16, int W=0 ) : m_TextU16(TextU16), m_XSize(W) {}

	public:	//as Menu Item Part
		int XSize() const {	return m_XSize;	}
		void Draw( HDC hdc, const Rect &RestReg, COLORREF Color ) const;

	public:	//�`��͈͓��ł̐��������`��ʒu�̐ݒ�F�f�t�H���g�͍�����
		Text &AlignCenter(){	m_XPosFormat=DT_CENTER;	return *this;	}
		Text &AlignLeft(){	m_XPosFormat=DT_LEFT;	return *this;	}
		Text &AlignRight(){	m_XPosFormat=DT_RIGHT;	return *this;	}

	private:
		int m_XSize;
		std::wstring m_TextU16;
		UINT m_XPosFormat = DT_LEFT;	//�f�t�H���g�͍�����
	};

	/// <summary>���j���[���ڃp�[�c�F�A�C�R����`�悷��</summary>
	class Icon
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="spIcon">
		/// �A�C�R���D
		/// �K�{�F�Q�Ɛ�I�u�W�F�N�g�͂��̃I�u�W�F�N�g�����������Ԃ��������ƁD
		/// </param>
		/// <param name="W">
		/// �`��͈͂̕��D
		/// �A�C�R���摜�̃T�C�Y�Ƃ͖��֌W�ɂ��̒l�� XSize() �̖߂�l�ƂȂ�D
		/// ���̒l���w�肵���ꍇ�ɂ́C�A�C�R���̃T�C�Y�ɐݒ肳���D
		/// </param>
		Icon( const CMonoBMP &rIcon, int W=-1 );

	public:	//MenuItemPart
		int XSize() const {	return m_XSize;	}
		void Draw( HDC hdc, const Rect &Reg, COLORREF Color ) const;

	public:
		//�`��͈͓��ł̐��������`��ʒu�̐ݒ�F�f�t�H���g�͍�����
		Icon &AlignCenter();
		Icon &AlignLeft(){	m_XPosOffset=0;	return *this;	}
		Icon &AlignRight();

		//��f�l0�̉ӏ��𓧉߂��邩�ۂ��F�f�t�H���g�͓���
		Icon &TransBit0Part( bool Trans ){	m_TransBit0Part=Trans;	return *this;	}
	private:
		int m_XSize;
		const CMonoBMP &m_rIcon;
		int m_XPosOffset = 0; 
		bool m_TransBit0Part = true;
	};
}