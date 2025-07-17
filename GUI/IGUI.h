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

		//GUIStack の描画処理の起点となるべきか否か．
		//trueを返す場合，このインスタンス以前に GUIStack にPushされているインスタンスの Paint() の呼び出しを抑制する．
		//このメソッドの戻り値はインスタンス毎に定数であること（常に同じ値を返さねばならない）．
		virtual bool IsPaintStart() const {	return false;	}

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
	/// * Paint() は Push 順にコールする．
	///   ただし IGUI::IsPaintStart() がtrueを返す物がある場合，
	///   その中で最も最後に Push された物が Paint() コールの先頭となる．
	///   （つまり，それよりも前に Push された物の Paint() は呼ばれない）
	/// </summary>
	class GUIStack : public IGUIStack
	{
	public:
		GUIStack(){	m_iPaintBegin = m_GUIs.end();	}
		~GUIStack(){	clear();	}

	private:
		GUIStack( const GUIStack & ) = delete;
		GUIStack &operator =( const GUIStack & ) = delete;

	public:	// IGUIStack Impl
		virtual GUIStack &Push( const std::unique_ptr<IGUI> upGUI ) override
		{
			if( !upGUI )return *this;

			const bool FromEmptyState = m_GUIs.empty();

			if( !FromEmptyState )
			{	m_GUIs.back()->OnLostFocus();	}

			m_GUIs.push_back( std::move(upGUI) );
			m_GUIs.back()->OnGotFocus();

			if( FromEmptyState  ||  m_GUIs.back()->IsPaintStart() )
			{	m_iPaintBegin = std::prev( m_GUIs.end() );	}

			return *this;
		}

	public:
		bool empty() const {	return m_GUIs.empty();	}

		void clear()
		{
			m_GUIs.clear();
			m_iPaintBegin = m_GUIs.end();
		}

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

		/// <summary>
		/// 所属要素群の Paint() を Push 順にコールする．
		/// ただし GUIPart::IsPaintStart() がtrueを返す物がある場合，
		/// その中で最も最後に Push された物が Paint() コールの先頭となる．
		/// （つまり，それよりも前に Push された物の Paint() は呼ばれない）
		/// </summary>
		/// <param name="hdc"></param>
		void Paint( HDC hdc )
		{
			for( auto i=m_iPaintBegin; i!=m_GUIs.end(); ++i )
			{	(*i)->Paint( hdc );	}
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
			//m_iPaintBegin の更新作業
			auto ri = std::find_if( m_GUIs.rbegin(), m_GUIs.rend(), []( const auto &upGUI )->bool{	return upGUI->IsPaintStart();	} );
			if( ri != m_GUIs.rend() )
			{	m_iPaintBegin = --(ri.base());	}
			else
			{	m_iPaintBegin = m_GUIs.begin();	}
		}

	private:
		std::list<   std::unique_ptr< IGUI >   > m_GUIs;	//最後にPushされた物が back 側
		Iter m_iPaintBegin;	//描画開始位置
	};
}
