#include "Item.h"

#include <map>
//#include <memory>
#include "ActProc/ActEfficacyImpl.h"
#include "RawAmountCalcFuncs.h"

#include "Rnd.h"

namespace GameContent
{
	//STR�ˑ��Ȉ�ʓI�ȕ���
	static inline Item STR_Weapon(
		int Per,	//�З͌v�Z�p�̃p�[�Z���g�l
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

	//STR�ˑ�, �{�����͈͂ȕ���
	static inline Item STR_Range_Weapon(
		int MinPer,	int MaxPer, //�З͌v�Z�p�̃p�[�Z���g�l
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

	//STR�ˑ�, �{��������Q�p�^�[���ȕ���
	static inline Item STR_HighLow_Weapon(
		int LowPer,	int HighPer, //�З͌v�Z�p�̃p�[�Z���g�l(��,��)
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

	//MAG�ˑ��Ȉ�ʓI�ȕ���
	static inline Item MAG_Weapon(
		int Per,	//�З͌v�Z�p�̃p�[�Z���g�l
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

	//STR,MAG ���ˑ��Ȉ�ʓI�ȕ���
	static inline Item STR_MAG_Weapon(
		int STR_Per, int MAG_Per,	//�З͌v�Z�p�̃p�[�Z���g�l
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

	//�|�[�V����
	static inline Item Potion(
		int Amount,	//�񕜗�
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

	//�S�̉񕜖���
	static inline Item RecovCrystal(
		int Amount,	//�񕜗�
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
	//�A�C�e���Q�̋�̓I�Ȓ�`
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

		// �����Ŏw�肵�����̂����A�C�e����ID�l��Ԃ��D
		// �������C���̂悤�ȃA�C�e������`����Ă��Ȃ��ꍇ�ɂ͕��̒l��Ԃ��D
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
		std::map< int, Item > m_Defs;	//<ID�l, ��`>
	};

	ItemCatalog::ItemCatalog()
	{
		//��
		Register( ItemID::HandMadePotion,
			Item(
				L"����|�[�V����", TgtRange::SingleFriend,
				RecoverHP(
					[]( const ICharacter & )->int{
						int Amount = 0;
						for( int i=0; i<3; ++i ){	Amount += RND().GetInt<int>( 1,6 );	}
						return Amount;
					},
					EfficacySource::Potion
				)
			)
			.Description( L"[�P; HP+ 3�`18]", L"�C���B����ō�����|�[�V����" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(2).ActSpdPercentage( 150 ).CanUseOnlyOnce( true )
		);

		Register( ItemID::Potion_LV0,
			Potion( 6, L"�A�r�g�E�L�A�̖�", L"�삯�o���`���҂������I�Ɏ����������Ƃ����������Ȗ�" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(1).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 18, L"�N�I�g�E�L�A�̖�", L"���\�͒Ⴂ���Ɠ��̊Ö����l�C" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(3).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 36, L"�~���g�E�L�A�̖�", L"�l�i�ƌ��ʂ̃o�����X���ǂ��񕜖�" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(8).ActSpdPercentage(150)
		);
		Register( ItemID::Potion_LV0,
			Potion( 72, L"�G���h�E�L�A�̖�", L"���ʂ̐M���������������ɒl������" )
			.ImgID( ResManage::ItemImg::Potion )
			.Price(20).ActSpdPercentage(150)
		);

		Register( ItemID::HPCrystal_LV1,
			RecovCrystal( 24, L"���g���E�L�A�̖���", L"����ȉ񕜖��@��������ꂽ������" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price(12).ActSpdPercentage(120)
		);

		Register( ItemID::HPCrystal_LV2,
			RecovCrystal( 48, L"�o���t�E�L�A�̖���", L"���@���Ɛ肳��Ă��鍂���Ȗ���" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price(36).ActSpdPercentage(120)
		);

		Register( ItemID::Antidote,
			Item( L"", TgtRange::SingleFriend, CurePoison )
			.Description( L"[�P; �ł���]", L"�ƂĂ��ɂ������ƂŗL���ȉ�Ŗ�" )
			.ImgID( ResManage::ItemImg::Antidote )
			.Price(6).CanUseOnlyOnce( true )
		);


		//��
		Register( ItemID::ShortSword,
			STR_Weapon( 120, L"�V���[�g�\�[�h", L"�З͂͒Ⴂ���f�����U���ł���Z�߂̌�" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(10).ActSpdPercentage( 1000 )
		);
		Register( ItemID::LongSword,
			STR_Weapon( 160, L"�����O�\�[�h", L"��ʐ��Y����Ă����ʓI�Ȍ�" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(25)
		);
		Register( ItemID::Claymore,
			STR_Range_Weapon( 150, 210, L"�N���C���A", L"�n���`���҂Ɉ��p�҂����������傫�ڂ̌�" )
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(120)
		);
		Register( ItemID::Flamberge,
			STR_Weapon( 230, L"�t�����x���W��", L"�����ڂ̔������ƍU���͂𗼗����������i" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price(360)
		);
		Register( ItemID::BastardSword,
			STR_HighLow_Weapon(
				100, 190,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.AGI()*4 );	},
				L"(AGIx4)[%]",
				L"�o�X�^�[�h�\�[�h", L"�t�@�i�����p���Ă��钆�r���[�ȃT�C�Y���̌�"
			)
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(50)
		);
		Register( ItemID::TwoHandedSword,
			STR_Range_Weapon( 180, 260, L"�c���@�C�n���_�[", L"������������莝���p�̑匕" )
			.ImgID( ResManage::ItemImg::SwordL )
			.Price(380)
		);

		Register( ItemID::HP_Sword,
			Item(
				L"�I�[���u���[�h", TgtRange::AllEnemy,
				Attack( []( const ICharacter &C )->auto{	return std::vector<double>{ C.HP() * 0.2 };	}, EfficacySource::Weapon )
			)
			.Description( L"[�S; " + NameStr(AttackElement::Physical) + L"(HPx0.2)]", L"���̐n�ōL�͈͂�ガ�������Ƃ��ł��閂�@����" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price( 200 )
		);

		Register( ItemID::SilverSword,
			STR_MAG_Weapon( 120, 100, L"����̖���", L"���p�I���l�ɂ�荂�l�Ŏ������钷��" )
			.ImgID( ResManage::ItemImg::Sword )
			.Price( 600 )
		);

		//��
		Register( ItemID::IryWand,
			MAG_Weapon( 25, L"�}�i�̖؂̏�", L"�C���B�����삵����" )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(6).ActSpdPercentage( 300 )
		);
		Register( ItemID::IryWand,
			STR_MAG_Weapon( 50, 25, L"�����m�̏�", L"�g�p�҂̖��͂�Ō��͂ɕϊ������" )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(20)
		);
		Register( ItemID::FireWand,
			STR_MAG_Weapon( 25, 50, L"�A�r�g�E�J�m�̏�", L"���̖��͂�������ꂽ��", AttackElement::Fire )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::IceWand,
			STR_MAG_Weapon( 25, 50, L"�A�r�g�E���Y�̏�", L"�X�̖��͂�������ꂽ��", AttackElement::Ice )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::ThunderWand,
			STR_MAG_Weapon( 25, 50, L"�A�r�g�E���g�̏�", L"���̖��͂�������ꂽ��", AttackElement::Thunder )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(80)
		);
		Register( ItemID::DarkRod,
			MAG_Weapon( 100, L"�_�[�N���b�h", L"��[�̕�ʂ��疂�͂̒e�ۂ�ł��o����", AttackElement::Physical, false )
			.ImgID( ResManage::ItemImg::Wand )
			.Price(280)
		);

		//���C�X
		Register( ItemID::Mace,
			STR_Range_Weapon( 80, 190, L"���C�X", L"���E�҂Ɏg����̑����������̑Ō�����" )
			.ImgID( ResManage::ItemImg::Mace )
			.Price(12)
		);
		Register( ItemID::HeavyMace,
			STR_Range_Weapon( 70, 230, L"�w�r�[���C�X", L"�ʏ�̕����ЂƉ��傫�ȃ��C�X" )
			.ImgID( ResManage::ItemImg::Mace )
			.Price(50)
		);

		//��
		Register( ItemID::BattleAxe,
			STR_HighLow_Weapon(
				100, 200,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.MaxHP() );	},
				L"MaxHP[%]",
				L"�o�g���A�N�X", L"�͎����̐�m�B�ɍD�܂��핀"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(40).ActSpdPercentage( 75 )
		);
		Register( ItemID::GreateAxe,
			STR_HighLow_Weapon(
				100, 270,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.HP() + C.STR() );	},
				L"(HP+STR)[%]",
				L"�O���[�g�A�N�X", L"�������������ł����Ȃ�̘r�͂��v�������啀"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(150).ActSpdPercentage( 75 )
		);
		Register( ItemID::BattleHammer,
			STR_HighLow_Weapon(
				50, 320,
				[](const ICharacter &C)->bool{	return ( RND().GetInt<int>(0,99) < C.HP() );	},
				L"HP[%]",
				L"�o�g���n���}�[", L"���̏d���̂ɖ������x�ɓ����S���̑��"
			)
			.ImgID( ResManage::ItemImg::Axe )
			.Price(80).ActSpdPercentage( 50 )
		);

		//�U���A�C�e��
		Register( ItemID::FireCrystal,
			Item(
				L"�~���g�E�J�m�̖����n", TgtRange::SingleEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 60 } );	}, EfficacySource::Weapon )
					.Elem( AttackElement::Fire )
			)
			.Description( L"[�P; " + NameStr(AttackElement::Fire) + L"(60)]", L"���̖����΂Ō`���ꂽ�g���̂Ă̐n" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 18 ).CanUseOnlyOnce( true )
		);
		Register( ItemID::IceCrystal,
			Item(
				L"�s�]�i�E���Y�̖����o", TgtRange::AllEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 10 } );	} )
				.Elem( AttackElement::Ice )
				.WithPoison()
			)
			.Description( L"[�S; " + NameStr(AttackElement::Ice) + L"(10)]", L"�L�͈͂ɂ΂�T�����ƂœŖ��̌��ʂ𔭐�������" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 10 ).CanUseOnlyOnce( true )
		);
		Register( ItemID::ThunderCrystal,
			Item(
				L"�o���t�E���g�̖���", TgtRange::AllEnemy,
				Attack( []( const ICharacter & )->auto{	return std::vector<double>( { 45 } );	} )
				.Elem( AttackElement::Thunder )
			)
			.Description( L"[�S; " + NameStr(AttackElement::Thunder) + L"(45)]", L"�ӂ����Ƃōr�ꋶ�����̗͂���������" )
			.ImgID( ResManage::ItemImg::Crystal )
			.Price( 20 ).CanUseOnlyOnce( true )
		);
	}

	//�B��̃C���X�^���X
	static const ItemCatalog Inst;


	//--------------------------

	const Item &Item::Definition( ItemID ID ){	return *Definition( (int)ID );	}
	const Item *Item::Definition( int ID ){	return Inst.FromID(ID);	}
	int Item::ID_from_Name( const std::wstring &Name ){	return Inst.ID_from_Name( Name );	}
}