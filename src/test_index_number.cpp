#include <gtest/gtest.h>

#include <sak/Index.hpp>

using IntIndex = sak::Index< int >;

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
