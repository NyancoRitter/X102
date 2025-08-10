#include "framework.h"
#include "EffectImpls.h"

#include "GUI/IPainter.h"

#include "ValueImgPainter.h"

namespace
{
	class HPRecovEffect : public EffectForCertainPeriod
	{
	public:
		HPRecovEffect(
			int Amount,
			const Vec2i &Center,
			int Period, 
			int SuppressSubsequentsPeriod
		)
			: EffectForCertainPeriod( Period, SuppressSubsequentsPeriod )
			, m_Painter( Amount )
		{
			m_Painter.Color( RGB(0,255,255) )
				.MagRate( 2 )
				.TopLeft( Center - m_Painter.Size()/2 );
		}

		virtual void Paint( HDC hdc ) const override {	m_Painter.Paint( hdc );	}
	protected:
		virtual void Update_( int ) override {	m_Painter.Offset( {0,-1} );	}
	private:
		ValueImgPainter m_Painter;
	};

	//
	class DmgEffect : public EffectForCertainPeriod
	{
	public:
		DmgEffect(
			int Amount,
			const Vec2i &Center,
			int Period, 
			int HopPeriod,
			int SuppressSubsequentsPeriod
		)
			: EffectForCertainPeriod( Period, SuppressSubsequentsPeriod )
			, m_Painter( Amount )
			, m_HopPeriod( HopPeriod )
		{
			m_Painter.Color( RGB(255,128,16) )
				.MagRate( 2 )
				.TopLeft( Center - m_Painter.Size()/2 );

			m_InitPos = m_Painter.TopLeft();
		}

		virtual void Paint( HDC hdc ) const override {	m_Painter.Paint( hdc );	}
	protected:
		virtual void Update_( int count ) override
		{
			if( count<=m_HopPeriod )
			{	m_Painter.TopLeft(   m_InitPos  -  Vec2i( 0, int( 16*sin( 3.14 * count/m_HopPeriod ) ) )   );	}
		}
	private:
		ValueImgPainter m_Painter;
		int m_HopPeriod;
		Vec2i m_InitPos;
	};

	//
	class PoisonCuredEffect : public IEffect
	{
	public:
		virtual bool Update() override {	return false;	}
	};
}

std::unique_ptr<IEffect> CreateHPRecovEffect( int Amount, const Vec2i &Center, int SuppressSubSequentsPeriod )
{	return std::make_unique<HPRecovEffect>( Amount, Center, 16, SuppressSubSequentsPeriod );	}

std::unique_ptr<IEffect> CreateDmgEffect( int Amount, const Vec2i &Center, int SuppressSubSequentsPeriod )
{	return std::make_unique<DmgEffect>( Amount, Center, 16, 6, SuppressSubSequentsPeriod );	}

std::unique_ptr<IEffect> CreatePoisonCuredEffect(){	return std::make_unique<PoisonCuredEffect>();	}

//DispForCertainPeriod::DispForCertainPeriod( std::unique_ptr<GUI::IPainter> upDispTgt, int nTimes, int nSuppressSubsequents )
//	: m_upDispTgt( std::move(upDispTgt) )
//	, m_nTimes(nTimes)
//	, m_nSuppressSubsequents(nSuppressSubsequents)
//{}
//
//Flags<CmdResult> DispForCertainPeriod::operator()( HDC hdc )
//{
//	if( m_Counter >= m_nTimes )return CmdResult::Finished;
//	++m_Counter;
//
//	if( m_UpdateFunc ){	m_UpdateFunc( *m_upDispTgt, m_Counter );	}
//	
//	Flags<CmdResult> Ret = CmdResult::ReqRedraw;
//	if( m_Counter <= m_nSuppressSubsequents )Ret |= CmdResult::SuppressSubsequents;
//	if( m_Counter >= m_nTimes )Ret |= CmdResult::Finished;
//	return Ret;
//}
//

