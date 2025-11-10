/**
 * @file IJsonValidater.hpp
 * @brief Interface for JSON validation functionality.
 * @author Ole Jørgensen
 * @date 2025-03-31
 */

#pragma once

#include <sak/details.hpp>
#include <sak/json/json.hpp>

namespace sak {

class IJsonValidater {
public:
  virtual ~IJsonValidater( ) = default;

public:
  virtual auto validate( sak::json const &json,
                         sak::SchemaPath const &schemapath ) const noexcept
    -> bool = 0;
  virtual auto validate( sak::json const &json,
                         sak::json const &root_schema ) const noexcept
    -> bool = 0;
};

} // namespace sak
