#pragma once
#include "GUI/TextLinePainter.h"

/// <summary>
/// Š‹à•\¦ˆæ
/// </summary>
class MoneyView : public GUI::IPainter, public GUI::IRectReg
{
public:
	MoneyView();

	//•\¦‚·‚é‹àŠz‚ğİ’è
	MoneyView &SetMoney( int money );

public:	// IRectReg Impl
	virtual Vec2i TopLeft() const override {	return m_TextView.TopLeft();	}
	virtual MoneyView &TopLeft( const Vec2i &TL ) override {	m_TextView.TopLeft( TL );	return *this;	}
	virtual Vec2i Size() const override {	return m_TextView.Size() + Vec2i{ ms_RPadding,0};	}
protected:	// IPainter Impl
	virtual void Paint_( HDC hdc ) const override;

private:
	GUI::TextLinePainter m_TextView;
	static constexpr int ms_RPadding = 8;
};
