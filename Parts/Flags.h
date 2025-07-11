#pragma once
#include <type_traits>	//for std::underlying_type

/// <summary>
/// 要素がビットフラッグな enum class の組み合わせヘルパ．
/// 
/// とりあえずフラグの組み合わせを作るための OR のみ実装：
/// Flags( XXX::A | XXX::B | XXX::C );
/// とか書ける感じ．
/// </summary>
/// <typeparam name="Enum_t">euum classの型</typeparam>
template< class Enum_t >
class Flags
{
public:
	using Val_t = std::underlying_type_t<Enum_t>;

	constexpr Flags() : m_Val(0) {}
	constexpr Flags( Enum_t v ) : m_Val( static_cast<Val_t>(v) ) {}
	constexpr Flags( const Flags<Enum_t> &rhs ) : m_Val( rhs.m_Val ) {}
	
	constexpr Flags<Enum_t> &operator |=( const Flags<Enum_t> &rhs )
	{
		m_Val |= rhs.m_Val;
		return *this;
	}

	/// <summary>フラグが立っているか否か</summary>
	/// <param name="flag">チェック対象フラグ</param>
	/// <returns>対象フラグが立っているか否か</returns>
	constexpr bool Has( Enum_t flag ) const
	{	return ( m_Val & static_cast<Val_t>(flag) );	}

private:
	Val_t m_Val;
};

template <typename Enum_t>
constexpr Flags<Enum_t> operator |( const Flags<Enum_t> &lhs, const Flags<Enum_t> &rhs )
{	return Flags<Enum_t>{ lhs } |= rhs;	}

template <typename Enum_t>
constexpr Flags<Enum_t> operator |( const Flags<Enum_t> &lhs, Enum_t rhs )
{	return lhs | Flags<Enum_t>(rhs);	}

template <typename Enum_t>
constexpr Flags<Enum_t> operator |( Enum_t lhs, Enum_t rhs )
{	return Flags<Enum_t>(lhs) | Flags<Enum_t>(rhs);	}
