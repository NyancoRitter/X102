#pragma once
#include <memory>

namespace Toyger
{

	/// <summary>
	/// Win32 �̃X�N���[���o�[�������ώG�ȏ������܂Ƃ߂����́D
	/// ���ۂɃX�N���[���o�[�������Ă���E�B���h�E������C���̃N���X�̃��\�b�h��K�؂ɃR�[�����Ďg���D
	/// </summary>
	class ScrollBarHandling
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="hWnd">
		/// �X�N���[���o�[����ΏۃE�B���h�E���w��D
		/// ���̃I�u�W�F�N�g���g�p����ԁC���̃n���h�����w���E�B���h�E���L���ł���i���݂��Ă���j�K�v������
		/// </param>
		/// <param name="bShowDisableScrollBar">�\����̃T�C�Y���R���e���c�T�C�Y���L���Ƃ��ɂ��X�N���[���o�[���������ɕ\�����邩�ۂ�</param>
		ScrollBarHandling( HWND hWnd, bool bShowDisableScrollBar = false )
			: m_hWnd( hWnd )
			, m_bShowDisableScrollBar(bShowDisableScrollBar)
		{}

	public:
		/// <summary>
		/// �\�����̃T�C�Y�i�F�\����̃T�C�Y������ȏ�̏ꍇ�̓X�N���[���o�[�s�v�j���w��D
		/// �p�@�F�\���R���e���c�̃T�C�Y���ς�����Ƃ��ɃR�[�����邱�ƁD
		/// </summary>
		/// <param name="W">�\�����T�C�Y</param>
		/// <param name="H">�\�����T�C�Y</param>
		void ChangeContentSize( int W, int H );

		/// <summary>
		/// �\����i�N���C�A���g�̈�j�T�C�Y���w��D
		/// �p�@�F�R���e���c�\����̃T�C�Y���ς�����Ƃ��i�E�B���h�E���T�C�Y���j�ɃR�[�����邱�ƁD
		/// </summary>
		/// <param name="W">�\����T�C�Y</param>
		/// <param name="H">�\����T�C�Y</param>
		void ChangeViewSize( int W, int H );

		/// <summary>
		/// �X�N���[���C�x���g���̏����D
		/// �p�@�FWM_HSCROLL, WM_VSCROLL �C�x���g�̏��������̃��\�b�h�ɈϏ�����D
		/// </summary>
		/// <param name="HorizontalBar">����Ώۂ̎w��F�����X�N���[���o�[�Ȃ��true, �����X�N���[���o�[�Ȃ��false</param>
		/// <param name="nSBCode">�X�N���[���o�[�R�[�h�D�C�x���g�� LOWORD(wParam) �����̂܂ܓn���΂n�j�D</param>
		/// <param name="nPos">nSBCode������̒l�̏ꍇ�̂ݗL���ȃX�N���[���ʒu�D�C�x���g�� HIWORD(wParam) �����̂܂ܓn���΂n�j�D</param>
		void HandleScrollEvent( bool HorizontalBar, UINT nSBCode, UINT nPos ) const;

		/// <summary>�X�N���[���o�[�̌��݂̃X�N���[���ʒu���擾</summary>
		/// <param name="HorizontalBar">�Ώۂ̎w��F�����X�N���[���o�[�Ȃ��true, �����X�N���[���o�[�Ȃ��false</param>
		/// <returns>�X�N���[���ʒu</returns>
		int GetScrollPos( bool HorizontalBar ) const;

		//�X�N���[���o�[�̃X�N���[���ʒu��ݒ�D
		//�i�������C�w��l���s�K�؂ȏꍇ�C��������̕ʂ̈ʒu�ɐݒ肳�꓾��j
		//HorizontalBar : �����X�N���[���o�[�Ȃ��true, �����X�N���[���o�[�Ȃ��false
		//Pos : ���]�̈ʒu�D

		/// <summary>
		/// �X�N���[���o�[�̃X�N���[���ʒu��ݒ�
		/// �i�������C�w��l���s�K�؂ȏꍇ�C��������̕ʂ̈ʒu�ɐݒ肳�꓾��j
		/// </summary>
		/// <param name="HorizontalBar">�Ώۂ̎w��F�����X�N���[���o�[�Ȃ��true, �����X�N���[���o�[�Ȃ��false</param>
		/// <param name="Pos">�X�N���[���ʒu</param>
		/// <returns>�X�N���[���ʒu���ω��������ۂ��D���{�O��ŕω������������ꍇ�ɂ�false��Ԃ��D</returns>
		bool SetScrollPos( bool HorizontalBar, int Pos ) const;

		//�R���e���c�̎w��ʒu���\�Ȍ���\����̒����ɗ���悤�ɃX�N���[���ʒu��ݒ肷��
		//[Args]
		//	cx.cy : �����ɗ���悤�ɂ������ʒu�D�R���e���c�̍��W�n�Ŏw��D

		/// <summary>
		/// �R���e���c�̎w��ʒu���\�Ȍ���\����̒����ɗ���悤�ɃX�N���[���ʒu��ݒ肷��
		/// </summary>
		/// <param name="cx">�����ɗ���悤�ɂ������ʒu</param>
		/// <param name="cy">�����ɗ���悤�ɂ������ʒu</param>
		/// <returns>�X�N���[���ʒu���ω��������ۂ��D���{�O��ŕω������������ꍇ�ɂ�false��Ԃ��D</returns>
		bool ScrollTo( int cx, int cy ) const;

	private:
		void UpdateScrollBar( bool bHorizonalBar );

		void SetSI(bool bHorizonalBar, SCROLLINFO & Info) const
		{	::SetScrollInfo( m_hWnd, (bHorizonalBar ? SB_HORZ : SB_VERT ), &Info, TRUE );	}

		void GetSI(bool bHorizonalBar, SCROLLINFO & Info) const
		{	::GetScrollInfo( m_hWnd, (bHorizonalBar ? SB_HORZ : SB_VERT ), &Info );	}

	private:
		HWND m_hWnd = NULL;
		int m_ViewW = 1;
		int m_ViewH = 1;
		int m_ContentW = 1;
		int m_ContentH = 1;
		bool m_bShowDisableScrollBar;
	};
}
