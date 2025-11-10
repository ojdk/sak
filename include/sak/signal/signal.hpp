#pragma once

#include <sak/TaskScheduler.hpp>

#include <set>
#include <tuple>

namespace sak {

// Signal handling utilities can be defined here in the future.

template < typename Tag, typename... data >
class Signal {
public:
  struct event {
    std::tuple< data... > info;
  };

public:
  //! IOnterface to matching signal receiver class.
  class Receiver {
  public:
    virtual ~Receiver( );

    void SetSignalHandler( sak::Signal< Tag, data... > *handler );

  public:
    virtual void
    OnSignalReceived( sak::Signal< Tag, data... >::event const &data ) = 0;

  protected:
    sak::Signal< Tag, data... > *handler_ = nullptr;
  };

public:
  Signal( ) = default;

protected:
  ~Signal( );

public:
  void RegisterReceiver( sak::Signal< Tag, data... >::Receiver *receiver );
  void UnRegisterReceiver( sak::Signal< Tag, data... >::Receiver *receiver );
  void NotifyReceivers( sak::Signal< Tag, data... >::event const &data );

protected:
  void _notify_receiver( sak::Signal< Tag, data... >::event const &data );

private:
  std::set< sak::Signal< Tag, data... >::Receiver * > receivers_;
};

//! AsyncSignal

template < typename Tag, typename... data >
class AsyncSignal : public sak::Signal< Tag, data... > {

public:
  AsyncSignal( sak::TaskScheduler & );

public:
  void ScheduleNotifyReceivers( sak::Signal< Tag, data... >::event &&data );

private:
  sak::TaskScheduler &m_scheduler;
};

} // namespace sak

namespace sak {

// Signal implementation

template < typename Tag, typename... data >
Signal< Tag, data... >::~Signal( )
{
  for ( auto it = receivers_.begin( ); it != receivers_.end( ); ) {
    ( *it )->SetSignalHandler( nullptr );
    it = receivers_.erase( it );
  }
}

template < typename Tag, typename... data >
void Signal< Tag, data... >::RegisterReceiver(
  sak::Signal< Tag, data... >::Receiver *receiver )
{
  receivers_.insert( receiver );
  receiver->SetSignalHandler( this );
}

template < typename Tag, typename... data >
void Signal< Tag, data... >::UnRegisterReceiver(
  sak::Signal< Tag, data... >::Receiver *receiver )
{
  auto it = receivers_.find( receiver );
  if ( it != receivers_.end( ) ) {
    receivers_.erase( it );
  }
}

template < typename Tag, typename... data >
void Signal< Tag, data... >::NotifyReceivers(
  sak::Signal< Tag, data... >::event const &data )
{
  _notify_receiver( data );
}

template < typename Tag, typename... data >
void Signal< Tag, data... >::_notify_receiver(
  sak::Signal< Tag, data... >::event const &data )
{
  for ( auto *receiver : receivers_ ) {
    receiver->OnSignalReceived( data );
  }
}

// Receiver implementation

template < typename Tag, typename... data >
Signal< Tag, data... >::Receiver::~Receiver( )
{
  if ( handler_ )
    handler_->UnRegisterReceiver( this );
}

template < typename Tag, typename... data >
void Signal< Tag, data... >::Receiver::SetSignalHandler(
  sak::Signal< Tag, data... > *handler )
{
  handler_ = handler;
}

// AsyncSignal implementation

template < typename Tag, typename... data >
AsyncSignal< Tag, data... >::AsyncSignal( sak::TaskScheduler &scheduler )
    : m_scheduler( scheduler )
{
}

template < typename Tag, typename... data >
void AsyncSignal< Tag, data... >::ScheduleNotifyReceivers(
  sak::Signal< Tag, data... >::event &&in_data )
{
  m_scheduler.addTask( [ this, data = std::move( in_data ) ]( ) {
    this->_notify_receiver( data );
  } );
}

} // namespace sak