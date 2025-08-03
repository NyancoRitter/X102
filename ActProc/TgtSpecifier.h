#pragma once

/// <summary>
/// �L�����N�^�w��p�D
/// �p�[�e�B���̈ʒu�Ŏw�肷��D
/// </summary>
struct TgtSpecifier
{
	/// <summary>ctor</summary>
	/// <param name="Order">�p�[�e�B���ł̃L�����N�^�̈ʒu(0-based)</param>
	/// <param name="IsPartySide">�����p�[�e�B�Ȃ�true, �G�̏ꍇ�ɂ�false</param>
	TgtSpecifier( int Order, bool IsPartySide=true )
		: m_Order(Order), m_IsPartySide(IsPartySide)
	{}

	int m_Order;
	bool m_IsPartySide;
};
