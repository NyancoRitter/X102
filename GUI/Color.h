#pragma once

namespace GUI::Color
{
	//一般
	constexpr COLORREF White = RGB(255,255,255);	//白
	constexpr COLORREF GrayOut = RGB(128,128,128);	//グレーアウト項目の色

	//メニューカーソルの色
	inline COLORREF MenuCursor( bool Focused ){	return ( Focused ? RGB(0,0,255) : RGB(96,96,96) );	}
}
