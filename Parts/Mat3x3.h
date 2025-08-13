#pragma once

#include "Vec3D.h"

//3*3マトリクス : Vec3D<VAL_T> に左から掛けることだけを目的としたもの．
template < class VAL_T >
class Mat3x3
{
private:	//メンバ変数
	Vec3D<VAL_T> m_RowVec[3];	//行ベクトル*3個

public:
	//ctor : 初期状態は単位行列
	Mat3x3(){	ResetToIdentity();	}

	//ベクタに左から掛ける
	Vec3D<VAL_T> operator *( const Vec3D<VAL_T> &V ) const
	{	return Vec3D<VAL_T>( m_RowVec[0]*V, m_RowVec[1]*V, m_RowVec[2]*V );	}

	//マトリクスに左から掛ける
	Mat3x3<VAL_T> operator *( const Mat3x3<VAL_T> &rM ) const
	{
		Mat3x3 R;
		Mat3x3 TM;
		rM.Transpose( TM );
		R.RowVec( 0 ).Assign( m_RowVec[0]*TM.RowVec(0) , m_RowVec[0]*TM.RowVec(1), m_RowVec[0]*TM.RowVec(2) );
		R.RowVec( 1 ).Assign( m_RowVec[1]*TM.RowVec(0) , m_RowVec[1]*TM.RowVec(1), m_RowVec[1]*TM.RowVec(2) );
		R.RowVec( 2 ).Assign( m_RowVec[2]*TM.RowVec(0) , m_RowVec[2]*TM.RowVec(1), m_RowVec[2]*TM.RowVec(2) );
		return R;
	}

	//加算・減算
	Mat3x3<VAL_T> operator +( const Mat3x3<VAL_T> &rhs ) const
	{
		Mat3x3<VAL_T> R = *this;
		R.RowVec(0) += rhs.RowVec(0);
		R.RowVec(1) += rhs.RowVec(1);
		R.RowVec(2) += rhs.RowVec(2);
		return R;
	}

	Mat3x3<VAL_T> operator -( const Mat3x3<VAL_T> &rhs ) const
	{
		Mat3x3<VAL_T> R = *this;
		R.RowVec(0) -= rhs.RowVec(0);
		R.RowVec(1) -= rhs.RowVec(1);
		R.RowVec(2) -= rhs.RowVec(2);
		return R;
	}

	Mat3x3<VAL_T> &operator +=( const Mat3x3<VAL_T> &rhs )
	{
		RowVec(0) += rhs.RowVec(0);
		RowVec(1) += rhs.RowVec(1);
		RowVec(2) += rhs.RowVec(2);
		return *this;
	}

	Mat3x3<VAL_T> &operator -=( const Mat3x3<VAL_T> &rhs )
	{
		RowVec(0) -= rhs.RowVec(0);
		RowVec(1) -= rhs.RowVec(1);
		RowVec(2) -= rhs.RowVec(2);
		return *this;
	}

	//定数倍
	Mat3x3<VAL_T> operator *( VAL_T s ) const
	{
		Mat3x3<VAL_T> K = *this;
		K.RowVec( 0 ) *= s;
		K.RowVec( 1 ) *= s;
		K.RowVec( 2 ) *= s;
		return K;
	}

	Mat3x3<VAL_T> &operator *=( VAL_T s )
	{
		RowVec( 0 ) *= s;
		RowVec( 1 ) *= s;
		RowVec( 2 ) *= s;
		return *this;
	}

	//行ベクトル
	Vec3D<VAL_T> &RowVec( unsigned char row ){	return m_RowVec[row];	}
	const Vec3D<VAL_T> &RowVec( unsigned char row ) const {	return m_RowVec[row];	}
	Vec3D<VAL_T> &operator[]( unsigned char row ){	return RowVec(row);	}
	const Vec3D<VAL_T> &operator []( unsigned char row ) const {	return RowVec(row);	}

	//単位行列にする
	void ResetToIdentity()
	{
		m_RowVec[0].Assign(1,0,0);
		m_RowVec[1].Assign(0,1,0);
		m_RowVec[2].Assign(0,0,1);
	}

	//転置行列の作成
	Mat3x3<VAL_T> Transpose() const
	{
		Mat3x3<VAL_T> R;
		R.RowVec( 0 ).Assign( m_RowVec[0][0], m_RowVec[1][0], m_RowVec[2][0] );
		R.RowVec( 1 ).Assign( m_RowVec[0][1], m_RowVec[1][1], m_RowVec[2][1] );
		R.RowVec( 2 ).Assign( m_RowVec[0][2], m_RowVec[1][2], m_RowVec[2][2] );
		return R;
	}

};

//定数倍（非メンバ関数）
template < class VAL_T >
inline Mat3x3<VAL_T> operator *( VAL_T n, const Mat3x3<VAL_T> &rM ){	return rM*n;	}

//alias
using Mat3x3d = Mat3x3<double>;
