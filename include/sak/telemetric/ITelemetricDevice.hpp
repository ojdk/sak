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
  TelemetricInfo( TelemetricId const &id,
                  TelemetricIdentifier const &identifier )
      : id( id )
      , identifier( identifier )
  {
  }
  TelemetricInfo( TelemetricInfo const & ) = default;
  TelemetricInfo( TelemetricInfo && ) = default;
  TelemetricInfo &operator=( TelemetricInfo const & ) = default;
  TelemetricInfo &operator=( TelemetricInfo && ) = default;
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
};

} // namespace sak