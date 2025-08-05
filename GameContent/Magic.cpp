#include "Magic.h"
#include "ActProc/ActEfficacy.h"
#include "RawAmountCalcFuncs.h"
#include <map>

namespace
{
	using namespace GameContent;

	inline bool IsSingleTgtSpell( FirstSpell Spell1st )
	{	return !( Spell1st==FirstSpell::All_LV1  ||  Spell1st==FirstSpell::All_LV2 );	}


	//HP回復魔法
	//DescTextU8 の手前に効果量表示用の文字列が付与される
	static inline Magic RecovHPMagic(
		int Per,	//回復威力計算用のパーセント値
		FirstSpell Spell1st,
		const std::wstring &FlavorTxt
		)
	{
		const bool IsSignleTgt = IsSingleTgtSpell(Spell1st);

		return Magic(
			Spell1st, SecondSpell::Recover,
			( IsSignleTgt ? TgtRange::SingleFriend : TgtRange::AllFriend ),
			RecoverHP( MAGx(Per) )
		).Description( RecovHPDescStr( MAGx::PowStr(Per), IsSignleTgt ), FlavorTxt );
	}

	//攻撃魔法
	static inline Magic DmgMagic(
		int Per,	//威力計算用のパーセント値
		FirstSpell Spell1st, SecondSpell Spell2nd,
		AttackElement Elem,
		const std::wstring &FlavorTxt
	)
	{
		const bool IsSignleTgt = IsSingleTgtSpell(Spell1st);

		auto Efficacy = Attack( [Per]( const ICharacter &C )->auto{	return std::vector<double>{ MAGx(Per)(C) };	} )
				.Elem( Elem );

		if( Spell1st == FirstSpell::Poison ){	Efficacy.WithPoison();	}

		return Magic(
			Spell1st, Spell2nd,
			( IsSignleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( MAGx::PowStr(Per), Elem, IsSignleTgt ), FlavorTxt );
	}

	static inline Magic FireMagic( int Per, FirstSpell Spell1st, const std::wstring &FlavorTxt )
	{	return DmgMagic( Per, Spell1st, SecondSpell::Fire, AttackElement::Fire, FlavorTxt );	}

	static inline Magic IceMagic( int Per, FirstSpell Spell1st, const std::wstring &FlavorTxt )
	{	return DmgMagic( Per, Spell1st, SecondSpell::Ice, AttackElement::Ice, FlavorTxt );	}

	static inline Magic ThunderMagic( int Per, FirstSpell Spell1st, const std::wstring &FlavorTxt )
	{	return DmgMagic( Per, Spell1st, SecondSpell::Thunder, AttackElement::Thunder, FlavorTxt );	}

	//魔法定義
	class MagicCatalog
	{
	public:
		MagicCatalog()
		{
			RegisterRecovMagics();
			RegisterAttackMagics();
		}
	private:
		MagicCatalog( const MagicCatalog & ) = delete;
		MagicCatalog &operator=( const MagicCatalog & ) = delete;

	public:
		const Magic &FromSpellIndex( FirstSpell i1st, SecondSpell i2nd ) const
		{	return m_Defs.at( MapKey(i1st,i2nd) );	}

	private:
		static int MapKey( FirstSpell i1st, SecondSpell i2nd ){	return (int)i1st * 100 + (int)i2nd;	}

		void Register( const Magic &M ){	m_Defs[ MapKey( M.Spell().first, M.Spell().second ) ] = M;	}
		void RegisterRecovMagics();
		void RegisterAttackMagics();

		std::map< int, Magic > m_Defs;
	};

	void MagicCatalog::RegisterRecovMagics()
	{
		Register( RecovHPMagic( 100, FirstSpell::Single_LV1, L"回復の光を照射する" ) );
		Register( RecovHPMagic( 200, FirstSpell::Single_LV2, L"まぶしい？と思ったらもう回復した後" ) );
		Register( RecovHPMagic( 400, FirstSpell::Single_LV3, L"急速回復するが 後で疲れるという人も" ) );
		Register( RecovHPMagic( 85, FirstSpell::All_LV1, L"光の渦から微かに甘い香りがするとか" ) );
		Register( RecovHPMagic( 240, FirstSpell::All_LV2, L"魔法陣に触れた箇所がしばらく蛍光する" ) );

		Register(
			Magic(
				FirstSpell::Poison, SecondSpell::Recover,
				TgtRange::SingleFriend,
				CurePoison()
			).Description( CurePoisonDescStr(true), L"この魔法による発汗作用を嫌う者も多い" )
		);
	}

	void MagicCatalog::RegisterAttackMagics()
	{
		//※火：単体が強く，範囲が弱い感じ
		Register( FireMagic( 170, FirstSpell::Single_LV1, L"小さな火球を投擲" ) );
		Register( FireMagic( 280, FirstSpell::Single_LV2, L"複数の炎の矢が襲い掛かる" ) );
		Register( FireMagic( 430, FirstSpell::Single_LV3, L"灼熱の大火球を撃ち込む" ) );
		Register( FireMagic( 120, FirstSpell::All_LV1, L"広範囲に火炎を放射" ) );
		Register( FireMagic( 280, FirstSpell::All_LV2, L"目標範囲を瞬時に爆破する" ) );
		Register( FireMagic( 120, FirstSpell::Poison, L"毒の魔力を込めた黒い炎" ) );
		//※氷：単体が弱く，範囲が強い感じ
		Register( IceMagic( 140, FirstSpell::Single_LV1, L"命中箇所を凍結させる魔法弾" ) );
		Register( IceMagic( 230, FirstSpell::Single_LV2, L"瞬時に形成される氷柱に巻き込む" ) );
		Register( IceMagic( 380, FirstSpell::Single_LV3, L"複数の氷塊による衝撃と急速凍結" ) );
		Register( IceMagic( 150, FirstSpell::All_LV1, L"凍てつく冷気を広範囲に浴びせる" ) );
		Register( IceMagic( 340, FirstSpell::All_LV2, L"吹き荒れる氷刃の嵐" ) );
		Register( IceMagic( 100, FirstSpell::Poison, L"有毒な吹雪を局所的に発生させる" ) );
		//※雷：低LV側は弱く，高LV側が強い感じ
		Register( ThunderMagic( 130, FirstSpell::Single_LV1, L"直線的に電撃を放つ" ) );
		Register( ThunderMagic( 250, FirstSpell::Single_LV2, L"無数の電撃の矢を高速連射" ) );
		Register( ThunderMagic( 480, FirstSpell::Single_LV3, L"強大な稲妻で撃ち滅ぼす" ) );
		Register( ThunderMagic( 120, FirstSpell::All_LV1, L"激しく放電する雷球を召喚" ) );
		Register( ThunderMagic( 320, FirstSpell::All_LV2, L"大雷球からの全方位放電で打ちのめす" ) );
		Register( ThunderMagic( 110, FirstSpell::Poison, L"地を這う電撃から毒素を送り込む" ) );
	}	
}

namespace GameContent
{
	const Magic &Magic::Definition( FirstSpell Spell1st, SecondSpell Spell2nd )
	{
		static const MagicCatalog CatalogInst;
		return CatalogInst.FromSpellIndex( Spell1st, Spell2nd );
	}
}
