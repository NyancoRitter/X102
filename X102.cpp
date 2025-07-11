// X102.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "X102.h"
#include "Toyger/Toyger_GameWnd.h"
#include "Toyger/Toyger_InputImpl.h"
#include <chrono>	//時間計測用
#include <cmath>
#include <algorithm>

#include "Game.h"

namespace
{
	//エラー時ヘルパ．メッセージボックスを表示し，値1を返す．
	inline int Err( LPCTSTR Msg, LPCTSTR Caption=L"ERR" )
	{	::MessageBox( NULL, Msg, Caption, MB_OK );	return 1;	}

	//StartからEndまでの時間をmsで計測
	class CTimeMeasure
	{
	public:
		CTimeMeasure(){	Start();	}
		void Start(){	m_ST = std::chrono::system_clock::now();	}
		double End()
		{
			auto Curr = std::chrono::system_clock::now();
			return std::chrono::duration< double, std::milli >( Curr - m_ST ).count();
		}
	private:
		std::chrono::system_clock::time_point m_ST;
	};

	/// <summary>
	/// メインループ処理
	/// </summary>
	/// <returns>エラー時は非0</returns>
	int MainLoop()
	{
		//ウィンドウ生成
		Toyger::ConcreteInputState< uint8_t > InputSt{ false };	//※false : ウィンドウを閉じる操作に対してはゲーム処理側で対処する
		Toyger::GameWnd Wnd{ InputSt };
		if( !Wnd.CreateWnd( L"Toyger", true ) ){	return Err( L"Wnd.CrateWnd() failed" );	}

		//{//Set Window Icon
		//	HANDLE hBigIcon = ::LoadImage( hInstance, MAKEINTRESOURCE(IDI_ICON32), IMAGE_ICON, 32,32, LR_DEFAULTCOLOR );
		//	Wnd.ChangeIcon( hBigIcon, false );
		//	HANDLE hSmallIcon = ::LoadImage( hInstance, MAKEINTRESOURCE(IDI_ICON16), IMAGE_ICON, 16,16, LR_DEFAULTCOLOR );
		//	Wnd.ChangeIcon( hSmallIcon, true );
		//}

		//ゲーム処理用準備
		Game TheGame(Wnd);
		if( !TheGame.Initialize() ){	return Err( L"TheGame.Initialize() failed" );	}

		//メインループ
		constexpr double MaxWait_ms = 33;
		double Wait_ms = 1.0;
		CTimeMeasure CTM;
		bool ShouldQuit = false;
		do
		{
			CTM.Start();
			if( ::MsgWaitForMultipleObjects( 0, NULL, FALSE, (DWORD)std::round(Wait_ms), QS_ALLINPUT ) == WAIT_FAILED )
			{	return Err( L"MsgWaitForMultipleObjects() returns WAIT_FAILED" );	}

			MSG msg;
			while( ::PeekMessageW( &msg, NULL, 0,0, PM_REMOVE ) != 0 )
			{
				if( msg.message == WM_QUIT )
				{
					ShouldQuit = true;
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			Wait_ms -= CTM.End();
			if( Wait_ms <= 0 )
			{
				CTM.Start();
				if( !TheGame.Update( InputSt ) ){	return 0;	}
				InputSt.ToNextStep();
				Wait_ms = std::max( 1.0, MaxWait_ms - CTM.End() );
			}
		}while( !ShouldQuit );
		
		return 0;
	}
}

//WinMain()
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow
)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//---多重起動の防止措置
	HANDLE hPreventMultiRunEvent = CreateEventW( NULL, FALSE, FALSE, L"X102_Prevent_multi_run" );

	if( hPreventMultiRunEvent == NULL )
	{	return Err( L"処理に失敗しました", L"多重起動チェック" );	}

	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle( hPreventMultiRunEvent );
		hPreventMultiRunEvent = NULL;
		return Err( L"既にAPPが実行中です", L"多重起動チェック" );
	}
	//---

	int Result = MainLoop();

	//---多重起動の防止措置
	CloseHandle( hPreventMultiRunEvent );
	//---

	return Result;
}



