#pragma once
#include "GUI/IPainter.h"
#include "Misc/Cmd.h"

class PainterSeq;

namespace Maze
{
	/// <summary>
	/// 迷路に入った際の表示用．
	/// 最初に一定期間，画面上部に文字列を表示するだけの用途
	/// </summary>
	class TitleEffect : public GUI::IPainter
	{
	public:
		TitleEffect( PainterSeq &PStoJoin, int Period ) : m_PStoJoin(PStoJoin), m_Period( Period ) {}
	public:
		Flags<CmdResult> operator()();
	public:	// IPainter Impl
		virtual void Paint_( HDC hdc ) const override;
	private:
		PainterSeq &m_PStoJoin;
		int m_Period;
		int m_Counter = 0;
	};
}
