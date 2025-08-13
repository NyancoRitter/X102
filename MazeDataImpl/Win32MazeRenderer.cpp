#include "framework.h"
#include <windowsx.h>

#include <algorithm>

#include "Win32MazeRenderer.h"
#include "MazeRendering/RenderProp.h"

#include "FloorData.h"
#include "WireFrameObj.h"
#include "Parts/Mat3x3.h"


namespace
{
	using namespace PSEUDO_3D_MAZE;

	class MazeRenderProp : public RenderProp
	{
	public:
		MazeRenderProp(
			const CPinholeCamera &rCamModel,
			const Vec2d &CamPos, const Vec2d &CamFrontDir, double CamHeight,
			double Near, double Far
		)
			: m_PinholeCam(rCamModel)
			, m_CamPos(CamPos), m_CamFrontDir(CamFrontDir), m_CamHeight(CamHeight)
			, m_Near(Near), m_Far(Far)
		{}

	public:	// RenderProp を介して継承されました
		virtual const CPinholeCamera & CamModel() const override {	return m_PinholeCam;	}
		virtual double NearDepth() const override {	return m_Near;	}
		virtual double FarDepth() const override {	return m_Far;	}
		virtual Vec2d CamPos() const override {	return m_CamPos;	}
		virtual double CamHeight() const override {	return m_CamHeight;	}
		virtual Vec2d CamFrontDir() const override {	return m_CamFrontDir;	}

	private:
		const CPinholeCamera &m_PinholeCam;
		Vec2d m_CamPos;
		Vec2d m_CamFrontDir;
		double m_CamHeight;
		double m_Near;
		double m_Far;
	};

	//R,G,Bをそれぞれrate倍した色を作る
	inline COLORREF Color( BYTE R, BYTE G, BYTE B, double rate )
	{	
		return RGB(
			std::min( (int)(R * rate + 0.5), 255 ),
			std::min( (int)(G * rate + 0.5), 255 ), 
			std::min( (int)(B * rate + 0.5), 255 )
		);
	}

	inline COLORREF Color( COLORREF Src, double rate )
	{	return Color( (BYTE)GetRValue(Src), (BYTE)GetGValue(Src), (BYTE)GetBValue(Src), rate );	}

	//線分 PQ のうち，i番目の要素の値が範囲(Min～Max)に入っている部分を取り出す．
	//可能な場合，P,Q を結果で上書きしてtrueを返す．
	//	その際，i番目の要素が小さい側がP, 大きい側がQ となる．
	//不可能な場合（両端点が範囲外の場合）はfalseを返す．
	template< class Vtx >
	bool TrimLineSeg(Vtx &P, Vtx &Q, int i, double Min, double Max)
	{
		if( (P[i]<Min && Q[i]<Min) || (P[i]>Max && Q[i]>Max) ){	return false;	}

		if( P[i]>Q[i] )
		{	std::swap( P,Q );	}

		if( P[i]<Min )
		{
			double rate = double( Q[i] - Min ) / ( Q[i] - P[i] );
			P = Q + ( P - Q )*rate;
		}
		if( Q[i]>Max )
		{
			double rate = double( Max - P[i] ) / ( Q[i] -  P[i] );
			Q = P + ( Q - P )*rate;
		}
		return true;
	}

	//オブジェクトデータ
	using MazeDataImpl::WireFrameObj;
	using MazeDataImpl::Direction;
	const WireFrameObj WFObjs[] = {
		WireFrameObj::CreateUpStair(),
		WireFrameObj::CreateDownStair(),
		WireFrameObj::CreateCharacter(),
		WireFrameObj::CreateMagicCircle(),
		WireFrameObj::CreateTreasureBox( Direction::EAST ),
		WireFrameObj::CreateTreasureBox( Direction::SOUTH ),
		WireFrameObj::CreateTreasureBox( Direction::WEST ),
		WireFrameObj::CreateTreasureBox( Direction::NORTH )
	};

	//ドア描画色
	constexpr int N_DoorColor = 4;	//ドア種類数
	constexpr COLORREF DoorColors[N_DoorColor] = {
		RGB(128,86,64),
		RGB(16,28,164),
		RGB(8,160,24),
		RGB(180,0,0)
	};
}

//-------------------------------------

namespace MazeDataImpl
{
	Win32MazeRenderer::Win32MazeRenderer( int ImgW, int ImgH, double HorizonalFOV_rad, double Far )
		: m_ImgW(ImgW), m_ImgH(ImgH), m_hRenderTgtDC(NULL)
	{
		double PinholeCam_f = 0.5*ImgW / tan(HorizonalFOV_rad*0.5);
		m_PinholeCam.SetIPVals( (ImgW-1)*0.5, (ImgH-1)*0.5, PinholeCam_f );
		SetFarDepth( Far );
		ResetCamera( Vec2i(0,0), Direction::SOUTH );
	}

	Win32MazeRenderer::~Win32MazeRenderer()
	{
		if( m_hBMP!=NULL )
		{	::DeleteObject( m_hBMP );	}
	}

	void Win32MazeRenderer::SetFarDepth( double Far )
	{
		const double ColorRate_at_Far = 0.1;

		m_Far = Far;
		m_Dist2t_Coeff = ( 1.0 / sqrt(ColorRate_at_Far) - 1 ) / m_Far;
		m_IsDirty = true;
	}

	double Win32MazeRenderer::ColorRateFromDistance( double Dist ) const
	{
		const double RawRate_Max = 0.8;

		double t = m_Dist2t_Coeff * Dist + 1.0;
		double RawRate = (std::min)( RawRate_Max,  1.0 / (t*t) );
		return RawRate * m_BrightnessRate;
	}

	void Win32MazeRenderer::DrawVerticalLine(int x, int y0, int y1, int EdgeAttrVal, double Depth, double Dist, double LRPosRateOnEdge)
	{
		m_DepthBuffer[x] = Depth;

		double ColorRate = ColorRateFromDistance( Dist );
		const int WallTop_PixY = (std::clamp)( y0, 0, m_ImgH );
		const int WallBottom_PixY = (std::clamp)( y1, 0, m_ImgH );

		constexpr double EndLineWidth = 0.01;	//細線の幅（壁全体の幅を1.0としたときの）
		if( LRPosRateOnEdge<EndLineWidth || 1.0-EndLineWidth < LRPosRateOnEdge )
		{//壁の左右の縁に細い縦線を入れる
			::SetDCPenColor( m_hRenderTgtDC, Color(230,0,0, ColorRate ) );
			::MoveToEx( m_hRenderTgtDC, x, WallTop_PixY, nullptr );
			::LineTo( m_hRenderTgtDC, x, WallBottom_PixY );
			return;
		}

		COLORREF WallColor = Color(255,0,0, ColorRate );
		::MoveToEx( m_hRenderTgtDC, x, WallTop_PixY, nullptr );

		if( EdgeAttrVal == (int)EdgeAttr::WALL )
		{//壁
			::SetDCPenColor( m_hRenderTgtDC, WallColor );
			::LineTo( m_hRenderTgtDC, x, WallBottom_PixY );
			return;
		}
		else if( EdgeAttrVal >= (int)EdgeAttr::DOOR )
		{//ドア
			constexpr double DoorWidth = 0.6;	//ドアの幅（壁全体の幅を1.0としたときの）
			if( double FromCenter = fabs(0.5 - LRPosRateOnEdge); FromCenter <= DoorWidth*0.5 )
			{//ドアを描画するx範囲
				constexpr double DoorTop = 0.25;	//ドアの上端位置（壁全体の高さを1.0とし，地面の位置を0としたときの）
				int EndY = (std::clamp)( int( y0*(1.0-DoorTop) + y1*DoorTop ), 0, m_ImgH );

				//ドアより上側を壁の色で描画
				SetDCPenColor( m_hRenderTgtDC, WallColor );
				LineTo( m_hRenderTgtDC, x, EndY );

				//
				constexpr double BorderThickness = 0.04;	//枠の太さ（壁全体の幅を1.0としたときの）
				COLORREF BorderColor = Color(232,160,28, ColorRate);
				if( FromCenter > DoorWidth*0.5 - BorderThickness )
				{//ドア左右の枠
					SetDCPenColor( m_hRenderTgtDC, BorderColor );
					LineTo( m_hRenderTgtDC, x, WallBottom_PixY );
					return;
				}

				//枠上側
				EndY = (std::clamp)( int( y0*(1.0-DoorTop-BorderThickness) + y1*(DoorTop+BorderThickness) ), 0, m_ImgH );
				SetDCPenColor( m_hRenderTgtDC, BorderColor );
				LineTo( m_hRenderTgtDC, x, EndY );

				//ドア面
				const int DoorColorIndex = std::min( EdgeAttrVal - (int)EdgeAttr::DOOR, N_DoorColor-1 );	//※念のため，想定外の値が範囲外参照にならないようにしておくが
				SetDCPenColor( m_hRenderTgtDC, Color( DoorColors[DoorColorIndex], ColorRate ) );
				LineTo( m_hRenderTgtDC, x, WallBottom_PixY );
			}
			else
			{//残りの左右は壁と同じ描画
				::SetDCPenColor( m_hRenderTgtDC, WallColor );
				::LineTo( m_hRenderTgtDC, x, WallBottom_PixY );
			}
		}
	}

	//
	bool Win32MazeRenderer::Draw( HDC hdc, const FloorData &MazeData )
	{
		if( m_hBMP == NULL )
		{
			m_hBMP = ::CreateCompatibleBitmap( hdc, m_ImgW, m_ImgH );
			if( m_hBMP==NULL )return false;
			m_IsDirty = true;
		}

		m_hRenderTgtDC = ::CreateCompatibleDC( hdc );
		if( m_hRenderTgtDC == NULL )return false;
		HBITMAP m_hOldBMP = SelectBitmap( m_hRenderTgtDC, m_hBMP );

		if( m_IsDirty )
		{
			m_IsDirty = false;

			{//黒で塗りつぶす
				RECT rect{ 0,0, m_ImgW, m_ImgH };
				::FillRect( m_hRenderTgtDC, &rect, (HBRUSH)::GetStockObject( BLACK_BRUSH ) );
			}

			HPEN hOldPen = SelectPen( m_hRenderTgtDC, GetStockPen( DC_PEN ) );
			{
				const Vec2d FrontDir( cos(m_CamDir_rad), sin(m_CamDir_rad) );
				const Vec2d CamPos = m_RawCamPos - m_BackOffsetAtDraw*FrontDir;
				m_DepthBuffer.assign( m_ImgW, std::numeric_limits<double>::max() );

				PSEUDO_3D_MAZE::RenderMazeEdge(
					MazeData,
					MazeRenderProp( m_PinholeCam, CamPos, FrontDir, m_CamHeight, ms_Near, m_Far ),
					*this
				);

				if( m_bDrawWireFrameObj )
				{	DrawWireFrameObjects( m_hRenderTgtDC, MazeData, CamPos, FrontDir );	}
			}
			SelectPen( hdc, hOldPen );
		}
		
		BitBlt( hdc, 0,0, m_ImgW, m_ImgH, m_hRenderTgtDC, 0,0, SRCCOPY );
		SelectBitmap( m_hRenderTgtDC, m_hOldBMP );
		DeleteDC( m_hRenderTgtDC );
		m_hRenderTgtDC = NULL;
		return true;
	}

	//
	void Win32MazeRenderer::DrawWireFrameObjects( HDC hdc, const FloorData &MazeData, const Vec2d &CamPos_2d, const Vec2d &FrontDir_d2 )
	{
		//※
		//迷路の壁の描画仕様および，
		//「オブジェクトは，１マスサイズ（底辺が地面に接する一辺の長さが1の立方体内）に収まっている」
		//…という前提条件に基づく実装になっている．

		//奥行が Near～Far にある物体の情報だけを収集
		using Info_t = std::pair< double, std::map<Vec2i,MazeDataImpl::ObjInfo>::const_iterator >;
		std::vector< Info_t > Objs;
		{
			const auto &ObjInfo = MazeData.GetObjectInfo();
			Objs.reserve( ObjInfo.size() );
			for( auto i=ObjInfo.begin(); i!=ObjInfo.end(); ++i )
			{
				Vec2d ObjCenter_2d( i->first[0]+0.5, i->first[1]+0.5 );
				Vec2d FromCam = ObjCenter_2d - CamPos_2d;
				double Dist = FromCam.L2Norm();
				if( FromCam * FrontDir_d2 < ms_Near   ||   Dist > m_Far )continue;

				Objs.emplace_back( Dist, i );
			}
		}
		if( Objs.empty() )return;
		//遠いものから先に描画するようにソート
		std::sort(
			Objs.begin(), Objs.end(),
			[](const Info_t &lhs, const Info_t &rhs)->bool{	return (lhs.first > rhs.first);	}
		);

		//
		const Vec3d CamPosW( CamPos_2d[0], CamPos_2d[1], m_CamHeight );
		Mat3x3d R;
		R.RowVec(0).Assign( -FrontDir_d2[1], FrontDir_d2[0], 0 );
		R.RowVec(1).Assign( 0, 0, -1 );
		R.RowVec(2).Assign( FrontDir_d2[0], FrontDir_d2[1], 0 );

		for( const auto &Info : Objs )
		{
			{
				unsigned char B = Info.second->second.Brightness;
				SetDCPenColor( hdc, Color( B,B,B, ColorRateFromDistance( Info.first ) ) );
			}

			const WireFrameObj &WFO = WFObjs[ (int)Info.second->second.Type ];

			//オブジェクトの頂点群のカメラ座標系での座標を計算
			std::vector< Vec3d > Vtx;
			{
				Vtx.reserve( WFO.P.size() );

				const auto &Pos2D = Info.second->first;
				const Vec3d T = Vec3d( Pos2D[0], Pos2D[1], 0 ) - CamPosW;
				for( const auto &P : WFO.P )
				{	Vtx.push_back( R * (P + T) );	}
			}

			for( const auto &L : WFO.L )
			{
				Vec3d V[2] = { Vtx[L.first], Vtx[L.second] };
				if( !TrimLineSeg( V[0], V[1], 2, ms_Near, m_Far ) )continue;

				for( auto &v : V )	//Vの内容を(pix_x, pix_y, depth) にする
				{
					auto pix =  m_PinholeCam.Cvt_CamCoord2PixCoord( v );
					v[0] = pix[0];
					v[1] = pix[1];
				}
				if( !TrimLineSeg( V[0], V[1], 1, 0, m_ImgH-1 ) )continue;
				if( !TrimLineSeg( V[0], V[1], 0, 0, m_ImgW-1 ) )continue; //関数仕様より，V[0][0] <= V[1][0] な結果になる

				Vec2i PixPos[2] = { {-1,-1}, {-1,-1} };

				int L = int(V[0][0]);
				int R = int(V[1][0]);
				if( L==R )
				{
					if( V[0][2] < m_DepthBuffer[L] )
					{
						for( int i=0; i<2; ++i )
						{	PixPos[i].Assign( int(V[i][0]), int(V[i][1]) );	}
					}
					else
					{	continue;	}
				}
				else if( V[0][2] < m_DepthBuffer[L]  &&  V[1][2] < m_DepthBuffer[R] )
				{
					for( int i=0; i<2; ++i )
					{	PixPos[i].Assign( int(V[i][0]), int(V[i][1]) );	}
				}
				else
				{
					auto d = ( V[1] - V[0] )/double(R - L);
					int iPixPos = 0;
					double depth = V[0][2];
					for( int x=L; x<=R; ++x, depth+=d[2] )
					{
						if( depth < m_DepthBuffer[x] )
						{
							PixPos[iPixPos][0] = x;
							if( iPixPos == 0 )
							{
								PixPos[1][0] = x;
								++iPixPos;
							}
						}
						else if( iPixPos == 1 )
						{	PixPos[1][0] = x;	break;	}
					}
					if( PixPos[0][0] < 0 )continue;

					PixPos[0][1] = int( V[0][1] + ( PixPos[0][0] - L ) * d[1] );
					PixPos[1][1] = int( V[0][1] + ( PixPos[1][0] - L ) * d[1] );
				}

				MoveToEx( hdc, PixPos[0][0], PixPos[0][1], nullptr );
				LineTo( hdc, PixPos[1][0], PixPos[1][1] );
			}
		}
	}

}

