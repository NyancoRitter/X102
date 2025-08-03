#pragma once
#include "Parts/Vec2D.h"
#include <vector>

class IController;

namespace GUI::Menu
{
	class IMenuItem;

	/// <summary>
	/// ���j���[�̓��e�D
	/// ���ڌQ�ƌ��݃J�[�\���ʒu�̏���ێ��D
	/// 
	///		<remarks>
	///		�����ł������j���[�Ƃ́F
	///		* ���ڂ��i�c����or�����т�)�P�����ɕ���ł������
	///		* ���ڌQ�̂�������J�[�\���Ŏw���������Ƃ��ł���i�ǂ̍��ڂ��w���Ȃ���Ԃ����蓾��j
	///		</remarks>
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
		virtual Vec2i ItemDrawSize() const = 0;

		/// <summary>���j���[���ڐ�</summary>
		/// <returns></returns>
		virtual int nItems() const = 0;

		/// <summary>
		/// ���ڂ��Q�ƁD
		/// index���͈͊O�̏ꍇ�͗�O���o�����
		/// </summary>
		/// <param name="index">����index</param>
		/// <returns>���ڂւ̎Q��</returns>
		virtual const IMenuItem &Item( int index ) const = 0;

		/// <summary>���݂̃J�[�\���ʒu</summary>
		/// <returns>�J�[�\�����w���Ă��鍀�ڂ�index�D���������̒l�́u�ǂ̍��ڂ��w���Ă��Ȃ��v��Ԃł��邱�Ƃ�����</returns>
		virtual int CursorPos() const = 0;

		/// <summary>
		/// �J�[�\���ʒu�̕ύX�D
		/// �J�[�\�������킹���Ȃ��ʒu�i�Ⴆ�Δ͈͊O�F���̒l�⍀�ڌ��ȏ�̒l�j���w�肵���ꍇ�C���̒l�i�ǂ̍��ڂ��w���Ȃ���ԁj�ɐݒ肳���D
		/// </summary>
		/// <param name="pos">�J�[�\�������킹�鍀�ڂ�index</param>
		/// <returns>�J�[�\���ʒu���ω��������ۂ�</returns>
		virtual bool CursorPos( int pos ) = 0;

		/// <summary>�J�[�\���ʒu���P���Ɉړ�</summary>
		/// <returns>�J�[�\���ʒu���ω��������ۂ�</returns>
		virtual bool IncCursorPos() = 0;

		/// <summary>�J�[�\���ʒu���P�O�Ɉړ�</summary>
		/// <returns>�J�[�\���ʒu���ω��������ۂ�</returns>
		virtual bool DecCursorPos() = 0;
	};


	/// <summary>
	/// MenuItem_t �^�����ڃf�[�^�^�Ƃ��� IMenuContent �����̂ЂȌ^�D
	/// ���ڕ��ѕ����� template �Ŏw��D
	/// </summary>
	/// <typeparam name="MenuItem_t">���j���[���ڂ̌^�DIMenuItem����̔h���N���X</typeparam>
	template< bool IsVertical, class MenuItem_t >
	class MenuContent : public IMenuContent
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemDrawSize">���ڂ̕`��T�C�Y���w��</param>
		MenuContent( const Vec2i &ItemDrawSize ) : m_ItemDrawSize(ItemDrawSize) {}

		virtual ~MenuContent() = default;

	public:
		//
		//�����ڂ̒ǉ���i�������̂Ŕh�����Ēǉ�����K�v������
		//

		/// <summary>���ڂ�S�j��</summary>
		void Clear(){	m_Items.clear();	m_CursorPos = -1;	}

		/// <summary>���ڃf�[�^�ێ�vector��reserve()�����{</summary>
		/// <param name="n">�f�[�^�ێ�vector��reserve()�ւ̈���</param>
		void Reserve( size_t n ){	m_Items.reserve(n);	}

		/// <summary>���ڃf�[�^�ւ̃A�N�Z�X</summary>
		/// <param name="index">����index�i0-based, Add()�������j</param>
		/// <returns>���ڃf�[�^�ւ̎Q��</returns>
		MenuItem_t &Item( int index )
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	public:	// IMenuContent Impl
		virtual bool IsVerticalMenu() const override {	return IsVertical;	}
		virtual Vec2i ItemDrawSize() const override {	return m_ItemDrawSize;	}
		virtual int nItems() const override {	return (int)m_Items.size();	}

		virtual int CursorPos() const override {	return m_CursorPos;	}
		virtual bool CursorPos( int pos ) override {	return ChangeCursorPosTo( (0<=pos && pos<nItems())  ?  pos  :  -1 );	}
		virtual bool IncCursorPos() override {	return ChangeCursorPosTo( m_CursorPos+1<nItems()  ?  m_CursorPos+1  :  ( m_Items.empty() ? -1 : 0 ) );	}
		virtual bool DecCursorPos() override {	return ChangeCursorPosTo( m_CursorPos>0  ?  m_CursorPos-1  :  nItems()-1 );	}

		virtual const MenuItem_t &Item( int index ) const override
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	private:
		//�J�[�\���ʒu�ύX�����D
		//�����ʒu�����ݒl�Ɠ��ꂠ��Ή���������false��Ԃ��D
		//�����łȂ��Ȃ�J�[�\���ʒu�������l�ɍX�V����true��Ԃ��D
		bool ChangeCursorPosTo( int NewPos )
		{
			if( m_CursorPos == NewPos )return false;
			m_CursorPos = NewPos;
			return true;
		}
	private:
		Vec2i m_ItemDrawSize;
		int m_CursorPos = -1;
	protected:
		std::vector< MenuItem_t > m_Items;
	};
}
