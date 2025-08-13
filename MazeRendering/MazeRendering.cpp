//[MazeRendering.cpp]

#include "framework.h"

#include "MazeRendering.h"
#include "IMazeData.h"
#include "RenderProp.h"
#include "Parts/Vec2D.h"

#include <cmath>
#include <vector>
#include <algorithm>

namespace
{//作業関数等
	//エッジ属性値が壁あるいはそれに準じるものか否か
	inline bool IsWallLikeFace( int Attr ){	return ( Attr >= 1 );	}

	//レイが交差するエッジの座標を表す用
	struct CheckPoint
	{
		double X,Y;
		bool IsHorizonalEdge;

		CheckPoint( double X=0, double Y=0, bool IsHorizonalEdge=false )
			: X(X), Y(Y), IsHorizonalEdge(IsHorizonalEdge)
		{}
	};

	//線分(OX,OY)-(TX,TY) 上で，x=整数 あるいは y=整数 となる点を全て求め，(OX,OY)から近い順にrDstに格納する
	void EnumCheckPoint( double OX, double OY, double TX, double TY, std::vector<CheckPoint> &rDst )
	{
		rDst.clear();
		const double dx = TX-OX;
		const double dy = TY-OY;

		const int MinX = (int)ceil( (std::min)(OX,TX) );
		const int MaxX = (int)floor( (std::max)(OX,TX) );
		if( MinX<=MaxX )
		{
			const double dy_per_x = dy/dx;
			for( int x=MinX; x<=MaxX; ++x )
			{	rDst.push_back(   CheckPoint( x, OY+(x-OX)*dy_per_x, false )   );	}
		}

		const int MinY = (int)ceil( (std::min)(OY,TY) );
		const int MaxY = (int)floor( (std::max)(OY,TY) );
		if( MinY <= MaxY )
		{
			const double dx_per_y = dx/dy;
			for( int y=MinY; y<=MaxY; ++y )
			{	rDst.push_back(   CheckPoint( OX+(y-OY)*dx_per_y, y, true )   );	}
		}

		std::sort(
			rDst.begin(), rDst.end(),
			[OX,OY]( const CheckPoint &lhs, const CheckPoint &rhs )->bool
			{	return (   ( fabs(lhs.X-OX)+fabs(lhs.Y-OY) )   <   ( fabs(rhs.X-OX)+fabs(rhs.Y-OY) )   );	}
		);
	}
}

namespace PSEUDO_3D_MAZE
{
	//迷路エッジの描画
	void RenderMazeEdge(
		const IMazeData &rMazeData,
		const RenderProp &rProp,
		IRenderTgt &rRenderTgt
	)
	{
		const double WallHeight = 1.0;	//壁の高さ=1.0固定

		const Vec2d FrontDir = rProp.CamFrontDir();	//カメラ前方向単位ベクトル
		const Vec2d RightDir( -FrontDir[1], FrontDir[0] ); //カメラ右手方向単位ベクトル
		const Vec2d CamPos = rProp.CamPos();
		const double CamHeight = rProp.CamHeight();

		//
		const double f = rProp.CamModel().f();
		const double Ox = rProp.CamModel().Ox();
		const double Oy = rProp.CamModel().Oy();
		const double Near = rProp.NearDepth();
		const double Far = rProp.FarDepth();
		
		//迷路エッジ描画
		std::vector<CheckPoint> CheckPoints;
		for( int Pix_x=0; Pix_x<rRenderTgt.ImageWidth(); ++Pix_x )
		{
			//Pix_x に対応する ray
			const Vec2d Ray1 = FrontDir  +  ( (Pix_x - Ox) / f ) * RightDir;	//奥行き1
			const Vec2d RayNear = CamPos + Ray1*Near;
			const Vec2d RayFar = CamPos + Ray1*Far;

			EnumCheckPoint( RayNear[0],RayNear[1],  RayFar[0],RayFar[1], CheckPoints );
			int EdgeAttr = 0;
			for( const CheckPoint &CP : CheckPoints )
			{
				double LRPosRateOnEdge = 0;	//エッジを真正面にみたときに，CPがエッジ左端からどれだけの位置か(0.0～1.0)
				if( CP.IsHorizonalEdge )
				{
					if( Ray1[1]>0 )
					{
						EdgeAttr = rMazeData.NorthSideEdgeAttr( (int)floor(CP.X), int(CP.Y) );
						LRPosRateOnEdge = ceil(CP.X) - CP.X;
					}
					else
					{
						EdgeAttr = rMazeData.SouthSideEdgeAttr( (int)floor(CP.X), int(CP.Y)-1 );
						LRPosRateOnEdge = CP.X - floor(CP.X);
					}
				}
				else
				{
					if( Ray1[0]>0 )
					{
						EdgeAttr = rMazeData.WestSideEdgeAttr( int(CP.X), (int)floor(CP.Y) );
						LRPosRateOnEdge = CP.Y - floor(CP.Y);
					}
					else
					{
						EdgeAttr = rMazeData.EastSideEdgeAttr( int(CP.X)-1, (int)floor(CP.Y) );
						LRPosRateOnEdge = ceil(CP.Y) - CP.Y;
					}
				}

				if( IsWallLikeFace( EdgeAttr ) )
				{
					Vec2d Cam2CP = Vec2d(CP.X,CP.Y) - CamPos;
					double Depth = Cam2CP * FrontDir;
					int Pix_y1 = (int)std::round( Oy - f*(WallHeight-CamHeight)/Depth );
					int Pix_y2 = (int)std::round( Oy + f*CamHeight/Depth );
					rRenderTgt.DrawVerticalLine( Pix_x, Pix_y1, Pix_y2, EdgeAttr, Depth, Cam2CP.L2Norm(), LRPosRateOnEdge );
					break;
				}
			}
		}
	}

}