/*
 *  CVOpenGLBuffer.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2015 Apple Inc. All rights reserved.
 *
 */
 
 /*! @header CVOpenGLBuffer.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later
    @discussion A CoreVideo buffer derives from a generic buffer and can be an ImageBuffer or PixelBuffer. 
		   
*/

#if !defined(__COREVIDEO_CVOPENGLBUFFER_H__)
#define __COREVIDEO_CVOPENGLBUFFER_H__ 1

#include <CoreVideo/CVImageBuffer.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gltypes.h>


#if defined(__cplusplus)
extern "C" {
#endif

CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferWidth COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferHeight COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferTarget COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferInternalFormat COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferMaximumMipmapLevel COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

typedef CVImageBufferRef CVOpenGLBufferRef;

CV_EXPORT CFTypeID CVOpenGLBufferGetTypeID(void) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferRetain
    @abstract   Retains a CVOpenGLBuffer object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVOpenGLBuffer object that you want to retain.
    @result     A CVOpenGLBuffer object that is the same as the passed in buffer.
*/
CV_EXPORT CVOpenGLBufferRef CV_NULLABLE CVOpenGLBufferRetain( CVOpenGLBufferRef CV_NULLABLE buffer ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferRelease
    @abstract   Releases a CVOpenGLBuffer object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVOpenGLBuffer object that you want to release.
*/
CV_EXPORT void CVOpenGLBufferRelease( CV_RELEASES_ARGUMENT CVOpenGLBufferRef CV_NULLABLE buffer ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferCreate
    @abstract   Create a new CVOpenGLBuffer that may be used for OpenGL rendering purposes
    @param      width The width of the buffer in pixels
    @param      height The height of the buffer in pixels
    @param      attributes A CFDictionaryRef containing other desired attributes of the buffer (texture target, internal format, max mipmap level, etc.).
		May be NULL.  Defaults are GL_TEXTURE_RECTANGLE_EXT, GL_RGBA, and 0 for kCVOpenGLBufferTarget, kCVOpenGLBufferInternalFormat and kCVOpenGLBufferMaximumMipmapLevel,
		respectively.
    @param      bufferOut    The newly created buffer will be placed here.
    @result     kCVReturnSuccess if the attachment succeeded
*/
CV_EXPORT CVReturn CVOpenGLBufferCreate(
    CFAllocatorRef CV_NULLABLE allocator,
    size_t width,
    size_t height,
    CFDictionaryRef CV_NULLABLE attributes,
    CV_RETURNS_RETAINED_PARAMETER CVOpenGLBufferRef CV_NULLABLE * CV_NONNULL bufferOut ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferGetAttributes
    @param      openGLBuffer Target OpenGL Buffer.
    @result     CVOpenGLBuffer attributes dictionary, NULL if not set.
*/
CV_EXPORT CFDictionaryRef CV_NULLABLE CVOpenGLBufferGetAttributes( CVOpenGLBufferRef CV_NONNULL openGLBuffer ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferAttach
    @param      openGLBuffer The buffer you wish to attach a GL context to
    @param      cglContext   The CGLContextObj you wish to attach
    @param      face	     The target GL face enum (0 for non cube maps)
    @param      level        The mipmap level index you wish to attach to
    @param      screen       The virtual screen number you want to use
    @result     kCVReturnSuccess if the attachment succeeded
*/
CV_EXPORT CVReturn CVOpenGLBufferAttach( CVOpenGLBufferRef CV_NONNULL openGLBuffer, CGLContextObj CV_NONNULL cglContext, GLenum face, GLint level, GLint screen ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

#if defined(__cplusplus)
}
#endif

#endif
