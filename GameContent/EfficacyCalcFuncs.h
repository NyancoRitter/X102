#pragma once
#include <functional>
#include <string>
#include "ActProc/ICharacter.h"
#include "Rnd.h"

namespace GameContent
{
	//�p�[�Z���g�l�Ȕ{����\�����邽�߂̕�����
	//�iPer �� 1 �ȏ�ł���z��j
	inline std::wstring PerStr( int Per )
	{
		if( Per < 10 ){	return L"0.0" + std::to_wstring(Per);	}

		auto d = std::div( Per, 100 );
		auto str = std::to_wstring(d.quot) + L"." + std::to_wstring(d.rem);
		const int n = (int)str.size();
		if( str[n-1]==L'0'  &&  str[n-2]!=L'.' )str.pop_back();	//"1.50" �̂悤�ȏꍇ�ɂ͖����� '0' ������
		return str;
	}

	//---------------------------------

	//STR�ˑ��З́i�{���Œ�j
	class STRx
	{
		int m_Per;
	public:
		STRx( int Per ) : m_Per(Per) {}
		double operator()( const ICharacter &C ) const {	return C.STR() * m_Per / 100.0;	}

		static std::wstring PowStr(int Per){	return L"(STR x " + PerStr(Per) + L")";	}
	};

	//STR�ˑ��З́i�{���͈́j
	class STRx_Range
	{
		int m_PerMin, m_PerMax;
	public:
		STRx_Range( int PerMin, int PerMax ) : m_PerMin(PerMin), m_PerMax(PerMax) {}
		double operator()( const ICharacter &C ) const 
		{	return C.STR() * RND().GetInt<int>( m_PerMin, m_PerMax ) /100.0;	}

		static std::wstring PowStr(int PerMin, int PerMax){	return L"(STR x " + PerStr(PerMin) + L"�`" + PerStr(PerMax) + L")";	}
	};

	//STR�ˑ� �З͔{��������Q�p�^�[��
	class STRx_HighLow
	{
		int m_PerLow, m_PerHigh;
		std::function<bool(const ICharacter &)> m_HighConditionFunc;
	public:
		STRx_HighLow( int PerLow, int PerHigh, std::function<bool(const ICharacter &)> HighConditionFunc )
			: m_PerLow(PerLow), m_PerHigh(PerHigh), m_HighConditionFunc(HighConditionFunc) {}

		double operator()( const ICharacter &C ) const 
		{	return C.STR() * ( m_HighConditionFunc(C)  ?  m_PerHigh  :  m_PerLow ) /100.0;	}

		static std::wstring PowStr( int PerLow, int PerHigh )
		{	return L"(STR x " + PerStr(PerLow) + L" or " + PerStr(PerHigh) + L")";	}
	};

	//MAG�ˑ��З́i�{���Œ�j
	class MAGx
	{
		int m_Per;
	public:
		MAGx( int Per ) : m_Per(Per) {}
		double operator()( const ICharacter &C ) const {	return C.MAG() * m_Per / 100.0;	}

		static std::wstring PowStr(int Per){	return L"(MAG x " + PerStr(Per) + L")";	}
	};

	//STR, MAG �����Ɉˑ�����З�
	class STRx_MAGx
	{
		int m_STR_Per, m_MAG_Per;
	public:
		STRx_MAGx( int STR_Per, int MAG_Per ) : m_STR_Per(STR_Per), m_MAG_Per(MAG_Per) {}
		double operator()( const ICharacter &C ) const
		{	return ( C.STR()*m_STR_Per + C.MAG()*m_MAG_Per ) /100.0;	}

		static std::wstring PowStr( int STR_Per, int MAG_Per ){	return L"(STR x " + PerStr(STR_Per) + L"  �{  MAG x " + PerStr(MAG_Per) + L")";	}
	};

	//�Œ�З�
	class Const
	{
		int m_Val;
	public:
		Const( int Val ) : m_Val(Val) {}
		double operator()( const ICharacter & ) const {	return m_Val;	}

		static std::wstring PowStr(int Val){	return std::to_wstring(Val);	}
	};

	//---------------------------------

	//�U�����ʐ����p������
	inline std::wstring AtkDescStr( const std::wstring &PowStr, AttackElement Elem, bool SingleTgt )
	{
		return std::wstring( SingleTgt ? L"[�P; " : L"[�S; " ) + NameStr(Elem) + PowStr + L"]";
	}

	//HP�񕜌��ʐ����p������
	inline std::wstring RecovHPDescStr( const std::wstring &PowStr, bool SingleTgt )
	{
		return std::wstring( SingleTgt ? L"[�P; HP+" : L"[�S; HP+" ) + PowStr + L"]";
	}

	//�ŉ񕜌��ʐ����p������
	inline std::wstring CurePoisonDescStr( bool SingleTgt )
	{	return ( SingleTgt ? L"[�P; ���" : L"[�S; ���" );	}
}
