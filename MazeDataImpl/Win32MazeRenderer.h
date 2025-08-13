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
	/// HDC �ɕ`�悷�� PSEUDO_3D_MAZE::IRenderTgt �����D
	/// �J�����ʒu�p����`�斾�邳�֌W�̐ݒ�l���������ێ��D
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

	public:	//�`��͈́C���邳�֌W
		void SetFarDepth( double Far );
		void SetBrightnessRate( double Rate ){	m_BrightnessRate = Rate;	m_IsDirty = true;	}
	private:
		double ColorRateFromDistance( double Dist ) const;

	public:	//�J�����ʒu�p��

		//�J�����ʒu�Ǝp���̐ݒ�
		//* Pos : ���݈ʒu�i���H�}�X���W�j
		//* Dir : �����i�S���ʁj
		//* CamHeight : �J��������
		void ResetCamera( const Vec2i &Pos, Direction Dir, double CamHeight=0.5 )
		{
			m_RawCamPos.Assign( Pos[0]+0.5, Pos[1]+0.5 );
			m_CamDir_rad = AngleOf(Dir);
			m_CamHeight = CamHeight;
			m_IsDirty = true;
		}

		//�J�����ʒu���w��ʂ������s�ړ�����
		void OffsetCameraPos( const Vec2d &dPos ){	m_RawCamPos += dPos;	m_IsDirty = true;	}
		//�J���������̕ύX
		void SetCameraHeight( double CamHeight ){	m_CamHeight = CamHeight;	m_IsDirty = true;	}
		//�J�����̍��E����
		void RotateCamera( double dDirRad ){	m_CamDir_rad += dDirRad;	m_IsDirty = true;	}

		//Draw()�ł̕`�掞�ɃJ�����ʒu�����ɉ�����ʁD
		//����Ɉ��ʂ������ɉ����ĕ`�悷�邱�Ƃŉ�p��傫�����邱�Ƃ�������E���m�ۂ���p�̐ݒ�D
		void BackOffsetAtDraw( double BackOffset ){	m_BackOffsetAtDraw = BackOffset;	m_IsDirty = true;	}

	public:	//�`�揈��

		//����� Draw() �Ŋ��S�ɍĕ`�悳����
		void RequestRefresh(){	m_IsDirty=true;	}

		//Draw()�Ń��C���[�t���[���I�u�W�F�N�g���`�悷�邩�ۂ��D
		//false�ݒ�̏ꍇ�ɂ́C�ǖʂ̕`��݂̂��s���D
		void EnableWireFrameObj( bool Enable ){	m_bDrawWireFrameObj = Enable;	m_IsDirty = true;	}

		/// <summary>
		/// ���H�̕`��
		///		<remarks>
		///		�O�񂩂�ݒ蓙���X�V����Ă��Ȃ��ꍇ�C�P����`�悷��̂ł͂Ȃ���
		///		�L���b�V�����Ă������O��̕`�挋�ʂ��p������D
		///		���ꂾ�Ƃ܂����ꍇ�ɂ́C���O�� RequestRefresh() ���ĂԂ��ƁD
		///		</remarks>
		/// </summary>
		/// <param name="hdc">�`��Ώ�</param>
		/// <param name="MazeData">���H�f�[�^</param>
		/// <returns>���ہD���s���ɂ�hdc�ɑ΂��Ă̕`���Ԃ͕s��D</returns>
		bool Draw( HDC hdc, const FloorData &MazeData );

	private:
		void DrawWireFrameObjects( HDC hdc, const FloorData &MazeData, const Vec2d &CamPos, const Vec2d &FrontDir );

	private:
		//�`��摜�T�C�Y[pixel]
		const int m_ImgW;
		const int m_ImgH;

		//��Draw()�͌��ʂ��L���b�V�����Ă���C�ă����_�����O���s�v�ȏꍇ�ɂ̓L���b�V���摜��BitBlt���邾���D
		HBITMAP m_hBMP = NULL;	//�摜�o�b�t�@
		bool m_IsDirty = true;	//�����_�����O�K�v�t���O

		//�`���HDC
		//��PSEUDO_3D_MAZE::IRenderTgt::DrawVerticalLine() �ŕK�v�Ȃ̂ňꎟ�I�ɋL������K�v������
		HDC m_hRenderTgtDC = NULL;

		//�J����
		Vec2d m_RawCamPos;
		double m_CamHeight;
		double m_CamDir_rad;
		PSEUDO_3D_MAZE::CPinholeCamera m_PinholeCam;
		double m_BackOffsetAtDraw = 0.25;

		//Near��Far
		static inline const double ms_Near = 0.01;
		double m_Far;

		//�F
		double m_Dist2t_Coeff;	//�i�ڍ׏�񂪎c���Ă��Ȃ��DNear->Far �ňÂ��Ȃ��Ă����`��F�����߂鐔�����̕������̒l�j
		double m_BrightnessRate = 1.0;	//�`��F�ɏ悶����W���D

		//Draw() �Ń��C���[�t���[���I�u�W�F�N�g��`�悷�邩�ۂ��̐ݒ�
		bool m_bDrawWireFrameObj = true;
		//�Ǖ`�挋�ʂ̃f�v�X�o�b�t�@(pixel x���W��)
		//�i���C���[�t���[���I�u�W�F�N�g�`�掞�Ɏg�p�j
		std::vector< double > m_DepthBuffer;
	};
}
