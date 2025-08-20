#pragma once

#include "Parts/Flags.h"
#include <functional>
#include <list>

/// <summary>Cmd �̖߂�l�p�r�b�g�t���O</summary>
enum class CmdResult : unsigned int
{
	/// <summary>���ɖ���</summary>
	None = 0,
	
	/// <summary>�R�}���h�̏����������������Ƃ�����</summary>
	Finished =  1,

	/// <summary>�ĕ`�悪�K�v�ł��邱�Ƃ�����</summary>
	ReqRedraw = 1 << 1,

	/// <summary>�����V�[�P���X�̌㑱�̃R�}���h�ɏ�����i�߂�ׂ��ł͂Ȃ����Ƃ�����</summary>
	SuppressSubsequents = 1 << 2
};

//Cmd
template< class ...Args >
using Cmd = std::function< Flags<CmdResult>( Args... ) >;

//Cmd�̃V�[�P���X
template< class ...Args >
using CmdSequence = std::list< Cmd<Args...> >;

/// <summary>
/// CmdSequence�̏�����i�߂�ėp����
/// 
/// �擪�̃R�}���h���珇�Ɏ��s
/// * �������N���� SuppressSubsequents ��Ԃ��Ă����炻�̎��_��return
/// * Finished ��Ԃ����R�}���h�̓V�[�P���X���珜�������
/// * �N���� ReqRedraw ��Ԃ����ꍇ�C�߂�l��true�ƂȂ�
/// </summary>
/// <typeparam name="...Args"></typeparam>
/// <param name="TgtSeq">�Ώۂ̃V�[�P���X</param>
/// <param name="...args">Cmd�ւ̈���</param>
/// <returns>
/// * �N���� ReqRedraw ��Ԃ����ꍇ�C ReqRedraw ���܂�
/// * �N���� SuppressSubsequents ��Ԃ��Ă������Ƃɂ����return����ꍇ�CSuppressSubsequents ���܂�
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
/// �E�F�C�g�D
/// operator() ���w��񐔂��� CmdResult::SuppressSubsequents ��Ԃ��D
/// ����ȍ~�� CmdResult::Finished ��Ԃ��D
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
