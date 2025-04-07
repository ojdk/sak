#pragma once

#include <optional>
#include <type_traits>

namespace sak {

template < typename T, typename U >
class expected
{
public:
  expected( ) = delete;

  expected( const expected & ) = default;

  expected( expected &&other )
      : m_val( std::move( other.m_val ) )
      , m_err( std::move( other.m_err ) )
      , m_has_val( other.m_has_val )
  {
  }

  expected( const T &val )
    requires std::is_copy_constructible_v< T >
      : m_val( val )
      , m_has_val( true )
  {
  }

  expected( T &&val )
    requires std::is_move_constructible_v< T >
      : m_val( std::move( val ) )
      , m_has_val( true )
  {
  }

  expected( const U &err )
      : m_err( err )
      , m_has_val( false )
  {
  }
  expected( U &&err )
      : m_err( std::move( err ) )
      , m_has_val( false )
  {
  }

  ~expected( ) = default;

  expected &operator=( const expected & ) = delete;
  expected &operator=( expected && ) = delete;

  T &value( )
  {
      if ( !m_has_val ) {
        throw std::runtime_error( "No value" );
    }
    return m_val.value( );
  }

  const T &value( ) const
  {
      if ( !m_has_val ) {
        throw std::runtime_error( "No value" );
    }
    return m_val.value( );
  }

  U &error( )
  {
      if ( m_has_val ) {
        throw std::runtime_error( "No error" );
    }
    return m_err;
  }

  const U &error( ) const
  {
      if ( m_has_val ) {
        throw std::runtime_error( "No error" );
    }
    return m_err;
  }

  bool has_value( ) const { return m_has_val; }

private:
  std::optional< T > m_val;
  U m_err;
  bool m_has_val = false;
};

template < typename T, typename U >
  requires std::is_void_v< T >
class expected< T, U >
{
public:
  expected( )
      : m_has_val( true )
  {
  }
  expected( const expected & ) = delete;
  expected( expected &&other )
      : m_err( std::move( other.m_err ) )
      , m_has_val( other.m_has_val )
  {
  }

  expected &operator=( const expected & ) = delete;
  expected &operator=( expected && ) = delete;
  ~expected( ) = default;

  expected( const U &err )
      : m_err( err )
      , m_has_val( false )
  {
  }
  expected( U &&err )
      : m_err( std::move( err ) )
      , m_has_val( false )
  {
  }

  U &error( )
  {
      if ( m_has_val ) {
        throw std::runtime_error( "No error" );
    }
    return m_err;
  }

  const U &error( ) const
  {
      if ( m_has_val ) {
        throw std::runtime_error( "No error" );
    }
    return m_err;
  }

  bool has_value( ) const { return m_has_val; }

private:
  U m_err;
  bool m_has_val = false;
};
} // namespace sak