/**
 * @file json.hpp
 * @brief JSON wrapper for handling JSON data and operations in the application.
 * @author Ole Jørgensen
 * @date 2025-03-31
 */

#pragma once

#include <sak/expected.hpp>
#include <sak/sak_api.hpp>

#include <istream>
#include <memory>
#include <string>

namespace sak {

struct json_impl;
struct json_iterator;
struct const_json_iterator;

class json {
public:
  enum class error { unknown_property, wrong_type };

public:
  SAK_API explicit json( );
  SAK_API explicit json( std::istream &&file );
  SAK_API json( json const & );
  SAK_API json( json && ) noexcept;
  SAK_API explicit json( std::string const &json_str ) noexcept;

  SAK_API ~json( ) noexcept;

  SAK_API auto operator=( json const & ) noexcept -> json &;
  SAK_API auto operator=( json && ) noexcept -> json &;

public:
  SAK_API auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  SAK_API auto getString( ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( ) const
    -> sak::expected< long long, sak::json::error >;

public:
  SAK_API auto setString( std::string const property_name,
                          std::string const &value )
    -> sak::expected< void, sak::json::error >;
  SAK_API auto setNumber( std::string const property_name, double value )
    -> sak::expected< void, sak::json::error >;
  SAK_API auto setInteger( std::string const property_name, long long value )
    -> sak::expected< void, sak::json::error >;

public:
  SAK_API auto begin( ) -> sak::json_iterator;
  SAK_API auto end( ) -> sak::json_iterator;

  SAK_API auto begin( ) const -> sak::const_json_iterator;
  SAK_API auto end( ) const -> sak::const_json_iterator;

public:
  SAK_API auto dump( ) const -> std::string;

private:
  std::unique_ptr< json_impl > m_impl;
};

//! json_ref class

struct json_ref_impl;

class json_ref {
public:
  json_ref( std::unique_ptr< sak::json_ref_impl > &&d );
  json_ref( json_ref const &other );
  ~json_ref( );

  auto operator=( sak::json_ref const &other ) -> sak::json_ref &;

public:
  SAK_API
  auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  SAK_API auto getString( ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( ) const
    -> sak::expected< long long, sak::json::error >;

  SAK_API auto getObject( std::string const property_name )
    -> sak::expected< sak::json_ref, sak::json::error >;
  SAK_API auto getObject( ) -> sak::expected< sak::json_ref, sak::json::error >;

  SAK_API auto setString( std::string const property_name,
                          std::string const &value )
    -> sak::expected< void, sak::json::error >;
  SAK_API auto setNumber( std::string const property_name, double value )
    -> sak::expected< void, sak::json::error >;
  SAK_API auto setInteger( std::string const property_name, long long value )
    -> sak::expected< void, sak::json::error >;

private:
  std::unique_ptr< sak::json_ref_impl > m_impl;
};

//! JSON  iterator

struct json_iterator_impl;

struct json_iterator {

  friend class sak::json;

  SAK_API json_iterator( sak::json_impl & );
  SAK_API json_iterator( sak::json_iterator const & );
  SAK_API ~json_iterator( );

  SAK_API bool operator!=( const sak::json_iterator &other ) const;
  SAK_API sak::json_iterator &operator++( );
  SAK_API sak::json_ref & operator*( );

private:
  std::unique_ptr< sak::json_iterator_impl > m_impl;
};

//! const_json_ref class

struct const_json_ref_impl;

class const_json_ref {
public:
  const_json_ref( std::unique_ptr< sak::const_json_ref_impl > &&d );
  ~const_json_ref( );

public:
  SAK_API
  auto getString( std::string const property_name ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( std::string const property_name ) const
    -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( std::string const property_name ) const
    -> sak::expected< long long, sak::json::error >;

  SAK_API auto getString( ) const
    -> sak::expected< std::string, sak::json::error >;
  SAK_API auto getNumber( ) const -> sak::expected< double, sak::json::error >;
  SAK_API auto getInteger( ) const
    -> sak::expected< long long, sak::json::error >;

private:
  std::unique_ptr< sak::const_json_ref_impl > m_impl;
};

//! JSON const iterator

struct const_json_iterator_impl;

struct const_json_iterator {

  friend class sak::json;

  SAK_API const_json_iterator( sak::json_impl & );
  SAK_API const_json_iterator( sak::const_json_iterator const & );
  SAK_API ~const_json_iterator( );

  SAK_API bool operator!=( const sak::const_json_iterator &other ) const;
  SAK_API sak::const_json_iterator &operator++( );
  SAK_API sak::const_json_ref operator*( ) const;

private:
  std::unique_ptr< sak::const_json_iterator_impl > m_impl;
};

} // namespace sak
