#include <stddef.h>
#include <stdlib.h>

extern void* zmeshMalloc(size_t size);
extern void* zmeshCalloc(size_t num, size_t size);
extern void* zmeshRealloc(void* ptr, size_t size);
extern void zmeshFree(void* ptr);

#define PAR_MALLOC(T, N) ((T*) zmeshMalloc(N * sizeof(T)))
#define PAR_CALLOC(T, N) ((T*) zmeshCalloc(N * sizeof(T), 1))
#define PAR_REALLOC(T, BUF, N) ((T*) zmeshRealloc(BUF, sizeof(T) * (N)))
#define PAR_FREE(BUF) zmeshFree(BUF)

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
