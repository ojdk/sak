/**
 * @file details.hpp
 * @brief Internal details for the Rock-Paper-Scissors (RPS) application.
 *
 * This file contains internal implementation details for the RPS application.
 * It is not intended to be included directly by external code.
 *
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