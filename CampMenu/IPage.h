#pragma once
#include "CampMenu.h"

namespace GameContent{	class PartyChar;	}

/// <summary>
/// �L�����v���j���[�̃y�[�W
/// </summary>
class CampMenu::IPage : public IGUI
{
public:
	/// <summary>
	/// �I������Ă���L�����N�^���ς�����ۂ̍X�V����
	/// </summary>
	virtual void OnSelectedCharChanged() = 0;

	/// <summary>
	/// ���̃y�[�W�𑀍�Ώۂɂł��邩�ۂ�
	/// </summary>
	/// <returns></returns>
	virtual bool CanEnter() const = 0;
};
