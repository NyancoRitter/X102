#pragma once

#include "ActResult.h"
#include <functional>
#include <vector>

class ActContext;

/// <summary>
/// �A�C�e���▂�@�ɂ�萶������ʂ̏���
/// </summary>
using ActEfficacy = 
std::function<
	std::vector< ActResult >(
		CharSpecifier Actor,	//���ʂ𔭐�������s���̎��{��
		std::vector< CharSpecifier > Tgts,	//���ʂ�^����Ώ�
		const ActContext &Context	//�����ɕK�v�ȕ���
	)
>;
