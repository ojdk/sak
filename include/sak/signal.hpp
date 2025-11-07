#pragma once

#include <set>
#include <tuple>

namespace sak {

// Signal handling utilities can be defined here in the future.

template < typename Tag, bool async_support, typename... data >
class Signal {
public:
  struct event {
    std::tuple< data... > info;
  };

public:
  class Receiver {
  public:
    virtual ~Receiver( );

    void SetSignalHandler( Signal< Tag, async_support, data... > *handler );

  public:
    virtual void OnSignalReceived( event const &data ) = 0;

  protected:
    Signal< Tag, async_support, data... > *handler_ = nullptr;
  };

protected:
  ~Signal( );

public:
  void RegisterReceiver( Receiver *receiver );
  void UnRegisterReceiver( Receiver *receiver );
  void NotifyReceivers( Signal< Tag,async_support, data... >::event const &data );

private:
  void _notify_receiver( Signal< Tag, async_support, data... >::event const &data );

private:
  std::set< Receiver * > receivers_;
};

} // namespace sak

namespace sak {

// Signal implementation

template < typename Tag, bool async_support, typename... data >
Signal< Tag, async_support, data... >::~Signal( )
{
    for ( auto it = receivers_.begin( ); it != receivers_.end( ); ) {
      ( *it )->SetSignalHandler( nullptr );
      it = receivers_.erase( it );
    }
}

template < typename Tag, bool async_support, typename... data >
void Signal< Tag, async_support, data... >::RegisterReceiver( Receiver *receiver )
{
  receivers_.insert( receiver );
  receiver->SetSignalHandler( this );
}

template < typename Tag, bool async_support, typename... data >
void Signal< Tag, async_support, data... >::UnRegisterReceiver( Receiver *receiver )
{
  auto it = receivers_.find( receiver );
    if ( it != receivers_.end( ) ) {
      receivers_.erase( it );
  }
}

template < typename Tag, bool async_support, typename... data >
void Signal< Tag, async_support, data... >::NotifyReceivers( event const &data )
{
  _notify_receiver( data );
}

template < typename Tag, bool async_support, typename... data >
void Signal< Tag, async_support, data... >::_notify_receiver( event const &data )
{
    for ( auto *receiver : receivers_ ) {
      receiver->OnSignalReceived( data );
    }
}

// Receiver implementation

template < typename Tag, bool async_support, typename... data >
Signal< Tag, async_support, data... >::Receiver::~Receiver( )
{
  if ( handler_ )
    handler_->UnRegisterReceiver( this );
}

template < typename Tag, bool async_support, typename... data >
void Signal< Tag, async_support, data... >::Receiver::SetSignalHandler(
  Signal< Tag, async_support, data... > *handler )
{
  handler_ = handler;
}

} // namespace sak