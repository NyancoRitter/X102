#pragma once
#include <memory>

namespace Toyger
{

	/// <summary>
	/// Win32 のスクロールバーを扱う煩雑な処理をまとめたもの．
	/// 実際にスクロールバーを持っているウィンドウ側から，このクラスのメソッドを適切にコールして使う．
	/// </summary>
	class ScrollBarHandling
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="hWnd">
		/// スクロールバー制御対象ウィンドウを指定．
		/// このオブジェクトを使用する間，このハンドルが指すウィンドウが有効である（存在している）必要がある
		/// </param>
		/// <param name="bShowDisableScrollBar">表示域のサイズがコンテンツサイズより広いときにもスクロールバーを消さずに表示するか否か</param>
		ScrollBarHandling( HWND hWnd, bool bShowDisableScrollBar = false )
			: m_hWnd( hWnd )
			, m_bShowDisableScrollBar(bShowDisableScrollBar)
		{}

	public:
		/// <summary>
		/// 表示物のサイズ（：表示域のサイズがこれ以上の場合はスクロールバー不要）を指定．
		/// 用法：表示コンテンツのサイズが変わったときにコールすること．
		/// </summary>
		/// <param name="W">表示物サイズ</param>
		/// <param name="H">表示物サイズ</param>
		void ChangeContentSize( int W, int H );

		/// <summary>
		/// 表示域（クライアント領域）サイズを指定．
		/// 用法：コンテンツ表示域のサイズが変わったとき（ウィンドウリサイズ時）にコールすること．
		/// </summary>
		/// <param name="W">表示域サイズ</param>
		/// <param name="H">表示域サイズ</param>
		void ChangeViewSize( int W, int H );

		/// <summary>
		/// スクロールイベント時の処理．
		/// 用法：WM_HSCROLL, WM_VSCROLL イベントの処理をこのメソッドに委譲する．
		/// </summary>
		/// <param name="HorizontalBar">制御対象の指定：水平スクロールバーならばtrue, 垂直スクロールバーならばfalse</param>
		/// <param name="nSBCode">スクロールバーコード．イベントの LOWORD(wParam) をそのまま渡せばＯＫ．</param>
		/// <param name="nPos">nSBCodeが特定の値の場合のみ有効なスクロール位置．イベントの HIWORD(wParam) をそのまま渡せばＯＫ．</param>
		void HandleScrollEvent( bool HorizontalBar, UINT nSBCode, UINT nPos ) const;

		/// <summary>スクロールバーの現在のスクロール位置を取得</summary>
		/// <param name="HorizontalBar">対象の指定：水平スクロールバーならばtrue, 垂直スクロールバーならばfalse</param>
		/// <returns>スクロール位置</returns>
		int GetScrollPos( bool HorizontalBar ) const;

		//スクロールバーのスクロール位置を設定．
		//（ただし，指定値が不適切な場合，何かしらの別の位置に設定され得る）
		//HorizontalBar : 水平スクロールバーならばtrue, 垂直スクロールバーならばfalse
		//Pos : 所望の位置．

		/// <summary>
		/// スクロールバーのスクロール位置を設定
		/// （ただし，指定値が不適切な場合，何かしらの別の位置に設定され得る）
		/// </summary>
		/// <param name="HorizontalBar">対象の指定：水平スクロールバーならばtrue, 垂直スクロールバーならばfalse</param>
		/// <param name="Pos">スクロール位置</param>
		/// <returns>スクロール位置が変化したか否か．実施前後で変化が無かった場合にはfalseを返す．</returns>
		bool SetScrollPos( bool HorizontalBar, int Pos ) const;

		//コンテンツの指定位置が可能な限り表示域の中央に来るようにスクロール位置を設定する
		//[Args]
		//	cx.cy : 中央に来るようにしたい位置．コンテンツの座標系で指定．

		/// <summary>
		/// コンテンツの指定位置が可能な限り表示域の中央に来るようにスクロール位置を設定する
		/// </summary>
		/// <param name="cx">中央に来るようにしたい位置</param>
		/// <param name="cy">中央に来るようにしたい位置</param>
		/// <returns>スクロール位置が変化したか否か．実施前後で変化が無かった場合にはfalseを返す．</returns>
		bool ScrollTo( int cx, int cy ) const;

	private:
		void UpdateScrollBar( bool bHorizonalBar );

		void SetSI(bool bHorizonalBar, SCROLLINFO & Info) const
		{	::SetScrollInfo( m_hWnd, (bHorizonalBar ? SB_HORZ : SB_VERT ), &Info, TRUE );	}

		void GetSI(bool bHorizonalBar, SCROLLINFO & Info) const
		{	::GetScrollInfo( m_hWnd, (bHorizonalBar ? SB_HORZ : SB_VERT ), &Info );	}

	private:
		HWND m_hWnd = NULL;
		int m_ViewW = 1;
		int m_ViewH = 1;
		int m_ContentW = 1;
		int m_ContentH = 1;
		bool m_bShowDisableScrollBar;
	};
}
