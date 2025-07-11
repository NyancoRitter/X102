#pragma once

namespace Toyger
{
	/// <summary>�Q�[�������������͏�Ԃ𓾂��i</summary> 
	/// <typeparam name="UINT_T">�L�[������ԗ���ێ��p�̕����Ȃ������^�i�����f�[�^��bit���ύX�p�j</typeparam>
	template< typename UINT_T = unsigned char >
	class IInputState
	{
	public:
		virtual ~IInputState(){}
	public:
		using KeyHistValType = UINT_T;
	public:
		/// <summary>�}�E�X�J�[�\�� X �ʒu�i�N���C�A���g�̈���W�j</summary>
		virtual int MouseX() const = 0;
		/// <summary>�}�E�X�J�[�\�� Y �ʒu�i�N���C�A���g�̈���W�j</summary>
		virtual int MouseY() const = 0;

		/// <summary>�L�[���邢�̓}�E�X�{�^���̉�����ԗ���</summary>
		/// <param name="VKeyCode">�Ώۂ̃L�[�܂��̓}�E�X�{�^���̉��z�L�[�R�[�h</param>
		/// <returns>
		/// UINT_T �^��bit�����̏�ԗ����D
		/// bit�������Ă���΁u������Ă���/�����v���Ƃ������D
		/// ���bit �قǌÂ��C����bit�قǐV�����D�i���ԂƋ��ɍ��V�t�g����Ă����j
		/// 	<remarks>�Ⴆ�΁C����l�̉���2bit�� b'01 �ł���΁u�V���������ꂽ�v�Ɣ��f�ł���D</remarks>
		/// </returns>
		virtual UINT_T KeyStateHist( unsigned char VKeyCode ) const = 0;

		/// <summary>�E�B���h�E����悤�Ƃ��鑀�삪�����ꂽ���ۂ�</summary>
		/// <returns>
		/// �E�B���h�E����鑀�삪�����ꂽ�i�����Ă��̑��쎩�̂͂��̎��_�ł͈���Ԃ��ꂽ�j�ꍇ�ɂ�true��Ԃ��D
		///		<remarks>���͏�ԃf�[�^���X�V���ꂽ�ۂɁC���̃��\�b�h�̖߂�l��false�Ƀ��Z�b�g���꓾��D</remarks>
		///</returns>
		virtual bool IsWndCloseOperationOccur() const = 0;
	};

	//---
	//�w���p�֐�
	
	//�L�[���邢�̓}�E�X�{�^����������Ă��邩
	template< class UINT_T >
	inline bool Pressed( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return IS.KeyStateHist(VKeyCode) & UINT_T{0x01};	}

	//�L�[���邢�̓}�E�X�{�^����������Ă��Ȃ���Ԃ��牟����Ă����ԂɂȂ�����
	template< class UINT_T >
	inline bool PosEdge( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return ( (IS.KeyStateHist(VKeyCode) & UINT_T{0x03}) == UINT_T(0x01) );	}

	//�L�[���邢�̓}�E�X�{�^����������Ă����Ԃ��牟����Ă��Ȃ���ԂɂȂ�����
	template< class UINT_T >
	inline bool NegEdge( const IInputState<UINT_T> &IS, unsigned char VKeyCode ){	return ( (IS.KeyStateHist(VKeyCode) & UINT_T{0x03}) == UINT_T(0x02) );	}
}
