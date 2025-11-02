#include "json_impl.hpp"

namespace sak {

namespace {

auto _string( nlohmann::json &m_json, std::string const &property_name )
  -> sak::expected< std::string, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_string( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< std::string >( );
}

auto _string( nlohmann::json &m_json )
  -> sak::expected< std::string, sak::json::error >
{
  if ( !m_json.is_string( ) )
    return sak::json::error::wrong_type;
  return m_json.get< std::string >( );
}

auto _number( nlohmann::json &m_json, std::string const &property_name )
  -> sak::expected< double, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_number_float( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< double >( );
}

auto _number( nlohmann::json &m_json )
  -> sak::expected< double, sak::json::error >
{

  if ( !m_json.is_number_float( ) )
    return sak::json::error::wrong_type;
  return m_json.get< double >( );
}

auto _integer( nlohmann::json &m_json, std::string const &property_name )
  -> sak::expected< long long, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_number_integer( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< long long >( );
}

auto _integer( nlohmann::json &m_json )
  -> sak::expected< long long, sak::json::error >
{
  if ( !m_json.is_number_integer( ) )
    return sak::json::error::wrong_type;
  return m_json.get< long long >( );
}

// const

auto _string( nlohmann::json const &m_json, std::string const &property_name )
  -> sak::expected< std::string, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_string( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< std::string >( );
}

auto _string( nlohmann::json const &m_json )
  -> sak::expected< std::string, sak::json::error >
{
  if ( !m_json.is_string( ) )
    return sak::json::error::wrong_type;
  return m_json.get< std::string >( );
}

auto _number( nlohmann::json const &m_json, std::string const &property_name )
  -> sak::expected< double, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_number_float( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< double >( );
}

auto _number( nlohmann::json const &m_json )
  -> sak::expected< double, sak::json::error >
{

  if ( !m_json.is_number_float( ) )
    return sak::json::error::wrong_type;
  return m_json.get< double >( );
}

auto _integer( nlohmann::json const &m_json, std::string const &property_name )
  -> sak::expected< long long, sak::json::error >
{
  if ( !m_json.contains( property_name ) )
    return sak::json::error::unknown_property;
  if ( !m_json[ property_name ].is_number_integer( ) )
    return sak::json::error::wrong_type;
  return m_json[ property_name ].get< long long >( );
}

auto _integer( nlohmann::json const &m_json )
  -> sak::expected< long long, sak::json::error >
{
  if ( !m_json.is_number_integer( ) )
    return sak::json::error::wrong_type;
  return m_json.get< long long >( );
}

} // namespace

json_impl::json_impl( ) = default;

json_impl::json_impl( std::istream &&file )
    : m_json( nlohmann::json::parse( file ) )
{
}

json_impl::json_impl( sak::json_impl const &other ) = default;

json_impl::json_impl::json_impl( sak::json_impl &&other ) = default;

json_impl::json_impl( std::string const &json_str )
    : m_json( nlohmann::json::parse( json_str ) )
{
}

sak::json_impl &json_impl::operator=( sak::json_impl const &other ) = default;
sak::json_impl &json_impl::operator=( sak::json_impl &&other ) = default;

json_impl::~json_impl( ) = default;

auto json_impl::dump( ) const -> std::string { return m_json.dump( ); }

auto json_impl::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json, property_name );
}

auto json_impl::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return _number( m_json, property_name );
}

auto json_impl::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{

  return _integer( m_json, property_name );
}

auto json_impl::getString( ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json );
}

auto json_impl::getNumber( ) const -> sak::expected< double, sak::json::error >
{
  return _number( m_json );
}

auto json_impl::getInteger( ) const
  -> sak::expected< long long, sak::json::error >
{
  return _integer( m_json );
}

//!

void json_iterator_impl::increment( )
{
  if ( cur != end ) {
    ++cur;
  }
}

json_ref_impl::json_ref_impl( nlohmann::json &json )
    : m_json( json )
{
}

auto json_ref_impl::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json, property_name );
}

auto json_ref_impl::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return _number( m_json, property_name );
}

auto json_ref_impl::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{
  return _integer( m_json, property_name );
}

auto json_ref_impl::getString( ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json );
}

auto json_ref_impl::getNumber( ) const
  -> sak::expected< double, sak::json::error >
{
  return _number( m_json );
}

auto json_ref_impl::getInteger( ) const
  -> sak::expected< long long, sak::json::error >
{
  return _integer( m_json );
}

auto json_ref_impl::getObject( std::string const property_name )
  -> nlohmann::json &
{
  return m_json.at( property_name );
}

auto json_ref_impl::getObject( ) -> nlohmann::json & { return m_json; }

auto json_ref_impl::haveObject( std::string const property_name )
  -> sak::expected< bool, sak::json::error >
{
  return m_json.contains( property_name ) &&
         m_json[ property_name ].is_object( );
}

auto json_ref_impl::isObject( ) -> sak::expected< bool, sak::json::error >
{
  return m_json.is_object( );
}

//! JSON iterator implementation

void const_json_iterator_impl::increment( )
{
  if ( cur != end ) {
    ++cur;
  }
}

const_json_ref_impl::const_json_ref_impl( nlohmann::json const &json )
    : m_json( json )
{
}

auto const_json_ref_impl::getString( std::string const property_name ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json, property_name );
}

auto const_json_ref_impl::getNumber( std::string const property_name ) const
  -> sak::expected< double, sak::json::error >
{
  return _number( m_json, property_name );
}

auto const_json_ref_impl::getInteger( std::string const property_name ) const
  -> sak::expected< long long, sak::json::error >
{
  return _integer( m_json, property_name );
}

auto const_json_ref_impl::getString( ) const
  -> sak::expected< std::string, sak::json::error >
{
  return _string( m_json );
}

auto const_json_ref_impl::getNumber( ) const
  -> sak::expected< double, sak::json::error >
{
  return _number( m_json );
}

auto const_json_ref_impl::getInteger( ) const
  -> sak::expected< long long, sak::json::error >
{
  return _integer( m_json );
}

} // namespace sak