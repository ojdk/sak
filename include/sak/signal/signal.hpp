#pragma once

#include <sak/TaskScheduler.hpp>

#include <set>
#include <tuple>

namespace sak {

// Signal handling utilities can be defined here in the future.

template < typename Tag, typename... Data >
class Signal {
public:
  struct event {
    std::tuple< Data... > info;
  };

public:
  //! IOnterface to matching signal receiver class.
  class Receiver {
  public:
    virtual ~Receiver( );

    void SetSignalHandler( sak::Signal< Tag, Data... > *handler );

  public:
    virtual void
    OnSignalReceived( sak::Signal< Tag, Data... >::event const &data ) = 0;

  protected:
    sak::Signal< Tag, Data... > *handler_ = nullptr;
  };

public:
  Signal( ) = default;

protected:
  ~Signal( );

public:
  void RegisterReceiver( sak::Signal< Tag, Data... >::Receiver *receiver );
  void UnRegisterReceiver( sak::Signal< Tag, Data... >::Receiver *receiver );
  void NotifyReceivers( sak::Signal< Tag, Data... >::event const &data );

protected:
  void _notify_receiver( sak::Signal< Tag, Data... >::event const &data );

private:
  std::set< sak::Signal< Tag, Data... >::Receiver * > receivers_;
};

//! AsyncSignal

template < typename Tag, typename... Data >
class AsyncSignal : public sak::Signal< Tag, Data... > {

public:
  AsyncSignal( sak::TaskScheduler & );

public:
  void ScheduleNotifyReceivers( sak::Signal< Tag, Data... >::event &&data );

private:
  sak::TaskScheduler &m_scheduler;
};

} // namespace sak

namespace sak {

// Signal implementation

template < typename Tag, typename... Data >
Signal< Tag, Data... >::~Signal( )
{
  for ( auto it = receivers_.begin( ); it != receivers_.end( ); ) {
    ( *it )->SetSignalHandler( nullptr );
    it = receivers_.erase( it );
  }
}

template < typename Tag, typename... Data >
void Signal< Tag, Data... >::RegisterReceiver(
  sak::Signal< Tag, Data... >::Receiver *receiver )
{
  receivers_.insert( receiver );
  receiver->SetSignalHandler( this );
}

template < typename Tag, typename... Data >
void Signal< Tag, Data... >::UnRegisterReceiver(
  sak::Signal< Tag, Data... >::Receiver *receiver )
{
  auto it = receivers_.find( receiver );
  if ( it != receivers_.end( ) ) {
    receivers_.erase( it );
  }
}

template < typename Tag, typename... Data >
void Signal< Tag, Data... >::NotifyReceivers(
  sak::Signal< Tag, Data... >::event const &data )
{
  _notify_receiver( data );
}

template < typename Tag, typename... Data >
void Signal< Tag, Data... >::_notify_receiver(
  sak::Signal< Tag, Data... >::event const &data )
{
  for ( auto *receiver : receivers_ ) {
    receiver->OnSignalReceived( data );
  }
}

// Receiver implementation

template < typename Tag, typename... Data >
Signal< Tag, Data... >::Receiver::~Receiver( )
{
  if ( handler_ )
    handler_->UnRegisterReceiver( this );
}

template < typename Tag, typename... Data >
void Signal< Tag, Data... >::Receiver::SetSignalHandler(
  sak::Signal< Tag, Data... > *handler )
{
  handler_ = handler;
}

// AsyncSignal implementation

template < typename Tag, typename... Data >
AsyncSignal< Tag, Data... >::AsyncSignal( sak::TaskScheduler &scheduler )
    : m_scheduler( scheduler )
{
}

template < typename Tag, typename... Data >
void AsyncSignal< Tag, Data... >::ScheduleNotifyReceivers(
  sak::Signal< Tag, Data... >::event &&in_data )
{
  m_scheduler.addTask( [ this, data = std::move( in_data ) ]( ) {
    this->_notify_receiver( data );
  } );
}

} // namespace sak