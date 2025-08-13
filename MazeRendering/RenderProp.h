//[RenderProp.h]

#pragma once
#include "PinholeCamera.h"
#include "Parts/Vec2D.h"

namespace PSEUDO_3D_MAZE
{
	//���H�����_�����O�ɕK�v�ȃp�����^�̎擾��i
	class RenderProp
	{
	public:
		virtual ~RenderProp(){}
	public:
		//�J�������f��
		virtual const CPinholeCamera &CamModel() const = 0;

		//Near��Far : �����_�����O�ŏ��C�ő剜�s
		virtual double NearDepth() const = 0;
		virtual double FarDepth() const = 0;

		//�J�����ʒu�ƌ���
		//	��Ԃ̍��W�n�ɂ��āF
		//	���_�F�}�X(0,0)�̖k���[
		//	���W�̌����F���ɂ����ق�X�������C��ɂ����ق�Y�������D
		//	���W�̎ړx�F1�}�X�̕ӂ̒�����1.0�Ƃ���D
		//	���Ȃ킿�C�}�X(x,y)�̒��S�ʒu = (x+0.5, y+0.5) �ƂȂ�D
		virtual Vec2d CamPos() const = 0;
		//�J�����ʒu�̍����D�`��A���S���Y���̎d�g�ݓI�ɁC�܂Ƃ��ȕ`�悪�\�Ȕ͈͂́C0.0<=����<=1.0 �ł���D
		virtual double CamHeight() const = 0;
		//�J�����̌����i�P�ʃx�N�g���j
		virtual Vec2d CamFrontDir() const = 0;
	};
}

//[EOF]