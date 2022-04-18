#include <stddef.h>
#include <stdlib.h>

typedef void* (*zmesh_MallocFn)(size_t size);
typedef void* (*zmesh_CallocFn)(size_t num, size_t size);
typedef void* (*zmesh_ReallocFn)(void* ptr, size_t size);
typedef void (*zmesh_FreeFn)(void* ptr);

static zmesh_MallocFn s_malloc_fn = malloc;
static zmesh_CallocFn s_calloc_fn = calloc;
static zmesh_ReallocFn s_realloc_fn = realloc;
static zmesh_FreeFn s_free_fn = free;

#ifdef __cplusplus
extern "C" {
#endif

void zmesh_setAllocator(
    zmesh_MallocFn malloc_fn,
    zmesh_CallocFn calloc_fn,
    zmesh_ReallocFn realloc_fn,
    zmesh_FreeFn free_fn
) {
    s_malloc_fn = malloc_fn;
    s_calloc_fn = calloc_fn;
    s_realloc_fn = realloc_fn;
    s_free_fn = free_fn;
}

#ifdef __cplusplus
}
#endif

#define PAR_MALLOC(T, N) ((T*) s_malloc_fn(N * sizeof(T)))
#define PAR_CALLOC(T, N) ((T*) s_calloc_fn(N * sizeof(T), 1))
#define PAR_REALLOC(T, BUF, N) ((T*) s_realloc_fn(BUF, sizeof(T) * (N)))
#define PAR_FREE(BUF) s_free_fn(BUF)

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
