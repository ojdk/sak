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
  requires std::integral< T >
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

  // template < typename oT >
  //   requires std::integral< oT > && std::same_as< T, oT >
  // Index( Number< oT > const &other )
  //     : m_value( other.valid( ) ? other.number( ) - 1
  //                               : std::numeric_limits< T >::max( ) )
  // {
  // }

  template < typename oT >
    requires std::integral< oT > && std::same_as< T, oT >
  auto operator=( Number< oT > const &other ) -> Index< T > &
  {
    m_value =
      other.valid( ) ? other.number( ) - 1 : std::numeric_limits< T >::max( );
    return *this;
  }

  template < typename oT >
    requires std::integral< oT > && std::same_as< T, oT >
  operator Number< oT >( ) const
  {
      if ( !valid( ) ) {
        return Number< oT >( 0 );
    }
    return Number< oT >( m_value + 1 );
  }

  auto operator<=>( Index const &other ) const;
  auto operator==( Index const &other ) const;
  auto operator!=( Index const &other ) const;

public:
  auto idx( ) const -> T const &;
  auto valid( ) const -> bool;

  // private:
  //   template < typename ot >
  //     requires std::integral< ot > && std::same_as< T, ot >
  //   Index( Number< ot > &&other ) = delete;
  //   template < typename ot >
  //     requires std::integral< ot > && std::same_as< T, ot >
  //   auto operator=( Number< ot > &&other ) -> Index< T > & = delete;

private:
  void _validate( ) const;

private:
  T m_value;
};

template < typename T >
  requires std::integral< T >
class Number {
public:
  explicit Number( T value );
  Number( Number const &other ) = default;
  Number( Number &&other ) = default;

  auto operator=( Number const &other ) -> Number & = default;
  auto operator=( Number &&other ) -> Number & = default;

  // template < typename oT >
  //   requires std::integral< oT > && std::same_as< T, oT >
  // Number( Index< oT > const &other )
  //     : m_value( other.valid( ) ? other.idx( ) + 1 : 0 )
  // {
  // }

  template < typename oT >
    requires std::integral< oT > && std::same_as< T, oT >
  auto operator=( Index< oT > const &other ) -> Number< T > &
  {
    m_value = other.valid( ) ? other.idx( ) + 1 : 0;
    return *this;
  }

  template < typename oT >
    requires std::integral< oT > && std::same_as< T, oT >
  operator Index< oT >( ) const
  {
      if ( !valid( ) ) {
        return Index< oT >( std::numeric_limits< T >::max( ) );
    }
    return Index< oT >( m_value - 1 );
  }

  auto operator<=>( Number const &other ) const;
  auto operator==( Number const &other ) const;
  auto operator!=( Number const &other ) const;

public:
  auto number( ) const -> T const &;
  auto valid( ) const -> bool;

private:
  auto _validate( ) const;

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
auto Index< T >::operator<=>( Index const &other ) const
{
  _validate( );
  other._validate( );
  return m_value <=> other.m_value;
}

template < typename T >
  requires std::integral< T >
auto Index< T >::operator==( Index const &other ) const
{
  _validate( );
  other._validate( );
  return m_value == other.m_value;
}

template < typename T >
  requires std::integral< T >
auto Index< T >::operator!=( Index const &other ) const
{
  return !operator==( other );
}

template < typename T >
  requires std::integral< T >
auto Index< T >::idx( ) const -> T const &
{
  _validate( );
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
void Index< T >::_validate( ) const
{
    if ( !valid( ) ) {
      throw std::out_of_range( "Index out of range" );
  }
}

// implementation of Number

template < typename T >
  requires std::integral< T >
Number< T >::Number( T value )
    : m_value( value )
{
}

template < typename T >
  requires std::integral< T >
auto Number< T >::operator<=>( Number const &other ) const
{
  _validate( );
  other._validate( );
  return m_value <=> other.m_value;
}

template < typename T >
  requires std::integral< T >
auto Number< T >::operator==( Number const &other ) const
{
  _validate( );
  other._validate( );
  return m_value == other.m_value;
}

template < typename T >
  requires std::integral< T >
auto Number< T >::operator!=( Number const &other ) const
{
  return !operator==( other );
}

template < typename T >
  requires std::integral< T >
auto Number< T >::number( ) const -> T const &
{
  _validate( );
  return m_value;
}

template < typename T >
  requires std::integral< T >
auto Number< T >::valid( ) const -> bool
{
  return m_value != 0;
}

template < typename T >
  requires std::integral< T >
auto Number< T >::_validate( ) const
{
    if ( !valid( ) ) {
      throw std::out_of_range( "Number out of range" );
  }
}

} // namespace sak