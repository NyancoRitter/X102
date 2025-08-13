#pragma once

#include <string>

//std::string -> T 変換．
//rStrの内容を T 型に変換した結果を rDst に格納する．
//戻り値は成否．
//falseを返した場合，結果受取用引数rDstの内容は保証されない．
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

//↓の再帰している関数テンプレートWords2Vals()の再帰の最後を解決するためのダミー関数実装
template< class ITER >
inline bool Words2Vals( ITER, ITER ){	return true;	}

//[begin, end) で指定された文字列群のうち，
//先頭側 N 個の文字列を指定の型に変換する．
//戻り値は成否．
//｛個数不足，変換処理の失敗の発生，etc｝による失敗時にはfalseを返す．
//falseを返した場合のデータ値受取引数群の内容は保証されない．
template< class ITER, class Head, class...Tail >
bool Words2Vals(
	ITER begin, ITER end,	//文字列群．beginが処理対象先頭を指す．
	Head &head, Tail &... tail	//結果受取用引数群
)
{
	if( begin == end )return false;
	if( !CvtStr2T<Head>( *begin, head ) )return false;	//headに関する処理をして…
	return Words2Vals( std::next(begin), end, tail... );	//残りは再帰処理
}
