#pragma once

namespace GUI
{
	/// <summary>
	/// 描画色
	/// </summary>
	class Color
	{
	private:
		Color() = delete;
	public:
		constexpr static COLORREF White = RGB(255,255,255);	//白
		constexpr static COLORREF GrayOut = RGB(128,128,128);	//グレーアウト項目の色

		//メニューカーソルの色
		inline static COLORREF MenuCursor( bool Focused ){	return ( Focused ? RGB(0,0,255) : GrayOut );	}
	};
}
