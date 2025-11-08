#pragma once

#include <sak/details.hpp>
#include <sak/telemetric/ITelemetricDevice.hpp>

namespace sak {
namespace details {
void report_telemetric(
  sak::TelemetricInfo const &info, sak::ITelemetricDevice &device,
  std::chrono::high_resolution_clock::time_point const &start,
  std::chrono::high_resolution_clock::duration const &m_duration );
} // namespace details

template < typename Func, typename... Args >
auto TelemetricExecutionTimeLimit(
  sak::TelemetricInfo const &info,
  std::chrono::high_resolution_clock::duration limit,
  sak::ITelemetricDevice &device, Func func, Args &...args ) -> void
{
  if ( !device.IsEnabled( ) )
    func( args... );
  else {
    auto start = std::chrono::high_resolution_clock::now( );
    try {
      func( args... );
      sak::details::report_telemetric( info, device, start, limit );
    } catch ( std::exception const &e ) {
      sak::details::report_telemetric( info, device, start, limit );
      throw e;
    }
  }
}

template < typename owner, typename Func, typename... Args >
auto TelemetricExecutionTimeLimit(
  owner *o, sak::TelemetricInfo const &info,
  std::chrono::high_resolution_clock::duration limit,
  sak::ITelemetricDevice &device, Func &&f, Args &...args ) -> void
{
  if ( !device.IsEnabled( ) )
    ( o->*f )( args... );
  else {
    auto start = std::chrono::high_resolution_clock::now( );
    try {
      ( o->*f )( args... );
      sak::details::report_telemetric( info, device, start, limit );
    } catch ( std::exception const &e ) {
      sak::details::report_telemetric( info, device, start, limit );
      throw e;
    }
  }
}

} // namespace sak
