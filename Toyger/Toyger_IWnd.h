#pragma once
#include "framework.h"
#include <string>
#include <memory>

namespace Toyger
{
	class IContentPainter;
	class IBGMPlayer;

	/// <summary>�Q�[������������̃E�B���h�E�ւ̃A�N�Z�XI/F</summary>
	/// <remarks>���ۂ�Ԃ����̃��\�b�h�́C�E�B���h�E�����O��E�B���h�E�������̏�Ԃł͎��s����D</remarks>
	class IWnd
	{
	public:
		virtual ~IWnd() = default;
	
	public:	//�\�����̃T�C�Y�w��C�`�� -------------------------

		/// <summary>�E�B���h�E�̃N���C�A���g�̈�ɕ\��������e��(�g�嗦1�{�ł�)�T�C�Y��ݒ�</summary>
		/// <param name="W">���]�̕�</param>
		/// <param name="H">���]�̍���</param>
		/// <returns>����</returns>
		virtual bool SetViewContentSize( int W, int H ) = 0;

		/// <summary>�\���g��{���̐ݒ�</summary>
		/// <param name="Rate">�{���D1�ȉ��̒l���w�肵���ꍇ�ɂ�1�ƌ��Ȃ����D</param>
		/// <returns>����</returns>
		virtual bool SetViewMagRate( int Rate ) = 0;

		/// <summary>���݂̕\���g��{��</summary>
		/// <returns>�{��</returns>
		virtual int GetViewMagRate() const = 0;

		/// <summary>�E�B���h�E�̃T�C�Y�����݂̕\�����̃T�C�Y�i= �R���e���c�T�C�Y * �\���{���j�ɍ��킹��</summary>
		/// <returns>����</returns>
		virtual bool ResizeToFit() = 0;

		/// <summary>�N���C�A���g�̈�̕\�����e�������̃I�u�W�F�N�g��p���čX�V����</summary>
		/// <param name="rPainter">�`�揈����</param>
		/// <returns>����</returns>
		virtual bool UpdateViewContent( IContentPainter &rPainter ) = 0;

		///// <summary>���݂̕\�����e�i�Ō�� UpdateViewContent() �ɂ��`�挋�ʁj�̃N���[���𐶐�����D</summary>
		///// <returns>
		///// �������͐��������r�b�g�}�b�v�̃n���h���D����͌Ăяo������ DeleteObject() API�ō폜����K�v������D
		///// ���s���� NULL ��Ԃ��D
		///// </returns>
		//virtual HBITMAP CloneCurrentViewContent() const = 0;

	public:	//�X�N���[��, ���W�ϊ� �֘A -----------------------------------

		/// <summary>�R���e���c�̎w����W���\�Ȍ���\����̒����ɗ���悤�ɃX�N���[���ʒu��ݒ肷��</summary>
		/// <param name="cx">�����ɗ���悤�ɂ������ʒu�D�R���e���c�̍��W�n�Ŏw��D</param>
		/// <param name="cy">�����ɗ���悤�ɂ������ʒu�D�R���e���c�̍��W�n�Ŏw��D</param>
		/// <returns>����</returns>
		virtual bool ScrollTo( int cx, int cy ) = 0;

		//�X�N���[���Ȃ�pixel���W���X�N���[�����̕`�悷�ׂ�pixel���W�@�ϊ�

		/// <summary>�R���e���c���W���N���C�A���g�̈���W�@�ϊ�</summary>
		/// <param name="ContentX">�R���e���c�̍��W�n�ł̈ʒu</param>
		/// <returns>
		/// �Ή�����N���C�A���g�̈�ł̒l�D
		/// �\�����g�傳��Ă���󋵂ł͊g��㐳���`�̍���̍��W�D
		/// </returns>
		virtual int ToClientAreaX( int ContentX ) const = 0;

		/// <summary>�R���e���c���W���N���C�A���g�̈���W�@�ϊ�</summary>
		/// <param name="ContentY">�R���e���c�̍��W�n�ł̈ʒu</param>
		/// <returns>
		/// �Ή�����N���C�A���g�̈�ł̈ʒu�D
		/// �\�����g�傳��Ă���󋵂ł͊g��㐳���`�̍���̍��W�D
		/// </returns>
		virtual int ToClientAreaY( int ContentY ) const = 0;

		/// <summary>�N���C�A���g�̈���W���R���e���c���W�@�ϊ�</summary>
		/// <param name="ClientAreaX">�N���C�A���g�̈���W�ł̈ʒu</param>
		/// <returns>�Ή�����R���e���c���W�ł̈ʒu</returns>
		virtual int ToContentX( int ClientAreaX ) const = 0;

		/// <summary>�N���C�A���g�̈���W���R���e���c���W�@�ϊ�</summary>
		/// <param name="ClientAreaY">�N���C�A���g�̈���W�ł̈ʒu</param>
		/// <returns>�Ή�����R���e���c���W�ł̈ʒu</returns>
		virtual int ToContentY( int ClientAreaY ) const = 0;

	public:	//���̑��E�B���h�E���� -----------------------------

		/// <summary>�E�B���h�E�L���v�V����������̕ύX</summary>
		/// <param name="pStr">�L���v�V����������</param>
		/// <returns>����</returns>
		virtual bool SetCaption( LPCTSTR pStr ) = 0;

		/// <summary>�E�B���h�E�ʒu�̕ύX</summary>
		/// <param name="rW">���]�̃E�B���h�E���[x���W</param>
		/// <param name="rH">���]�̃E�B���h�E��[y���W</param>
		/// <returns>����</returns>
		virtual bool MoveTo( int x, int y ) = 0;

		///<summary>���̃E�B���h�E��e�Ƃ��Ẵ��b�Z�[�W�{�b�N�X�̃��[�_���\��</summary>
		virtual int MsgBox( LPCTSTR Msg, LPCTSTR Caption, UINT MsgBoxType ) = 0;

		/// <summary>
		/// �w�蕶������w��̃t�H���g�ŕ`�悵���ۂ̃T�C�Y�𓾂�
		/// </summary>
		/// <param name="pStrU16">UTF16������</param>
		/// <param name="hFont">�t�H���g�i������NULL���w�肵���ꍇ�ɂ͌��݂̃t�H���g���g�p�j</param>
		/// <returns>�T�C�Y</returns>
		virtual SIZE CalcTextDrawSize( LPCWSTR pStrU16, HFONT hFont ) const = 0;

		/// <summary>
		/// �E�B���h�E�̃A�C�R����ύX
		/// </summary>
		/// <param name="IconHandle">�A�C�R���̃n���h��</param>
		/// <param name="ForSmall">
		/// �Ώۂ̎w��D�������A�C�R�����ۂ��Dfalse�Ȃ�傫���A�C�R��
		/// </param>
		/// <returns>����</returns>
		virtual bool ChangeIcon( HANDLE IconHandle, bool ForSmall ) = 0;

	public:	//BMG ----------------------------------------------

		/// <summary>
		/// IBGMPlayer�ւ̃|�C���^��Ԃ��D
		/// </summary>
		/// <returns>
		/// ���s����nullptr�D
		/// * �E�B���h�E�����O�ɌĂԂƎ��s����D
		/// * �߂�l���w���I�u�W�F�N�g���g�p�ł�����Ԃ̓E�B���h�E��������܂łł���D
		///   �i������ւ�𒍈ӂ��Ďg�����Ƃ͂��̃��\�b�h���Ăԑ��̐ӔC�Ƃ���j
		/// </returns>
		virtual IBGMPlayer *BGMPlayer() = 0;
	};
}
