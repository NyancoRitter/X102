#pragma once

#include <variant>
#include "TgtSpecifier.h"

//
//アイテムや魔法を使う等，何らかの行動を行った結果，発生した効果
//

/// <summary>HPの変更(回復orダメージ)</summary>
struct HPChanged
{
	HPChanged( TgtSpecifier TgtChar, int Amount, int PrevHP, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), PrevHP( PrevHP ), AfterHP( AfterHP )
	{}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifier TgtChar;

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

/// <summary>毒状態になった</summary>
struct PoisonInfected
{
	PoisonInfected( TgtSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifier TgtChar;
};

/// <summary>毒状態が解消された</summary>
/// <typeparam name="TgtSpecifer">効果発生対象を示す型</typeparam>
struct PoisonCured
{
	PoisonCured( TgtSpecifier TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifier TgtChar;
};

//------------------------------------
	
using ActResult = std::variant<
	HPChanged,
	PoisonInfected,
	PoisonCured
>;
