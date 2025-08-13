#pragma once
#include <map>
#include <fstream>
#include <string>
#include <filesystem>

//INI���ۂ��Z�N�V������؂�t�@�C���̓ǂݍ��݁D
//
// * �e�s�̐擪����і����̃X�y�[�X�ƃ^�u�͏��������D
//  �i�ȉ��̋L�q�͏�����̓��e�Ɋւ���j
// * �Z�N�V������
//  [SectionName]
//  �Ƃ����`�ŃZ�N�V��������[]�ň͂񂾋L�q�݂̂̍s�ŊJ�n�D
//		* �t�@�C�����ōŏ��̃Z�N�V�����J�n�s�����O�ɏ����ꂽ�L�q�́C
//		  �Z�N�V���������󕶎���ȃZ�N�V�������̋L�q�ƌ��Ȃ����D
// * ��s�͖��������D
// * �R�����g�s�����������
//
namespace SectionFile
{

	//������̑O��̕s�v�ȃX�y�[�X�������������Ɗ֐�
	inline
	std::string EraseSpaceAtHeadAndTail( const std::string &rSrc, const char EraseChars[] = " \t\n\r" )
	{
		const auto spos = rSrc.find_first_not_of( EraseChars );
		if( spos==std::string::npos ){	return "";	}
		const auto epos = rSrc.find_last_not_of( EraseChars );
		return rSrc.substr( spos, epos-spos+1 );
	}


	//�t�@�C�����[�_
	//	SectionData_T :
	//		�e�Z�N�V�����̓Ǎ����ʃf�[�^�̌^�D
	//		�f�t�H���g�R���X�g���N�g�� empty() �ɂ��󔻒肪�K�v�D
	template< class SectionData_T >
	class FileLoader
	{
	public:
		//�t�@�C�����e�i�[�p�f�[�^�\��
		//map< SectionName, �Z�N�V�������̍s�̃f�[�^ >
		using FileData = std::map< std::string, SectionData_T >;

	public:
		FileLoader() : m_LastLineNum(0) {}
		virtual ~FileLoader() = default;
		FileLoader( const FileLoader & ) = delete;
		FileLoader &operator=( const FileLoader & ) = delete;

	public:
		//Load()�ōŌ�ɓǂ񂾍s�̏��D�G���[���ɎQ�Ƃ���p�D
		//GetLastLineNum()�����̒l��Ԃ��ꍇ�́C�t�@�C���I�[�v���G���[�D
		int GetLastLineNum() const {	return m_LastLineNum;	}
		std::string GetLastLineStr() const {	return m_LastLineStr;	}

	protected:
		//Load() ������Ă΂��R�����g�s�̔���p�����D
		//true��Ԃ����ꍇ�C���̍s�͒P�ɖ��������D
		//�����́C�擪�Ɩ����̋󔒕����������ς݂̋�łȂ��s������D
		virtual bool IsCommentLine( const std::string &LineStr ) const
		{	//�f�t�H���g�����ł� "//" ����n�܂�s���R�����g�s�Ƃ���
			//if( LineStr[0]==';' || LineStr[0]=='#' )return  true;
			if( LineStr.size()>=2  &&  LineStr[0]=='/'  &&  LineStr[1]=='/' )return true;
			return false;
		}

		//Load() ������Ă΂��s�̃p�[�X�����D
		//�����́C�擪�Ɩ����̋󔒕����������ς݂̋�łȂ��s������D
		//�������͏������ʂ� SecData �� ���f/�i�[ ���� true ��Ԃ��D
		//false ��Ԃ����ꍇ�CLoad() �̓G���[�Ƃ��ďI������D
		virtual bool ParseLine( const std::string &LineStr, SectionData_T &SecData, int LineNo ) = 0;

	public:
		/// <summary>
		/// �t�@�C���Ǎ�
		/// * �������ɂ� rDstData �ɓǍ����ʂ��i�[���� true ��Ԃ��D
		/// * false ��Ԃ��ꍇ�C rDstData �̓��e�͕ۏ؂���Ȃ��D
		/// </summary>
		/// <param name="rFilePathNameU8">�t�@�C���p�X (UTF8)</param>
		/// <param name="rDstData">���ʎ󂯎��</param>
		/// <returns>����</returns>
		bool Load( const std::string &rFilePathNameU8, FileData &rDstData )
		{
			m_LastLineStr.clear();
			std::ifstream File( std::filesystem::u8path(rFilePathNameU8) );
			if( !File.is_open() ){	m_LastLineNum = -1;	return false;	}

			SectionData_T NoSecParams;	//�Z�N�V�������̖���(�����Ȃ菑����Ă���)�s�̈ꎞ�i�[�p
			SectionData_T *pCurrSection = &NoSecParams;
			m_LastLineNum = 0;
			rDstData.clear();

			std::string LineStr;
			std::string Key,Value;
			while( std::getline( File, m_LastLineStr ) )
			{
				++m_LastLineNum;

				//�O��̕s�v�ȃX�y�[�X��������
				LineStr = EraseSpaceAtHeadAndTail( m_LastLineStr );
				if( LineStr.empty() )continue;	//��s�͖���
				if( IsCommentLine( LineStr ) )continue;	//�R�����g�s����

				//�Z�N�V��������
				if( LineStr[0]=='[' && (*LineStr.rbegin())==']' )
				{
					std::string SectionName = EraseSpaceAtHeadAndTail( LineStr.substr( 1, LineStr.length()-2 ) );
					if( SectionName.empty() )return false;
					pCurrSection = &rDstData[ SectionName ];
					continue;
				}

				//�s���e���p�[�X
				if( !ParseLine( LineStr, *pCurrSection, m_LastLineNum ) )
				{	return false;	}
			}
			File.close();

			//�Z�N�V�������̖����s�̃f�[�^������Ό��ʂɒǉ�
			if( !NoSecParams.empty() )
			{
				rDstData.insert( std::pair< std::string, SectionData_T >( "", std::move(NoSecParams) ) );
			}

			return File.eof();
		}

	private:
		std::string m_LastLineStr;
		int m_LastLineNum;
	};

}