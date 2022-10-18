/*
 *  CVPixelBufferPool.h
 *  CoreVideo
 *
 *  Copyright 2004-2015 Apple Inc. All rights reserved.
 *
 */

 /*! @header CVPixelBufferPool.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later, and iOS 4.0 or later
    @discussion CVPixelBufferPool is a utility object for managing a set of CVPixelBuffer objects that are going to be recycled.
		   
*/

#if !defined(__COREVIDEO__CVPIXELBUFFERPOOL_H__)
#define __COREVIDEO__CVPIXELBUFFERPOOL_H__ 1

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>

#include <CoreVideo/CVBase.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVPixelBuffer.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct CV_BRIDGED_TYPE(id) __CVPixelBufferPool *CVPixelBufferPoolRef;

// By default, buffers will age out after one second.   If required, setting an age of zero will disable
// the age-out mechanism completely.

CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPoolMinimumBufferCountKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPoolMaximumBufferAgeKey __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


CV_EXPORT CFTypeID CVPixelBufferPoolGetTypeID(void) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferPoolRetain
    @abstract   Retains a CVPixelBufferPoolRef object
    @discussion Equivalent to CFRetain, but NULL safe
    @param      buffer A CVPixelBufferPoolRef object that you want to retain.
    @result     A CVPixelBufferPoolRef object that is the same as the passed in buffer.
*/
CV_EXPORT CVPixelBufferPoolRef CV_NULLABLE CVPixelBufferPoolRetain( CVPixelBufferPoolRef CV_NULLABLE pixelBufferPool ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0); // NULL-safe

/*!
    @function   CVPixelBufferPoolRelease
    @abstract   Releases a CVPixelBufferPoolRef object
    @discussion Equivalent to CFRelease, but NULL safe
    @param      buffer A CVPixelBufferPoolRef object that you want to release.
*/
CV_EXPORT void CVPixelBufferPoolRelease( CV_RELEASES_ARGUMENT CVPixelBufferPoolRef CV_NULLABLE pixelBufferPool ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0); // NULL-safe

/*!
    @function   CVPixelBufferPoolCreate
    @abstract   Creates a new Pixel Buffer pool.
    @param      allocator The CFAllocatorRef to use for allocating this buffer pool.  May be NULL.
    @param      attributes   A CFDictionaryRef containing the attributes to be used for creating new PixelBuffers within the pool.
    @param      poolOut   The newly created pool will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVPixelBufferPoolCreate(
    CFAllocatorRef CV_NULLABLE allocator,
    CFDictionaryRef CV_NULLABLE poolAttributes,
    CFDictionaryRef CV_NULLABLE pixelBufferAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferPoolRef CV_NULLABLE * CV_NONNULL poolOut ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferPoolGetAttributes
    @abstract   Returns the pool attributes dictionary for a CVPixelBufferPool
    @param      pool  The CVPixelBufferPoolRef to retrieve the attributes from
    @result     Returns the pool attributes dictionary, or NULL on failure.
*/
CV_EXPORT CFDictionaryRef CF_RETURNS_NOT_RETAINED CV_NULLABLE CVPixelBufferPoolGetAttributes( CVPixelBufferPoolRef CV_NONNULL pool ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferPoolGetPixelBufferAttributes
    @abstract   Returns the attributes of pixel buffers that will be created from this pool.
    @discussion This function is provided for those cases where you may need to know some information about the buffers that
                will be created up front.
    @param      pool  The CVPixelBufferPoolRef to retrieve the attributes from
    @result     Returns the pixel buffer attributes dictionary, or NULL on failure.
*/
CV_EXPORT CFDictionaryRef CF_RETURNS_NOT_RETAINED CV_NULLABLE CVPixelBufferPoolGetPixelBufferAttributes( CVPixelBufferPoolRef CV_NONNULL pool ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);

/*!
    @function   CVPixelBufferPoolCreatePixelBuffer
    @abstract   Creates a new PixelBuffer object from the pool.
    @discussion The function creates a new (attachment-free) CVPixelBuffer using the pixel buffer attributes specifed during pool creation.
    @param      allocator The CFAllocatorRef to use for creating the pixel buffer.  May be NULL.
    @param      pool      The CVPixelBufferPool that should create the new CVPixelBuffer.
    @param      pixelBufferOut   The newly created pixel buffer will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVPixelBufferPoolCreatePixelBuffer(
    CFAllocatorRef CV_NULLABLE allocator,
    CVPixelBufferPoolRef CV_NONNULL pixelBufferPool,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferRef CV_NULLABLE * CV_NONNULL pixelBufferOut ) __OSX_AVAILABLE_STARTING(__MAC_10_4,__IPHONE_4_0);


/*
    @function   CVPixelBufferPoolCreatePixelBufferWithAuxAttributes
    @abstract   Creates a new PixelBuffer object from the pool.
    @discussion This function creates a new CVPixelBuffer using the pixel buffer attributes specified during pool creation and the attributes specified in the auxAttributes parameter.
    @param      allocator The CFAllocatorRef to use for creating the pixel buffer.  May be NULL.
    @param      pixelBufferPool      The CVPixelBufferPool that should create the new CVPixelBuffer.
    @param      auxAttributes	Attributes describing this specific allocation request.  May be NULL.
    @param      pixelBufferOut   The newly created pixel buffer will be placed here
    @result     Returns kCVReturnSuccess on success
*/
CV_EXPORT CVReturn CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(
    CFAllocatorRef CV_NULLABLE allocator,
    CVPixelBufferPoolRef CV_NONNULL pixelBufferPool,
    CFDictionaryRef CV_NULLABLE auxAttributes,
    CV_RETURNS_RETAINED_PARAMETER CVPixelBufferRef CV_NULLABLE * CV_NONNULL pixelBufferOut ) __OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_4_0);

// Key for the auxiliary attributes dictionary passed to CVPixelBufferPoolCreatePixelBufferWithAuxAttributes().

// When set, indicates that a new pixel buffer should not be allocated if
// the pool already has this many or more pixel buffers allocated.
// This does not prevent already-allocated buffers from being recycled.
// If this key causes CVPixelBufferPoolCreatePixelBufferWithAuxAttributes to fail,
// it will return kCVReturnWouldExceedAllocationThreshold.
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPoolAllocationThresholdKey __OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_4_0); // CFNumberRef -- for use only in auxAttributes

// kCVPixelBufferPoolFreeBufferNotification is posted if a buffer becomes available after
// CVPixelBufferPoolCreatePixelBufferWithAuxAttributes has failed due to kCVPixelBufferPoolAllocationThresholdKey.
// This notification will not be posted by the pool if kCVPixelBufferPoolAllocationThresholdKey 
// has never been passed to CVPixelBufferPoolCreatePixelBufferWithAuxAttributes.
CV_EXPORT const CFStringRef CV_NONNULL kCVPixelBufferPoolFreeBufferNotification __OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_4_0);


/*!
	@enum CVPixelBufferPoolFlush flags
	@discussion Flags to pass to CVPixelBufferPoolFlush()
	@constant kCVPixelBufferPoolFlushExcessBuffers
		This flag will cause CVPixelBufferPoolFlush to flush all unused buffers regardless of age.
*/

typedef CF_OPTIONS(CVOptionFlags,CVPixelBufferPoolFlushFlags)
{
	kCVPixelBufferPoolFlushExcessBuffers = 1,
};

/*!
	@function       CVPixelBufferPoolFlush
	@abstract       Frees as many buffers from the pool as possible.
	@discussion     By default, this function will free all aged out buffers.  Setting the
            kCVPixelBufferPoolFlushExcessBuffers flag will cause this call to free all unused
            buffers regardless of age.
	@param		pool    The CVPixelBufferPool to be flushed.
	@param		options Set to kCVPixelBufferPoolFlushExcessBuffers to free all unused buffers
            regardless of their age.
*/
CV_EXPORT void CVPixelBufferPoolFlush( CVPixelBufferPoolRef CV_NONNULL pool, CVPixelBufferPoolFlushFlags options );


#if defined(__cplusplus)
}
#endif

#endif
