#include "CvtStr2T.h"

namespace
{
	inline bool StrToLong( const std::string &str, long &rDst )
	{
		char *endptr;
		rDst = std::strtol( str.c_str(), &endptr, 0 );
		return ( *endptr == '\0' );
	}

	inline bool StrToULong( const std::string &str, unsigned long &rDst )
	{
		char *endptr;
		rDst = std::strtoul( str.c_str(), &endptr, 0 );
		return ( *endptr == '\0' );
	}

	inline bool StrToULL( const std::string &str, unsigned long long &rDst )
	{
		char *endptr;
		rDst = std::strtoull( str.c_str(), &endptr, 0 );
		return ( *endptr == '\0' );
	}

	inline bool StrToFloat( const std::string &str, float &rDst )
	{
		char *endptr;
		rDst = std::strtof( str.c_str(), &endptr );
		return ( *endptr == '\0' );
	}

	inline bool StrToDouble( const std::string &str, double &rDst )
	{
		char *endptr;
		rDst = std::strtod( str.c_str(), &endptr );
		return ( *endptr == '\0' );
	}

	//
	template< class T >	//T must be one of { char, short, int, long }
	bool CvtStrToSigned( const std::string &rStr, T &rDst )
	{
		long i;
		if( !StrToLong( rStr, i ) )return false;
		rDst = (T)i;
		return true;
	}

	template< class T >	//T must be one of unsigned{ char, short, int, long }, or bool
	bool CvtStrToUnsigned( const std::string &rStr, T &rDst )
	{
		unsigned long i;
		if( !StrToULong( rStr, i ) )return false;
		rDst = (T)i;
		return true;
	}
}

template<> bool CvtStr2T<bool>( const std::string &S, bool &D )
{
	if( S=="true" || S=="TRUE" ){	D=true;	return true;	}
	if( S=="false" || S=="FALSE" ){	D=false;	return true;	}
	return CvtStrToUnsigned( S, D );
}

template<> bool CvtStr2T<char>( const std::string &S, char &D ){	return CvtStrToSigned( S,D );	}
template<> bool CvtStr2T<short>( const std::string &S, short &D ){	return CvtStrToSigned( S,D );	}
template<> bool CvtStr2T<int>( const std::string &S, int &D ){	return CvtStrToSigned( S,D );	}

template<> bool CvtStr2T<unsigned char>( const std::string &S, unsigned char &D ){	return CvtStrToUnsigned( S,D );	}
template<> bool CvtStr2T<unsigned short>( const std::string &S, unsigned short &D ){	return CvtStrToUnsigned( S,D );	}
template<> bool CvtStr2T<unsigned int>( const std::string &S, unsigned int &D ){	return CvtStrToUnsigned( S,D );	}
template<> bool CvtStr2T<unsigned long long>( const std::string &S, unsigned long long &D ){	return StrToULL( S,D );	}

template<> bool CvtStr2T<float>( const std::string &S, float &D ){	return StrToFloat( S,D );	}
template<> bool CvtStr2T<double>( const std::string &S, double &D ){	return StrToDouble( S,D );	}
