#include <sak/telemetric/TelemetricExecutionTimeLimit.hpp>

#include <iostream>

namespace sak::details {
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
} // namespace sak::details