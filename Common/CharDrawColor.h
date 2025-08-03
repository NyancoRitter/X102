#pragma once

#include "GUI/Color.h"

constexpr COLORREF CharColor_Down = RGB( 255,0,0 );	//HP<=0 状態
constexpr COLORREF CharColor_PoisonInfected = RGB( 0,160,64 );	//毒状態

/// <summary>
/// キャラクターの状態から描画色を決定
/// </summary>
/// <param name="HP">現在HP</param>
/// <param name="PosionInfected">毒状態か否か</param>
/// <returns>色</returns>
inline COLORREF CharDrawColor( int HP, bool PosionInfected )
{
	if( HP<=0 )return CharColor_Down;
	if( PosionInfected )return CharColor_PoisonInfected;
	return GUI::Color::White;
}
