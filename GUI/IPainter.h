#pragma once
#include "Parts/Vec2D.h"
#include "Parts/Rect.h"

namespace GUI
{
	/// <summary>
	/// 何かを描画するやつ．
	/// 
	/// * 位置やサイズに関するメソッドを持つ
	/// </summary>
	class IPainter
	{
	public:
		virtual ~IPainter() = default;

	public:
		/// <summary>可視性（描画処理の有効/無効）</summary>
		/// <returns></returns>
		bool Visible() const {	return m_IsVisible;	}

		/// <summary>可視性（描画処理の有効/無効）の変更</summary>
		/// <param name="Visiblity"></param>
		/// <returns>*this</returns>
		IPainter &Visible( bool Visiblity ){	m_IsVisible=Visiblity;	return *this;	}

		/// <summary>
		/// 描画処理．ただし Visible() がfalseの状態では何もしない
		/// </summary>
		/// <param name="hdc">描画対象</param>
		void Paint( HDC hdc ) const {	if( Visible() )Paint_(hdc);	}

	public:
		/// <summary>描画範囲（最小包括矩形）の左上座標の取得</summary>
		/// <returns>左上座標</returns>
		virtual Vec2i TopLeft() const = 0;

		/// <summary>描画範囲（最小包括矩形）の左上座標の設定</summary>
		/// <param name="TL">座標</param>
		/// <returns>*this</returns>
		virtual IPainter &TopLeft( const Vec2i &TL ) = 0;

		/// <summary>描画範囲（最小包括矩形）のサイズ取得</summary>
		/// <returns>サイズ</returns>
		virtual Vec2i Size() const = 0;

	protected:
		/// <summary>描画処理</summary>
		/// <param name="hdc">描画対象</param>
		virtual void Paint_( HDC hdc ) const = 0;

	public:	//ヘルパ
		/// <summary>描画範囲（最小包括矩形）取得</summary>
		/// <returns>範囲</returns>
		Rect BoundingRect() const
		{
			auto size = Size();
			return Rect( TopLeft(), size[0], size[1] );
		}

		/// <summary>位置をオフセット</summary>
		/// <param name="dPos">オフセット量</param>
		/// <returns>*this</returns>
		IPainter &Offset( const Vec2i &dPos ){	return TopLeft( TopLeft()+dPos );	}

		/// <summary>x方向位置を 幅 W の領域の中心にセット</summary>
		/// <param name="W">配置範囲幅</param>
		/// <returns>*this</returns>
		IPainter &XCenter( int W ){	return TopLeft( Vec2i{ ( W - Size()[0] )/2, TopLeft()[1] } );	}

		/// <summary>y方向位置を 高さ H の領域の中心にセット</summary>
		/// <param name="H">配置範囲高さ</param>
		/// <returns>*this</returns>
		IPainter &YCenter( int H ){	return TopLeft( Vec2i{ TopLeft()[0], ( H - Size()[1] )/2 } );	}

	private:
		bool m_IsVisible = true;
	};
}
