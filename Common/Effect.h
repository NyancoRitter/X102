#pragma once

#include <functional>
#include <list>

/// <summary>
/// 何からの入力無しの更新ができるもの．
/// 物によっては何かを描画する(アニメーション等の表示用)．
/// </summary>
class IEffect
{
public:
	virtual ~IEffect() = default;

	/// <summary>更新</summary>
	/// <returns>falseを返した場合，完了したことを示す</returns>
	virtual bool Update() = 0;

	/// <summary>処理時間の消費の仕方に関する</summary>
	/// <returns>trueを返した場合，所属シーケンスの後続のエフェクトに処理を進めるべきではないことを示す</returns>
	virtual bool SuppressSubsequents() const {	return false;	}

	/// <summary>描画</summary>
	/// <param name="hdc"></param>
	virtual void Paint( HDC hdc ) const {}
};


/// <summary>
/// 処理期間管理ヘルパ
/// </summary>
class EffectForCertainPeriod : public IEffect
{
public:
	/// <summary>ctor</summary>
	/// <param name="Period">実施期間</param>
	/// <param name="SuppressSubsequentsPeriod">SuppressSubsequents を返す期間</param>
	EffectForCertainPeriod( int Period, int SuppressSubsequentsPeriod )
		: m_Period( Period ), m_nSuppressSubsequents(SuppressSubsequentsPeriod)
	{}

	virtual bool Update() override final
	{
		if( m_Counter >= m_Period )return false;
		++m_Counter;
		Update_( m_Counter );
		return ( m_Counter < m_Period );
	}

	virtual bool SuppressSubsequents() const override final {	return ( m_Counter <= m_nSuppressSubsequents );	}

protected:
	virtual void Update_( int count ) = 0;

private:
	int m_Counter = 0;
	const int m_Period;
	const int m_nSuppressSubsequents;
};

/// <summary>
/// ウェイト
/// </summary>
class Wait : public EffectForCertainPeriod
{
public:
	Wait( int Period ) : EffectForCertainPeriod( Period, Period ) {}
protected:
	virtual void Update_( int ) override {}
};
