#include <gtest/gtest.h>

#include <sak/json/json.hpp>

#include <string>

using namespace std::string_literals;

namespace sak {

TEST( sak_json, json_ctor_default )
{
  sak::json json;
  EXPECT_EQ( json.dump( ), "null" );
}

TEST( sak_json, json_ctor_string )
{
  sak::json json{ R"({})"s };
  EXPECT_EQ( json.dump( ), "{}" );
}

TEST( sak_json, json_ctor_copy )
{
  sak::json json{ R"({})"s };
  EXPECT_EQ( json.dump( ), "{}" );
  sak::json json_copy{ json };
  EXPECT_EQ( json_copy.dump( ), "{}" );
}

TEST( sak_json, json_ctor_move )
{
  sak::json json{ R"({})"s };
  EXPECT_EQ( json.dump( ), "{}" );
  sak::json json_move{ std::move( json ) };
  EXPECT_EQ( json_move.dump( ), "{}" );
}

TEST( sak_json, json_assign_copy )
{
  sak::json json{ R"({})"s };
  EXPECT_EQ( json.dump( ), "{}" );
  sak::json json_copy;
  EXPECT_EQ( json_copy.dump( ), "null" );
  json_copy = json;
  EXPECT_EQ( json_copy.dump( ), "{}" );
}

TEST( sak_json, json_assign_move )
{
  sak::json json{ R"({})"s };
  EXPECT_EQ( json.dump( ), "{}" );
  sak::json json_move;
  EXPECT_EQ( json_move.dump( ), "null" );
  json_move = std::move( json );
  EXPECT_EQ( json_move.dump( ), "{}" );
}

TEST( sak_json, json_ctor_string_arrray )
{
  sak::json json{ R"([])"s };
  EXPECT_EQ( json.dump( ), "[]" );
}

TEST( sak_json, json_ctor_string_simple )
{
  sak::json json{ R"({"a":"b"})"s };
  EXPECT_EQ( json.dump( ), R"({"a":"b"})" );
}

TEST( sak_json, json_ctor_string_complex )
{
  sak::json json{ R"({"a":{"b":[1,2,3],"c":true},"d":null})"s };
  EXPECT_EQ( json.dump( ), R"({"a":{"b":[1,2,3],"c":true},"d":null})" );
}

//! getString tests

TEST( sak_json, json_getString_simple )
{
  sak::json json{ R"({"key":"value"})"s };
  EXPECT_EQ( json.getString( "key" ).value( ), "value" );
}

TEST( sak_json, json_getString_unkown_property )
{
  sak::json json{ R"({"key":"value"})"s };
  EXPECT_EQ( json.getString( "not existing property" ).error( ),
             sak::json::error::unknown_property );
}

TEST( sak_json, json_getString_invalid_type )
{
  sak::json json{ R"({"key":4.5})"s };
  EXPECT_EQ( json.getString( "key" ).error( ), sak::json::error::wrong_type );
}

//! getNumber

TEST( sak_json, json_getNumber_simple )
{
  sak::json json{ R"({"key":4.5})"s };
  EXPECT_EQ( json.getNumber( "key" ).value( ), 4.5 );
}

TEST( sak_json, json_getNumber_unkown_property )
{
  sak::json json{ R"({"key":"value"})"s };
  EXPECT_EQ( json.getNumber( "not existing property" ).error( ),
             sak::json::error::unknown_property );
}

TEST( sak_json, json_GetNumber_invalid_type )
{
  sak::json json{ R"({"key":"not a number"})"s };
  EXPECT_EQ( json.getNumber( "key" ).error( ), sak::json::error::wrong_type );
}

//! getInteger

TEST( sak_json, json_getInteger_simple )
{
  sak::json json{ R"({"key":4})"s };
  EXPECT_EQ( json.getInteger( "key" ).value( ), 4ll );
}

TEST( sak_json, json_getInteger_unkown_property )
{
  sak::json json{ R"({"key":"value"})"s };

  EXPECT_EQ( json.getInteger( "not existing property"s ).error( ),
             sak::json::error::unknown_property );
}

TEST( sak_json, json_getInteger_invalid_type )
{
  sak::json json{ R"({"key":"not a integer"})"s };
  EXPECT_EQ( json.getInteger( "key" ).error( ), sak::json::error::wrong_type );
}

TEST( sak_json, json_iterate_array_const )
{
  sak::json json{ R"([1, 2, 3, 4, 5])"s };
  long long i = 0;
  for ( auto const &item : json ) {
    EXPECT_EQ( item.getInteger( ).value( ), ++i );
  }
}

TEST( sak_json, json_iterate_array_none_const )
{
  sak::json json{ R"([1, 2, 3, 4, 5])"s };
  long long i = 0;
  for ( auto item : json ) {
    EXPECT_EQ( item.getInteger( ).value( ), ++i );
  }
}

TEST( sak_json, json_iterate_array_obj )
{
  sak::json json{ R"([{"name":"a"},{"name":"b"}])"s };
  long long i = 0;

  std::vector< std::string > val{ "a"s, "b"s };

  for ( auto & item : json ) {
    EXPECT_EQ( item.getObject( ).value( ).getString( "name" ).value( ),
               val[ i++ ] );
  }
}

TEST( sak_json, json_set_string )
{
  sak::json json{ R"({})"s };
  json.setString( "a", "b" );
  EXPECT_EQ( json.dump( ), R"({"a":"b"})" );
}

TEST( sak_json, json_set_number )
{
  sak::json json{ R"({})"s };
  json.setNumber( "a", 4.6 );
  EXPECT_EQ( json.dump( ), R"({"a":4.6})" );
}

TEST( sak_json, json_set_integer )
{
  sak::json json{ R"({})"s };
  json.setInteger( "a", 42 );
  EXPECT_EQ( json.dump( ), R"({"a":42})" );
}

} // namespace sak
