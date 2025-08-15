#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
#   define BFVM_WINDOWS
#elif defined(__linux__)
#   define BFVM_LINUX
#elif defined(__APPLE__)
#   define BFVM_APPLE
#else
#   error "Unsupported platform"
#endif

#if defined(DEBUG)
#   define BFVM_DEBUG
#elif defined(NDEBUG)
#   define BFVM_RELEASE
#else
#   define BFVM_DEBUG
#endif

#endif /* PLATFORM_H */
