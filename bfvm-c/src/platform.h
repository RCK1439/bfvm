#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32)
#   define BFVM_WINDOWS
#elif defined(__linux__)
#   define BFVM_LINUX
#else
#   error "Only Windows and Linux platforms supported"
#endif

#endif /* PLATFORM_H */
