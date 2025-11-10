#pragma once

#include <sak/details.hpp>
#include <sak/json/IJsonValidater.hpp>
#include <sak/json/json.hpp>

namespace sak {
class LohmannJsonValidater : public IJsonValidater {

public:
  LohmannJsonValidater( ) = default;
  ~LohmannJsonValidater( ) override = default;

  auto validate( sak::json const &json,
                 sak::SchemaPath const &schemapath ) const noexcept
    -> bool override;

  auto validate( sak::json const &json,
                 sak::json const &root_schema ) const noexcept -> bool override;
};

} // namespace sak