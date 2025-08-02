#pragma once

#include <string>

class CMonoBMP;

void DrawCharPanel(
	HDC hdc,
	const RECT &DrawRegion,
	const std::wstring &CharNameU16,
	const CMonoBMP &Icon,
	int HP,
	int MaxHP,
	COLORREF ForeColor,
	COLORREF BkColor
);
