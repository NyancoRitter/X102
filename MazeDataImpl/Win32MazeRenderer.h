#pragma once

#include "MazeRendering/MazeRendering.h"
#include "MazeRendering/PinholeCamera.h"
//#include "Parts/Vec2D.h"
//#include "MazeSceneDefs.h"
#include "Direction.h"

#include <vector>

namespace MazeDataImpl
{
	class FloorData;

	/// <summary>
	/// HDC に描画する PSEUDO_3D_MAZE::IRenderTgt 実装．
	/// カメラ位置姿勢や描画明るさ関係の設定値もここが保持．
	/// </summary>
	class Win32MazeRenderer : public PSEUDO_3D_MAZE::IRenderTgt
	{
	public:
		Win32MazeRenderer( int ImgW, int ImgH, double HorizonalFOV_rad, double Far=2.8 );
		~Win32MazeRenderer();
	private:
		Win32MazeRenderer( const Win32MazeRenderer & ) = delete;
		Win32MazeRenderer &operator=( const Win32MazeRenderer & ) = delete;

	public:	// PSEUDO_3D_MAZE::IRenderTgt
		virtual int ImageWidth() const override {	return m_ImgW;	}
		virtual int ImageHeight() const override {	return m_ImgH;	}
		virtual void DrawVerticalLine(int x, int y0, int y1, int EdgeAttr, double Depth, double Dist, double LRPosRateOnEdge) override;

	public:	//描画範囲，明るさ関係
		void SetFarDepth( double Far );
		void SetBrightnessRate( double Rate ){	m_BrightnessRate = Rate;	m_IsDirty = true;	}
	private:
		double ColorRateFromDistance( double Dist ) const;

	public:	//カメラ位置姿勢

		//カメラ位置と姿勢の設定
		//* Pos : 現在位置（迷路マス座標）
		//* Dir : 向き（４方位）
		//* CamHeight : カメラ高さ
		void ResetCamera( const Vec2i &Pos, Direction Dir, double CamHeight=0.5 )
		{
			m_RawCamPos.Assign( Pos[0]+0.5, Pos[1]+0.5 );
			m_CamDir_rad = AngleOf(Dir);
			m_CamHeight = CamHeight;
			m_IsDirty = true;
		}

		//カメラ位置を指定量だけ平行移動する
		void OffsetCameraPos( const Vec2d &dPos ){	m_RawCamPos += dPos;	m_IsDirty = true;	}
		//カメラ高さの変更
		void SetCameraHeight( double CamHeight ){	m_CamHeight = CamHeight;	m_IsDirty = true;	}
		//カメラの左右旋回
		void RotateCamera( double dDirRad ){	m_CamDir_rad += dDirRad;	m_IsDirty = true;	}

		//Draw()での描画時にカメラ位置を後ろに下げる量．
		//※常に一定量だけ後ろに下げて描画することで画角を大きくすることを避けつつ視界を確保する用の設定．
		void BackOffsetAtDraw( double BackOffset ){	m_BackOffsetAtDraw = BackOffset;	m_IsDirty = true;	}

	public:	//描画処理

		//次回の Draw() で完全に再描画させる
		void RequestRefresh(){	m_IsDirty=true;	}

		//Draw()でワイヤーフレームオブジェクトも描画するか否か．
		//false設定の場合には，壁面の描画のみを行う．
		void EnableWireFrameObj( bool Enable ){	m_bDrawWireFrameObj = Enable;	m_IsDirty = true;	}

		/// <summary>
		/// 迷路の描画
		///		<remarks>
		///		前回から設定等が更新されていない場合，１から描画するのではなくて
		///		キャッシュしておいた前回の描画結果が用いられる．
		///		それだとまずい場合には，事前に RequestRefresh() を呼ぶこと．
		///		</remarks>
		/// </summary>
		/// <param name="hdc">描画対象</param>
		/// <param name="MazeData">迷路データ</param>
		/// <returns>成否．失敗時にはhdcに対しての描画状態は不定．</returns>
		bool Draw( HDC hdc, const FloorData &MazeData );

	private:
		void DrawWireFrameObjects( HDC hdc, const FloorData &MazeData, const Vec2d &CamPos, const Vec2d &FrontDir );

	private:
		//描画画像サイズ[pixel]
		const int m_ImgW;
		const int m_ImgH;

		//※Draw()は結果をキャッシュしており，再レンダリングが不要な場合にはキャッシュ画像をBitBltするだけ．
		HBITMAP m_hBMP = NULL;	//画像バッファ
		bool m_IsDirty = true;	//レンダリング必要フラグ

		//描画先HDC
		//※PSEUDO_3D_MAZE::IRenderTgt::DrawVerticalLine() で必要なので一次的に記憶する必要がある
		HDC m_hRenderTgtDC = NULL;

		//カメラ
		Vec2d m_RawCamPos;
		double m_CamHeight;
		double m_CamDir_rad;
		PSEUDO_3D_MAZE::CPinholeCamera m_PinholeCam;
		double m_BackOffsetAtDraw = 0.25;

		//NearとFar
		static inline const double ms_Near = 0.01;
		double m_Far;

		//色
		double m_Dist2t_Coeff;	//（詳細情報が残っていない．Near->Far で暗くなっていく描画色を決める数式内の部分式の値）
		double m_BrightnessRate = 1.0;	//描画色に乗じられる係数．

		//Draw() でワイヤーフレームオブジェクトを描画するか否かの設定
		bool m_bDrawWireFrameObj = true;
		//壁描画結果のデプスバッファ(pixel x座標毎)
		//（ワイヤーフレームオブジェクト描画時に使用）
		std::vector< double > m_DepthBuffer;
	};
}
