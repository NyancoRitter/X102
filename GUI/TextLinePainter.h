#pragma once
#include "IPainter.h"
#include <vector>
#include <string>

namespace GUI
{

	class TextLinePainter : public IPainter
	{
	public:
		TextLinePainter( int LineH = 24 ) : m_LineH(LineH) {	AlignCenter();	}

	public:
		/// <summary>
		/// �`��͈͂̃T�C�Y��ݒ�D
		/// 
		/// �`�悪���̃T�C�Y���ɃN���b�v�����킯�ł͂Ȃ��C�e��v�Z�ɗp�����邾���D
		/// �`�悷�镶�����s�̍����Ȃǂ̐ݒ�͐��������͗��p�����S�ۂ���K�v������D
		/// </summary>
		/// <param name="S">�͈̓T�C�Y</param>
		/// <returns>*this</returns>
		TextLinePainter &Size( const Vec2i &S ){	m_Size=S;	return *this;	}

		//�P�s�̍���[pixel]
		TextLinePainter &LineHeight( int H ){	m_LineH=H;	return *this;	}
		int LineHeight() const {	return m_LineH;	}

		/// <summary>
		/// �\�����ׂ��e�L�X�g���w��D
		/// Size() �� LineHeight() �Ƃ̊Ԃł̐�������ۂi�e�L�X�g�̍s���≡�����`��͈͂Ɏ��܂�悤�ɂ���j���Ƃ͗��p���̐ӔC�D
		/// </summary>
		/// <param name="rTextLinesU16">
		/// �e�L�X�g�f�[�^(UTF16)�D�����s�\������ꍇ�͂��̕������̕�����Q�D
		/// </param>
		/// <returns>*this</returns>
		TextLinePainter &TextLines( const std::vector<std::wstring> &rTextLinesU16 )
		{
			m_TextLinesU16 = rTextLinesU16;
			return *this;
		}

		// �e�L�X�g�̐��������`��ʒu�̐ݒ�
		TextLinePainter &AlignCenter();
		TextLinePainter &AlignLeft();
		TextLinePainter &AlignRight();

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual TextLinePainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}
		virtual Vec2i Size() const override {	return m_Size;	}
	protected:
		/// <summary>�`�揈��</summary>
		/// <param name="hdc">�`��Ώ�</param>
		virtual void Paint_( HDC hdc ) const override;

	private:
		int m_LineH;
		Vec2i m_TopLeft;
		Vec2i m_Size;
		UINT m_HorizontalFormat;
		std::vector< std::wstring > m_TextLinesU16;
	};

}
