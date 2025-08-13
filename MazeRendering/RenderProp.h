//[RenderProp.h]

#pragma once
#include "PinholeCamera.h"
#include "Parts/Vec2D.h"

namespace PSEUDO_3D_MAZE
{
	//迷路レンダリングに必要なパラメタの取得手段
	class RenderProp
	{
	public:
		virtual ~RenderProp(){}
	public:
		//カメラモデル
		virtual const CPinholeCamera &CamModel() const = 0;

		//NearとFar : レンダリング最小，最大奥行
		virtual double NearDepth() const = 0;
		virtual double FarDepth() const = 0;

		//カメラ位置と向き
		//	空間の座標系について：
		//	原点：マス(0,0)の北西端
		//	座標の向き：東にいくほどXが増加，南にいくほどYが増加．
		//	座標の尺度：1マスの辺の長さを1.0とする．
		//	すなわち，マス(x,y)の中心位置 = (x+0.5, y+0.5) となる．
		virtual Vec2d CamPos() const = 0;
		//カメラ位置の高さ．描画アルゴリズムの仕組み的に，まともな描画が可能な範囲は，0.0<=高さ<=1.0 である．
		virtual double CamHeight() const = 0;
		//カメラの向き（単位ベクトル）
		virtual Vec2d CamFrontDir() const = 0;
	};
}

//[EOF]