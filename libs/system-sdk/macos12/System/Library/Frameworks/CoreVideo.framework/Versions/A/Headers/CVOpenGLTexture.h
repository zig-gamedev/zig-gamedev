/*
 *  CVOpenGLTexture.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2015 Apple Inc. All rights reserved.
 *
 */
 
 /*! @header CVOpenGLTexture.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later
    @discussion A CoreVideo Texture derives from an ImageBuffer, and is used for supplying source image data to OpenGL.
    		   
*/

#if !defined(__COREVIDEO_CVOPENGLTEXTURE_H__)
#define __COREVIDEO_CVOPENGLTEXTURE_H__ 1

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVImageBuffer.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gltypes.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#pragma mark CVOpenGLTexture

/*!
    @typedef	CVOpenGLTextureRef
    @abstract   OpenGL texture based image buffer

*/
typedef CVImageBufferRef CVOpenGLTextureRef;

CV_EXPORT CFTypeID CVOpenGLTextureGetTypeID(void) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureRetain
    @abstract   Retains a CVOpenGLTexture object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVOpenGLTexture object that you want to retain.
    @result     A CVOpenGLTexture object that is the same as the passed in buffer.
*/
CV_EXPORT CVOpenGLTextureRef CV_NULLABLE CVOpenGLTextureRetain( CVOpenGLTextureRef CV_NULLABLE texture ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureRelease
    @abstract   Releases a CVOpenGLTexture object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVOpenGLTexture object that you want to release.
*/
CV_EXPORT void CVOpenGLTextureRelease( CV_RELEASES_ARGUMENT CVOpenGLTextureRef CV_NULLABLE texture ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureGetTarget
    @abstract   Returns the texture target (eg. 2D vs. rect texture extension) of the CVOpenGLTexture
    @param      image Target CVOpenGLTexture
    @result     OpenGL texture target
*/
CV_EXPORT GLenum CVOpenGLTextureGetTarget( CVOpenGLTextureRef CV_NONNULL image ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureGetName
    @abstract   Returns the texture target name of the CVOpenGLTexture
    @param      image Target CVOpenGLTexture
    @result     OpenGL texture target name
*/
CV_EXPORT GLuint CVOpenGLTextureGetName( CVOpenGLTextureRef CV_NONNULL image ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureIsFlipped
    @abstract   Returns whether the image is flipped vertically or not.
    @param      image Target CVOpenGLTexture
    @result     True if 0,0 in the texture is upper left, false if 0,0 is lower left
*/
CV_EXPORT Boolean CVOpenGLTextureIsFlipped( CVOpenGLTextureRef CV_NONNULL image ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLTextureGetCleanTexCoords 
    @abstract   Returns convenient texture coordinates for the part of the image that should be displayed
    @discussion This function automatically takes into account whether or not the texture is flipped.
    @param      image Target CVOpenGLTexture
    @param      lowerLeft  - array of two GLfloats where the s and t texture coordinates of the lower left corner of the image will be stored
    @param      lowerRight - array of two GLfloats where the s and t texture coordinates of the lower right corner of the image will be stored
    @param      upperRight - array of two GLfloats where the s and t texture coordinates of the upper right corner of the image will be stored
    @param      upperLeft  - array of two GLfloats where the s and t texture coordinates of the upper right corner of the image will be stored
*/
CV_EXPORT void CVOpenGLTextureGetCleanTexCoords( CVOpenGLTextureRef CV_NONNULL image,
						 GLfloat lowerLeft[CV_NONNULL 2],
						 GLfloat lowerRight[CV_NONNULL 2],
						 GLfloat upperRight[CV_NONNULL 2],
						 GLfloat upperLeft[CV_NONNULL 2] ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

#if defined(__cplusplus)
}
#endif

#endif
