/*
 *  CVMetalTexture.h
 *  CoreVideo
 *
 *  Copyright (c) 2011-2015 Apple Inc. All rights reserved.
 *
 */
 
 /*! @header CVMetalTexture.h
	@copyright 2011-2015 Apple Inc. All rights reserved.
	@availability iOS 8.0 or later. Mac OS X 10.11 or later.
    @discussion A CoreVideo Metal Texture derives from an ImageBuffer, and is used for supplying source image data to Metal.
    		   
*/

#if !defined(__COREVIDEO_CVMETALTEXTURE_H__)
#define __COREVIDEO_CVMETALTEXTURE_H__ 1

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVImageBuffer.h>
#include <CoreFoundation/CoreFoundation.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
/* CVMetalTextureRef is only available to Objective C code */
#if COREVIDEO_SUPPORTS_METAL
#if defined(__OBJC__)
@protocol MTLTexture;
    
#pragma mark CVMetalTexture

/*!
    @typedef	CVMetalTextureRef
    @abstract   Metal texture based image buffer

*/
typedef CVImageBufferRef CVMetalTextureRef;
	
CV_EXPORT CFTypeID CVMetalTextureGetTypeID(void) API_AVAILABLE(macosx(10.11), ios(8.0), tvos(9.0)) __WATCHOS_PROHIBITED;

/*!
    @function   CVMetalTextureGetTexture
    @abstract   Returns the Meta MTLTexture object of the CVMetalTextureRef
    @param      image Target CVMetalTexture
    @result     Metal texture
*/
CV_EXPORT id <MTLTexture> CV_NULLABLE CVMetalTextureGetTexture( CVMetalTextureRef CV_NONNULL image ) API_AVAILABLE(macosx(10.11), ios(8.0), tvos(9.0)) __WATCHOS_PROHIBITED;

/*!
    @function   CVMetalTextureIsFlipped
    @abstract   Returns whether the image is flipped vertically or not.
    @param      image Target CVMetalTexture
    @result     True if 0,0 in the texture is upper left, false if 0,0 is lower left
*/
CV_EXPORT Boolean CVMetalTextureIsFlipped( CVMetalTextureRef CV_NONNULL image ) API_AVAILABLE(macosx(10.11), ios(8.0), tvos(9.0)) __WATCHOS_PROHIBITED;

/*!
    @function   CVMetalTextureGetCleanTexCoords 
    @abstract   Returns convenient normalized texture coordinates for the part of the image that should be displayed
    @discussion This function automatically takes into account whether or not the texture is flipped.
    @param      image Target CVMetalTexture
    @param      lowerLeft  - array of two floats where the s and t normalized texture coordinates of the lower left corner of the image will be stored
    @param      lowerRight - array of two floats where the s and t normalized texture coordinates of the lower right corner of the image will be stored
    @param      upperRight - array of two floats where the s and t normalized texture coordinates of the upper right corner of the image will be stored
    @param      upperLeft  - array of two floats where the s and t normalized texture coordinates of the upper right corner of the image will be stored
*/
CV_EXPORT void CVMetalTextureGetCleanTexCoords( CVMetalTextureRef CV_NONNULL image,
                                                   float lowerLeft[CV_NONNULL 2],
                                                   float lowerRight[CV_NONNULL 2],
                                                   float upperRight[CV_NONNULL 2],
                                                   float upperLeft[CV_NONNULL 2] ) API_AVAILABLE(macosx(10.11), ios(8.0), tvos(9.0)) __WATCHOS_PROHIBITED;
#endif // defined(__OBJC__)
	
/*!
    @discussion kCVMetalTextureUsage is a property that can be placed on a CVMetalTextureCache to instruct the MTLTextureUsage of the created MTLTexture. Values for this can can be read from MTLTexture.h
 */
CV_EXPORT const CFStringRef CV_NONNULL kCVMetalTextureUsage API_AVAILABLE(macosx(10.13), ios(11.0), tvos(11.0)) __WATCHOS_PROHIBITED;
	
	
/*!
 @discussion kCVMetalTextureStorageMode is a property that can be placed on a CVMetalTextureCache to instruct the MTLTextureStorageMode of the created MTLTexture. Values for this can can be read from MTLTexture.h
 */
CV_EXPORT const CFStringRef CV_NONNULL kCVMetalTextureStorageMode API_AVAILABLE(macosx(10.15), ios(13.0), tvos(13.0)) __WATCHOS_PROHIBITED;

#endif // COREVIDEO_SUPPORTS_METAL
	
#if defined(__cplusplus)
}
#endif

#endif
