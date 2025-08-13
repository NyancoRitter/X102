#include "framework.h"
#include "WireFrameObj.h"

namespace
{
#if 1
	//Z軸周りに回す作業
	class Rot
	{
	private:
		double S,C;
	public:
		Rot( double angle_rad ) : S{ sin(angle_rad) }, C{ cos(angle_rad) } {}
		void operator ()( Vec3d &V ) const
		{
			double X = V[0];
			double Y = V[1];
			V[0] = C*X + S*Y;
			V[1] = -S*X + C*Y;
		}
	};
#endif

	//ヘルパ．Linesに indexes を順に結ぶループを追加．
	inline void AddLoop( std::vector<std::pair<int,int>> &Lines, std::vector<int> indexes )
	{
		const int N = (int)indexes.size();
		for( int i=0; i<N-1; ++i ){	Lines.emplace_back( indexes[i], indexes[i+1] );	}
		Lines.emplace_back( indexes[N-1], indexes[0] );
	}
}

namespace MazeDataImpl
{
	//昇りはしご
	WireFrameObj WireFrameObj::CreateUpStair()
	{
		WireFrameObj Obj;
		{//---頂点---
			auto &P = Obj.P;
			P.resize( 4 + 2*2 + 2*6 );
			//穴
			P[0].Assign( 0.15, 0.15, 1.0 );
			P[1].Assign( 0.85, 0.15, 1.0 );
			P[2].Assign( 0.85, 0.85, 1.0 );
			P[3].Assign( 0.15, 0.85, 1.0 );
			//
			double x1 = 0.4;	double y1 = 0.35;
			double x2 = 0.6;	double y2 = 0.65;
			//はしご左右の棒
			P[4].Assign( x1, y1, 1.0 );
			P[5].Assign( x1, y1, 0.0 );
			P[6].Assign( x2, y2, 1.0 );
			P[7].Assign( x2, y2, 0.0 );
			//はしご横棒群
			for( int i=0; i<6; ++i )
			{
				P[8+i*2].Assign( x1, y1, (i+1)*0.15 );
				P[9+i*2].Assign( x2, y2, (i+1)*0.15 );
			}

			P.shrink_to_fit();
		}
		{//---線---
			auto &L = Obj.L;
			L.reserve( 4 + 2 + 6 );
			//穴
			AddLoop( L, {0,1,2,3} );
			//はしご左右の棒
			L.emplace_back( 4,5 );
			L.emplace_back( 6,7 );
			//はしご横棒群
			for( int i=0; i<6; ++i )
			{	L.emplace_back( 8+i*2, 9+i*2 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}

	//降りはしご
	WireFrameObj WireFrameObj::CreateDownStair()
	{
		WireFrameObj Obj;
		{//---頂点---
			auto &P = Obj.P;
			P.resize( 4 + 2*2 + 2*3 );

			P[0].Assign( 0.15, 0.15, 0.0 );
			P[1].Assign( 0.85, 0.15, 0.0 );
			P[2].Assign( 0.85, 0.85, 0.0 );
			P[3].Assign( 0.15, 0.85, 0.0 );
			//
			double x1 = 0.35;	double y1 = 0.4;
			double x2 = 0.65;	double y2 = 0.6;
			P[4].Assign( x1, y1, 0.5 );
			P[5].Assign( x1, y1, -0.1 );
			P[6].Assign( x2, y2, 0.5 );
			P[7].Assign( x2, y2, -0.1 );
			//
			for( int i=0; i<3; ++i )
			{
				P[8+i*2].Assign( x1, y1, i*0.15 + 0.05 );
				P[9+i*2].Assign( x2, y2, i*0.15 + 0.05 );
			}

			P.shrink_to_fit();
		}
		{//---線---
			auto &L = Obj.L;
			L.reserve( 4 + 2 + 3 );

			AddLoop( L, {0,1,2,3} );

			L.emplace_back( 4,5 );
			L.emplace_back( 6,7 );

			for( int i=0; i<3; ++i )
			{	L.emplace_back( 8+i*2, 9+i*2 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}

	//人物
	WireFrameObj WireFrameObj::CreateCharacter()
	{
		WireFrameObj Obj;

		{//---頂点---
			auto &P = Obj.P;
			P.reserve( 6 + 5 );

			//頭
			constexpr double cz = 0.52;
			constexpr double r = 0.035;
			P.emplace_back( 0,0,cz-r );
			P.emplace_back( r,0,cz );
			P.emplace_back( 0,r,cz );
			P.emplace_back( -r,0,cz );
			P.emplace_back( 0,-r,cz );
			P.emplace_back( 0,0,cz+r );
			//体
			constexpr double tz = 0.47;
			constexpr double r2 = 0.065 / 1.4;
			P.emplace_back( r2,-r2,tz );
			P.emplace_back( r2,r2,tz );
			P.emplace_back( -r2,r2,tz );
			P.emplace_back( -r2,-r2,tz );
			P.emplace_back( 0, 0, 0.3 );

			Vec3d T{ 0.5, 0.5, 0.0 };
			for( auto &p : P ){	p+=T;	}

			P.shrink_to_fit();
		}
		{//---線---
			auto &L = Obj.L;
			L.reserve( 12 + 8 );
			//頭
			AddLoop( L, { 1,2,3,4 } );
			for( int i=1; i<=4; ++i )
			{
				L.emplace_back( 0,i );
				L.emplace_back( i,5 );
			}
			//体
			AddLoop( L, { 6,7,8,9 } );
			for( int i=6; i<=9; ++i )
			{	L.emplace_back( i,10 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}

	//魔法陣
	WireFrameObj WireFrameObj::CreateMagicCircle()
	{
		WireFrameObj Obj;
		constexpr int Reso = 3;
		constexpr int N = Reso*5;
		{//---頂点---
			auto &P = Obj.P;
			P.reserve( N );
			constexpr double r = 0.42;
			for( int i=0; i<N; ++i )
			{
				const double theta = 2*PI * i / N;
				P.emplace_back( 0.5+r*cos(theta), 0.5+r*sin(theta), 0 );
			}
			P.shrink_to_fit();
		}
		{//---線---
			auto &L = Obj.L;
			L.reserve( N + 5 );
			//円
			for( int i=0; i+1<N; ++i )L.emplace_back( i, i+1 );
			L.emplace_back( N-1, 0 );
			//星
			AddLoop( L, { 0, Reso*2, Reso*4, Reso, Reso*3 } );

			L.shrink_to_fit();
		}
		return Obj;
	}

	//宝箱
	WireFrameObj WireFrameObj::CreateTreasureBox(  Direction Dir )
	{
		WireFrameObj Obj;

		{//---頂点---
			auto &P = Obj.P;
			P.reserve( 8 );

			//※簡便さのため，マスの中心を原点とする座標系で頂点位置を定義
			constexpr double hW = 0.12;
			constexpr double hD = 0.1;
			constexpr double BoxH = 0.25;

			P.emplace_back( -hD, -hW, 0 );
			P.emplace_back( hD, -hW, 0 );
			P.emplace_back( hD, hW, 0 );
			P.emplace_back( -hD, hW, 0 );

			P.emplace_back( -hD, -hW, BoxH );
			P.emplace_back( hD, -hW, BoxH );
			P.emplace_back( hD, hW, BoxH );
			P.emplace_back( -hD, hW, BoxH );

			double xmax = 0;
			{//軸と完全に向きが揃っていると見栄えが悪いのですこしだけ傾ける
				auto R = Rot( PI * 15 / 180.0 );
				for( auto &p : P )
				{	
					R(p);
					if( xmax<p[0] )xmax=p[0];
				}
			}
			Vec3d T{ 0.5-(xmax+0.01), 0.0, 0.0 };	//壁際に位置を寄せるための量

			int RotDeg = -90 * (int)Dir;
			if( RotDeg != 0 )
			{//引数による回転
				auto R = Rot( PI * RotDeg / 180.0 );
				for( auto &p : P )R(p);
				R(T);
			}

			//壁際に寄せる + 座標変換のオフセット
			T += { 0.5, 0.5, 0.0 };
			for( auto &p : P ){	p += T;	}
			
			//
			P.shrink_to_fit();
		}
		{//---線---
			auto &L = Obj.L;
			L.reserve( 12 );

			AddLoop( L, { 0,1,2,3 } );
			AddLoop( L, { 4,5,6,7 } );
			for( int i=0; i<4; ++i ){	L.emplace_back( i, i+4 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}
}