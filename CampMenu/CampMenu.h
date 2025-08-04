#pragma once

#include <functional>
#include "GUI/IGUI.h"
//#include "GUI/Menu.h"
//#include "GUI/MenuContent.h"
//#include "GUI/TextLinePainter.h"
//#include "GUI/ImgPainter.h"
#include "Common/MoneyView.h"
//#include "Common/CharSelMenuContent.h"

//class CMonoBMP;

class PlayData;
class TgtSpecifier;
namespace GameContent{	class Magic;	}

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

	/// <summary>
	/// アイテムや魔法の使用対象キャラクタ選択用UIをUIスタックにプッシュ
	/// </summary>
	/// <param name="ForAll">対象が全員のものか否か</param>
	/// <param name="Callback">選択用UIに渡すCallback（仕様は TgtSelection を参照 ）</param>
	void PushTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback );

	/// <summary>
	/// 魔法の使用処理
	/// * 使用者は暗黙的に現在されているキャラクタ( m_iCurrChar )となる．
	/// * MP的に使えるか否か等のチェックもここで行われる
	/// </summary>
	/// <param name="Magic">使用する魔法</param>
	/// <param name="iTgtOrder">使用対象キャラクタの位置．魔法効果範囲が単体である場合にのみ使用される</param>
	void UseMagic( const GameContent::Magic &Magic, int iTgtOrder );

	////
	//PartyChar &ToChar( const TgtSpecifier &tgt );

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	MoneyView m_MoneyView;

	int m_iCurrChar = -1;
	int m_iCurrPage = -1;
	std::unique_ptr<TopLVMenu> m_upTopLVMenu;
	std::unique_ptr< IPage > m_Pages[3];

private:	//レイアウト用
	//ページ表示領域の枠
	static const Rect MainAreaRect;

};

