#pragma once

#include "IPainter.h"

class CMonoBMP;

namespace GUI
{

	/// <summary>
	/// モノクロ画像を描画する
	/// </summary>
	class MonoImgPainter : public IPainter
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="pBMP">描画する画像</param>
		MonoImgPainter( const CMonoBMP *pBMP = nullptr );

	public:
		/// <summary>
		/// 描画対象画像を指定
		/// </summary>
		/// <param name="pBMP"></param>
		/// <returns></returns>
		MonoImgPainter &SetImg( const CMonoBMP *pBMP ){	m_pBMP = pBMP;	return *this;	}

		/// <summary>
		/// 画像描画色の指定
		/// </summary>
		/// <param name="col">画素値1の個所の描画色</param>
		/// <returns>*this</returns>
		MonoImgPainter &SetColor( COLORREF col ){	m_Color = col;	return *this;	}

		/// <summary>
		/// 画素値0の箇所を透過するか否か：デフォルトは透過
		/// </summary>
		/// <param name="Trans">透過するならtrue</param>
		/// <returns>*this</returns>
		MonoImgPainter &TransBit0Part( bool Trans ){	m_TransBit0Part=Trans;	return *this;	}

	public:	// IPainter Impl
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual MonoImgPainter &TopLeft( const Vec2i &TL ) override  {	m_TopLeft=TL;	return *this;	}

		//描画対象画像のサイズを返す．ただし画像が設定されていない状況では(0,0)を返す．
		virtual Vec2i Size() const override;
	protected:
		/// <summary>描画処理．画像が設定されていない場合には何もしない</summary>
		/// <param name="hdc">描画対象</param>
		virtual void Paint_( HDC hdc ) const override;

	private:
		COLORREF m_Color;
		bool m_TransBit0Part = true;
		const CMonoBMP *m_pBMP;
		Vec2i m_TopLeft;
	};
}
