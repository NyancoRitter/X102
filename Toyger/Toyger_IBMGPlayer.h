#pragma once
#include "framework.h"
#include <string>

namespace Toyger
{
	//------------------------------------

	/// <summary>BGM���t��i</summary>
	class IBGMPlayer
	{
	public:
		~IBGMPlayer() = default;

		/// <summary>
		/// BGM�t�@�C����o�^����
		/// </summary>
		/// <param name="specifier">�o�^�ԍ��DChangeBMG()�ł̈����Ɏg�p����</param>
		/// <param name="rBGMFilePath">BGM�t�@�C���p�X</param>
		/// <param name="ShouldKeepLong">
		/// ����BGM�t�@�C����ǂݍ��񂾓��e�𒷊��ێ����ׂ����ۂ��D
		/// false�̏ꍇ�C����BGM�̍Đ����n�߂��ۂɔj�������D
		/// </param>
		/// <returns>
		/// ���ہD�w��̔ԍ������ɓo�^����Ă���ꍇ�ɂ͎��s�ƂȂ�D
		/// </returns>
		virtual bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong ) = 0;

		//�Đ�BGM�̕ύX
		virtual bool ChangeBGM( int specifier ) = 0;
		//���t���~�߂�
		virtual bool StopBGM() = 0;
	};
}