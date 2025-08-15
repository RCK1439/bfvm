#include "memory.h"
#include "error.h"

#include <stdlib.h>

void *bfvmMalloc(size_t numBytes)
{
    void *const p = malloc(numBytes);
    if (!p)
    {
        bfvmPanic("Failed to allocate %zu bytes", numBytes);
    }

    return p;
}

void *bfvmCalloc(size_t numElements, size_t bytesPerElement)
{
    void *const p = calloc(numElements, bytesPerElement);
    if (!p)
    {
        bfvmPanic("Failed to allocate %zu bytes", numElements * bytesPerElement);
    }

    return p;
}

void *bfvmRealloc(void *ptr, size_t numBytes)
{
    void *const p = realloc(ptr, numBytes);
    if (!p)
    {
        bfvmPanic("Failed to reallocate %zu bytes", numBytes);
    }

    return p;
}

void bfvmFree(void *ptr)
{
    free(ptr);
}
