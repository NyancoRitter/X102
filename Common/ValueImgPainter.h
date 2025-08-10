#pragma once

#include "GUI/IPainter.h"
#include <vector>
#include <memory>
class CMonoBMP;

//�摜��p���Đ��l��\������
class ValueImgPainter : public GUI::IPainter
{
public:
	//ctor. �\���l���w�肵�č\�z
	ValueImgPainter( unsigned int val );

public:	//IPainter Impl
	virtual ValueImgPainter &TopLeft( const Vec2i &Pos ) override {	m_TopLeft=Pos;	return *this;	}
	virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
	virtual Vec2i Size() const override;
protected:
	virtual void Paint_( HDC hdc ) const override;

public:
	//�`��F
	ValueImgPainter &Color( COLORREF Color ){	m_Color=Color;	return *this;	}
	COLORREF Color() const {	return m_Color;	}

	//�\���{��
	ValueImgPainter &MagRate( int Rate ){	m_MagRate=(Rate>=1 ? Rate : 1);	return *this;	}
	int MagRate() const {	return m_MagRate;	}

private:
	std::vector< unsigned char > m_Vals;	//�\���l�������Ƀo���V�ĕێ�
	Vec2i m_TopLeft;	//�`��ʒu����[pixel]
	COLORREF m_Color;	//�\���F
	int m_MagRate = 1;	//�\���{��

	//
	static std::unique_ptr<CMonoBMP> ms_upValImg;
	static std::unique_ptr<CMonoBMP> ms_upValFrameImg;

	//�P���̐����摜�̃T�C�Y
	static constexpr int ValImgW = 14;
	static constexpr int ValImgH = 18;
};

