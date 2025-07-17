#pragma once

#include "IPainter.h"
#include "Parts/Flags.h"
#include <list>
#include <memory>

class IController;

namespace GUI
{
	class IGUIStack;

	/// <summary>
	/// IGUI.Exec()の戻り値用ビットフラグ
	/// </summary>
	enum class GUIResult : unsigned int
	{
		/// <summary>特に無し</summary>
		None = 0,

		/// <summary>
		/// 所属StackからPopされるべきことを示す．
		/// </summary>
		ShouldPop =  1,

		/// <summary>
		/// 再描画が必要であることを示す．
		/// </summary>
		ReqRedraw = 1 << 1,
	};

	/// <summary>
	/// GUI構成物．
	/// 入力を処理できて，状況を描画できるもの．
	/// 
	/// GUIStackにPushして使う型．
	/// </summary>
	class IGUI : public IPainter
	{
	public:
		virtual ~IGUI() = default;
	public:
		/// <summary>更新処理</summary>
		/// <param name="Stack">このUpdateを呼び出したGUIStack（つまりこのインスタンスが所属するGUIStack）への要素追加手段</param>
		/// <param name="Controller">入力</param>
		/// <returns>再描画が必要か否か．</returns>
		virtual Flags<GUIResult> Update( IGUIStack &Stack, const IController &Controller ) = 0;

		/// <summary>
		/// Update()が呼ばれる対象になった際
		/// （所属するStackの他の要素がPopされた結果としてそうなった場合，あるいは自身がStackにPushされた場合）に呼ばれる．
		/// </summary>
		virtual void OnGotFocus(){}

		/// <summary>
		/// Update()が呼ばれる対象ではなくなった際 （所属するStackに他要素がPushされた場合）に呼ばれる．
		/// このオブジェクトが Stack から Pop された際には呼ばれない．
		/// </summary>
		virtual void OnLostFocus(){}
	};

	/// <summary>
	/// スタックへの要素Push手段
	/// </summary>
	class IGUIStack
	{
	public:
		virtual ~IGUIStack() = default;

		/// <summary>
		/// スタックへの要素追加
		/// </summary>
		/// <param name="upGUI">
		/// 追加する要素．
		/// * nullptr を指定した場合には何もしない
		/// * 同一対象を多重に追加した場合の動作は保証しない
		/// </param>
		/// <returns>*this</returns>
		virtual IGUIStack &Push( std::unique_ptr<IGUI> upGUI ) = 0;
	};

	/// <summary>
	/// IGUIのスタック的な．以下の仕事をするだけのコンテナみたいな．
	/// 
	/// * 最後に Push された物の Update() をコールする
	/// * Paint() は Push された物全ての Paint() を Push 順にコールする．
	/// </summary>
	class GUIStack : public IGUIStack
	{
	public:
		~GUIStack(){	clear();	}

	private:
		GUIStack( const GUIStack & ) = delete;
		GUIStack &operator =( const GUIStack & ) = delete;

	public:	// IGUIStack Impl
		virtual GUIStack &Push( const std::unique_ptr<IGUI> upGUI ) override
		{
			if( !upGUI )return *this;

			if( !m_GUIs.empty() )
			{	m_GUIs.back()->OnLostFocus();	}

			m_GUIs.push_back( std::move(upGUI) );
			m_GUIs.back()->OnGotFocus();
			return *this;
		}

	public:
		bool empty() const {	return m_GUIs.empty();	}
		void clear(){	m_GUIs.clear();	}

		/// <summary>
		/// 最後に Push された物の Update() をコールする
		/// </summary>
		/// <param name="Controller"></param>
		/// <returns>再描画（ Draw() の実施）が必要か否か</returns>
		bool Update( const IController &Controller )
		{
			if( empty() )return false;

			auto iCurr = std::prev( m_GUIs.end() );
			auto Result = (*iCurr)->Update( *this, Controller );

			bool NeedToRedraw = false;
			if( Result.Has( GUIResult::ShouldPop ) )
			{
				Remove( iCurr );
				NeedToRedraw = true;
			}
			if( Result.Has( GUIResult::ReqRedraw ) )
			{	NeedToRedraw = true;	}

			return NeedToRedraw;
		}

		/// <summary>所属要素群の Paint() を Push 順にコールする</summary>
		/// <param name="hdc"></param>
		void Paint( HDC hdc )
		{
			for( const auto &upGUI : m_GUIs )
			{	upGUI->Paint( hdc );	}
		}

	private:
		using Iter = std::list<   std::unique_ptr< IGUI >   >::iterator;

		/// <summary>
		/// 要素の除去作業．
		/// * 必要に応じて UIPart::OnGotFocus() をコール
		/// * m_iPaintBeginの更新
		/// </summary>
		/// <param name="iTgt">削除対象</param>
		void Remove( Iter iTgt )
		{
			{//要素削除
				bool TgtIsCurrent = ( iTgt == std::prev( m_GUIs.end() ) );
				m_GUIs.erase( iTgt );

				if( TgtIsCurrent && !m_GUIs.empty() )
				{	m_GUIs.back()->OnGotFocus();	}
			}
		}

	private:
		std::list<   std::unique_ptr< IGUI >   > m_GUIs;	//最後にPushされた物が back 側
	};
}
