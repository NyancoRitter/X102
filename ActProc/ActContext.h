#pragma once

#include "CharSpecifier.h"

namespace GameContent{	class ICharacter;	}

class ActContext
{
public:
	virtual ~ActContext() = default;

	/// <summary>
	/// CharSpecifier �ɑΉ����� ICharacter �ւ̃A�N�Z�X�񋟎�i
	/// </summary>
	virtual GameContent::ICharacter &CharAt( const CharSpecifier &Tgt ) const = 0;

	/// <summary>
	/// �i�퓬���́j���݂̃^�[��
	/// </summary>
	/// <returns>
	/// ���݂̃^�[��(�ŏ��̃^�[����1�Ƃ���)�D
	/// �������C�퓬���łȂ��ꍇ�ɂ� 0 �ȉ��̒l�D
	/// </returns>
	virtual int CurrentTurn() const {	return -1;	}
	bool DuringBattle() const {	return CurrentTurn()<=0;	}
};
