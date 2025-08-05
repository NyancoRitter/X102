#pragma once
#include "PartyCharID.h"
#include "ICharacter.h"
#include "Spell.h"
#include "ItemID.h"
#include <array>
#include "Misc/DataVec.h"

namespace GameContent
{
	enum class AttackElement;

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

	public:	// ICharacter Impl
		virtual int MaxHP() const override {	return MaxHP_at(0);	}
		virtual int STR() const override {	return STR_at(0);	}
		virtual int MAG() const override {	return MAG_at(0);	}
		virtual int AGI() const override {	return AGI_at(0);	}

		virtual int DmgReducePercentage( AttackElement Elem ) const override;
		virtual int PoisonResistPercentage() const override;

		virtual int HP() const override {	return m_HP;	}
		virtual void ChangeHP( int dHP ) override;

		virtual bool PoisonInfected() const override {	return m_PoisonInfected;	}
		virtual bool InfectPoison() override {	return ChangePoisonStateTo( true );	}
		virtual bool CurePoison() override {	return ChangePoisonStateTo( false );	}

#if false  //TODO : スキル効果未実装
		virtual std::vector<double> ChangeRecovHPAmount(
			EfficacySource Source,
			double RawAmount,
			const ActContext &Context
		) const override;

		virtual std::vector<double> ChangeAtkPow(
			EfficacySource Source,
			AttackElement Elem,
			double RawPow,
			const ICharacter &TgtChar,
			const ActContext &Context
		) const override;
#endif

	public:
		//TODO : Add ステータス画面に表示する用の保持スキル情報を返す処理

	public:
		PartyCharID ID() const {	return m_ID;	}
		const std::wstring &Name() const {	return NameStr(m_ID);	}

		//現在LV
		int LV() const {	return m_LV;	}
		//現在MP
		int MP( FirstSpell Spell ) const {	return m_MP1st[(int)Spell];	}
		int MP( SecondSpell Spell ) const {	return m_MP2nd[(int)Spell];	}

		//指定LVにおける各種ステータス．
		//ただし引数に0以下の値を指定した場合には現在LVでのステータスを返す．
		int MaxHP_at( int LV=0 ) const;
		int STR_at( int LV=0 ) const;
		int MAG_at( int LV=0 ) const;
		int AGI_at( int LV=0 ) const;
		int MaxMP( FirstSpell Spell, int LV=0 ) const;
		int MaxMP( SecondSpell Spell, int LV=0 ) const;

		/// <summary>LV Up/Down</summary>
		/// <param name="nUP">所望のLV変化量（下げるなら負の値を指定する）</param>
		/// <returns>LVがいくつ変化したのかを返す（LVには 上限/下限 があるため必ずしも引数分だけ上がるわけではない）</returns>
		int ChangeLV( int dLV );

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
