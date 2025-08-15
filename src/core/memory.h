#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define BFVM_MALLOC(T, N)     (T *)bfvmMalloc(sizeof(T) * (N))
#define BFVM_CALLOC(T, N)     (T *)bfvmCalloc(N, sizeof(T))
#define BFVM_REALLOC(T, P, N) (T *)bfvmRealloc(P, sizeof(T) * (N))
#define BFVM_FREE(P)          bfvmFree(P)

void *bfvmMalloc(size_t numBytes);
void *bfvmCalloc(size_t numElements, size_t bytesPerElement);
void *bfvmRealloc(void *ptr, size_t numBytes);
void bfvmFree(void *ptr);

#endif /* MEMORY_H */
