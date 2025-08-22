#pragma once
#include "IPainter.h"
#include "IRectReg.h"
#include <vector>
#include <string>

namespace GUI
{

	/// <summary>
	/// テキストを何行か表示する
	/// </summary>
	class TextLinePainter : public IPainter, public IRectReg
	{
	public:
		//ctor
		TextLinePainter( int LineW=100, int LineH = 24 );

	public:
		/// <summary>
		/// 行の幅[pixel] を設定．
		/// * この範囲に描画がクリップされるというわけではない：
		///   描画する文字列とこの設定との兼ね合いは利用側でうまく調整する必要がある．
		/// * Size() の戻り値に用いられる
		/// </summary>
		/// <param name="W">幅[pixel]</param>
		/// <returns>*this</returns>
		TextLinePainter &LineWidth( int W ){	m_LineW=W;	return *this;	}

		int LineWidth() const {	return m_LineW;	}

		/// <summary>
		/// １行の高さ[pixel] を設定．
		/// * 各行はこの高さの範囲にY方向中央ぞろえで描画される．
		/// * Size()の戻り値に影響．
		/// </summary>
		/// <param name="H">高さ[pxiel]</param>
		/// <returns>*this</returns>
		TextLinePainter &LineHeight( int H ){	m_LineH=H;	return *this;	}

		int LineHeight() const {	return m_LineH;	}

		/// <summary>
		/// 表示すべきテキストを指定．
		/// LineWidth()等の設定との間でうまいこと調整する（想定描画範囲に文字列描画結果が収まるようにする）のは利用側の責任．
		/// </summary>
		/// <typeparam name="TextLineU16Iter">文字列群指定用イテレータの型</typeparam>
		/// <param name="begin">文字列(行)群指定</param>
		/// <param name="end">文字列(行)群</param>
		/// <returns>*this</returns>
		template<class TextLineU16Iter >
		TextLinePainter &TextLines( TextLineU16Iter begin, TextLineU16Iter end )
		{	m_TextLinesU16.assign( begin, end );	return *this;	}

		/// <summary>
		/// 表示すべきテキストを指定．
		/// LineWidth()等の設定との間でうまいこと調整する（想定描画範囲に文字列描画結果が収まるようにする）のは利用側の責任．
		/// </summary>
		/// <param name="TextLinesU16">テキストデータ(UTF16)．複数行表示する場合はその分だけの文字列群．</param>
		/// <returns>*this</returns>
		TextLinePainter &TextLines( std::vector<std::wstring> TextLinesU16 )
		{	m_TextLinesU16 = std::move(TextLinesU16);	return *this;	}

		// テキストの水平方向描画位置の設定
		TextLinePainter &AlignCenter();
		TextLinePainter &AlignLeft();
		TextLinePainter &AlignRight();

		/// <summary>文字の描画色の指定</summary>
		/// <param name="col">描画色</param>
		/// <returns>*this</returns>
		TextLinePainter &SetColor( COLORREF col ){	m_Color = col;	return *this;	}

	public:	// IRectReg Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual TextLinePainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}
		virtual Vec2i Size() const override {	return Vec2i{ m_LineW, m_LineH*(int)m_TextLinesU16.size() };	}
	protected:	// IPainter Impl
		virtual void Paint_( HDC hdc ) const override;

	private:
		int m_LineW;
		int m_LineH;
		Vec2i m_TopLeft;
		UINT m_HorizontalFormat;
		std::vector< std::wstring > m_TextLinesU16;
		COLORREF m_Color;
	};

}
