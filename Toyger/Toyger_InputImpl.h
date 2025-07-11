#pragma once
#include "Toyger_IWndInputListener.h"
#include "Toyger_IInputState.h"

namespace Toyger
{
	/// <summary>
	///	IWndInputListener �� IInputState �̎����D
	/// �E�B���h�E�ɑ΂�����̓C�x���g�̏������W���C�Q�[���������ɏ���񋟂���D
	/// </summary>
	template< typename UINT_T = unsigned char >
	class ConcreteInputState : public IWndInputListener, public IInputState<UINT_T>
	{
	private:
		UINT_T m_History[256];
		int m_MouseX, m_MouseY;
		bool m_WndCloseOPFlag;
		const bool m_OnWndCloseOP_RetVal;

	public:
		/// <summary>ctor</summary>
		/// <param name="OnWndCloseOP_RetVal">IWndInputListener::OnWndCloseOP()���\�b�h�̖߂�l�Ƃ���l�D </param>
		/// <remarks>������ctor�����s�����Ƃ��C������̃L�[/�}�E�X�{�^�����������ł͂Ȃ����Ƃ�O��Ƃ��Ă���</remarks>
		ConcreteInputState( bool OnWndCloseOP_RetVal )
			: m_MouseX(0), m_MouseY(0), m_WndCloseOPFlag(false), m_OnWndCloseOP_RetVal(OnWndCloseOP_RetVal)
		{	for(int i=0; i<256; ++i ){	m_History[i]=UINT_T(0);	}	}

		///<summary>�p�r�F�Q�[�����[�v���ł̃Q�[��������ɌĂ�</summary>
		void ToNextStep()
		{
			m_WndCloseOPFlag = false;
			for(int i=0; i<256; ++i ){	m_History[i] = (m_History[i]&UINT_T(0x01)) | (m_History[i]<<1);	}
		}

	public:	// IInputState ����
		virtual int MouseX() const override {	return m_MouseX;	}
		virtual int MouseY() const override {	return m_MouseY;	}
		virtual bool IsWndCloseOperationOccur() const override {	return m_WndCloseOPFlag;	}

		/// <summary>�L�[���邢�̓}�E�X�{�^���̉�����ԗ���</summary>
		/// <remarks>���̗���l�� <see cref="ToNextStep"/> ���R�[�����閈�ɏ��bit���ɃV�t�g�i���V�t�g�j����Ă����D</remarks>
		virtual UINT_T KeyStateHist( unsigned char VKeyCode ) const override {	return m_History[VKeyCode];	}

	public:	// IWndInputListener ����
		virtual void OnPress(unsigned char VKeyCode) override {	m_History[VKeyCode] |= UINT_T(0x01);	}
		virtual void OnRelease(unsigned char VKeyCode) override {	m_History[VKeyCode] &= ~(UINT_T(0x01));	}
		virtual void OnMouseMove(int x, int y) override {	m_MouseX=x;	m_MouseY=y;	}
		virtual bool OnWndCloseOP() override {	m_WndCloseOPFlag=true;	return m_OnWndCloseOP_RetVal;	}
	};

}
