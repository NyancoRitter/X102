#pragma once

#include "ActResult.h"
#include "EfficacySource.h"
#include "GameContent/AttackElement.h"

#include <functional>
#include <vector>

namespace GameContent{	class ICharacter;	}
class ActContext;

//HP回復処理
class RecoverHP
{
public:	//定義用

	//回復効果量決定手段．引数は回復行動実行者
	using CalcRawAmountFunc = std::function< double( const GameContent::ICharacter & ) >;

	RecoverHP( const CalcRawAmountFunc &CalcAmount, EfficacySource RecovSource=EfficacySource::Other )
		: m_CalcRawAmount(CalcAmount), m_RecovSource(RecovSource)
	{}

public:
	std::vector< ActResult > operator()(
		CharSpecifier Actor,
		std::vector< CharSpecifier > Tgts,
		const ActContext &Context
		) const;
private:
	CalcRawAmountFunc m_CalcRawAmount;
	EfficacySource m_RecovSource;
};

//毒回復処理
std::vector< ActResult > CurePoison(
	CharSpecifier Actor,
	std::vector< CharSpecifier > Tgts,
	const ActContext &Context
);


//攻撃処理
class Attack
{
public:	//定義用
	//攻撃威力決定手段．引数は攻撃行動実行者．
	//多段攻撃の場合には複数要素を返す．
	using CalcRawPowerFunc = std::function< std::vector<double>(const GameContent::ICharacter &) >;

	Attack( const CalcRawPowerFunc &CalcPower, EfficacySource AtkSource=EfficacySource::Other )
		: m_CalcRawPower( CalcPower ), m_AtkSource(AtkSource)
	{}

	//攻撃属性．指定しない場合のデフォルトは物理．
	Attack &Elem( GameContent::AttackElement AtkElem ){	m_AtkElem=AtkElem;	return *this;	}
	//HP吸収攻撃にする
	Attack &Suction(){	m_IsSuction=true;	return *this;	}
	//毒付与効果を持たせる
	Attack &WithPoison(){	m_WithPoison=true;	return *this;	}

public:
	GameContent::AttackElement Element() const {	return m_AtkElem;	}

	std::vector< ActResult > operator()(
		CharSpecifier Actor,
		std::vector< CharSpecifier > Tgts,
		const ActContext &Context
		) const;

private:
	CalcRawPowerFunc m_CalcRawPower;
	EfficacySource m_AtkSource;
	GameContent::AttackElement m_AtkElem = GameContent::AttackElement::Physical;
	bool m_IsSuction = false;
	bool m_WithPoison = false;
};
