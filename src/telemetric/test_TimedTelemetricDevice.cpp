#include <gtest/gtest.h>

#include <sak/expected.hpp>
#include <sak/telemetric/ITelemetricDevice.hpp>
#include <sak/telemetric/TelemetricTimer.hpp>

#include <format>

namespace sak {

void report_telemetric(
  sak::TelemetricInfo const &info, sak::ITelemetricDevice &device,
  std::chrono::high_resolution_clock::time_point const &start,
  std::chrono::high_resolution_clock::duration const &m_duration )
{
  auto end = std::chrono::high_resolution_clock::now( );
  auto duration = end - start;

  if ( duration > m_duration )
    device.ReportExecutionTimeViolation( info, duration );
}

template < typename Func, typename... Args >
auto TelemetricExecuteLimit( sak::TelemetricInfo const &info,
                             std::chrono::high_resolution_clock::duration limit,
                             sak::ITelemetricDevice &device, Func func,
                             Args &...args ) -> void
{
  if ( !device.IsEnabled( ) )
    func( args... );
  else {
    auto start = std::chrono::high_resolution_clock::now( );
    try {
      func( args... );
    } catch ( std::exception const &e ) {
      report_telemetric( info, device, start, limit );
      throw e;
    }
    report_telemetric( info, device, start, limit );
  }
}

template < typename owner, typename Func, typename... Args >
auto TelemetricExecuteLimit( owner *o, sak::TelemetricInfo const &info,
                             std::chrono::high_resolution_clock::duration limit,
                             sak::ITelemetricDevice &device, Func &&f,
                             Args &...args ) -> void
{
  if ( !device.IsEnabled( ) )
    ( o->*f )( args... );
  else {
    auto start = std::chrono::high_resolution_clock::now( );
    try {
      ( o->*f )( args... );
    } catch ( std::exception const &e ) {
      report_telemetric( info, device, start, limit );
      throw e;
    }
    report_telemetric( info, device, start, limit );
  }
}

} // namespace sak

struct mock_base_TelemetricDevice : public sak::ITelemetricDevice {
  void ReportExecutionTimeViolation(
    sak::TelemetricInfo const &info,
    std::chrono::high_resolution_clock::duration const &duration ) override
  {
    report =
      std::format( "Id: {}. Identifier: {}. Duration: ", info.id.value( ),
                   info.identifier.value( ) );
  }

  std::string report;
};

struct mock_EnabledTelemetricDevice : public mock_base_TelemetricDevice {
  auto IsEnabled( ) const -> bool override { return true; }
};

struct mock_DisabledTelemetricDevice : public mock_base_TelemetricDevice {
  auto IsEnabled( ) const -> bool override { return false; }
};

struct TimedTelemetricDevice : public ::testing::Test {
  TimedTelemetricDevice( ) {}

  mock_EnabledTelemetricDevice enabled_device;
  mock_DisabledTelemetricDevice disabled_device;
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

void free_function_sleep( unsigned &count )
{
  count++;
  std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
}
void free_function_no_sleep( unsigned &count ) { count++; }

TEST_F( TimedTelemetricDevice, run_lambda )
{
  unsigned count = 0;
  auto func = [ & ]( ) { count++; };

  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               enabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TimedTelemetricDevice, run_lambda_sleep_enabled )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };
  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               enabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}

TEST_F( TimedTelemetricDevice, run_lambda_sleep_disabled )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };
  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               disabled_device, func );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TimedTelemetricDevice, run_member_function )
{
  sak::TelemetricExecuteLimit(
    this, sak::TelemetricInfo{ id, identifier },
    std::chrono::milliseconds( 1000 ), enabled_device,
    &TimedTelemetricDevice::member_function_no_sleep );
  EXPECT_EQ( internal_counter, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TimedTelemetricDevice, run_member_function_sleep )
{
  sak::TelemetricExecuteLimit( this, sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               enabled_device,
                               &TimedTelemetricDevice::member_function );
  EXPECT_EQ( internal_counter, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}

TEST_F( TimedTelemetricDevice, run_lambda_throw )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    throw std::runtime_error( "test" );
  };

  EXPECT_ANY_THROW( sak::TelemetricExecuteLimit(
    sak::TelemetricInfo{ id, identifier }, std::chrono::milliseconds( 1000 ),
    enabled_device, func ) );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TimedTelemetricDevice, run_free_function_no_sleep )
{
  unsigned count = 0;

  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               enabled_device, free_function_no_sleep, count );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "" );
}

TEST_F( TimedTelemetricDevice, run_free_function_sleep )
{
  unsigned count = 0;

  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ),
                               enabled_device, free_function_sleep, count );
  EXPECT_EQ( count, 1 );
  EXPECT_EQ( enabled_device.report, "Id: 1. Identifier: test. Duration: " );
}