#pragma once

/// <summary>
/// アイテム等の使用効果の対象範囲
/// </summary>
enum class TgtRange
{
	/// <summary>なし（全く対象を取らないアイテムや使用できないアイテム等）</summary>
	None = 0,

	/// <summary>味方(使用者から見て味方)単体</summary>
	SingleFriend = 0x11,
	/// <summary>敵(使用者から見て敵)単体</summary>
	SingleEnemy = 0x12,

	/// <summary>味方(使用者から見て味方)全体</summary>
	AllFriend = 0x21,
	/// <summary>敵(使用者から見て敵)全体</summary>
	AllEnemy = 0x22
};

inline bool IsForFriend( TgtRange tr ){	return ( (unsigned int)tr & 0x0Fu )==0x01u;	}
inline bool IsForEnemy( TgtRange tr ){	return ( (unsigned int)tr & 0x0Fu )==0x02u;	}

inline bool IsForSingle( TgtRange tr ){	return ( (unsigned int)tr & 0xF0u )==0x10u;	}
inline bool IsForAll( TgtRange tr ){	return ( (unsigned int)tr & 0xF0u )==0x20u;	}
