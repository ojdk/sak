#include <sak/telemetric/TelemetricTimer.hpp>

#include <iostream>

namespace sak {

sak::TelemetricTimer::TelemetricTimer(
  sak::ITelemetricDevice &telemetric_device, TelemetricInfo const &identifier )
    : m_telemetric_device( telemetric_device )
    , m_identifier( identifier )
{
}

} // namespace sak