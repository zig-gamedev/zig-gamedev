/*
 *  CVOpenGLBufferPool.h
 *  CoreVideo
 *
 *  Copyright 2004-2015 Apple Inc. All rights reserved.
 *
 */

 /*! @header CVOpenGLBufferPool.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later
    @discussion CVOpenGLBufferPool is a utility object for managing a set of CVOpenGLBuffer objects that are going to be recycled.
		   
*/

#if !defined(__COREVIDEO__CVOPENGLBUFFERPOOL_H__)
#define __COREVIDEO__CVOPENGLBUFFERPOOL_H__ 1

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVOpenGLBuffer.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct CV_BRIDGED_TYPE(id) __CVOpenGLBufferPool *CVOpenGLBufferPoolRef;

CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferPoolMinimumBufferCountKey COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

// By default, buffers will age out after one second.   If required, setting an age of zero will disable
// the age-out mechanism completely.
CV_EXPORT const CFStringRef CV_NONNULL kCVOpenGLBufferPoolMaximumBufferAgeKey COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

CV_EXPORT CFTypeID CVOpenGLBufferPoolGetTypeID(void) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferPoolRetain
    @abstract   Retains a CVOpenGLBufferPoolRef object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVOpenGLBufferPoolRef object that you want to retain.
    @result     A CVOpenGLBufferPoolRef object that is the same as the passed in buffer.
*/
CV_EXPORT CVOpenGLBufferPoolRef CV_NULLABLE CVOpenGLBufferPoolRetain( CVOpenGLBufferPoolRef CV_NULLABLE openGLBufferPool ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst); // NULL-safe

/*!
    @function   CVOpenGLBufferPoolRelease
    @abstract   Releases a CVOpenGLBufferPoolRef object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVOpenGLBufferPoolRef object that you want to release.
*/
CV_EXPORT void CVOpenGLBufferPoolRelease( CV_RELEASES_ARGUMENT CVOpenGLBufferPoolRef CV_NULLABLE openGLBufferPool ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst); // NULL-safe

/*!
    @function   CVOpenGLBufferPoolCreate
    @abstract   Creates a new OpenGL Buffer pool.
    @discussion Equivalent to CFRelease, but NULL safe
    @param      allocator The CFAllocatorRef to use for allocating this buffer pool.  May be NULL.
    @param      poolAttributes   A CFDictionaryRef containing the attributes to be used for the pool itself.
    @param      openGLBufferAttributes   A CFDictionaryRef containing the attributes to be used for creating new OpenGLBuffers within the pool.
    @param      poolOut   The newly created pool will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVOpenGLBufferPoolCreate(
    CFAllocatorRef CV_NULLABLE allocator,
    CFDictionaryRef CV_NULLABLE poolAttributes,
    CFDictionaryRef CV_NULLABLE openGLBufferAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVOpenGLBufferPoolRef CV_NULLABLE * CV_NONNULL poolOut ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferPoolGetOpenGLBufferAttributes
    @abstract   Returns the pool attributes dictionary for a CVOpenGLBufferPool
    @param      pool  The CVOpenGLBufferPoolRef to retrieve the attributes from
    @result     Returns the pool attributes dictionary, or NULL on failure.
*/
CV_EXPORT CFDictionaryRef CV_NULLABLE CVOpenGLBufferPoolGetAttributes( CVOpenGLBufferPoolRef CV_NONNULL pool ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferPoolGetOpenGLBufferAttributes
    @abstract   Returns the attributes of OpenGL buffers that will be created from this pool.
    @discussion This function is provided for those cases where you may need to know some information about the buffers that
                will be created up front.
    @param      pool  The CVOpenGLBufferPoolRef to retrieve the attributes from
    @result     Returns the OpenGL buffer attributes dictionary, or NULL on failure.
*/
CV_EXPORT CFDictionaryRef CV_NULLABLE CVOpenGLBufferPoolGetOpenGLBufferAttributes( CVOpenGLBufferPoolRef CV_NONNULL pool ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

/*!
    @function   CVOpenGLBufferPoolCreateOpenGLBuffer
    @abstract   Creates a new OpenGLBuffer object from the pool.
    @discussion The function creates a new CVOpenGLBuffer with the default attachments using the OpenGL buffer attributes specifed during pool creation.
    @param      allocator The CFAllocatorRef to use for creating the OpenGL buffer.  May be NULL.
    @param      openGLBufferPool      The CVOpenGLBufferPool that should create the new CVOpenGLBuffer.
    @param      openGLBufferOut   The newly created OpenGL buffer will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVOpenGLBufferPoolCreateOpenGLBuffer(
    CFAllocatorRef CV_NULLABLE allocator,
    CVOpenGLBufferPoolRef CV_NONNULL openGLBufferPool,
    CV_RETURNS_RETAINED_PARAMETER CVOpenGLBufferRef CV_NULLABLE * CV_NONNULL openGLBufferOut ) COREVIDEO_GL_DEPRECATED(macosx, 10.4, 10.14) API_UNAVAILABLE(macCatalyst);

#if defined(__cplusplus)
}
#endif

#endif
