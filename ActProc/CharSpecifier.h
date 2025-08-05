#pragma once

/// <summary>
/// キャラクタ指定用．パーティ内の位置で指定する．
///		<remarks>
///		この型を使う理由は，主に表示系の都合．
///		（表示系の処理では表示場所の決定のために「何番目の～」という情報が欲しい）
///		</remarks>
/// </summary>
struct CharSpecifier
{
	/// <summary>ctor</summary>
	/// <param name="Order">パーティ内でのキャラクタの位置(0-based)</param>
	/// <param name="IsPartySide">味方パーティならtrue, 敵の場合にはfalse</param>
	CharSpecifier( int Order, bool IsPartySide=true )
		: m_Order(Order), m_IsPartySide(IsPartySide)
	{}

	int m_Order;
	bool m_IsPartySide;
};
