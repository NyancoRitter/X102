#pragma once
#include "Parts/Vec2D.h"
#include "MenuItem.h"

class IController;

namespace GUI::Menu
{
	/// <summary>
	/// メニューの内容．
	/// 項目群と現在カーソル位置の情報を保持．
	/// </summary>
	class IMenuContent
	{
	public:
		virtual ~IMenuContent() = default;
	public:
		/// <summary>項目の並び方向</summary>
		/// <returns>trueなら縦，falseなら横</returns>
		virtual bool IsVerticalMenu() const = 0;

		/// <summary>項目描画サイズ（メニューの全項目は同一サイズであるという前提）</summary>
		/// <returns></returns>
		virtual Vec2i ItemSize() const = 0;

		//メニュー項目数．
		virtual int nItems() const = 0;

		//現在のカーソル位置
		//負の値は「どの項目も指していない」状態であることを示す．
		virtual int CursorPos() const = 0;

		//カーソル位置の変更
		//範囲外（負の値や項目個数以上の値）を指定した場合，負の値（どの項目も指さない状態）に設定される．
		virtual void CursorPos( int pos ) = 0;

		//項目への参照
		//indexが範囲外の場合は例外送出される
		virtual const IMenuItem &Item( int index ) const = 0;
	};

}
