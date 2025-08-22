#pragma once

#include "IGUI.h"
#include <memory>
#include <queue>
#include <string>
#include "Parts/Rect.h"

namespace GUI
{
	class TextLinePainter;

	/// <summary>
	/// シンプルなメッセージウィンドウ
	/// 
	/// * 指定されたテキストをctorで指定された行数ずつ表示する
	/// * 描画具合：
	///		* テキスト表示域は枠で囲まれる
	///			* 描画域は，X方向に関しては常に中央ぞろえとなる．Y方向に関してはctorで指定．
	///		* 表示に続きがある場合にはその旨を示すための▼が点滅描画される
	/// </summary>
	class MsgWnd : public IGUI
	{
	public:
		//表示の垂直位置指定用
		enum class VerticalLocation{	TOP, CENTER, BOTTOM	};

	public:
		/// <summary>ctor．</summary>
		/// <param name="nLinesPerPage">一度に表示する行数</param>
		/// <param name="VerticalLoc">Y方向の位置の指定</param>
		/// <param name="Width">描画幅[pixel]．0以下を指定した場合にはおおよそ左右方向全域を占めるサイズとなる</param>
		MsgWnd(
			int nLinesPerPage = 3,
			VerticalLocation VerticalLoc=VerticalLocation::BOTTOM,
			int Width = 0
		);

		~MsgWnd();

	public:
		//テキストの横方向位置指定．（いずれも *this を返す）
		MsgWnd &AlignCenter();
		MsgWnd &AlignLeft();
		MsgWnd &AlignRight();

		/// <summary>
		/// 表示すべき文字列群(UTF16)を指定
		/// </summary>
		/// <typeparam name="LineIter"></typeparam>
		/// <param name="begin"></param>
		/// <param name="end"></param>
		/// <returns>*this</returns>
		template< class LineIter >
		MsgWnd &TextLines( LineIter begin, LineIter end )
		{
			std::queue<std::wstring> NewLines;
			for( LineIter i=begin; i!=end; ++i ){	NewLines.emplace(*i);	}
			m_RestLines.swap( NewLines );

			ToNextPage();
			return *this;
		}

		/// <summary>表示すべき文字列群(UTF16)を指定</summary>
		/// <param name="LinesU16"></param>
		/// <returns>*this</returns>
		MsgWnd &TextLines( std::initializer_list<std::wstring> LinesU16 )
		{	return TextLines( LinesU16.begin(), LinesU16.end() );	}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void ToNextPage();
	private:
		std::unique_ptr<TextLinePainter> m_upTextPainter;
		Rect m_FrameRect;
		int m_nLinesPerPage = 1;
		std::queue<std::wstring> m_RestLines;	//まだ表示していないテキスト行群

		//テキストに続きがあることを示すマークの表示用
		int m_NextPageIndicatorAnimCounter = 0;
		bool m_bDispNextPageIndicator = false;
		static constexpr int ms_NextPageIndicatorAnimInterval = 12;
	};
}
