#pragma once

#include "Parts/Flags.h"
#include <functional>
#include <list>

/// <summary>Cmd の戻り値用ビットフラグ</summary>
enum class CmdResult : unsigned int
{
	/// <summary>特に無し</summary>
	None = 0,
	
	/// <summary>コマンドの処理が完了したことを示す</summary>
	Finished =  1,

	/// <summary>再描画が必要であることを示す</summary>
	ReqRedraw = 1 << 1,

	/// <summary>所属シーケンスの後続のコマンドに処理を進めるべきではないことを示す</summary>
	SuppressSubsequents = 1 << 2
};

//Cmd
template< class ...Args >
using Cmd = std::function< Flags<CmdResult>( Args... ) >;

//Cmdのシーケンス
template< class ...Args >
using CmdSequence = std::list< Cmd<Args...> >;

/// <summary>
/// CmdSequenceの処理を進める汎用処理
/// 
/// 先頭のコマンドから順に実行
/// * ただし誰かが SuppressSubsequents を返してきたらその時点でreturn
/// * Finished を返したコマンドはシーケンスから除去される
/// * 誰かが ReqRedraw を返した場合，戻り値はtrueとなる
/// </summary>
/// <typeparam name="...Args"></typeparam>
/// <param name="TgtSeq">対象のシーケンス</param>
/// <param name="...args">Cmdへの引数</param>
/// <returns>
/// * 誰かが ReqRedraw を返した場合， ReqRedraw を含む
/// * 誰かが SuppressSubsequents を返してきたことによってreturnする場合，SuppressSubsequents を含む
/// </returns>
template< class ...Args >
Flags<CmdResult> UpdateCmdSeq( CmdSequence<Args...> &TgtSeq, Args... args )
{
	Flags<CmdResult> Ret = CmdResult::None;

	auto i=TgtSeq.begin();
	while( i!=TgtSeq.end() )
	{
		const auto Result = (*i)( args... );
		if( Result.Has( CmdResult::ReqRedraw ) ){	Ret |= CmdResult::ReqRedraw;	}

		if( Result.Has( CmdResult::Finished ) ){	i = TgtSeq.erase(i);	}
		else{	i++;	}

		if( Result.Has( CmdResult::SuppressSubsequents ) )
		{	return Ret | CmdResult::SuppressSubsequents;	}
	}

	return Ret;
}

/// <summary> 
/// ウェイト．
/// operator() が指定回数だけ CmdResult::SuppressSubsequents を返す．
/// それ以降は CmdResult::Finished を返す．
/// </summary>
template< class ...Args >
class WaitCmd
{
public:
	WaitCmd( int Period ) : m_nRest( Period ) {}

	Flags<CmdResult> operator()( Args... args )
	{
		if( m_nRest<=0 )return CmdResult::Finished;
		--m_nRest;
		return CmdResult::SuppressSubsequents;
	}
private:
	int m_nRest;
};
