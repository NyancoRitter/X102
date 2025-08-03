#pragma once

namespace GUI::Color
{
	//���
	constexpr COLORREF White = RGB(255,255,255);	//��
	constexpr COLORREF GrayOut = RGB(128,128,128);	//�O���[�A�E�g���ڂ̐F

	//���j���[�J�[�\���̐F
	inline COLORREF MenuCursor( bool Focused ){	return ( Focused ? RGB(0,0,255) : RGB(96,96,96) );	}
}
