/**
 * @file Index.hpp
 * @brief Introduce a strongly checked index for type safety.
 * @author Ole Jørgensen
 * @date 2025-04-07
 */

#pragma once

// Add your code here

namespace sak {

template < typename T, typename trait_t >
class Number;

template < typename T, typename trait_t >
class Index {
public:
  explicit Index( T value );
  Index( Index const &other ) = default;
  Index( Index &&other ) = default;
  auto operator=( Index const &other ) -> Index & = default;
  auto operator=( Index &&other ) -> Index & = default;

public:
  auto idx( ) const -> T const & { return m_value; }

private:
  T m_value;
};

} // namespace sak