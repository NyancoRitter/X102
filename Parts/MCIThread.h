//[MCIThread.h]
//	2015/7～

#pragma once
#include <string>

//MCIをワーカスレッドで用いて音を鳴らすためのクラス
class CMCIThread
{
public:
	CMCIThread();
	~CMCIThread();
private:
	CMCIThread( const CMCIThread & ) = delete;
	CMCIThread &operator=( const CMCIThread & ) = delete;

public:
	//スレッド開始．
	//※注意：既に開始されてる場合，このメソッドは何もせずにfalseを返す．
	//
	//hNotifyMsgTgtWnd :
	//	演奏終了時に MM_MCINOTIFY メッセージが飛ぶ対象のウィンドウハンドル．
	//	正常に終了した場合は wParamが MCI_NOTIFY_SUCCESSFUL になっている．
	//	→演奏をループさせるにはRestartBGM()を呼べばいい．
	//
	//	なお，何かMCI関連でエラーがあったときは ERR_MSG_VAL をメッセージとして投げる．
	//	その場合，GetLastMCIErrMsg() でエラーメッセージを取得できる．
	//
	//ERR_MSG_VAL :
	//	エラーがあったときに投げられるメッセージの値．
	bool BeginThread( HWND hNotifyMsgTgtWnd, UINT ERR_MSG_VAL=WM_APP+1 );

	//スレッドの終了
	//当然，音の再生は停止される．
	void EndThread();

	//スレッドが動いている状態か否かを返す
	bool IsThreadRunning() const;

	/// <summary>
	/// BGMファイルを登録する
	/// </summary>
	/// <param name="specifier">登録番号．ChangeBMG()での引数に使用する</param>
	/// <param name="rBGMFilePath">BGMファイルパス</param>
	/// <param name="ShouldKeepLong">
	/// このBGMファイルを読み込んだ内容を長期保持すべきか否か．
	/// falseの場合，他のBGMの再生を始めた際に破棄される．
	/// 使い方としては，
	/// 何度も再生する可能性があるBGMについてはtrueを，
	/// 一度しか再生機会のないBGMについてはfalseを指定するのが良い．
	/// </param>
	/// <returns>
	/// 成否．
	/// 指定の番号が既に登録されている場合には失敗となる．
	/// </returns>
	bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong );

	/// <summary>
	/// 再生BGMの変更
	/// </summary>
	/// <param name="specifier">BMG指定．RegisterBGM()で指定した値に対応</param>
	/// <returns>
	/// 成否．
	/// * スレッドが開始されていない状態では，失敗する．
	/// * 引数が現在のBGMを指している場合には何もせずにtrueを返す．
	/// </returns>
	bool ChangeBGM( int specifier );

	//演奏を止める
	//スレッドが開始されていない状態では，失敗する．
	bool StopBGM();

	//最初から再演奏
	//スレッドが開始されていない状態では，失敗する．
	bool RestartBGM();

	//エラーメッセージの取得
	std::wstring GetLastMCIErrMsg();

private:
	class Impl;
	Impl *m_pImpl;
};

//[EOF]