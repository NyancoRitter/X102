#pragma once

#include "IMenuItem.h"
#include "IMenuContent.h"
#include <functional>
#include <string>

class CMonoBMP;

namespace GUI::Menu
{
	/// <summary>
	/// 汎用 IMenuItem 実装．
	/// いくつかの描画パーツで構成されたメニュー項目．
	/// 
	/// ctorで指定したパーツ群を左側から横並びに描画する．
	/// 各パーツの型は以下の呼び出しで描画処理が可能なもの．
	///		
	/// int ( HDC hdc, const Rect &Reg, COLORREF Color ) const;
	///		Reg はメニュー項目の描画範囲からこのパーツよりも左側のパーツ群が占める範囲を除いたもの．
	///		Colorは項目の描画色である．
	/// 
	///		戻り値は，意味的にはこのパーツが横方向に占めるサイズ[pixel]を返すもの．
	///		パーツは左から順に描画されていくが，その過程で次のパーツの描画範囲を決めるのに用いられる．
	///		具体的には右隣りのパーツの描画範囲（のleft）はこの分だけ右にずらされる．
	///		（故に，最も右端のパーツが返す値は描画に影響を与えない）
	/// </summary>
	class GenericMenuItem : public IMenuItem
	{
	public:
		using PartFunc = std::function< int( HDC, const Rect &, COLORREF ) >;

		/// <summary>ctor</summary>
		/// <param name="Parts">パーツ群を指定</param>
		GenericMenuItem( std::initializer_list<PartFunc> Parts );

	public:	// IMenuItem Impl
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused ) const override;
		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual GenericMenuItem &DrawColor( COLORREF Color ) override {	m_Color = Color;	return *this;	}

	private:
		std::vector< PartFunc > m_Parts;
		COLORREF m_Color;
	};

	/// <summary>
	/// GenericMenuItem 型を項目データ型とする IMenuContent 実装．
	/// 項目並び方向を template で指定．
	/// </summary>
	template< bool IsVertical >
	class GenericMenuContent : public MenuContent<IsVertical,GenericMenuItem>
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemDrawSize">項目の描画サイズを指定</param>
		GenericMenuContent( const Vec2i &ItemDrawSize )
			: MenuContent<IsVertical,GenericMenuItem>(ItemDrawSize)
		{}

		virtual ~GenericMenuContent() = default;

	public:
		/// <summary>項目を追加 </summary>
		/// <param name="...ItemParts">項目構成要素群</param>
		/// <returns>*this</returns>
		GenericMenuContent &Add( std::initializer_list< GenericMenuItem::PartFunc > ItemParts )
		{
			this->m_Items.emplace_back( ItemParts );
			return *this;
		}
	};

	//------------------------------------

	/// <summary>メニュー項目パーツ：Textを描画する</summary>
	class Text
	{
	public:
		/// <summary>ctor </summary>
		/// <param name="TextU16">描画Text</param>
		/// <param name="W">描画範囲の幅. 0以下を指定した場合は Draw() の引数範囲をそのまま描画範囲とする</param>
		Text( const std::wstring &TextU16, int W=0 ) : m_TextU16(TextU16), m_XSize(W) {}

	public:	//as Menu Item Part
		int operator()( HDC hdc, const Rect &RestReg, COLORREF Color ) const;

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
		int operator()( HDC hdc, const Rect &Reg, COLORREF Color ) const;

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