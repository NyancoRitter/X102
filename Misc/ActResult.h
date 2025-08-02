#pragma once

#include <variant>

//
//アイテムや魔法を使う等，何らかの行動を行った結果，発生した効果
//

/// <summary>HPの変更(回復orダメージ)</summary>
/// <typeparam name="TgtSpecifer">効果発生対象を示す型</typeparam>
template< class TgtSpecifer >
struct ChangeHP
{
	ChangeHP( TgtSpecifer TgtChar, int Amount, int AfterHP )
		: TgtChar( TgtChar ), Amount( Amount ), AfterHP( AfterHP )
	{}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifer TgtChar;

	//効果量（変化させようとした値）．
	//回復の場合には 1以上の値，ダメージの場合には 0以下の値．
	//（0ダメージはあり得るが，0回復は無い，という想定）
	//
	//例えばアイテムや魔法に定義された回復効果量のような値であり，使用時の主に表示に用いる想定のもの．
	//必ずしも実際にこの値だけHPが変化したわけではない．
	//（例えば，HPを100回復させるアイテムを使用したとしても，HPは最大HPの30にしかならなかったかもしれない）

	/// <summary>
	/// 効果量（変化させようとした値）
	/// 例えばアイテムや魔法に定義された回復効果量のような値．
	/// 必ずしも実際にこの値だけHPが変化したわけではない．
	/// 
	/// * 回復の場合には 1以上の値，ダメージの場合には 0以下の値．
	///   （0ダメージはあり得るが，0回復は無い，という想定）
	/// </summary>
	int Amount;

	/// <summary>効果発揮処理後の対象のHP</summary>
	int AfterHP;
};

/// <summary>毒状態になった</summary>
/// <typeparam name="TgtSpecifer">効果発生対象を示す型</typeparam>
template< class TgtSpecifer >
struct PoionInfected
{
	PoionInfected( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifer TgtChar;
};

/// <summary>毒状態が解消された</summary>
/// <typeparam name="TgtSpecifer">効果発生対象を示す型</typeparam>
template< class TgtSpecifer >
struct PoionCured
{
	PoionCured( TgtSpecifer TgtChar ) : TgtChar( TgtChar ) {}

	/// <summary>対象キャラクタ</summary>
	TgtSpecifer TgtChar;
};

//------------------------------------
	
/// <summary>
/// 非戦闘時用．
/// 
/// 効果対象はパーティメンバーのみなので，
/// 各型の TgtSpecifer は，パーティ内での対象キャラクタの位置(並び順, 0-based) を示す int ．
/// </summary>
using PartyActResult = std::variant<
	ChangeHP<int>,
	PoionCured<int>
>;
