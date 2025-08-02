#pragma once

#include "IMenuItem.h"
#include "IMenuContent.h"
#include <functional>
#include <string>

class CMonoBMP;

namespace GUI::Menu
{
	/// <summary>
	/// �ėp IMenuItem �����D
	/// �������̕`��p�[�c�ō\�����ꂽ���j���[���ځD
	/// 
	/// ctor�Ŏw�肵���p�[�c�Q���������牡���тɕ`�悷��D
	/// �e�p�[�c�̌^�͈ȉ��̌Ăяo���ŕ`�揈�����\�Ȃ��́D
	///		
	/// int ( HDC hdc, const Rect &Reg, COLORREF Color ) const;
	///		Reg �̓��j���[���ڂ̕`��͈͂��炱�̃p�[�c���������̃p�[�c�Q����߂�͈͂����������́D
	///		Color�͍��ڂ̕`��F�ł���D
	/// 
	///		�߂�l�́C�Ӗ��I�ɂ͂��̃p�[�c���������ɐ�߂�T�C�Y[pixel]��Ԃ����́D
	///		�p�[�c�͍����珇�ɕ`�悳��Ă������C���̉ߒ��Ŏ��̃p�[�c�̕`��͈͂����߂�̂ɗp������D
	///		��̓I�ɂ͉E�ׂ�̃p�[�c�̕`��͈́i��left�j�͂��̕������E�ɂ��炳���D
	///		�i�̂ɁC�ł��E�[�̃p�[�c���Ԃ��l�͕`��ɉe����^���Ȃ��j
	/// </summary>
	class GenericMenuItem : public IMenuItem
	{
	public:
		using PartFunc = std::function< int( HDC, const Rect &, COLORREF ) >;

		/// <summary>ctor</summary>
		/// <param name="Parts">�p�[�c�Q���w��</param>
		GenericMenuItem( std::initializer_list<PartFunc> Parts );

	public:	// IMenuItem Impl
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const override;
		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual GenericMenuItem &DrawColor( COLORREF Color ) override {	m_Color = Color;	return *this;	}

	private:
		std::vector< PartFunc > m_Parts;
		COLORREF m_Color;
	};

	/// <summary>
	/// GenericMenuItem �^�����ڃf�[�^�^�Ƃ��� IMenuContent �����D
	/// ���ڕ��ѕ����� template �Ŏw��D
	/// </summary>
	template< bool IsVertical >
	class GenericMenuContent : public MenuContent<IsVertical,GenericMenuItem>
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemDrawSize">���ڂ̕`��T�C�Y���w��</param>
		GenericMenuContent( const Vec2i &ItemDrawSize )
			: MenuContent<IsVertical,GenericMenuItem>(ItemDrawSize)
		{}

		virtual ~GenericMenuContent() = default;

	public:
		/// <summary>���ڂ�ǉ� </summary>
		/// <param name="...ItemParts">���ڍ\���v�f�Q</param>
		/// <returns>*this</returns>
		GenericMenuContent &Add( std::initializer_list< GenericMenuItem::PartFunc > ItemParts )
		{
			this->m_Items.emplace_back( ItemParts );
			return *this;
		}
	};

	//------------------------------------

	/// <summary>���j���[���ڃp�[�c�FText��`�悷��</summary>
	class Text
	{
	public:
		/// <summary>ctor </summary>
		/// <param name="TextU16">�`��Text</param>
		/// <param name="W">�`��͈͂̕�. 0�ȉ����w�肵���ꍇ�� Draw() �̈����͈͂����̂܂ܕ`��͈͂Ƃ���</param>
		Text( const std::wstring &TextU16, int W=0 ) : m_TextU16(TextU16), m_XSize(W) {}

	public:	//as Menu Item Part
		int operator()( HDC hdc, const Rect &RestReg, COLORREF Color ) const;

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
		int operator()( HDC hdc, const Rect &Reg, COLORREF Color ) const;

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