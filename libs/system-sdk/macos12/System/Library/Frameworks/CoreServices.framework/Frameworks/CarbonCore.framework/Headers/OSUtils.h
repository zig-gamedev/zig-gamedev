/*
     File:       CarbonCore/OSUtils.h
 
     Contains:   OS Utilities Interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __OSUTILS__
#define __OSUTILS__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

/*  HandToHand and other memory utilties were moved to MacMemory.h */
#ifndef __MACMEMORY__
#include <CarbonCore/MacMemory.h>
#endif

/*  Date and Time utilties were moved to DateTimeUtils.h */
#ifndef __DATETIMEUTILS__
#include <CarbonCore/DateTimeUtils.h>
#endif

#ifndef __CFSTRING__
#include <CoreFoundation/CFString.h>
#endif


#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

enum {
                                        /* result types for RelString Call */
  sortsBefore                   = -1,   /*first string < second string*/
  sortsEqual                    = 0,    /*first string = second string*/
  sortsAfter                    = 1     /*first string > second string*/
};

enum {
  dummyType                     = 0,
  vType                         = 1,
  ioQType                       = 2,
  drvQType                      = 3,
  evType                        = 4,
  fsQType                       = 5,
  sIQType                       = 6,
  dtQType                       = 7,
  nmType                        = 8
};

typedef SignedByte                      QTypes;
struct QElem {
  struct QElem *      qLink;
  short               qType;
  short               qData[1];
};
typedef struct QElem                    QElem;
typedef QElem *                         QElemPtr;
struct QHdr {
  volatile short      qFlags;
  volatile QElemPtr   qHead;
  volatile QElemPtr   qTail;
};
typedef struct QHdr                     QHdr;
typedef QHdr *                          QHdrPtr;
/* 
    In order for MachineLocation to be endian-safe, a new member 
    has been added to the 'u' union in the structure. You are 
    encouraged to use the new member instead of the old one.
    
    If your code looked like this:
    
        MachineLocation.u.dlsDelta = isDLS? 0x80: 0x00;
    
    you should change it to this:
    
        MachineLocation.u.dls.Delta = isDLS? 0x80: 0x00;
    
    to be endian safe. The gmtDelta remains the same; the low 24-bits
    are used. Remember that order of assignment DOES matter:
    
    This will overwrite results:
    
        MachineLocation.u.dls.Delta = 0xAA;         // u = 0xAAGGGGGG; G=Garbage
        MachineLocation.u.gmtDelta = 0xBBBBBB;      // u = 0x00BBBBBB;
    
    when in fact reversing the assignment would have preserved the values:

        MachineLocation.u.gmtDelta = 0xBBBBBB;      // u = 0x00BBBBBB;  
        MachineLocation.u.dls.Delta = 0xAA;         // u = 0xAABBBBBB;
        
    NOTE:   The information regarding dlsDelta in Inside Mac is INCORRECT.
            It's always 0x80 for daylight-saving time or 0x00 for standard time.
*/
struct MachineLocation {
    Fract   latitude;
    Fract   longitude;
    union {
    #if TARGET_RT_BIG_ENDIAN
        SInt8 dlsDelta;
    #endif
        long    gmtDelta;           /* use low 24-bits only */
        struct {
        #if TARGET_RT_LITTLE_ENDIAN
            SInt8   pad[3];
        #endif
            SInt8   Delta;          /* signed byte; daylight savings delta */
        } dls;
    } u;
};
typedef struct MachineLocation MachineLocation;
/*
 *  IsMetric()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CFLocaleGetValue() and the property kCFLocaleUsesMetricSystem
 *    to determine this value.
 *  
 *  Summary:
 *    Verifies whether the current script system is using the metric
 *    system or the English system of measurement.
 *  
 *  Discussion:
 *    The IsMetric function examines the metricSys field of the
 *    numeric-format resource (resource type 'itl0') to determine if
 *    the current script is using the metric system. A value of 255 in
 *    the metricSys field indicates that the metric system
 *    (centimeters, kilometers, milligrams, degrees Celsius, and so on)
 *    is being used. A value of 0 in the metricSys field indicates that
 *    the English system of measurement (inches, miles, ounces, degrees
 *    Fahrenheit, and so on) is used.
 *    If you want to use units of measurement different from that of
 *    the current script, you need to override the value of the
 *    metricSys field in the current numeric-format resource. You can
 *    do this by using your own version of the numeric-format resource
 *    instead of the current script system’s default international
 *    resource.
 *    The IsMetric function is the same as the IUMetric function, which
 *    was previously available with the International Utilities Package.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    TRUE if the metric system is being used; FALSE if the English
 *    system is being used.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.7
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
IsMetric(void)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_7, __IPHONE_NA, __IPHONE_NA);


/*
 *  Delay()
 *  
 *  Summary:
 *    Delays execture for the specified amount of time.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    numTicks:
 *      the number of ticks to delay  for
 *    
 *    finalTicks:
 *      on return, if not NULL, will contain the value of TickCount()
 *      at the end of the delay period
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
Delay(
  unsigned long    numTicks,
  unsigned long *  finalTicks)                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Enqueue()
 *  
 *  Summary:
 *    Atomically adds a queue element to the given queue
 *  
 *  Discussion:
 *    A queue ( represented by a QHdrPtr ) is a singly linked list of
 *    elements.  Enqueue inserts the given element into the queue in a
 *    multi-thread safe way.  If the element is already in the queue,
 *    or in some other queue, the data structures will be corrupted and
 *    will likely cause a crash or infinite loop.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    qElement:
 *      a pointer to the element to be inserted
 *    
 *    qHeader:
 *      a pointer to the queue header.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
Enqueue(
  QElemPtr   qElement,
  QHdrPtr    qHeader)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  Dequeue()
 *  
 *  Summary:
 *    Atomically removes a queue element from the given queue
 *  
 *  Discussion:
 *    A queue ( represented by a QHdrPtr ) is a singly linked list of
 *    elements.  Dequeue removes the given element from the queue in a
 *    multi-thread safe way.  If the element is not in the queue, qErr
 *    is returned.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    qElement:
 *      a pointer to the element to be removed
 *    
 *    qHeader:
 *      a pointer to the queue header.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
Dequeue(
  QElemPtr   qElement,
  QHdrPtr    qHeader)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



#if !__LP64__
/*
 *  MakeDataExecutable()
 *  
 *  Summary:
 *    Notifies the system that the specified data is subject to
 *    execution.
 *  
 *  Discussion:
 *    On some computer architectures it is necessary to tell the
 *    processor that an area of memory should be made executable.  This
 *    function does the necessary operations ( if possible ) to make it
 *    possible to execute code in the given address range.
 *    MakeDataExecutable is not supported for 64-bit applications. Use
 *    sys_icache_invalidate(3) and/or mprotect(2) as appropriate.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    baseAddress:
 *      the starting address to be made executable
 *    
 *    length:
 *      the length of the data pointed to by the baseAddress parameter.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.5 and later
 */
extern void 
MakeDataExecutable(
  void *          baseAddress,
  unsigned long   length)                                     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  ReadLocation()
 *  
 *  Summary:
 *    Obtains information about a geographic location or time zone.
 *  
 *  Discussion:
 *    The latitude and longitude are stored as Fract values, giving
 *    accuracy to within one foot. For example, a Fract value of 1.0
 *    equals 90 degrees –1.0 equals –90 degrees and –2.0 equals –180
 *    degrees.
 *    To convert these values to a degrees format, you need to convert
 *    the Fract values first to the Fixed data type, then to the
 *    LongInt data type. Use the Mathematical and Logical Utilities
 *    functions Fract2Fix and Fix2Long to accomplish this task.
 *    The DST value is a signed byte value that specifies the offset
 *    for the hour field—whether to add one hour, subtract one hour, or
 *    make no change at all.
 *    The GMT value is in seconds east of GMT. For example, San
 *    Francisco is at –28,800 seconds (8 hours * 3,600 seconds per
 *    hour) east of GMT. The gmtDelta field is a 3-byte value contained
 *    in a long word, so you must take care to get it properly.
 *    For more information on the Fract data type and the conversion
 *    routines Long2Fix, Fix2Fract, Fract2Fix, and Fix2Long, see
 *    Mathematical and Logical Utilities.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    loc:
 *      On return, the fields of the geographic location structure
 *      containing the geographic location and the time-zone
 *      information.
 *      You can get values for the latitude, longitude, daylight
 *      savings time (DST), or Greenwich mean time (GMT). If the
 *      geographic location record has never been set, all fields
 *      contain 0.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ReadLocation(MachineLocation * loc)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  TickCount()
 *  
 *  Summary:
 *    Obtains the current number of ticks (a tick is approximately 1/60
 *    of a second) approximately since the system last started up.
 *  
 *  Discussion:
 *    The TickCount function returns an unsigned 32-bit integer that
 *    indicates the current number of ticks since the system last
 *    started up. You can use this value to compare the number of ticks
 *    that have elapsed since a given event or other action occurred.
 *    For example, you could compare the current value returned by
 *    TickCount with the value of the when field of an event
 *    structure.
 *    The tick count rolls over in approximately 2 years 3 months,
 *    which means you should not use this to time intervals which may
 *    exceed ( or even approach ) this interval.
 *    Do not rely on the tick count being exact; it is usually accurate
 *    to within one tick, but this level of accuracy is not guaranteed.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Result:
 *    the tick count
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern UInt32 
TickCount(void)                                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CSCopyUserName()
 *  
 *  Summary:
 *    Returns a reference to the CFString that represents the user name.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Parameters:
 *    
 *    useShortName:
 *      A Boolean value that specifies whether to return the short name
 *      or full name of the user.
 *  
 *  Result:
 *    the requested name in a CFStringRef.  You should release this
 *    when you are done with it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSCopyUserName(Boolean useShortName)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  CSCopyMachineName()
 *  
 *  Summary:
 *    Returns a reference to the CFString that represents the computer
 *    name.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Result:
 *    the name of this machine in a CFStringRef.  You should release
 *    this when you are done with it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern CFStringRef 
CSCopyMachineName(void)                                       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


enum {
  useFree                       = 0,
  useATalk                      = 1,
  useAsync                      = 2,
  useExtClk                     = 3,    /*Externally clocked*/
  useMIDI                       = 4
};

enum {
  false32b                      = 0,    /*24 bit addressing error*/
  true32b                       = 1     /*32 bit addressing error*/
} __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if ALLOW_OBSOLETE_CARBON_OSUTILS
struct SysParmType {
  UInt8               valid;
  UInt8               aTalkA;
  UInt8               aTalkB;
  UInt8               config;
  short               portA;
  short               portB;
  long                alarm;
  short               font;
  short               kbdPrint;
  short               volClik;
  short               misc;
};
typedef struct SysParmType              SysParmType;
typedef SysParmType *                   SysPPtr;
#else
typedef void *                          SysPPtr;
#endif  /* ALLOW_OBSOLETE_CARBON_OSUTILS */

#if TARGET_API_MAC_CARBON
#define GetMMUMode() ((SInt8)true32b)
#define SwapMMUMode(x) (*(SInt8*)(x) = true32b)
#endif
#if !__LP64__
/*
 *  GetSysPPtr()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Don't use this function; it always returns NULL on Mac OS X.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SysPPtr 
GetSysPPtr(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
    NOTE: SysBeep() has been moved to Sound.h.  
 We could not automatically #include Sound.h in this file
 because Sound.h indirectly #include's OSUtils.h which
 would make a circular include.
 */
#endif  /* !__LP64__ */

typedef CALLBACK_API( void , DeferredTaskProcPtr )(long dtParam);
typedef STACK_UPP_TYPE(DeferredTaskProcPtr)                     DeferredTaskUPP;
/*
 *  NewDeferredTaskUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern DeferredTaskUPP
NewDeferredTaskUPP(DeferredTaskProcPtr userRoutine)           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeDeferredTaskUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeDeferredTaskUPP(DeferredTaskUPP userUPP)               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeDeferredTaskUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeDeferredTaskUPP(
  long             dtParam,
  DeferredTaskUPP  userUPP)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline DeferredTaskUPP                                      NewDeferredTaskUPP(DeferredTaskProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeDeferredTaskUPP(DeferredTaskUPP) { }
    inline void                                                 InvokeDeferredTaskUPP(long dtParam, DeferredTaskUPP userUPP) { (*userUPP)(dtParam); }
  #else
    #define NewDeferredTaskUPP(userRoutine)                     ((DeferredTaskUPP)userRoutine)
    #define DisposeDeferredTaskUPP(userUPP)
    #define InvokeDeferredTaskUPP(dtParam, userUPP)             (*userUPP)(dtParam)
  #endif
#endif

struct DeferredTask {
  volatile QElemPtr   qLink;
  short               qType;
  volatile short      dtFlags;
  DeferredTaskUPP     dtAddr;
  long                dtParam;
  long                dtReserved;
};
typedef struct DeferredTask             DeferredTask;
typedef DeferredTask *                  DeferredTaskPtr;
#if !__LP64__
/*
 *  DTInstall()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    The Deferred Task Manager is deprecated.  Look into restructuring
 *    your code to use threads, or MPTasks, or some other threading
 *    solution.
 *  
 *  Summary:
 *    Adds the specified task record to the deferred-task queue.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
DTInstall(DeferredTaskPtr dtTaskPtr)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  DTUninstall()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    The Deferred Task Manager is deprecated.  Look into restructuring
 *    your code to use threads, or MPTasks, or some other threading
 *    solution.
 *  
 *  Summary:
 *    Adds the specified task record to the deferred-task queue.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSErr 
DTUninstall(DeferredTaskPtr dtTaskPtr)                        __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetCurrentA5()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    You no longer need to use SetCurrentA5() on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
SetCurrentA5(void)                                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetA5()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    You no longer need to use SetA5() on Mac OS X.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
SetA5(long newA5)                                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  InitUtil()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    It is not necessary to call InitUtil on Mac OS X.  You should
 *    remove all calls to this from your code.
 *  
 *  Mac OS X threading:
 *    Thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
InitUtil(void)                                                __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


/*
 *  WriteParam()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This function no longer does anything on Mac OS X; you should
 *    remove all calls to it from your code.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
WriteParam(void)                                              __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  WriteLocation()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    WriteLocation can not be used to set the geographic information
 *    on Mac OS X.  You should remove all calls to this function from
 *    your code.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.0
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
WriteLocation(const MachineLocation * loc)                    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_1, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if OLDROUTINENAMES
#define IUMetric() IsMetric()
#endif  /* OLDROUTINENAMES */

#if ALLOW_OBSOLETE_CARBON_OSUTILS
/*
    NOTE: SysEnvirons is obsolete.  You should be using Gestalt.
*/
/* Environs Equates */
enum {
  curSysEnvVers                 = 2     /*Updated to equal latest SysEnvirons version*/
};

struct SysEnvRec {
  short               environsVersion;
  short               machineType;
  short               systemVersion;
  short               processor;
  Boolean             hasFPU;
  Boolean             hasColorQD;
  short               keyBoardType;
  short               atDrvrVersNum;
  short               sysVRefNum;
};
typedef struct SysEnvRec                SysEnvRec;
enum {
                                        /* Machine Types */
  envMac                        = -1,
  envXL                         = -2,
  envMachUnknown                = 0,
  env512KE                      = 1,
  envMacPlus                    = 2,
  envSE                         = 3,
  envMacII                      = 4,
  envMacIIx                     = 5,
  envMacIIcx                    = 6,
  envSE30                       = 7,
  envPortable                   = 8,
  envMacIIci                    = 9,
  envMacIIfx                    = 11
};

enum {
                                        /* CPU types */
  envCPUUnknown                 = 0,
  env68000                      = 1,
  env68010                      = 2,
  env68020                      = 3,
  env68030                      = 4,
  env68040                      = 5
};

enum {
                                        /* Keyboard types */
  envUnknownKbd                 = 0,
  envMacKbd                     = 1,
  envMacAndPad                  = 2,
  envMacPlusKbd                 = 3,
  envAExtendKbd                 = 4,
  envStandADBKbd                = 5,
  envPrtblADBKbd                = 6,
  envPrtblISOKbd                = 7,
  envStdISOADBKbd               = 8,
  envExtISOADBKbd               = 9
};

#endif  /* ALLOW_OBSOLETE_CARBON_OSUTILS */




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __OSUTILS__ */

