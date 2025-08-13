#pragma once

namespace MazeDataImpl
{
	//���H�G�b�W����
	enum class EdgeAttr
	{
		NONE = 0,	//���������F�ʂ��ꏊ
		WALL = 1,	//�ǁF�ʂ�Ȃ��ꏊ
		DOOR = 2,	//���̒l�ȍ~�̓h�A
	};

	//���H���I�u�W�F�N�g���
	enum class ObjType : unsigned char
	{
		UP_STAIR = 0,	//�o��K�i
		DOWN_STAIR,	//����K�i
		CHAR,	//�l��
		MAGIC_CIRCLE,	//���@�w

		TREASURE_BOX_E,	//��
		TREASURE_BOX_S,
		TREASURE_BOX_W,
		TREASURE_BOX_N
	};

	inline bool IsTreasureBox( ObjType t ){	return ( ObjType::TREASURE_BOX_E<=t && t<=ObjType::TREASURE_BOX_N );	}

	//FloorData ���ň����I�u�W�F�N�g���
	struct ObjInfo
	{
		ObjType Type = ObjType::UP_STAIR;	//�I�u�W�F�N�g���
		unsigned char Brightness = 0;	//�`�掞���邳�i�\���p�j
	};
}
