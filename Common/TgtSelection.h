#pragma once

#include "GUI/IGUI.h"
#include "Parts/Rect.h"
#include <vector>
#include <memory>
#include <functional>

namespace GUI
{
	/// <summary>
	/// ���炩�̕\�����̕��т����݂���Ƃ��ɁC�����̂����̂ǂꂩ��I��UI.
	/// ���ɕ`�悳��Ă���\�����̏�ɏd�˂�悤�ɑI���J�[�\����`�悷��D
	/// �i�\�����͏c���邢�͉������ɊT�ˈ��ɕ���ł���z��D�j
	///
	/// �퓬�V�[����p�[�e�B���j���[��ʂŃA�C�e�������g���Ώۂ�I������UI�̎����D
	/// 
	///  �@�\�F
	/// * �u�P�̂�ΏۂƂ���v�A�C�e�������g���Ώۂ�I�ԍۂɂ́C�O�p�`�̃J�[�\���𓮂����đI�ԁD
	/// * �u�S�̂�ΏۂƂ���v�A�C�e�������g���ꍇ�̕\���p�Ƃ��āC�S���ڂɎO�p�`�J�[�\����\������D
	/// </summary>
	class TgtSelection : public GUI::IGUI
	{
	public:
		/// <summary>ctor</summary>
		/// <param name="ItemRects">
		///	���ڌ��Ɗe���ڂ̕\���͈͏����w�肷��D
		/// �J�[�\���`��ʒu������Ɋ�Â��Č��߂���D
		/// ����w�肵�Ă͂Ȃ�Ȃ��i���w�肵���ꍇ�ɂ͗�O�𑗏o�j
		/// </param>
		/// <param name="ForAll">
		/// �u�S�̂�ΏۂƂ��Ă���v�̂��ۂ��D
		/// �P�̂�I������p�r�̏ꍇ�ɂ� false ���w�肷��D
		/// </param>
		/// <param name="IsVertical">
		/// ���ڕ��т��c�������Dfalse���w�肵���ꍇ�ɂ͉������D
		/// �J�[�\���ړ�������@�ƁC�J�[�\���`����@���ς��D
		/// </param>
		/// <param name="Callback">
		/// Update() ���őI�����葀�삠�邢�̓L�����Z���������ꂽ�ۂɌĂ΂��R�[���o�b�N�D
		/// �����́C
		///		* �I��(true) or �L�����Z��(false)
		///		* �J�[�\���ʒu�i0-based index, ������ ForAll ���w�肵���ꍇ�ɂ͕��̒l�j�D
		/// �߂�l�͂��̂܂� Update() ����̖߂�l�Ƃ��ėp������D
		/// </param>
		TgtSelection(
			const std::vector<Rect> &ItemRects,
			bool ForAll,
			bool IsVertical,
			const std::function< Flags<GUIResult>( bool, int ) > &Callback
		)
			: m_ItemRects( ItemRects )
			, m_IsForAll( ForAll )
			, m_IsVertical( IsVertical )
			, m_OnDecided( Callback )
		{
			if( m_ItemRects.empty() )throw std::exception( "ItemRects.empty() !!" );
		}

	public:	// IGUI Impl
		virtual Flags<GUIResult> Update( const IController &Controller ) override;
	protected:
		virtual void Paint_( HDC hdc ) const override;

	private:
		void DrawCursorFor( HDC hdc, const Rect &ItemRect ) const;
	private:
		const std::vector<Rect> m_ItemRects;
		const bool m_IsForAll;
		const bool m_IsVertical;
		std::function< Flags<GUI::GUIResult>( bool, int ) > m_OnDecided;

		int m_CursorPos = 0;

		//�J�[�\���A�j���[�V�����p
		unsigned char m_AnimCounter = 0;
		static constexpr unsigned char ms_AnimBit = 1<<3;
	};
}
