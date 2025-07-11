#pragma once

namespace GUI
{
	/// <summary>
	/// �`��F
	/// </summary>
	class Color
	{
	private:
		Color() = delete;
	public:
		constexpr static COLORREF White = RGB(255,255,255);	//��
		constexpr static COLORREF GrayOut = RGB(128,128,128);	//�O���[�A�E�g���ڂ̐F

		//���j���[�J�[�\���̐F
		inline static COLORREF MenuCursor( bool Focused ){	return ( Focused ? RGB(0,0,255) : GrayOut );	}
	};
}
