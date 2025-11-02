#pragma once

#include <sak/logging/LogContextData.hpp>

#include <string>

namespace sak {

class ILogSink {
public:
  virtual void log( sak::LogContextData &&contextData,
                    std::string const &message ) = 0;
  virtual ~ILogSink( ) = default;
};

} // namespace sak