#pragma once

#include <chrono>
#include <source_location>

namespace sak {

enum class logging_filter : unsigned short {
  none = 0,
  error = 1,
  warning = 2,
  info = 3,
  debug = 4,
};

struct LogContextData {
  std::source_location location;
  std::chrono::high_resolution_clock::time_point timePoint;
  sak::logging_filter filter;
};

} // namespace sak