#include "memory.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

void *bfcMalloc(size_t numBytes)
{
    void *const p = malloc(numBytes);
    if (!p)
    {
        bfcPanic("Failed to allocate %zu bytes", numBytes);
    }

    return p;
}

void *bfcCalloc(size_t numElements, size_t bytesPerElement)
{
    void *const p = calloc(numElements, bytesPerElement);
    if (!p)
    {
        bfcPanic("Failed to allocate %zu bytes", numElements * bytesPerElement);
    }

    return p;
}

void *bfcRealloc(void *ptr, size_t numBytes)
{
    void *const p = realloc(ptr, numBytes);
    if (!p)
    {
        bfcPanic("Failed to reallocate %zu bytes", numBytes);
    }

    return p;
}

void bfcFree(void *ptr)
{
    free(ptr);
}

char *bfcCloneString(const char *s)
{
    const size_t length = strlen(s) + 1;
    char *const clone = BFC_MALLOC(char, length);

#if defined(BFC_PLATFORM_WINDOWS)
    strcpy_s(clone, sizeof(char) * length, s);
#elif defined(BFC_PLATFORM_LINUX)
    strncpy(clone, s, length);
#endif

    return clone;
}
