#pragma once
#include "IPainter.h"
#include "IRectReg.h"
#include <vector>
#include <string>

namespace GUI
{

	/// <summary>
	/// �e�L�X�g�����s���\������
	/// </summary>
	class TextLinePainter : public IPainter, public IRectReg
	{
	public:
		//ctor
		TextLinePainter( int LineW=100, int LineH = 24 );

	public:
		/// <summary>
		/// �s�̕�[pixel] ��ݒ�D
		/// * ���͈̔͂ɕ`�悪�N���b�v�����Ƃ����킯�ł͂Ȃ��F
		///   �`�悷�镶����Ƃ��̐ݒ�Ƃ̌��ˍ����͗��p���ł��܂���������K�v������D
		/// * Size() �̖߂�l�ɗp������
		/// </summary>
		/// <param name="W">��[pixel]</param>
		/// <returns>*this</returns>
		TextLinePainter &LineWidth( int W ){	m_LineW=W;	return *this;	}

		int LineWidth() const {	return m_LineW;	}

		/// <summary>
		/// �P�s�̍���[pixel] ��ݒ�D
		/// * �e�s�͂��̍����͈̔͂�Y�����������낦�ŕ`�悳���D
		/// * Size()�̖߂�l�ɉe���D
		/// </summary>
		/// <param name="H">����[pxiel]</param>
		/// <returns>*this</returns>
		TextLinePainter &LineHeight( int H ){	m_LineH=H;	return *this;	}

		int LineHeight() const {	return m_LineH;	}

		/// <summary>
		/// �\�����ׂ��e�L�X�g���w��D
		/// LineWidth()���̐ݒ�Ƃ̊Ԃł��܂����ƒ�������i�z��`��͈͂ɕ�����`�挋�ʂ����܂�悤�ɂ���j�̂͗��p���̐ӔC�D
		/// </summary>
		/// <typeparam name="TextLineU16Iter">������Q�w��p�C�e���[�^�̌^</typeparam>
		/// <param name="begin">������(�s)�Q�w��</param>
		/// <param name="end">������(�s)�Q</param>
		/// <returns>*this</returns>
		template<class TextLineU16Iter >
		TextLinePainter &TextLines( TextLineU16Iter begin, TextLineU16Iter end )
		{	m_TextLinesU16.assign( begin, end );	return *this;	}

		/// <summary>
		/// �\�����ׂ��e�L�X�g���w��D
		/// LineWidth()���̐ݒ�Ƃ̊Ԃł��܂����ƒ�������i�z��`��͈͂ɕ�����`�挋�ʂ����܂�悤�ɂ���j�̂͗��p���̐ӔC�D
		/// </summary>
		/// <param name="TextLinesU16">�e�L�X�g�f�[�^(UTF16)�D�����s�\������ꍇ�͂��̕������̕�����Q�D</param>
		/// <returns>*this</returns>
		TextLinePainter &TextLines( std::vector<std::wstring> TextLinesU16 )
		{	m_TextLinesU16 = std::move(TextLinesU16);	return *this;	}

		// �e�L�X�g�̐��������`��ʒu�̐ݒ�
		TextLinePainter &AlignCenter();
		TextLinePainter &AlignLeft();
		TextLinePainter &AlignRight();

		/// <summary>�����̕`��F�̎w��</summary>
		/// <param name="col">�`��F</param>
		/// <returns>*this</returns>
		TextLinePainter &SetColor( COLORREF col ){	m_Color = col;	return *this;	}

	public:	// IRectReg Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual TextLinePainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}
		virtual Vec2i Size() const override {	return Vec2i{ m_LineW, m_LineH*(int)m_TextLinesU16.size() };	}
	protected:	// IPainter Impl
		virtual void Paint_( HDC hdc ) const override;

	private:
		int m_LineW;
		int m_LineH;
		Vec2i m_TopLeft;
		UINT m_HorizontalFormat;
		std::vector< std::wstring > m_TextLinesU16;
		COLORREF m_Color;
	};

}
