#pragma once

#include "GUI/IGUI.h"
//#include "GUI/Menu.h"
//#include "GUI/MenuContent.h"
//#include "GUI/TextLinePainter.h"
//#include "GUI/ImgPainter.h"
//#include "GUI/Menu.h"
#include "Common/MoneyView.h"
//#include "Common/CharSelMenuContent.h"

//class CMonoBMP;

class PlayData;

class CampMenu final : public GUI::IGUI
{
public:
	CampMenu( PlayData &rPlayData );
	~CampMenu();

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	//virtual void OnGotFocus() override;

	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
	virtual CampMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//���g��Ȃ��̂�Dummy����
	virtual Vec2i Size() const override {	return Vec2i();	}	//���g��Ȃ��̂�Dummy����
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	class TopLVMenu;

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	

	

	MoneyView m_MoneyView;

private:	//���C�A�E�g�p
	//���C���̈�̘g
	static const Rect MainAreaRect;//{ 168, 24, GC_W-2, GC_H-2 };
//	//���C���̈���ɕ\�����镨�̈ʒu
//	static const Vec2i MainAreaContent_TopLeft;//{ 168+16, 24+32 };
//
//
//	//�L�����N�^�I��
//	inline static const Vec2i CharSelUI_TopLeft{4,64};
//	inline static const Vec2i CharSelUI_ItemSize{ 160, 64 };
//	inline static constexpr int CharSelUI_ItemSpacing = 8;
};

