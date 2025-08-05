#include "ActEfficacy.h"
#include "GameContent/ICharacter.h"
#include "Rnd.h"

std::vector< ActResult > RecoverHP::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;
	const auto &ActChar = Context.CharAt( Actor );
	for( auto Specifier : Tgts )
	{
		auto &TgtChar = Context.CharAt( Specifier );

		const auto ActualAmounts = ActChar.ChangeRecovHPAmount( m_RecovSource, m_CalcRawAmount(ActChar), Context );
		for( double d : ActualAmounts )
		{
			const int dHP = std::max( 1, (int)std::round( d ) );	//���Œ�񕜗� 1 ��ۏ�
			const int PrevHP = TgtChar.HP();
			TgtChar.ChangeHP( dHP );
			Results.emplace_back( HPChanged( Specifier, dHP, PrevHP, TgtChar.HP() ) );
		}
	}
	return Results;
}


std::vector< ActResult > CurePoison::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;
	for( auto Specifier : Tgts )
	{
		if( Context.CharAt( Specifier ).CurePoison() )
		{	Results.emplace_back( PoisonCured( Specifier ) );	}
	}
	return Results;
}



std::vector< ActResult > Attack::operator()(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
	) const
{
	std::vector< ActResult > Results;

	auto &ActChar = Context.CharAt( Actor );
	int SuctionAmount = 0;

	for( auto Specifier : Tgts )
	{
		auto &TgtChar = Context.CharAt( Specifier );

		const auto RawPowers = m_CalcRawPower( ActChar );	//��`��̐��̈З�
		for( double RawPow : RawPowers )
		{
			//�X�L�����ʂȂǂ̉e���̉���
			const auto ActualRawPowers = ActChar.ChangeAtkPow( m_AtkSource, m_AtkElem, RawPow, TgtChar, Context );
			for( double Dmg : ActualRawPowers )
			{
				//��U���҂̑ϐ��ɂ��y��
				Dmg *= ( ( 100 - TgtChar.DmgReducePercentage(m_AtkElem) ) / 100.0 );

				//����
				if( Dmg<=10.0 )
				{	Dmg += RND().GetInt<int>( -1, 1 );	}
				else
				{	Dmg *= ( RND().GetInt<int>( 90, 110 ) / 100.0f );	}

				//������
				int IntDmg = (int)std::round( Dmg );

				//�S���_���[�W���ʂ��Ȃ��Ƃ������ƂɂȂ�Ȃ��悤��
				//�_���[�W�v�Z���ʂ�0�ɂȂ����ꍇ�C���m���� 1 �ɏC������
				if( IntDmg==0 ){	IntDmg = RND().GetInt<int>( 0,1 );	}

				//
				if( IntDmg > 0 ){	SuctionAmount += IntDmg;	}

				//�_���[�W����
				const int PrevHP = TgtChar.HP();
				TgtChar.ChangeHP( IntDmg );
				Results.emplace_back( HPChanged( Specifier, IntDmg, PrevHP, TgtChar.HP() ) );

				if( m_WithPoison  &&  RND().GetInt<int>( 1, 100 ) > TgtChar.PoisonResistPercentage() )
				{//�ŕt�^����
					if( TgtChar.InfectPoison() )
					{	Results.emplace_back( PoisonInfected(Specifier) );	}
				}
			}
		}
	}

	if( m_IsSuction  &&  SuctionAmount>0 )
	{//�z������
		const int PrevHP = ActChar.HP();
		ActChar.ChangeHP( SuctionAmount );
		Results.emplace_back( HPChanged( Actor, SuctionAmount, PrevHP, ActChar.HP() ) );
	}

	return Results;
}
