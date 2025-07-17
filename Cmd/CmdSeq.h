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
	/// ���̃V�[�P���X�̑S�ẴR�}���h���I������܂Ō㑱��҂����邩�ۂ��D
	/// true ���w�肵���ꍇ�C��łȂ��ꍇ�� Exec() ����� SuppressSubsequents ���܂ޖ߂�l��Ԃ��D
	/// �ifalse ���w�肵���ꍇ�CExec() �̖߂�l�� SuppressSubsequents ���܂ނ��ۂ��͓o�^����Ă���R�}���h�Ɉˑ��j
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
