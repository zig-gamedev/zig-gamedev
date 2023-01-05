// TODO: So that libc dependency can be removed, define the following:
//
// for stb_rect_pack:
//      STBRP_SORT
//      STBRP_ASSERT
//
// for stb_truetype:
//      STBTT_ifloor
//      STBTT_iceil
//      STBTT_sqrt
//      STBTT_pow
//      STBTT_fmod
//      STBTT_cos
//      STBTT_acos
//      STBTT_fabs
//      STBTT_strlen
//      STBTT_memcpy
//      STBTT_memset
//
// Also see TODOs in zstbtt.zig and build.zig

void* (*zstbttMallocPtr)(unsigned long long size, void *userdata) = 0;
void (*zstbttFreePtr)(void* ptr, void *userdata) = 0;
void (*zstbttAssertPtr)(int condition) = 0;

#define STBRP_ASSERT(condition) zstbttAssertPtr(condition)

#define STBTT_malloc(size, userdata) zstbttMallocPtr(size, userdata)
#define STBTT_free(ptr, userdata) zstbttFreePtr(ptr, userdata)
#define STBTT_assert(condition) zstbttAssertPtr(condition)

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
