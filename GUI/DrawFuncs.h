#pragma once
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// �͂ݘg�̕`��D�p�ۂ̋�`�g��`�悷��D
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect">��`</param>
	/// <param name="Color">�g�̕`��F</param>
	/// <param name="RoundedCornerRadius">�p�ە����̔��a[pixel]</param>
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, int RoundedCornerRadius=5 );

	/// <summary>
	/// �͂ݘg�̕`��(������h��Ԃ���)�D�p�ۂ̋�`�g��`�悷��D
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="LineColor">�g�̕`��F</param>
	/// <param name="FillColor">�g����h��Ԃ��F</param>
	/// <param name="RoundedCornerRadius">�p�ە����̔��a[pixel]</param>
	void DrawFilledFrame( HDC hdc, const Rect &Rect, COLORREF LineColor, COLORREF FillColor, int RoundedCornerRadius=5 );

	/// <summary>
	/// ��`�̈���w��F�œh��Ԃ�
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect">�͈�</param>
	/// <param name="Color">�h��Ԃ��F</param>
	void FillRectReg( HDC hdc, const Rect &Rect, COLORREF Color );

	/// <summary>
	/// ���O�p�`�̕`��
	/// </summary>
	/// <param name="hdc">�y����u���V���w��ς݂̂���</param>
	/// <param name="Center">�ʒu�i�O�p�`�̊O�ڋ�`�̒��S�j</param>
	/// <param name="EdgeLength">�P�ӂ̒���</param>
	/// <param name="RotDeg">��]�� (90�x�P�ʁD���̒lx90�x������]). �i0�̂Ƃ��̌����́��j</param>
	void DrawTriangle( HDC hdc, const Vec2i &Center, int EdgeLength, unsigned int Rot=0 );

	/// <summary>
	/// ���j���[�p�̃J�[�\���`��D
	/// ItemDrawReg �Ŏw�肳�ꂽ��`�͈͂��t�H�[�J�X��Ԃɉ������F�œh��Ԃ��D
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="ItemDrawReg">�J�[�\�����w�����ڂ̕`��͈�</param>
	/// <param name="IsMenuFocused">���j���[���t�H�[�J�X��Ԃ��ۂ��i�`��F�ɉe���j</param>
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused );

}
