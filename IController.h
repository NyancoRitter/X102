#pragma once

/// <summary>
/// 操作入力情報．
/// 各種操作入力が成されたか否かをチェックする手段
/// </summary>
class IController
{
public:
	virtual ~IController() = default;

public:
	//Test (開発中テスト処理用)
	virtual bool TestKey() const = 0;

	//カーソルの上下左右移動
	virtual bool CursorUp() const = 0;
	virtual bool CursorDown() const = 0;
	virtual bool CursorLeft() const = 0;
	virtual bool CursorRight() const = 0;
	//選択，キャンセル
	virtual bool Select() const = 0;
	virtual bool Cancel() const = 0;

	//パーティメニューを開く
	virtual bool OpenPartyMenu() const = 0;

	//アイテムの　キャラクタ←→ストック　移動
	virtual bool MoveItem() const = 0;
	//アイテム等の整理（一番後ろに移動させる）
	virtual bool MoveItemToBack() const = 0;

	//テキストを次に進める
	virtual bool ToNextText() const = 0;

	//迷路内移動
	virtual bool MoveFront() const = 0;
	virtual bool MoveBack() const = 0;
	virtual bool TurnLeft() const = 0;
	virtual bool TurnRight() const = 0;
	//迷路内で調べる
	virtual bool Inspect() const = 0;
};

