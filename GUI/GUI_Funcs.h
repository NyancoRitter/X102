#pragma once
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// 囲み枠の描画．
	/// 角丸の矩形枠を描画する．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color">枠の描画色</param>
	/// <param name="bFill">trueの場合，枠内を黒で塗りつぶす</param>
	/// <param name="RoundedCornerRadius">角丸部分の半径[pixel]</param>
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, bool bFill=true, int RoundedCornerRadius=5 );

	/// <summary>
	/// メニュー用のカーソル描画．
	/// ItemDrawReg で指定された矩形範囲を塗りつぶす．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="ItemDrawReg">カーソルが指す項目の描画範囲</param>
	/// <param name="IsMenuFocused">メニューがフォーカス状態か否か（描画色に影響）</param>
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused );
}
