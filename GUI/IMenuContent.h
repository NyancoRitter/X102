#pragma once
#include "Parts/Vec2D.h"
#include <vector>

class IController;

namespace GUI::Menu
{
	class IMenuItem;

	/// <summary>
	/// メニューの内容．
	/// 項目群と現在カーソル位置の情報を保持．
	/// 
	///		<remarks>
	///		ここでいうメニューとは：
	///		* 項目が（縦並びor横並びで)１次元に並んでいるもの
	///		* 項目群のいずれをカーソルで指し示すことができる（どの項目も指さない状態もあり得る）
	///		</remarks>
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
		virtual Vec2i ItemDrawSize() const = 0;

		/// <summary>メニュー項目数</summary>
		/// <returns></returns>
		virtual int nItems() const = 0;

		/// <summary>
		/// 項目を参照．
		/// indexが範囲外の場合は例外送出される
		/// </summary>
		/// <param name="index">項目index</param>
		/// <returns>項目への参照</returns>
		virtual const IMenuItem &Item( int index ) const = 0;

		/// <summary>現在のカーソル位置</summary>
		/// <returns>カーソルが指している項目のindex．ただし負の値は「どの項目も指していない」状態であることを示す</returns>
		virtual int CursorPos() const = 0;

		/// <summary>
		/// カーソル位置の変更．
		/// カーソルを合わせられない位置（例えば範囲外：負の値や項目個数以上の値）を指定した場合，負の値（どの項目も指さない状態）に設定される．
		/// </summary>
		/// <param name="pos">カーソルを合わせる項目のindex</param>
		/// <returns>カーソル位置が変化したか否か</returns>
		virtual bool CursorPos( int pos ) = 0;

		/// <summary>カーソル位置を１つ次に移動</summary>
		/// <returns>カーソル位置が変化したか否か</returns>
		virtual bool IncCursorPos() = 0;

		/// <summary>カーソル位置を１つ前に移動</summary>
		/// <returns>カーソル位置が変化したか否か</returns>
		virtual bool DecCursorPos() = 0;
	};


	/// <summary>
	/// MenuItem_t 型を項目データ型とする IMenuContent 実装のひな型．
	/// 項目並び方向も template で指定．
	/// </summary>
	/// <typeparam name="MenuItem_t">メニュー項目の型．IMenuItemからの派生クラス</typeparam>
	template< bool IsVertical, class MenuItem_t >
	class MenuContent : public IMenuContent
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemDrawSize">項目の描画サイズを指定</param>
		MenuContent( const Vec2i &ItemDrawSize ) : m_ItemDrawSize(ItemDrawSize) {}

		virtual ~MenuContent() = default;

	public:
		//
		//※項目の追加手段が無いので派生して追加する必要がある
		//

		/// <summary>項目を全破棄</summary>
		void Clear(){	m_Items.clear();	m_CursorPos = -1;	}

		/// <summary>項目データ保持vectorのreserve()を実施</summary>
		/// <param name="n">データ保持vectorのreserve()への引数</param>
		void Reserve( size_t n ){	m_Items.reserve(n);	}

		/// <summary>項目データへのアクセス</summary>
		/// <param name="index">項目index（0-based, Add()した順）</param>
		/// <returns>項目データへの参照</returns>
		MenuItem_t &Item( int index )
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	public:	// IMenuContent Impl
		virtual bool IsVerticalMenu() const override {	return IsVertical;	}
		virtual Vec2i ItemDrawSize() const override {	return m_ItemDrawSize;	}
		virtual int nItems() const override {	return (int)m_Items.size();	}

		virtual int CursorPos() const override {	return m_CursorPos;	}
		virtual bool CursorPos( int pos ) override {	return ChangeCursorPosTo( (0<=pos && pos<nItems())  ?  pos  :  -1 );	}
		virtual bool IncCursorPos() override {	return ChangeCursorPosTo( m_CursorPos+1<nItems()  ?  m_CursorPos+1  :  ( m_Items.empty() ? -1 : 0 ) );	}
		virtual bool DecCursorPos() override {	return ChangeCursorPosTo( m_CursorPos>0  ?  m_CursorPos-1  :  nItems()-1 );	}

		virtual const MenuItem_t &Item( int index ) const override
		{
			if( index<0 || index>=nItems() )throw std::exception( "Invalid index" );
			return m_Items[index];
		}

	private:
		//カーソル位置変更処理．
		//引数位置が現在値と同一あれば何もせずにfalseを返す．
		//そうでないならカーソル位置を引数値に更新してtrueを返す．
		bool ChangeCursorPosTo( int NewPos )
		{
			if( m_CursorPos == NewPos )return false;
			m_CursorPos = NewPos;
			return true;
		}
	private:
		Vec2i m_ItemDrawSize;
		int m_CursorPos = -1;
	protected:
		std::vector< MenuItem_t > m_Items;
	};
}
