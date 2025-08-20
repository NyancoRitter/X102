#pragma once

#include "Misc/Cmd.h"
#include "Parts/Vec2D.h"
#include "PainterSeq.h"

Cmd<> CreateHPRecovEffect( PainterSeq &PStoJoin, int Amount, const Vec2i &Center, int Period=16, int SuppressSubSequentsPeriod=12 );
Cmd<> CreateDmgEffect( PainterSeq &PStoJoin, int Amount, const Vec2i &Center, int Period=16, int HopPeriod=6, int SuppressSubSequentsPeriod=12 );

