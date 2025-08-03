#pragma once
#include "ICharacter.h"
#include "PartyCharID.h"
#include "Spell.h"
#include "ItemID.h"
#include <array>
#include "Misc/DataVec.h"

namespace GameContent
{
	/// <summary>
	/// パーティキャラクタデータ
	/// </summary>
	class PartyChar : public ICharacter
	{
	public:
		//LV最大値
		inline static constexpr int MaxLV = 999;

	public:
		/// <summary>初期状態で構築（ゲーム新規開始時用）</summary>
		/// <param name="ID">キャラクタID</param>
		PartyChar( PartyCharID ID );
		
		/// <summary> 全データを指定して構築（ゲームデータロード時用）</summary>
		/// <param name="ID">キャラクタID</param>
		/// <param name="LV">LV</param>
		/// <param name="HP">HP</param>
		/// <param name="MP1st">MP</param>
		/// <param name="MP2nd">MP</param>
		/// <param name="PoisonInfected">毒状態か否か</param>
		/// <param name="Items">所有アイテム群</param>
		PartyChar(
			PartyCharID ID,
			int LV,
			int HP,
			const std::array<int, N_FirstSpell> &MP1st,
			const std::array<int, N_SecondSpell> &MP2nd,
			bool PoisonInfected,
			std::vector<ItemID> Items
		);

	public:
		PartyCharID ID() const {	return m_ID;	}
		const std::wstring &Name() const {	return NameStr(m_ID);	}

	public:	//ステータス参照関連
		//現在LV
		virtual int LV() const override {	return m_LV;	}
		//現在HP
		virtual int HP() const override {	return m_HP;	}
		//現在MP
		int MP( FirstSpell Spell ) const {	return m_MP1st[(int)Spell];	}
		int MP( SecondSpell Spell ) const {	return m_MP2nd[(int)Spell];	}

		//毒付与状態か
		virtual bool PoisonInfected() const override {	return m_PoisonInfected;	}

		//指定LVにおける各種ステータス．
		//ただし引数に0以下の値を指定した場合には現在LVでのステータスを返す．
		int MaxHP_at( int LV=0 ) const;
		int MaxMP( FirstSpell Spell, int LV=0 ) const;
		int MaxMP( SecondSpell Spell, int LV=0 ) const;
		int STR_at( int LV=0 ) const;
		int MAG_at( int LV=0 ) const;
		int AGI_at( int LV=0 ) const;

		virtual int MaxHP() const override {	return MaxHP_at(0);	}
		virtual int STR() const override {	return STR_at(0);	}
		virtual int MAG() const override {	return MAG_at(0);	}
		virtual int AGI() const override {	return AGI_at(0);	}

		//ダメージ軽減率[%]
		virtual int DmgReducePercentage( AttackElement Elem ) const override;
		//毒付与回避率[%]
		virtual int PoisonResistPercentage() const override;

	public:	//ステータス変化関連

		/// <summary>LV Up/Down</summary>
		/// <param name="nUP">所望のLV変化量（下げるなら負の値を指定する）</param>
		/// <returns>LVがいくつ変化したのかを返す（LVには 上限/下限 があるため必ずしも引数分だけ上がるわけではない）</returns>
		int ChangeLV( int dLV );

		/// <summary>
		/// HPの増減(回復/ダメージ)．
		/// ただし結果は 上限/下限 を超えない値となる．
		/// </summary>
		/// <param name="dHP">所望の変化量．正ならば回復，負ならダメージ</param>
		virtual void ChangeHP( int dHP ) override;

		/// <summary>
		/// MPを1ずつ減らす（魔法使用時のMP消費）
		/// </summary>
		/// <param name="Spell1st"></param>
		/// <param name="Spell2nd"></param>
		void DecMP( FirstSpell Spell1st, SecondSpell Spell2nd )
		{
			--m_MP1st[ (int)Spell1st ];
			--m_MP2nd[ (int)Spell2nd ];
		}

		/// <summary>
		/// MPを完全回復（宿泊した際の回復）
		/// </summary>
		void FullRecoverMP()
		{
			for( int i=0; i<N_FirstSpell; ++i ){	m_MP1st[i] = MaxMP( (FirstSpell)i );	}
			for( int i=0; i<N_SecondSpell; ++i ){	m_MP2nd[i] = MaxMP( (SecondSpell)i );	}
		}

		/// <summary>毒を付与</summary>
		/// <returns>新たに付与したか否か．毒状態であった場合にはfalse．</returns>
		virtual bool InfectPoison() override {	return ChangePoisonStateTo( true );	}

		/// <summary>毒を回復</summary>
		/// <returns>回復したか否か．毒状態でなかった場合にはfalse．</returns>
		virtual bool CurePoison() override {	return ChangePoisonStateTo( false );	}

	public:	//所有アイテム : ※特にカプセル化しない
		DataVec< ItemID > &Items() {	return m_Items;	}
		const DataVec< ItemID > &Items() const {	return m_Items;	}

	private:
		bool ChangePoisonStateTo( bool State )
		{
			if( m_PoisonInfected == State )return false;
			m_PoisonInfected = State;
			return true;
		}

	private:
		PartyCharID m_ID;

		int m_LV;
		int m_HP;
		std::array<int, N_FirstSpell> m_MP1st;
		std::array<int, N_SecondSpell> m_MP2nd;
		bool m_PoisonInfected = false;

		DataVec< ItemID > m_Items;
	};


	/// <summary>
	/// キャラクタのLVを次のLVに上げるための費用
	/// （ただし，この関数はLV上限の存在を考慮しない）
	/// </summary>
	/// <param name="CurrLV">現在LV</param>
	/// <returns>費用</returns>
	inline int LVUP_Cost( int CurrLV ){	return 1 + CurrLV/2;	}
}
