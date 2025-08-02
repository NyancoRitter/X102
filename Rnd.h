#pragma once

#include "Parts/StlRnd.h"

/// <summary>‹^——”</summary>
/// <returns></returns>
inline StlRnd &RND()
{
	static StlRnd Rnd;
	return Rnd;
}
