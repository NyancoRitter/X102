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

	/// <summary>APP���I��������ׂ����Ǝ����i�ǂ����悤���Ȃ��G���[�����������ꍇ�Ȃǁj</summary>
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

//-------------------------------------

class ITopLV;

/// <summary>�X�̃V�[��</summary>
class ITownScene : public IScene
{
public:
	/// <summary>
	/// �w�i�摜���C�ʏ�̊X�̊G�ł͂Ȃ��u���͂���v�ɂ���D
	/// �������C���̕ύX�͎{�݁i�X�Ȃǁj�ւ̈ړ��������������_�Ŏ����I�Ƀ��Z�b�g�����D
	///		<remarks>
	///		���p�[�e�B���퓬�s�k������̕\���p
	///		</remarks>
	/// </summary>
	virtual void SetImgToOutskirts() = 0;

public:
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="rTopLV">���̎Q�Ƃ͐������ꂽ�C���X�^���X�ɂ���Ē����I�ɗ��p�����</param>
	/// <returns>�C���X�^���X��ۗL����unique_ptr</returns>
	static std::unique_ptr<ITownScene> Create( ITopLV &rTopLV );
};

/// <summary>���H�̃V�[��</summary>
//class IMazeScene : public IScene
//{
//public:
//	static std::unique_ptr<IMazeScene> Create( ITopLV &rTopLV );
//};

/// <summary>
/// ���H�V�[���̃C���X�^���X�����D
/// 
/// ���H�f�[�^�̓Ǎ����s����D
/// �Ǎ����s���ɂ� UTF-8 ������ŃG���[���b�Z�[�W������ std::runtime_error �����o�����D
/// </summary>
/// <param name="rTopLV">���̎Q�Ƃ͐������ꂽ�C���X�^���X�ɂ���Ē����I�ɗ��p�����</param>
/// <returns>�������̓C���X�^���X��ۗL����unique_ptr</returns>
std::unique_ptr<IScene> CreateMazeScene( ITopLV &rTopLV );

