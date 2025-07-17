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
	/// ���j���[�p�̃J�[�\���`��D
	/// ItemDrawReg �Ŏw�肳�ꂽ��`�͈͂�h��Ԃ��D
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="ItemDrawReg">�J�[�\�����w�����ڂ̕`��͈�</param>
	/// <param name="IsMenuFocused">���j���[���t�H�[�J�X��Ԃ��ۂ��i�`��F�ɉe���j</param>
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused );
}
