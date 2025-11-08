#pragma once

#include <sak/StrongType.hpp>
#include <sak/details.hpp>

#include <chrono>
#include <string_view>

namespace sak {

using TelemetricId = sak::StrongType< uint64_t, struct TelemetricIdTag >;
using TelemetricIdentifier =
  sak::StrongType< std::string, struct TelemetricIdentifierTag >;

struct TelemetricInfo {
  TelemetricInfo( sak::TelemetricId const &id,
                  sak::TelemetricIdentifier const &identifier )
      : id( id )
      , identifier( identifier )
  {
  }
  TelemetricInfo( sak::TelemetricInfo const & ) = default;
  TelemetricInfo( sak::TelemetricInfo && ) = default;
  sak::TelemetricInfo &operator=( sak::TelemetricInfo const & ) = default;
  sak::TelemetricInfo &operator=( sak::TelemetricInfo && ) = default;
  ~TelemetricInfo( ) = default;
  sak::TelemetricId id;
  sak::TelemetricIdentifier identifier;
};

class ITelemetricDevice {
public:
  virtual ~ITelemetricDevice( ) = default;

public:
  virtual auto IsEnabled( ) const -> bool = 0;
  virtual void ReportExecutionTimeViolation(
    sak::TelemetricInfo const &info,
    std::chrono::high_resolution_clock::duration const &duration ) = 0;

  virtual auto GetTelemetricId( ) const -> sak::TelemetricId = 0;
};

} // namespace sak