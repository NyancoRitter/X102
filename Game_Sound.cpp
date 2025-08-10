#include "framework.h"
#include "Game.h"

#include "Toyger/Toyger_IWnd.h"
#include "Toyger/Toyger_IBMGPlayer.h"

#include "DataPath.h"
#include "Parts/IniLikeFile.h"
#include "Parts/CvtUTF.h"

#include "ResManage/SoundBank.h"

bool Game::ChangeBGM( int BGM_index )
{
	if( auto *player = m_rWnd.BGMPlayer(); player )
	{	return player->ChangeBGM( BGM_index );	}

	return false;
}

void Game::StopBMG()
{
	if( auto *player = m_rWnd.BGMPlayer(); player )
	{	player->StopBGM();	}
}

//音関係セットアップ
void Game::SetupSound()
{
	IniLikeFile::FileData FileData;
	{
		IniLikeFile::FileLoader Loader;
		if( !Loader.Load( DataPath::SoundEnumerationFilePathU8(), FileData ) )
		{
			int LineNo = Loader.GetLastLineNum();
			std::wstring ErrMsg =
				LineNo < 0 ?
				L"File Open Err(" + Cvt_UTF8_to_UTF16( DataPath::SoundEnumerationFilePathU8() ) + L")" :
				L"Err at line " + std::to_wstring(LineNo);

			m_rWnd.MsgBox( ErrMsg.c_str(), L"BGM Setup Err", MB_OK );
			return;
		}
	}

	const std::wstring SoundFileDir = DataPath::SoundDirU16();
	{//[BGM]
		const auto iSection = FileData.find( "BGM" );
		if( iSection == FileData.end() )
		{
			m_rWnd.MsgBox( L"No BGM Section", L"BGB Setup Err", MB_OK );
		}
		else if( auto *pBGMPlayer = m_rWnd.BGMPlayer(); pBGMPlayer )
		{
			const auto &SectionData = iSection->second;
			int index = 0;
			while( true )
			{
				auto iData = SectionData.find( "BGM" + std::to_string(index) );
				if( iData == SectionData.end() )break;

				pBGMPlayer->RegisterBMG(
					index,
					SoundFileDir + Cvt_UTF8_to_UTF16(iData->second),
					(index!=(int)BGMSpecifier::Title) && (index!=(int)BGMSpecifier::GameClear)	//※現在，タイトルやゲームクリアシーンには二度と行かない想定
				);

				++index;
			}
		}
		else
		{
			m_rWnd.MsgBox( L"!pBGMPlayer", L"BGB Setup Err", MB_OK );
		}
	}

	{//[SE]
		const auto iSection = FileData.find( "SE" );
		if( iSection == FileData.end() )
		{
			m_rWnd.MsgBox( L"No SE Section", L"SE Setup Err", MB_OK );
		}
		else
		{
			std::array< std::wstring, ResManage::N_SE > FileNames;
			const auto &SectionData = iSection->second;
			for( int index=0; index<ResManage::N_SE; ++index )
			{
				auto iData = SectionData.find( "SE" + std::to_string(index) );
				if( iData == SectionData.end() )continue;

				FileNames[index] = Cvt_UTF8_to_UTF16(iData->second);
			}
			ResManage::SetupSE( FileNames );
		}
	}
}
