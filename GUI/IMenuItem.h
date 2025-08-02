#pragma once

class Rect;

namespace GUI::Menu
{
	/// <summary>
	/// メニュー内の１項目の描画手段
	/// 
	/// * 設定として描画色を保持
	/// </summary>
	class IMenuItem
	{
	public:
		virtual ~IMenuItem() = default;

		/// <summary>項目の描画色</summary>
		/// <returns>描画色</returns>
		virtual COLORREF DrawColor() const = 0;

		/// <summary>項目の描画色を変更</summary>
		/// <returns>*this</returns>
		virtual IMenuItem &DrawColor( COLORREF Color ) = 0;

		/// <summary>
		/// メニュー項目の描画
		/// </summary>
		/// <param name="hdc">描画対象hdc</param>
		/// <param name="ItemDrawReg">項目描画範囲</param>
		/// <param name="IsAtCursorPos">この項目がカーソル位置にあるか否か</param>
		/// <param name="IsMenuFocused">所属メニューがフォーカス状態か否か</param>
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const = 0;
	};
}
