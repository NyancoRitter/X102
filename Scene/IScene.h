#pragma once
#include "Parts/Flags.h"
#include <memory>

class IController;

/// <summary>
/// IScene.Update()の戻り値用ビットフラグ
/// </summary>
enum class SceneUpdateResult : unsigned int
{
	/// <summary>特に無し</summary>
	None = 0,

	/// <summary>APPを終了させるべきこと示す（どうしようもないエラーが発生した場合など）</summary>
	ReqAppQuit =  1,

	/// <summary>再描画が必要であることを示す</summary>
	ReqRedraw = 1 << 1
};

/// <summary>
/// シーン：ゲーム状態の排他的な切り替え単位
/// </summary>
class IScene
{
public:
	virtual ~IScene() = default;

public:
	/// <summary>カレントのシーンになった際に呼ばれる</summary>
	virtual void OnEnter(){}

	/// <summary>カレントシーンではなくなった（他のシーンがカレントになった）際に呼ばれる</summary>
	virtual void OnLeave(){}

	/// <summary>更新</summary>
	/// <param name="Controller">入力情報</param>
	/// <returns>処理結果情報</returns>
	virtual Flags<SceneUpdateResult> Update( const IController &Controller ) = 0;

	/// <summary>描画</summary>
	/// <param name="hdc">描画先DC</param>
	virtual void Draw( HDC hdc ) = 0;
};

//-------------------------------------

class ITopLV;

/// <summary>街のシーン</summary>
class ITownScene : public IScene
{
public:
	/// <summary>
	/// 背景画像を，通常の街の絵ではなく「町はずれ」にする．
	/// ただし，この変更は施設（店など）への移動が発生した時点で自動的にリセットされる．
	///		<remarks>
	///		※パーティが戦闘敗北した後の表示用
	///		</remarks>
	/// </summary>
	virtual void SetImgToOutskirts() = 0;

public:
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="rTopLV">この参照は生成されたインスタンスによって長期的に利用される</param>
	/// <returns>インスタンスを保有するunique_ptr</returns>
	static std::unique_ptr<ITownScene> Create( ITopLV &rTopLV );
};

/// <summary>迷路のシーン</summary>
//class IMazeScene : public IScene
//{
//public:
//	static std::unique_ptr<IMazeScene> Create( ITopLV &rTopLV );
//};

/// <summary>
/// 迷路シーンのインスタンス生成．
/// 
/// 迷路データの読込が行われる．
/// 読込失敗時には UTF-8 文字列でエラーメッセージを持つ std::runtime_error が送出される．
/// </summary>
/// <param name="rTopLV">この参照は生成されたインスタンスによって長期的に利用される</param>
/// <returns>成功時はインスタンスを保有するunique_ptr</returns>
std::unique_ptr<IScene> CreateMazeScene( ITopLV &rTopLV );

