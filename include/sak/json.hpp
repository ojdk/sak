/**
 * @file json.hpp
 * @brief JSON wrapper for handling JSON data and operations in the application.
 * @author Ole Jørgensen
 * @date 2025-03-31
 */

#pragma once

#include <sak/sak_api.hpp>

#include <istream>
#include <memory>
#include <string>

namespace sak {

class json {
public:
  SAK_API explicit json( );
  SAK_API explicit json( std::istream &&file );
  SAK_API explicit json( json const & ) noexcept;
  SAK_API explicit json( json && ) noexcept;
  SAK_API explicit json( std::string const &json_str ) noexcept;

  SAK_API ~json( ) noexcept;

  SAK_API auto operator=( json const & ) noexcept -> json &;
  SAK_API auto operator=( json && ) noexcept -> json &;

public:
  SAK_API auto dump( ) const -> std::string;

private:
  std::unique_ptr< struct json_impl > m_impl;
};

} // namespace sak
