#pragma once

/// <summary>
/// �L�����N�^�w��p�D�p�[�e�B���̈ʒu�Ŏw�肷��D
///		<remarks>
///		���̌^���g�����R�́C��ɕ\���n�̓s���D
///		�i�\���n�̏����ł͕\���ꏊ�̌���̂��߂Ɂu���Ԗڂ́`�v�Ƃ�����񂪗~�����j
///		</remarks>
/// </summary>
struct CharSpecifier
{
	/// <summary>ctor</summary>
	/// <param name="Order">�p�[�e�B���ł̃L�����N�^�̈ʒu(0-based)</param>
	/// <param name="IsPartySide">�����p�[�e�B�Ȃ�true, �G�̏ꍇ�ɂ�false</param>
	CharSpecifier( int Order, bool IsPartySide=true )
		: m_Order(Order), m_IsPartySide(IsPartySide)
	{}

	int m_Order;
	bool m_IsPartySide;
};
