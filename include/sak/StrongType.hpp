#pragma once

#include <compare>

namespace sak {

template < typename T, typename trait_t >
class StrongType
{
public:
  explicit StrongType( T value )
      : m_value( value )
  {
  }
  // Copy constructor
  StrongType( StrongType const &other ) = default;

  // Move constructor
  StrongType( StrongType &&other ) = default;

  // Copy assignment operator
  auto operator=( StrongType const &other ) -> StrongType & = default;

  // Move assignment operator
  auto operator=( StrongType &&other ) -> StrongType & = default;

  auto value( ) const -> T const & { return m_value; }

  auto operator<=>( StrongType< T, trait_t > const &other ) const = default;

private:
  T m_value; // The underlying value of the strong type
};

} // namespace sak
