#pragma once

#include "IScene.h"
#include "GUI/IGUI.h"
#include "MazeDataImpl/FloorData.h"
#include "MazeDataImpl/Win32MazeRenderer.h"
#include <string>
#include <memory>
#include "Misc/Cmd.h"
#include "Common/PainterSeq.h"

class ITopLV;
class PlayData;

namespace Maze
{
	//class WalkEffect;
	//class TurnEffect;

	/// <summary>
	/// ���H�T���V�[��
	/// </summary>
	class MazeScene final : public IScene
	{
	public:
		MazeScene( ITopLV &rTopLV, const std::string &MazeFilePathU8 );

	public:	//IMazeScene
		virtual void OnEnter() override;
		virtual void OnLeave() override;
		virtual Flags<SceneUpdateResult> Update( const IController &Controller ) override;
		virtual void Draw( HDC hdc ) override;

	private:
		void OpenCampMenu();

		bool BeginWalk( MazeDataImpl::Direction Dir );
		void BeginTurn( bool ToRight );
		bool Inspect();

		//bool StartEventProc( Event::EventTrigger Trigger );
		void RandomEncount();
		void Encount( const std::vector<unsigned int> &EnemyIDs, bool IsEscapableBattle );
		void DrawPosInfo( HDC hdc, const RECT &Rect );

		//void PlayMazeBGM();

	

	private:
		//���H�T��������
		class UsualUpdater : public GUI::IGUI
		{
		private:
			MazeScene &m_Outer;
		public:
			UsualUpdater( MazeScene &Outer ) : m_Outer(Outer) {}
		public:	//GUI::IGUI
			virtual Flags<GUI::GUIResult> Update( const IController &Controller ) override {	return m_Outer.UsualUpdate(Controller);	}
			//virtual void OnGotFocus() override {	m_Outer.m_bShowPosInfo = true;	}
			//virtual void OnLostFocus() override {	m_Outer.m_bShowPosInfo = false;	}
		protected:
			virtual void Paint_( HDC hdc ) const override {	m_Outer.UsualPaint(hdc);	}
		};

		void UsualPaint( HDC hdc );
		Flags<GUI::GUIResult> UsualUpdate( const IController &Controller );

	private:
		ITopLV &m_rTopLV;
		GUI::GUIStack m_Stack;
		CmdSequence<> m_CmdSeq;
		PainterSeq m_EffectsPainter;

		MazeDataImpl::Win32MazeRenderer m_Renderer;

		UsualUpdater m_UsualUpdater;

		//�}�b�v�f�[�^
		std::vector< MazeDataImpl::FloorData > m_MazeMap;
		Vec2i m_StartPos;	//���{�̃X�^�[�g�}�X
		MazeDataImpl::Direction m_StartDir = MazeDataImpl::Direction::EAST;	//�X�^�[�g���̌����i�������l�͓��ɈӖ������j

		//���݈ʒu��
		Vec2i m_CurrPos;	//���݈ʒu
		MazeDataImpl::Direction m_CurrDir = MazeDataImpl::Direction::EAST;	//���݂̌����i�������l�͓��ɈӖ������j
		int m_CurrFloor = 0;	//���݉��K�ɂ��邩(0-based)
		int m_EncountPercentage = 0;	//�G���J�E���g��[%]
		bool m_bShowPosInfo = true;

#ifdef _DEBUG
		//�f�o�b�O�p
		bool m_TestModeFlag = false;
#endif
	private:
		//�e��G�t�F�N�g�̑��x
		static inline const int ms_nAnimFrame_Walk = 8;
		static inline const int ms_nAnimFrame_Turn = 8;
		static inline const int ms_nAnimFrame_Ladder = 40;
	};
}