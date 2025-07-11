#pragma once
#include "Toyger_IWndInputListener.h"
#include "Toyger_IInputState.h"

namespace Toyger
{
	/// <summary>
	///	IWndInputListener と IInputState の実装．
	/// ウィンドウに対する入力イベントの情報を収集し，ゲーム実装側に情報を提供する．
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
		/// <param name="OnWndCloseOP_RetVal">IWndInputListener::OnWndCloseOP()メソッドの戻り値とする値． </param>
		/// <remarks>※このctorが実行されるとき，いずれのキー/マウスボタンも押下中ではないことを前提としている</remarks>
		ConcreteInputState( bool OnWndCloseOP_RetVal )
			: m_MouseX(0), m_MouseY(0), m_WndCloseOPFlag(false), m_OnWndCloseOP_RetVal(OnWndCloseOP_RetVal)
		{	for(int i=0; i<256; ++i ){	m_History[i]=UINT_T(0);	}	}

		///<summary>用途：ゲームループ内でのゲーム処理後に呼ぶ</summary>
		void ToNextStep()
		{
			m_WndCloseOPFlag = false;
			for(int i=0; i<256; ++i ){	m_History[i] = (m_History[i]&UINT_T(0x01)) | (m_History[i]<<1);	}
		}

	public:	// IInputState 実装
		virtual int MouseX() const override {	return m_MouseX;	}
		virtual int MouseY() const override {	return m_MouseY;	}
		virtual bool IsWndCloseOperationOccur() const override {	return m_WndCloseOPFlag;	}

		/// <summary>キーあるいはマウスボタンの押下状態履歴</summary>
		/// <remarks>この履歴値は <see cref="ToNextStep"/> をコールする毎に上位bit側にシフト（左シフト）されていく．</remarks>
		virtual UINT_T KeyStateHist( unsigned char VKeyCode ) const override {	return m_History[VKeyCode];	}

	public:	// IWndInputListener 実装
		virtual void OnPress(unsigned char VKeyCode) override {	m_History[VKeyCode] |= UINT_T(0x01);	}
		virtual void OnRelease(unsigned char VKeyCode) override {	m_History[VKeyCode] &= ~(UINT_T(0x01));	}
		virtual void OnMouseMove(int x, int y) override {	m_MouseX=x;	m_MouseY=y;	}
		virtual bool OnWndCloseOP() override {	m_WndCloseOPFlag=true;	return m_OnWndCloseOP_RetVal;	}
	};

}
