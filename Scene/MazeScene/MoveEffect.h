#pragma once

#include "Common/Effect.h"
#include "MazeDataImpl/Win32MazeRenderer.h"
#include <functional>
#include <memory>

namespace Maze
{
	using namespace MazeDataImpl;

	/// <summary>�ړ����A�j���[�V�����p</summary>
	class WalkEffect
	{
	public:
		/// <summary>
		/// ctor. �uRenderer �� �J�����ʒu�� nFrame ������ From ���� To �Ɉړ�������v�Ƃ����ݒ�
		/// </summary>
		WalkEffect( Win32MazeRenderer &Renderer, const Vec2i &From, const Vec2i &To, int nFrame )
			: m_Renderer( Renderer )
			, m_nRestFrame( nFrame )
			, m_To( To )
		{
			auto d = To - From;
			m_dPosPerFrame.Assign( (double)d[0]/nFrame, (double)d[1]/nFrame );
		}

	public:
		Flags<CmdResult> operator()()
		{
			if( m_nRestFrame > 0 )
			{
				--m_nRestFrame;
				m_Renderer.OffsetCameraPos( m_dPosPerFrame );
				return CmdResult::ReqRedraw | CmdResult::SuppressSubsequents;
			}
			else
			{	return CmdResult::Finished;	}
		}

	private:
		Win32MazeRenderer &m_Renderer;
		int m_nRestFrame = 0;
		Vec2d m_dPosPerFrame;
		Vec2i m_To;
	};

	/// <summary>���񎞃A�j���[�V�����p</summary>
	class TurnEffect
	{
	public:
		/// <summary>
		/// ctor. �uRenderer �� �J�����̌����� nFrame ������ CurrDir ���� 90�x �񂷁v�Ƃ����ݒ�
		/// </summary>
		TurnEffect( Win32MazeRenderer &Renderer, Direction CurrDir, bool RightTurn, int nFrame )
			: m_Renderer( Renderer )
			, m_nRestFrame( nFrame )
		{
			m_dAngle = (PI * 0.5) / nFrame;
			if( !RightTurn )m_dAngle = -m_dAngle;
		}

	public:
		Flags<CmdResult> operator()()
		{
			if( m_nRestFrame > 0 )
			{
				--m_nRestFrame;
				m_Renderer.RotateCamera( m_dAngle );
				return CmdResult::ReqRedraw | CmdResult::SuppressSubsequents;
			}
			else
			{	return CmdResult::Finished;	}
		}

	private:
		Win32MazeRenderer &m_Renderer;
		int m_nRestFrame = 0;
		double m_dAngle = 0;
	};


	//�͂����ɂ��㉺�ړ��A�j���[�V����
	class LadderEffect
	{
	public:
		static LadderEffect DownToFloor( Win32MazeRenderer &Renderer, int nFrame ){	return LadderEffect( Renderer, 0.5, 0, nFrame );	}
		static LadderEffect DownFromCeil( Win32MazeRenderer &Renderer, int nFrame ){	return LadderEffect( Renderer, 1.0, 0.5, nFrame );	}
		static LadderEffect UpFromFloor( Win32MazeRenderer &Renderer, int nFrame ){	return LadderEffect( Renderer, 0, 0.5, nFrame );	}
		static LadderEffect UpToCeil( Win32MazeRenderer &Renderer, int nFrame ){	return LadderEffect( Renderer, 0.5, 1.0, nFrame );	}
	public:
		/// <summary>
		/// ctor. �uRenderer �̃J���������� FromHeight ���� ToHeight �� nFrame  �����ē������i�`�斾�邳�������ɉ����ĕω��j�v�Ƃ����ݒ�D
		/// </summary>
		LadderEffect( Win32MazeRenderer &Renderer, double FromHeight, double ToHeight, int nFrame )
			: m_Renderer(Renderer), m_CamHeight(FromHeight), m_ToHeight(ToHeight), m_nRestFrame( nFrame )
		{	m_dHeight = (ToHeight - FromHeight ) / nFrame;	}

	public:
		Flags<CmdResult> operator()()
		{
			if( m_nRestFrame > 0 )
			{
				--m_nRestFrame;
				m_CamHeight += m_dHeight;
				m_Renderer.SetCameraHeight( m_CamHeight );
				SetBrightnessRate( m_CamHeight );
				return CmdResult::ReqRedraw | CmdResult::SuppressSubsequents;
			}
			else
			{
				m_Renderer.SetCameraHeight( m_ToHeight );
				SetBrightnessRate( m_ToHeight );
				return CmdResult::Finished;
			}
		}

	private:
		//�J���������ɉ������`�斾�邳�ݒ�
		void SetBrightnessRate( double H ){	m_Renderer.SetBrightnessRate( 1.0 - fabs(0.5 - H)/0.5 );	}
	private:
		Win32MazeRenderer &m_Renderer;
		int m_nRestFrame;
		double m_CamHeight;
		double m_dHeight;
		double m_ToHeight;
	};

}
