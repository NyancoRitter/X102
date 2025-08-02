#pragma once
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// 囲み枠の描画．角丸の矩形枠を描画する．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect">矩形</param>
	/// <param name="Color">枠の描画色</param>
	/// <param name="RoundedCornerRadius">角丸部分の半径[pixel]</param>
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, int RoundedCornerRadius=5 );

	/// <summary>
	/// 囲み枠の描画(内部を塗りつぶす版)．角丸の矩形枠を描画する．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="LineColor">枠の描画色</param>
	/// <param name="FillColor">枠内を塗りつぶす色</param>
	/// <param name="RoundedCornerRadius">角丸部分の半径[pixel]</param>
	void DrawFilledFrame( HDC hdc, const Rect &Rect, COLORREF LineColor, COLORREF FillColor, int RoundedCornerRadius=5 );

	/// <summary>
	/// 矩形領域を指定色で塗りつぶす
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect">範囲</param>
	/// <param name="Color">塗りつぶす色</param>
	void FillRectReg( HDC hdc, const Rect &Rect, COLORREF Color );

	/// <summary>
	/// 正三角形の描画
	/// </summary>
	/// <param name="hdc">ペンやブラシが指定済みのもの</param>
	/// <param name="Center">位置（三角形の外接矩形の中心）</param>
	/// <param name="EdgeLength">１辺の長さ</param>
	/// <param name="RotDeg">回転量 (90度単位．この値x90度だけ回転). （0のときの向きは△）</param>
	void DrawTriangle( HDC hdc, const Vec2i &Center, int EdgeLength, unsigned int Rot=0 );

	/// <summary>
	/// メニュー用のカーソル描画．
	/// ItemDrawReg で指定された矩形範囲をフォーカス状態に応じた色で塗りつぶす．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="ItemDrawReg">カーソルが指す項目の描画範囲</param>
	/// <param name="IsMenuFocused">メニューがフォーカス状態か否か（描画色に影響）</param>
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused );

}
