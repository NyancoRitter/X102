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

}
