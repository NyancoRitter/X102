#pragma once

#include "CharSpecifier.h"

namespace GameContent{	class ICharacter;	}

class ActContext
{
public:
	virtual ~ActContext() = default;

	/// <summary>
	/// CharSpecifier に対応する ICharacter へのアクセス提供手段
	/// </summary>
	virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const = 0;

	/// <summary>
	/// （戦闘中の）現在のターン
	/// </summary>
	/// <returns>
	/// 現在のターン(最初のターンを1とする)．
	/// ただし，戦闘中でない場合には 0 以下の値．
	/// </returns>
	virtual int CurrentTurn() const {	return -1;	}
	bool DuringBattle() const {	return CurrentTurn()<=0;	}
};
