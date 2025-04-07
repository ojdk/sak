#include "json_impl.hpp"

namespace sak {

json_impl::json_impl( ) = default;

json_impl::json_impl( std::istream &&file )
    : m_json( nlohmann::json::parse( file ) )
{
}

json_impl::json_impl( json_impl const &other ) = default;

json_impl::json_impl::json_impl( json_impl &&other ) = default;

json_impl::json_impl( std::string const &json_str )
    : m_json( nlohmann::json::parse( json_str ) )
{
}

json_impl &json_impl::operator=( json_impl const &other ) = default;
json_impl &json_impl::operator=( json_impl &&other ) = default;

json_impl::~json_impl( ) = default;

auto json_impl::dump( ) const -> std::string { return m_json.dump( ); }

} // namespace sak