#pragma once
#include "Parts/Vec2D.h"
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// ������`�悷���D
	/// 
	/// * �ʒu��T�C�Y�Ɋւ��郁�\�b�h������
	/// </summary>
	class IPainter
	{
	public:
		virtual ~IPainter() = default;

	public:
		/// <summary>�����i�`�揈���̗L��/�����j</summary>
		/// <returns></returns>
		bool Visible() const {	return m_IsVisible;	}

		/// <summary>�����i�`�揈���̗L��/�����j�̕ύX</summary>
		/// <param name="Visiblity"></param>
		/// <returns>*this</returns>
		IPainter &Visible( bool Visiblity ){	m_IsVisible=Visiblity;	return *this;	}

		/// <summary>
		/// �`�揈���D������ Visible() ��false�̏�Ԃł͉������Ȃ�
		/// </summary>
		/// <param name="hdc">�`��Ώ�</param>
		void Paint( HDC hdc ) const {	if( Visible() )Paint_(hdc);	}

	public:
		/// <summary>�`��͈́i�ŏ����`�j�̍�����W�̎擾</summary>
		/// <returns>������W</returns>
		virtual Vec2i TopLeft() const = 0;

		/// <summary>�`��͈́i�ŏ����`�j�̍�����W�̐ݒ�</summary>
		/// <param name="TL">���W</param>
		/// <returns>*this</returns>
		virtual IPainter &TopLeft( const Vec2i &TL ) = 0;

		/// <summary>�`��͈́i�ŏ����`�j�̃T�C�Y�擾</summary>
		/// <returns>�T�C�Y</returns>
		virtual Vec2i Size() const = 0;

	protected:
		/// <summary>�`�揈��</summary>
		/// <param name="hdc">�`��Ώ�</param>
		virtual void Paint_( HDC hdc ) const = 0;

	public:	//�w���p
		/// <summary>�`��͈́i�ŏ����`�j�擾</summary>
		/// <returns>�͈�</returns>
		Rect BoundingRect() const
		{
			auto size = Size();
			return Rect( TopLeft(), size[0], size[1] );
		}

		/// <summary>�ʒu���I�t�Z�b�g</summary>
		/// <param name="dPos">�I�t�Z�b�g��</param>
		/// <returns>*this</returns>
		IPainter &Offset( const Vec2i &dPos ){	return TopLeft( TopLeft()+dPos );	}

		/// <summary>x�����ʒu�� �� W �̗̈�̒��S�ɃZ�b�g</summary>
		/// <param name="W">�z�u�͈͕�</param>
		/// <returns>*this</returns>
		IPainter &XCenter( int W ){	return TopLeft( Vec2i{ ( W - Size()[0] )/2, TopLeft()[1] } );	}

		/// <summary>y�����ʒu�� ���� H �̗̈�̒��S�ɃZ�b�g</summary>
		/// <param name="H">�z�u�͈͍���</param>
		/// <returns>*this</returns>
		IPainter &YCenter( int H ){	return TopLeft( Vec2i{ TopLeft()[0], ( H - Size()[1] )/2 } );	}

	private:
		bool m_IsVisible = true;
	};
}
