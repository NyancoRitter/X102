#include "framework.h"
#include "Toyger_ScrollBarHandling.h"
#include <algorithm>

namespace Toyger
{

	void ScrollBarHandling::ChangeContentSize(int W, int H)
	{
		if( m_ContentW != W )
		{
			m_ContentW = W;
			UpdateScrollBar( true );
		}
		if( m_ContentH != H )
		{
			m_ContentH = H;
			UpdateScrollBar( false );
		}
	}

	void ScrollBarHandling::ChangeViewSize(int W, int H)
	{
		if( m_ViewW != W )
		{
			m_ViewW = W;
			UpdateScrollBar( true );
		}
		if( m_ViewH != H )
		{
			m_ViewH = H;
			UpdateScrollBar( false );
		}
	}

	void ScrollBarHandling::UpdateScrollBar( bool bHorizonalBar )
	{
		SCROLLINFO Info;
		Info.cbSize = sizeof( SCROLLINFO );
		Info.fMask = SIF_ALL;
		GetSI( bHorizonalBar, Info );
		Info.nMin = 0;
		Info.nMax = ( bHorizonalBar ? m_ContentW : m_ContentH ) - 1;
		Info.nPage = ( bHorizonalBar ? m_ViewW : m_ViewH );
		if( m_bShowDisableScrollBar ){	Info.fMask |= SIF_DISABLENOSCROLL;	}
		SetSI( bHorizonalBar, Info );
	}

	void ScrollBarHandling::HandleScrollEvent( bool HorizontalBar, UINT nSBCode, UINT nPos ) const
	{
		SCROLLINFO Info;
		Info.cbSize = sizeof( SCROLLINFO );
		Info.fMask = SIF_POS;
		GetSI( HorizontalBar, Info );

		int NewPos = Info.nPos;

		switch( nSBCode )
		{
			//���{�^��
		case SB_LINEUP:	--NewPos;	break;
		case SB_LINEDOWN:	++NewPos;	break;
			//�X���C�_�Ɩ��{�^���̊Ԃ̌���
		case SB_PAGEUP:	NewPos -= std::max( ( HorizontalBar ? m_ContentH : m_ContentH )/8, 1 );	break;
		case SB_PAGEDOWN:	NewPos += std::max( ( HorizontalBar ? m_ContentH : m_ContentH )/8, 1 );	break;
			//�X���C�_�h���b�O
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			NewPos=nPos;	break;
			//�[�܂ňړ�
		case SB_TOP:	NewPos = 0;	break;
		case SB_BOTTOM:	NewPos = ( HorizontalBar ? m_ContentW : m_ContentH );	break;
			//���m
		default:	return;
		}

		Info.nPos = NewPos;
		SetSI( HorizontalBar, Info );
	}

	//�X�N���[���o�[�̌��݂̃X�N���[���ʒu���擾
	int ScrollBarHandling::GetScrollPos( bool HorizontalBar ) const
	{
		SCROLLINFO Info;
		Info.cbSize = sizeof( SCROLLINFO );
		Info.fMask = SIF_POS;
		GetSI( HorizontalBar, Info );
		return Info.nPos;
	}

	//�X�N���[���o�[�̃X�N���[���ʒu��ݒ�D
	bool ScrollBarHandling::SetScrollPos( bool HorizontalBar, int Pos ) const
	{
		int BeforePos = GetScrollPos( HorizontalBar );
		if( BeforePos == Pos )return false;

		SCROLLINFO Info;
		Info.cbSize = sizeof( SCROLLINFO );
		Info.fMask = SIF_POS;
		Info.nPos = Pos;
		SetSI( HorizontalBar, Info );

		return ( GetScrollPos( HorizontalBar ) != BeforePos );
	}

	//�w��ʒu���Ȃ�ׂ��\���撆���ɗ���悤�Ȉʒu�ɃX�N���[������
	bool ScrollBarHandling::ScrollTo( int cx, int cy ) const
	{
		bool XChanged = SetScrollPos( true, std::clamp( cx - m_ViewW/2, 0, m_ContentW-1 ) );
		bool YChanged = SetScrollPos( false, std::clamp( cy - m_ViewH/2, 0, m_ContentH-1 ) );
		return (XChanged || YChanged );
	}

}