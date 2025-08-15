#pragma once
#include "Parts/Vec2D.h"


namespace GUI
{
	/// <summary>‰½‚©‚ğ•`‰æ‚·‚é‚â‚Â</summary>
	class IPainter
	{
	public:
		virtual ~IPainter() = default;

	public:
		/// <summary>‰Â‹«i•`‰æˆ—‚Ì—LŒø/–³Œøj</summary>
		/// <returns></returns>
		bool Visible() const {	return m_IsVisible;	}

		/// <summary>‰Â‹«i•`‰æˆ—‚Ì—LŒø/–³Œøj‚Ì•ÏX</summary>
		/// <param name="Visiblity"></param>
		/// <returns>*this</returns>
		IPainter &Visible( bool Visiblity ){	m_IsVisible=Visiblity;	return *this;	}

		/// <summary>
		/// •`‰æˆ—D‚½‚¾‚µ Visible() ‚ªfalse‚Ìó‘Ô‚Å‚Í‰½‚à‚µ‚È‚¢
		/// </summary>
		/// <param name="hdc">•`‰æ‘ÎÛ</param>
		void Paint( HDC hdc ) const {	if( Visible() )Paint_(hdc);	}

	protected:
		/// <summary>•`‰æˆ—</summary>
		/// <param name="hdc">•`‰æ‘ÎÛ</param>
		virtual void Paint_( HDC hdc ) const = 0;

	private:
		bool m_IsVisible = true;
	};

	
}
