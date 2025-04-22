#include <gtest/gtest.h>

#include <sak/telemetric/ITelemetricDevice.hpp>
#include <sak/telemetric/TelemetricTimer.hpp>

namespace sak {

struct mockTelemetricDevice : public sak::ITelemetricDevice {

  auto IsEnabled( ) const -> bool override { return true; }
};

template < typename Func, typename... Args >
struct TTelemetricTimer {
  TTelemetricTimer( sak::TelemetricInfo const &info,
                    sak::ITelemetricDevice &device, Func func, Args... args )
  {
    if ( !device.IsEnabled( ) )
      func( args... );
    else {
      auto start = std::chrono::high_resolution_clock::now( );
      func( args... );
      auto end = std::chrono::high_resolution_clock::now( );
      auto duration =
        std::chrono::duration_cast< std::chrono::microseconds >( end - start );

      if ( duration > m_duration )
        std::cout << "Execution time: " << duration.count( ) << " microseconds"
                  << std::endl;
    }
  }
  template < typename owner >
  TTelemetricTimer( sak::TelemetricInfo const &info,
                    sak::ITelemetricDevice &device, owner *o, Func &&f,
                    Args... args )
  {
    if ( !device.IsEnabled( ) )
      ( o->*f )( args... );
    else {
      auto start = std::chrono::high_resolution_clock::now( );
      ( o->*f )( args... );
      auto end = std::chrono::high_resolution_clock::now( );
      auto duration =
        std::chrono::duration_cast< std::chrono::microseconds >( end - start );
      if ( duration > m_duration )

        std::cout << "Execution time: " << duration.count( ) << " microseconds"
                  << std::endl;
    }
  }

  const std::chrono::microseconds m_duration{ 1000 };
};

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
  auto func1 = [ & ]( ) { count++; };
  auto func2 = [ & ]( ) {
    count++;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
  };

  sak::TTelemetricTimer( sak::TelemetricInfo{ id, identifier }, *device,
                         func1 );
  EXPECT_EQ( count, 1 );

  sak::TTelemetricTimer( sak::TelemetricInfo{ id, identifier }, *device,
                         func2 );
  EXPECT_EQ( count, 2 );
}

TEST_F( TimedTelemetricDevice, run_member_function )
{
  sak::TTelemetricTimer( sak::TelemetricInfo{ id, identifier }, *device, this,
                         &TimedTelemetricDevice::member_function );
  EXPECT_EQ( internal_counter, 1 );

  sak::TTelemetricTimer{ sak::TelemetricInfo{ id, identifier }, *device, this,
                         &TimedTelemetricDevice::member_function_no_sleep };
  EXPECT_EQ( internal_counter, 2 );
}
