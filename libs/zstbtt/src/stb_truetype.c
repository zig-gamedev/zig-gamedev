#include <stdlib.h>

void* (*zstbttMallocPtr)(size_t size, void *userdata) = NULL;
void (*zstbttFreePtr)(void* ptr, void *userdata) = NULL;

#define STBTT_malloc(size, userdata) zstbttMallocPtr(size, userdata)
#define STBTT_free(ptr, userdata) zstbttFreePtr(ptr, userdata)

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
