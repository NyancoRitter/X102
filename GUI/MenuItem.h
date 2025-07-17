#pragma once

#include "Parts/Vec2D.h"
#include "Parts/TupleForEach.h"
#include <string>
#include "GUI_Funcs.h"

class CMonoBMP;

namespace GUI::Menu
{
	/// <summary>
	/// メニュー内の１項目の描画手段
	/// 
	/// * 設定として描画色を保持
	/// </summary>
	class IMenuItem
	{
	public:
		virtual ~IMenuItem() = default;

		/// <summary>項目の描画色</summary>
		/// <returns>描画色</returns>
		virtual COLORREF DrawColor() const = 0;

		/// <summary>項目の描画色を変更</summary>
		/// <returns>*this</returns>
		virtual IMenuItem &DrawColor( COLORREF Color ) = 0;

		/// <summary>
		/// メニュー項目の描画
		/// </summary>
		/// <param name="hdc">描画対象hdc</param>
		/// <param name="ItemDrawReg">項目描画範囲</param>
		/// <param name="IsAtCursorPos">この項目がカーソル位置にあるか否か</param>
		/// <param name="IsMenuFocused">所属メニューがフォーカス状態か否か</param>
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const = 0;
	};

	/// <summary>
	/// 汎用．
	/// いくつかの描画パーツで構成されたメニュー項目．
	/// 
	/// templateで指定したパーツ群を左側から横並びに描画する．
	/// 各パーツの型は以下のメソッドを持つ必要がある．
	/// * int XSize() const;
	///		意味的にはこのパーツが横方向に占めるサイズ[pixel]を返すもの．
	///		パーツは左から順に描画されていくが，その過程で次のパーツの描画範囲を決めるのに用いられる．
	///		具体的には右隣りのパーツの描画範囲（のleft）はこの分だけ右にずらされる．
	///		（故に，最も右端のパーツが返す値は描画に影響を与えない）
	/// * void Draw( HDC hdc, const Rect &Reg, COLORREF Color ) const;
	///		描画処理．
	///		Reg はメニュー項目の描画範囲からこのパーツよりも左側のパーツ群が占める範囲を除いたもの．
	///		Colorは項目の描画色である．
	/// </summary>
	/// <typeparam name="...DrawParts">項目を構成するパーツ（描画要素）群</typeparam>
	template< class ...DrawParts >
	class MenuItem : public IMenuItem
	{
	public:
		using Parts_t = std::tuple< DrawParts... >;

	public:
		MenuItem( const DrawParts& ...Parts )
			: m_Parts( Parts... )
		{}

		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const override
		{
			Rect RestReg = ItemDrawReg;
			TupleForEach(
				m_Parts,
				[hdc, &RestReg, Color=m_Color]( const auto &Part )
				{
					Part.Draw( hdc, RestReg, Color );
					RestReg.TopLeft()[0] += Part.XSize();
				}
			);
		}

		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual MenuItem &DrawColor( COLORREF Color ) override {	m_Color = Color;	return *this;	}

	private:
		Parts_t m_Parts;
		COLORREF m_Color = 0xFFFFFF;
	};

	//--------------------------
	//MenuItem<> 用のメニュー項目パーツ実装

	/// <summary>メニュー項目パーツ：Textを描画する</summary>
	class Text
	{
	public:
		/// <summary>ctor </summary>
		/// <param name="TextU16">描画Text</param>
		/// <param name="W">描画範囲の幅</param>
		Text( const std::wstring &TextU16, int W=0 ) : m_TextU16(TextU16), m_XSize(W) {}

	public:	//as Menu Item Part
		int XSize() const {	return m_XSize;	}
		void Draw( HDC hdc, const Rect &RestReg, COLORREF Color ) const;

	public:	//描画範囲内での水平方向描画位置の設定：デフォルトは左揃え
		Text &AlignCenter(){	m_XPosFormat=DT_CENTER;	return *this;	}
		Text &AlignLeft(){	m_XPosFormat=DT_LEFT;	return *this;	}
		Text &AlignRight(){	m_XPosFormat=DT_RIGHT;	return *this;	}

	private:
		int m_XSize;
		std::wstring m_TextU16;
		UINT m_XPosFormat = DT_LEFT;	//デフォルトは左揃え
	};

	/// <summary>メニュー項目パーツ：アイコンを描画する</summary>
	class Icon
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="spIcon">
		/// アイコン．
		/// 必須：参照先オブジェクトはこのオブジェクトよりも生存期間が長いこと．
		/// </param>
		/// <param name="W">
		/// 描画範囲の幅．
		/// アイコン画像のサイズとは無関係にこの値が XSize() の戻り値となる．
		/// 負の値を指定した場合には，アイコンのサイズに設定される．
		/// </param>
		Icon( const CMonoBMP &rIcon, int W=-1 );

	public:	//MenuItemPart
		int XSize() const {	return m_XSize;	}
		void Draw( HDC hdc, const Rect &Reg, COLORREF Color ) const;

	public:
		//描画範囲内での水平方向描画位置の設定：デフォルトは左揃え
		Icon &AlignCenter();
		Icon &AlignLeft(){	m_XPosOffset=0;	return *this;	}
		Icon &AlignRight();

		//画素値0の箇所を透過するか否か：デフォルトは透過
		Icon &TransBit0Part( bool Trans ){	m_TransBit0Part=Trans;	return *this;	}
	private:
		int m_XSize;
		const CMonoBMP &m_rIcon;
		int m_XPosOffset = 0; 
		bool m_TransBit0Part = true;
	};
}