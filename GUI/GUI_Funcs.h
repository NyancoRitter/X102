#pragma once
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// ˆÍ‚İ˜g‚Ì•`‰æD
	/// ŠpŠÛ‚Ì‹éŒ`˜g‚ğ•`‰æ‚·‚éD
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color">˜g‚Ì•`‰æF</param>
	/// <param name="bFill">true‚Ìê‡C˜g“à‚ğ•‚Å“h‚è‚Â‚Ô‚·</param>
	/// <param name="RoundedCornerRadius">ŠpŠÛ•”•ª‚Ì”¼Œa[pixel]</param>
	void DrawFrame( HDC hdc, const Rect &Rect, COLORREF Color, bool bFill=true, int RoundedCornerRadius=5 );

	/// <summary>
	/// ‹éŒ`—Ìˆæ‚ğw’èF‚Å“h‚è‚Â‚Ô‚·
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color"></param>
	void FillRectReg( HDC hdc, const Rect &Rect, COLORREF Color );
}
