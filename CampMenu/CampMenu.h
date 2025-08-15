#pragma once

#include "GUI/IGUI.h"
#include "Common/MoneyView.h"
#include "Common/EffectList.h"
#include "ActProc/ActEfficacy.h"
#include "ActProc/TgtRange.h"

//class CMonoBMP;

class PlayData;
class TgtSpecifier;
namespace GameContent
{
	class PartyChar;
	class Magic;
}

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
protected:
	virtual void Paint_( HDC hdc ) const override;

private:
	class TopLVMenu;

	class IPage;
	class StatusPage;
	class ItemPage;
	class MagicPage;

	/// <summary>現在選択されているキャラクタのデータ参照用．</summary>
	/// <returns>該当なしの状態ではnullptrを返す</returns>
	GameContent::PartyChar *CurrSelChar();

	//--- for TopLVMenu
	void OnTopLVMenuCursorMoved( int CharOrder, int CmdOrder );
	void OnTopLVMenuSelected( int CharOrder, int CmdOrder );

	//--- for MagicPage

	/// <summary>MagicPage で魔法が選択された際の処理</summary>
	/// <param name="Magic">
	/// 選択された魔法．
	/// 使用できない魔法を指定した場合には何もしない．
	/// （この場面で使えない魔法である場合や，現在のキャラクタのMP的に使えない場合等）
	/// </param>
	void OnMagicSelected( const GameContent::Magic &Magic );

	//--- for ItemPage

	/// <summary>ItemPage でアイテムが選択された際の処理</summary>
	/// <param name="iOrder">
	/// 選択されたアイテムの（現在のキャラクタの持ち物内での）index．
	/// 範囲外等の有効でない値を渡した場合には何もしない．
	/// </param>
	void OnItemSelected( int iOrder );

	/// <summary>（持ち物整理用処理）引数で指定されたキャラクタ所持品の位置を末尾に回す</summary>
	/// <param name="iOrder">対象アイテムの（現在のキャラクタの持ち物内での）index</param>
	/// <returns>成否．範囲外等の有効でない値を指定した場合には何もせずにfalseを返す</returns>
	bool MoveBackCharItem( int iOrder );

	/// <summary>（持ち物整理用処理）引数で指定されたStock内アイテムの位置を末尾に回す</summary>
	/// <param name="iOrder">対象アイテムの（Stock内での）index</param>
	/// <returns>成否．範囲外等の有効でない値を指定した場合には何もせずにfalseを返す</returns>
	bool MoveBackStockItem( int iOrder );

	/// <summary>キャラクタの所持品をStockに移動する</summary>
	///  <param name="iOrder">対象アイテムの（現在のキャラクタの持ち物内での）index</param>
	/// <returns>成否．範囲外等の有効でない値を指定した場合には何もせずにfalseを返す</returns>
	bool MoveItemFromCharToStock( int iOrder );

	/// <summary>Stock内のアイテムをキャラクタ所持品に移動する</summary>
	///  <param name="iOrder">対象アイテムの（Stock内での）index</param>
	/// <returns>成否．範囲外等の有効でない値を指定した場合には何もせずにfalseを返す</returns>
	bool MoveItemFromStockToChar( int iOrder );

private:
	/// <summary>
	/// アイテムや魔法の使用対象キャラクタ選択用UIをUIスタックにプッシュ
	/// </summary>
	/// <param name="ForAll">対象が全員のものか否か</param>
	/// <param name="Callback">選択用UIに渡すCallback（仕様は TgtSelection を参照 ）</param>
	void PushTgtCharSelector( bool ForAll, const std::function< Flags<GUI::GUIResult>( bool, int ) > &Callback );

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

	EffectList m_EffectList;

private:	//レイアウト用
	//ページ表示領域の枠
	static const Rect MainAreaRect;

};

