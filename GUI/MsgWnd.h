#pragma once

#include "IGUI.h"
#include <memory>
#include <queue>
#include <string>
#include "Parts/Rect.h"

namespace GUI
{
	class TextLinePainter;

	/// <summary>
	/// �V���v���ȃ��b�Z�[�W�E�B���h�E
	/// 
	/// * �w�肳�ꂽ�e�L�X�g��ctor�Ŏw�肳�ꂽ�s�����\������
	/// * �`���F
	///		* �e�L�X�g�\����͘g�ň͂܂��
	///			* �`���́CX�����Ɋւ��Ă͏�ɒ������낦�ƂȂ�DY�����Ɋւ��Ă�ctor�Ŏw��D
	///		* �\���ɑ���������ꍇ�ɂ͂��̎|���������߂́����_�ŕ`�悳���
	/// </summary>
	class MsgWnd : public IGUI
	{
	public:
		//�\���̐����ʒu�w��p
		enum class VerticalLocation{	TOP, CENTER, BOTTOM	};

	public:
		/// <summary>ctor�D</summary>
		/// <param name="nLinesPerPage">��x�ɕ\������s��</param>
		/// <param name="VerticalLoc">Y�����̈ʒu�̎w��</param>
		/// <param name="Width">�`�敝[pixel]�D0�ȉ����w�肵���ꍇ�ɂ͂����悻���E�����S����߂�T�C�Y�ƂȂ�</param>
		MsgWnd(
			int nLinesPerPage = 3,
			VerticalLocation VerticalLoc=VerticalLocation::BOTTOM,
			int Width = 0
		);

		~MsgWnd();

	public:
		//�e�L�X�g�̉������ʒu�w��D�i������� *this ��Ԃ��j
		MsgWnd &AlignCenter();
		MsgWnd &AlignLeft();
		MsgWnd &AlignRight();

		/// <summary>
		/// �\�����ׂ�������Q(UTF16)���w��
		/// </summary>
		/// <typeparam name="LineIter"></typeparam>
		/// <param name="begin"></param>
		/// <param name="end"></param>
		/// <returns>*this</returns>
		template< class LineIter >
		MsgWnd &TextLines( LineIter begin, LineIter end )
		{
			std::queue<std::wstring> NewLines;
			for( LineIter i=begin; i!=end; ++i ){	NewLines.emplace(*i);	}
			m_RestLines.swap( NewLines );

			ToNextPage();
			return *this;
		}

		/// <summary>�\�����ׂ�������Q(UTF16)���w��</summary>
		/// <param name="LinesU16"></param>
		/// <returns>*this</returns>
		MsgWnd &TextLines( std::initializer_list<std::wstring> LinesU16 )
		{	return TextLines( LinesU16.begin(), LinesU16.end() );	}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void ToNextPage();
	private:
		std::unique_ptr<TextLinePainter> m_upTextPainter;
		Rect m_FrameRect;
		int m_nLinesPerPage = 1;
		std::queue<std::wstring> m_RestLines;	//�܂��\�����Ă��Ȃ��e�L�X�g�s�Q

		//�e�L�X�g�ɑ��������邱�Ƃ������}�[�N�̕\���p
		int m_NextPageIndicatorAnimCounter = 0;
		bool m_bDispNextPageIndicator = false;
		static constexpr int ms_NextPageIndicatorAnimInterval = 12;
	};
}
