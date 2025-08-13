#pragma once
#include "Parts/Vec3D.h"
#include <vector>
#include "Direction.h"

namespace MazeDataImpl
{
	//���C���[�t���[���I�u�W�F�N�g�f�[�^
	//
	//[���W]
	//X,Y : ���H�̃����_�����O�������g���Ă���̂Ɠ��������D���Ȃ킿�C
	//	���_�F(�I�u�W�F�N�g���z�u�����)�}�X�̖k���[
	//	���W�̌����F���ɂ����ق�X�������C��ɂ����ق�Y�������D
	//	���W�̎ړx�F1�}�X�̕ӂ̒�����1.0�Ƃ���D
	//Z : �n�ʂ���̍����D
	//	0.0�����C1.0���V��̍����D
	struct WireFrameObj
	{
	public:
		//���_�Q���W
		std::vector< Vec3d > P;
		//�����Q�D�P�����𒸓_index��pair�ŕ\��
		std::vector< std::pair<int,int> > L;

	public:
		//����͂���
		static WireFrameObj CreateUpStair();
		//�~��͂���
		static WireFrameObj CreateDownStair();
		//�l��
		static WireFrameObj CreateCharacter();
		//���@�w
		static WireFrameObj CreateMagicCircle();
		//��
		//	Dir : �ʒu���}�X���łǂ��瑤�Ɋ񂹂邩�D
		static WireFrameObj CreateTreasureBox( Direction Dir );
	};
}
