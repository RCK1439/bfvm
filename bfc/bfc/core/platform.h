#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
#   define BFC_PLATFORM_WINDOWS
#elif defined(__linux__)
#   define BFC_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define BFC_PLATFORM_APPLE
#else
#   error "Unsupported platform"
#endif

#if defined(DEBUG)
#   define BFC_DEBUG
#elif defined(NDEBUG)
#   define BFC_RELEASE
#else
#   define BFC_DEBUG
#endif

#endif /* PLATFORM_H */
