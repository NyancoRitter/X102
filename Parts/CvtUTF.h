#pragma once

#include <string>
#include <Windows.h>

inline
std::wstring Cvt_UTF8_to_UTF16( const std::string &UTF8Str )
{
	int BuffSize = ::MultiByteToWideChar( CP_UTF8, 0, UTF8Str.c_str(), -1, nullptr, 0 );
	wchar_t *pBuff = new wchar_t[BuffSize];
	::MultiByteToWideChar( CP_UTF8, 0, UTF8Str.c_str(), -1, pBuff, BuffSize );
	std::wstring Ret = pBuff;
	delete[] pBuff;
	return Ret;
}

inline
std::string Cvt_UTF16_to_UTF8( const std::wstring &UTF16Str )
{
	int BuffSize = ::WideCharToMultiByte( CP_UTF8, 0, UTF16Str.c_str(), -1, NULL, 0, NULL, NULL );
	char *pBuff = new char[BuffSize];
	::WideCharToMultiByte( CP_UTF8, 0, UTF16Str.c_str(), -1, pBuff, BuffSize, NULL, NULL );
	std::string Ret = pBuff;
	delete[] pBuff;
	return Ret;
}
