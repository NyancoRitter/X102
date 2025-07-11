#pragma once

#include <string>
#include <memory>
class CMonoBMP;

namespace ResManage
{
	/// <summary>
	/// ���m�N��BMP��bitmap�t�@�C�����烍�[�h����D
	/// �������t�@�C�����[�h�����s�����ꍇ�ł����炩�̃_�~�[�摜��Ԃ��D
	/// </summary>
	/// <param name="FilePathNameU16">�t�@�C���p�X</param>
	/// <returns></returns>
	std::unique_ptr<CMonoBMP> LoadMonoBMP_or_Dummy( const std::wstring &FilePathNameU16 );
}
