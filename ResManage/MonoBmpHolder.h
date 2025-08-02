#pragma once

#include <string>
#include <memory>
#include <vector>
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

	/// <summary>
	/// ���m�N��BMP�Q�̕ێ��ҁD
	/// �e�摜�Ɋւ��ėv�����ꂽ����Ƀ��[�h����D
	/// </summary>
	class MonoBmpHolder
	{
	public:
		/// <summary>�����w���p</summary>
		/// <param name="ImgDirU16">�摜�t�@�C���Q�̂���p�X�iUTF16, �������p�X��؂�L���ŏI����Ă��邱�Ɓj</param>
		/// <param name="ImgFileNamesU16">�摜�t�@�C���Q�̃t�@�C����(UTF16)</param>
		/// <returns></returns>
		static MonoBmpHolder Create(
			const std::wstring &ImgDirU16,
			std::initializer_list<std::wstring> ImgFileNamesU16
		)
		;//{	return MonoBmpHolder( ImgDirU16, ImgFileNamesU16.begin(), ImgFileNamesU16.end() );	}

	public:
		/// <summary>
		/// ctor.�ŏ��ɉ摜�t�@�C������S�Ďw�肵�Ă����K�v������D
		/// </summary>
		/// <typeparam name="FileNameU16Iter">�t�@�C�����w��p�C�e���[�^�̌^</typeparam>
		/// <param name="ImgDirU16">�摜�t�@�C���Q�̂���p�X�iUTF16, �������p�X��؂�L���ŏI����Ă��邱�Ɓj</param>
		/// <param name="begin">[begin,end) �ŉ摜�t�@�C���Q�̃t�@�C����(UTF16)���w��</param>
		/// <param name="end">[begin,end) �ŉ摜�t�@�C���Q�̃t�@�C����(UTF16)���w��</param>
		template<class FileNameU16Iter >
		MonoBmpHolder(
			const std::wstring &ImgDirU16,
			FileNameU16Iter begin, FileNameU16Iter end
 )
			: m_ImgDirU16(ImgDirU16)
			, m_ImgFileNamesU16( begin, end )
		{	m_Bank.resize( m_ImgFileNamesU16.size() );	}

		~MonoBmpHolder();

	public:
		size_t size() const {	return m_Bank.size();	}

		/// <summary>
		/// �摜�̎擾�D����index�l�ɑ΂��ď���̌Ăяo���ł���΂����ŉ摜�Ǎ������{�����D
		/// </summary>
		/// <param name="index">�摜index�Dctor�Ɏw�肵���t�@�C�����̕��тƑΉ��D</param>
		/// <returns>
		/// �摜�ǂݍ��݂Ɏ��s�����ꍇ�ɂ́C��������̑�ւ̃_�~�[�摜���Ԃ����D
		/// </returns>
		const CMonoBMP &operator[]( int index );

	private:
		MonoBmpHolder( const MonoBmpHolder& ) = delete;
		MonoBmpHolder &operator=( const MonoBmpHolder& ) = delete;
	private:
		std::vector< std::unique_ptr<const CMonoBMP> > m_Bank;
		std::vector< std::wstring > m_ImgFileNamesU16;
		std::wstring m_ImgDirU16;
	};
}
