#pragma once

#include "IPainter.h"

class CMonoBMP;

namespace GUI
{

	/// <summary>
	/// ���m�N���摜��`�悷��
	/// </summary>
	class MonoImgPainter : public IPainter
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="pBMP">�`�悷��摜</param>
		MonoImgPainter( const CMonoBMP *pBMP = nullptr );

	public:
		/// <summary>
		/// �`��Ώۉ摜���w��
		/// </summary>
		/// <param name="pBMP"></param>
		/// <returns></returns>
		MonoImgPainter &SetImg( const CMonoBMP *pBMP ){	m_pBMP = pBMP;	return *this;	}

		/// <summary>
		/// �摜�`��F�̎w��
		/// </summary>
		/// <param name="col">��f�l1�̌��̕`��F</param>
		/// <returns>*this</returns>
		MonoImgPainter &SetColor( COLORREF col ){	m_Color = col;	return *this;	}

		/// <summary>
		/// ��f�l0�̉ӏ��𓧉߂��邩�ۂ��F�f�t�H���g�͓���
		/// </summary>
		/// <param name="Trans">���߂���Ȃ�true</param>
		/// <returns>*this</returns>
		MonoImgPainter &TransBit0Part( bool Trans ){	m_TransBit0Part=Trans;	return *this;	}

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual MonoImgPainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}

		//�`��Ώۉ摜�̃T�C�Y��Ԃ��D�������摜���ݒ肳��Ă��Ȃ��󋵂ł�(0,0)��Ԃ��D
		virtual Vec2i Size() const override;
	protected:
		/// <summary>�`�揈���D�摜���ݒ肳��Ă��Ȃ��ꍇ�ɂ͉������Ȃ�</summary>
		/// <param name="hdc">�`��Ώ�</param>
		virtual void Paint_( HDC hdc ) const override;

	private:
		COLORREF m_Color;
		bool m_TransBit0Part = true;
		const CMonoBMP *m_pBMP;
		Vec2i m_TopLeft;
	};
}
