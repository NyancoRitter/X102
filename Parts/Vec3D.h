#pragma once

//3D Vector
template< class VAL_T >
class Vec3D
{
public:
	//data is public.
	VAL_T v[3];

public:
	constexpr Vec3D( VAL_T v0=0, VAL_T v1=0, VAL_T v2=0 ) : v{v0,v1,v2} {}

public:
	VAL_T operator[]( int i ) const {	return v[i];	}
	VAL_T &operator[]( int i ){	return v[i];	}
	Vec3D &Assign( VAL_T v0, VAL_T v1, VAL_T v2 ){	v[0]=v0;	v[1]=v1;	v[2]=v2;	return *this;	}

public:
	//Unary operators
	Vec3D operator -() const {	return Vec3D( -v[0], -v[1], -v[2] );	}
	Vec3D &operator +=( const Vec3D &rhs ){	v[0]+=rhs.v[0];	v[1]+=rhs.v[1];	v[2]+=rhs.v[2];	return *this;	}
	Vec3D &operator -=( const Vec3D &rhs ){	v[0]-=rhs.v[0];	v[1]-=rhs.v[1];	v[2]-=rhs.v[2];	return *this;	}
	Vec3D &operator *=( VAL_T s ){	v[0]*=s;	v[1]*=s;	v[2]*=s;	return *this;	}
	Vec3D &operator /=( VAL_T s ){	v[0]/=s;	v[1]/=s;	v[2]/=s;	return *this;	}

public:
	//Norm, Unitize
	VAL_T SqL2Norm() const {	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];	}
	VAL_T L2Norm() const {	return sqrt(SqL2Norm());	}
	Vec3D &Unitize(){	return operator/=( L2Norm() );	}
};

//Binary operators
template<class VAL_T> inline Vec3D<VAL_T> operator *( VAL_T s, const Vec3D<VAL_T> &V ){	return Vec3D<VAL_T>(V)*=s; }
template<class VAL_T> inline Vec3D<VAL_T> operator *( const Vec3D<VAL_T> &V, VAL_T s ){	return Vec3D<VAL_T>(V)*=s; }
template<class VAL_T> inline Vec3D<VAL_T> operator /( const Vec3D<VAL_T> &V, VAL_T d ){	return Vec3D<VAL_T>(V)/=d; }
template<class VAL_T> inline Vec3D<VAL_T> operator +( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs ){	return Vec3D<VAL_T>(lhs)+=rhs; }
template<class VAL_T> inline Vec3D<VAL_T> operator -( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs ){	return Vec3D<VAL_T>(lhs)-=rhs; }
template<class VAL_T> inline bool operator ==( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs ){	return (lhs[0]==rhs[0] && lhs[1]==rhs[1] && lhs[2]==rhs[2]); }
template<class VAL_T> inline bool operator !=( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs ){	return !(lhs==rhs); }

	//Inner prod
template<class VAL_T> inline VAL_T operator *( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs ){	return ( lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2]); }
	//Cross-prod (Pay attention to the weak operator priority)
template<class VAL_T> inline VAL_T operator ^( const Vec3D<VAL_T> &lhs, const Vec3D<VAL_T> &rhs )
{	return Vec3D<VAL_T>( lhs[1]*rhs[2] - lhs[2]*rhs[1], lhs[2]*rhs[0] - lhs[0]*rhs[2], lhs[0]*rhs[1] - lhs[1]*rhs[0] );	}

//Unit Vector
template<class VAL_T> inline Vec3D<VAL_T> GetUnit( const Vec3D<VAL_T> &V ){	return Vec3D<VAL_T>(V).Unitize(); }

//alias
using Vec3i = Vec3D<int>;
using Vec3f = Vec3D<float>;
using Vec3d = Vec3D<double>;
