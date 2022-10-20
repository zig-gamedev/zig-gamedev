/*==================================================================================================
     File:       CoreAudio/HostTime.h

     Contains:   Routines for accessing the host's time base

     Copyright:  (c) 1985-2010 by Apple, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

==================================================================================================*/
#if !defined(CoreAudio_HostTime_h)
#define CoreAudio_HostTime_h

//==================================================================================================

/*!
    @header HostTime
    This collection of functions provides access to the host's time base. It also provides
    discriptive information about the time base and translations to and from nanoseconds.
*/

//==================================================================================================
#pragma mark -
#pragma mark Includes

#include <Availability.h>
#include <CoreAudioTypes/CoreAudioTypes.h>

//==================================================================================================

#if defined(__cplusplus)
extern "C"
{
#endif

//==================================================================================================
#pragma mark -
#pragma mark    Functions

/*!
    @functiongroup  HostTime
*/

/*!
    @function       AudioGetCurrentHostTime
    @abstract       Gets the current host time.
    @result         A UInt64 containing the current host time.
*/
extern UInt64
AudioGetCurrentHostTime(void)                                                                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioGetHostClockFrequency
    @abstract       Gets the number of ticks per second in the host time base.
    @result         A Float64 containing the number of ticks per second in the host time base.
*/
extern Float64
AudioGetHostClockFrequency(void)                                                                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioGetHostClockMinimumTimeDelta
    @abstract       Gets the smallest number of ticks that two succeeding values will ever differ.
                    by.
    @result         A UInt32 containing the smallest number of ticks that two succeeding values will
                    ever differ.
*/
extern UInt32
AudioGetHostClockMinimumTimeDelta(void)                                                             __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioConvertHostTimeToNanos
    @abstract       Convert the given host time into a time in nanoseconds.
    @param          inHostTime
                        A UInt64 containing the host time to convert.
    @result         A UInt64 containining the converted host time.
*/
extern UInt64
AudioConvertHostTimeToNanos(UInt64 inHostTime)                                                      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/*!
    @function       AudioConvertNanosToHostTime
    @abstract       Convert the given nanosecond time into a host time.
    @param          inNanos
                        A UInt64 containing the nanosecond time to convert.
    @result         A UInt64 containining the converted nanosecond time.
*/
extern UInt64
AudioConvertNanosToHostTime(UInt64 inNanos)                                                         __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

//==================================================================================================

#if defined(__cplusplus)
}
#endif

#endif  //  CoreAudio_HostTime_h
