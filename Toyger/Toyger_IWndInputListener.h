#pragma once

namespace Toyger
{
	/// <summary>Toyger::GameWnd のウィンドウプロシージャから入力情報を受ける人</summary>
	class IWndInputListener
	{
	public:
		virtual ~IWndInputListener() = default;
	public:
		/// <summary>キーあるいはマウスボタンが押されたことの通知</summary>
		/// <param name="VKeyCode">対象のキーまたはマウスボタンの仮想キーコード</param>
		virtual void OnPress( unsigned char VKeyCode ) = 0;

		/// <summary>キーあるいはマウスボタンが離されたことの通知</summary>
		/// <param name="VKeyCode">対象のキーまたはマウスボタンの仮想キーコード</param>
		virtual void OnRelease( unsigned char VKeyCode ) = 0;

		/// <summary>マウスカーソルが移動したことの通知</summary>
		/// <param name="x">クライアント領域上での座標</param>
		/// <param name="y">クライアント領域上での座標</param>
		virtual void OnMouseMove( int x, int y ) = 0;

		/// <summary>ウィンドウを閉じる操作が成された際の通知</summary>
		/// <returns>ウィンドウを閉じて良いかどうか．falseを返すと閉じない（操作が棄却される）</returns>
		virtual bool OnWndCloseOP() = 0;
	};
}