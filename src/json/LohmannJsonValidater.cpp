#include "LohmannJsonValidater.hpp"

#include <nlohmann/json-schema.hpp>

#include <fstream>
#include <iostream>
namespace sak {

auto LohmannJsonValidater::validate(
  sak::json const &json, sak::SchemaPath const &schemapath ) const noexcept
  -> bool
{
  std::ifstream f( schemapath.value( ) );
  return validate( json, sak::json( std::move( f ) ) );
}

auto LohmannJsonValidater::validate(
  sak::json const &json, sak::json const &root_schema ) const noexcept -> bool
{
  nlohmann::json_schema::json_validator validator;

  validator.set_root_schema( nlohmann::json::parse( root_schema.dump( ) ) );

  try {
    validator.validate( nlohmann::json::parse( json.dump( ) ) );
    return true;
  } catch ( std::exception const &e ) {
    // Handle validation error
    std::cerr << "Validation error: " << e.what( ) << std::endl;
  }

  return false;
}

} // namespace sak