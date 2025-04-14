#include <gtest/gtest.h>

#include <sak/StrongType.hpp>

#include <string>
#include <utility>

// Define a StrongType for std::string
using StrongString = sak::StrongType< std::string, struct StringTag >;

TEST( StrongTypeTest, StringInitialization )
{
  StrongString strongStr( "Hello, World!" );
  EXPECT_EQ( strongStr.value( ), "Hello, World!" );
}

TEST( StrongTypeTest, CopyConstructor )
{
  StrongString str1( "Initial" );
  StrongString str2( str1 );
  EXPECT_EQ( str1.value( ), "Initial" );
  EXPECT_EQ( str2.value( ), "Initial" );
}

TEST( StrongTypeTest, MoveConstructor )
{
  StrongString str1( "Initial" );
  StrongString str2( std::move( str1 ) );
  EXPECT_EQ( str2.value( ), "Initial" );
}

TEST( StrongTypeTest, MoveConstructorWithUniquePtr )
{
  StrongString str1( "Initial" );
  auto str2 = std::make_unique< StrongString >( std::move( str1 ) );
  EXPECT_EQ( str2->value( ), "Initial" );
}

TEST( StrongTypeTest, CopyAssignment )
{
  StrongString str1( "Initial" );
  StrongString str2( "Another" );
  str2 = str1;
  EXPECT_EQ( str1.value( ), "Initial" );
  EXPECT_EQ( str2.value( ), "Initial" );
}

TEST( StrongTypeTest, MoveAssignment )
{
  StrongString str1( "Initial" );
  StrongString str2( "Another" );
  str2 = std::move( str1 );
  EXPECT_EQ( str2.value( ), "Initial" );
}
TEST( StrongTypeTest, Comparison )
{
  StrongString str1( "Hello" );
  StrongString str2( "Hello" );
  StrongString str3( "World" );

  EXPECT_EQ( str1, str2 );
  EXPECT_NE( str1, str3 );
  EXPECT_LT( str1, str3 );
  EXPECT_GT( str3, str1 );
}

using StrongInt = sak::StrongType< int, struct IntTag >;

TEST( StrongTypeTest, IntInitialization )
{
  StrongInt strongInt( 42 );
  EXPECT_EQ( strongInt.value( ), 42 );
}

TEST( StrongTypeTest, IntCopyConstructor )
{
  StrongInt int1( 42 );
  StrongInt int2( int1 );
  EXPECT_EQ( int1.value( ), 42 );
  EXPECT_EQ( int2.value( ), 42 );
}
TEST( StrongTypeTest, IntMoveConstructor )
{
  StrongInt int1( 42 );
  StrongInt int2( std::move( int1 ) );
  EXPECT_EQ( int2.value( ), 42 );
}
TEST( StrongTypeTest, IntCopyAssignment )
{
  StrongInt int1( 42 );
  StrongInt int2( 100 );
  int2 = int1;
  EXPECT_EQ( int1.value( ), 42 );
  EXPECT_EQ( int2.value( ), 42 );
}
TEST( StrongTypeTest, IntMoveAssignment )
{
  StrongInt int1( 42 );
  StrongInt int2( 100 );
  int2 = std::move( int1 );
  EXPECT_EQ( int2.value( ), 42 );
}
TEST( StrongTypeTest, IntComparison )
{
  StrongInt int1( 42 );
  StrongInt int2( 42 );
  StrongInt int3( 100 );

  EXPECT_EQ( int1, int2 );
  EXPECT_NE( int1, int3 );
  EXPECT_LT( int1, int3 );
  EXPECT_GT( int3, int1 );
}