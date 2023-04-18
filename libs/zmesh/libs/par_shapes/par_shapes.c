#include <stddef.h>
#include <stdlib.h>

#ifndef ZMESH_API
#define ZMESH_API
#endif

ZMESH_API void* (*zmeshMallocPtr)(size_t size) = NULL;
ZMESH_API void* (*zmeshCallocPtr)(size_t num, size_t size) = NULL;
ZMESH_API void* (*zmeshReallocPtr)(void* ptr, size_t size) = NULL;
ZMESH_API void (*zmeshFreePtr)(void* ptr) = NULL;

#define PAR_MALLOC(T, N) ((T*) zmeshMallocPtr(N * sizeof(T)))
#define PAR_CALLOC(T, N) ((T*) zmeshCallocPtr(N * sizeof(T), 1))
#define PAR_REALLOC(T, BUF, N) ((T*) zmeshReallocPtr(BUF, sizeof(T) * (N)))
#define PAR_FREE(BUF) zmeshFreePtr(BUF)

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
