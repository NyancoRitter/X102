#pragma once

/// <summary>
/// キャラクタ指定用．
/// パーティ内の位置で指定する．
/// </summary>
struct TgtSpecifier
{
	/// <summary>ctor</summary>
	/// <param name="Order">パーティ内でのキャラクタの位置(0-based)</param>
	/// <param name="IsPartySide">味方パーティならtrue, 敵の場合にはfalse</param>
	TgtSpecifier( int Order, bool IsPartySide=true )
		: m_Order(Order), m_IsPartySide(IsPartySide)
	{}

	int m_Order;
	bool m_IsPartySide;
};
