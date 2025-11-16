#include <gtest/gtest.h>

#include "../telemetric/test_mock_TelemetricDevice.hpp"

#include <sak/signal/signal.hpp>

namespace sak::signal {

// Placeholder for future signal handling utilities.

using Signal1 = sak::Signal< struct test_signal1 >;
using Signal2 = sak::Signal< struct test_signal2 >;

struct Sender : public Signal1 {
  void EmitSignal( ) { NotifyReceivers( Signal1::event{ } ); }
};

struct Receiver : public Signal1::Receiver {
  unsigned signal_count = 0;
  void OnSignalReceived( Signal1::event const &data ) { ++signal_count; }
};

struct Sender2 : public Signal1, Signal2 {
  using Signal1::NotifyReceivers;
  using Signal2::NotifyReceivers;

  void EmitSignal1( ) { NotifyReceivers( Signal1::event{ } ); }
  void EmitSignal2( ) { NotifyReceivers( Signal2::event{ } ); }
};

struct Receiver2 : public Signal1::Receiver, Signal2::Receiver {
  unsigned signal_count1 = 0;
  unsigned signal_count2 = 0;
  void OnSignalReceived( Signal1::event const &data ) { ++signal_count1; }
  void OnSignalReceived( Signal2::event const &data ) { ++signal_count2; }
};

TEST( SignalTest, SignalReceived_order1 )
{

  Sender sender;
  Receiver receiver;

  sender.RegisterReceiver( &receiver );
  EXPECT_EQ( receiver.signal_count, 0u );

  sender.EmitSignal( );
  EXPECT_EQ( receiver.signal_count, 1u );
}

TEST( SignalTest, SignalReceived_order2 )
{

  Receiver receiver;
  Sender sender;

  sender.RegisterReceiver( &receiver );
  EXPECT_EQ( receiver.signal_count, 0u );

  sender.EmitSignal( );
  EXPECT_EQ( receiver.signal_count, 1u );
}

TEST( SignalTest, MultipleSignals )
{
  Sender2 sender;
  Receiver2 receiver;
  static_cast< Signal1 & >( sender ).RegisterReceiver( &receiver );
  static_cast< Signal2 & >( sender ).RegisterReceiver( &receiver );
  EXPECT_EQ( receiver.signal_count1, 0u );
  EXPECT_EQ( receiver.signal_count2, 0u );
  sender.EmitSignal1( );
  EXPECT_EQ( receiver.signal_count1, 1u );
  EXPECT_EQ( receiver.signal_count2, 0u );
  sender.EmitSignal2( );
  EXPECT_EQ( receiver.signal_count1, 1u );
  EXPECT_EQ( receiver.signal_count2, 1u );
}

using AsyncSignal1 = sak::AsyncSignal< struct Aync_test_signal1 >;
using AsyncSignal2 = sak::AsyncSignal< struct Aync_test_signal2 >;

struct AsyncSender1 : public AsyncSignal1 {
  void EmitSignal( ) { ScheduleNotifyReceivers( AsyncSignal1::event{ } ); }
};

struct AsyncReceiver1 : AsyncSignal1::Receiver {

  unsigned signal_count = 0;
  void OnSignalReceived( AsyncSignal1::event const &data ) { ++signal_count; }
};

TEST( AsyncSignalTest, SignalReceived )
{

  sak::test::mock_EnabledTelemetricDevice m_telemetric_device;
  sak::TaskScheduler scheduler{ m_telemetric_device };

  AsyncSender1 sender{ scheduler };
  AsyncReceiver1 receiver;
  sender.RegisterReceiver( &receiver );
  EXPECT_EQ( receiver.signal_count, 0u );
  sender.EmitSignal( );
  EXPECT_EQ( receiver.signal_count, 0u );
  scheduler.pump( 1 );
  EXPECT_EQ( receiver.signal_count, 1u );


}

} // namespace sak::signal