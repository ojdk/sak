#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>

namespace sak {

struct json_impl {

  json_impl( );
  explicit json_impl( std::istream &&file );
  explicit json_impl( json_impl const &other );
  explicit json_impl( json_impl &&other );
  explicit json_impl( std::string const &json_str );

  json_impl &operator=( json_impl const &other );
  json_impl &operator=( json_impl &&other );

  ~json_impl( );

  auto dump( ) const -> std::string;

  nlohmann::json m_json;
};

} // namespace sak