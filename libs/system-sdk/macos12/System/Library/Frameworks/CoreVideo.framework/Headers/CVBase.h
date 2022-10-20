/*
 *  CVBase.h
 *  CoreVideo
 *
 *  Copyright (c) 2004-2020 Apple Inc. All rights reserved.
 *
 */
 
 /*! @header CVBase.h
	@copyright 2004-2017 Apple Inc. All rights reserved.
	@availability Mac OS X 10.4 or later, and iOS 4.0 or later
    @discussion Here you can find the type declarations for CoreVideo. CoreVideo uses a CVTimeStamp structure to store video display time stamps.
*/

 
#if !defined(__COREVIDEO_CVBASE_H__)
#define __COREVIDEO_CVBASE_H__ 1

#include <TargetConditionals.h>
#include <Availability.h>
#include <AvailabilityMacros.h>
#include <stdint.h>

#if TARGET_OS_WIN32
#pragma warning (disable: 4068)		// ignore unknown pragmas
#endif



#include <CoreFoundation/CFBase.h>

#if defined(__cplusplus)
extern "C" {
#endif
#define COREVIDEO_TRUE (1 && 1)
#define COREVIDEO_FALSE (0 && 1)

#if TARGET_OS_WIN32
	#define COREVIDEO_SUPPORTS_DIRECT3D 	COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_DIRECT3D 	COREVIDEO_FALSE
#endif

#if TARGET_OS_MACCATALYST
#define COREVIDEO_SUPPORTS_OPENGL 		COREVIDEO_TRUE
#define COREVIDEO_SUPPORTS_OPENGLES		COREVIDEO_FALSE
#else
#if TARGET_OS_OSX
	#define COREVIDEO_SUPPORTS_OPENGL 		COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_OPENGL 		COREVIDEO_FALSE
#endif

#if TARGET_OS_MAC && TARGET_OS_IPHONE
	#define COREVIDEO_SUPPORTS_OPENGLES		COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_OPENGLES		COREVIDEO_FALSE
#endif
#endif

#if ((TARGET_OS_MAC && ! TARGET_OS_IPHONE) || (TARGET_OS_WIN32))
	#define COREVIDEO_SUPPORTS_COLORSPACE 	COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_COLORSPACE 	COREVIDEO_FALSE
#endif

#if (TARGET_OS_MAC && ! TARGET_OS_IPHONE)
	#define COREVIDEO_SUPPORTS_DISPLAYLINK 	COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_DISPLAYLINK 	COREVIDEO_FALSE
#endif

#if TARGET_OS_MAC
	#define COREVIDEO_SUPPORTS_IOSURFACE COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_IOSURFACE COREVIDEO_FALSE
#endif

#if TARGET_OS_EMBEDDED && (__IPHONE_OS_VERSION_MIN_REQUIRED >= 80300)
	#define COREVIDEO_SUPPORTS_PREFETCH    COREVIDEO_TRUE
#elif TARGET_OS_OSX
	#define COREVIDEO_SUPPORTS_PREFETCH    COREVIDEO_TRUE	
#else
	#define COREVIDEO_SUPPORTS_PREFETCH    COREVIDEO_FALSE
#endif
	
#if TARGET_OS_EMBEDDED && (__IPHONE_OS_VERSION_MIN_REQUIRED >= 80300)
	#define COREVIDEO_SUPPORTS_IOSURFACE_PREFETCH    COREVIDEO_TRUE
#elif TARGET_OS_OSX && (__MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
	#define COREVIDEO_SUPPORTS_IOSURFACE_PREFETCH    COREVIDEO_TRUE	
#else
	#define COREVIDEO_SUPPORTS_IOSURFACE_PREFETCH    COREVIDEO_FALSE
#endif
	
#if TARGET_OS_SIMULATOR
	#define COREVIDEO_SUPPORTS_GLES_TEX_IMAGE_IOSURFACE    COREVIDEO_FALSE
#else 
	#define COREVIDEO_SUPPORTS_GLES_TEX_IMAGE_IOSURFACE    COREVIDEO_SUPPORTS_IOSURFACE
#endif

#if TARGET_OS_IPHONE
	#define COREVIDEO_USE_IOSURFACEREF		COREVIDEO_TRUE
#else
	#define COREVIDEO_USE_IOSURFACEREF		COREVIDEO_FALSE
#endif

#if COREVIDEO_SUPPORTS_IOSURFACE && (TARGET_OS_IPHONE || (MAC_OS_X_VERSION_MIN_REQUIRED >= 101100)) && (! TARGET_OS_WATCH)
	#define COREVIDEO_SUPPORTS_METAL        COREVIDEO_TRUE
#else
	#define COREVIDEO_SUPPORTS_METAL        COREVIDEO_FALSE
#endif

#ifndef COREVIDEO_SUPPORTS_PERMANENT_ALLOCATOR
#if TARGET_OS_MAC
    #define COREVIDEO_SUPPORTS_PERMANENT_ALLOCATOR  COREVIDEO_FALSE // set this to COREVIDEO_TRUE when this is fixed [64465605] malloc zone allocations all show up as non-object in heap
#else
    #define COREVIDEO_SUPPORTS_PERMANENT_ALLOCATOR  COREVIDEO_FALSE
#endif  // TARGET_OS_MAC
#endif  // COREVIDEO_SUPPORTS_PERMANENT_ALLOCATOR

#if (__cplusplus && __cplusplus >= 201103L && (__has_extension(cxx_strong_enums) || __has_feature(objc_fixed_enum))) || (!__cplusplus && __has_feature(objc_fixed_enum))
#define COREVIDEO_USE_DERIVED_ENUMS_FOR_CONSTANTS	COREVIDEO_TRUE
#else
#define COREVIDEO_USE_DERIVED_ENUMS_FOR_CONSTANTS	COREVIDEO_FALSE
#endif

#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && defined(__has_feature) && __has_feature(nullability)
	#define COREVIDEO_DECLARE_NULLABILITY COREVIDEO_TRUE
#else
	#define COREVIDEO_DECLARE_NULLABILITY COREVIDEO_FALSE
#endif

#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && defined(__has_feature) && __has_feature(attribute_cf_returns_retained)
    #define CV_RETURNS_RETAINED CF_RETURNS_RETAINED
#else
    #define CV_RETURNS_RETAINED 
#endif

#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && defined(__has_feature) && __has_feature(attribute_cf_returns_on_parameters)
#define CV_RETURNS_RETAINED_PARAMETER		CF_RETURNS_RETAINED
#else
#define CV_RETURNS_RETAINED_PARAMETER
#endif

#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && defined(__has_feature) && __has_feature(attribute_cf_consumed)
#define CV_RELEASES_ARGUMENT        CF_RELEASES_ARGUMENT
#else
#define CV_RELEASES_ARGUMENT
#endif


#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && defined(__has_feature) && __has_feature(objc_bridge_id)
#define CV_BRIDGED_TYPE(type)       CF_BRIDGED_TYPE(type)
#else
#define CV_BRIDGED_TYPE(type)
#endif

#if COREVIDEO_DECLARE_NULLABILITY
#define CV_NULLABLE __nullable
#define CV_NONNULL __nonnull
#else
#define CV_NULLABLE
#define CV_NONNULL
#endif
	
#define CV_INTERNAL __attribute__((visibility("hidden")))

#if TARGET_OS_WIN32 && defined(CV_BUILDING_CV) && defined(__cplusplus)
#define CV_EXPORT extern "C" __declspec(dllexport) 
#elif TARGET_OS_WIN32 && defined(CV_BUILDING_CV) && !defined(__cplusplus)
#define CV_EXPORT extern __declspec(dllexport) 
#elif TARGET_OS_WIN32 && defined(__cplusplus)
#define CV_EXPORT extern "C" __declspec(dllimport) 
#elif TARGET_OS_WIN32
#define CV_EXPORT extern __declspec(dllimport) 
#else
#define CV_EXPORT __attribute__((visibility("default"))) CF_EXPORT 
#endif

#define CV_INLINE CF_INLINE

#define CVDIRECT3DDEVICE LPDIRECT3DDEVICE9
#define CVDIRECT3DTEXTURE LPDIRECT3DTEXTURE9
#define CVDIRECT3DSURFACE LPDIRECT3DSURFACE9
#define CVDIRECT3D LPDIRECT3D9

#if defined(COREVIDEO_SILENCE_GL_DEPRECATION) || \
	defined(GL_SILENCE_DEPRECATION) || \
	defined(GLES_SILENCE_DEPRECATION)
	#define COREVIDEO_GL_DEPRECATED(platform, from, to) API_AVAILABLE(platform(from))
#else
	#define COREVIDEO_GL_DEPRECATED(platform, from, to) API_DEPRECATED("OpenGL/OpenGLES is no longer supported. Use Metal APIs instead. (Define COREVIDEO_SILENCE_GL_DEPRECATION to silence these warnings)", platform(from, to))
#endif // COREVIDEO_SILENCE_GL_DEPRECATION

/*!
    @typedef	CVOptionFlags
    @abstract   Flags to be used for the display and render call back functions.
    @discussion ***Values to be defined***
*/
typedef uint64_t CVOptionFlags;

/*!
    @struct         CVSMPTETime
    @abstract       A structure for holding a SMPTE time.
    @field          subframes
                        The number of subframes in the full message.
    @field          subframeDivisor
                        The number of subframes per frame (typically 80).
    @field          counter
                        The total number of messages received.
    @field          type
                        The kind of SMPTE time using the SMPTE time type constants.
    @field          flags
                        A set of flags that indicate the SMPTE state.
    @field          hours
                        The number of hours in the full message.
    @field          minutes
                        The number of minutes in the full message.
    @field          seconds
                        The number of seconds in the full message.
    @field          frames
                        The number of frames in the full message.
*/
struct CVSMPTETime
{
    SInt16  subframes;
    SInt16  subframeDivisor;
    UInt32  counter;
    UInt32  type;
    UInt32  flags;
    SInt16  hours;
    SInt16  minutes;
    SInt16  seconds;
    SInt16  frames;
};
typedef struct CVSMPTETime    CVSMPTETime;

/*!
    @enum           SMPTE Time Types
    @abstract       Constants that describe the type of SMPTE time.
    @constant       kCVSMPTETimeType24
                        24 Frame
    @constant       kCVSMPTETimeType25
                        25 Frame
    @constant       kCVSMPTETimeType30Drop
                        30 Drop Frame
    @constant       kCVSMPTETimeType30
                        30 Frame
    @constant       kCVSMPTETimeType2997
                        29.97 Frame
    @constant       kCVSMPTETimeType2997Drop
                        29.97 Drop Frame
    @constant       kCVSMPTETimeType60
                        60 Frame
    @constant       kCVSMPTETimeType5994
                        59.94 Frame
*/
typedef CF_ENUM(uint32_t, CVSMPTETimeType)
{
    kCVSMPTETimeType24        = 0,
    kCVSMPTETimeType25        = 1,
    kCVSMPTETimeType30Drop    = 2,
    kCVSMPTETimeType30        = 3,
    kCVSMPTETimeType2997      = 4,
    kCVSMPTETimeType2997Drop  = 5,
    kCVSMPTETimeType60        = 6,
    kCVSMPTETimeType5994      = 7
};

/*!
    @enum           SMPTE State Flags
    @abstract       Flags that describe the SMPTE time state.
    @constant       kCVSMPTETimeValid
                        The full time is valid.
    @constant       kCVSMPTETimeRunning
                        Time is running.
*/
typedef CF_OPTIONS(uint32_t, CVSMPTETimeFlags)
{
    kCVSMPTETimeValid     = (1L << 0),
    kCVSMPTETimeRunning   = (1L << 1)
};


typedef CF_OPTIONS(int32_t, CVTimeFlags) {
    kCVTimeIsIndefinite = 1 << 0
};

typedef struct
{
    int64_t	    timeValue;
    int32_t	    timeScale;
    int32_t	    flags;
} CVTime;

/*!
    @struct CVTimeStamp
    @abstract CoreVideo uses a CVTimeStamp structure to store video display time stamps.
    @discussion This structure is purposely very similar to AudioTimeStamp defined in the CoreAudio framework. 
		Most of the CVTimeStamp struct should be fairly self-explanatory. However, it is probably worth pointing out that unlike the audio time stamps, floats are not used to represent the video equivalent of sample times. This was done partly to avoid precision issues, and partly because QuickTime still uses integers for time values and time scales. In the actual implementation it has turned out to be very convenient to use integers, and we can represent framerates like NTSC (30000/1001 fps) exactly. The mHostTime structure field uses the same Mach absolute time base that is used in CoreAudio, so that clients of the CoreVideo API can synchronize between the two subsystems.
    @field version The current CVTimeStamp is version 0.
    @field videoTimeScale The scale (in units per second) of the videoTime and videoPeriod values
    @field videoTime This represents the start of a frame (or field for interlaced)
    @field hostTime Host root timebase time
    @field rateScalar This is the current rate of the device as measured by the timestamps, divided by the nominal rate
    @field videoRefreshPeriod This is the nominal update period of the current output device
    @field smpteTime SMPTE time representation of the time stamp. 
    @field flags Possible values are:		
		kCVTimeStampVideoTimeValid
		kCVTimeStampHostTimeValid
		kCVTimeStampSMPTETimeValid
		kCVTimeStampVideoPeriodValid
		kCVTimeStampRateScalarValid
		There are flags for each field to make it easier to detect interlaced vs progressive output
		kCVTimeStampTopField
		kCVTimeStampBottomField
		Some commonly used combinations of timestamp flags
		kCVTimeStampVideoHostTimeValid
		kCVTimeStampIsInterlaced
    @field reserved Reserved. Do not use.

*/
typedef struct
{
    uint32_t			version;
    int32_t			videoTimeScale;
    int64_t			videoTime;
    uint64_t			hostTime;
    double			rateScalar;
    int64_t			videoRefreshPeriod;
    CVSMPTETime			smpteTime;
    uint64_t			flags;
    uint64_t			reserved;
} CVTimeStamp; 

// Flags for the CVTimeStamp structure
typedef CF_OPTIONS(uint64_t, CVTimeStampFlags)
{
    kCVTimeStampVideoTimeValid              = (1L << 0),
    kCVTimeStampHostTimeValid               = (1L << 1),
    kCVTimeStampSMPTETimeValid              = (1L << 2),
    kCVTimeStampVideoRefreshPeriodValid     = (1L << 3),
    kCVTimeStampRateScalarValid             = (1L << 4),
    
    // There are flags for each field to make it easier to detect interlaced vs progressive output
    kCVTimeStampTopField                    = (1L << 16),
    kCVTimeStampBottomField                 = (1L << 17),

    // Some commonly used combinations of timestamp flags
    kCVTimeStampVideoHostTimeValid          = (kCVTimeStampVideoTimeValid | kCVTimeStampHostTimeValid),
    kCVTimeStampIsInterlaced                = (kCVTimeStampTopField | kCVTimeStampBottomField)
};

CV_EXPORT const CVTime kCVZeroTime;
CV_EXPORT const CVTime kCVIndefiniteTime;

#if defined(__cplusplus)
}
#endif

#endif
