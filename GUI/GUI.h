#pragma once

namespace GUI
{
	/// <summary>
	/// �p�ۂ̋�`�g��`��
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color"></param>
	/// <param name="bFill">true�̏ꍇ�C�g�������œh��Ԃ�</param>
	void DrawFrame( HDC hdc, const RECT &Rect, COLORREF Color, bool bFill=true );
}
