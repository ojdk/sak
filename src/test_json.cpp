#include <gtest/gtest.h>

#include <sak/json.hpp>

#include <string>

using namespace std::string_literals;

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