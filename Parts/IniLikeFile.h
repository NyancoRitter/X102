//[IniLikeFile.h]

#pragma once

#include "SectionFileRead.h"
#include <vector>

//INI���ۂ��Z�N�V������؂�t�@�C���̓ǂݍ��݁D
//
// * �Z�N�V��������
//	Key = Value
//  �Ƃ��� '=' ��p�����L�q�D
//

namespace IniLikeFile
{
	//�Z�N�V�����̓ǂݍ��݌��ʃf�[�^
	//  map< Key, Value >
	using SectionData = std::map< std::string, std::string >;
	//�t�@�C���ǂݍ��݌��ʃf�[�^
	//  map< Section��, SectionData >
	using FileData = std::map< std::string, SectionData >;

	//�t�@�C�����[�_
	class FileLoader : public SectionFile::FileLoader<SectionData>
	{
	protected:
		virtual bool ParseLine( const std::string &LineStr, SectionData &SecData, int LineNo ) override;
	};

	/// <summary>
	/// �w���p�֐��D
	/// �Z�N�V�����f�[�^ SecData �� Key �Ŏw�肵���v�f�� Val ���J���}�ŕ�������
	/// SecData ��Key���܂܂�Ă��Ȃ��ꍇ�Cstd::runtime_error �𑗏o�D
	/// </summary>
	/// <param name="SecData">�Z�N�V�����f�[�^</param>
	/// <param name="Key">�L�[</param>
	/// <param name="delimiters">�f���~�^</param>
	/// <returns>
	/// �������ʁD
	/// �P���Ƀf���~�^�Ő؂蕪�������ʁi�F�X�̗v�f�̑O��̃X�y�[�X�������͂Ȃ���Ȃ��j
	/// </returns>
	std::vector< std::string > SplitVal(
		const SectionData &SecData,
		const std::string &Key,
		const std::string &delimiters=","
	);
}
