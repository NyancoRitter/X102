#pragma once
#include "GUI/IMenuItem.h"
#include "GUI/IMenuContent.h"
#include "GameContent/PartyCharID.h"
#include <vector>

namespace GUI::Menu
{

	/// <summary>
	/// キャラクタの顔画像とHP状態を表示するメニュー項目
	/// </summary>
	class CharSelMenuItem : public IMenuItem
	{
	public:
		CharSelMenuItem( GameContent::PartyCharID TgtChar, int HP=0, int MaxHP=0 );
		CharSelMenuItem &HPInfo( int HP, int MaxHP ){	m_HP=HP;	m_MaxHP=MaxHP;	return *this;	}

	public:
		/// <summary>この項目がカーソル位置にある場合に，描画位置を Draw() で指定された個所からオフセットする量</summary>
		/// <param name="Offset">オフセット量</param>
		/// <returns>*this</returns>
		CharSelMenuItem &OffsetWhenCursorPos( const Vec2i &Offset ){	m_OffsetWhenCursorPos=Offset;	return *this;	}

	public:	// IMenuItem Impl
		virtual COLORREF DrawColor() const override {	return m_Color;	}
		virtual CharSelMenuItem &DrawColor( COLORREF Color ) override {	m_Color=Color;	return *this;	}
		virtual void Draw( HDC hdc, const Rect &ItemDrawReg, bool IsAtCursorPos, bool IsMenuFocused, bool ShouldDrawCursor ) const override;

	private:
		GameContent::PartyCharID m_TgtChar;
		COLORREF m_Color;
		int m_HP;
		int m_MaxHP;
		Vec2i m_OffsetWhenCursorPos;
	};

	/// <summary>
	/// CharSelMenuItem 型を項目データ型とする IMenuContent 実装．
	/// 項目並び方向を template で指定．
	/// </summary>
	template< bool IsVertical >
	class CharSelMenuContent : public MenuContent<IsVertical,CharSelMenuItem>
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="CursorPosItemOffset">カーソル位置にある項目の描画位置をオフセットする量</param>
		/// <param name="ItemDrawSize">項目の描画サイズを指定</param>
		CharSelMenuContent(
			const Vec2i &CursorPosItemOffset,
			const Vec2i &ItemDrawSize = Vec2i{ 160, 64 }
		)
			: MenuContent<IsVertical,CharSelMenuItem>(ItemDrawSize)
			, m_CursorPosItemOffset( CursorPosItemOffset )
		{}

	public:
		/// <summary>項目を追加</summary>
		/// <param name="TgtChar">キャラクタID</param>
		/// <returns>追加された項目への参照を返す</returns>
		CharSelMenuItem &Add( GameContent::PartyCharID TgtChar )
		{
			this->m_Items.emplace_back( TgtChar );
			return this->m_Items.back().OffsetWhenCursorPos( m_CursorPosItemOffset );
		}

	private:
		Vec2i m_CursorPosItemOffset;
	};

}
