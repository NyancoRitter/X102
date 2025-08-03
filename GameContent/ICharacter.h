#pragma once

namespace GameContent
{
	enum class AttackElement;

	/// <summary>
	/// キャラクタ
	/// アイテムや攻撃等の対象となるためのIF
	/// </summary>
	class ICharacter
	{
	public:
		virtual ~ICharacter() = default;

	public:	//ステータス参照関連
		//現在LV
		virtual int LV() const = 0;
		//現在HP
		virtual int HP() const = 0;

		//毒付与状態か
		virtual bool PoisonInfected() const = 0;

		virtual int MaxHP() const = 0;
		virtual int STR() const = 0;
		virtual int MAG() const = 0;
		virtual int AGI() const = 0;

		//ダメージ軽減率[%]
		virtual int DmgReducePercentage( AttackElement Elem ) const = 0;
		//毒付与回避率[%]
		virtual int PoisonResistPercentage() const = 0;

	public:	//ステータス変化関連

		/// <summary>
		/// HPの増減(回復/ダメージ)．
		/// ただし結果は 上限/下限 を超えない値となる．
		/// </summary>
		/// <param name="dHP">所望の変化量．正ならば回復，負ならダメージ</param>
		virtual void ChangeHP( int dHP ) = 0;

		/// <summary>毒を付与</summary>
		/// <returns>新たに付与したか否か．毒状態であった場合にはfalse．</returns>
		virtual bool InfectPoison() = 0;

		/// <summary>毒を回復</summary>
		/// <returns>回復したか否か．毒状態でなかった場合にはfalse．</returns>
		virtual bool CurePoison() = 0;
	};

}
