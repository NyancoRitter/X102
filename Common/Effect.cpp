#include "framework.h"
#include "Effect.h"

#include "ValueImgPainter.h"

namespace
{
	class HPRecovEffect
	{
	public:
		HPRecovEffect(
			PainterSeq &PStoJoin,
			int Amount,
			const Vec2i &Center,
			int Period,
			int SuppressSubsequentsPeriod
		)
			: m_PStoJoin(PStoJoin)
			, m_Painter( Amount )
			, m_Period( Period )
			, m_SuppressSubsequentsPeriod( SuppressSubsequentsPeriod )
		{
			m_Painter.Color( RGB(0,255,255) )
				.MagRate( 2 )
				.TopLeft( Center - m_Painter.Size()/2 );
		}

		Flags<CmdResult> operator()()
		{
			if( m_Counter >= m_Period )
			{
				return ( m_PStoJoin.Remove( &m_Painter )  ?  CmdResult::Finished|CmdResult::ReqRedraw  :  CmdResult::Finished );
			}

			++m_Counter;
			if( m_Counter == 1 )
			{	m_PStoJoin.PushBack( &m_Painter );	}
			else
			{	m_Painter.Offset( {0,-1} );	}

			Flags<CmdResult> Ret = CmdResult::ReqRedraw;
			if( m_Counter <= m_SuppressSubsequentsPeriod )Ret |= CmdResult::SuppressSubsequents;
			return Ret;
		}

	private:
		PainterSeq &m_PStoJoin;
		ValueImgPainter m_Painter;
		int m_Counter = 0;
		int m_Period;
		int m_SuppressSubsequentsPeriod;
	};

	//
	class DmgEffect
	{
	public:
		DmgEffect(
			PainterSeq &PStoJoin,
			int Amount,
			const Vec2i &Center,
			int Period, 
			int HopPeriod,
			int SuppressSubsequentsPeriod
		)
			: m_PStoJoin( PStoJoin )
			, m_Painter( Amount )
			, m_Period( Period )
			, m_HopPeriod( HopPeriod )
			, m_SuppressSubsequentsPeriod( SuppressSubsequentsPeriod )
		{
			m_Painter.Color( RGB(255,128,16) )
				.MagRate( 2 )
				.TopLeft( Center - m_Painter.Size()/2 );

			m_InitPos = m_Painter.TopLeft();
		}

		Flags<CmdResult> operator()()
		{
			if( m_Counter >= m_Period )
			{
				return ( m_PStoJoin.Remove( &m_Painter )  ?  CmdResult::Finished|CmdResult::ReqRedraw  :  CmdResult::Finished );
			}

			++m_Counter;
			if( m_Counter == 1 )
			{	m_PStoJoin.PushBack( &m_Painter );	}
			else if( m_Counter<=m_HopPeriod )
			{	
				m_Painter.TopLeft(   m_InitPos  -  Vec2i( 0, int( 16*sin( 3.1415926 * m_Counter/m_HopPeriod ) ) )   );
			}

			Flags<CmdResult> Ret = CmdResult::ReqRedraw;
			if( m_Counter <= m_SuppressSubsequentsPeriod )Ret |= CmdResult::SuppressSubsequents;
			return Ret;
		}

	private:
		PainterSeq &m_PStoJoin;
		ValueImgPainter m_Painter;
		int m_Counter = 0;
		int m_Period;
		int m_SuppressSubsequentsPeriod;
		int m_HopPeriod;
		Vec2i m_InitPos;
	};

}


Cmd<> CreateHPRecovEffect( PainterSeq &PStoJoin, int Amount, const Vec2i &Center, int Period, int SuppressSubSequentsPeriod )
{	return HPRecovEffect( PStoJoin, Amount, Center, Period, SuppressSubSequentsPeriod );	}

Cmd<> CreateDmgEffect( PainterSeq &PStoJoin, int Amount, const Vec2i &Center, int Period, int HopPeriod, int SuppressSubSequentsPeriod )
{	return DmgEffect( PStoJoin, Amount, Center, Period, HopPeriod, SuppressSubSequentsPeriod );	}

