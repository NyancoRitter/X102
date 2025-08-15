#pragma once
#include "Parts/Vec2D.h"


namespace GUI
{
	/// <summary>������`�悷����</summary>
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

	protected:
		/// <summary>�`�揈��</summary>
		/// <param name="hdc">�`��Ώ�</param>
		virtual void Paint_( HDC hdc ) const = 0;

	private:
		bool m_IsVisible = true;
	};

	
}
