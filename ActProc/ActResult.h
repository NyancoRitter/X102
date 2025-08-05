#pragma once

#include <variant>
#include "CharSpecifier.h"

//
//アイテムや魔法を使う等，何らかの行動を行った結果，発生した効果
//

/// <summary>
/// 発生した(回復orダメージ)の情報．
/// （ただし実際にはHPが変化しなかった場合もあり得る）
/// </summary>
struct HPChanged
{
	HPChanged( CharSpecifier TgtChar, int Amount, int PrevHP, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), PrevHP( PrevHP ), AfterHP( AfterHP )
	{}

	/// <summary>対象キャラクタ</summary>
	CharSpecifier TgtChar;

	/// <summary>
	/// 効果量（変化させようとした値）
	/// 例えばアイテムや魔法に定義された回復効果量のような値．
	/// 必ずしも実際にこの値だけHPが変化したわけではない．
	/// 
	/// * 回復の場合には 1以上の値，ダメージの場合には 0以下の値．
	///   （0ダメージはあり得るが，0回復は無い，という想定）
	/// </summary>
	int Amount;

	/// <summary>効果発揮処理前の対象のHP</summary>
	int PrevHP;

	/// <summary>効果発揮処理後の対象のHP</summary>
	int AfterHP;
};

/// <summary>キャラクタが毒状態になった旨を示す情報</summary>
struct PoisonInfected
{
	PoisonInfected( CharSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	CharSpecifier TgtChar;
};

/// <summary>キャラクタの毒状態が解消された旨を示す情報</summary>
struct PoisonCured
{
	PoisonCured( CharSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	CharSpecifier TgtChar;
};

//------------------------------------
	
using ActResult = std::variant<
	HPChanged,
	PoisonInfected,
	PoisonCured
>;
