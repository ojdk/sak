#pragma once

#include <istream>
#include <memory>
#include <string>

namespace sak {

class json
{
public:
  explicit json( );
  explicit json( std::istream &&file );
  explicit json( json const & ) noexcept;
  explicit json( json && ) noexcept;
  explicit json( std::string const &json_str ) noexcept;

  ~json( ) noexcept;

  auto operator=( json const & ) noexcept -> json &;
  auto operator=( json && ) noexcept -> json &;

public:
  auto dump( ) const -> std::string;

private:
  std::unique_ptr< struct json_impl > m_impl;
};

} // namespace sak
