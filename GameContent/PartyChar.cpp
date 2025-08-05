#include "PartyChar.h"
#include <cmath>
#include <algorithm>
#include "AttackElement.h"

namespace
{
	using namespace GameContent;

	//パーティキャラクタ性能定義データ
	class PartyCharDef
	{
	public:
		//LV1時点でのステータスを指定
		PartyCharDef( int MaxHP ) : m_BaseMaxHP(MaxHP) {}
		PartyCharDef &BasicStats( int STR, int MAG, int AGI ){	m_BaseSTR=STR;	m_BaseMAG=MAG;	m_BaseAGI=AGI;	return *this;	}

		//最大MP設定 (引数の意味は MPDef を参照）
		PartyCharDef &MP( FirstSpell Spell, int InitLV, int InitMP, int IncLV=0 ){	m_MP1st[(int)Spell] = MPDef( InitLV,InitMP,IncLV );	return *this;	}
		PartyCharDef &MP( SecondSpell Spell, int InitLV, int InitMP, int IncLV=0 ){	m_MP2nd[(int)Spell] = MPDef( InitLV,InitMP,IncLV );	return *this;	}

		//抵抗値
		PartyCharDef &DmgReducePercentage( const std::array<int,4> &DRP ){	m_DmgReducePercentage=DRP;	return *this;	}
		PartyCharDef &PoisonResistPercentage( int Per ){	m_PoisonRegistPercentage=Per;	return *this;	}

		//アイテム最大保有個数
		PartyCharDef &nMaxItem( int n ){	m_nMaxItem=n;	return *this;	}

		//PartyCharDef &InitItems( const std::vector<ItemID> &Items ){	m_InitItems=Items;	return *this;	}
		//PartyCharDef &Skill( SkillID ID ){	m_Skill=ID;	return *this;	}

	public:
		//指定LVにおけるステータス値
		int MaxHP( int LV ) const {	return StatusVal( LV, m_BaseMaxHP, 30 );	}
		int STR( int LV ) const {	return StatusVal( LV, m_BaseSTR, 60 );	}
		int MAG( int LV ) const {	return StatusVal( LV, m_BaseMAG, 60 );	}
		int AGI( int LV ) const {	return StatusVal( LV, m_BaseAGI, 60 );	}
		int MaxMP( FirstSpell Spell, int LV ) const {	return m_MP1st[(int)Spell].MPAt(LV);	}
		int MaxMP( SecondSpell Spell, int LV ) const {	return m_MP2nd[(int)Spell].MPAt(LV);	}

		//
		int DmgReducePercentage( AttackElement Elem ) const {	return m_DmgReducePercentage[(int)Elem];	}
		int PoisonRegistPercentage() const {	return m_PoisonRegistPercentage;	}
		int nMaxItem() const {	return m_nMaxItem;	}

	private:
		/// <summary>
		/// 特定LVにおける MaxHP, STR, MAG, AGI 値計算用関数．
		/// 単純に線形的に増えていく．
		/// </summary>
		/// <param name="LV">LV</param>
		/// <param name="BaseVal">LV1 でのステータス値</param>
		/// <param name="Factor">ステータス値が BaseVal の倍に達するLV</param>
		/// <returns>指定LVでのステータス値</returns>
		static int StatusVal( int LV, int BaseVal, int Factor )
		{	return (int)std::round( BaseVal * ( Factor + LV - 1 ) / (double)Factor );	}

		//MP定義
		class MPDef
		{
		public:
			//InitLV 未満では MP は 0,
			//InitLV 時点で MP は InitMP,
			//以降，LVが IncLV 上がるごとに MP が 1 増える（ただし InvLV<=0 の場合，MPは増えない）
			MPDef( int InitLV=0, int InitMP=0, int IncLV=0 )
				: m_InitLV{ InitLV }, m_InitMP{ InitMP }, m_IncLV{ IncLV }
			{}

			int MPAt( int LV ) const
			{
				if( LV < m_InitLV )return 0;
				if( m_IncLV<=0 )return m_InitMP;
				return m_InitMP + (LV - m_InitLV)/m_IncLV;
			}
		private:
			int m_InitLV, m_InitMP, m_IncLV;
		};

	private:
		int m_BaseSTR=1;
		int m_BaseMAG=1;
		int m_BaseAGI=1;
		int m_BaseMaxHP=1;
		std::array< MPDef, N_FirstSpell > m_MP1st;
		std::array< MPDef, N_SecondSpell > m_MP2nd;
		std::array<int, 4> m_DmgReducePercentage{0};
		int m_PoisonRegistPercentage = 0;
		int m_nMaxItem = 7;
		//std::vector<ItemID> m_InitItems;
		//SkillID m_Skill = -1;
	};

	//
	const std::array< PartyCharDef, N_PARTY_CHAR_DEF > Defs{
		//エイキンス
		PartyCharDef( 60 ).BasicStats( 15, 1, 2 ).nMaxItem(5)
		.DmgReducePercentage( { 75, 0, 0, -10 } ).PoisonResistPercentage( 5 )
		.MP( FirstSpell::Poison, 3,1 )
		.MP( SecondSpell::Recover, 3,1 ),

		//ファナ
		PartyCharDef( 32 ).BasicStats( 10, 8, 15 ).nMaxItem(6)
		.DmgReducePercentage( { 35, 25, 10, 10 } ).PoisonResistPercentage( 50 )
		.MP( FirstSpell::Single_LV1, 1,3, 8 )
		.MP( FirstSpell::Single_LV2, 5,1, 15 )
		.MP( FirstSpell::All_LV1, 10,1, 18 )
		.MP( FirstSpell::All_LV2, 30,1 )
		.MP( SecondSpell::Recover, 1,3, 12 )
		.MP( SecondSpell::Fire, 1,2, 9 ),

		//ヴィスマト
		PartyCharDef( 44 ).BasicStats( 8, 10, 12 )
		.DmgReducePercentage( { 30, 30, 30, 30 } ).PoisonResistPercentage( 95 )
		.MP( FirstSpell::Single_LV1, 8,1, 20 )
		.MP( FirstSpell::Single_LV2, 20,1, 30 )
		.MP( FirstSpell::Single_LV3, 1,1 )
		.MP( FirstSpell::Poison, 1,8, 6 )
		.MP( SecondSpell::Fire, 1,3, 7 )
		.MP( SecondSpell::Ice, 1,3, 7 )
		.MP( SecondSpell::Thunder, 1,3, 7),

		//リーベル
		PartyCharDef( 42 ).BasicStats( 7, 10, 6 )
		.DmgReducePercentage( { 20, 40, 40, 40 } ).PoisonResistPercentage( 60 )
		.MP( FirstSpell::Single_LV1, 1,12, 4 )
		.MP( FirstSpell::Single_LV2, 7,1, 5 )
		.MP( FirstSpell::Single_LV3, 12,1, 7 )
		.MP( FirstSpell::All_LV1, 1,2, 5 )
		.MP( FirstSpell::All_LV2, 20,1, 4 )
		.MP( FirstSpell::Poison, 1,6, 10 )
		.MP( SecondSpell::Recover, 1,77, 50 ),

		//イリィ
		PartyCharDef( 18 ).BasicStats( 1, 15, 5 )
		.DmgReducePercentage( { 0, 70, 95, 75 } ).PoisonResistPercentage( 75 )
		.MP( FirstSpell::Single_LV1, 1,10, 2 )
		.MP( FirstSpell::Single_LV2, 1,4, 4 )
		.MP( FirstSpell::Single_LV3, 1,1, 7 )
		.MP( FirstSpell::All_LV1, 1,3, 7 )
		.MP( FirstSpell::All_LV2, 1,1, 9 )
		.MP( FirstSpell::Poison, 1,4, 5 )
		.MP( SecondSpell::Recover, 1,7, 5 )
		.MP( SecondSpell::Fire, 1,2, 6 )
		.MP( SecondSpell::Ice, 1,15, 3 )
		.MP( SecondSpell::Thunder, 1,3, 5 ),

		//アスレイ
		PartyCharDef( 30 ).BasicStats( 5, 12, 10 ).nMaxItem(8)
		.DmgReducePercentage( { 10, 65, 50, 65 } ).PoisonResistPercentage( 40 )
		.MP( FirstSpell::Single_LV1, 1,5, 2 )
		.MP( FirstSpell::Single_LV2, 6,1, 4 )
		.MP( FirstSpell::All_LV1, 1,5, 6 )
		.MP( FirstSpell::All_LV2, 8,1, 7 )
		.MP( SecondSpell::Fire, 1,7, 3 )
		.MP( SecondSpell::Ice, 12,1, 10 )
		.MP( SecondSpell::Thunder, 1,6, 3 ),

		//エナ・メア
		PartyCharDef( 98 ).BasicStats( 6, 7, 12 ).nMaxItem(10)
		.DmgReducePercentage( { 12, -30, 0, 30 } ).PoisonResistPercentage( 20 )
		.MP( FirstSpell::All_LV1, 1,2, 4 )
		.MP( FirstSpell::Poison, 10,1, 7 )
		.MP( SecondSpell::Ice, 3,1, 5 )
		.MP( SecondSpell::Thunder, 1,1, 4 ),
	};
}


namespace GameContent
{
	PartyChar::PartyChar( PartyCharID ID )
		: m_ID( ID )
		, m_LV(1)
		, m_Items( Defs[ (int)ID ].nMaxItem() )
	{
		const auto &Def = Defs[ (int)ID ];
		m_HP = MaxHP();
		FullRecoverMP();
	}
		
	PartyChar::PartyChar(
		PartyCharID ID,
		int LV,
		int HP,
		const std::array<int, N_FirstSpell> &MP1st,
		const std::array<int, N_SecondSpell> &MP2nd,
		bool PoisonInfected,
		std::vector<ItemID> Items
	)
		: m_ID( ID )
		, m_LV( LV )
		, m_HP( HP )
		, m_MP1st( MP1st )
		, m_MP2nd( MP2nd )
		, m_PoisonInfected( PoisonInfected )
		, m_Items( Defs[ (int)ID ].nMaxItem() )
	{
		if( !m_Items.Assign( std::move(Items) ) )
		{	throw std::exception("Invalid Items.size()");	}
	}

	int PartyChar::MaxHP_at( int LV ) const {	return Defs[ (int)m_ID ].MaxHP( LV>0 ? LV : m_LV );	}
	int PartyChar::STR_at( int LV ) const {	return Defs[ (int)m_ID ].STR( LV>0 ? LV : m_LV );	}
	int PartyChar::MAG_at( int LV ) const {	return Defs[ (int)m_ID ].MAG( LV>0 ? LV : m_LV );	}
	int PartyChar::AGI_at( int LV ) const {	return Defs[ (int)m_ID ].AGI( LV>0 ? LV : m_LV );	}
	int PartyChar::MaxMP( FirstSpell Spell, int LV ) const {	return Defs[ (int)m_ID ].MaxMP( Spell, ( LV>0 ? LV : m_LV ) );	}
	int PartyChar::MaxMP( SecondSpell Spell, int LV ) const {	return Defs[ (int)m_ID ].MaxMP( Spell, ( LV>0 ? LV : m_LV ) );	}

	int PartyChar::DmgReducePercentage( AttackElement Elem ) const {	return Defs[ (int)m_ID ].DmgReducePercentage( Elem );	}
	int PartyChar::PoisonResistPercentage() const {	return Defs[ (int)m_ID ].PoisonRegistPercentage();	}

	int PartyChar::ChangeLV( int dLV )
	{
		if( dLV==0 )return 0;

		const int PrevLV = m_LV;
		m_LV = std::clamp( m_LV+dLV, 1, MaxLV );
		const int nChanged = m_LV - PrevLV;

		if( nChanged < 0 )
		{//LVダウン時に HP,MP が最大値を超えないように修正．
			m_HP = std::min( m_HP, MaxHP() );
			
			for( int i=0; i<N_FirstSpell; ++i ){	m_MP1st[i] = std::min( m_MP1st[i], MaxMP( (FirstSpell)i ) );	}
			for( int i=0; i<N_SecondSpell; ++i ){	m_MP2nd[i] = std::min( m_MP2nd[i], MaxMP( (SecondSpell)i ) );	}
		}

		return nChanged;
	}

	void PartyChar::ChangeHP( int dHP )
	{
		const int Max = MaxHP();
		const int Min = -5 * Max;
		m_HP = std::clamp( m_HP+dHP, Min, Max );
	}
}
