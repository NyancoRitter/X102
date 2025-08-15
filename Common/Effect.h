#pragma once

#include <functional>
#include <list>

/// <summary>
/// ������̓��͖����̍X�V���ł�����́D
/// ���ɂ���Ă͉�����`�悷��(�A�j���[�V�������̕\���p)�D
/// </summary>
class IEffect
{
public:
	virtual ~IEffect() = default;

	/// <summary>�X�V</summary>
	/// <returns>false��Ԃ����ꍇ�C�����������Ƃ�����</returns>
	virtual bool Update() = 0;

	/// <summary>�������Ԃ̏���̎d���Ɋւ���</summary>
	/// <returns>true��Ԃ����ꍇ�C�����V�[�P���X�̌㑱�̃G�t�F�N�g�ɏ�����i�߂�ׂ��ł͂Ȃ����Ƃ�����</returns>
	virtual bool SuppressSubsequents() const {	return false;	}

	/// <summary>�`��</summary>
	/// <param name="hdc"></param>
	virtual void Paint( HDC hdc ) const {}
};


/// <summary>
/// �������ԊǗ��w���p
/// </summary>
class EffectForCertainPeriod : public IEffect
{
public:
	/// <summary>ctor</summary>
	/// <param name="Period">���{����</param>
	/// <param name="SuppressSubsequentsPeriod">SuppressSubsequents ��Ԃ�����</param>
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
/// �E�F�C�g
/// </summary>
class Wait : public EffectForCertainPeriod
{
public:
	Wait( int Period ) : EffectForCertainPeriod( Period, Period ) {}
protected:
	virtual void Update_( int ) override {}
};
