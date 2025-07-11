#pragma once

#include <cmath>

//2D Vector
template< class VAL_T >
class Vec2D
{
public:
	//data is public.
	VAL_T v[2];

public:
	constexpr Vec2D( VAL_T v0=0, VAL_T v1=0 ) : v{v0,v1} {}

public:
	VAL_T operator[]( int i ) const {	return v[i];	}
	VAL_T &operator[]( int i ){	return v[i];	}
	Vec2D &Assign( VAL_T v0, VAL_T v1 ){	v[0]=v0;	v[1]=v1;	return *this;	}

public:
	//Unary operators
	Vec2D operator -() const {	return Vec2D( -v[0], -v[1] );	}
	Vec2D &operator +=( const Vec2D &rhs ){	v[0]+=rhs.v[0];	v[1]+=rhs.v[1];	return *this;	}
	Vec2D &operator -=( const Vec2D &rhs ){	v[0]-=rhs.v[0];	v[1]-=rhs.v[1];	return *this;	}
	Vec2D &operator *=( VAL_T s ){	v[0]*=s;	v[1]*=s;	return *this;	}
	Vec2D &operator /=( VAL_T s ){	v[0]/=s;	v[1]/=s;	return *this;	}

public:
	//Norm, Unitize
	VAL_T SqL2Norm() const {	return v[0]*v[0] + v[1]*v[1];	}
	VAL_T L2Norm() const {	return std::sqrt(SqL2Norm());	}
	Vec2D &Unitize(){	return operator/=( L2Norm() );	}
};

//Binary operators
template<class VAL_T> inline Vec2D<VAL_T> operator *( VAL_T s, const Vec2D<VAL_T> &V ){	return Vec2D<VAL_T>(V)*=s; }
template<class VAL_T> inline Vec2D<VAL_T> operator *( const Vec2D<VAL_T> &V, VAL_T s ){	return Vec2D<VAL_T>(V)*=s; }
template<class VAL_T> inline Vec2D<VAL_T> operator /( const Vec2D<VAL_T> &V, VAL_T d ){	return Vec2D<VAL_T>(V)/=d; }
template<class VAL_T> inline Vec2D<VAL_T> operator +( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return Vec2D<VAL_T>(lhs)+=rhs; }
template<class VAL_T> inline Vec2D<VAL_T> operator -( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return Vec2D<VAL_T>(lhs)-=rhs; }
template<class VAL_T> inline bool operator ==( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return (lhs[0]==rhs[0] && lhs[1]==rhs[1]); }
template<class VAL_T> inline bool operator !=( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return !(lhs==rhs); }

template<class VAL_T> inline bool operator <( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs )
{
	if( lhs[0] < rhs[0] )return true;
	if( lhs[0] > rhs[0] )return false;
	return ( lhs[1] < rhs[1] );
}

	//Inner prod
template<class VAL_T> inline VAL_T operator *( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return ( lhs[0]*rhs[0] + lhs[1]*rhs[1] ); }
	//Cross-prod (Pay attention to the weak operator priority)
	//	Assuming both arguments are 3D vectors (x,y,0), and returns the z-value of the cross-product.
template<class VAL_T> inline VAL_T operator ^( const Vec2D<VAL_T> &lhs, const Vec2D<VAL_T> &rhs ){	return ( lhs[0]*rhs[1] - lhs[1]*rhs[0] ); }

//Unit Vector
template<class VAL_T> inline Vec2D<VAL_T> GetUnit( const Vec2D<VAL_T> &V ){	return Vec2D<VAL_T>(V).Unitize(); }

//alias
using Vec2i = Vec2D<int>;
using Vec2f = Vec2D<float>;
using Vec2d = Vec2D<double>;
