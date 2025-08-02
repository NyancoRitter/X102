#pragma once

class Rect;

namespace GUI::Menu
{
	/// <summary>
	/// ���j���[���̂P���ڂ̕`���i
	/// 
	/// * �ݒ�Ƃ��ĕ`��F��ێ�
	/// </summary>
	class IMenuItem
	{
	public:
		virtual ~IMenuItem() = default;

		/// <summary>���ڂ̕`��F</summary>
		/// <returns>�`��F</returns>
		virtual COLORREF DrawColor() const = 0;

		/// <summary>���ڂ̕`��F��ύX</summary>
		/// <returns>*this</returns>
		virtual IMenuItem &DrawColor( COLORREF Color ) = 0;

		/// <summary>
		/// ���j���[���ڂ̕`��
		/// </summary>
		/// <param name="hdc">�`��Ώ�hdc</param>
		/// <param name="ItemDrawReg">���ڕ`��͈�</param>
		/// <param name="IsAtCursorPos">���̍��ڂ��J�[�\���ʒu�ɂ��邩�ۂ�</param>
		/// <param name="IsMenuFocused">�������j���[���t�H�[�J�X��Ԃ��ۂ�</param>
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const = 0;
	};
}
