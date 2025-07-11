//[MCIThread.cpp]

#include "framework.h"	//VC
#include "MCIThread.h"

#include <mmsystem.h>
#include <thread>
#include <condition_variable>
#include <map>

#pragma comment( lib, "winmm.lib" )

///////////////////////////////////////////////////////////
//	CMCIThread::Impl
///////////////////////////////////////////////////////////

class CMCIThread::Impl
{
private:
	enum REQUEST
	{
		NO_REQUEST = 0,
		REQUEST_END_THREAD,
		REQUEST_CHANGE_BGM,
		REQUEST_STOP_BGM,
		REQUEST_RESTART_BGM
	};

public:
	Impl()
		: m_CurrRequestState(NO_REQUEST), m_MCI_ERR_MSG(WM_NULL), m_LastMCIErr(0)
	{	m_iCurrData = m_iChangeTgtData = m_Data.end();	}

	~Impl(){	EndThread();	}
private:
	Impl( const Impl & );
	Impl &operator=( const Impl & );

public:
	bool BeginThread(  HWND hNotifyMsgTgtWnd, UINT ERR_MSG_VAL );
	void EndThread();
	bool IsThreadRunning() const {	return m_Thread.joinable();	}

	//
	bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong );

	//
	bool StopBGM(){	return Request( REQUEST_STOP_BGM );	}
	bool RestartBGM(){	return Request( REQUEST_RESTART_BGM );	}
	bool ChangeBGM( int specifier );

	//
	std::wstring GetLastMCIErrMsg();

private:
	void ThreadFunc( HWND hNotifyMsgTgtWnd );
	bool Request( REQUEST req );
	bool SendMCICommand( MCIDEVICEID, UINT, DWORD_PTR, DWORD_PTR, HWND );

private:
	std::thread m_Thread;

	REQUEST m_CurrRequestState;
	std::mutex m_RequestMutex;
	std::condition_variable m_RequestCondVar;

	std::mutex m_DataMutex;

	UINT m_MCI_ERR_MSG;
	std::mutex m_MCIErrMutex;
	MCIERROR m_LastMCIErr;

	struct BGMData
	{
		BGMData( const std::wstring &FilePath=L"", bool ShouldKeepLong=true )
			: FilePath{FilePath}, m_ShouldKeepLong(ShouldKeepLong), DevID{0} {}
		std::wstring FilePath;
		bool m_ShouldKeepLong;
		MCIDEVICEID DevID;
	};
	std::map< int, BGMData > m_Data;
	std::map< int, BGMData >::iterator m_iCurrData;
	std::map< int, BGMData >::iterator m_iChangeTgtData;
};

//---

std::wstring CMCIThread::Impl::GetLastMCIErrMsg()
{
	MCIERROR err = 0;
	{
		std::lock_guard< std::mutex > lock( m_MCIErrMutex );
		err = m_LastMCIErr;
	}

	WCHAR ErrMsg[ 129 ] = { 0 };
	::mciGetErrorStringW( err, ErrMsg, 129 );
	return std::wstring( ErrMsg );
}

//ワーカスレッド内で使う作業関数
bool CMCIThread::Impl::SendMCICommand( MCIDEVICEID MCIDevID, UINT Command, DWORD_PTR Flags, DWORD_PTR Params, HWND hNotifyMsgTgtWnd )
{
	MCIERROR ret = ::mciSendCommand( MCIDevID, Command, Flags, Params );
	if( ret==0 )
	{	return true;	}
	else
	{	//MCI失敗時の処理
		{
			std::lock_guard<std::mutex> lock( m_MCIErrMutex );
			m_LastMCIErr = ret;
		}
		if( hNotifyMsgTgtWnd )
		{	::PostMessage( hNotifyMsgTgtWnd, m_MCI_ERR_MSG, Command, 0 );	}
		return false;
	}
}

//ワーカスレッド処理
void CMCIThread::Impl::ThreadFunc( HWND hNotifyMsgTgtWnd )
{
	bool bLoop = true;
	while( bLoop )
	{
		REQUEST RecvRequest = NO_REQUEST;
		{//処理リクエスト待ち
			std::unique_lock<std::mutex> uniq_lock( m_RequestMutex );
			m_RequestCondVar.wait( uniq_lock, [this]()->bool{	return ( this->m_CurrRequestState!=NO_REQUEST );	} );

			RecvRequest = m_CurrRequestState;
			m_CurrRequestState = NO_REQUEST;
		}

		//リクエストに対応する処理
		switch( RecvRequest )
		{
		case REQUEST_END_THREAD:
			{	bLoop = false;	}
			break;

		case REQUEST_CHANGE_BGM:
			{
				std::lock_guard<std::mutex> lock( m_DataMutex );

				if( m_iChangeTgtData->second.DevID == 0 )
				{//open
					MCI_OPEN_PARMS open_params;
					open_params.dwCallback = (DWORD_PTR)hNotifyMsgTgtWnd;
					open_params.wDeviceID = 0;
					open_params.lpstrDeviceType = NULL;
					open_params.lpstrElementName = m_iChangeTgtData->second.FilePath.c_str();
					open_params.lpstrAlias = NULL;

					if( SendMCICommand( 0, MCI_OPEN, MCI_WAIT|MCI_OPEN_ELEMENT, (DWORD_PTR)&open_params, hNotifyMsgTgtWnd ) )
					{	
						m_iChangeTgtData->second.DevID = open_params.wDeviceID;
					}
				}
				if( auto NewDevID = m_iChangeTgtData->second.DevID;	NewDevID!=0 )
				{
					//Stop Current
					if( m_iCurrData != m_Data.end() )
					{
						if( auto CurrDevID = m_iCurrData->second.DevID;	CurrDevID!=0 )
						{	SendMCICommand( CurrDevID, MCI_STOP, MCI_WAIT,0, hNotifyMsgTgtWnd );	}
					}
					//Play
					MCI_PLAY_PARMS params;
					params.dwCallback = (DWORD_PTR)hNotifyMsgTgtWnd;
					SendMCICommand( NewDevID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&params, hNotifyMsgTgtWnd );
					
					//停止したBMGへの処理
					if( m_iCurrData != m_Data.end() )
					{
						if( auto CurrDevID = m_iCurrData->second.DevID;	CurrDevID!=0 )
						{
							if( m_iCurrData->second.m_ShouldKeepLong )
							{//次回のために再生位置を先頭に戻しておく
								SendMCICommand( CurrDevID, MCI_SEEK, MCI_SEEK_TO_START, 0, hNotifyMsgTgtWnd );
							}
							else
							{//close
								SendMCICommand( CurrDevID, MCI_CLOSE, MCI_WAIT,0, hNotifyMsgTgtWnd );
								m_iCurrData->second.DevID = 0;
							}
						}
					}
					//
					m_iCurrData = m_iChangeTgtData;
				}
			}
			break;

		case REQUEST_STOP_BGM:
			if( m_iCurrData != m_Data.end() )
			{
				if( auto CurrDevID = m_iCurrData->second.DevID;	CurrDevID!=0 )
				{
					SendMCICommand( CurrDevID, MCI_STOP, MCI_WAIT,0, hNotifyMsgTgtWnd );
				}
			}
			break;

		case REQUEST_RESTART_BGM:
			if( m_iCurrData != m_Data.end() )
			{
				if( auto CurrDevID = m_iCurrData->second.DevID;	CurrDevID!=0 )
				{
					if( SendMCICommand( CurrDevID, MCI_SEEK, MCI_SEEK_TO_START, 0, hNotifyMsgTgtWnd ) )
					{
						MCI_PLAY_PARMS params;
						params.dwCallback = (DWORD_PTR)hNotifyMsgTgtWnd;
						SendMCICommand( CurrDevID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&params, hNotifyMsgTgtWnd );
					}
				}
			}
			break;

		default:
			break;
		}
	}

	{//MCI後始末
		std::lock_guard<std::mutex> lock( m_DataMutex );
		if( m_iCurrData != m_Data.end() )
		{
			if( auto CurrDevID = m_iCurrData->second.DevID;	CurrDevID!=0 )
			{	SendMCICommand( CurrDevID, MCI_STOP, MCI_WAIT,0, hNotifyMsgTgtWnd );	}

			m_iCurrData = m_Data.end();
		}

		for( auto &d : m_Data )
		{
			if( auto DevID = d.second.DevID;	DevID!=0 )
			{
				SendMCICommand( DevID, MCI_CLOSE, MCI_WAIT,0, hNotifyMsgTgtWnd );
				d.second.DevID = 0;
			}
		}
	}
}

//
bool CMCIThread::Impl::BeginThread( HWND hNotifyMsgTgtWnd, UINT ERR_MSG_VAL )
{
	if( IsThreadRunning() )return false;

	m_CurrRequestState = NO_REQUEST;
	m_MCI_ERR_MSG = ERR_MSG_VAL;
	m_Thread = std::thread( [this, hNotifyMsgTgtWnd]()->void{	this->ThreadFunc( hNotifyMsgTgtWnd );	} );
	return IsThreadRunning();
}

//
bool CMCIThread::Impl::Request( REQUEST req )
{
	if( !IsThreadRunning() )return false;
	{
		std::lock_guard<std::mutex> lock( m_RequestMutex );
		m_CurrRequestState = req;
	}
	m_RequestCondVar.notify_all();
	return true;
}

//
bool CMCIThread::Impl::RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong )
{
	{
		std::lock_guard<std::mutex> lock( m_DataMutex );
		
		auto i = m_Data.find( specifier );
		if( i!=m_Data.end() )return false;

		m_Data[specifier] = BGMData(rBGMFilePath, ShouldKeepLong);
	}
	return true;
}

//
bool CMCIThread::Impl::ChangeBGM( int specifier )
{
	{
		std::lock_guard<std::mutex> lock( m_DataMutex );

		auto i = m_Data.find( specifier );
		if( i==m_Data.end() )return false;
		if( i==m_iCurrData )return true;
		
		m_iChangeTgtData = i;
	}
	return Request( REQUEST_CHANGE_BGM );
}

//
void CMCIThread::Impl::EndThread()
{	
	if( Request( REQUEST_END_THREAD ) )
	{	m_Thread.join();	}
}


///////////////////////////////////////////////////////////
//	CMCIThread::Impl
///////////////////////////////////////////////////////////

CMCIThread::CMCIThread()
{	m_pImpl = new Impl;	}

CMCIThread::~CMCIThread()
{	delete m_pImpl;	}

bool CMCIThread::BeginThread( HWND hNotifyMsgTgtWnd, UINT ERR_MSG_VAL )
{	return m_pImpl->BeginThread( hNotifyMsgTgtWnd, ERR_MSG_VAL );	}

void CMCIThread::EndThread(){	m_pImpl->EndThread();	}
bool CMCIThread::IsThreadRunning() const {	return m_pImpl->IsThreadRunning();	}

bool CMCIThread::RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong )
{	return m_pImpl->RegisterBMG( specifier, rBGMFilePath, ShouldKeepLong );	}

bool CMCIThread::ChangeBGM( int specifier ){	return m_pImpl->ChangeBGM( specifier );	}
bool CMCIThread::StopBGM(){	return m_pImpl->StopBGM();	}
bool CMCIThread::RestartBGM(){	return m_pImpl->RestartBGM();	}
std::wstring CMCIThread::GetLastMCIErrMsg(){	return m_pImpl->GetLastMCIErrMsg();	}
