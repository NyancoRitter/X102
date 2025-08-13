#pragma once

#include <string>

//std::string -> T �ϊ��D
//rStr�̓��e�� T �^�ɕϊ��������ʂ� rDst �Ɋi�[����D
//�߂�l�͐��ہD
//false��Ԃ����ꍇ�C���ʎ��p����rDst�̓��e�͕ۏ؂���Ȃ��D
template< class T >
bool CvtStr2T( const std::string &rStr, T &rDst ){	rDst=rStr;	return true;	}

template<> bool CvtStr2T<bool>( const std::string &, bool & );
template<> bool CvtStr2T<char>( const std::string &, char & );
template<> bool CvtStr2T<unsigned char>( const std::string &, unsigned char & );
template<> bool CvtStr2T<short>( const std::string &, short & );
template<> bool CvtStr2T<unsigned short>( const std::string &, unsigned short & );
template<> bool CvtStr2T<int>( const std::string &, int & );
template<> bool CvtStr2T<unsigned int>( const std::string &, unsigned int & );
template<> bool CvtStr2T<float>( const std::string &, float & );
template<> bool CvtStr2T<double>( const std::string &, double & );
template<> bool CvtStr2T<unsigned long long>( const std::string &, unsigned long long & );

//---------------------------

//���̍ċA���Ă���֐��e���v���[�gWords2Vals()�̍ċA�̍Ō���������邽�߂̃_�~�[�֐�����
template< class ITER >
inline bool Words2Vals( ITER, ITER ){	return true;	}

//[begin, end) �Ŏw�肳�ꂽ������Q�̂����C
//�擪�� N �̕�������w��̌^�ɕϊ�����D
//�߂�l�͐��ہD
//�o���s���C�ϊ������̎��s�̔����Cetc�p�ɂ�鎸�s���ɂ�false��Ԃ��D
//false��Ԃ����ꍇ�̃f�[�^�l�������Q�̓��e�͕ۏ؂���Ȃ��D
template< class ITER, class Head, class...Tail >
bool Words2Vals(
	ITER begin, ITER end,	//������Q�Dbegin�������Ώې擪���w���D
	Head &head, Tail &... tail	//���ʎ��p�����Q
)
{
	if( begin == end )return false;
	if( !CvtStr2T<Head>( *begin, head ) )return false;	//head�Ɋւ��鏈�������āc
	return Words2Vals( std::next(begin), end, tail... );	//�c��͍ċA����
}
