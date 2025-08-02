#pragma once

#include "Vec2D.h"
#include <windef.h>

class Rect
{
public:
	explicit Rect( int left=0, int top=0, int W=0, int H=0 ) : Rect( {left,top}, W,H ) {}
	explicit Rect( const Vec2i &TL, int W, int H )
	{
		m_TL = TL;
		SetWidth(W);
		SetHeight(H);
	}

	explicit Rect( const Vec2i &TL, const Vec2i &RB )
		: m_TL(TL), m_RB(RB)
	{}

	/// <summary>
	/// Src Çç∂âEÇ… dx, è„â∫Ç… dy ÇæÇØçLÇ∞ÇΩÇ‡ÇÃÇìæÇÈ
	/// </summary>
	/// <param name="Src"></param>
	/// <param name="dx"></param>
	/// <param name="dy"></param>
	/// <returns></returns>
	static Rect Dilation( const Rect &Src, int dx, int dy )
	{	return Rect( Src.TopLeft()-Vec2i{dx,dy}, Src.Width()+2*dx, Src.Height()+2*dy );	}

	RECT AsRECT() const
	{
		RECT ret;
		ret.left = m_TL[0];
		ret.top = m_TL[1];
		ret.right = m_RB[0];
		ret.bottom = m_RB[1];
		return ret;
	}

	Vec2i &TopLeft(){	return m_TL;	}
	const Vec2i &TopLeft() const {	return m_TL;	}
	int Left() const {	return m_TL[0];	}
	int Top() const {	return m_TL[1];	}

	Vec2i &RightBottom(){	return m_RB;	}
	const Vec2i &RightBottom() const {	return m_RB;	}
	int Right() const {	return m_RB[0];	}
	int Bottom() const {	return m_RB[1];	}

	Rect &Offset( const Vec2i &d )
	{
		m_TL += d;
		m_RB += d;
		return *this;
	}

	int Width() const {	return m_RB[0] - m_TL[0];	}
	int Height() const {	return m_RB[1] - m_TL[1];	}

	Rect &SetWidth( int W ){	m_RB[0] = m_TL[0]+W;	return *this;	}
	Rect &SetHeight( int H ){	m_RB[1] = m_TL[1]+H;	return *this;	}

private:
	Vec2i m_TL;
	Vec2i m_RB;
};
