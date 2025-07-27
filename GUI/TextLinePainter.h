#pragma once
#include "IPainter.h"
#include <vector>
#include <string>

namespace GUI
{

	class TextLinePainter : public IPainter
	{
	public:
		TextLinePainter( int LineH = 24 ) : m_LineH(LineH) {	AlignCenter();	}

	public:
		/// <summary>
		/// 描画範囲のサイズを設定．
		/// 
		/// 描画がこのサイズ内にクリップされるわけではなく，各種計算に用いられるだけ．
		/// 描画する文字列や行の高さなどの設定は整合性等は利用側が担保する必要がある．
		/// </summary>
		/// <param name="S">範囲サイズ</param>
		/// <returns>*this</returns>
		TextLinePainter &Size( const Vec2i &S ){	m_Size=S;	return *this;	}

		//１行の高さ[pixel]
		TextLinePainter &LineHeight( int H ){	m_LineH=H;	return *this;	}
		int LineHeight() const {	return m_LineH;	}

		/// <summary>
		/// 表示すべきテキストを指定．
		/// Size() や LineHeight() との間での整合性を保つ（テキストの行数や横幅が描画範囲に収まるようにする）ことは利用側の責任．
		/// </summary>
		/// <param name="rTextLinesU16">
		/// テキストデータ(UTF16)．複数行表示する場合はその分だけの文字列群．
		/// </param>
		/// <returns>*this</returns>
		TextLinePainter &TextLines( const std::vector<std::wstring> &rTextLinesU16 )
		{
			m_TextLinesU16 = rTextLinesU16;
			return *this;
		}

		// テキストの水平方向描画位置の設定
		TextLinePainter &AlignCenter();
		TextLinePainter &AlignLeft();
		TextLinePainter &AlignRight();

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual TextLinePainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}
		virtual Vec2i Size() const override {	return m_Size;	}
	protected:
		/// <summary>描画処理</summary>
		/// <param name="hdc">描画対象</param>
		virtual void Paint_( HDC hdc ) const override;

	private:
		int m_LineH;
		Vec2i m_TopLeft;
		Vec2i m_Size;
		UINT m_HorizontalFormat;
		std::vector< std::wstring > m_TextLinesU16;
	};

}
