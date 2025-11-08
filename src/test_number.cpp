#include <gtest/gtest.h>

#include <sak/Index.hpp>

using IntIndex = sak::Index< int >;
using IntNumber = sak::Number< int >;

TEST( sak_number, number_ctor_default )
{
  IntNumber number{ 1 };
  EXPECT_EQ( number.number( ), 1 );
}

TEST( sak_number, number_ctor_max )
{
  IntNumber number{ std::numeric_limits< int >::max( ) };
  EXPECT_EQ( number.number( ), std::numeric_limits< int >::max( ) );
}

TEST( sak_number, number_ctor_default_fail )
{
  IntNumber number{ 0 };
  EXPECT_ANY_THROW( number.number( ) );
}

TEST( sak_number, number_ctor_copy )
{
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  IntNumber number_copy{ number };
  EXPECT_EQ( number_copy.number( ), 5 );
}

TEST( sak_number, number_ctor_move )
{
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  IntNumber number_move{ std::move( number ) };
  EXPECT_EQ( number_move.number( ), 5 );
}

TEST( sak_number, number_assign_copy )
{
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  IntNumber number_copy = number;
  EXPECT_EQ( number_copy.number( ), 5 );
}

TEST( sak_number, number_assign_move )
{
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  IntNumber number_move = std::move( number );
  EXPECT_EQ( number_move.number( ), 5 );
}

TEST( sak_number, number_assign_index )
{
  IntIndex index{ 4 };
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  number = index;
  EXPECT_EQ( number.number( ), 5 );
}

TEST( sak_number, number_assign_index_fail )
{
  IntIndex index{ std::numeric_limits< int >::max( ) };
  IntNumber number{ 5 };
  EXPECT_EQ( number.number( ), 5 );
  number = index;
  EXPECT_ANY_THROW( number.number( ) );
}

TEST( sak_number, number_compare_equal )
{
  IntNumber number1{ 5 };
  IntNumber number2{ 5 };
  EXPECT_TRUE( number1 == number2 );
  EXPECT_FALSE( number1 != number2 );
}

TEST( sak_number, number_compare_not_equal )
{
  IntNumber number1{ 5 };
  IntNumber number2{ 6 };
  EXPECT_TRUE( number1 != number2 );
  EXPECT_FALSE( number1 == number2 );
}

TEST( sak_number, number_compare_less_equal )
{
  IntNumber number1{ 5 };
  IntNumber number2{ 6 };
  EXPECT_TRUE( number1 <= number2 );
  EXPECT_FALSE( number1 >= number2 );
}
TEST( sak_number, number_compare_greater_equal )
{
  IntNumber number1{ 6 };
  IntNumber number2{ 5 };
  EXPECT_TRUE( number1 >= number2 );
  EXPECT_FALSE( number1 <= number2 );
}
TEST( sak_number, number_compare_less_equal_equal )
{
  IntNumber number1{ 5 };
  IntNumber number2{ 5 };
  EXPECT_TRUE( number1 <= number2 );
  EXPECT_TRUE( number1 >= number2 );
}
TEST( sak_number, number_compare_greater_equal_equal )
{
  IntNumber number1{ 5 };
  IntNumber number2{ 5 };
  EXPECT_TRUE( number1 >= number2 );
  EXPECT_TRUE( number1 <= number2 );
}

TEST( sak_number, number_valid )
{
  IntNumber number{ 5 };
  EXPECT_TRUE( number.valid( ) );
  IntNumber invalid_number{ 0 };
  EXPECT_FALSE( invalid_number.valid( ) );
}

TEST( sak_number, number_validate )
{
  IntNumber number{ 5 };
  EXPECT_NO_THROW( number.number( ) );
  IntNumber invalid_number{ 0 };
  EXPECT_ANY_THROW( invalid_number.number( ) );
}

TEST( sak_number, number_index_convert )
{
  auto num_function = []( IntIndex num ) -> int { return num.idx( ); };

  IntNumber number{ 5 };

  EXPECT_EQ( num_function( number ), 4 );

  EXPECT_EQ( num_function( IntNumber{ 1 } ), 0 );
  EXPECT_ANY_THROW( num_function( IntNumber{ 0 } ) );
}
