#include <stdlib.h>

extern void* zstbiMalloc(size_t size);
extern void* zstbiRealloc(void* ptr, size_t size);
extern void zstbiFree(void* ptr);

#define STBI_MALLOC(size) zstbiMalloc(size)
#define STBI_REALLOC(ptr, size) zstbiRealloc(ptr, size)
#define STBI_FREE(ptr) zstbiFree(ptr)

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
