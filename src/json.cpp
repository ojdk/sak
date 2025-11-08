#include "json_impl.hpp"

#include <sak/json.hpp>

namespace sak {

//! JSON iterator implementation
//!
json_iterator::json_iterator( sak::json_impl &obj )
    : m_impl( std::make_unique< sak::json_iterator_impl >( obj.m_json.begin( ),
                                                           obj.m_json.end( ) ) )
{
}

sak::json_iterator::json_iterator( sak::json_iterator const &other )
    : m_impl( std::make_unique< sak::json_iterator_impl >( other.m_impl->cur,
                                                           other.m_impl->end ) )
{
}

sak::json_iterator::~json_iterator( ) = default;

bool sak::json_iterator::operator!=( const sak::json_iterator &other ) const
{
  return m_impl->cur != other.m_impl->cur;
}

sak::json_iterator &sak::json_iterator::operator++( )
{
  m_impl->increment( );
  return *this;
}

sak::json_ref &sak::json_iterator::operator*( )
{
  return m_impl->cur_ref;

  // return sak::json_ref{
  //   std::make_unique< sak::json_ref_impl >( *( m_impl->cur ) ) };
}

//! const_json_iterator class implementation

const_json_iterator::const_json_iterator( sak::json_impl &obj )
    : m_impl( std::make_unique< sak::const_json_iterator_impl >(
        obj.m_json.begin( ), obj.m_json.end( ) ) )
{
}

sak::const_json_iterator::const_json_iterator(
  sak::const_json_iterator const &other )
    : m_impl( std::make_unique< sak::const_json_iterator_impl >(
        other.m_impl->cur, other.m_impl->end ) )
{
}

sak::const_json_iterator::~const_json_iterator( ) = default;

bool sak::const_json_iterator::operator!=(
  const sak::const_json_iterator &other ) const
{
  return m_impl->cur != other.m_impl->cur;
}

sak::const_json_iterator &sak::const_json_iterator::operator++( )
{
  m_impl->increment( );
  return *this;
}

sak::const_json_ref sak::const_json_iterator::operator*( ) const
{
  return sak::const_json_ref{
    std::make_unique< sak::const_json_ref_impl >( *( m_impl->cur ) ) };
}

//! json class implementation

json::json( )
    : m_impl( std::make_unique< json_impl >( ) )
{
}

json::json( std::istream &&file )
    : m_impl( std::make_unique< json_impl >( std::move( file ) ) )
{
}

json::json( json const &other )
    : m_impl( std::make_unique< json_impl >( other.dump( ) ) )
{
}

json::json( json &&other ) noexcept
    : m_impl( std::move( other.m_impl ) )
{
}

json::json( std::string const &json_str ) noexcept
    : m_impl( std::make_unique< json_impl >( json_str ) )
{
}

json::~json( ) noexcept = default;

auto json::operator=( json const &other ) noexcept -> json &
{
  if ( this != &other ) {
    m_impl = std::make_unique< json_impl >( other.dump( ) );
  }
  return *this;
}

auto json::operator=( json &&other ) noexcept -> json &
{
  if ( this != &other ) {
    m_impl = std::move( other.m_impl );
  }
  return *this;
}

auto json::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( property_name );
}

auto json::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( property_name );
}

auto json::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( property_name );
}

auto json::getString( ) const -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( );
}

auto json::getNumber( ) const -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( );
}

auto json::getInteger( ) const -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( );
}

auto json::setString( std::string const property_name,
                      std::string const &value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

auto json::setNumber( std::string const property_name, double value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

auto json::setInteger( std::string const property_name, long long value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

auto json::begin( ) -> sak::json_iterator
{
  return sak::json_iterator( *m_impl );
}

auto json::end( ) -> sak::json_iterator
{

  sak::json_iterator iter( *m_impl );
  iter.m_impl->cur = iter.m_impl->end;
  return iter;
}

auto json::begin( ) const -> const_json_iterator
{
  return sak::const_json_iterator( *m_impl );
}

auto json::end( ) const -> const_json_iterator
{
  sak::const_json_iterator iter( *m_impl );
  iter.m_impl->cur = iter.m_impl->end;
  return iter;
}

auto json::dump( ) const -> std::string { return m_impl->dump( ); }

//! json_ref class implementation

json_ref::json_ref( std::unique_ptr< sak::json_ref_impl > &&d )
    : m_impl{ std::move( d ) }
{
}

json_ref::json_ref( json_ref const &other )
    : m_impl{ std::make_unique< sak::json_ref_impl >( other.m_impl->m_json ) }
{
}

json_ref::~json_ref( ) = default;

auto json_ref::operator=( sak::json_ref const &other ) -> sak::json_ref &
{
  if ( this != &other ) {
    if ( other.m_impl )
      m_impl = std::make_unique< sak::json_ref_impl >( other.m_impl->m_json );
    else
      m_impl = nullptr;
  }
  return *this;
}

auto json_ref::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( property_name );
}

auto json_ref::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( property_name );
}

auto json_ref::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( property_name );
}

auto json_ref::getString( ) const
  -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( );
}

auto json_ref::getNumber( ) const -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( );
}

auto json_ref::getInteger( ) const
  -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( );
}

auto json_ref::getObject( std::string const property_name )
  -> sak::expected< sak::json_ref, sak::json::error >
{
  auto query = m_impl->haveObject( property_name );
  if ( query.has_value( ) )
    return json_ref{ std::make_unique< sak::json_ref_impl >(
      m_impl->getObject( property_name ) ) };

  return query.error( );
}

auto json_ref::getObject( ) -> sak::expected< sak::json_ref, sak::json::error >
{
  auto query = m_impl->isObject( );
  if ( query.has_value( ) )
    return json_ref{
      std::make_unique< sak::json_ref_impl >( m_impl->getObject( ) ) };

  return query.error( );
}

auto json_ref::setString( std::string const property_name,
                          std::string const &value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

auto json_ref::setNumber( std::string const property_name, double value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

auto json_ref::setInteger( std::string const property_name, long long value )
  -> sak::expected< void, sak::json::error >
{
  return m_impl->setValue( property_name, value );
}

//! const_json_ref class implementation

const_json_ref::const_json_ref(
  std::unique_ptr< sak::const_json_ref_impl > &&d )
    : m_impl{ std::move( d ) }
{
}

const_json_ref::~const_json_ref( ) = default;

auto const_json_ref::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( property_name );
}

auto const_json_ref::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( property_name );
}

auto const_json_ref::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( property_name );
}

auto const_json_ref::getString( ) const
  -> sak::expected< std::string, sak::json::error >
{
  return m_impl->getString( );
}

auto const_json_ref::getNumber( ) const
  -> sak::expected< double, sak::json::error >
{
  return m_impl->getNumber( );
}

auto const_json_ref::getInteger( ) const
  -> sak::expected< long long, sak::json::error >
{
  return m_impl->getInteger( );
}

} // namespace sak