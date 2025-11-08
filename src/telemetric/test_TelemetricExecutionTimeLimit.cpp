#include <gtest/gtest.h>

#include "test_mock_TelemetricDevice.hpp"

#include <sak/expected.hpp>
#include <sak/telemetric/ITelemetricDevice.hpp>
#include <sak/telemetric/TelemetricExecutionTimeLimit.hpp>

// fixture for testing TelemetricExecutionTimeLimit
struct TelemetricExecutionTimeLimitFixture : public ::testing::Test {
  TelemetricExecutionTimeLimitFixture( ) {}

  sak::test::mock_EnabledTelemetricDevice enabled_device;
  sak::test::mock_DisabledTelemetricDevice disabled_device;
  sak::TelemetricId id{ 1 };
  sak::TelemetricIdentifier identifier{ "test" };

  void member_function( )
  {
    internal_counter++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  }

  void member_function_no_sleep( ) { internal_counter++; }

  unsigned internal_counter = 0;
};

// free function to be used in tests
void free_function_sleep( unsigned &count )
{
  count++;
  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
}
void free_function_no_sleep( unsigned &count ) { count++; }

// Tests for TelemetricExecutionTimeLimit
TEST_F( TelemetricExecutionTimeLimitFixture, run_lambda )
{
  unsigned count = 0;
  auto func = [ & ]( ) { count++; };

  sak::TelemetricExecutionTimeLimit( sak::TelemetricInfo{ id, identifier },
                                     std::chrono::milliseconds( 1000 ),
                                     enabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_lambda_sleep_enabled )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };
  sak::TelemetricExecutionTimeLimit( sak::TelemetricInfo{ id, identifier },
                                     std::chrono::milliseconds( 1000 ),
                                     enabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_lambda_sleep_disabled )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };
  sak::TelemetricExecutionTimeLimit( sak::TelemetricInfo{ id, identifier },
                                     std::chrono::milliseconds( 1000 ),
                                     disabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_member_function )
{
  sak::TelemetricExecutionTimeLimit(
    this, sak::TelemetricInfo{ id, identifier },
    std::chrono::milliseconds( 1000 ), enabled_device,
    &TelemetricExecutionTimeLimitFixture::member_function_no_sleep );
  EXPECT_EQ( internal_counter, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_member_function_sleep )
{
  sak::TelemetricExecutionTimeLimit(
    this, sak::TelemetricInfo{ id, identifier },
    std::chrono::milliseconds( 1000 ), enabled_device,
    &TelemetricExecutionTimeLimitFixture::member_function );
  EXPECT_EQ( internal_counter, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_lambda_throw )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    throw std::runtime_error( "test" );
  };

  EXPECT_ANY_THROW( sak::TelemetricExecutionTimeLimit(
    sak::TelemetricInfo{ id, identifier }, std::chrono::milliseconds( 1000 ),
    enabled_device, func ) );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_free_function_no_sleep )
{
  unsigned count = 0;

  sak::TelemetricExecutionTimeLimit(
    sak::TelemetricInfo{ id, identifier }, std::chrono::milliseconds( 1000 ),
    enabled_device, free_function_no_sleep, count );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TelemetricExecutionTimeLimitFixture, run_free_function_sleep )
{
  unsigned count = 0;

  sak::TelemetricExecutionTimeLimit(
    sak::TelemetricInfo{ id, identifier }, std::chrono::milliseconds( 1000 ),
    enabled_device, free_function_sleep, count );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}