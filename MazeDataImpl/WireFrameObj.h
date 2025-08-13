#pragma once
#include "Parts/Vec3D.h"
#include <vector>
#include "Direction.h"

namespace MazeDataImpl
{
	//ワイヤーフレームオブジェクトデータ
	//
	//[座標]
	//X,Y : 迷路のレンダリング処理が使っているのと同じ方向．すなわち，
	//	原点：(オブジェクトが配置される)マスの北西端
	//	座標の向き：東にいくほどXが増加，南にいくほどYが増加．
	//	座標の尺度：1マスの辺の長さを1.0とする．
	//Z : 地面からの高さ．
	//	0.0が床，1.0が天井の高さ．
	struct WireFrameObj
	{
	public:
		//頂点群座標
		std::vector< Vec3d > P;
		//線分群．１線分を頂点indexのpairで表す
		std::vector< std::pair<int,int> > L;

	public:
		//昇りはしご
		static WireFrameObj CreateUpStair();
		//降りはしご
		static WireFrameObj CreateDownStair();
		//人物
		static WireFrameObj CreateCharacter();
		//魔法陣
		static WireFrameObj CreateMagicCircle();
		//宝箱
		//	Dir : 位置をマス内でどちら側に寄せるか．
		static WireFrameObj CreateTreasureBox( Direction Dir );
	};
}
