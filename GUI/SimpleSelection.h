#pragma once

#include <functional>
#include <string>
#include "IGUI.h"
#include "GenericMenuContent.h"
#include "Menu.h"

namespace GUI::Menu
{

	/// <summary>
	/// 単純な選択肢UI
	/// 
	/// 1行のテキストの下に（横線を挟んで）単純な選択メニューを表示する．
	///		<remarks>ちょっとした Yes/No みたいなのを出す用</remarks>
	/// </summary>
	class SimpleSelection : public IGUI
	{
	public:
		SimpleSelection( const std::wstring &MsgText, const Vec2i &ItemDrawSize )
			: m_Content( ItemDrawSize ), m_MsgText(MsgText)
		{
			m_Menu.SetContent( &m_Content ).WithFrame(false);
			UpdateMenuPos();
		}

	public:
		SimpleSelection &Reserve( size_t n ){	m_Content.Reserve(n);	return *this;	};

		/// <summary>選択肢の追加</summary>
		/// <param name="ItemText">選択肢文字列</param>
		/// <returns>*this</returns>
		SimpleSelection &Add( const std::wstring &ItemText );

		/// <summary>カーソル位置設定</summary>
		/// <param name="pos">カーソル位置</param>
		/// <returns>*this</returns>
		SimpleSelection &CursorPos( int pos ){	m_Content.CursorPos(pos);	return *this;	}

		/// <summary>
		/// 選択操作が成されたときのコールバックを設定．
		/// これを指定しない場合，選択操作をしても何も起きない．
		/// </summary>
		/// <param name="Callback">
		/// * 引数は選択された項目のindex．
		///   必ずいずれかの選択肢を指し示す（：カーソルが選択肢を指し示さない状態ではこのコールバックは呼ばれない）．
		/// * 戻り値はこの選択肢UIを用いた処理が完了したか否か．
		/// </param>
		/// <returns>*this</returns>
		SimpleSelection &OnSelect(
			std::function<bool(int)> Callback
		)
		{	m_OnSelect=Callback;	return *this;	}

		/// <summary>
		/// 全体の描画幅を指定．
		/// 指定しない場合，何らかのデフォルトサイズとなる．
		/// </summary>
		/// <param name="w">幅</param>
		/// <returns>*this</returns>
		SimpleSelection &Width( int w ){	m_W=w;	UpdateMenuPos();	return *this;	}

		/// <summary>
		/// キャンセル操作が可能か？　デフォルトはtrue．
		/// </summary>
		/// <param name="val">キャンセル可能とするならtrue</param>
		/// <returns>*this</returns>
		SimpleSelection &Cancelable( bool val ){	m_bCancelable=val;	return *this;	}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
		virtual void OnGotFocus() override;
		virtual void OnLostFocus() override;
	
		virtual Vec2i TopLeft() const override {	return m_TopLeft;	}
		virtual SimpleSelection &TopLeft( const Vec2i &TL ) override {	m_TopLeft=TL;	UpdateMenuPos();	return *this;	}
		virtual Vec2i Size() const override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void UpdateMenuPos();

	private:
		GenericMenuContent<true> m_Content;
		Menu m_Menu;
		std::wstring m_MsgText;
		Vec2i m_TopLeft;
		int m_W = 480;
		bool m_bCancelable = true;
		std::function<bool(int)> m_OnSelect;
	};

}