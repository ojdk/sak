#pragma once

#include <sak/details.hpp>
#include <sak/telemetric/ITelemetricDevice.hpp>

namespace sak {

class TelemetricTimer {
public:
  TelemetricTimer( sak::ITelemetricDevice &telemetric_device,
                   sak::TelemetricInfo const &identifier );

private:
  TelemetricTimer( sak::TelemetricTimer const & ) = delete;

  TelemetricTimer( sak::TelemetricTimer && ) = delete;
  TelemetricTimer &operator=( sak::TelemetricTimer const & ) = delete;
  TelemetricTimer &operator=( sak::TelemetricTimer && ) = delete;
  ~TelemetricTimer( ) = default;

private:
  sak::ITelemetricDevice &m_telemetric_device;
  TelemetricInfo m_identifier;
};

} // namespace sak