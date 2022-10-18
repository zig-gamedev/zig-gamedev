/*
	Copyright:	(c) 2008-2012 Apple Inc. All rights reserved.
*/

#ifndef _CGLIOSURFACE_H
#define _CGLIOSURFACE_H

#include <AvailabilityMacros.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/gltypes.h>

#ifdef __cplusplus
extern "C" {
#endif

OPENGL_ASSUME_NONNULL_BEGIN

typedef struct  OPENGL_BRIDGED_TYPE(id) __IOSurface *IOSurfaceRef OPENGL_SWIFT_NAME(IOSurfaceRef);

/*!
            @function  CGLTexImageIOSurface2D
            
            CGLTexImageIOSurface2D() allows you to bind a single image plane from an IOSurfaceRef to
            an OpenGL texture object.   It is the rough equivalent to glTexImage2D(), except that
            the underlying source data comes from an IOSurface rather than from an explicit pointer.
            Note that unlike glTexImage2D(), the binding is "live", in that if the contents of
            the IOSurface change, the contents become visible to OpenGL without making another
            call to CGLTexImageIOSurface2D().   That being said, there are a few synchronization
            things to worry about if you are using IOSurface to pass data between contexts and/or
            different processes, or between the CPU and OpenGL.
            
            In general IOSurface follows Apple's cross-context synchronization rules for OpenGL. Put
            simply, in order for changes done to an IOSurface on context A to become visible to context
            B, you must flush context A's command stream (via an explicit call to glFlush, glFlushRenderAPPLE,
            etc.), and then perform a 'bind' (in this case, glBindTexture()) on context B.  Note that
            in the case of an IOSurface backed texture used as a color buffer attachment for an FBO,
            you are only required to call glBindFramebuffer() again.  You do not have to call
            glFramebufferTexture2D().
            
            Likewise, if you make changes to an IOSurface with the CPU, you *must* wrap those changes
            with calls to IOSurfaceLock() and IOSurfaceUnlock(), and call glBindTexture() again for
            OpenGL to pick up the changes.
            
            Going in the other direction, if you want changes made with OpenGL to an IOSurface to become
            visible to the CPU, you must also perform an explicit flush of the OpenGL context before
            calling IOSurfaceLock(), or you may not get the most recent data.
            
            IOSurface and OpenGL make no guarantees about correctness if these rules are not followed.
            
            Because of the way IOSurface is currently implemented, it does not allow for any kind of
            automatic format conversion to take place between the data in the IOSurface and the data seen
            by the GPU.   This means that OpenGL is going to interpret the data in the IOSurface exactly
            as how it is specified by the format and type parameters passed to CGLTexImageIOSurface2D().
            
            The upshot if this is that the only supported format/type combinations are those that have
            exact matching hardware formats.   The table at the bottom of this file lists the format/type/
            internalFormat combinations that are supported in Mac OS X 10.6.
            
            In cases where a given format/type combination has both an Internal Format and Sized Internal
            Format, either one may be used.
            
            Note that CGLTexImageIOSurface2D for the most part doesn't care what the pixel format or bytes
            per element of the IOSurface is.  Instead it only enforces the rule that there must be enough
            data in the plane being bound to cover what is required by the width/height/format/type
            parameters.   However, if you are using the IOSurface texture as a color buffer attachment to
            an FBO, then you must ensure that the bytes per element value of the IOSurface must match the
            effective bytes per pixel value for the format and type combination you are passing to OpenGL.
            
            @param ctx              The CGLContextObj for the OpenGL context you are using
            @param target           Must currently be GL_TEXTURE_RECTANGLE_ARB as of Mac OS X 10.6.0.
            @param internal_format  One of the supported values from the table below that matches
                                    the format/type combination.
            @param width            The width of the texture in pixels.
            @param height           The height of the texture in pixels.
            @param format           The OpenGL texture format enum
            @param type             The OpenGL texture type enum
            @param ioSurface        The IOSurfaceRef this texture is being bound to
            @param plane            Which plane of the IOSurface this texture is being bound to
            
*/
extern CGLError CGLTexImageIOSurface2D(CGLContextObj ctx, GLenum target, GLenum internal_format,
						GLsizei width, GLsizei height, GLenum format, GLenum type, IOSurfaceRef ioSurface, GLuint plane) OPENGL_AVAILABLE(10_6);
/*!

    Supported IOSurface texture format/type/internal format combinations in Mac OS X 10.6

    Format                          Type                                    Internal Format         Sized Internal Format
    GL_ALPHA                        GL_UNSIGNED_BYTE                        GL_ALPHA                GL_ALPHA8
    GL_LUMINANCE                    GL_UNSIGNED_BYTE                        GL_LUMINANCE            GL_LUMINANCE8
    GL_LUMINANCE                    GL_UNSIGNED_BYTE                        GL_INTENSITY            GL_INTENSITY8
    GL_LUMINANCE_ALPHA              GL_UNSIGNED_BYTE                        GL_LUMINANCE_ALPHA      GL_LUMINANCE8_ALPHA8

    GL_ALPHA                        GL_UNSIGNED_SHORT                       GL_ALPHA                GL_ALPHA16
    GL_LUMINANCE                    GL_UNSIGNED_SHORT                       GL_LUMINANCE            GL_LUMINANCE16
    GL_LUMINANCE                    GL_UNSIGNED_SHORT                       GL_INTENSITY            GL_INTENSITY16

    GL_BGRA                         GL_UNSIGNED_SHORT_1_5_5_5_REV           GL_RGBA                 GL_RGB5_A1
    GL_BGRA                         GL_UNSIGNED_SHORT_4_4_4_4_REV           GL_RGBA                 GL_RGBA4

    GL_BGRA                         GL_UNSIGNED_INT_8_8_8_8_REV             GL_RGB                  GL_RGB8
    GL_BGRA                         GL_UNSIGNED_INT_8_8_8_8_REV             GL_RGBA                 GL_RGBA8
    GL_BGRA                         GL_UNSIGNED_INT_2_10_10_10_REV          GL_RGBA                 GL_RGB10_A2

    // "Video" formats

    GL_RGB_422_APPLE                GL_UNSIGNED_SHORT_8_8_APPLE             GL_RGB                  GL_RGB8
    GL_RGB_422_APPLE                GL_UNSIGNED_SHORT_8_8_REV_APPLE         GL_RGB                  GL_RGB8
    GL_YCBCR_422_APPLE              GL_UNSIGNED_SHORT_8_8_APPLE             GL_RGB                  GL_RGB8
    GL_YCBCR_422_APPLE              GL_UNSIGNED_SHORT_8_8_REV_APPLE         GL_RGB                  GL_RGB8

    // sRGB formats

    GL_BGRA                         GL_UNSIGNED_INT_8_8_8_8_REV             GL_SRGB                 GL_SRGB8
    GL_BGRA                         GL_UNSIGNED_INT_8_8_8_8_REV             GL_SRGB_ALPHA           GL_SRGB8_ALPHA8

    // Float formats for drivers that support APPLE_float_pixels:

    GL_RGBA                         GL_HALF_APPLE                           GL_RGB                  GL_RGB_FLOAT16_APPLE
    GL_RGBA                         GL_HALF_APPLE                           GL_RGBA                 GL_RGBA_FLOAT16_APPLE
    GL_RGBA                         GL_FLOAT                                GL_RGB                  GL_RGB_FLOAT32_APPLE
    GL_RGBA                         GL_FLOAT                                GL_RGBA                 GL_RGBA_FLOAT32_APPLE

    // 16-bit unorm textures
    GL_RGBA                         GL_UNSIGNED_SHORT                       GL_RGBA                  GL_RGBA16

    // Integer formats for drivers that support EXT_texture_integer:

    GL_ALPHA_INTEGER_EXT            GL_UNSIGNED_BYTE                                                GL_ALPHA8UI
    GL_ALPHA_INTEGER_EXT            GL_BYTE                                                         GL_ALPHA8I
    GL_ALPHA_INTEGER_EXT            GL_UNSIGNED_SHORT                                               GL_ALPHA16UI
    GL_ALPHA_INTEGER_EXT            GL_SHORT                                                        GL_ALPHA16I
    GL_ALPHA_INTEGER_EXT            GL_UNSIGNED_INT                                                 GL_ALPHA32UI
    GL_ALPHA_INTEGER_EXT            GL_INT                                                          GL_ALPHA32I

    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_BYTE                                                GL_LUMINANCE8UI
    GL_LUMINANCE_INTEGER_EXT        GL_BYTE                                                         GL_LUMINANCE8I
    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_SHORT                                               GL_LUMINANCE16UI
    GL_LUMINANCE_INTEGER_EXT        GL_SHORT                                                        GL_LUMINANCE16I
    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_INT                                                 GL_LUMINANCE32UI
    GL_LUMINANCE_INTEGER_EXT        GL_INT                                                          GL_LUMINANCE32I

    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_BYTE                                                GL_INTENSITY8UI
    GL_LUMINANCE_INTEGER_EXT        GL_BYTE                                                         GL_INTENSITY8I
    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_SHORT                                               GL_INTENSITY16UI
    GL_LUMINANCE_INTEGER_EXT        GL_SHORT                                                        GL_INTENSITY16I
    GL_LUMINANCE_INTEGER_EXT        GL_UNSIGNED_INT                                                 GL_INTENSITY32UI
    GL_LUMINANCE_INTEGER_EXT        GL_INT                                                          GL_INTENSITY32I

    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_UNSIGNED_BYTE                                                GL_LUMINANCE_ALPHA8UI
    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_BYTE                                                         GL_LUMINANCE_ALPHA8I
    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_UNSIGNED_SHORT                                               GL_LUMINANCE_ALPHA16UI
    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_SHORT                                                        GL_LUMINANCE_ALPHA16I
    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_UNSIGNED_INT                                                 GL_LUMINANCE_ALPHA32UI
    GL_LUMINANCE_ALPHA_INTEGER_EXT  GL_INT                                                          GL_LUMINANCE_ALPHA32I

    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_BYTE                                                GL_RGB8UI
    GL_RGBA_INTEGER_EXT             GL_BYTE                                                         GL_RGB8I
    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_SHORT                                               GL_RGB16UI
    GL_RGBA_INTEGER_EXT             GL_SHORT                                                        GL_RGB16I
    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_INT                                                 GL_RGB32UI
    GL_RGBA_INTEGER_EXT             GL_INT                                                          GL_RGB32I

    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_BYTE                                                GL_RGBA8UI
    GL_RGBA_INTEGER_EXT             GL_BYTE                                                         GL_RGBA8I
    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_SHORT                                               GL_RGBA16UI
    GL_RGBA_INTEGER_EXT             GL_SHORT                                                        GL_RGBA16I
    GL_RGBA_INTEGER_EXT             GL_UNSIGNED_INT                                                 GL_RGBA32UI
    GL_RGBA_INTEGER_EXT             GL_INT                                                          GL_RGBA32I

    // Additional supported formats for drivers that support ARB_texture_rg:

    GL_RED                          GL_UNSIGNED_BYTE                        GL_RED                  GL_R8
    GL_RED                          GL_UNSIGNED_SHORT                       GL_RED                  GL_R16
    GL_RED                          GL_HALF_APPLE                           GL_RED                  GL_R16F
    GL_RED                          GL_FLOAT                                GL_RED                  GL_R32F

    GL_RG                           GL_UNSIGNED_BYTE                        GL_RG                   GL_RG8
    GL_RG                           GL_UNSIGNED_SHORT                       GL_RG                   GL_RG16
    GL_RG                           GL_HALF_APPLE                           GL_RG                   GL_R16F
    GL_RG                           GL_FLOAT                                GL_RG                   GL_R32F

    GL_RED_INTEGER                  GL_UNSIGNED_BYTE                                                GL_R8UI
    GL_RED_INTEGER                  GL_BYTE                                                         GL_R8I
    GL_RED_INTEGER                  GL_UNSIGNED_SHORT                                               GL_R16UI
    GL_RED_INTEGER                  GL_SHORT                                                        GL_R16I
    GL_RED_INTEGER                  GL_UNSIGNED_INT                                                 GL_R32UI
    GL_RED_INTEGER                  GL_INT                                                          GL_R32I

    GL_RG_INTEGER                   GL_UNSIGNED_BYTE                                                GL_RG8UI
    GL_RG_INTEGER                   GL_BYTE                                                         GL_RG8I
    GL_RG_INTEGER                   GL_UNSIGNED_SHORT                                               GL_RG16UI
    GL_RG_INTEGER                   GL_SHORT                                                        GL_RG16I
    GL_RG_INTEGER                   GL_UNSIGNED_INT                                                 GL_RG32UI
    GL_RG_INTEGER                   GL_INT                                                          GL_RG32I

*/

OPENGL_ASSUME_NONNULL_END

#ifdef __cplusplus
}
#endif

#endif /* _CGLSURFACE_H */

