#pragma once

#include <sak/telemetric/ITelemetricDevice.hpp>

#include <chrono>
#include <format>

namespace sak::test {

// Mock class for ITelemetricDevice
struct mock_base_TelemetricDevice : public sak::ITelemetricDevice {
  inline void ReportExecutionTimeViolation(
    sak::TelemetricInfo const &info,
    std::chrono::high_resolution_clock::duration const &duration ) override
  {
    report =
      std::format( "Id: {}. Identifier: {}. Duration: ", info.id.value( ),
                   info.identifier.value( ) );
  }

  inline auto GetTelemetricId( ) const -> sak::TelemetricId override
  {
    return sak::TelemetricId{ ++m_id };
  }

  std::string report;
  mutable uint64_t m_id{ 0 };
};

struct mock_EnabledTelemetricDevice : public mock_base_TelemetricDevice {
  inline auto IsEnabled( ) const -> bool override { return true; }
};

struct mock_DisabledTelemetricDevice : public mock_base_TelemetricDevice {
  inline auto IsEnabled( ) const -> bool override { return false; }
};

} // namespace sak::test