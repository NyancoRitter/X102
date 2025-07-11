#pragma once
#include "Parts/Vec2D.h"
#include "MenuItem.h"

class IController;

namespace GUI::Menu
{
	/// <summary>
	/// ���j���[�̓��e�D
	/// ���ڌQ�ƌ��݃J�[�\���ʒu�̏���ێ��D
	/// </summary>
	class IMenuContent
	{
	public:
		virtual ~IMenuContent() = default;
	public:
		/// <summary>���ڂ̕��ѕ���</summary>
		/// <returns>true�Ȃ�c�Cfalse�Ȃ牡</returns>
		virtual bool IsVerticalMenu() const = 0;

		/// <summary>���ڕ`��T�C�Y�i���j���[�̑S���ڂ͓���T�C�Y�ł���Ƃ����O��j</summary>
		/// <returns></returns>
		virtual Vec2i ItemSize() const = 0;

		//���j���[���ڐ��D
		virtual int nItems() const = 0;

		//���݂̃J�[�\���ʒu
		//���̒l�́u�ǂ̍��ڂ��w���Ă��Ȃ��v��Ԃł��邱�Ƃ������D
		virtual int CursorPos() const = 0;

		//�J�[�\���ʒu�̕ύX
		//�͈͊O�i���̒l�⍀�ڌ��ȏ�̒l�j���w�肵���ꍇ�C���̒l�i�ǂ̍��ڂ��w���Ȃ���ԁj�ɐݒ肳���D
		virtual void CursorPos( int pos ) = 0;

		//���ڂւ̎Q��
		//index���͈͊O�̏ꍇ�͗�O���o�����
		virtual const IMenuItem &Item( int index ) const = 0;
	};

	/// <summary>
	/// ���j���[�̕`�揈���̎����D
	/// * �`��̂��߂̊e��ݒ��ێ��D
	/// </summary>
	class MenuPainter
	{
	public:
		//�`��ʒu
		Vec2i TopLeft() const {	return m_TopLeft;	}
		MenuPainter &TopLeft( const Vec2i &TL ){	m_TopLeft = TL;	return *this;	}

		//�`�掞�ɍ��ڊԂɊJ����Ԋu[pixel]
		int ItemSpacing() const {	return m_ItemSpacing;	}
		MenuPainter &ItemSpacing( int space ){	m_ItemSpacing = space;	return *this;	}

		//�`�掞�ɍ��ڕ`��͈͂̊O���i�㉺���E�j�ɐ݂���]���D
		//* ���ڌQ�̕`��ʒu�� TopLeft() ���炱�̕������I�t�Z�b�g�����D
		//* Size() ���Ԃ��l�͂���������������̂ƂȂ�D
		Vec2i OuterMargin() const {	return m_OuterMargin;	}
		MenuPainter &OuterMargin( const Vec2i &Margin ){	m_OuterMargin = Margin;	return *this;	}
		
		//�`��T�C�Y
		Vec2i TotalSize( const IMenuContent &Content ) const;

		//�O�g�`��̗L��
		bool WithFrame() const {	return m_bDrawFrame;	}
		MenuPainter &WithFrame( bool Enable ){	m_bDrawFrame = Enable;	return *this;	}

		//���j���[�̃t�H�[�J�X��ԁF�`���ɉe��
		bool IsFocused() const {	return m_bFocused;	}
		MenuPainter &IsFocused( bool Focused ){	m_bFocused = Focused;	return *this;	}

		//���ڂ̕`��͈�
		Rect ItemDrawRECT( const IMenuContent &Content, int index ) const;

		//�`��
		void Draw( HDC hdc, const IMenuContent &Content ) const;

	private:
		IMenuContent *m_pContent = nullptr;
		Vec2i m_TopLeft;
		int m_ItemSpacing = 0;
		Vec2i m_OuterMargin;
		bool m_bDrawFrame = false;
		bool m_bFocused = false;
	};


	//
	enum class HandleInputResult
	{
		None,	//�L���ȑ��삪�s���Ȃ������i�����ω������j
		CursorMoved,	//�J�[�\���ʒu���ω�����
		Selected,	//���ڑI�𑀍삪�����ꂽ
		Canceled	//�L�����Z�����삪�����ꂽ
	};

	HandleInputResult HandleInput( IMenuContent &Content, const IController &Controller );
}
