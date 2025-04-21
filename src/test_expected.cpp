
#include <gtest/gtest.h>

#include <sak/expected.hpp>

#include <string>

TEST( ExpectedTest, DefaultConstructor_int )
{
  sak::expected< int, std::string > e{ 12 };
  EXPECT_TRUE( e.has_value( ) );
  EXPECT_EQ( e.value( ), 12 );
}

TEST( ExpectedTest, DefaultConstructor_string )
{
  sak::expected< std::string, int > e{ "test" };
  EXPECT_TRUE( e.has_value( ) );
  EXPECT_EQ( e.value( ), "test" );
}

TEST( ExpectedTest, ErrorConstructor_int )
{
  sak::expected< int, std::string > e{ "error" };
  EXPECT_FALSE( e.has_value( ) );
  EXPECT_EQ( e.error( ), "error" );
}

TEST( ExpectedTest, ErrorConstructor_void )
{
  sak::expected< void, int > e{ 12 };
  EXPECT_FALSE( e.has_value( ) );
}

TEST( ExpectedTest, ErrorConstructor_void_with_error )
{
  sak::expected< void, int > e{ 12 };
  EXPECT_FALSE( e.has_value( ) );
  EXPECT_EQ( e.error( ), 12 );
}

TEST( ExpectedTest, MoveConstructor_int )
{
  sak::expected< int, std::string > e{ 12 };
  sak::expected< int, std::string > e2{ std::move( e ) };
  EXPECT_TRUE( e2.has_value( ) );
  EXPECT_EQ( e2.value( ), 12 );
}

TEST( ExpectedTest, MoveConstructor_string )
{
  sak::expected< std::string, int > e{ "test" };
  sak::expected< std::string, int > e2{ std::move( e ) };
  EXPECT_TRUE( e2.has_value( ) );
  EXPECT_EQ( e2.value( ), "test" );
}

TEST( ExpectedTest, MoveConstructor_void )
{
  sak::expected< void, int > e{ 12 };
  sak::expected< void, int > e2{ std::move( e ) };
  EXPECT_FALSE( e2.has_value( ) );
  EXPECT_EQ( e2.error( ), 12 );
}

TEST( ExpectedTest, MoveConstructor_void_with_error )
{
  sak::expected< void, int > e{ 12 };
  sak::expected< void, int > e2{ std::move( e ) };
  EXPECT_FALSE( e2.has_value( ) );
  EXPECT_EQ( e2.error( ), 12 );
}

TEST( ExpectedTest, CopyConstructor_int )
{
  sak::expected< int, std::string > e{ 12 };
  sak::expected< int, std::string > e2{ e };
  EXPECT_TRUE( e2.has_value( ) );
  EXPECT_EQ( e2.value( ), 12 );
}

TEST( ExpectedTest, Constructor_none_default_constructable_value_copy )
{
  struct NonMoveConstructable {
    explicit NonMoveConstructable( int v )
        : value( v )
    {
    }
    NonMoveConstructable( const NonMoveConstructable & ) = default;
    NonMoveConstructable( NonMoveConstructable && ) = delete;
    ~NonMoveConstructable( ) = default;

    int value;
  };

  sak::expected< NonMoveConstructable, int > e( NonMoveConstructable( 12 ) );
  EXPECT_TRUE( e.has_value( ) );
  EXPECT_EQ( e.value( ).value, 12 );

  const sak::expected< NonMoveConstructable, int > ce(
    NonMoveConstructable( 13 ) );
  EXPECT_TRUE( ce.has_value( ) );
  EXPECT_EQ( ce.value( ).value, 13 );
}

TEST( ExpectedTest, Constructor_none_default_constructable_value_move )
{
  struct NonCopyConstructable {
    explicit NonCopyConstructable( int v )
        : value( v )
    {
    }
    NonCopyConstructable( const NonCopyConstructable & ) = delete;
    NonCopyConstructable( NonCopyConstructable && ) = default;
    ~NonCopyConstructable( ) = default;

    int value;
  };

  sak::expected< NonCopyConstructable, int > e( NonCopyConstructable( 12 ) );
  EXPECT_TRUE( e.has_value( ) );
  EXPECT_EQ( e.value( ).value, 12 );

  const sak::expected< NonCopyConstructable, int > ce(
    NonCopyConstructable( 13 ) );
  EXPECT_TRUE( ce.has_value( ) );
  EXPECT_EQ( ce.value( ).value, 13 );
}

TEST( ExpectedTest, Expect_unique_pointer )
{
  using uintptr = std::unique_ptr< int >;
  sak::expected< uintptr, int > e( 1 );
  EXPECT_FALSE( e.has_value( ) );
  EXPECT_EQ( e.error( ), 1 );

  const sak::expected< uintptr, int > ce( std::make_unique< int >( 13 ) );
  EXPECT_TRUE( ce.has_value( ) );
  EXPECT_EQ( *ce.value( ), 13 );
}
