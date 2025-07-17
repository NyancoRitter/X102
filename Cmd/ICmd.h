#pragma once
#include <memory>
#include "Parts/Flags.h"
#include <functional>

/// <summary>
/// ICmd.Exec()�̖߂�l�p�r�b�g�t���O
/// </summary>
enum class CmdResult : unsigned int
{
	/// <summary>���ɖ���</summary>
	None = 0,
	
	/// <summary>
	/// �R�}���h�̏����������������Ƃ������D
	/// </summary>
	Finished =  1,

	/// <summary>
	/// �ĕ`�悪�K�v�ł��邱�Ƃ������D
	/// </summary>
	ReqRedraw = 1 << 1,

	/// <summary>
	/// �㑱�̃R�}���h�ɏ�����i�߂�ׂ��ł͂Ȃ����Ƃ������D
	/// �i�߂�l��Finished���܂܂�Ă���ꍇ�ɂ͖��������j
	/// </summary>
	SuppressSubsequents = 1 << 2
};


//
class IAddCmdToSeq;

/// <summary>
/// �R�}���h�D
/// ���炩�̃R�}���h�V�[�P���X�ɓo�^����C�������� Exec() ���Ă΂��z��D
/// </summary>
class ICmd
{
public:
	virtual ~ICmd() = default;

	/// <summary>�������������{����</summary>
	/// <param name="Owner">��������V�[�P���X�ւ̃R�}���h�ǉ��o�^��i</param>
	/// <returns>��������</returns>
	virtual Flags<CmdResult> Exec( IAddCmdToSeq &Owner ) = 0;
};


/// <summary>
/// �R�}���h�V�[�P���X�ւ̃R�}���h�o�^��i
/// </summary>
class IAddCmdToSeq
{
public:
	virtual ~IAddCmdToSeq() = default;

	/// <summary>�����ɒǉ�</summary>
	/// <param name="spCmd">�ǉ��ΏہD������nullptr�̏ꍇ�͒ǉ����Ȃ�</param>
	/// <returns>*this��Ԃ�</returns>
	virtual IAddCmdToSeq &PushBack( std::unique_ptr<ICmd> spCmd ) = 0;
};

//-----------------

/// <summary>�P�� ctor �ŗ^�������������{���邾���� ICmd ����</summary>
class FuncCmd : public ICmd
{
public:
	FuncCmd( std::function< Flags<CmdResult>() > &Func ) : m_Func(Func) {}
	virtual Flags<CmdResult> Exec( IAddCmdToSeq & ) override {	return m_Func();	}
private:
	std::function< Flags<CmdResult>() > m_Func;
};
