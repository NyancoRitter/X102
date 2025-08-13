//[IMazeData.h]

#pragma once

namespace PSEUDO_3D_MAZE
{
	//迷路のレンダリングに必要な情報の取得手段：
	//
	//迷路は普通に２次元のマス目で構成されているとして，
	//隣接するマスの間(この部分を何と呼べばいいのかわからんが，以下では便宜上「エッジ」と呼称する)
	//に壁等があるかどうかという情報を，疑似3D描画者に返す．
	//
	//メソッドが返す属性値が 1以上の値である場合，壁あるいはそれに準じるものである
	// （：そこに何らかの壁状の物のレンダリングを要する）ことを意味する．
	class IMazeData
	{
	public:
		virtual ~IMazeData() = default;

	public:
		//マス(x,y)の北側エッジの属性問い合わせ
		//	「マス(x,y) の外側（北側）から見たときに，エッジが何に見えるか」である．
		//	「マス(x,y) の内側から見たとき」ではなく．
		//	（他の同類のメソッドも同様．）
		virtual int NorthSideEdgeAttr( int x, int y ) const = 0;

		//マス(x,y)の南側エッジの属性問い合わせ
		virtual int SouthSideEdgeAttr( int x, int y ) const = 0;

		//マス(x,y)の西側エッジの属性問い合わせ
		virtual int WestSideEdgeAttr( int x, int y ) const = 0;

		//マス(x,y)の東側エッジの属性問い合わせ
		virtual int EastSideEdgeAttr( int x, int y ) const = 0;
	};

	//エッジ属性値が壁あるいはそれに準じるものか否か
	inline bool IsWallLikeFace( int Attr ){	return ( Attr >= 1 );	}
}

//[EOF]