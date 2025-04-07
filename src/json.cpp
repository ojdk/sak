#include "json_impl.hpp"

#include <sak/json.hpp>

namespace sak {

json::json( )
    : m_impl( std::make_unique< json_impl >( ) )
{
}

json::json( std::istream &&file )
    : m_impl( std::make_unique< json_impl >( std::move( file ) ) )
{
}

json::json( json const &other ) noexcept
    : m_impl( std::make_unique< json_impl >( other.dump( ) ) )
{
}

json::json( json &&other ) noexcept
    : m_impl( std::move( other.m_impl ) )
{
}

json::json( std::string const &json_str ) noexcept
    : m_impl( std::make_unique< json_impl >( json_str ) )
{
}

auto json::operator=( json const &other ) noexcept -> json &
{
    if ( this != &other ) {
      m_impl = std::make_unique< json_impl >( other.dump( ) );
  }
  return *this;
}

auto json::operator=( json &&other ) noexcept -> json &
{
    if ( this != &other ) {
      m_impl = std::move( other.m_impl );
  }
  return *this;
}

json::~json( ) noexcept = default;

auto json::dump( ) const -> std::string { return m_impl->dump( ); }

} // namespace sak