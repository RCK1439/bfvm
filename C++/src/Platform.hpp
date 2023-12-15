/**
 * @file   Platform.hpp
 * @brief  Platform detection when compiling BFVM.
 * @author Ruan C. Keet
 * @date   2023-11-16
 */

#pragma once

#ifdef _WIN32
#   ifdef _WIN64
#       define BFVM_PLATFORM_WINDOWS
#   else
#       error "64-bit mode only"
#   endif
#elif __linux__
#   define BFVM_PLATFORM_LINUX
#else
#   error "only Linux and Windows supported"
#endif
