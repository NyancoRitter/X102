#pragma once

/// <summary>
/// �A�C�e�����̎g�p���ʂ̑Ώ۔͈�
/// </summary>
enum class TgtRange
{
	/// <summary>�Ȃ��i�S���Ώۂ����Ȃ��A�C�e����g�p�ł��Ȃ��A�C�e�����j</summary>
	None = 0,

	/// <summary>����(�g�p�҂��猩�Ė���)�P��</summary>
	SingleFriend = 0x11,
	/// <summary>�G(�g�p�҂��猩�ēG)�P��</summary>
	SingleEnemy = 0x12,

	/// <summary>����(�g�p�҂��猩�Ė���)�S��</summary>
	AllFriend = 0x21,
	/// <summary>�G(�g�p�҂��猩�ēG)�S��</summary>
	AllEnemy = 0x22
};

inline bool IsForFriend( TgtRange tr ){	return ( (unsigned int)tr & 0x0Fu )==0x01u;	}
inline bool IsForEnemy( TgtRange tr ){	return ( (unsigned int)tr & 0x0Fu )==0x02u;	}

inline bool IsForSingle( TgtRange tr ){	return ( (unsigned int)tr & 0xF0u )==0x10u;	}
inline bool IsForAll( TgtRange tr ){	return ( (unsigned int)tr & 0xF0u )==0x20u;	}
