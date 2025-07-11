#pragma once

namespace Toyger
{
	/// <summary> ゲーム側がウィンドウクライアント領域への表示内容を描画する手段．</summary>
	class IContentPainter
	{
	public:
		virtual ~IContentPainter() = default;

		/// <summary>描画処理</summary>
		/// <param name="hdc">描画先</param>
		/// <param name="W">hdcに関連づけられているBITMAPのサイズ</param>
		/// <param name="H">hdcに関連づけられているBITMAPのサイズ</param>
		virtual void Paint( HDC hdc, int W, int H ) = 0;
	};

}
