#pragma once
#include "Toyger_IWnd.h"

namespace Toyger
{
	class IWndInputListener;
	class ScrollBarHandling;

	/// <summary>�E�B���h�E����</summary>
	class GameWnd : public IWnd
	{
	public:
		/// <summary>ctor.</summary>
		/// <param name="rInputListener">
		/// ���̓��X�i�������Ŏw�肷��
		///		<remarks>���^���琄���ł���ʂ�C���̃��X�i�̎����̓E�B���h�E���������K�v������</remarks>
		/// </param>
		GameWnd( IWndInputListener &rInputListener );

		/// <summary>dtor. DstroyWnd()���Ă΂��D</summary>
		virtual ~GameWnd();

	public:
		/// <summary>
		/// �E�B���h�E����
		///		<remarks>���ɃE�B���h�E���������ꂢ��ꍇ�ɂ͉���������true��Ԃ�</remarks>
		/// </summary>
		/// <param name="pCaptionStr">�^�C�g���e�L�X�g</param>
		/// <param name="ResizableBorder">�T�C�Y�ύX�\�ȃE�B���h�E�Ƃ��邩�ۂ�</param>
		/// <param name="left">�ꏊ</param>
		/// <param name="top">�ꏊ</param>
		/// <param name="width">�傫��</param>
		/// <param name="height">�傫��</param>
		/// <returns>����</returns>
		bool CreateWnd(
			LPCTSTR pCaptionStr,
			bool Resizable,
			int left = CW_USEDEFAULT,
			int top = CW_USEDEFAULT,
			int width = CW_USEDEFAULT,
			int height = CW_USEDEFAULT
			);

		/// <summary>�E�B���h�E�j��</summary>
		/// <remarks>�E�B���h�E����������Ă��Ȃ��ꍇ�ɂ͉���������true��Ԃ�</remarks>
		/// <returns>����</returns>
		bool DestroyWnd();

		/// <summary>�E�B���h�E��������Ă��邩</summary>
		bool IsWndCreated() const {	return (m_hWnd != NULL);	}

		/// <summary>�E�B���h�E���\����Ԃ�</summary>
		/// <returns>
		/// �\����Ԃł����true�D
		/// �������C�E�B���h�E����������Ă��Ȃ��ꍇ�ɂ�false��Ԃ��D
		/// </returns>
		bool IsWndVisible() const;

		/// <summary>�E�B���h�E�\����Ԃ̕ύX</summary>
		/// <param name="bShow">true���w�肷��ƕ\����ԂɁCfalse���w�肷��Ɣ�\����Ԃɂ���</param>
		/// <remarks>�E�B���h�E����������Ă��Ȃ��ꍇ�ɂ͉������Ȃ�</remarks>
		void ShowWnd( bool bShow );

	public:	//IWnd����

		virtual bool SetViewContentSize( int W, int H ) override;
		virtual bool SetViewMagRate( int Rate ) override;
		virtual int GetViewMagRate() const override {	return m_ViewMagRate;	}
		virtual bool ResizeToFit() override;
		virtual bool UpdateViewContent( IContentPainter &rPainter ) override;
		//virtual HBITMAP CloneCurrentViewContent() const override;

		virtual bool ScrollTo( int cx, int cy ) override ;
		virtual int ToClientAreaX( int ContentX ) const override;
		virtual int ToClientAreaY( int ContentY ) const override;
		virtual int ToContentX( int ClientAreaX ) const override;
		virtual int ToContentY( int ClientAreaY ) const override;

		virtual bool SetCaption( LPCTSTR pStr ) override;
		virtual bool MoveTo( int x, int y ) override;
		virtual int MsgBox( LPCTSTR Msg, LPCTSTR Caption, UINT MsgBoxType ) override;
		virtual SIZE CalcTextDrawSize( LPCWSTR pStrU16, HFONT hFont ) const override;
		virtual bool ChangeIcon( HANDLE IconHandle, bool ForSmall ) override;

		virtual IBGMPlayer* BGMPlayer() override;

	private:	//private methods
		GameWnd( const GameWnd & ) = delete;	//�����̋֎~
		GameWnd &operator =( const GameWnd & ) = delete;	//�����̋֎~
		LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam );
		bool SetClientAreaSize( int W, int H );

	private:	//data
		HWND m_hWnd = NULL;	//�E�B���h�E�n���h�� or NULL
		IWndInputListener &m_rInputListener;	//ctor�Ŏw�肳�ꂽ���̓C�x���g���X�i
		std::unique_ptr<ScrollBarHandling> m_upSBHandler;	//�X�N���[���֘A�����p

		HBITMAP m_hViewContent = NULL;	//�N���C�A���g�̈�ւ̕\�����e
		int m_ViewContentW = 320;	//m_hViewContent�̃T�C�Y
		int m_ViewContentH = 240;	//m_hViewContent�̃T�C�Y
		int m_ViewMagRate = 1;	//�\���g��{��

	private:	//static
		static bool RegisterWndClass();
		static LRESULT CALLBACK WndProc_s( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		static HINSTANCE GetHInstance(){	return ::GetModuleHandle( NULL );	}
		
		static bool ms_bRegisterClassFlag;	//RegisterClass�������Ƃ��o���Ă����t���O
		static LPCTSTR ms_WndClassName;	//�E�B���h�E�N���X��

	private:
		static constexpr UINT MCITHREAD_ERR_MSG = (WM_APP+1);

		class MCI_BGM_Player;
		std::unique_ptr<MCI_BGM_Player> m_upMCIPlayer;
	};

}

