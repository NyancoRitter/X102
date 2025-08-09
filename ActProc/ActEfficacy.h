#pragma once

#include "ActResult.h"
#include <functional>
#include <vector>

class ActContext;

/// <summary>
/// アイテムや魔法により生じる効果の処理
/// </summary>
using ActEfficacy = 
std::function<
	std::vector< ActResult >(
		CharSpecifier Actor,	//効果を発生させる行動の実施者
		std::vector< CharSpecifier > Tgts,	//効果を与える対象
		const ActContext &Context	//処理に必要な文脈
	)
>;
