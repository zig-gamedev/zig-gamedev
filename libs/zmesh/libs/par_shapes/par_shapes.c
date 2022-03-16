#include <stddef.h>
#include <stdlib.h>

typedef void* (*zmesh_malloc_fn)(size_t size);
typedef void* (*zmesh_calloc_fn)(size_t num, size_t size);
typedef void* (*zmesh_realloc_fn)(void* ptr, size_t size);
typedef void (*zmesh_free_fn)(void* ptr);

static zmesh_malloc_fn s_malloc_fn = malloc;
static zmesh_calloc_fn s_calloc_fn = calloc;
static zmesh_realloc_fn s_realloc_fn = realloc;
static zmesh_free_fn s_free_fn = free;

void zmesh_set_allocator(
    zmesh_malloc_fn malloc_fn,
    zmesh_calloc_fn calloc_fn,
    zmesh_realloc_fn realloc_fn,
    zmesh_free_fn free_fn
) {
    s_malloc_fn = malloc_fn;
    s_calloc_fn = calloc_fn;
    s_realloc_fn = realloc_fn;
    s_free_fn = free_fn;
}

#define PAR_MALLOC(T, N) ((T*) s_malloc_fn(N * sizeof(T)))
#define PAR_CALLOC(T, N) ((T*) s_calloc_fn(N * sizeof(T), 1))
#define PAR_REALLOC(T, BUF, N) ((T*) s_realloc_fn(BUF, sizeof(T) * (N)))
#define PAR_FREE(BUF) s_free_fn(BUF)

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"
