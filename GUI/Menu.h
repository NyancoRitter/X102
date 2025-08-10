#pragma once

#include "IPainter.h"
#include "HandleMenuInput.h"

namespace GUI::Menu
{
	/// <summary>
	/// ���j���[�̓��͑���ƕ`�揈���̎����D
	/// 
	/// * ����/�`�� �Ώۂ� SetContet() �Ŏw�肷��K�v������D
	///   ���R�C�Ώۂ̎����Ɋւ��钍�ӂ��K�v�D
	/// * �`��̂��߂̊e��ݒ��ێ��D
	/// </summary>
	class Menu : public IPainter
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="pContent">�`��Ώ�</param>
		Menu( IMenuContent *pContent = nullptr ){	SetContent( pContent );	}

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override{	return m_TopLeft;	}
		virtual Menu &TopLeft( const Vec2i &TL ) override {	m_TopLeft = TL;	return *this;	}

		/// <summary>�`��͈̓T�C�Y�D</summary>
		/// <returns>
		/// �T�C�Y�D������ SetContent() �ŗL���ȕ`��Ώۂ��w�肵�Ă��Ȃ���Ԃł͕s��D
		/// </returns>
		virtual Vec2i Size() const override;

	protected:
		/// <summary>
		/// �`��D������ SetContent() �ŗL���ȕ`��Ώۂ��w�肵�Ă��Ȃ���Ԃł͉������Ȃ��D
		/// </summary>
		/// <param name="hdc"></param>
		virtual void Paint_( HDC hdc ) const override;

	public:
		/// <summary>�����Ώۂ̎w��</summary>
		/// <param name="pContent">
		/// �����ΏہD
		/// nullptr���w�肵���ꍇ�C�������̃��\�b�h���Ӗ����Ȃ��Ȃ�����ƂȂ�D
		/// </param>
		/// <returns>*this</returns>
		Menu &SetContent( IMenuContent *pContent )
		{
			m_pContent=pContent;
			m_iDrawBegin=0;
			return *this;
		}

		/// <summary>
		/// �X�N���[����Ԃ����݂̃R���e���c�ɍ��킹�čX�V����D
		/// SetContent() �Ŏw�肳��Ă���R���e���c�̓��e�i���ڐ���J�[�\���ʒu�Ȃǁj��
		/// �O���ŕύX���ꂽ�ꍇ�C�\����Ԃ�����ɍ��킹�邽�߂ɂ�����ĂԕK�v������D
		/// </summary>
		/// <returns>*this</returns>
		Menu &UpdateScrollState();

		/// <summary>���j���[�̑��쏈��</summary>
		/// <param name="Controller">�������</param>
		/// <returns>��������</returns>
		HandleInputResult HandleInput( const IController &Controller );

		//�`�掞�ɍ��ڊԂɊJ����Ԋu[pixel]
		int ItemSpacing() const {	return m_ItemSpacing;	}
		Menu &ItemSpacing( int space ){	m_ItemSpacing = space;	return *this;	}

		//�`�掞�ɍ��ڕ`��͈͂̊O���i�㉺���E�j�ɐ݂���]���D
		//* ���ڌQ�̕`��ʒu�� TopLeft() ���炱�̕������I�t�Z�b�g�����D
		//* Size() ���Ԃ��l�͂���������������̂ƂȂ�D
		Vec2i OuterMargin() const {	return m_OuterMargin;	}
		Menu &OuterMargin( const Vec2i &Margin ){	m_OuterMargin = Margin;	return *this;	}

		//�O�g�`��̗L��
		bool WithFrame() const {	return m_bDrawFrame;	}
		Menu &WithFrame( bool Enable ){	m_bDrawFrame = Enable;	return *this;	}

		//�J�[�\���`��̗L��
		bool CursorVisible() const {	return m_bDrawCursor;	}
		Menu &CursorVisible( bool Visible ){	m_bDrawCursor=Visible;	return *this;	}

		//���j���[�̃t�H�[�J�X��ԁF�`���ɉe��
		bool IsFocused() const {	return m_bFocused;	}
		Menu &IsFocused( bool Focused ){	m_bFocused = Focused;	return *this;	}

		/// <summary>���ڂ̕`��͈�</summary>
		/// <param name="index">����index</param>
		/// <returns>
		/// �`��͈́D������ SetContent() �ŗL���ȕ`��Ώۂ��w�肵�Ă��Ȃ���Ԃł͕s��D
		/// </returns>
		Rect ItemDrawRect( int index ) const;

		// ���ڕ\����̍L���i�\���ł���A�C�e���̌��j�̐ݒ�
		// * 1�ȏ�̒l��ݒ肵���ꍇ�C�R���e���c�̍��ڌ��Ƃ͖��֌W�Ɏw�蕪�̍��ڕ\����̍L�����Ƃ���D
		//   * �R���e���c�̍��ڌ������̍L���������Ȃ��ꍇ�ɂ́C�`�悳��Ȃ��󔒂����܂��D
		//   * �R���e���c�̍��ڌ������̍L�����������ꍇ�ɂ́C�X�N���[������`�̑Ή��ƂȂ�D
		// * 0�ȉ��̒l���w�肵���ꍇ�u�R���e���c�̍��ڌ����v���Ӗ�����D�i�f�t�H���g�l��0�j
		int ItemAreaSpace() const {	return m_ItemAreaSpace;	}
		Menu &ItemAreaSpace( int nItem ){	m_ItemAreaSpace=nItem;	return *this;	}

	private:
		int ActualItemAreaSpace() const;

	private:
		IMenuContent *m_pContent = nullptr;
		Vec2i m_TopLeft;
		int m_ItemSpacing = 0;
		Vec2i m_OuterMargin;
		bool m_bDrawFrame = false;
		bool m_bDrawCursor = true;
		bool m_bFocused = false;

		int m_ItemAreaSpace = 0;
		int m_iDrawBegin = 0;	//���ݕ`�悷��͈͂̍��ڌQ�̐擪
	};
}
