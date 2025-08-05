#include "Magic.h"
#include "ActProc/ActEfficacy.h"
#include "RawAmountCalcFuncs.h"
#include <map>

namespace
{
	using namespace GameContent;

	inline bool IsSingleTgtSpell( FirstSpell Spell1st )
	{	return !( Spell1st==FirstSpell::All_LV1  ||  Spell1st==FirstSpell::All_LV2 );	}


	//HP�񕜖��@
	//DescTextU8 �̎�O�Ɍ��ʗʕ\���p�̕����񂪕t�^�����
	static inline Magic RecovHPMagic(
		int Per,	//�񕜈З͌v�Z�p�̃p�[�Z���g�l
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

	//�U�����@
	static inline Magic DmgMagic(
		int Per,	//�З͌v�Z�p�̃p�[�Z���g�l
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

	//���@��`
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
		Register( RecovHPMagic( 100, FirstSpell::Single_LV1, L"�񕜂̌����Ǝ˂���" ) );
		Register( RecovHPMagic( 200, FirstSpell::Single_LV2, L"�܂Ԃ����H�Ǝv����������񕜂�����" ) );
		Register( RecovHPMagic( 400, FirstSpell::Single_LV3, L"�}���񕜂��邪 ��Ŕ���Ƃ����l��" ) );
		Register( RecovHPMagic( 85, FirstSpell::All_LV1, L"���̉Q��������ɊÂ����肪����Ƃ�" ) );
		Register( RecovHPMagic( 240, FirstSpell::All_LV2, L"���@�w�ɐG�ꂽ�ӏ������΂炭�u������" ) );

		Register(
			Magic(
				FirstSpell::Poison, SecondSpell::Recover,
				TgtRange::SingleFriend,
				CurePoison()
			).Description( CurePoisonDescStr(true), L"���̖��@�ɂ�锭����p�������҂�����" )
		);
	}

	void MagicCatalog::RegisterAttackMagics()
	{
		//���΁F�P�̂������C�͈͂��ア����
		Register( FireMagic( 170, FirstSpell::Single_LV1, L"�����ȉ΋��𓊝�" ) );
		Register( FireMagic( 280, FirstSpell::Single_LV2, L"�����̉��̖�P���|����" ) );
		Register( FireMagic( 430, FirstSpell::Single_LV3, L"�ܔM�̑�΋�����������" ) );
		Register( FireMagic( 120, FirstSpell::All_LV1, L"�L�͈͂ɉΉ������" ) );
		Register( FireMagic( 280, FirstSpell::All_LV2, L"�ڕW�͈͂��u���ɔ��j����" ) );
		Register( FireMagic( 120, FirstSpell::Poison, L"�ł̖��͂����߂�������" ) );
		//���X�F�P�̂��キ�C�͈͂���������
		Register( IceMagic( 140, FirstSpell::Single_LV1, L"�����ӏ��𓀌������閂�@�e" ) );
		Register( IceMagic( 230, FirstSpell::Single_LV2, L"�u���Ɍ`�������X���Ɋ�������" ) );
		Register( IceMagic( 380, FirstSpell::Single_LV3, L"�����̕X��ɂ��Ռ��Ƌ}������" ) );
		Register( IceMagic( 150, FirstSpell::All_LV1, L"���Ă���C���L�͈͂ɗ��т���" ) );
		Register( IceMagic( 340, FirstSpell::All_LV2, L"�����r���X�n�̗�" ) );
		Register( IceMagic( 100, FirstSpell::Poison, L"�L�łȐ�����Ǐ��I�ɔ���������" ) );
		//�����F��LV���͎キ�C��LV������������
		Register( ThunderMagic( 130, FirstSpell::Single_LV1, L"�����I�ɓd�������" ) );
		Register( ThunderMagic( 250, FirstSpell::Single_LV2, L"�����̓d���̖�������A��" ) );
		Register( ThunderMagic( 480, FirstSpell::Single_LV3, L"����Ȉ�ȂŌ����łڂ�" ) );
		Register( ThunderMagic( 120, FirstSpell::All_LV1, L"���������d���闋��������" ) );
		Register( ThunderMagic( 320, FirstSpell::All_LV2, L"�嗋������̑S���ʕ��d�őł��̂߂�" ) );
		Register( ThunderMagic( 110, FirstSpell::Poison, L"�n�𔇂��d������őf�𑗂荞��" ) );
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
