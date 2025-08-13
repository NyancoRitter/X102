#include "framework.h"
#include "WireFrameObj.h"

namespace
{
#if 1
	//Z������ɉ񂷍��
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

	//�w���p�DLines�� indexes �����Ɍ��ԃ��[�v��ǉ��D
	inline void AddLoop( std::vector<std::pair<int,int>> &Lines, std::vector<int> indexes )
	{
		const int N = (int)indexes.size();
		for( int i=0; i<N-1; ++i ){	Lines.emplace_back( indexes[i], indexes[i+1] );	}
		Lines.emplace_back( indexes[N-1], indexes[0] );
	}
}

namespace MazeDataImpl
{
	//����͂���
	WireFrameObj WireFrameObj::CreateUpStair()
	{
		WireFrameObj Obj;
		{//---���_---
			auto &P = Obj.P;
			P.resize( 4 + 2*2 + 2*6 );
			//��
			P[0].Assign( 0.15, 0.15, 1.0 );
			P[1].Assign( 0.85, 0.15, 1.0 );
			P[2].Assign( 0.85, 0.85, 1.0 );
			P[3].Assign( 0.15, 0.85, 1.0 );
			//
			double x1 = 0.4;	double y1 = 0.35;
			double x2 = 0.6;	double y2 = 0.65;
			//�͂������E�̖_
			P[4].Assign( x1, y1, 1.0 );
			P[5].Assign( x1, y1, 0.0 );
			P[6].Assign( x2, y2, 1.0 );
			P[7].Assign( x2, y2, 0.0 );
			//�͂������_�Q
			for( int i=0; i<6; ++i )
			{
				P[8+i*2].Assign( x1, y1, (i+1)*0.15 );
				P[9+i*2].Assign( x2, y2, (i+1)*0.15 );
			}

			P.shrink_to_fit();
		}
		{//---��---
			auto &L = Obj.L;
			L.reserve( 4 + 2 + 6 );
			//��
			AddLoop( L, {0,1,2,3} );
			//�͂������E�̖_
			L.emplace_back( 4,5 );
			L.emplace_back( 6,7 );
			//�͂������_�Q
			for( int i=0; i<6; ++i )
			{	L.emplace_back( 8+i*2, 9+i*2 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}

	//�~��͂���
	WireFrameObj WireFrameObj::CreateDownStair()
	{
		WireFrameObj Obj;
		{//---���_---
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
		{//---��---
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

	//�l��
	WireFrameObj WireFrameObj::CreateCharacter()
	{
		WireFrameObj Obj;

		{//---���_---
			auto &P = Obj.P;
			P.reserve( 6 + 5 );

			//��
			constexpr double cz = 0.52;
			constexpr double r = 0.035;
			P.emplace_back( 0,0,cz-r );
			P.emplace_back( r,0,cz );
			P.emplace_back( 0,r,cz );
			P.emplace_back( -r,0,cz );
			P.emplace_back( 0,-r,cz );
			P.emplace_back( 0,0,cz+r );
			//��
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
		{//---��---
			auto &L = Obj.L;
			L.reserve( 12 + 8 );
			//��
			AddLoop( L, { 1,2,3,4 } );
			for( int i=1; i<=4; ++i )
			{
				L.emplace_back( 0,i );
				L.emplace_back( i,5 );
			}
			//��
			AddLoop( L, { 6,7,8,9 } );
			for( int i=6; i<=9; ++i )
			{	L.emplace_back( i,10 );	}

			L.shrink_to_fit();
		}
		return Obj;
	}

	//���@�w
	WireFrameObj WireFrameObj::CreateMagicCircle()
	{
		WireFrameObj Obj;
		constexpr int Reso = 3;
		constexpr int N = Reso*5;
		{//---���_---
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
		{//---��---
			auto &L = Obj.L;
			L.reserve( N + 5 );
			//�~
			for( int i=0; i+1<N; ++i )L.emplace_back( i, i+1 );
			L.emplace_back( N-1, 0 );
			//��
			AddLoop( L, { 0, Reso*2, Reso*4, Reso, Reso*3 } );

			L.shrink_to_fit();
		}
		return Obj;
	}

	//��
	WireFrameObj WireFrameObj::CreateTreasureBox(  Direction Dir )
	{
		WireFrameObj Obj;

		{//---���_---
			auto &P = Obj.P;
			P.reserve( 8 );

			//���ȕւ��̂��߁C�}�X�̒��S�����_�Ƃ�����W�n�Œ��_�ʒu���`
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
			{//���Ɗ��S�Ɍ����������Ă���ƌ��h���������̂ł����������X����
				auto R = Rot( PI * 15 / 180.0 );
				for( auto &p : P )
				{	
					R(p);
					if( xmax<p[0] )xmax=p[0];
				}
			}
			Vec3d T{ 0.5-(xmax+0.01), 0.0, 0.0 };	//�ǍۂɈʒu���񂹂邽�߂̗�

			int RotDeg = -90 * (int)Dir;
			if( RotDeg != 0 )
			{//�����ɂ���]
				auto R = Rot( PI * RotDeg / 180.0 );
				for( auto &p : P )R(p);
				R(T);
			}

			//�ǍۂɊ񂹂� + ���W�ϊ��̃I�t�Z�b�g
			T += { 0.5, 0.5, 0.0 };
			for( auto &p : P ){	p += T;	}
			
			//
			P.shrink_to_fit();
		}
		{//---��---
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