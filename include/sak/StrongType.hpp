/**
 * @file StrongType.hpp
 * @brief Strongly typed wrapper for type safety and clarity.
 * @author Ole Jørgensen
 * @date 2025-03-31
 */

#pragma once

#include <compare>

namespace sak {

template < typename T, typename trait_t >
class StrongType {
public:
  explicit StrongType( T value );
  explicit StrongType( StrongType const &other ) = default;
  explicit StrongType( StrongType &&other ) = default;
  auto operator=( StrongType const &other ) -> StrongType & = default;
  auto operator=( StrongType &&other ) -> StrongType & = default;
  auto value( ) const -> T const & { return m_value; }
  auto operator<=>( StrongType< T, trait_t > const &other ) const = default;

private:
  T m_value; // The underlying value of the strong type
};

// implementatio of StrongType constructor
template < typename T, typename trait_t >
StrongType< T, trait_t >::StrongType( T value )
    : m_value( value )
{
}

} // namespace sak
