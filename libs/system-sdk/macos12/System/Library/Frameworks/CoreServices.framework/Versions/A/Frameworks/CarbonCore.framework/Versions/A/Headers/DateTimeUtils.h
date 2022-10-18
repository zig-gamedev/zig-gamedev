/*
     File:       CarbonCore/DateTimeUtils.h
 
     Contains:   International Date and Time Interfaces (previously in TextUtils)
 
     Copyright:  © 1994-2011 by Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __DATETIMEUTILS__
#define __DATETIMEUTILS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __UTCUTILS__
#include <CarbonCore/UTCUtils.h>
#endif

#ifndef __CFDATE__
#include <CoreFoundation/CFDate.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*

    Here are the current routine names and the translations to the older forms.
    Please use the newer forms in all new code and migrate the older names out of existing
    code as maintainance permits.
    
    New Name                    Old Name(s)
    
    DateString                  IUDatePString IUDateString 
    InitDateCache
    LongDateString              IULDateString
    LongTimeString              IULTimeString
    StringToDate                String2Date
    StringToTime                                
    TimeString                  IUTimeString IUTimePString
    LongDateToSeconds           LongDate2Secs
    LongSecondsToDate           LongSecs2Date
    DateToSeconds               Date2Secs
    SecondsToDate               Secs2Date


    Carbon only supports the new names.  The old names are undefined for Carbon targets.
    This is true for C, Assembly and Pascal.
    
    InterfaceLib always has exported the old names.  For C macros have been defined to allow
    the use of the new names.  For Pascal and Assembly using the new names will result
    in link errors. 
    
*/

typedef SInt16 ToggleResults;
enum {
                                        /* Toggle results */
  toggleUndefined               = 0,
  toggleOK                      = 1,
  toggleBadField                = 2,
  toggleBadDelta                = 3,
  toggleBadChar                 = 4,
  toggleUnknown                 = 5,
  toggleBadNum                  = 6,
  toggleOutOfRange              = 7,    /*synonym for toggleErr3*/
  toggleErr3                    = 7,
  toggleErr4                    = 8,
  toggleErr5                    = 9
};

enum {
                                        /* Date equates */
  smallDateBit                  = 31,   /*Restrict valid date/time to range of Time global*/
  togChar12HourBit              = 30,   /*If toggling hour by char, accept hours 1..12 only*/
  togCharZCycleBit              = 29,   /*Modifier for togChar12HourBit: accept hours 0..11 only*/
  togDelta12HourBit             = 28,   /*If toggling hour up/down, restrict to 12-hour range (am/pm)*/
  genCdevRangeBit               = 27,   /*Restrict date/time to range used by genl CDEV*/
  validDateFields               = -1,
  maxDateField                  = 10
};

enum {
  eraMask                       = 0x0001,
  yearMask                      = 0x0002,
  monthMask                     = 0x0004,
  dayMask                       = 0x0008,
  hourMask                      = 0x0010,
  minuteMask                    = 0x0020,
  secondMask                    = 0x0040,
  dayOfWeekMask                 = 0x0080,
  dayOfYearMask                 = 0x0100,
  weekOfYearMask                = 0x0200,
  pmMask                        = 0x0400,
  dateStdMask                   = 0x007F /*default for ValidDate flags and ToggleDate TogglePB.togFlags*/
};

typedef SInt8 LongDateField;
enum {
  eraField                      = 0,
  yearField                     = 1,
  monthField                    = 2,
  dayField                      = 3,
  hourField                     = 4,
  minuteField                   = 5,
  secondField                   = 6,
  dayOfWeekField                = 7,
  dayOfYearField                = 8,
  weekOfYearField               = 9,
  pmField                       = 10,
  res1Field                     = 11,
  res2Field                     = 12,
  res3Field                     = 13
};

typedef SInt8 DateForm;
enum {
  shortDate                     = 0,
  longDate                      = 1,
  abbrevDate                    = 2
};

enum {
                                        /* StringToDate status values */
  fatalDateTime                 = 0x8000, /* StringToDate and String2Time mask to a fatal error */
  longDateFound                 = 1,    /* StringToDate mask to long date found */
  leftOverChars                 = 2,    /* StringToDate & Time mask to warn of left over characters */
  sepNotIntlSep                 = 4,    /* StringToDate & Time mask to warn of non-standard separators */
  fieldOrderNotIntl             = 8,    /* StringToDate & Time mask to warn of non-standard field order */
  extraneousStrings             = 16,   /* StringToDate & Time mask to warn of unparsable strings in text */
  tooManySeps                   = 32,   /* StringToDate & Time mask to warn of too many separators */
  sepNotConsistent              = 64,   /* StringToDate & Time mask to warn of inconsistent separators */
  tokenErr                      = 0x8100, /* StringToDate & Time mask for 'tokenizer err encountered' */
  cantReadUtilities             = 0x8200,
  dateTimeNotFound              = 0x8400,
  dateTimeInvalid               = 0x8800
};

typedef short                           StringToDateStatus;
typedef StringToDateStatus              String2DateStatus;
struct DateCacheRecord {
  short               hidden[256];            /* only for temporary use */
};
typedef struct DateCacheRecord          DateCacheRecord;
typedef DateCacheRecord *               DateCachePtr;
struct DateTimeRec {
  short               year;
  short               month;
  short               day;
  short               hour;
  short               minute;
  short               second;
  short               dayOfWeek;
};
typedef struct DateTimeRec              DateTimeRec;

typedef SInt64                          LongDateTime;
#if TARGET_RT_BIG_ENDIAN
union LongDateCvt {
  SInt64              c;
  struct {
    UInt32              lHigh;
    UInt32              lLow;
  }                       hl;
};
typedef union LongDateCvt               LongDateCvt;
#else
union LongDateCvt {
  SInt64              c;
  struct {
    UInt32              lLow;
    UInt32              lHigh;
  }                       hl;
};
typedef union LongDateCvt               LongDateCvt;
#endif  /* TARGET_RT_BIG_ENDIAN */

union LongDateRec {
  struct {
    short               era;
    short               year;
    short               month;
    short               day;
    short               hour;
    short               minute;
    short               second;
    short               dayOfWeek;
    short               dayOfYear;
    short               weekOfYear;
    short               pm;
    short               res1;
    short               res2;
    short               res3;
  }                       ld;
  short               list[14];               /*Index by LongDateField!*/
  struct {
    short               eraAlt;
    DateTimeRec         oldDate;
  }                       od;
};
typedef union LongDateRec               LongDateRec;

typedef SInt8                           DateDelta;
struct TogglePB {
  long                togFlags;               /*caller normally sets low word to dateStdMask=$7F*/
  ResType             amChars;                /*from 'itl0', but uppercased*/
  ResType             pmChars;                /*from 'itl0', but uppercased*/
  long                reserved[4];
};
typedef struct TogglePB                 TogglePB;
/*
    Conversion utilities between CF and Carbon time types. 
*/
/*
 *  UCConvertUTCDateTimeToCFAbsoluteTime()
 *  
 *  Discussion:
 *    Use UCConvertUTCDateTimeToCFAbsoluteTime to convert from a
 *    UTCDDateTime to a CFAbsoluteTime. Remember that the epoch for
 *    UTCDateTime is January 1, 1904 while the epoch for CFAbsoluteTime
 *    is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iUTCDate:
 *      A pointer to a UTCDateTime struct that represents the time you
 *      wish to convert from.
 *    
 *    oCFTime:
 *      A pointer to a CFAbsoluteTime. On successful return, this will
 *      contain the converted time from the input time type.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertUTCDateTimeToCFAbsoluteTime(
  const UTCDateTime *  iUTCDate,
  CFAbsoluteTime *     oCFTime)                               __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  UCConvertSecondsToCFAbsoluteTime()
 *  
 *  Discussion:
 *    Use UCConvertSecondsToCFAbsoluteTime to convert from the normal
 *    seconds representation of time to a CFAbsoluteTime. Remember that
 *    the epoch for seconds is January 1, 1904 while the epoch for
 *    CFAbsoluteTime is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iSeconds:
 *      A UInt32 value that represents the time you wish to convert
 *      from.
 *    
 *    oCFTime:
 *      A pointer to a CFAbsoluteTime. On successful return, this will
 *      contain the converted time from the input time type.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertSecondsToCFAbsoluteTime(
  UInt32            iSeconds,
  CFAbsoluteTime *  oCFTime)                                  __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  UCConvertLongDateTimeToCFAbsoluteTime()
 *  
 *  Discussion:
 *    Use UCConvertLongDateTimeToCFAbsoluteTime to convert from a
 *    LongDateTime to a CFAbsoluteTime. Remember that the epoch for
 *    LongDateTime is January 1, 1904 while the epoch for
 *    CFAbsoluteTime is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iLongTime:
 *      A LongDateTime value that represents the time you wish to
 *      convert from.
 *    
 *    oCFTime:
 *      A pointer to a CFAbsoluteTime. On successful return, this will
 *      contain the converted time from the input time type.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertLongDateTimeToCFAbsoluteTime(
  LongDateTime      iLongTime,
  CFAbsoluteTime *  oCFTime)                                  __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  UCConvertCFAbsoluteTimeToUTCDateTime()
 *  
 *  Discussion:
 *    Use UCConvertCFAbsoluteTimeToUTCDateTime to convert from a
 *    CFAbsoluteTime to a UTCDateTime. Remember that the epoch for
 *    UTCDateTime is January 1, 1904 while the epoch for CFAbsoluteTime
 *    is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iCFTime:
 *      A CFAbsoluteTime value that represents the time you wish to
 *      convert from.
 *    
 *    oUTCDate:
 *      A pointer to a UTCDateTime. On successful return, this will
 *      contain the converted time from the CFAbsoluteTime input.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertCFAbsoluteTimeToUTCDateTime(
  CFAbsoluteTime   iCFTime,
  UTCDateTime *    oUTCDate)                                  __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  UCConvertCFAbsoluteTimeToSeconds()
 *  
 *  Discussion:
 *    Use UCConvertCFAbsoluteTimeToSeconds to convert from a
 *    CFAbsoluteTime to a UInt32 representation of seconds. Remember
 *    that the epoch for seconds is January 1, 1904 while the epoch for
 *    CFAbsoluteTime is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iCFTime:
 *      A CFAbsoluteTime value that represents the time you wish to
 *      convert from.
 *    
 *    oSeconds:
 *      A pointer to a UInt32. On successful return, this will contain
 *      the converted time from the CFAbsoluteTime input.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertCFAbsoluteTimeToSeconds(
  CFAbsoluteTime   iCFTime,
  UInt32 *         oSeconds)                                  __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


/*
 *  UCConvertCFAbsoluteTimeToLongDateTime()
 *  
 *  Discussion:
 *    Use UCConvertCFAbsoluteTimeToLongDateTime to convert from a
 *    CFAbsoluteTime to a LongDateTime. Remember that the epoch for
 *    LongDateTime is January 1, 1904 while the epoch for
 *    CFAbsoluteTime is January 1, 2001.
 *  
 *  Parameters:
 *    
 *    iCFTime:
 *      A CFAbsoluteTime value that represents the time you wish to
 *      convert from.
 *    
 *    oLongDate:
 *      A pointer to a LongDateTime. On successful return, this will
 *      contain the converted time from the CFAbsoluteTime input.
 *  
 *  Result:
 *    A result code indicating whether or not conversion was successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in CoreServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UCConvertCFAbsoluteTimeToLongDateTime(
  CFAbsoluteTime   iCFTime,
  LongDateTime *   oLongDate)                                 __OSX_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);


#if !__LP64__
/*
    These routine are available in Carbon with their new name
*/
#if !__LP64__
/*
 *  DateString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateStringWithDate instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateStringWithDate instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
DateString(
  SInt32     dateTime,
  DateForm   longFlag,
  Str255     result,
  Handle     intlHandle)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  TimeString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateStringWithDate instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateStringWithDate instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
TimeString(
  SInt32    dateTime,
  Boolean   wantSeconds,
  Str255    result,
  Handle    intlHandle)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  LongDateString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateStringWithDate instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateStringWithDate instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
LongDateString(
  const LongDateTime *  dateTime,
  DateForm              longFlag,
  Str255                result,
  Handle                intlHandle)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  LongTimeString()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateStringWithDate instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateStringWithDate instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
LongTimeString(
  const LongDateTime *  dateTime,
  Boolean               wantSeconds,
  Str255                result,
  Handle                intlHandle)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);



/*
    These routine are available in Carbon and InterfaceLib with their new name
*/
/*
 *  InitDateCache()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    No longer needed on MacOS X.
 *  
 *  Discussion:
 *    This function is obsolate.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
InitDateCache(DateCachePtr theCache)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  StringToDate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateDateFromString instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateDateFromString instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringToDateStatus 
StringToDate(
  Ptr            textPtr,
  SInt32         textLen,
  DateCachePtr   theCache,
  SInt32 *       lengthUsed,
  LongDateRec *  dateTime)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  StringToTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFDateFormatterCreateDateFromString instead
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFDateFormatterCreateDateFromString instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern StringToDateStatus 
StringToTime(
  Ptr            textPtr,
  SInt32         textLen,
  DateCachePtr   theCache,
  SInt32 *       lengthUsed,
  LongDateRec *  dateTime)                                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  LongDateToSeconds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LongDateToSeconds(
  const LongDateRec *  lDate,
  LongDateTime *       lSecs)                                 __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  LongSecondsToDate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
LongSecondsToDate(
  const LongDateTime *  lSecs,
  LongDateRec *         lDate)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  ToggleDate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern ToggleResults 
ToggleDate(
  LongDateTime *    lSecs,
  LongDateField     field,
  DateDelta         delta,
  short             ch,
  const TogglePB *  params)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  ValidDate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
ValidDate(
  const LongDateRec *  vDate,
  long                 flags,
  LongDateTime *       newSecs)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReadDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFAbsoluteTimeGetCurrent instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFAbsoluteTimeGetCurrent instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ReadDateTime(unsigned long * datetime)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFAbsoluteTimeGetCurrent instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFAbsoluteTimeGetCurrent instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetDateTime(unsigned long * secs)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetDateTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    deprecated it without replacement
 *  
 *  Discussion:
 *    This function is no longer recommended.  One has to be root on
 *    MacOSX to set. This is done through other means on OS X
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetDateTime(unsigned long datetime)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    deprecated it without replacement
 *  
 *  Discussion:
 *    This function is no longer recommended.  One has to be root on
 *    MacOSX to set. This is done through other means on OS X
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetTime(const DateTimeRec * d)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  GetTime()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFAbsoluteTimeGetCurrent instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use
 *    CFAbsoluteTimeGetCurrent instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetTime(DateTimeRec * d)                                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  DateToSeconds()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DateToSeconds(
  const DateTimeRec *  d,
  unsigned long *      secs)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  SecondsToDate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use CFCalendar instead.
 *  
 *  Discussion:
 *    This function is no longer recommended. Please use CFCalendar
 *    instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SecondsToDate(
  unsigned long   secs,
  DateTimeRec *   d)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
    These routine are available in InterfaceLib using their old name.
    Macros allow using the new names in all source code.
    
    @deprecated use CFDateFormatter instead.
*/
/*
 *  IUDateString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUTimeString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUDatePString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IUTimePString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IULDateString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  IULTimeString()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only] but deprecated in 10.3
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */



#endif  /* !__LP64__ */

#if CALL_NOT_IN_CARBON
#define DateString(dateTime, longFlag, result, intlHandle) \
         IUDatePString( dateTime, longFlag, result, intlHandle)
#define TimeString(dateTime, wantSeconds, result, intlHandle) \
         IUTimePString(dateTime, wantSeconds, result, intlHandle)
#define LongDateString(dateTime, longFlag, result, intlHandle) \
         IULDateString(dateTime, longFlag, result, intlHandle)
#define LongTimeString(dateTime, wantSeconds, result, intlHandle) \
         IULTimeString(dateTime, wantSeconds, result, intlHandle)
#endif /* CALL_NOT_IN_CARBON */
#if OLDROUTINENAMES
#define String2Date(textPtr, textLen, theCache, lengthUsed, dateTime)  \
         StringToDate(textPtr, textLen, theCache, lengthUsed, dateTime)
#define String2Time(textPtr, textLen, theCache, lengthUsed, dateTime)  \
         StringToTime(textPtr, textLen, theCache, lengthUsed, dateTime)
#define LongDate2Secs(lDate, lSecs) LongDateToSeconds(lDate, lSecs)
#define LongSecs2Date(lSecs, lDate) LongSecondsToDate(lSecs, lDate)
#define Date2Secs(d, secs) DateToSeconds(d, secs)
#define Secs2Date(secs, d) SecondsToDate(secs, d)
#endif  /* OLDROUTINENAMES */

/*
 *  iudatestring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iudatepstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iutimestring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iutimepstring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iuldatestring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


/*
 *  iultimestring()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */


#endif  /* !__LP64__ */



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __DATETIMEUTILS__ */

