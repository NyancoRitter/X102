#pragma once

#include <vector>
#include "FloorData.h"

namespace MazeDataImpl
{
	/// <summary>
	/// ���H�̃f�[�^���t�@�C������Ǎ��ށD
	/// 
	/// �G���[���ɂ� UTF-8 ������ŃG���[���b�Z�[�W������ std::runtime_error �𑗏o�D
	/// �Ȃ��C���̍ۂɂ͌��ʎ��p�����̓��e�͕ۏ؂���Ȃ��D
	/// </summary>
	/// <param name="MazeFilePathNameU8">�t�@�C���p�X(UTF8)</param>
	/// <param name="StartPos">�X�^�[�g�n�_���W���</param>
	/// <param name="StartDir">�X�^�[�g���̌������</param>
	/// <returns>
	/// �Ǎ��񂾖��H�}�b�v���D
	/// �L�q����Ă����K�w���D
	/// </returns>
	std::vector< FloorData > LoadMazeMapFile( const std::string &MazeFilePathNameU8, Vec2i &StartPos, Direction &StartDir );
}
