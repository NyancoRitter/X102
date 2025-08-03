#pragma once

/// <summary>
/// ������͏��D
/// �e�푀����͂������ꂽ���ۂ����`�F�b�N�����i
/// </summary>
class IController
{
public:
	virtual ~IController() = default;

public:
	//Test (�J�����e�X�g�����p)
	virtual bool TestKey() const = 0;

	//�J�[�\���̏㉺���E�ړ�
	virtual bool CursorUp() const = 0;
	virtual bool CursorDown() const = 0;
	virtual bool CursorLeft() const = 0;
	virtual bool CursorRight() const = 0;
	//�I���C�L�����Z��
	virtual bool Select() const = 0;
	virtual bool Cancel() const = 0;

	//�p�[�e�B���j���[���J��
	virtual bool OpenPartyMenu() const = 0;

	//�A�C�e���́@�L�����N�^�����X�g�b�N�@�ړ�
	virtual bool MoveItem() const = 0;
	//�A�C�e�����̐����i��Ԍ��Ɉړ�������j
	virtual bool MoveItemToBack() const = 0;

	//�e�L�X�g�����ɐi�߂�
	virtual bool ToNextText() const = 0;

	//���H���ړ�
	virtual bool MoveFront() const = 0;
	virtual bool MoveBack() const = 0;
	virtual bool TurnLeft() const = 0;
	virtual bool TurnRight() const = 0;
	//���H���Œ��ׂ�
	virtual bool Inspect() const = 0;
};

