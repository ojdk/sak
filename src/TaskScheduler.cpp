#include <sak/TaskScheduler.hpp>
#include <sak/expected.hpp>

#include <iostream>
#include <list>
#include <mutex>

namespace sak {

struct task_data {
  task_t m_task;
  taskId_t m_id;
};

using task_expected = sak::expected< task_data, sak::scheduler_error >;

struct TaskSchedulerImpl {

  TaskSchedulerImpl( sak::ITelemetricDevice::uptr &&telemetric_device )
      : m_telemetric_device( std::move( telemetric_device ) )
  {
  }

  auto addTask( task_t &&task ) -> taskId_t
  {
    std::lock_guard lock( m_mutex );
    auto id = ++m_taskId;
    m_tasks.emplace_back( std::move( task ), id );
    return id;
  }

  void pump( unsigned count_hint )
  {
    unsigned count = 0;
    while ( true ) {
      if ( count_hint && count >= count_hint )
        break;

      auto task_data = _get_taskData( );
      if ( !task_data.has_value( ) )
        break;
      _execute_task_safely( task_data.value( ) );
      ++count;
    }
  }

  void _execute_task_safely( task_data const &data )
  {
    m_telemetric_device->startTelemetry( );
    try {
      data.m_task( );
    } catch ( std::exception const &e ) {
      // @todo : log error with upcoming logging framework
      std::cerr << "Error executing task: " << e.what( ) << std::endl;
    } catch ( ... ) {
      // @todo : log error with upcoming logging framework
      std::cerr << "Unknown error executing task" << std::endl;
    }
    m_telemetric_device->stopTelemetry( );
  }

  auto _get_taskData( ) noexcept -> task_expected
  {
    try {
      std::lock_guard lock( m_mutex );
      if ( m_tasks.empty( ) ) {
        return sak::scheduler_error::no_task_available;
      }
      auto it = m_tasks.begin( );
      auto task_data = std::move( *it );
      m_tasks.erase( it );
      return task_data;
    } catch ( std::exception const &e ) {
      // @todo: log error with upcoming logging framework
      std::cerr << "Error: " << e.what( ) << std::endl;
      return sak::scheduler_error::task_failed;
    } catch ( ... ) {
      // @todo: log error with upcoming logging framework
      std::cerr << "Unknown error occurred in _get_taskData" << std::endl;
      return sak::scheduler_error::task_failed;
    }
  }

  taskId_t m_taskId = 0;

  std::mutex m_mutex;
  std::list< task_data > m_tasks;
  sak::ITelemetricDevice::uptr m_telemetric_device;
};

TaskScheduler::TaskScheduler( sak::ITelemetricDevice::uptr &&telemetric_device )
    : m_impl{ std::make_unique< TaskSchedulerImpl >(
        std::move( telemetric_device ) ) }
{
}

TaskScheduler::~TaskScheduler( ) = default;

auto TaskScheduler::addTask( task_t &&task ) -> taskId_t
{
  return m_impl->addTask( std::move( task ) );
}

void TaskScheduler::pump( unsigned count_hint ) { m_impl->pump( count_hint ); }

} // namespace sak