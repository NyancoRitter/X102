//[IniLikeFile.cpp]

#include "IniLikeFile.h"
#include <fstream>
#include <list>

namespace IniLikeFile
{
	//"Key = Value"
	//�Ƃ����`��Str���Ȃ��Ă��邩�ǂ����̃`�F�b�N�ƕ���
	bool FileLoader::ParseLine( const std::string &LineStr, SectionData &SecData, int LineNo ) 
	{
		const auto length = LineStr.length();

		//'='�̏ꏊ��������
		const auto EqPos = LineStr.find_first_of( '=' );
		if( EqPos == std::string::npos  ||  EqPos==0  ||  EqPos==length-1 )return false;

		//'='�̍���
		std::string Key = SectionFile::EraseSpaceAtHeadAndTail( LineStr.substr( 0, EqPos ) );
		if( Key.empty() )return false;
		if( Key.find_first_of( '\"' ) != std::string::npos )return false;	//Key�ɂ̓_�u���N�H�[�g���܂߂Ă͂̓_���Ƃ������Ƃɂ���
		//'='�̉E��
		std::string Value = SectionFile::EraseSpaceAtHeadAndTail( LineStr.substr( EqPos+1, length-EqPos-1 ) );
		if( Value.empty() )return false;
		//
		SecData[ Key ] = Value;
		return true;
	}


	//IniData�� Key �Ŏw�肵���v�f�� Val ���J���}�ŕ�������
	//IniData��Key���܂܂�Ă��Ȃ��ꍇ�Cstd::runtime_error �𑗏o�D
	std::vector< std::string > SplitVal(
		const SectionData &SecData,
		const std::string &Key,
		const std::string &delimiters
	)
	{
		const auto iKey = SecData.find( Key );
		if( iKey==SecData.end() )
		{	throw std::runtime_error( u8"Key `" + Key + u8"` not found" );	}

		const auto &rStr = iKey->second;
		std::list< std::string > L;
		std::string::size_type i = 0;
		bool Loop = true;
		while( Loop )
		{
			std::string::size_type delimPos = rStr.find_first_of( delimiters, i );
			if( delimPos == std::string::npos )
			{
				delimPos = rStr.length();
				Loop = false;
			}
			std::string::size_type len = delimPos - i;
			L.emplace_back( rStr.substr(i,len) );
			i = delimPos + 1;
		}
		return std::vector< std::string >( L.begin(), L.end() );
	}

}//end of namespace UCIniFile
