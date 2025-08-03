#pragma once
#include "PartyCharID.h"
#include "Spell.h"
#include "ItemID.h"
#include <array>
#include "Misc/DataVec.h"

namespace GameContent
{
	enum class AttackElement;

	/// <summary>
	/// �p�[�e�B�L�����N�^�f�[�^
	/// </summary>
	class PartyChar
	{
	public:
		//LV�ő�l
		inline static constexpr int MaxLV = 999;

	public:
		/// <summary>������Ԃō\�z�i�Q�[���V�K�J�n���p�j</summary>
		/// <param name="ID">�L�����N�^ID</param>
		PartyChar( PartyCharID ID );
		
		/// <summary> �S�f�[�^���w�肵�č\�z�i�Q�[���f�[�^���[�h���p�j</summary>
		/// <param name="ID">�L�����N�^ID</param>
		/// <param name="LV">LV</param>
		/// <param name="HP">HP</param>
		/// <param name="MP1st">MP</param>
		/// <param name="MP2nd">MP</param>
		/// <param name="PoisonInfected">�ŏ�Ԃ��ۂ�</param>
		/// <param name="Items">���L�A�C�e���Q</param>
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

	public:	//�X�e�[�^�X�Q�Ɗ֘A
		//����LV
		int LV() const {	return m_LV;	}
		//����HP
		int HP() const {	return m_HP;	}
		//����MP
		int MP( FirstSpell Spell ) const {	return m_MP1st[(int)Spell];	}
		int MP( SecondSpell Spell ) const {	return m_MP2nd[(int)Spell];	}

		//�ŕt�^��Ԃ�
		bool PoisonInfected() const {	return m_PoisonInfected;	}

		//�w��LV�ɂ�����e��X�e�[�^�X�D
		//������������0�ȉ��̒l���w�肵���ꍇ�ɂ͌���LV�ł̃X�e�[�^�X��Ԃ��D
		int MaxHP( int LV=0 ) const;
		int MaxMP( FirstSpell Spell, int LV=0 ) const;
		int MaxMP( SecondSpell Spell, int LV=0 ) const;
		int STR( int LV=0 ) const;
		int MAG( int LV=0 ) const;
		int AGI( int LV=0 ) const;

		//�_���[�W�y����[%]
		int DmgReducePercentage( AttackElement Elem ) const;
		//�ŕt�^���[%]
		int PoisonResistPercentage() const;

	public:	//�X�e�[�^�X�ω��֘A

		/// <summary>LV Up/Down</summary>
		/// <param name="nUP">���]��LV�ω��ʁi������Ȃ畉�̒l���w�肷��j</param>
		/// <returns>LV�������ω������̂���Ԃ��iLV�ɂ� ���/���� �����邽�ߕK�����������������オ��킯�ł͂Ȃ��j</returns>
		int ChangeLV( int dLV );

		/// <summary>
		/// HP�̑���(��/�_���[�W)�D
		/// ���������ʂ� ���/���� �𒴂��Ȃ��l�ƂȂ�D
		/// </summary>
		/// <param name="dHP">���]�̕ω��ʁD���Ȃ�Ή񕜁C���Ȃ�_���[�W</param>
		void ChangeHP( int dHP );

		/// <summary>
		/// MP��1�����炷�i���@�g�p����MP����j
		/// </summary>
		/// <param name="Spell1st"></param>
		/// <param name="Spell2nd"></param>
		void DecMP( FirstSpell Spell1st, SecondSpell Spell2nd )
		{
			--m_MP1st[ (int)Spell1st ];
			--m_MP2nd[ (int)Spell2nd ];
		}

		/// <summary>
		/// MP�����S�񕜁i�h�������ۂ̉񕜁j
		/// </summary>
		void FullRecoverMP()
		{
			for( int i=0; i<N_FirstSpell; ++i ){	m_MP1st[i] = MaxMP( (FirstSpell)i );	}
			for( int i=0; i<N_SecondSpell; ++i ){	m_MP2nd[i] = MaxMP( (SecondSpell)i );	}
		}

		/// <summary>�ł�t�^</summary>
		/// <returns>�V���ɕt�^�������ۂ��D�ŏ�Ԃł������ꍇ�ɂ�false�D</returns>
		bool InfectPoison(){	return ChangePoisonStateTo( true );	}

		/// <summary>�ł���</summary>
		/// <returns>�񕜂������ۂ��D�ŏ�ԂłȂ������ꍇ�ɂ�false�D</returns>
		bool CurePoison(){	return ChangePoisonStateTo( false );	}

	public:	//���L�A�C�e�� : �����ɃJ�v�Z�������Ȃ�
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
	/// �L�����N�^��LV������LV�ɏグ�邽�߂̔�p
	/// �i�������C���̊֐���LV����̑��݂��l�����Ȃ��j
	/// </summary>
	/// <param name="CurrLV">����LV</param>
	/// <returns>��p</returns>
	inline int LVUP_Cost( int CurrLV ){	return 1 + CurrLV/2;	}
}
