#pragma once

#include "GUI/IGUI.h"
#include "Common/MoneyView.h"

#include "ActProc/ActEfficacy.h"
#include "ActProc/TgtRange.h"

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

	//MagicPage で魔法が選択された際の処理
	void OnMagicSelected( const GameContent::Magic &Magic );

	/// <summary>
	/// 魔法/アイテム 共通の使用時効果処理
	/// </summary>
	/// <param name="Efficacy">効果</param>
	/// <param name="Range">効果範囲</param>
	/// <param name="iTgtOrder">用対象キャラクタの位置．Range が示す範囲が単体である場合にのみ使用される</param>
	/// <returns>使用の成否．使用行動自体を棄却べき場合にはfalse．</returns>
	bool Affect( const ActEfficacy &Efficacy, TgtRange Range, int iTgtOrder );

private:
	PlayData &m_rPlayData;

	GUI::GUIStack m_LocalStack;
	MoneyView m_MoneyView;

	int m_iCurrChar = -1;
	int m_iCurrPage = -1;
	std::unique_ptr<TopLVMenu> m_upTopLVMenu;
	std::unique_ptr<StatusPage> m_upStatusPage;
	std::unique_ptr<ItemPage> m_upItemPage;
	std::unique_ptr<MagicPage> m_upMagicPage;
	IPage *m_Pages[3];

private:	//レイアウト用
	//ページ表示領域の枠
	static const Rect MainAreaRect;

};

