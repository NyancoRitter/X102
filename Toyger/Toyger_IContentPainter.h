#pragma once

namespace Toyger
{
	/// <summary> �Q�[�������E�B���h�E�N���C�A���g�̈�ւ̕\�����e��`�悷���i�D</summary>
	class IContentPainter
	{
	public:
		virtual ~IContentPainter() = default;

		/// <summary>�`�揈��</summary>
		/// <param name="hdc">�`���</param>
		/// <param name="W">hdc�Ɋ֘A�Â����Ă���BITMAP�̃T�C�Y</param>
		/// <param name="H">hdc�Ɋ֘A�Â����Ă���BITMAP�̃T�C�Y</param>
		virtual void Paint( HDC hdc, int W, int H ) = 0;
	};

}
