#pragma once

namespace GUI
{
	/// <summary>
	/// ŠpŠÛ‚Ì‹éŒ`˜g‚ğ•`‰æ
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="Rect"></param>
	/// <param name="Color"></param>
	/// <param name="bFill">true‚Ìê‡C˜g“à‚ğ•‚Å“h‚è‚Â‚Ô‚·</param>
	void DrawFrame( HDC hdc, const RECT &Rect, COLORREF Color, bool bFill=true );
}
