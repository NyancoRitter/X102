#pragma once
#include "GUI/IPainter.h"
#include "Misc/Cmd.h"

class PainterSeq;

namespace Maze
{
	/// <summary>
	/// ���H�ɓ������ۂ̕\���p�D
	/// �ŏ��Ɉ����ԁC��ʏ㕔�ɕ������\�����邾���̗p�r
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
