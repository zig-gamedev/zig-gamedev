/*
	Copyright:	(c) 2010-2013 Apple Inc. All rights reserved.
*/

#ifndef __gl3ext_h_
#define __gl3ext_h_

#include <OpenGL/gltypes.h>
#include <OpenGL/OpenGLAvailability.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#if defined(__APPLE__)
#define GL3_PROTOTYPES
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

/****** Enums ************************************************/

#ifndef GL_ARB_instanced_arrays
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB 0x88FE
#endif

#ifndef GL_ARB_internalformat_query
#define GL_NUM_SAMPLE_COUNTS              0x9380
#endif

#ifndef GL_ARB_sample_shading
#define GL_SAMPLE_SHADING_ARB             0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB   0x8C37
#endif

#ifndef GL_ARB_shading_language_include
#define GL_SHADER_INCLUDE_ARB             0x8DAE
#define GL_NAMED_STRING_LENGTH_ARB        0x8DE9
#define GL_NAMED_STRING_TYPE_ARB          0x8DEA
#endif

#ifndef GL_ARB_texture_cube_map_array
#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB     0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB     0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F
#endif

#ifndef GL_ARB_texture_gather
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F
#endif

#ifndef GL_ARB_texture_storage
#define GL_TEXTURE_IMMUTABLE_FORMAT       0x912F
#endif

#ifndef GL_EXT_debug_label
/* reuse GL_TEXTURE */
/* reuse GL_FRAMEBUFFER */
/* reuse GL_RENDERBUFFER */
/* reuse GL_TRANSFORM_FEEDBACK */
#define GL_BUFFER_OBJECT_EXT              0x9151
#define GL_SHADER_OBJECT_EXT              0x8B48
#define GL_PROGRAM_OBJECT_EXT             0x8B40
#define GL_VERTEX_ARRAY_OBJECT_EXT        0x9154
#define GL_QUERY_OBJECT_EXT               0x9153
#define GL_PROGRAM_PIPELINE_OBJECT_EXT    0x8A4F
#define GL_SYNC_OBJECT_APPLE              0x8A53
#define GL_SAMPLER                        0x82E6
#endif

#ifndef GL_EXT_depth_bounds_test
#define GL_DEPTH_BOUNDS_TEST_EXT          0x8890
#define GL_DEPTH_BOUNDS_EXT               0x8891
#endif

#ifndef GL_EXT_framebuffer_multisample_blit_scaled
#define GL_SCALED_RESOLVE_FASTEST_EXT     0x90BA
#define GL_SCALED_RESOLVE_NICEST_EXT      0x90BB
#endif

#ifndef GL_EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT  0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F
#endif

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

#ifndef GL_EXT_texture_mirror_clamp
#define GL_MIRROR_CLAMP_EXT               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT       0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT     0x8912
#endif

#ifndef GL_EXT_texture_sRGB_decode
#define GL_TEXTURE_SRGB_DECODE_EXT        0x8A48
#define GL_DECODE_EXT                     0x8A49
#define GL_SKIP_DECODE_EXT                0x8A4A
#endif

#ifndef GL_APPLE_client_storage
#define GL_UNPACK_CLIENT_STORAGE_APPLE    0x85B2
#endif

#ifndef GL_APPLE_object_purgeable
#define GL_RELEASED_APPLE                 0x8A19
#define GL_VOLATILE_APPLE                 0x8A1A
#define GL_RETAINED_APPLE                 0x8A1B
#define GL_UNDEFINED_APPLE                0x8A1C
#define GL_PURGEABLE_APPLE                0x8A1D
#endif

#ifndef GL_APPLE_rgb_422
#define GL_RGB_422_APPLE                  0x8A1F
#define GL_UNSIGNED_SHORT_8_8_APPLE       0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE   0x85BB
#define GL_RGB_RAW_422_APPLE              0x8A51
#endif

#ifndef GL_APPLE_row_bytes
#define GL_PACK_ROW_BYTES_APPLE           0x8A15
#define GL_UNPACK_ROW_BYTES_APPLE         0x8A16
#define GL_PACK_IMAGE_BYTES_APPLE         0x8A17
#define GL_UNPACK_IMAGE_BYTES_APPLE       0x8A18
#endif

#ifndef GL_APPLE_texture_range
#define GL_TEXTURE_RANGE_LENGTH_APPLE     0x85B7
#define GL_TEXTURE_RANGE_POINTER_APPLE    0x85B8
#define GL_TEXTURE_STORAGE_HINT_APPLE     0x85BC
#define GL_TEXTURE_MINIMIZE_STORAGE_APPLE 0x85B6
#define GL_STORAGE_PRIVATE_APPLE          0x85BD
#define GL_STORAGE_CACHED_APPLE           0x85BE
#define GL_STORAGE_SHARED_APPLE           0x85BF
#endif

#ifndef GL_ATI_texture_mirror_once
#define GL_MIRROR_CLAMP_ATI               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI       0x8743
#endif

/****** Functions Prototypes *********************************/

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glBlendEquationiARB (GLuint buf, GLenum mode) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glBlendEquationSeparateiARB (GLuint buf, GLenum modeRGB, GLenum modeAlpha) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glBlendFunciARB (GLuint buf, GLenum src, GLenum dst) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glBlendFuncSeparateiARB (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRYP PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
#endif

#ifndef GL_ARB_instanced_arrays
#define GL_ARB_instanced_arrays 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glVertexAttribDivisorARB (GLuint index, GLuint divisor) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);
#endif

#ifndef GL_ARB_internalformat_query
#define GL_ARB_internalformat_query 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glGetInternalformativ (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
#endif

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glMinSampleShadingARB (GLclampf value) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLMINSAMPLESHADINGARBPROC) (GLclampf value);
#endif

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glNamedStringARB (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glDeleteNamedStringARB (GLint namelen, const GLchar *name) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glCompileShaderIncludeARB (GLuint shader, GLsizei count, const GLchar* const *path, const GLint *length) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI GLboolean APIENTRY glIsNamedStringARB (GLint namelen, const GLchar *name) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glGetNamedStringARB (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glGetNamedStringivARB (GLint namelen, const GLchar *name, GLenum pname, GLint *params) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
typedef void (APIENTRYP PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name);
typedef void (APIENTRYP PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const GLchar* const *path, const GLint *length);
typedef GLboolean (APIENTRYP PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name);
typedef void (APIENTRYP PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
typedef void (APIENTRYP PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const GLchar *name, GLenum pname, GLint *params);
#endif

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1
#endif

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1
#endif

#ifndef GL_ARB_texture_storage
#define GL_ARB_texture_storage 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glTexStorage1D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (APIENTRYP PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
#endif

#ifndef GL_EXT_debug_label
#define GL_EXT_debug_label 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glLabelObjectEXT (GLenum type, GLuint object, GLsizei length, const GLchar *label) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glGetObjectLabelEXT (GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLLABELOBJECTEXTPROC) (GLenum type, GLuint object, GLsizei length, const GLchar *label);
typedef void (APIENTRYP PFNGLGETOBJECTLABELEXTPROC) (GLenum type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label);
#endif

#ifndef GL_EXT_debug_marker
#define GL_EXT_debug_marker 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glInsertEventMarkerEXT (GLsizei length, const char *marker) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glPushGroupMarkerEXT (GLsizei length, const char *marker) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glPopGroupMarkerEXT (void) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLINSERTEVENTMARKEREXTPROC) (GLsizei length, const char *marker);
typedef void (APIENTRYP PFNGLPUSHGROUPMARKEREXTPROC) (GLsizei length, const char *marker);
typedef void (APIENTRYP PFNGLPOPGROUPMARKEREXTPROC) (void);
#endif

#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glDepthBoundsEXT (GLclampd zmin, GLclampd zmax) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);
#endif

#ifndef GL_EXT_framebuffer_multisample_blit_scaled
#define GL_EXT_framebuffer_multisample_blit_scaled 1
#endif

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1
#endif

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1
#endif

#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp 1
#endif

#ifndef GL_EXT_texture_sRGB_decode
#define GL_EXT_texture_sRGB_decode 1
#endif

#ifndef GL_APPLE_client_storage
#define GL_APPLE_client_storage 1
#endif

#ifndef GL_APPLE_container_object_shareable
#define GL_APPLE_container_object_shareable 1
#endif

#ifndef GL_APPLE_flush_render
#define GL_APPLE_flush_render 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glFlushRenderAPPLE (void) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glFinishRenderAPPLE (void) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glSwapAPPLE (void) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLFLUSHRENDERAPPLEPROC) (void);
typedef void (APIENTRYP PFNGLFINISHRENDERAPPLEPROC) (void);
typedef void (APIENTRYP PFNGLSWAPAPPLEPROC) (void);
#endif

#ifndef GL_APPLE_object_purgeable
#define GL_APPLE_object_purgeable 1
#ifdef GL3_PROTOTYPES
GLAPI GLenum APIENTRY glObjectPurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI GLenum APIENTRY glObjectUnpurgeableAPPLE (GLenum objectType, GLuint name, GLenum option) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glGetObjectParameterivAPPLE (GLenum objectType, GLuint name, GLenum pname, GLint *params) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef GLenum (APIENTRYP PFNGLOBJECTPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum (APIENTRYP PFNGLOBJECTUNPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef void (APIENTRYP PFNGLGETOBJECTPARAMETERIVAPPLEPROC) (GLenum objectType, GLuint name, GLenum pname, GLint *params);
#endif

#ifndef GL_APPLE_rgb_422
#define GL_APPLE_rgb_422 1
#endif

#ifndef GL_APPLE_row_bytes
#define GL_APPLE_row_bytes 1
#endif

#ifndef GL_APPLE_texture_range
#define GL_APPLE_texture_range 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glTextureRangeAPPLE (GLenum target, GLsizei length, const GLvoid *pointer) OPENGL_DEPRECATED(10.6, 10.14);
GLAPI void APIENTRY glGetTexParameterPointervAPPLE (GLenum target, GLenum pname, GLvoid **params) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLTEXTURERANGEAPPLEPROC) (GLenum target, GLsizei length, const GLvoid *pointer);
typedef void (APIENTRYP PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC) (GLenum target, GLenum pname, GLvoid **params);
#endif

#ifndef GL_ATI_texture_mirror_once
#define GL_ATI_texture_mirror_once 1
#endif

#ifndef GL_NV_texture_barrier
#define GL_NV_texture_barrier 1
#ifdef GL3_PROTOTYPES
GLAPI void APIENTRY glTextureBarrierNV (void) OPENGL_DEPRECATED(10.6, 10.14);
#endif /* GL3_PROTOTYPES */
typedef void (APIENTRYP PFNGLTEXTUREBARRIERNVPROC) (void);
#endif

#ifdef __cplusplus
}
#endif

#endif

