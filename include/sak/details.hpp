/**
 * @file details.hpp
 * @brief Internal details on strong types and other details.
 * @author Ole Jørgensen
 * @date 2025-03-31
 */

#pragma once

#include <sak/StrongType.hpp>

#include <filesystem>

namespace sak {

using SchemaPath =
  sak::StrongType< std::filesystem::path, struct SchemaPathTag >;

}