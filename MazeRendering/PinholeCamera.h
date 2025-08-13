//[Pinhole_CamModel.h]

#pragma once

#include <limits>
#include "Parts/Vec3D.h"
#include "Parts/Vec2D.h"

namespace PSEUDO_3D_MAZE
{
	class CPinholeCamera
	{
	public:
		CPinholeCamera()
		{	
			SetIPVals( 319.5, 239.5, 1.0 );//�Ă��Ɓ[�ɖ��Ӗ��Ȓl����荞��ł���
		}

	public:
		//�����p�����^
		bool SetIPVals( double Ox, double Oy, double f )
		{
			if( f<=0 )return false;
			m_Ox = Ox;
			m_Oy = Oy;
			m_f = f;
			return true;
		}

		double Ox() const {	return m_Ox;	}
		double Oy() const {	return m_Oy;	}
		double f() const {	return m_f;	}

		////�摜��pixel���W�ɑΉ�����C�J�������W�n�ɂ�����P�ʕ����x�N�g�������߂�D
		//void Cvt_PixCoord2UnitCamCoord(
		//	double PixX, double PixY,
		//	double &rCX, double &rCY, double &rCZ
		//	) const
		//{
		//	const double x = ( PixX - m_Ox) / m_f;
		//	const double y = ( PixY - m_Oy) / m_f;
		//	const double z = 1.0;
		//	double L = sqrt( x*x + y*y + z*z );
		//	rCX = x / L;
		//	rCY = y / L;
		//	rCZ = z / L;
		//}

		////�J�������W�n�ɂ�����ʒu�x�N�g���ɑΉ�����C�B�e�摜��pixel���W�����߂�D
		////�iCvt_Pixel2UnitDirVec()�Ƌt�����̍��W�ϊ��D�j
		////[Args]
		////	CX,CY,CZ : �J�������W�n�̈ʒu�D
		////	rPixX,rPixY : ���ʊi�[�ꏊ�D
		////	              (CX,CY,CZ)�����e������f�ʒu�D
		////[Ret]
		////	��������true���C���s����false��Ԃ��D
		//virtual bool Cvt_CamCoord2PixCoord(
		//	double CX, double CY, double CZ,
		//	double &rPixX, double &rPixY
		//	) const
		//{
		//	if( CZ<=0.0 )return false;
		//	const double rate = m_f / CZ;
		//	rPixX = m_Ox + CX * rate;
		//	rPixY = m_Oy + CY * rate;
		//	return true;
		//}

		//�J�������W�n�ɂ�����ʒu�x�N�g���ɑΉ�����C�B�e�摜��pixel���W�����߂�D
		//[Args]
		//	P : �J�������W�n�̈ʒu�D�v�f�� ( �J�����E������C�J����������, �J�����O�� )
		//[Ret]
		//	P�����e������f�ʒu
		virtual Vec2d Cvt_CamCoord2PixCoord( const Vec3d &P ) const
		{
			const double rate = m_f / P[2];
			return Vec2d( m_Ox + P[0]*rate, m_Oy + P[1]*rate );
		}

	private:
		double m_Ox, m_Oy, m_f;	//optical center(Ox,Oy) & Focal_Length f
	};

}
