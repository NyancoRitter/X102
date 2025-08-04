#pragma once

namespace GameContent{	enum class AttackElement;	}

/// <summary>
/// キャラクタ
/// 回復や攻撃等の対象
/// </summary>
class ICharacter
{
public:
	virtual ~ICharacter() = default;

public:	//各種ステータス値
	//※実行値を返す：何らかの効果で増減しているならその値を返す

	virtual int MaxHP() const = 0;
	virtual int STR() const = 0;
	virtual int MAG() const = 0;
	virtual int AGI() const = 0;
	virtual int DmgReducePercentage( GameContent::AttackElement Elem ) const = 0;
	//毒付与回避率[%]
	virtual int PoisonResistPercentage() const = 0;

public:
	//現在HP
	virtual int HP() const = 0;

	/// <summary>
	/// HPの増減(回復/ダメージ)．
	/// ただし結果は 上限/下限 を超えない値となる．
	/// </summary>
	/// <param name="dHP">所望の変化量．正ならば回復，負ならダメージ</param>
	virtual void ChangeHP( int dHP ) = 0;

	//毒付与状態か
	virtual bool PoisonInfected() const = 0;

	/// <summary>毒を付与</summary>
	/// <returns>新たに付与したか否か．毒状態であった場合にはfalse．</returns>
	virtual bool InfectPoison() = 0;

	/// <summary>毒を回復</summary>
	/// <returns>回復したか否か．毒状態でなかった場合にはfalse．</returns>
	virtual bool CurePoison() = 0;
};
