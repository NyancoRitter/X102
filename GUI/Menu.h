#pragma once
#include "Parts/Vec2D.h"
#include "MenuItem.h"

class IController;

namespace GUI::Menu
{
	/// <summary>
	/// メニューの内容．
	/// 項目群と現在カーソル位置の情報を保持．
	/// </summary>
	class IMenuContent
	{
	public:
		virtual ~IMenuContent() = default;
	public:
		/// <summary>項目の並び方向</summary>
		/// <returns>trueなら縦，falseなら横</returns>
		virtual bool IsVerticalMenu() const = 0;

		/// <summary>項目描画サイズ（メニューの全項目は同一サイズであるという前提）</summary>
		/// <returns></returns>
		virtual Vec2i ItemSize() const = 0;

		//メニュー項目数．
		virtual int nItems() const = 0;

		//現在のカーソル位置
		//負の値は「どの項目も指していない」状態であることを示す．
		virtual int CursorPos() const = 0;

		//カーソル位置の変更
		//範囲外（負の値や項目個数以上の値）を指定した場合，負の値（どの項目も指さない状態）に設定される．
		virtual void CursorPos( int pos ) = 0;

		//項目への参照
		//indexが範囲外の場合は例外送出される
		virtual const IMenuItem &Item( int index ) const = 0;
	};

	/// <summary>
	/// メニューの描画処理の実装．
	/// * 描画のための各種設定を保持．
	/// </summary>
	class MenuPainter
	{
	public:
		//描画位置
		Vec2i TopLeft() const {	return m_TopLeft;	}
		MenuPainter &TopLeft( const Vec2i &TL ){	m_TopLeft = TL;	return *this;	}

		//描画時に項目間に開ける間隔[pixel]
		int ItemSpacing() const {	return m_ItemSpacing;	}
		MenuPainter &ItemSpacing( int space ){	m_ItemSpacing = space;	return *this;	}

		//描画時に項目描画範囲の外周（上下左右）に設ける余白．
		//* 項目群の描画位置は TopLeft() からこの分だけオフセットされる．
		//* Size() が返す値はこれを加味したものとなる．
		Vec2i OuterMargin() const {	return m_OuterMargin;	}
		MenuPainter &OuterMargin( const Vec2i &Margin ){	m_OuterMargin = Margin;	return *this;	}
		
		//描画サイズ
		Vec2i TotalSize( const IMenuContent &Content ) const;

		//外枠描画の有無
		bool WithFrame() const {	return m_bDrawFrame;	}
		MenuPainter &WithFrame( bool Enable ){	m_bDrawFrame = Enable;	return *this;	}

		//メニューのフォーカス状態：描画具合に影響
		bool IsFocused() const {	return m_bFocused;	}
		MenuPainter &IsFocused( bool Focused ){	m_bFocused = Focused;	return *this;	}

		//項目の描画範囲
		Rect ItemDrawRECT( const IMenuContent &Content, int index ) const;

		//描画
		void Draw( HDC hdc, const IMenuContent &Content ) const;

	private:
		IMenuContent *m_pContent = nullptr;
		Vec2i m_TopLeft;
		int m_ItemSpacing = 0;
		Vec2i m_OuterMargin;
		bool m_bDrawFrame = false;
		bool m_bFocused = false;
	};


	//
	enum class HandleInputResult
	{
		None,	//有効な操作が行われなかった（何も変化無し）
		CursorMoved,	//カーソル位置が変化した
		Selected,	//項目選択操作が成された
		Canceled	//キャンセル操作が成された
	};

	HandleInputResult HandleInput( IMenuContent &Content, const IController &Controller );
}
