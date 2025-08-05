#pragma once
#include "AttackElement.h"
#include "ActProc/EfficacySource.h"
#include "ActProc/ActContext.h"
#include <vector>

namespace GameContent
{
	/// <summary>
	/// キャラクタ
	/// 回復や攻撃等の 実行者/対象 の I/F
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

		//ダメージ軽減率[%]
		virtual int DmgReducePercentage( GameContent::AttackElement Elem ) const = 0;
		//毒付与回避率[%]
		virtual int PoisonResistPercentage() const = 0;

	public:	//HP, 状態異常関連
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

	public:	//保持スキル効果関連

		/// <summary>
		/// 自身の行動により起きるHP回復効果量への補正処理
		/// </summary>
		/// <param name="Source">効果発生源情報</param>
		/// <param name="RawAmount">無補正の効果量</param>
		/// <param name="Context">行動文脈情報</param>
		/// <returns>
		/// 補正後のHP回復量．
		/// 複数値を返した場合，連続的に効果を生じる．
		/// </returns>
		virtual std::vector<double> ChangeRecovHPAmount(
			EfficacySource Source,
			double RawAmount,
			const ActContext &Context
		) const
		{	return {	RawAmount	};	}

		/// <summary>
		/// 自身の行動により起きる攻撃の威力への補正処理
		/// </summary>
		/// <param name="Source">効果発生源情報</param>
		/// <param name="Elem">攻撃属性</param>
		/// <param name="RawPow">無補正の攻撃威力</param>
		/// <param name="TgtChar">攻撃対象キャラクタ情報</param>
		/// <param name="Context">行動文脈情報</param>
		/// <returns>補正後の攻撃威力．
		/// 複数値を返した場合，多段攻撃となる．</returns>
		virtual std::vector<double> ChangeAtkPow(
			EfficacySource Source,
			AttackElement Elem,
			double RawPow,
			const ICharacter &TgtChar,
			const ActContext &Context
		) const
		{	return { RawPow };	}
	};

}