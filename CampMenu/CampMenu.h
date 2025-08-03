#pragma once

#include "GUI/IGUI.h"
//#include "GUI/Menu.h"
//#include "GUI/MenuContent.h"
//#include "GUI/TextLinePainter.h"
//#include "GUI/ImgPainter.h"
#include "Common/MoneyView.h"
//#include "Common/CharSelMenuContent.h"

//class CMonoBMP;

class PlayData;

/// <summary>
/// �L�����v���j���[
/// �i�X�e�[�^�X�m�F�C�A�C�e���▂�@�̎g�p�����s����ʁj
/// </summary>
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

	class IPage;
	class StatusPage;
	class ItemPage;
	class MagicPage;

	void OnTopLVMenuCursorMoved( int CharOrder, int CmdOrder );
	void OnTopLVMenuSelected( int CharOrder, int CmdOrder );

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	MoneyView m_MoneyView;

	int m_iCurrChar = -1;
	int m_iCurrPage = -1;
	std::unique_ptr< IPage > m_Pages[3];

private:	//���C�A�E�g�p
	//�y�[�W�\���̈�̘g
	static const Rect MainAreaRect;

};

