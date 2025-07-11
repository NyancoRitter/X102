#pragma once
#include <memory>
#include "Parts/Flags.h"
#include <functional>

/// <summary>
/// ICmd.Exec()の戻り値用ビットフラグ
/// </summary>
enum class CmdResult : unsigned int
{
	/// <summary>特に無し</summary>
	None = 0,
	
	/// <summary>
	/// コマンドの処理が完了したことを示す．
	/// </summary>
	Finished =  1,

	/// <summary>
	/// 再描画が必要であることを示す．
	/// </summary>
	ReqRedraw = 1 << 1,

	/// <summary>
	/// 後続のコマンドに処理を進めるべきではないことを示す．
	/// （戻り値にFinishedが含まれている場合には無視される）
	/// </summary>
	SuppressSubsequents = 1 << 2
};


//
class IAddCmdToSeq;

/// <summary>
/// コマンド．
/// 何らかのコマンドシーケンス（ ICmdDeq ）に登録され，そこから Exec() が呼ばれる想定．
/// </summary>
class ICmd
{
public:
	virtual ~ICmd() = default;

	/// <summary>何か処理を実施する</summary>
	/// <param name="Owner">所属するシーケンスへのコマンド追加登録手段</param>
	/// <returns>処理結果</returns>
	virtual Flags<CmdResult> Exec( IAddCmdToSeq &Owner ) = 0;
};


/// <summary>
/// コマンドシーケンスへのコマンド登録手段
/// </summary>
class IAddCmdToSeq
{
public:
	virtual ~IAddCmdToSeq() = default;

	/// <summary>
	/// 末尾に追加（shared_ptr版）
	/// </summary>
	/// <param name="spCmd">追加対象．ただしnullptrの場合は追加しない</param>
	/// <returns>*thisを返す</returns>
	virtual IAddCmdToSeq &PushBack( std::shared_ptr<ICmd> spCmd ) = 0;

	/// <summary>
	/// 末尾に追加（ポインタ版）
	/// </summary>
	/// <param name="pCmd">追加対象．ただしnullptrの場合は追加しない</param>
	/// <returns>*thisを返す</returns>
	IAddCmdToSeq &PushBack( ICmd *pCmd )
	{	//※何もしないカスタムデリータを持たせたshared_ptrとしてAddする
		if( !pCmd )return *this;
		return PushBack( std::shared_ptr<ICmd>{ pCmd, NOP_Deleter } );
	}

protected:
	static void NOP_Deleter( ICmd* ){}
};

//-----------------

/// <summary>単に ctor で与えた処理を実施するだけの ICmd 実装</summary>
class FuncCmd : public ICmd
{
public:
	FuncCmd( std::function< Flags<CmdResult>() > &Func ) : m_Func(Func) {}
	virtual Flags<CmdResult> Exec( IAddCmdToSeq & ) override {	return m_Func();	}
private:
	std::function< Flags<CmdResult>() > m_Func;
};
