//[StlRnd.h]

#pragma once

#include <random>

//std::mt19937‚ğ—p‚¢‚½—”¶¬
class StlRnd
{
public:
	StlRnd() : m_MT( std::random_device().operator()() ){}
	StlRnd( std::mt19937::result_type Seed ) : m_MT( Seed ){}
public:
	template< class T_INT=int >
	T_INT GetInt( T_INT Min, T_INT Max ){	return std::uniform_int_distribution<T_INT>( Min,Max )( m_MT );	}

	template< class T_REAL=double >
	T_REAL GetReal( T_REAL Min, T_REAL Max ){	return std::uniform_real_distribution<T_REAL>( Min,Max )( m_MT );	}
private:
	std::mt19937 m_MT;
};

//[EOF]