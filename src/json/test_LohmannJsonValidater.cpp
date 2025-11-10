#include <gtest/gtest.h>

#include "LohmannJsonValidater.hpp"

#include <sak/json/json.hpp>

#include <filesystem>
#include <iostream>
#include <source_location>

namespace sak {

struct LohmnannJsonValidaterTest : public ::testing::Test {
  LohmnannJsonValidaterTest( )
      : schema_root_path{
          std::filesystem::path( std::source_location::current( ).file_name( ) )
            .parent_path( )
            .parent_path( ) /
          "../schemas" / "test" }

  {
    EXPECT_TRUE( std::filesystem::exists( schema_root_path ) )
      << "Schema file does not exist: " << schema_root_path.string( );
  }

  std::filesystem::path schema_root_path;
  LohmannJsonValidater validator;
};

TEST_F( LohmnannJsonValidaterTest, ValidateValidJson )
{
  sak::json valid_json( R"({"name": "John", "age": 30})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_TRUE( is_valid ) << "validation failed";
}

TEST_F( LohmnannJsonValidaterTest, InValidateValidJson_bad_name )
{
  sak::json valid_json( R"({"name": 12, "age": 30})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_FALSE( is_valid ) << "validation failed";
}

TEST_F( LohmnannJsonValidaterTest, InValidateValidJson_bad_age )
{
  sak::json valid_json( R"({"name": "John", "age": "30"})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_FALSE( is_valid ) << "validation failed";
}

TEST_F( LohmnannJsonValidaterTest, InValidateValidJson_no_name )
{
  sak::json valid_json( R"({ "age": 30})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_FALSE( is_valid ) << "validation failed";
}

TEST_F( LohmnannJsonValidaterTest, InValidateValidJson_no_age )
{
  sak::json valid_json( R"({"name": "John"})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_FALSE( is_valid ) << "validation failed";
}

TEST_F( LohmnannJsonValidaterTest, InValidateValidJson_extra )
{
  sak::json valid_json( R"({"name": "John","age":30,"extra":"not allowed"})" );

  bool is_valid = true;
  EXPECT_NO_THROW( {
    is_valid = validator.validate(
      valid_json, sak::SchemaPath{ schema_root_path / "test_schema.json" } );
  } )
    << "validation crashed";

  EXPECT_FALSE( is_valid ) << "validation failed";
}

} // namespace sak
