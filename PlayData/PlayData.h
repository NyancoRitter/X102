#pragma once

#include <array>
#include <string>
#include <memory>
#include "Misc/DataVec.h"
#include "ActProc/ActResult.h"
#include "GameContent/PartyChar.h"

/// <summary>
/// �p�[�e�B�̍ő�l��
/// </summary>
constexpr int PARTY_CAPACITY = 3;

/// <summary>
/// �X�g�b�N�̃A�C�e���������̒l�ȏ�ł���ꍇ�ɁC�u�������v�Ƃ��鐔�D
///		<remarks>
///		�u�X�g�b�N�̍ő�T�C�Y�v�ł͂Ȃ��D
///		�x���z�������̃A�C�e�����������悤�Ƃ��邱�Ƃ�}�����邽�߂̒l�ł���D
///		�Ⴆ�ΓX�ł̃A�C�e���w�����u�ו�����������v�Ƃ��Đ�������悤�ȗp�r�̒l�D
///		</reamrks>
/// </summary>
constexpr int N_TOO_MANY_STOCK_ITEMS = 32;


/// <summary>
/// �v���C�󋵃f�[�^�D
/// �i�Z�[�u�⃍�[�h�̑ΏۂɂȂ�悤�ȃf�[�^�j
/// </summary>
class PlayData
{
public:
	PlayData();
	~PlayData();

public:	//--- �Z�[�u�ƃ��[�h ----------------------------------------

	/// <summary>�Z�[�u</summary>
	/// <param name="FilePathU8">�Z�[�u��t�@�C����</param>
	/// <returns>����</returns>
	bool Save( const std::string &FilePathU8 ) const;

	/// <summary>���[�h</summary>
	/// <param name="FilePathU8">���[�h����Z�[�u�t�@�C����</param>
	/// <returns>���[�h���ʁD���������s���ɂ�nullptr��Ԃ�</returns> 
	static std::unique_ptr<PlayData> Load( const std::string &FilePathU8 );


public:	//--- �p�[�e�B�Ґ��֘A --------------------------------------

	/// <summary>�����̃p�[�e�B</summary>
	/// <returns>���݂̃p�[�e�B�\���D0�l�Ƃ�����Ԃ����蓾��</returns>
	const DataVec< GameContent::PartyCharID > &CurrParty() const {	return m_CurrParty;	}

	/// <summary>�p�[�e�B�Ґ���ύX</summary>
	/// <param name="Chars">
	/// �p�[�e�B�̐l�����̃L�����N�^ID���p�[�e�B�����я��Ŏw�肷��i��vector�̎w����L���j�C
	/// * �p�[�e�B�ő�l���𒴂���v�f���̃f�[�^��^�����ꍇ�C�������镪�͖��������i�F�擪���݂̂��g�p�����j�D
	/// * �����f�[�^�ɏd��������ꍇ�C����͕s��D�i�s���ȓ��͂ł��邪�C���\�b�h���Ń`�F�b�N����Ȃ��j
	/// </param>
	void SetupParty( const std::vector<GameContent::PartyCharID> &Chars );

public:	//--- �L�����N�^, �A�C�e���X�g�b�N�f�[�^�̌�����Q�� --------------------------
	
	GameContent::PartyChar &Char( GameContent::PartyCharID ID ){	return m_Chars[(int)ID];	}
	const GameContent::PartyChar &Char( GameContent::PartyCharID ID ) const {	return m_Chars[(int)ID];	}

	DataVec< GameContent::ItemID > &ItemStock(){	return m_ItemStock;	}
	const DataVec< GameContent::ItemID > &ItemStock() const {	return m_ItemStock;	}

public:	//--- ������ ------------------------------------------------

	/// <summary>���݂̏�����</summary>
	/// <returns>�������D���̒l�����蓾��</returns>
	int Money() const {	return m_Money;	}

	/// <summary>�������̑����D���ʂƂ��ď����������̒l�ɂȂ蓾��D</summary>
	/// <param name="delta">�����l</param>
	void AddMoney( int delta ){	m_Money += delta;	}

public:	//--- ���̑� ------------------------------------------------

	/// <summary>
	/// �h���ɔ��܂����ۂ̏���(�e��񕜓�)�D
	/// �������C�������̑��������͂����ł͍s��Ȃ� �� �ʓrAddMoney()�Ŏ��{�̂��ƁD
	/// </summary>
	/// <returns>���������񕜌���</returns>
	std::vector< ActResult > ProcOfINN();

	/// <summary>
	/// ���{���̃h�A��ʉ߂ł��邩�ۂ�
	/// </summary>
	/// <param name="DoorLockLV">
	/// �h�A�̌�LV(0�`)�D
	/// 0�͌��̂������Ă��Ȃ��h�A�D
	/// 1�ȏ�͌���LV�������D
	/// </param>
	/// <returns>�ʉ߂ł��邩�ۂ�</returns>
	bool CanMoveThrough( int DoorLockLV ) const;

	//===============================================================
private:
	

private:
	//�L�����N�^�S�����̃f�[�^�D�v�f�̏����� PartyCharID �̗v�f�̕��я��ƑΉ�
	std::array< GameContent::PartyChar, GameContent::N_PARTY_CHAR_DEF > m_Chars;

	//���݂̃p�[�e�B
	DataVec< GameContent::PartyCharID > m_CurrParty;
	//������
	int m_Money = 0;
	//�A�C�e�� Stock
	DataVec< GameContent::ItemID > m_ItemStock;


};
