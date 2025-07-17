#pragma once

#include "IPainter.h"

class IController;

namespace GUI::Menu
{
	class IMenuContent;

	//Menu::HandleInput()の戻り値用
	enum class HandleInputResult
	{
		None,	//有効な操作が行われなかった（何も変化無し）
		CursorMoved,	//カーソル位置が変化した
		Selected,	//項目選択操作が成された
		Canceled	//キャンセル操作が成された
	};

	/// <summary>
	/// メニューの入力操作と描画処理の実装．
	/// 
	/// * 操作/描画 対象を SetContet() で指定する必要がある．
	///   当然，対象の寿命に関する注意が必要．
	/// * 描画のための各種設定を保持．
	/// </summary>
	class Menu : public IPainter
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="pContent">描画対象</param>
		Menu( IMenuContent *pContent = nullptr ){	SetContent( pContent );	}

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override{	return m_TopLeft;	}
		virtual Menu &TopLeft( const Vec2i &TL ) override {	m_TopLeft = TL;	return *this;	}

		/// <summary>描画範囲サイズ．</summary>
		/// <returns>
		/// サイズ．ただし SetContent() で有効な描画対象を指定していない状態では不定．
		/// </returns>
		virtual Vec2i Size() const override;

	protected:
		/// <summary>
		/// 描画．ただし SetContent() で有効な描画対象を指定していない状態では何もしない．
		/// </summary>
		/// <param name="hdc"></param>
		virtual void Paint_( HDC hdc ) const override;

	public:
		/// <summary>処理対象の指定</summary>
		/// <param name="pContent">
		/// 処理対象．
		/// nullptrを指定した場合，いくつかのメソッドが意味をなさない動作となる．
		/// </param>
		/// <returns>*this</returns>
		Menu &SetContent( IMenuContent *pContent ){	m_pContent = pContent;	return *this;	}

		/// <summary>メニューの操作処理</summary>
		/// <param name="Controller">操作入力</param>
		/// <returns>処理結果</returns>
		HandleInputResult HandleInput( const IController &Controller );

		//描画時に項目間に開ける間隔[pixel]
		int ItemSpacing() const {	return m_ItemSpacing;	}
		Menu &ItemSpacing( int space ){	m_ItemSpacing = space;	return *this;	}

		//描画時に項目描画範囲の外周（上下左右）に設ける余白．
		//* 項目群の描画位置は TopLeft() からこの分だけオフセットされる．
		//* Size() が返す値はこれを加味したものとなる．
		Vec2i OuterMargin() const {	return m_OuterMargin;	}
		Menu &OuterMargin( const Vec2i &Margin ){	m_OuterMargin = Margin;	return *this;	}

		//外枠描画の有無
		bool WithFrame() const {	return m_bDrawFrame;	}
		Menu &WithFrame( bool Enable ){	m_bDrawFrame = Enable;	return *this;	}

		//メニューのフォーカス状態：描画具合に影響
		bool IsFocused() const {	return m_bFocused;	}
		Menu &IsFocused( bool Focused ){	m_bFocused = Focused;	return *this;	}

		/// <summary>項目の描画範囲</summary>
		/// <param name="index">項目index</param>
		/// <returns>
		/// 描画範囲．ただし SetContent() で有効な描画対象を指定していない状態では不定．
		/// </returns>
		Rect ItemDrawRect( int index ) const;

	private:
		IMenuContent *m_pContent = nullptr;
		Vec2i m_TopLeft;
		int m_ItemSpacing = 0;
		Vec2i m_OuterMargin;
		bool m_bDrawFrame = false;
		bool m_bFocused = false;
	};

	/// <summary>
	/// メニュー用のカーソル描画．
	/// ItemDrawReg で指定された矩形範囲を塗りつぶす．
	/// </summary>
	/// <param name="hdc"></param>
	/// <param name="ItemDrawReg">カーソルが指す項目の描画範囲</param>
	/// <param name="IsMenuFocused">メニューがフォーカス状態か否か（描画色に影響）</param>
	void DrawMenuCursor( HDC hdc, const Rect &ItemDrawReg, bool IsMenuFocused );
}
