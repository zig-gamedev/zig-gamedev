/*
     File:       CarbonCore/UTCUtils.h
 
     Contains:   Interface for UTC to Local Time conversion and 64 Bit Clock routines
 
     Copyright:  © 1999-2011 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __UTCUTILS__
#define __UTCUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACERRORS__
#include <CarbonCore/MacErrors.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* Options for Set & Get DateTime Routines */
enum {
  kUTCDefaultOptions            = 0
};

/* 64 Bit Clock Typedefs */
struct UTCDateTime {
  UInt16              highSeconds;
  UInt32              lowSeconds;
  UInt16              fraction;
};
typedef struct UTCDateTime              UTCDateTime;
typedef UTCDateTime *                   UTCDateTimePtr;
typedef UTCDateTimePtr *                UTCDateTimeHandle;
struct LocalDateTime {
  UInt16              highSeconds;
  UInt32              lowSeconds;
  UInt16              fraction;
};
typedef struct LocalDateTime            LocalDateTime;
typedef LocalDateTime *                 LocalDateTimePtr;
typedef LocalDateTimePtr *              LocalDateTimeHandle;
/* Classic 32 bit clock conversion routines */
#if !__LP64__
/*
 *  ConvertLocalTimeToUTC()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use
 *    UCConvertUTCDateTimeToCFAbsoluteTime/CFTimeZoneGetSecondsFromGMT
 *    instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCConvertUTCDateTimeToCFAbsoluteTime and
 *    CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
ConvertLocalTimeToUTC(
  UInt32    localSeconds,
  UInt32 *  utcSeconds)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ConvertUTCToLocalTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use
 *    UCConvertUTCDateTimeToCFAbsoluteTime/CFTimeZoneGetSecondsFromGMT
 *    instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCConvertUTCDateTimeToCFAbsoluteTime and
 *    CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
ConvertUTCToLocalTime(
  UInt32    utcSeconds,
  UInt32 *  localSeconds)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* 64 bit clock conversion routines */
/*
 *  ConvertUTCToLocalDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use
 *    UCConvertUTCDateTimeToCFAbsoluteTime/CFTimeZoneGetSecondsFromGMT
 *    instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCConvertUTCDateTimeToCFAbsoluteTime and
 *    CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
ConvertUTCToLocalDateTime(
  const UTCDateTime *  utcDateTime,
  LocalDateTime *      localDateTime)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  ConvertLocalToUTCDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use
 *    UCConvertUTCDateTimeToCFAbsoluteTime/CFTimeZoneGetSecondsFromGMT
 *    instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    UCConvertUTCDateTimeToCFAbsoluteTime and
 *    CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
ConvertLocalToUTCDateTime(
  const LocalDateTime *  localDateTime,
  UTCDateTime *          utcDateTime)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/* Getter and Setter Clock routines using 64 Bit values */
/*
 *  GetUTCDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFAbsoluteTimeGetCurrent instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFAbsoluteTimeGetCurrent instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
GetUTCDateTime(
  UTCDateTime *  utcDateTime,
  OptionBits     options)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetUTCDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use settimeofday (2) instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Setting the time requires
 *    root privileges. If you must, use settimeofday (2)
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
SetUTCDateTime(
  const UTCDateTime *  utcDateTime,
  OptionBits           options)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetLocalDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFAbsoluteTimeGetCurrent/CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFAbsoluteTimeGetCurrent and CFTimeZoneGetSecondsFromGMT instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
GetLocalDateTime(
  LocalDateTime *  localDateTime,
  OptionBits       options)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetLocalDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    it without replacement
 *  
 *  Discussion:
 *    This function is no longer recommended. There is no replacement.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in UTCUtils 1.0 and later
 */
extern OSStatus 
SetLocalDateTime(
  const LocalDateTime *  localDateTime,
  OptionBits             options)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __UTCUTILS__ */

