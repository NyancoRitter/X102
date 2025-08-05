#pragma once

/// <summary>
/// 効果発生要因．
/// 攻撃や回復などの発生要因の情報．
/// </summary>
enum class EfficacySource
{
	Other,	//現実装において特に名言する必要のない場合(利用されない要因)用の値

	Potion,	//ポーション類の使用
	Weapon	//武器による攻撃
};
