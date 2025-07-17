#pragma once

#include "IPainter.h"
#include "Parts/Flags.h"
#include <list>
#include <memory>

class IController;

namespace GUI
{
	class IGUIStack;

	/// <summary>
	/// IGUI.Exec()�̖߂�l�p�r�b�g�t���O
	/// </summary>
	enum class GUIResult : unsigned int
	{
		/// <summary>���ɖ���</summary>
		None = 0,

		/// <summary>
		/// ����Stack����Pop�����ׂ����Ƃ������D
		/// </summary>
		ShouldPop =  1,

		/// <summary>
		/// �ĕ`�悪�K�v�ł��邱�Ƃ������D
		/// </summary>
		ReqRedraw = 1 << 1,
	};

	/// <summary>
	/// GUI�\�����D
	/// ���͂������ł��āC�󋵂�`��ł�����́D
	/// 
	/// GUIStack��Push���Ďg���^�D
	/// </summary>
	class IGUI : public IPainter
	{
	public:
		virtual ~IGUI() = default;
	public:
		/// <summary>�X�V����</summary>
		/// <param name="Stack">����Update���Ăяo����GUIStack�i�܂肱�̃C���X�^���X����������GUIStack�j�ւ̗v�f�ǉ���i</param>
		/// <param name="Controller">����</param>
		/// <returns>�ĕ`�悪�K�v���ۂ��D</returns>
		virtual Flags<GUIResult> Update( IGUIStack &Stack, const IController &Controller ) = 0;

		/// <summary>
		/// Update()���Ă΂��ΏۂɂȂ�����
		/// �i��������Stack�̑��̗v�f��Pop���ꂽ���ʂƂ��Ă����Ȃ����ꍇ�C���邢�͎��g��Stack��Push���ꂽ�ꍇ�j�ɌĂ΂��D
		/// </summary>
		virtual void OnGotFocus(){}

		/// <summary>
		/// Update()���Ă΂��Ώۂł͂Ȃ��Ȃ����� �i��������Stack�ɑ��v�f��Push���ꂽ�ꍇ�j�ɌĂ΂��D
		/// ���̃I�u�W�F�N�g�� Stack ���� Pop ���ꂽ�ۂɂ͌Ă΂�Ȃ��D
		/// </summary>
		virtual void OnLostFocus(){}
	};

	/// <summary>
	/// �X�^�b�N�ւ̗v�fPush��i
	/// </summary>
	class IGUIStack
	{
	public:
		virtual ~IGUIStack() = default;

		/// <summary>
		/// �X�^�b�N�ւ̗v�f�ǉ�
		/// </summary>
		/// <param name="upGUI">
		/// �ǉ�����v�f�D
		/// * nullptr ���w�肵���ꍇ�ɂ͉������Ȃ�
		/// * ����Ώۂ𑽏d�ɒǉ������ꍇ�̓���͕ۏ؂��Ȃ�
		/// </param>
		/// <returns>*this</returns>
		virtual IGUIStack &Push( std::unique_ptr<IGUI> upGUI ) = 0;
	};

	/// <summary>
	/// IGUI�̃X�^�b�N�I�ȁD�ȉ��̎d�������邾���̃R���e�i�݂����ȁD
	/// 
	/// * �Ō�� Push ���ꂽ���� Update() ���R�[������
	/// * Paint() �� Push ���ꂽ���S�Ă� Paint() �� Push ���ɃR�[������D
	/// </summary>
	class GUIStack : public IGUIStack
	{
	public:
		~GUIStack(){	clear();	}

	private:
		GUIStack( const GUIStack & ) = delete;
		GUIStack &operator =( const GUIStack & ) = delete;

	public:	// IGUIStack Impl
		virtual GUIStack &Push( const std::unique_ptr<IGUI> upGUI ) override
		{
			if( !upGUI )return *this;

			if( !m_GUIs.empty() )
			{	m_GUIs.back()->OnLostFocus();	}

			m_GUIs.push_back( std::move(upGUI) );
			m_GUIs.back()->OnGotFocus();
			return *this;
		}

	public:
		bool empty() const {	return m_GUIs.empty();	}
		void clear(){	m_GUIs.clear();	}

		/// <summary>
		/// �Ō�� Push ���ꂽ���� Update() ���R�[������
		/// </summary>
		/// <param name="Controller"></param>
		/// <returns>�ĕ`��i Draw() �̎��{�j���K�v���ۂ�</returns>
		bool Update( const IController &Controller )
		{
			if( empty() )return false;

			auto iCurr = std::prev( m_GUIs.end() );
			auto Result = (*iCurr)->Update( *this, Controller );

			bool NeedToRedraw = false;
			if( Result.Has( GUIResult::ShouldPop ) )
			{
				Remove( iCurr );
				NeedToRedraw = true;
			}
			if( Result.Has( GUIResult::ReqRedraw ) )
			{	NeedToRedraw = true;	}

			return NeedToRedraw;
		}

		/// <summary>�����v�f�Q�� Paint() �� Push ���ɃR�[������</summary>
		/// <param name="hdc"></param>
		void Paint( HDC hdc )
		{
			for( const auto &upGUI : m_GUIs )
			{	upGUI->Paint( hdc );	}
		}

	private:
		using Iter = std::list<   std::unique_ptr< IGUI >   >::iterator;

		/// <summary>
		/// �v�f�̏�����ƁD
		/// * �K�v�ɉ����� UIPart::OnGotFocus() ���R�[��
		/// * m_iPaintBegin�̍X�V
		/// </summary>
		/// <param name="iTgt">�폜�Ώ�</param>
		void Remove( Iter iTgt )
		{
			{//�v�f�폜
				bool TgtIsCurrent = ( iTgt == std::prev( m_GUIs.end() ) );
				m_GUIs.erase( iTgt );

				if( TgtIsCurrent && !m_GUIs.empty() )
				{	m_GUIs.back()->OnGotFocus();	}
			}
		}

	private:
		std::list<   std::unique_ptr< IGUI >   > m_GUIs;	//�Ō��Push���ꂽ���� back ��
	};
}
