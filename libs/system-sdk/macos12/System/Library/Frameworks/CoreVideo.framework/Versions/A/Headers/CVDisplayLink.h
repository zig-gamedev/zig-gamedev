/*
 *  CVDisplayLink.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2015 Apple Inc. All rights reserved.
 *
 */
 
  /*! @header CVDisplayLink.h
	@copyright 2004-2015 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later
    @discussion The main purpose of the CoreVideo DisplayLink API is to provide a worker thread to the VideoUnit subsystem that is clocked based on the refresh rate of a CGDirectDisplay device. In the current implementation, these DisplayLinks are created automatically by the Video Unit display nodes, and the developer does not have to deal with them directly.
		A CoreVideo DisplayLink is represented in code by a CVDisplayLinkRef. The CVDisplayLinkRef API uses the CoreFoundation class system internally to provide reference counting behaviour and other such goodies. There are three different ways to create a CVDisplayLinkRef in the current API. The first call is the most general case, and the other two are provided as a convenience (the third will probably go away, as it's only marginally useful). 
		   
*/


#if !defined(__COREVIDEO_CVDISPLAYLINK_H__)
#define __COREVIDEO_CVDISPLAYLINK_H__ 1



#include <CoreGraphics/CoreGraphics.h>
#include <CoreVideo/CVReturn.h>
#include <CoreVideo/CVBase.h>
#include <OpenGL/OpenGL.h>

typedef struct CV_BRIDGED_TYPE(id) __CVDisplayLink *CVDisplayLinkRef;

#if defined(__cplusplus)
extern "C" {
#endif

typedef CVReturn (*CVDisplayLinkOutputCallback)(
		CVDisplayLinkRef CV_NONNULL displayLink,
		const CVTimeStamp * CV_NONNULL inNow,
		const CVTimeStamp * CV_NONNULL inOutputTime,
		CVOptionFlags flagsIn, 
		CVOptionFlags * CV_NONNULL flagsOut,
		void * CV_NULLABLE displayLinkContext );

#if __BLOCKS__
typedef CVReturn (^CVDisplayLinkOutputHandler)(
		CVDisplayLinkRef CV_NONNULL displayLink,
		const CVTimeStamp * CV_NONNULL inNow,
		const CVTimeStamp * CV_NONNULL inOutputTime,
		CVOptionFlags flagsIn,
		CVOptionFlags * CV_NONNULL flagsOut );
#endif // __BLOCKS__

CV_EXPORT CFTypeID CVDisplayLinkGetTypeID(void) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkCreateWithCGDisplays
    @abstract   General call to create a CVDisplayLink
    @discussion Use this call to create a CVDisplayLink for a set of displays indentified by the CGDirectDisplayIDs.
    @param      displayArray array of CGDirectDisplayIDs
    @param      count   number of displays in the displayArray
    @param      displayLisk The new display link will be returned here
    @result	returns kCVReturnSuccess on success.
*/
CV_EXPORT CVReturn CVDisplayLinkCreateWithCGDisplays(
    CGDirectDisplayID * CV_NONNULL displayArray,
    CFIndex count,
    CV_RETURNS_RETAINED_PARAMETER CVDisplayLinkRef CV_NULLABLE * CV_NONNULL displayLinkOut ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkCreateWithOpenGLDisplayMask
    @abstract   Convenience call to create a CVDisplayLink from an OpenGL display mask.
    @discussion Use this call to create a CVDisplayLink for a CGOpenGLDisplayMask.
    @param      mask CGOpenGLDisplayMask describing the display
    @param      displayLisk The new display link will be returned here
    @result	returns kCVReturnSuccess on success.
*/
CV_EXPORT CVReturn CVDisplayLinkCreateWithOpenGLDisplayMask(
    CGOpenGLDisplayMask mask,
    CV_RETURNS_RETAINED_PARAMETER CVDisplayLinkRef CV_NULLABLE * CV_NONNULL displayLinkOut ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkCreateWithCGDisplay
    @abstract   Convenience call to create a CVDisplayLink for a single CGDirectDisplay.
    @discussion Use this call to create a CVDisplayLink for a single CGDirectDisplay.
    @param      displayID CGDirectDisplayID of the target display
    @param      displayLisk The new display link will be returned here
    @result	returns kCVReturnSuccess on success.
*/
CV_EXPORT CVReturn CVDisplayLinkCreateWithCGDisplay(
    CGDirectDisplayID displayID,
    CV_RETURNS_RETAINED_PARAMETER CVDisplayLinkRef CV_NULLABLE * CV_NONNULL displayLinkOut ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkCreateWithActiveCGDisplays
    @abstract   Convenience function to create a CVDisplayLink capable of being used with all active CGDisplays
    @param      displayLinkOut The newly created CVDisplayLink
    @result     kCVReturnSuccess if the device was created, or failure
*/
CV_EXPORT CVReturn CVDisplayLinkCreateWithActiveCGDisplays(
    CV_RETURNS_RETAINED_PARAMETER CVDisplayLinkRef CV_NULLABLE * CV_NONNULL displayLinkOut ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkSetCurrentCGDisplay
    @abstract   Sets the current display of a DisplayLink
    @discussion It is safe to call this with a running display link, but be aware that there will likely be a timestamp
                discontinuity in the video time stamp
    @param      displayLink target CVDisplayLinkRef
    @param      displayID target CGDirectDisplayID
    @result     CVReturn. kCVReturnSuccess if successfull.
*/
CV_EXPORT CVReturn CVDisplayLinkSetCurrentCGDisplay( CVDisplayLinkRef CV_NONNULL displayLink, CGDirectDisplayID displayID ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext
    @abstract   Convenience function to select a CVDisplayLink most optimal for the current renderer of the passed in OpenGL context
    @param      displayLink The CVDisplayLink for which you want to set the current CGDisplay
    @param      cglContext The OpenGL context to retrieve the current renderer from.
    @param      cglPixelFormat The OpenGL pixel format used to create the passed in OpenGL context
    @result     kCVReturnSuccess if a device was found, or failure.
*/
CV_EXPORT CVReturn CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(
																  CVDisplayLinkRef CV_NONNULL displayLink,
																  CGLContextObj CV_NONNULL cglContext,
																  CGLPixelFormatObj CV_NONNULL cglPixelFormat) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;
/*!
    @function   CVDisplayLinkGetCurrentCGDisplay
    @abstract   Gets the current display of a DisplayLink
    @discussion (description)
    @param      displayLink target CVDisplayLinkRef
    @result     CGDirectDisplayID
*/
CV_EXPORT CGDirectDisplayID CVDisplayLinkGetCurrentCGDisplay( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkSetOutputCallback
    @abstract   Set the renderer output callback function	
    @discussion The DisplayLink will invoke this callback whenever it wants you to output a frame.
    @param      displayLink target CVDisplayLinkRef
    @param	callback	CVDisplayLinkOutputCallback function
    @param	userInfo  User data for the callback to identify the context.
    @result     CVReturn. kCVReturnSuccess if successfull.
*/
CV_EXPORT CVReturn CVDisplayLinkSetOutputCallback( CVDisplayLinkRef CV_NONNULL displayLink, CVDisplayLinkOutputCallback CV_NULLABLE callback, void * CV_NULLABLE userInfo ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
	 @function   CVDisplayLinkSetOutputHandler
	 @abstract   Set the renderer output callback block
	 @discussion The DisplayLink will invoke this block whenever it wants you to output a frame. You can only have a block or a callback function; not both.
	 @param      displayLink target CVDisplayLinkRef
	 @param      handler	CVDisplayLinkOutputHandler block
	 @result     CVReturn. kCVReturnSuccess if successful.
*/
	
CV_EXPORT CVReturn CVDisplayLinkSetOutputHandler( CVDisplayLinkRef CV_NONNULL displayLink, CVDisplayLinkOutputHandler CV_NONNULL handler );

/*!
    @function   CVDisplayLinkStart
    @abstract   Start timer for DisplayLink
    @discussion This call should not be made from inside the CVDisplayLinkOutputCallback
    @param      displayLink target CVDisplayLinkRef
    @result     CVReturn. kCVReturnSuccess if successfull.
                kCVReturnDisplayLinkCallbacksNotSet The DisplayLink cannot be started until the output callback is set.
*/
CV_EXPORT CVReturn CVDisplayLinkStart( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkStop
    @abstract   Stop timer for DisplayLink
    @discussion (description)
    @param      displayLink target CVDisplayLinkRef
    @result     CVReturn. kCVReturnSuccess if successfull.
*/
CV_EXPORT CVReturn CVDisplayLinkStop( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkGetNominalOutputVideoRefreshPeriod
    @abstract   Retrieves the nominal refresh period of a CVDisplayLink.
    @discussion This call allows one to retrieve the device's "ideal" refresh period.   For example, an NTSC output device might report 1001/60000 to represent the exact NTSC vertial refresh rate.
    @param      displayLink The CVDisplayLink to get the refresh period from.
    @result     A CVTime struct that holds the nominal refresh period.    This value may be indefinite.
*/
CV_EXPORT CVTime CVDisplayLinkGetNominalOutputVideoRefreshPeriod( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkGetOutputVideoLatency
    @abstract   Retrieves the nominal latency of a CVDisplayLink.
    @discussion This call allows one to retrieve the device's built in output latency. An NTSC device with one frame of latency might report back 1001/30000 or 2002/60000, for example.
    @param      displayLink The CVDisplayLink to get the latency period from.
    @result     A CVTime struct that holds the latency.   This value may be indefinite.
*/
CV_EXPORT CVTime CVDisplayLinkGetOutputVideoLatency( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkGetActualOutputVideoRefreshPeriod
    @abstract   Retrieves the actual output refresh period of a display as measured by the host timebase.
    @discussion This call returns the actual output refresh period (in seconds) as computed relative to the host's timebase.
    @param      displayLink The CVDisplayLink to get the refresh period from.
    @result     A double containing the actual refresh period.   This value may be zero if the device is not running, or is otherwise unavailable.
*/
CV_EXPORT double CVDisplayLinkGetActualOutputVideoRefreshPeriod( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkIsRunning
    @abstract   Retrieves the running state of a CVDisplayLink.
    @discussion This call queries the running state of the given CVDisplayLink.
    @param      displayLink The CVDisplayLink to get the running state from.
    @result     A boolean describing the running state. It returns true if it is running and false if it is not running or the CVDisplayLink is invalid.
*/
CV_EXPORT Boolean CVDisplayLinkIsRunning( CVDisplayLinkRef CV_NONNULL displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkGetCurrentTime
    @abstract   Retrieves the current ("now") time of a given CVDisplayLink
    @discussion This call may be used to get the current time of a running CVDisplayLink, outside of the output callback.
    @param      displayLink The CVDisplayLink to get the current time from.
    @param      outTime A pointer to a CVTimeStamp struct.  This struct's version field must currently be set correctly (currently 0) to indicate which version of the timestamp struct is desired.
    @result     kCVReturnSuccess if the current time could be retrieved, otherwise an error indicating why the operation failed.
*/
CV_EXPORT CVReturn CVDisplayLinkGetCurrentTime( CVDisplayLinkRef CV_NONNULL displayLink, CVTimeStamp * CV_NONNULL outTime ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkTranslateTime
    @abstract   Translates the time in the CVDisplayLink's time base from one representation to
                    another. Note that the device has to be running for this call to succeed.
    @param      displayLink The CVDisplayLink who's timebase should be used to do the translation.
    @param      inTime A CVTimeStamp containing the source time to be translated.
    @param      outTime A CVTimeStamp into which the target time will be written. This struct's version field must currently be set correctly 
                (currently 0) to indicate which version of the timestamp struct is desired.  As well, the flags field should be used to specify
                which representations to translate to.
    @result     kCVReturnSuccess if the time could be translated, otherwise an error indicating why the operation failed.
*/
CV_EXPORT CVReturn CVDisplayLinkTranslateTime( CVDisplayLinkRef CV_NONNULL displayLink, const CVTimeStamp * CV_NONNULL inTime, CVTimeStamp * CV_NONNULL outTime ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkRetain
    @abstract   Retains the CVDisplayLink
    @discussion Use this call to retain a CVDisplayLink.
    @param      displayLink target CVDisplayLinkRef.   NULL safe.
	@result		If successfull the passed in dislplayLink
*/
CV_EXPORT CVDisplayLinkRef CV_NULLABLE CVDisplayLinkRetain( CVDisplayLinkRef CV_NULLABLE displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

/*!
    @function   CVDisplayLinkRelease
    @abstract   Releases the CVDisplayLink
    @discussion Use this call to release a CVDisplayLink.
    @param      displayLink target CVDisplayLinkRef.  NULL safe.
*/
CV_EXPORT void CVDisplayLinkRelease( CV_RELEASES_ARGUMENT CVDisplayLinkRef CV_NULLABLE displayLink ) AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

#if defined(__cplusplus)
}
#endif

#endif
