#include "DataPath.h"
#include "Parts/CvtUTF.h"

namespace DataPath
{

	const std::wstring &ExeDirU16()
	{
		static std::wstring ExeDirPath;
		
		if( ExeDirPath.empty() )
		{
			wchar_t Buff[MAX_PATH] = { 0 };
			::GetModuleFileNameW( NULL, Buff, MAX_PATH );

			std::wstring Path( Buff );
			auto i = Path.find_last_of( L"\\" );
			ExeDirPath = Path.substr( 0, i ) + L"/";

	#ifdef _DEBUG
			//※開発中対応：DebugフォルダではなくReleaseフォルダを示すように
			ExeDirPath += L"../Release/";
	#endif
		}
		return ExeDirPath;
	}

	const std::string &ExeDirU8()
	{
		static std::string ExeDirPath;

		if( ExeDirPath.empty() )
		{	ExeDirPath = Cvt_UTF16_to_UTF8( ExeDirU16() );	}

		return ExeDirPath;

	}

}
