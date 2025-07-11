//[MCIThread.h]
//	2015/7�`

#pragma once
#include <string>

//MCI�����[�J�X���b�h�ŗp���ĉ���炷���߂̃N���X
class CMCIThread
{
public:
	CMCIThread();
	~CMCIThread();
private:
	CMCIThread( const CMCIThread & ) = delete;
	CMCIThread &operator=( const CMCIThread & ) = delete;

public:
	//�X���b�h�J�n�D
	//�����ӁF���ɊJ�n����Ă�ꍇ�C���̃��\�b�h�͉���������false��Ԃ��D
	//
	//hNotifyMsgTgtWnd :
	//	���t�I������ MM_MCINOTIFY ���b�Z�[�W����ԑΏۂ̃E�B���h�E�n���h���D
	//	����ɏI�������ꍇ�� wParam�� MCI_NOTIFY_SUCCESSFUL �ɂȂ��Ă���D
	//	�����t�����[�v������ɂ�RestartBGM()���Ăׂ΂����D
	//
	//	�Ȃ��C����MCI�֘A�ŃG���[���������Ƃ��� ERR_MSG_VAL �����b�Z�[�W�Ƃ��ē�����D
	//	���̏ꍇ�CGetLastMCIErrMsg() �ŃG���[���b�Z�[�W���擾�ł���D
	//
	//ERR_MSG_VAL :
	//	�G���[���������Ƃ��ɓ������郁�b�Z�[�W�̒l�D
	bool BeginThread( HWND hNotifyMsgTgtWnd, UINT ERR_MSG_VAL=WM_APP+1 );

	//�X���b�h�̏I��
	//���R�C���̍Đ��͒�~�����D
	void EndThread();

	//�X���b�h�������Ă����Ԃ��ۂ���Ԃ�
	bool IsThreadRunning() const;

	/// <summary>
	/// BGM�t�@�C����o�^����
	/// </summary>
	/// <param name="specifier">�o�^�ԍ��DChangeBMG()�ł̈����Ɏg�p����</param>
	/// <param name="rBGMFilePath">BGM�t�@�C���p�X</param>
	/// <param name="ShouldKeepLong">
	/// ����BGM�t�@�C����ǂݍ��񂾓��e�𒷊��ێ����ׂ����ۂ��D
	/// false�̏ꍇ�C����BGM�̍Đ����n�߂��ۂɔj�������D
	/// �g�����Ƃ��ẮC
	/// ���x���Đ�����\��������BGM�ɂ��Ă�true���C
	/// ��x�����Đ��@��̂Ȃ�BGM�ɂ��Ă�false���w�肷��̂��ǂ��D
	/// </param>
	/// <returns>
	/// ���ہD
	/// �w��̔ԍ������ɓo�^����Ă���ꍇ�ɂ͎��s�ƂȂ�D
	/// </returns>
	bool RegisterBMG( int specifier, const std::wstring &rBGMFilePath, bool ShouldKeepLong );

	/// <summary>
	/// �Đ�BGM�̕ύX
	/// </summary>
	/// <param name="specifier">BMG�w��DRegisterBGM()�Ŏw�肵���l�ɑΉ�</param>
	/// <returns>
	/// ���ہD
	/// * �X���b�h���J�n����Ă��Ȃ���Ԃł́C���s����D
	/// * ���������݂�BGM���w���Ă���ꍇ�ɂ͉���������true��Ԃ��D
	/// </returns>
	bool ChangeBGM( int specifier );

	//���t���~�߂�
	//�X���b�h���J�n����Ă��Ȃ���Ԃł́C���s����D
	bool StopBGM();

	//�ŏ�����ĉ��t
	//�X���b�h���J�n����Ă��Ȃ���Ԃł́C���s����D
	bool RestartBGM();

	//�G���[���b�Z�[�W�̎擾
	std::wstring GetLastMCIErrMsg();

private:
	class Impl;
	Impl *m_pImpl;
};

//[EOF]