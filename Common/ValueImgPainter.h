#pragma once

#include "GUI/IPainter.h"
#include <vector>
#include <memory>
class CMonoBMP;

//画像を用いて数値を表示する
class ValueImgPainter : public GUI::IPainter
{
public:
	//ctor. 表示値を指定して構築
	ValueImgPainter( unsigned int val );

public:	//IPainter Impl
	virtual ValueImgPainter &TopLeft( const Vec2i &Pos ) override {	m_TopLeft=Pos;	return *this;	}
	virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
	virtual Vec2i Size() const override;
protected:
	virtual void Paint_( HDC hdc ) const override;

public:
	//描画色
	ValueImgPainter &Color( COLORREF Color ){	m_Color=Color;	return *this;	}
	COLORREF Color() const {	return m_Color;	}

	//表示倍率
	ValueImgPainter &MagRate( int Rate ){	m_MagRate=(Rate>=1 ? Rate : 1);	return *this;	}
	int MagRate() const {	return m_MagRate;	}

private:
	std::vector< unsigned char > m_Vals;	//表示値を桁毎にバラシて保持
	Vec2i m_TopLeft;	//描画位置左上[pixel]
	COLORREF m_Color;	//表示色
	int m_MagRate = 1;	//表示倍率

	//
	static std::unique_ptr<CMonoBMP> ms_upValImg;
	static std::unique_ptr<CMonoBMP> ms_upValFrameImg;

	//１桁の数字画像のサイズ
	static constexpr int ValImgW = 14;
	static constexpr int ValImgH = 18;
};

