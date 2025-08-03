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
/// キャンプメニュー
/// （ステータス確認，アイテムや魔法の使用等を行う画面）
/// </summary>
class CampMenu final : public GUI::IGUI
{
public:
	CampMenu( PlayData &rPlayData );
	~CampMenu();

public:	// IGUI Impl
	virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override;
	//virtual void OnGotFocus() override;

	virtual Vec2i TopLeft() const override {	return Vec2i();	}	//※使わないのでDummy実装
	virtual CampMenu &TopLeft( const Vec2i &TL ) override {	return *this;	}	//※使わないのでDummy実装
	virtual Vec2i Size() const override {	return Vec2i();	}	//※使わないのでDummy実装
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

private:	//レイアウト用
	//ページ表示領域の枠
	static const Rect MainAreaRect;

};

