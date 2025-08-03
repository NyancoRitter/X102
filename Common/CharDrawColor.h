#pragma once

#include "GUI/Color.h"

constexpr COLORREF CharColor_Down = RGB( 255,0,0 );	//HP<=0 ���
constexpr COLORREF CharColor_PoisonInfected = RGB( 0,160,64 );	//�ŏ��

/// <summary>
/// �L�����N�^�[�̏�Ԃ���`��F������
/// </summary>
/// <param name="HP">����HP</param>
/// <param name="PosionInfected">�ŏ�Ԃ��ۂ�</param>
/// <returns>�F</returns>
inline COLORREF CharDrawColor( int HP, bool PosionInfected )
{
	if( HP<=0 )return CharColor_Down;
	if( PosionInfected )return CharColor_PoisonInfected;
	return GUI::Color::White;
}
