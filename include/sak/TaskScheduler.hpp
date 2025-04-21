#pragma once

#include <sak/ITelemetricDevice.hpp>
#include <sak/details.hpp>

#include <memory>

namespace sak {

enum class scheduler_error : unsigned short {
  no_task_available,
  task_failed,
  task_cancelled,
  task_timeout,
};

class TaskScheduler {
public:
  TaskScheduler( sak::ITelemetricDevice::uptr &&telemetric_controller );
  TaskScheduler( TaskScheduler const & ) = delete;
  TaskScheduler( TaskScheduler && ) = delete;
  TaskScheduler &operator=( TaskScheduler const & ) = delete;
  TaskScheduler &operator=( TaskScheduler && ) = delete;
  ~TaskScheduler( );

public:
  auto addTask( task_t &&task ) -> taskId_t;
  void pump( unsigned count_hint = 0 );

private:
  std::unique_ptr< struct TaskSchedulerImpl > m_impl;
};

} // namespace sak