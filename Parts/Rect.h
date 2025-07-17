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

	Vec2i &RightBottom(){	return m_RB;	}
	const Vec2i &RightBottom() const {	return m_RB;	}

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
