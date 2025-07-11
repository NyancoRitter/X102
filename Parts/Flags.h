#pragma once
#include <type_traits>	//for std::underlying_type

/// <summary>
/// �v�f���r�b�g�t���b�O�� enum class �̑g�ݍ��킹�w���p�D
/// 
/// �Ƃ肠�����t���O�̑g�ݍ��킹����邽�߂� OR �̂ݎ����F
/// Flags( XXX::A | XXX::B | XXX::C );
/// �Ƃ������銴���D
/// </summary>
/// <typeparam name="Enum_t">euum class�̌^</typeparam>
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

	/// <summary>�t���O�������Ă��邩�ۂ�</summary>
	/// <param name="flag">�`�F�b�N�Ώۃt���O</param>
	/// <returns>�Ώۃt���O�������Ă��邩�ۂ�</returns>
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
