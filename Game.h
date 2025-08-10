#pragma once
#include "Toyger/Toyger_InputImpl.h"
#include "Toyger/Toyger_IContentPainter.h"
#include <memory>
#include "ITopLV.h"

namespace Toyger{ class IWnd; }
using InputState = Toyger::IInputState<uint8_t>;

class IScene;
namespace Town{	class TownScene;	}

class PlayData;

/// <summary>
/// ゲーム実装
/// </summary>
class Game final
	: public Toyger::IContentPainter
	, public ITopLV
{
public:
	/// <summary>
	/// ctor. ウィンドウをDI
	/// </summary>
	/// <param name="Wnd">
	/// 生成済のウィンドウを管理しているインスタンスへの参照．
	/// この参照先のインスタンスの寿命だけでなく，
	/// ウィンドウの有効期間も，このGameインスタンスよりも長い
	/// （：要は，このGameインスタンスのメソッドが呼ばれるとき，ウィンドウは閉じられていない）という想定．
	/// </param>
	Game( Toyger::IWnd &Wnd );
	~Game();

public:
	/// <summary>
	/// 準備処理．
	/// プログラム起動後～メインループまでの間に１度だけ呼ばれる想定．
	/// （２回以上呼び出した際の動作は未保障）
	/// </summary>
	/// <returns>成否．続行不可能な失敗があった際はfalse</returns>
	bool Initialize();

	/// <summary>
	/// 更新処理．
	/// メインループから呼ばれる想定．
	///		<remarks>
	///		事前に Initialize() が１度だけ呼ばれ，
	///		且つ，その際， Initialize() が true を返している必要がある．
	///		この事前条件を満たさずにこのメソッドを呼んだ際の動作は未保障．
	/// 
	///		また，このメソッドが false を返して以降は，
	///		このメソッドの動作は未保障．
	///		</remarks>
	/// </summary>
	/// <param name="Input">入力状態</param>
	/// <returns>
	/// 続行 or Not．
	/// falseはプログラムを終了させるべきことを示す．
	/// </returns>
	bool Update( const InputState &Input );

public:	// Toyger::IContentPainter Impl
	virtual void Paint( HDC hdc, int W, int H ) override;

public:	// ITopLV Imp;
	virtual PlayData &CurrPlayData() override;
	virtual void ChangeToTownScene() override;
	
	virtual bool ChangeBGM( int BGM_index ) override;
	virtual void StopBMG() override;

	virtual void ShowMsgBox( const std::wstring &Title, const std::wstring &Msg ) const override;

private:
	//音関係の準備
	void SetupSound();

	//引数が指すシーンをカレントシーンにする
	void ChangeCurrSceneTo( IScene *pScene );

	//表示倍率を変更
	void ChangeViewMagRate( int Rate );

private:
	Toyger::IWnd &m_rWnd;	//ウィンドウへのアクセス手段．ctorで指定された参照を保持
	HFONT m_hDefaultFont = NULL;	//デフォルトのフォント

	//
	std::unique_ptr< PlayData > m_upPlayData;	//プレイ状況データ

	//
	IScene *m_pCurrScene = nullptr;	//カレントシーン
	std::unique_ptr< Town::TownScene > m_upTownScene;	//テスト用
};
