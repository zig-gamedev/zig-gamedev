/*
 *  CVOpenGLTextureCache.h
 *  CoreVideo
 *
 *  Copyright 2004-2015 Apple Inc. All rights reserved.
 *
 */

#if !defined(__COREVIDEO__CVOPENGLTEXTURECACHE_H__)
#define __COREVIDEO__CVOPENGLTEXTURECACHE_H__ 1

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVBuffer.h>
#include <CoreVideo/CVOpenGLTexture.h>
#include <OpenGL/OpenGL.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
    @typedef	CVOpenGLTextureCacheRef
    @abstract   CoreVideo OpenGL Texture Cache

*/
typedef struct CV_BRIDGED_TYPE(id) __CVOpenGLTextureCache *CVOpenGLTextureCacheRef;

/* Dictionary keys and values for use with the 'cacheAttributes' parameter of CVOpenGLTextureCacheCreate */

/* CVOpenGLTextureCache can (in some cases) do higher quality chroma upsampling on GPUs that support ARB_fragment_program.  By
   default it will be enabled automatically if the texture cache determines that the GPU has the needed support and the image
   size is something reasonable for the GPU being used.   The automatic behaviour can be overridden below.  Note that setting
   kCVOpenGLTextureCacheChromaSamplingModeHighQuality is only a request.   GPUs that don't support ARB_fragment_program will still
   resort back to the native hardware support for YCbCr textures. */
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLTextureCacheChromaSamplingModeKey COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLTextureCacheChromaSamplingModeAutomatic COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);	// Defaut if the key is not present
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLTextureCacheChromaSamplingModeHighestQuality COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);	// Force highest quality regardless of performance impact
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLTextureCacheChromaSamplingModeBestPerformance COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);	// Do it the quickest way possible

CV_EXPORT CFTypeID CVOpenGLTextureCacheGetTypeID(void) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureCacheRetain
    @abstract   Retains a CVOpenGLTextureCache object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVOpenGLTextureCache object that you want to retain.
    @result     A CVOpenGLTextureCache object that is the same as the passed in buffer.
*/
CV_EXPORT CVOpenGLTextureCacheRef CV_NULLABLE CVOpenGLTextureCacheRetain( CVOpenGLTextureCacheRef CV_NULLABLE textureCache ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst); // NULL-safe

/*!
    @function   CVOpenGLTextureCacheRelease
    @abstract   Releases a CVOpenGLTextureCache object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVOpenGLTextureCache object that you want to release.
*/
CV_EXPORT void CVOpenGLTextureCacheRelease( CV_RELEASES_ARGUMENT CVOpenGLTextureCacheRef CV_NULLABLE textureCache ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst); // NULL-safe

/*!
    @function   CVOpenGLTextureCacheCreate
    @abstract   Creates a new Texture Cache.
    @param      allocator The CFAllocatorRef to use for allocating the cache.  May be NULL.
    @param      cacheAttributes A CFDictionaryRef containing the attributes of the cache itself.   May be NULL.
    @param      cglContext The OpenGL context into which the texture objects will be created
    @param      cglPixelFormat The OpenGL pixel format object used to create the passed in OpenGL context
    @param      textureAttributes A CFDictionaryRef containing the attributes to be used for creating the CVOpenGLTexture objects.  May be NULL.
    @param      cacheOut   The newly created texture cache will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVOpenGLTextureCacheCreate(
    CFAllocatorRef CV_NULLABLE allocator,
    CFDictionaryRef CV_NULLABLE cacheAttributes,
    CGLContextObj CV_NONNULL cglContext,
    CGLPixelFormatObj CV_NONNULL cglPixelFormat,
    CFDictionaryRef CV_NULLABLE textureAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVOpenGLTextureCacheRef CV_NULLABLE * CV_NONNULL cacheOut ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureCacheCreateTextureFromImage
    @abstract   Creates a CVOpenGLTexture object from an existing CVImageBuffer
    @param      allocator The CFAllocatorRef to use for allocating the CVOpenGLTexture object.  May be NULL.
    @param      textureCache The texture cache object that will manage the texture
    @param      sourceImage The CVImageBuffer that you want to create a CVOpenGLTexture from.
    @param      attributes For Future use only! - The desired buffer attributes for the CVOpenGLTexture.
    @param      textureOut The newly created texture object will be placed here.
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVOpenGLTextureCacheCreateTextureFromImage(
    CFAllocatorRef CV_NULLABLE allocator,
    CVOpenGLTextureCacheRef CV_NONNULL textureCache,
    CVImageBufferRef CV_NONNULL sourceImage,
    CFDictionaryRef CV_NULLABLE attributes,
    CV_RETURNS_RETAINED_PARAMETER CVOpenGLTextureRef CV_NULLABLE * CV_NONNULL textureOut ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureCacheFlush
    @abstract   Performs internal housekeeping/recycling operations
    @discussion This call must be made periodically to give the texture cache a chance to make OpenGL calls
                on the OpenGL context used to create it in order to do housekeeping operations.
    @param      textureCache The texture cache object to flush
    @param      options Currently unused, set to 0.
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT void CVOpenGLTextureCacheFlush( CVOpenGLTextureCacheRef CV_NONNULL textureCache, CVOptionFlags options ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

#if defined(__cplusplus)
}
#endif

#endif
