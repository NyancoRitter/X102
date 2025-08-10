#pragma once

#include "Effect.h"
#include "Parts/Vec2d.h"
#include <memory>

std::unique_ptr<IEffect> CreateHPRecovEffect( int Amount, const Vec2i &Center, int SuppressSubSequentsPeriod=12 );
std::unique_ptr<IEffect> CreateDmgEffect( int Amount, const Vec2i &Center, int SuppressSubSequentsPeriod=12 );
std::unique_ptr<IEffect> CreatePoisonCuredEffect();
