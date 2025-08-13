//[MazeRendering.h]

#pragma once

namespace PSEUDO_3D_MAZE
{
	class IMazeData;
	class RenderProp;

	//迷路描画処理で必要な描画処理を実際に実施する者．
	//表示用画像バッファに描画を行う役目．
	class IRenderTgt
	{
	public:
		virtual ~IRenderTgt() = default;

	public:
		//迷路を描画する絵のサイズ[pixel]を返す
		virtual int ImageWidth() const = 0;
		virtual int ImageHeight() const = 0;

		//エッジの一部として縦線を描画する．
		//エッジが壁，あるいはそれに準じるものであるときにそれを描画するために呼ばれるので，
		//描画画像上の，幅1pixelの線領域 (x,y0)-(x,y1) を描画する．（引数値の大小関係は y0 <= y1 である）
		//
		//EdgeAttr : 描画対象エッジの属性値
		//Depth : 描画対象位置のカメラからの奥行き
		//Dist : 描画対象位置のカメラからの距離
		//LRPosRateOnEdge : 描画対象エッジを真正面にみたときの，描画縦線のエッジ上での横方向位置．(左端0.0～右端1.0)
		virtual void DrawVerticalLine( int x, int y0, int y1, int EdgeAttr, double Depth, double Dist, double LRPosRateOnEdge ) = 0;
	};


	//ピンホールカメラモデルによる3D迷路の描画
	//
	//空間の座標系について：
	//	原点：マス(0,0)の北西端
	//	座標の向き：東にいくほどPosXが増加，南にいくほどPosYが増加．
	//	座標の尺度：1マスの辺の長さを1.0とする．
	//	すなわち，マス(x,y)の中心位置 = (x+0.5,y+0.5) となる．
	//
	//[Args]
	//	rMazeData :
	//		迷路データ．描画処理が必要とするデータを提供する．
	//	RenderProp :
	//		レンダリング用プロパティ．描画具合の設定．
	//	rRenderTgt :
	//		画像バッファへの描画処理者．
	void RenderMazeEdge(
		const IMazeData &rMazeData,	//迷路データ
		const RenderProp &rProp,	//レンダリング用プロパティ
		IRenderTgt &rRenderTgt	//描画処理者
	);

}

//[EOF]