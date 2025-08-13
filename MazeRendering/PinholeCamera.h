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
			SetIPVals( 319.5, 239.5, 1.0 );//てきとーに無意味な値を放り込んでおく
		}

	public:
		//内部パラメタ
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

		////画像のpixel座標に対応する，カメラ座標系における単位方向ベクトルを求める．
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

		////カメラ座標系における位置ベクトルに対応する，撮影画像のpixel座標を求める．
		////（Cvt_Pixel2UnitDirVec()と逆方向の座標変換．）
		////[Args]
		////	CX,CY,CZ : カメラ座標系の位置．
		////	rPixX,rPixY : 結果格納場所．
		////	              (CX,CY,CZ)が投影される画素位置．
		////[Ret]
		////	成功時はtrueを，失敗時はfalseを返す．
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

		//カメラ座標系における位置ベクトルに対応する，撮影画像のpixel座標を求める．
		//[Args]
		//	P : カメラ座標系の位置．要素は ( カメラ右手方向，カメラ下方向, カメラ前方 )
		//[Ret]
		//	Pが投影される画素位置
		virtual Vec2d Cvt_CamCoord2PixCoord( const Vec3d &P ) const
		{
			const double rate = m_f / P[2];
			return Vec2d( m_Ox + P[0]*rate, m_Oy + P[1]*rate );
		}

	private:
		double m_Ox, m_Oy, m_f;	//optical center(Ox,Oy) & Focal_Length f
	};

}
