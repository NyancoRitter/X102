#pragma once
#include "framework.h"
#include <string>
#include <memory>

namespace Toyger
{
	class IContentPainter;
	class IBGMPlayer;

	/// <summary>ゲーム実装側からのウィンドウへのアクセスI/F</summary>
	/// <remarks>成否を返す大抵のメソッドは，ウィンドウ生成前やウィンドウを閉じた後の状態では失敗する．</remarks>
	class IWnd
	{
	public:
		virtual ~IWnd() = default;
	
	public:	//表示物のサイズ指定，描画 -------------------------

		/// <summary>ウィンドウのクライアント領域に表示する内容の(拡大率1倍での)サイズを設定</summary>
		/// <param name="W">所望の幅</param>
		/// <param name="H">所望の高さ</param>
		/// <returns>成否</returns>
		virtual bool SetViewContentSize( int W, int H ) = 0;

		/// <summary>表示拡大倍率の設定</summary>
		/// <param name="Rate">倍率．1以下の値を指定した場合には1と見なされる．</param>
		/// <returns>成否</returns>
		virtual bool SetViewMagRate( int Rate ) = 0;

		/// <summary>現在の表示拡大倍率</summary>
		/// <returns>倍率</returns>
		virtual int GetViewMagRate() const = 0;

		/// <summary>ウィンドウのサイズを現在の表示物のサイズ（= コンテンツサイズ * 表示倍率）に合わせる</summary>
		/// <returns>成否</returns>
		virtual bool ResizeToFit() = 0;

		/// <summary>クライアント領域の表示内容を引数のオブジェクトを用いて更新する</summary>
		/// <param name="rPainter">描画処理者</param>
		/// <returns>成否</returns>
		virtual bool UpdateViewContent( IContentPainter &rPainter ) = 0;

		///// <summary>現在の表示内容（最後の UpdateViewContent() による描画結果）のクローンを生成する．</summary>
		///// <returns>
		///// 成功時は生成したビットマップのハンドル．これは呼び出し側で DeleteObject() APIで削除する必要がある．
		///// 失敗時は NULL を返す．
		///// </returns>
		//virtual HBITMAP CloneCurrentViewContent() const = 0;

	public:	//スクロール, 座標変換 関連 -----------------------------------

		/// <summary>コンテンツの指定座標が可能な限り表示域の中央に来るようにスクロール位置を設定する</summary>
		/// <param name="cx">中央に来るようにしたい位置．コンテンツの座標系で指定．</param>
		/// <param name="cy">中央に来るようにしたい位置．コンテンツの座標系で指定．</param>
		/// <returns>成否</returns>
		virtual bool ScrollTo( int cx, int cy ) = 0;

		//スクロールなしpixel座標→スクロール時の描画すべきpixel座標　変換

		/// <summary>コンテンツ座標→クライアント領域座標　変換</summary>
		/// <param name="ContentX">コンテンツの座標系での位置</param>
		/// <returns>
		/// 対応するクライアント領域での値．
		/// 表示が拡大されている状況では拡大後正方形の左上の座標．
		/// </returns>
		virtual int ToClientAreaX( int ContentX ) const = 0;

		/// <summary>コンテンツ座標→クライアント領域座標　変換</summary>
		/// <param name="ContentY">コンテンツの座標系での位置</param>
		/// <returns>
		/// 対応するクライアント領域での位置．
		/// 表示が拡大されている状況では拡大後正方形の左上の座標．
		/// </returns>
		virtual int ToClientAreaY( int ContentY ) const = 0;

		/// <summary>クライアント領域座標→コンテンツ座標　変換</summary>
		/// <param name="ClientAreaX">クライアント領域座標での位置</param>
		/// <returns>対応するコンテンツ座標での位置</returns>
		virtual int ToContentX( int ClientAreaX ) const = 0;

		/// <summary>クライアント領域座標→コンテンツ座標　変換</summary>
		/// <param name="ClientAreaY">クライアント領域座標での位置</param>
		/// <returns>対応するコンテンツ座標での位置</returns>
		virtual int ToContentY( int ClientAreaY ) const = 0;

	public:	//その他ウィンドウ操作 -----------------------------

		/// <summary>ウィンドウキャプション文字列の変更</summary>
		/// <param name="pStr">キャプション文字列</param>
		/// <returns>成否</returns>
		virtual bool SetCaption( LPCTSTR pStr ) = 0;

		/// <summary>ウィンドウ位置の変更</summary>
		/// <param name="rW">所望のウィンドウ左端x座標</param>
		/// <param name="rH">所望のウィンドウ上端y座標</param>
		/// <returns>成否</returns>
		virtual bool MoveTo( int x, int y ) = 0;

		///<summary>このウィンドウを親としてのメッセージボックスのモーダル表示</summary>
		virtual int MsgBox( LPCTSTR Msg, LPCTSTR Caption, UINT MsgBoxType ) = 0;

		/// <summary>
		/// 指定文字列を指定のフォントで描画した際のサイズを得る
		/// </summary>
		/// <param name="pStrU16">UTF16文字列</param>
		/// <param name="hFont">フォント（ただしNULLを指定した場合には現在のフォントを使用）</param>
		/// <returns>サイズ</returns>
		virtual SIZE CalcTextDrawSize( LPCWSTR pStrU16, HFONT hFont ) const = 0;

		/// <summary>
		/// ウィンドウのアイコンを変更
		/// </summary>
		/// <param name="IconHandle">アイコンのハンドル</param>
		/// <param name="ForSmall">
		/// 対象の指定．小さいアイコンか否か．falseなら大きいアイコン
		/// </param>
		/// <returns>成否</returns>
		virtual bool ChangeIcon( HANDLE IconHandle, bool ForSmall ) = 0;

	public:	//BMG ----------------------------------------------

		/// <summary>
		/// IBGMPlayerへのポインタを返す．
		/// </summary>
		/// <returns>
		/// 失敗時はnullptr．
		/// * ウィンドウ生成前に呼ぶと失敗する．
		/// * 戻り値が指すオブジェクトを使用できる期間はウィンドウが閉じられるまでである．
		///   （そこらへんを注意して使うことはこのメソッドを呼ぶ側の責任とする）
		/// </returns>
		virtual IBGMPlayer *BGMPlayer() = 0;
	};
}
