#include <gtest/gtest.h>

#include <sak/Index.hpp>

using IntIndex = sak::Index< int >;
using IntNumber = sak::Number< int >;

TEST( sak_index, index_ctor_default )
{
  IntIndex index{ 0 };
  EXPECT_EQ( index.idx( ), 0 );
}

TEST( sak_index, index_ctor_max )
{
  IntIndex index{ std::numeric_limits< int >::max( ) };
}

TEST( sak_index, index_ctor_copy )
{
  IntIndex index{ 4 };
  EXPECT_EQ( index.idx( ), 4 );
  IntIndex index_copy{ index };
  EXPECT_EQ( index_copy.idx( ), 4 );
}

TEST( sak_index, index_ctor_move )
{
  IntIndex index{ 4 };
  EXPECT_EQ( index.idx( ), 4 );
  IntIndex index_move{ std::move( index ) };
  EXPECT_EQ( index_move.idx( ), 4 );
}

TEST( sak_index, index_assign_copy )
{
  IntIndex index{ 4 };
  EXPECT_EQ( index.idx( ), 4 );
  IntIndex index_copy = index;
  EXPECT_EQ( index_copy.idx( ), 4 );
}

TEST( sak_index, index_assign_move )
{
  IntIndex index{ 4 };
  EXPECT_EQ( index.idx( ), 4 );
  IntIndex index_move = std::move( index );
  EXPECT_EQ( index_move.idx( ), 4 );
}

TEST( sak_index, index_construct_number )
{
  IntNumber number{ 5 };
  IntIndex index{ number };
  EXPECT_EQ( index.idx( ), 4 );
}

TEST( sak_index, index_assign_number )
{
  IntIndex index{ 4 };
  EXPECT_EQ( index.idx( ), 4 );
  IntNumber number{ 6 };
  index = number;
  EXPECT_EQ( index.idx( ), 5 );
}

TEST( sak_index, index_compare_less )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 5 };
  EXPECT_TRUE( index1 < index2 );
  EXPECT_FALSE( index1 > index2 );
}

TEST( sak_index, index_compare_greater )
{
  IntIndex index1{ 5 };
  IntIndex index2{ 4 };
  EXPECT_TRUE( index1 > index2 );
  EXPECT_FALSE( index1 < index2 );
}
TEST( sak_index, index_compare_equal )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 4 };
  EXPECT_TRUE( index1 == index2 );
  EXPECT_FALSE( index1 != index2 );
}
TEST( sak_index, index_compare_not_equal )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 5 };
  EXPECT_TRUE( index1 != index2 );
  EXPECT_FALSE( index1 == index2 );
}

TEST( sak_index, index_compare_less_equal )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 5 };
  EXPECT_TRUE( index1 <= index2 );
  EXPECT_FALSE( index1 >= index2 );
}
TEST( sak_index, index_compare_greater_equal )
{
  IntIndex index1{ 5 };
  IntIndex index2{ 4 };
  EXPECT_TRUE( index1 >= index2 );
  EXPECT_FALSE( index1 <= index2 );
}
TEST( sak_index, index_compare_less_equal_equal )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 4 };
  EXPECT_TRUE( index1 <= index2 );
  EXPECT_TRUE( index1 >= index2 );
}
TEST( sak_index, index_compare_greater_equal_equal )
{
  IntIndex index1{ 4 };
  IntIndex index2{ 4 };
  EXPECT_TRUE( index1 >= index2 );
  EXPECT_TRUE( index1 <= index2 );
}
TEST( sak_index, index_valid )
{
  IntIndex index{ 4 };
  EXPECT_TRUE( index.valid( ) );
  IntIndex invalid_index{ std::numeric_limits< int >::max( ) };
  EXPECT_FALSE( invalid_index.valid( ) );
}
TEST( sak_index, index_number_convert )
{
  auto num_function = []( IntNumber num ) -> int { return num.number( ); };

  IntIndex index{ 4 };

  EXPECT_EQ( num_function( index ), 5 );

  EXPECT_EQ( num_function( IntIndex{ 0 } ), 1 );
  EXPECT_ANY_THROW(
    num_function( IntIndex{ std::numeric_limits< int >::max( ) } ) );
}
