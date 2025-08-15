#pragma once

#include "IPainter.h"
#include "Parts/Rect.h"

namespace GUI
{

	/// <summary>
	/// 矩形範囲の設定を有するもの（描画範囲の設定を想定）
	/// </summary>
	class IRectReg
	{
	public:
		virtual ~IRectReg() = default;
	public:
		/// <summary>描画範囲（最小包括矩形）の左上座標の取得</summary>
		/// <returns>左上座標</returns>
		virtual Vec2i TopLeft() const = 0;

		/// <summary>描画範囲（最小包括矩形）の左上座標の設定</summary>
		/// <param name="TL">座標</param>
		/// <returns>*this</returns>
		virtual IRectReg &TopLeft( const Vec2i &TL ) = 0;

		/// <summary>描画範囲（最小包括矩形）のサイズ取得</summary>
		/// <returns>サイズ</returns>
		virtual Vec2i Size() const = 0;

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
		IRectReg &Offset( const Vec2i &dPos ){	return TopLeft( TopLeft()+dPos );	}

		/// <summary>x方向位置を 幅 W の領域の中心にセット</summary>
		/// <param name="RangeW">配置範囲幅</param>
		/// <param name="RangeLeft">配置範囲の左端座標</param>
		/// <returns>*this</returns>
		IRectReg &XCenter( int RangeW, int RangeLeft=0 ){	return TopLeft( Vec2i{ RangeLeft + ( RangeW - Size()[0] )/2, TopLeft()[1] } );	}

		/// <summary>y方向位置を 高さ H の領域の中心にセット</summary>
		/// <param name="RangeH">配置範囲高さ</param>
		/// <param name="RangeTop">配置範囲の上端座標</param>
		/// <returns>*this</returns>
		IRectReg &YCenter( int RangeH, int RangeTop=0 ){	return TopLeft( Vec2i{ TopLeft()[0], RangeTop + ( RangeH - Size()[1] )/2 } );	}
	};

}
