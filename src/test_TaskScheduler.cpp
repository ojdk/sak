#include <gtest/gtest.h>

#include <sak/TaskScheduler.hpp>

struct mockTelemetricDevice : public sak::ITelemetricDevice {

  auto IsEnabled( ) const -> bool override { return true; }
};

struct TaskSchedulerTest : public ::testing::Test {
  TaskSchedulerTest( )
      : m_telemetric_device{ }
      , scheduler( m_telemetric_device )
  {
  }

  mockTelemetricDevice m_telemetric_device;
  sak::TaskScheduler scheduler;
};

TEST_F( TaskSchedulerTest, AddTask )
{
  auto taskId = scheduler.addTask( [] { /* Do something */ } );
  EXPECT_NE( taskId, 0 );
}

TEST_F( TaskSchedulerTest, PumpTasks )
{
  int taskCount = 0;
  for ( int i = 0; i < 5; ++i ) {
    scheduler.addTask( [ &taskCount ] { ++taskCount; } );
  }
  scheduler.pump( 5 );
  EXPECT_EQ( taskCount, 5 );
}

TEST_F( TaskSchedulerTest, PumpTasksWithHint )
{
  int taskCount = 0;
  for ( int i = 0; i < 10; ++i ) {
    scheduler.addTask( [ &taskCount ] { ++taskCount; } );
  }
  scheduler.pump( 5 );
  EXPECT_EQ( taskCount, 5 );
}

TEST_F( TaskSchedulerTest, NoTasksAvailable )
{
  scheduler.pump( 0 ); // Should not throw or crash
}

TEST_F( TaskSchedulerTest, NoTasksAvailableTryOne )
{
  scheduler.pump( 1 ); // Should not throw or crash
}

TEST_F( TaskSchedulerTest, AddTaskWithException )
{
  auto taskId =
    scheduler.addTask( [] { throw std::runtime_error( "Error" ); } );
  EXPECT_NE( taskId, 0 );
  EXPECT_NO_THROW( scheduler.pump( 1 ) );
}