#pragma once

#include <memory>

namespace sak {

class ITelemetricDevice {
public:
  using uptr = std::unique_ptr< ITelemetricDevice >;

public:
  virtual ~ITelemetricDevice( ) = default;

  virtual void startTelemetry( ) = 0;
  virtual void stopTelemetry( ) = 0;
  virtual bool isTelemetryActive( ) const = 0;
};

} // namespace sak