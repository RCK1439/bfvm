#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define BFC_MALLOC(T, N)     (T *)bfcMalloc(sizeof(T) * (N))
#define BFC_CALLOC(T, N)     (T *)bfcCalloc(N, sizeof(T))
#define BFC_REALLOC(T, P, N) (T *)bfcRealloc(P, sizeof(T) * (N))
#define BFC_FREE(P)          bfcFree(P)

void *bfcMalloc(size_t numBytes);
void *bfcCalloc(size_t numElements, size_t bytesPerElement);
void *bfcRealloc(void *ptr, size_t numBytes);
void bfcFree(void *ptr);

char *bfcCloneString(const char *s);

#endif /* MEMORY_H */
