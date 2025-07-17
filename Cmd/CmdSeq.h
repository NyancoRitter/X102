#pragma once

#include "ICmd.h"
#include <list>

class CmdSeq :
	public IAddCmdToSeq,
	public ICmd
{
public:
	/// <summary>ctor</summary>
	/// <param name="SuppressSubsequents_when_NotEmpty">
	/// このシーケンスの全てのコマンドが終了するまで後続を待たせるか否か．
	/// true を指定した場合，空でない場合に Exec() が常に SuppressSubsequents を含む戻り値を返す．
	/// （false を指定した場合，Exec() の戻り値が SuppressSubsequents を含むか否かは登録されているコマンドに依存）
	/// </param>
	CmdSeq( bool SuppressSubsequents_when_NotEmpty=true )
		: m_bSuppressSubsequents_when_NotEmpty(SuppressSubsequents_when_NotEmpty)
	{}

private:
	CmdSeq( const CmdSeq & ) = delete;
	CmdSeq &operator =( const CmdSeq & ) = delete;
public:
	bool empty() const {	return m_CmdList.empty();	}
	void clear(){	m_CmdList.clear();	}

public:	//IAddCmdToSeq Impl
	virtual CmdSeq &PushBack( std::unique_ptr<ICmd> spCmd ) override {	m_CmdList.push_back( std::move(spCmd) );	return *this;	}

public:	//ICmd Impl
	virtual Flags<CmdResult> Exec( IAddCmdToSeq &Owner ) override
	{
		if( empty() )return CmdResult::Finished;

		Flags<CmdResult> Ret;
		auto isp = m_CmdList.begin();
		while( isp != m_CmdList.end() )
		{
			const auto result = (*isp)->Exec( *this );

			if( result.Has( CmdResult::ReqRedraw ) )
			{	Ret |= CmdResult::ReqRedraw;	}

			if( result.Has( CmdResult::Finished ) )
			{	isp = m_CmdList.erase( isp );	}
			else
			{
				if( result.Has( CmdResult::SuppressSubsequents ) )
				{	return ( Ret | CmdResult::SuppressSubsequents );	}

				++isp;
			}
		}

		if( empty() )
		{	Ret |= CmdResult::Finished;	}
		else if( m_bSuppressSubsequents_when_NotEmpty )
		{	Ret |= CmdResult::SuppressSubsequents;	}

		return Ret;
	}

private:
	std::list< std::unique_ptr<ICmd> > m_CmdList;
	bool m_bSuppressSubsequents_when_NotEmpty;
};
