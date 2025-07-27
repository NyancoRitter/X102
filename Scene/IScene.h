#pragma once
#include "Parts/Flags.h"
#include <memory>

class IController;

/// <summary>
/// IScene.Update()�̖߂�l�p�r�b�g�t���O
/// </summary>
enum class SceneUpdateResult : unsigned int
{
	/// <summary>���ɖ���</summary>
	None = 0,

	/// <summary>
	/// APP���I��������ׂ����Ǝ����D
	/// �i�ǂ����悤���Ȃ��G���[�����������ꍇ�Ȃǁj
	/// </summary>
	ReqAppQuit =  1,

	/// <summary>�ĕ`�悪�K�v�ł��邱�Ƃ�����</summary>
	ReqRedraw = 1 << 1
};

/// <summary>
/// �V�[���F�Q�[����Ԃ̔r���I�Ȑ؂�ւ��P��
/// </summary>
class IScene
{
public:
	virtual ~IScene() = default;

public:
	/// <summary>�J�����g�̃V�[���ɂȂ����ۂɌĂ΂��</summary>
	virtual void OnEnter(){}

	/// <summary>�J�����g�V�[���ł͂Ȃ��Ȃ����i���̃V�[�����J�����g�ɂȂ����j�ۂɌĂ΂��</summary>
	virtual void OnLeave(){}

	/// <summary>�X�V</summary>
	/// <param name="Controller">���͏��</param>
	/// <returns>�������ʏ��</returns>
	virtual Flags<SceneUpdateResult> Update( const IController &Controller ) = 0;

	/// <summary>�`��</summary>
	/// <param name="hdc">�`���DC</param>
	virtual void Draw( HDC hdc ) = 0;
};
