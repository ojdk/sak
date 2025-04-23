#include <gtest/gtest.h>

#include <sak/expected.hpp>
#include <sak/telemetric/ITelemetricDevice.hpp>
#include <sak/telemetric/TelemetricTimer.hpp>

namespace sak {

struct mockTelemetricDevice : public sak::ITelemetricDevice {

  auto IsEnabled( ) const -> bool override { return true; }
};

void report_telemetric(
  sak::TelemetricInfo const &info, sak::ITelemetricDevice &device,
  std::chrono::high_resolution_clock::time_point const &start,
  std::chrono::high_resolution_clock::duration const &m_duration )
{
  auto end = std::chrono::high_resolution_clock::now( );
  auto duration = end - start;

  if ( duration > m_duration )
    std::cout << "Execution time: "
              << std::chrono::duration_cast< std::chrono::microseconds >(
                   duration )
                   .count( )
              << " microseconds" << std::endl;
}

template < typename Func, typename... Args >
auto TelemetricExecuteLimit( sak::TelemetricInfo const &info,
                             std::chrono::high_resolution_clock::duration limit,
                             sak::ITelemetricDevice &device, Func func,
                             Args... args ) -> void
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
auto TelemetricExecuteLimit( sak::TelemetricInfo const &info,
                             std::chrono::high_resolution_clock::duration limit,
                             sak::ITelemetricDevice &device, owner *o, Func &&f,
                             Args... args ) -> void
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

struct TimedTelemetricDevice : public ::testing::Test {
  TimedTelemetricDevice( )
      : device{ std::make_unique< sak::mockTelemetricDevice >( ) }
  {
  }

  std::unique_ptr< sak::ITelemetricDevice > device;
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

TEST_F( TimedTelemetricDevice, run_lambda )
{
  unsigned count = 0;
  auto func = [ & ]( ) { count++; };

  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ), *device,
                               func );
  EXPECT_EQ( count, 1 );
}

TEST_F( TimedTelemetricDevice, run_lambda_sleep )
{
  unsigned count = 0;
  auto func = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };
  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ), *device,
                               func );
  EXPECT_EQ( count, 1 );
}

TEST_F( TimedTelemetricDevice, run_member_function )
{
  sak::TelemetricExecuteLimit(
    sak::TelemetricInfo{ id, identifier }, std::chrono::milliseconds( 1000 ),
    *device, this, &TimedTelemetricDevice::member_function_no_sleep );
  EXPECT_EQ( internal_counter, 1 );
}

TEST_F( TimedTelemetricDevice, run_member_function_sleep )
{
  sak::TelemetricExecuteLimit( sak::TelemetricInfo{ id, identifier },
                               std::chrono::milliseconds( 1000 ), *device, this,
                               &TimedTelemetricDevice::member_function );
  EXPECT_EQ( internal_counter, 1 );
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
    *device, func ) );
  EXPECT_EQ( count, 1 );
}