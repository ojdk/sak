/**
 * @file sak_api.hpp
 * @author Ole Jørgensen
 * @date 2025-04-07
 */                                                                            \
#pragma once

// clang-format off
#ifdef WINDOWS
#  ifdef SAK_DYNAMIC
#    ifdef SAK_EXPORT
#      define SAK_API __declspec( dllexport )
#    else
#      define SAK_API __declspec( dllimport )
#    endif
#  else
#    define SAK_API
#  endif
#else
#  define SAK_API
#endif
// clang-format on