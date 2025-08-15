#pragma once

#include "Common/Effect.h"
#include "MazeDataImpl/Win32MazeRenderer.h"
#include <functional>
#include <memory>

namespace Maze
{
	using namespace MazeDataImpl;

	/// <summary>
	/// 移動．完了時に座標データを更新．
	/// </summary>
	class WalkEffect : public IEffect
	{
	public:
		WalkEffect( Win32MazeRenderer &Renderer ) : m_Renderer(Renderer) {}

	public:
		//「nFrame かけて 位置を From から To へ変化させる」ようにカメラを動かす．
		//UpdatePosFunc : 位置データ更新手段
		void Reset( const Vec2i &From, const Vec2i &To, int nFrame, std::function<void(Vec2i)> UpdatePosFunc )
		{
			m_UpdatePosFunc = UpdatePosFunc;
			m_nRestFrame = nFrame;
			m_To = To;

			auto d = To - From;
			m_dPosPerFrame.Assign( (double)d[0]/nFrame, (double)d[1]/nFrame );
		}

	public:	// IEffect
		virtual bool Update() override
		{
			--m_nRestFrame;
			if( m_nRestFrame > 0 )
			{
				m_Renderer.OffsetCameraPos( m_dPosPerFrame );
				return true;
			}
			else
			{//データ更新
				m_UpdatePosFunc( m_To );
				return false;
			}
		}

		virtual bool SuppressSubsequents() const {	return true;	}

	private:
		Win32MazeRenderer &m_Renderer;
		std::function<void(Vec2i)> m_UpdatePosFunc;

		int m_nRestFrame = 0;
		Vec2d m_dPosPerFrame;
		Vec2i m_To;
	};

	/// <summary>
	/// 左右90度旋回．完了後に向きデータを更新．
	/// </summary>
	class TurnEffect : public IEffect
	{
	public:
		TurnEffect( Win32MazeRenderer &Renderer ) : m_Renderer(Renderer) {}

	public:
		//「nFrame かけて 向き CurrDir から 90 回す」ようにカメラを動かす．
		//RightTurn : true なら右旋回，false なら左旋回．
		//UpdateDirFunc : 向きデータ更新手段
		void Reset( Direction CurrDir, bool RightTurn, int nFrame, std::function<void(Direction)> UpdateDirFunc )
		{
			m_UpdateDirFunc = UpdateDirFunc;
			m_nRestFrame = nFrame;
			m_DirAtFinish = ( RightTurn ? RightDirOf(CurrDir) : LeftDirOf(CurrDir) );
			m_dAngle = (PI * 0.5) / nFrame;
			if( !RightTurn )m_dAngle = -m_dAngle;
		}

	public:	// IGameCmd を介して継承されました
		virtual bool Update() override
		{
			--m_nRestFrame;
			if( m_nRestFrame > 0 )
			{
				m_Renderer.RotateCamera( m_dAngle );
				return true;
			}
			else
			{//データ更新
				m_UpdateDirFunc( m_DirAtFinish );
				return false;
			}
		}

		virtual bool SuppressSubsequents() const {	return true;	}

	private:
		Win32MazeRenderer &m_Renderer;
		std::function<void(Direction)> m_UpdateDirFunc;

		int m_nRestFrame = 0;
		Direction m_DirAtFinish = Direction::EAST;	//初期化値には意味ない
		double m_dAngle = 0;
	};

	//はしごによる上下移動（カメラ演出のみ）
	class LadderEffect : public IEffect
	{
	public:
		static std::unique_ptr<LadderEffect> DownToFloor( Win32MazeRenderer &Renderer, int nFrame ){	return std::make_unique<LadderEffect>( Renderer, 0.5, 0, nFrame );	}
		static std::unique_ptr<LadderEffect> DownFromCeil( Win32MazeRenderer &Renderer, int nFrame ){	return std::make_unique<LadderEffect>( Renderer, 1.0, 0.5, nFrame );	}
		static std::unique_ptr<LadderEffect> UpFromFloor( Win32MazeRenderer &Renderer, int nFrame ){	return std::make_unique<LadderEffect>( Renderer, 0, 0.5, nFrame );	}
		static std::unique_ptr<LadderEffect> UpToCeil( Win32MazeRenderer &Renderer, int nFrame ){	return std::make_unique<LadderEffect>( Renderer, 0.5, 1.0, nFrame );	}
	public:
		//「nFrame かけてカメラ高さを FromHeight から ToHeight に動かす」
		LadderEffect( Win32MazeRenderer &Renderer, double FromHeight, double ToHeight, int nFrame )
			: m_Renderer(Renderer), m_CamHeight(FromHeight), m_ToHeight(ToHeight), m_nRestFrame( nFrame )
		{	m_dHeight = (ToHeight - FromHeight ) / nFrame;	}

	public:	// IGameCmd を介して継承されました
		virtual bool Update() override
		{
			--m_nRestFrame;
			if( m_nRestFrame > 0 )
			{
				m_CamHeight += m_dHeight;
				m_Renderer.SetCameraHeight( m_CamHeight );
				SetBrightnessRate( m_CamHeight );
				return true;
			}
			else
			{
				m_Renderer.SetCameraHeight( m_ToHeight );
				SetBrightnessRate( m_ToHeight );
				return false;
			}
		}

		virtual bool SuppressSubsequents() const {	return true;	}

	private:
		void SetBrightnessRate(double H )
		{	m_Renderer.SetBrightnessRate( 1.0 - fabs(0.5 - H)/0.5 );	}
	private:
		Win32MazeRenderer &m_Renderer;
		int m_nRestFrame;
		double m_CamHeight;
		double m_dHeight;
		double m_ToHeight;
	};

	/// <summary>
	/// 階層移動．完了時に現在階層データを更新．
	/// </summary>
	class ChangeFloorEffect : public IEffect
	{
	public:
		//「nFrame*2 フレームをかけて 隣の階層に移動する」ようにカメラを動かす．
		//ToUpperFloor : 上下どちらの隣接階に移動するか．trueなら上の階（：階層値が小さくなる側）．
		//UpdateFloorFunc : 階層データ更新手段．移動先の階層が引数に与えられる．
		ChangeFloorEffect( Win32MazeRenderer &Renderer, int CurrFloor, bool ToUpperFloor, int nFrame, std::function<void(int)> UpdateFloorFunc )
		{
			m_UpdateFloorFunc = UpdateFloorFunc;
			if( ToUpperFloor )
			{
				m_ToFloor = CurrFloor - 1;
				m_upAnim[0] = LadderEffect::UpToCeil( Renderer, nFrame );
				m_upAnim[1] = LadderEffect::UpFromFloor( Renderer, nFrame );
			}
			else
			{
				m_ToFloor = CurrFloor + 1;
				m_upAnim[0] = LadderEffect::DownToFloor( Renderer, nFrame );
				m_upAnim[1] = LadderEffect::DownFromCeil( Renderer, nFrame );
			}
		}

	public:	// IEffect
		virtual bool Update() override
		{
			if( m_iCurrAnim>=2 )return false;

			if( !m_upAnim[m_iCurrAnim]->Update() )
			{
				if( m_iCurrAnim==0 )
				{	m_UpdateFloorFunc( m_ToFloor );	}

				++m_iCurrAnim;
				if( m_iCurrAnim>=2 )return false;
			}
			return true;
		}

		virtual bool SuppressSubsequents() const {	return true;	}

	private:
		std::function<void(int)> m_UpdateFloorFunc;

		int m_ToFloor;
		int m_iCurrAnim = 0;
		std::unique_ptr<LadderEffect> m_upAnim[2];
	};
}