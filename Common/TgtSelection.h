#pragma once

#include "GUI/IGUI.h"
#include "Parts/Rect.h"
#include <vector>
#include <memory>
#include <functional>

namespace GUI
{
	/// <summary>
	/// 何らかの表示物の並びが存在するときに，それらのうちのどれかを選ぶUI.
	/// 既に描画されている表示物の上に重ねるように選択カーソルを描画する．
	/// （表示物は縦あるいは横方向に概ね一列に並んでいる想定．）
	///
	/// 戦闘シーンやパーティメニュー画面でアイテム等を使う対象を選択するUIの実装．
	/// 
	///  機能：
	/// * 「単体を対象とする」アイテム等を使う対象を選ぶ際には，三角形のカーソルを動かして選ぶ．
	/// * 「全体を対象とする」アイテム等を使う場合の表示用として，全項目に三角形カーソルを表示する．
	/// </summary>
	class TgtSelection : public GUI::IGUI
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemRects">
		///	項目個数と各項目の表示範囲情報を指定する．
		/// カーソル描画位置がこれに基づいて決められる．
		/// 空を指定してはならない（→指定した場合には例外を送出）
		/// </param>
		/// <param name="ForAll">
		/// 「全体を対象としている」のか否か．
		/// 単体を選択する用途の場合には false を指定する．
		/// </param>
		/// <param name="IsVertical">
		/// 項目並びが縦方向か．falseを指定した場合には横方向．
		/// カーソル移動操作方法と，カーソル描画方法が変わる．
		/// </param>
		/// <param name="Callback">
		/// Update() 内で選択決定操作あるいはキャンセルが成された際に呼ばれるコールバック．
		/// 引数は，
		///		* 選択(true) or キャンセル(false)
		///		* カーソル位置（0-based index, ただし ForAll を指定した場合には負の値）．
		/// 戻り値はそのまま Update() からの戻り値として用いられる．
		/// </param>
		TgtSelection(
			const std::vector<Rect> &ItemRects,
			bool ForAll,
			bool IsVertical,
			const std::function< Flags<GUIResult>( bool, int ) > &Callback
		)
			: m_ItemRects( ItemRects )
			, m_IsForAll( ForAll )
			, m_IsVertical( IsVertical )
			, m_OnDecided( Callback )
		{
			if( m_ItemRects.empty() )throw std::exception( "ItemRects.empty() !!" );
		}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void DrawCursorFor( HDC hdc, const Rect &ItemRect ) const;
	private:
		const std::vector<Rect> m_ItemRects;
		const bool m_IsForAll;
		const bool m_IsVertical;
		std::function< Flags<GUI::GUIResult>( bool, int ) > m_OnDecided;

		int m_CursorPos = 0;

		//カーソルアニメーション用
		unsigned char m_AnimCounter = 0;
		static constexpr unsigned char ms_AnimBit = 1<<3;
	};
}
