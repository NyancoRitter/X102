#include "Item.h"

#include <map>
//#include <memory>
#include "ActProc/ActEfficacyImpl.h"
#include "RawAmountCalcFuncs.h"

#include "Rnd.h"

namespace GameContent
{
	//STR依存な一般的な武器
	static inline Item STR_Weapon(
		int Per,	//威力計算用のパーセント値
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt,
		//Resource::ItemIconID IconID,
		AttackElement Elem = AttackElement::Physical,
		bool SingleTgt = true
	)
	{
		auto Pow = STRx( Per );
		auto Efficacy = Attack( [Pow]( const ICharacter &C )->auto{	return std::vector<double>{ Pow(C) };	}, EfficacySource::Weapon )
			.Elem( Elem );

		return Item(
			NameU8,
			( SingleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( STRx::PowStr(Per), Elem, SingleTgt ), FlavorTxt );
	}

	//STR依存, 倍率が範囲な武器
	static inline Item STR_Range_Weapon(
		int MinPer,	int MaxPer, //威力計算用のパーセント値
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt,
		//Resource::ItemIconID IconID,
		AttackElement Elem = AttackElement::Physical,
		bool SingleTgt = true
	)
	{
		auto Pow = STRx_Range( MinPer, MaxPer );
		auto Efficacy = Attack( [Pow]( const ICharacter &C )->auto{	return std::vector<double>{ Pow(C) };	}, EfficacySource::Weapon )
			.Elem( Elem );

		return Item(
			NameU8,
			( SingleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( STRx_Range::PowStr(MinPer,MaxPer), Elem, SingleTgt ), FlavorTxt );
	}

	//STR依存, 倍率が高低２パターンな武器
	static inline Item STR_HighLow_Weapon(
		int LowPer,	int HighPer, //威力計算用のパーセント値(高,低)
		std::function<bool(const ICharacter &)> HighConditionFunc,
		const std::wstring &HighConditionStr,
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt,
		//Resource::ItemIconID IconID,
		AttackElement Elem = AttackElement::Physical,
		bool SingleTgt = true
	)
	{
		auto Pow = STRx_HighLow( LowPer, HighPer, HighConditionFunc );
		auto Efficacy = Attack( [Pow]( const ICharacter &C )->auto{	return std::vector<double>{ Pow(C) };	}, EfficacySource::Weapon )
			.Elem( Elem );

		return Item(
			NameU8,
			( SingleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( STRx_HighLow::PowStr(LowPer, HighPer,HighConditionStr), Elem, SingleTgt ), FlavorTxt );
	}

	//MAG依存な一般的な武器
	static inline Item MAG_Weapon(
		int Per,	//威力計算用のパーセント値
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt,
		//Resource::ItemIconID IconID,
		AttackElement Elem = AttackElement::Physical,
		bool SingleTgt = true
	)
	{
		auto Pow = MAGx( Per );
		auto Efficacy = Attack( [Pow]( const ICharacter &C )->auto{	return std::vector<double>{ Pow(C) };	}, EfficacySource::Weapon )
			.Elem( Elem );

		return Item(
			NameU8,
			( SingleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( MAGx::PowStr(Per), Elem, SingleTgt ), FlavorTxt );
	}

	//STR,MAG 両依存な一般的な武器
	static inline Item STR_MAG_Weapon(
		int STR_Per, int MAG_Per,	//威力計算用のパーセント値
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt,
		//Resource::ItemIconID IconID,
		AttackElement Elem = AttackElement::Physical,
		bool SingleTgt = true
	)
	{
		auto Pow = STRx_MAGx( STR_Per, MAG_Per );
		auto Efficacy = Attack( [Pow]( const ICharacter &C )->auto{	return std::vector<double>{ Pow(C) };	}, EfficacySource::Weapon )
			.Elem( Elem );

		return Item(
			NameU8,
			( SingleTgt ? TgtRange::SingleEnemy : TgtRange::AllEnemy ),
			Efficacy
		).Description( AtkDescStr( STRx_MAGx::PowStr(STR_Per, MAG_Per), Elem, SingleTgt ), FlavorTxt );
	}

	//ポーション
	static inline Item Potion(
		int Amount,	//回復量
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt
		//Resource::ItemIconID IconID,
	)
	{
		auto Efficacy = RecoverHP( [Amount]( const ICharacter &C )->auto{	return Amount;	}, EfficacySource::Potion );

		return Item( NameU8, TgtRange::SingleFriend, Efficacy )
			.Description( RecovHPDescStr( std::to_wstring(Amount), true ), FlavorTxt )
			.CanUseOnlyOnce( true );
	}

	//全体回復魔晶
	static inline Item RecovCrystal(
		int Amount,	//回復量
		const std::wstring &NameU8,
		const std::wstring &FlavorTxt
		//Resource::ItemIconID IconID,
	)
	{
		auto Efficacy = RecoverHP( [Amount]( const ICharacter &C )->auto{	return Amount;	} );

		return Item( NameU8, TgtRange::AllFriend, Efficacy )
			.Description( RecovHPDescStr( std::to_wstring(Amount), true ), FlavorTxt )
			.CanUseOnlyOnce( true );
	}

	//------------------------------------
	//アイテム群の具体的な定義
	class ItemCatalog
	{
	public:
		ItemCatalog();
		ItemCatalog( const ItemCatalog & ) = delete;
		ItemCatalog &operator=( const ItemCatalog & ) = delete;
	public:
		const Item *FromID( int ID ) const
		{
			auto i = m_Defs.find( ID );
			if( i == m_Defs.end() )return nullptr;
			return &(i->second);
		}

		// 引数で指定した名称を持つアイテムのID値を返す．
		// ただし，そのようなアイテムが定義されていない場合には負の値を返す．
		int ID_from_Name( const std::wstring &Name ) const
		{
			for( auto i=m_Defs.begin(); i!=m_Defs.end(); ++i )
			{
				if( i->second.Name() == Name )
				{	return i->first;	}
			}
			return -1;
		}

	private:
		void Register( ItemID ID, const Item &Definition ){	m_Defs[ (int)ID ] = Definition;	}
	private:
		std::map< int, Item > m_Defs;	//<ID値, 定義>
	};

	ItemCatalog::ItemCatalog()
	{
		//回復
		Register( ItemID::HandMadePotion,
			Item(
				L"手作りポーション", TgtRange::SingleFriend,
				RecoverHP(
					[]( const ICharacter & )->int{
						int Amount = 0;
						for( int i=0; i<3; ++i ){	Amount += RND().GetInt<int>( 1,6 );	}
						return Amount;
					},
					EfficacySource::Potion
				)
			)
			.Description( L"[単; HP+ 3～18]", L"イリィが趣味で作ったポーション" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(2).ActSpdPercentage( 150 ).CanUseOnlyOnce( true )
		);

		Register( ItemID::Potion_LV0,
			Potion( 6, L"アビト・キアの薬", L"駆け出し冒険者がお守り的に持ち歩くことが多い安価な薬" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(1).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 18, L"クオト・キアの薬", L"効能は低いが独特の甘味が人気" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(3).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 36, L"ミルト・キアの薬", L"値段と効果のバランスが良い回復薬" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(8).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 72, L"エルド・キアの薬", L"効果の信頼性が高いが非常に値が張る" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(20).ActSpdPercentage(150)
		);

		Register( ItemID::HPCrystal_LV1,
			RecovCrystal( 24, L"ラトラ・キアの魔晶", L"微弱な回復魔法が封じられた魔晶片" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price(12).ActSpdPercentage(120)
		);

		Register( ItemID::HPCrystal_LV2,
			RecovCrystal( 48, L"バルフ・キアの魔晶", L"製法が独占されている高価な魔晶" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price(36).ActSpdPercentage(120)
		);

		Register( ItemID::Antidote,
			Item( L"", TgtRange::SingleFriend, CurePoison )
			.Description( L"[単; 毒を回復]", L"とてもにがいことで有名な解毒薬" )
			.ImgID( ResManage::ItemImg::Antidote )
			.Price(6).CanUseOnlyOnce( true )
		);


		//剣
		Register( ItemID::ShortSword,
			STR_Weapon( 120, L"ショートソード", L"威力は低いが素早く攻撃できる短めの剣" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(10).ActSpdPercentage( 1000 )
		);
		Register( ItemID::LongSword,
			STR_Weapon( 160, L"ロングソード", L"大量生産されている一般的な剣" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(25)
		);
		Register( ItemID::Claymore,
			STR_Range_Weapon( 150, 210, L"クレイモア", L"熟練冒険者に愛用者が多い少し大き目の剣" )
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(120)
		);
		Register( ItemID::Flamberge,
			STR_Weapon( 230, L"フランベルジュ", L"見た目の美しさと攻撃力を両立した高級品" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(360)
		);
		Register( ItemID::BastardSword,
			STR_HighLow_Weapon(
				100, 190,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.AGI()*4 );	},
				L"(AGIx4)[%]",
				L"バスタードソード", L"ファナが愛用している中途半端なサイズ感の剣"
			)
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(50)
		);
		Register( ItemID::TwoHandedSword,
			STR_Range_Weapon( 180, 260, L"ツヴァイハンダー", L"扱いが難しい両手持ち用の大剣" )
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(380)
		);

		Register( ItemID::HP_Sword,
			Item(
				L"オーラブレード", TgtRange::AllEnemy,
				Attack( []( const ICharacter &C )->auto{	return std::vector<double>{ C.HP() * 0.2 };	}, EfficacySource::Weapon )
			)
			.Description( L"[全; " + NameStr(AttackElement::Physical) + L"(HPx0.2)]", L"光の刃で広範囲を薙ぎ払うことができる魔法武器" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price( 200 )
		);

		Register( ItemID::SilverSword,
			STR_MAG_Weapon( 120, 100, L"白銀の魔剣", L"美術的価値により高値で取引される長剣" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price( 600 )
		);

		//杖
		Register( ItemID::IryWand,
			MAG_Weapon( 25, L"マナの木の杖", L"イリィが自作した杖" )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(6).ActSpdPercentage( 300 )
		);
		Register( ItemID::IryWand,
			STR_MAG_Weapon( 50, 25, L"魔導士の杖", L"使用者の魔力を打撃力に変換する杖" )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(20)
		);
		Register( ItemID::FireWand,
			STR_MAG_Weapon( 25, 50, L"アビト・カノの杖", L"炎の魔力が封じられた杖", AttackElement::Fire )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::IceWand,
			STR_MAG_Weapon( 25, 50, L"アビト・リズの杖", L"氷の魔力が封じられた杖", AttackElement::Ice )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::ThunderWand,
			STR_MAG_Weapon( 25, 50, L"アビト・ルトの杖", L"雷の魔力が封じられた杖", AttackElement::Thunder )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::DarkRod,
			MAG_Weapon( 100, L"ダークロッド", L"先端の宝玉から魔力の弾丸を打ち出す杖", AttackElement::Physical, false )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(280)
		);

		//メイス
		Register( ItemID::Mace,
			STR_Range_Weapon( 80, 190, L"メイス", L"聖職者に使い手の多い金属製の打撃武器" )
			.ImgID( ResManage::ItemImg::Mace )
			.Price(12)
		);
		Register( ItemID::HeavyMace,
			STR_Range_Weapon( 70, 230, L"ヘビーメイス", L"通常の物よりひと回り大きなメイス" )
			.ImgID( ResManage::ItemImg::Mace )
			.Price(50)
		);

		//斧
		Register( ItemID::BattleAxe,
			STR_HighLow_Weapon(
				100, 200,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.MaxHP() );	},
				L"MaxHP[%]",
				L"バトルアクス", L"力自慢の戦士達に好まれる戦斧"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(40).ActSpdPercentage( 75 )
		);
		Register( ItemID::GreateAxe,
			STR_HighLow_Weapon(
				100, 270,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.HP() + C.STR() );	},
				L"(HP+STR)[%]",
				L"グレートアクス", L"持ち歩くだけでもかなりの腕力が要求される大斧"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(150).ActSpdPercentage( 75 )
		);
		Register( ItemID::BattleHammer,
			STR_HighLow_Weapon(
				50, 320,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.HP() );	},
				L"HP[%]",
				L"バトルハンマー", L"その重さ故に命中精度に難がある鉄製の大槌"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(80).ActSpdPercentage( 50 )
		);

		//攻撃アイテム
		Register( ItemID::FireCrystal,
			Item(
				L"ミルト・カノの魔晶刃", TgtRange::SingleEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 60 } );	}, EfficacySource::Weapon )
					.Elem( AttackElement::Fire )
			)
			.Description( L"[単; " + NameStr(AttackElement::Fire) + L"(60)]", L"炎の魔晶石で形作られた使い捨ての刃" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 18 ).CanUseOnlyOnce( true )
		);
		Register( ItemID::IceCrystal,
			Item(
				L"ピゾナ・リズの魔晶塵", TgtRange::AllEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 10 } );	} )
				.Elem( AttackElement::Ice )
				.WithPoison()
			)
			.Description( L"[全; " + NameStr(AttackElement::Ice) + L"(10)]", L"広範囲にばら撒くことで毒霧の効果を発生させる" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 10 ).CanUseOnlyOnce( true )
		);
		Register( ItemID::ThunderCrystal,
			Item(
				L"バルフ・ルトの魔晶", TgtRange::AllEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 45 } );	} )
				.Elem( AttackElement::Thunder )
			)
			.Description( L"[全; " + NameStr(AttackElement::Thunder) + L"(45)]", L"砕くことで荒れ狂う雷の力が解放される" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 20 ).CanUseOnlyOnce( true )
		);
	}

	//唯一のインスタンス
	static const ItemCatalog Inst;


	//--------------------------

	const Item &Item::Definition( ItemID ID ){	return *Definition( (int)ID );	}
	const Item *Item::Definition( int ID ){	return Inst.FromID(ID);	}
	int Item::ID_from_Name( const std::wstring &Name ){	return Inst.ID_from_Name( Name );	}
}