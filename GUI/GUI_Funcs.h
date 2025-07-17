#pragma once
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// �͂ݘg�̕`��D
	/// �p�ۂ̋�`�g��`�悷��D
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color">�g�̕`��F</param>
	/// <param name="bFill">true�̏ꍇ�C�g�������œh��Ԃ�</param>
	/// <param name="RoundedCornerRadius">�p�ە����̔��a[pixel]</param>
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, bool bFill=true, int RoundedCornerRadius=5 );

	/// <summary>
	/// ��`�̈���w��F�œh��Ԃ�
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color"></param>
	void FillRectReg( HDC hdc, const Rect &Rect, COLORREF Color );
}
