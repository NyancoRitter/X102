#pragma once
#include <functional>
#include <string>
#include "ActProc/ICharacter.h"
#include "Rnd.h"

namespace GameContent
{
	//パーセント値な倍率を表示するための文字列
	//（Per は 1 以上である想定）
	inline std::wstring PerStr( int Per )
	{
		if( Per < 10 ){	return L"0.0" + std::to_wstring(Per);	}

		auto d = std::div( Per, 100 );
		auto str = std::to_wstring(d.quot) + L"." + std::to_wstring(d.rem);
		const int n = (int)str.size();
		if( str[n-1]==L'0'  &&  str[n-2]!=L'.' )str.pop_back();	//"1.50" のような場合には末尾の '0' を消去
		return str;
	}

	//---------------------------------

	//STR依存威力（倍率固定）
	class STRx
	{
		int m_Per;
	public:
		STRx( int Per ) : m_Per(Per) {}
		double operator()( const ICharacter &C ) const {	return C.STR() * m_Per / 100.0;	}

		static std::wstring PowStr(int Per){	return L"(STR x " + PerStr(Per) + L")";	}
	};

	//STR依存威力（倍率範囲）
	class STRx_Range
	{
		int m_PerMin, m_PerMax;
	public:
		STRx_Range( int PerMin, int PerMax ) : m_PerMin(PerMin), m_PerMax(PerMax) {}
		double operator()( const ICharacter &C ) const 
		{	return C.STR() * RND().GetInt<int>( m_PerMin, m_PerMax ) /100.0;	}

		static std::wstring PowStr(int PerMin, int PerMax){	return L"(STR x " + PerStr(PerMin) + L"～" + PerStr(PerMax) + L")";	}
	};

	//STR依存 威力倍率が高低２パターン
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

	//MAG依存威力（倍率固定）
	class MAGx
	{
		int m_Per;
	public:
		MAGx( int Per ) : m_Per(Per) {}
		double operator()( const ICharacter &C ) const {	return C.MAG() * m_Per / 100.0;	}

		static std::wstring PowStr(int Per){	return L"(MAG x " + PerStr(Per) + L")";	}
	};

	//STR, MAG 両方に依存する威力
	class STRx_MAGx
	{
		int m_STR_Per, m_MAG_Per;
	public:
		STRx_MAGx( int STR_Per, int MAG_Per ) : m_STR_Per(STR_Per), m_MAG_Per(MAG_Per) {}
		double operator()( const ICharacter &C ) const
		{	return ( C.STR()*m_STR_Per + C.MAG()*m_MAG_Per ) /100.0;	}

		static std::wstring PowStr( int STR_Per, int MAG_Per ){	return L"(STR x " + PerStr(STR_Per) + L"  ＋  MAG x " + PerStr(MAG_Per) + L")";	}
	};

	//固定威力
	class Const
	{
		int m_Val;
	public:
		Const( int Val ) : m_Val(Val) {}
		double operator()( const ICharacter & ) const {	return m_Val;	}

		static std::wstring PowStr(int Val){	return std::to_wstring(Val);	}
	};

	//---------------------------------

	//攻撃効果説明用文字列
	inline std::wstring AtkDescStr( const std::wstring &PowStr, AttackElement Elem, bool SingleTgt )
	{
		return std::wstring( SingleTgt ? L"[単; " : L"[全; " ) + NameStr(Elem) + PowStr + L"]";
	}

	//HP回復効果説明用文字列
	inline std::wstring RecovHPDescStr( const std::wstring &PowStr, bool SingleTgt )
	{
		return std::wstring( SingleTgt ? L"[単; HP+" : L"[全; HP+" ) + PowStr + L"]";
	}

	//毒回復効果説明用文字列
	inline std::wstring CurePoisonDescStr( bool SingleTgt )
	{	return ( SingleTgt ? L"[単; 解毒" : L"[全; 解毒" );	}
}
