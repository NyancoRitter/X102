//[IMazeData.h]

#pragma once

namespace PSEUDO_3D_MAZE
{
	//���H�̃����_�����O�ɕK�v�ȏ��̎擾��i�F
	//
	//���H�͕��ʂɂQ�����̃}�X�ڂō\������Ă���Ƃ��āC
	//�אڂ���}�X�̊�(���̕��������ƌĂׂ΂����̂��킩��񂪁C�ȉ��ł͕֋X��u�G�b�W�v�ƌď̂���)
	//�ɕǓ������邩�ǂ����Ƃ��������C�^��3D�`��҂ɕԂ��D
	//
	//���\�b�h���Ԃ������l�� 1�ȏ�̒l�ł���ꍇ�C�ǂ��邢�͂���ɏ�������̂ł���
	// �i�F�����ɉ��炩�̕Ǐ�̕��̃����_�����O��v����j���Ƃ��Ӗ�����D
	class IMazeData
	{
	public:
		virtual ~IMazeData() = default;

	public:
		//�}�X(x,y)�̖k���G�b�W�̑����₢���킹
		//	�u�}�X(x,y) �̊O���i�k���j���猩���Ƃ��ɁC�G�b�W�����Ɍ����邩�v�ł���D
		//	�u�}�X(x,y) �̓������猩���Ƃ��v�ł͂Ȃ��D
		//	�i���̓��ނ̃��\�b�h�����l�D�j
		virtual int NorthSideEdgeAttr( int x, int y ) const = 0;

		//�}�X(x,y)�̓쑤�G�b�W�̑����₢���킹
		virtual int SouthSideEdgeAttr( int x, int y ) const = 0;

		//�}�X(x,y)�̐����G�b�W�̑����₢���킹
		virtual int WestSideEdgeAttr( int x, int y ) const = 0;

		//�}�X(x,y)�̓����G�b�W�̑����₢���킹
		virtual int EastSideEdgeAttr( int x, int y ) const = 0;
	};

	//�G�b�W�����l���ǂ��邢�͂���ɏ�������̂��ۂ�
	inline bool IsWallLikeFace( int Attr ){	return ( Attr >= 1 );	}
}

//[EOF]