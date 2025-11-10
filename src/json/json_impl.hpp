#pragma once

#include <sak/expected.hpp>
#include <sak/json/json.hpp>

#include <nlohmann/json.hpp>

#include <filesystem>

namespace sak {

struct json_impl {

  json_impl( );
  explicit json_impl( std::istream &&file );
  explicit json_impl( sak::json_impl const &other );
  explicit json_impl( sak::json_impl &&other );
  explicit json_impl( std::string const &json_str );

  json_impl &operator=( sak::json_impl const &other );
  json_impl &operator=( sak::json_impl &&other );

  ~json_impl( );

  auto dump( ) const -> std::string;

  auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  auto getString( ) const -> sak::expected< std::string, sak::json::error >;
  auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  auto getInteger( ) const -> sak::expected< long long, sak::json::error >;

  auto setValue( std::string const property_name, auto value )
    -> sak::expected< void, sak::json::error >
  {
    if ( !m_json.is_object( ) )
      return sak::json::error::wrong_type;
    m_json[ property_name ] = value;
    return { };
  }

  nlohmann::json m_json;
};

//! JSON iterator implementation

struct json_iterator_impl {

  explicit json_iterator_impl( nlohmann::json::iterator c,
                               nlohmann::json::iterator e );
  void increment( );

  nlohmann::json::iterator cur;
  nlohmann::json::iterator end;

  sak::json_ref cur_ref;
};

struct json_ref_impl {
  json_ref_impl( nlohmann::json &json );

  auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  auto getString( ) const -> sak::expected< std::string, sak::json::error >;
  auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  auto getInteger( ) const -> sak::expected< long long, sak::json::error >;

  auto getObject( std::string const property_name ) -> nlohmann::json &;
  auto getObject( ) -> nlohmann::json &;

  auto haveObject( std::string const property_name )
    -> sak::expected< bool, sak::json::error >;
  auto isObject( ) -> sak::expected< bool, sak::json::error >;

  auto setValue( std::string const property_name, auto value )
    -> sak::expected< void, sak::json::error >
  {
    if ( !m_json.is_object( ) )
      return sak::json::error::wrong_type;
    m_json[ property_name ] = value;
    return { };
  }

  nlohmann::json &m_json;
};

//! JSON const iterator implementation

struct const_json_iterator_impl {
  void increment( );

  nlohmann::json::const_iterator cur;
  nlohmann::json::const_iterator end;
};

struct const_json_ref_impl {
  const_json_ref_impl( nlohmann::json const &json );

  auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  auto getString( ) const -> sak::expected< std::string, sak::json::error >;
  auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  auto getInteger( ) const -> sak::expected< long long, sak::json::error >;

private:
  nlohmann::json const &m_json;
};

} // namespace sak