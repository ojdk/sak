/**
 * @file Index.hpp
 * @brief Introduce a strongly checked index for type safety.
 * @author Ole Jørgensen
 * @date 2025-04-07
 */

#pragma once

#include <compare>
#include <concepts>
#include <limits>
#include <stdexcept>
#include <type_traits>
namespace sak {

template < typename T >
class Number;

template < typename T >
  requires std::integral< T >
class Index {
public:
  explicit Index( T value );
  Index( Index const &other ) = default;
  Index( Index &&other ) = default;

  auto operator=( Index const &other ) -> Index & = default;
  auto operator=( Index &&other ) -> Index & = default;

  Index( Number< T > const &other );
  Index( Number< T > &&other ) = delete;
  auto operator=( Number< T > const &other ) -> Index &;
  auto operator=( Number< T > &&other ) -> Index & = delete;

  auto operator<=>( Index const &other ) const = default;

  operator Number< T >( ) const;

public:
  auto idx( ) const -> T const &;
  auto valid( ) const -> bool;

private:
  void _valid( ) const;

private:
  T m_value;
};

// implementation of Index
template < typename T >
  requires std::integral< T >
Index< T >::Index( T value )
    : m_value( value )
{
}

template < typename T >
  requires std::integral< T >
Index< T >::Index( Number< T > const &other )
    : m_value( other.valid( ) ? other.value( )
                              : std::numeric_limits< T >::max( ) )
{
}

template < typename T >
  requires std::integral< T >
auto Index< T >::operator=( Number< T > const &other ) -> Index &
{
  m_value = other.valid( ) ? other.value( ) : std::numeric_limits< T >::max( );
  return *this;
}

template < typename T >
  requires std::integral< T >
Index< T >::operator Number< T >( ) const
{
  return Number< T >( m_value + 1 );
}

template < typename T >
  requires std::integral< T >
auto Index< T >::idx( ) const -> T const &
{
  _valid( );
  return m_value;
}

template < typename T >
  requires std::integral< T >
auto Index< T >::valid( ) const -> bool
{
  return m_value != std::numeric_limits< T >::max( );
}

template < typename T >
  requires std::integral< T >
void Index< T >::_valid( ) const
{
    if ( !valid( ) ) {
      throw std::out_of_range( "Index out of range" );
  }
}

} // namespace sak