/*
     File:       CarbonCore/DriverServices.h
 
     Contains:   Driver Services Interfaces.
                 The contents of this header file are deprecated.
                 Use CFAbsoluteTime or mach time routines instead.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __DRIVERSERVICES__
#define __DRIVERSERVICES__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACERRORS__
#include <CarbonCore/MacErrors.h>
#endif

#ifndef __MACHINEEXCEPTIONS__
#include <CarbonCore/MachineExceptions.h>
#endif

#ifndef __DRIVERSYNCHRONIZATION__
#include <CarbonCore/DriverSynchronization.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma options align=power


enum {
  durationMicrosecond           = -1L,  /* Microseconds are negative*/
  durationMillisecond           = 1,    /* Milliseconds are positive*/
  durationSecond                = 1000, /* 1000 * durationMillisecond*/
  durationMinute                = 60000, /* 60 * durationSecond,*/
  durationHour                  = 3600000, /* 60 * durationMinute,*/
  durationDay                   = 86400000, /* 24 * durationHour,*/
  durationNoWait                = 0,    /* don't block*/
  durationForever               = 0x7FFFFFFF /* no time limit*/
};


typedef UnsignedWide                    Nanoseconds;
/*
 *  UpTime()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
UpTime(void)                                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetTimeBaseInfo()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */



/*
 *  AbsoluteToNanoseconds()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Nanoseconds 
AbsoluteToNanoseconds(AbsoluteTime absoluteTime)              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AbsoluteToDuration()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Duration 
AbsoluteToDuration(AbsoluteTime absoluteTime)                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NanosecondsToAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
NanosecondsToAbsolute(Nanoseconds nanoseconds)                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DurationToAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
DurationToAbsolute(Duration duration)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddAbsoluteToAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
AddAbsoluteToAbsolute(
  AbsoluteTime   absoluteTime1,
  AbsoluteTime   absoluteTime2)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SubAbsoluteFromAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
SubAbsoluteFromAbsolute(
  AbsoluteTime   leftAbsoluteTime,
  AbsoluteTime   rightAbsoluteTime)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddNanosecondsToAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
AddNanosecondsToAbsolute(
  Nanoseconds    nanoseconds,
  AbsoluteTime   absoluteTime)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AddDurationToAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
AddDurationToAbsolute(
  Duration       duration,
  AbsoluteTime   absoluteTime)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SubNanosecondsFromAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
SubNanosecondsFromAbsolute(
  Nanoseconds    nanoseconds,
  AbsoluteTime   absoluteTime)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SubDurationFromAbsolute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern AbsoluteTime 
SubDurationFromAbsolute(
  Duration       duration,
  AbsoluteTime   absoluteTime)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AbsoluteDeltaToNanoseconds()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Nanoseconds 
AbsoluteDeltaToNanoseconds(
  AbsoluteTime   leftAbsoluteTime,
  AbsoluteTime   rightAbsoluteTime)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  AbsoluteDeltaToDuration()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Duration 
AbsoluteDeltaToDuration(
  AbsoluteTime   leftAbsoluteTime,
  AbsoluteTime   rightAbsoluteTime)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DurationToNanoseconds()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Nanoseconds 
DurationToNanoseconds(Duration theDuration)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  NanosecondsToDuration()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 8.6 and later
 */
extern Duration 
NanosecondsToDuration(Nanoseconds theNanoseconds)             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);





#pragma options align=reset

#ifdef __cplusplus
}
#endif

#endif /* __DRIVERSERVICES__ */

