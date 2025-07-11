#pragma once

namespace Toyger
{
	/// <summary>Toyger::GameWnd �̃E�B���h�E�v���V�[�W��������͏����󂯂�l</summary>
	class IWndInputListener
	{
	public:
		virtual ~IWndInputListener() = default;
	public:
		/// <summary>�L�[���邢�̓}�E�X�{�^���������ꂽ���Ƃ̒ʒm</summary>
		/// <param name="VKeyCode">�Ώۂ̃L�[�܂��̓}�E�X�{�^���̉��z�L�[�R�[�h</param>
		virtual void OnPress( unsigned char VKeyCode ) = 0;

		/// <summary>�L�[���邢�̓}�E�X�{�^���������ꂽ���Ƃ̒ʒm</summary>
		/// <param name="VKeyCode">�Ώۂ̃L�[�܂��̓}�E�X�{�^���̉��z�L�[�R�[�h</param>
		virtual void OnRelease( unsigned char VKeyCode ) = 0;

		/// <summary>�}�E�X�J�[�\�����ړ��������Ƃ̒ʒm</summary>
		/// <param name="x">�N���C�A���g�̈��ł̍��W</param>
		/// <param name="y">�N���C�A���g�̈��ł̍��W</param>
		virtual void OnMouseMove( int x, int y ) = 0;

		/// <summary>�E�B���h�E����鑀�삪�����ꂽ�ۂ̒ʒm</summary>
		/// <returns>�E�B���h�E����ėǂ����ǂ����Dfalse��Ԃ��ƕ��Ȃ��i���삪���p�����j</returns>
		virtual bool OnWndCloseOP() = 0;
	};
}