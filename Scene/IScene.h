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

	/// <summary>
	/// APPを終了させるべきこと示す．
	/// （どうしようもないエラーが発生した場合など）
	/// </summary>
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
