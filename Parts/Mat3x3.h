#pragma once

#include "Vec3D.h"

//3*3�}�g���N�X : Vec3D<VAL_T> �ɍ�����|���邱�Ƃ�����ړI�Ƃ������́D
template < class VAL_T >
class Mat3x3
{
private:	//�����o�ϐ�
	Vec3D<VAL_T> m_RowVec[3];	//�s�x�N�g��*3��

public:
	//ctor : ������Ԃ͒P�ʍs��
	Mat3x3(){	ResetToIdentity();	}

	//�x�N�^�ɍ�����|����
	Vec3D<VAL_T> operator *( const Vec3D<VAL_T> &V ) const
	{	return Vec3D<VAL_T>( m_RowVec[0]*V, m_RowVec[1]*V, m_RowVec[2]*V );	}

	//�}�g���N�X�ɍ�����|����
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

	//���Z�E���Z
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

	//�萔�{
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

	//�s�x�N�g��
	Vec3D<VAL_T> &RowVec( unsigned char row ){	return m_RowVec[row];	}
	const Vec3D<VAL_T> &RowVec( unsigned char row ) const {	return m_RowVec[row];	}
	Vec3D<VAL_T> &operator[]( unsigned char row ){	return RowVec(row);	}
	const Vec3D<VAL_T> &operator []( unsigned char row ) const {	return RowVec(row);	}

	//�P�ʍs��ɂ���
	void ResetToIdentity()
	{
		m_RowVec[0].Assign(1,0,0);
		m_RowVec[1].Assign(0,1,0);
		m_RowVec[2].Assign(0,0,1);
	}

	//�]�u�s��̍쐬
	Mat3x3<VAL_T> Transpose() const
	{
		Mat3x3<VAL_T> R;
		R.RowVec( 0 ).Assign( m_RowVec[0][0], m_RowVec[1][0], m_RowVec[2][0] );
		R.RowVec( 1 ).Assign( m_RowVec[0][1], m_RowVec[1][1], m_RowVec[2][1] );
		R.RowVec( 2 ).Assign( m_RowVec[0][2], m_RowVec[1][2], m_RowVec[2][2] );
		return R;
	}

};

//�萔�{�i�񃁃��o�֐��j
template < class VAL_T >
inline Mat3x3<VAL_T> operator *( VAL_T n, const Mat3x3<VAL_T> &rM ){	return rM*n;	}

//alias
using Mat3x3d = Mat3x3<double>;
