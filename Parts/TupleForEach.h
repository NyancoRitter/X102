#pragma once

#include <utility>

//Tuple�̑S���ڂ� Func ��K�p����
//
//�i��jt �̂R�̗v�f�� std::cout �ɏo�́F
//	Tuple<X,Y,Z> t;
//	TupleForEach( t, []( const auto &mem ){ std::cout << mem; } );

template< std::size_t TupleIndex=0, typename Func, typename... TupleItemTypes >
void TupleForEach( const std::tuple<TupleItemTypes...> &t, const Func &f )
{
	f( std::get<TupleIndex>(t) );

	if constexpr( TupleIndex+1 < std::tuple_size_v< std::tuple<TupleItemTypes...> > )
	{	TupleForEach< TupleIndex+1, Func, TupleItemTypes...>( t, f );	}
}
