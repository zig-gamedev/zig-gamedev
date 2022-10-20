/*
     File:       OSServices/Power.h
 
     Contains:   *** DEPRECATED *** Power Manager Interfaces.
 
     Copyright:  (c) 1990-2012 Apple Inc. All rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __POWER__
#define __POWER__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif

#ifndef __MULTIPROCESSING__
#include <CarbonCore/Multiprocessing.h>
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

enum {
                                        /* commands to SleepQRec sleepQProc */
  kSleepRequest                 = 1,
  kSleepDemand                  = 2,
  kSleepWakeUp                  = 3,
  kSleepRevoke                  = 4,
  kSleepUnlock                  = 4,
  kSleepDeny                    = 5,    /* A non-zero value clients can use to deny requests*/
  kSleepNow                     = 6,
  kDozeDemand                   = 7,
  kDozeWakeUp                   = 8,
  kDozeRequest                  = 9,    /* additional messages for Power Mgr 2.0*/
  kEnterStandby                 = 10,   /* Idle Queue Only*/
  kEnterRun                     = 11,   /* Idle Queue Only*/
  kSuspendRequest               = 12,
  kSuspendDemand                = 13,
  kSuspendRevoke                = 14,
  kSuspendWakeUp                = 15,
  kGetPowerLevel                = 16,
  kSetPowerLevel                = 17,
  kDeviceInitiatedWake          = 18,
  kWakeToDoze                   = 19,
  kDozeToFullWakeUp             = 20,
  kGetPowerInfo                 = 21,
  kGetWakeOnNetInfo             = 22,
  kSuspendWakeToDoze            = 23,
  kEnterIdle                    = 24,   /* Idle Queue Only*/
  kStillIdle                    = 25,   /* Idle Queue Only*/
  kExitIdle                     = 26    /* Idle Queue Only*/
};

enum {
                                        /* SleepQRec.sleepQFlags */
  noCalls                       = 1,
  noRequest                     = 2,
  slpQType                      = 16,
  sleepQType                    = 16
};

/* System Activity Selectors */
/* Notes:  The IdleActivity selector is not available unless the hasAggressiveIdling PMFeatures bit is set.     */
/* Use IdleActivity where you used to use OverallAct if necessary. OverallAct will only delay power cycling     */
/* if it's enabled, and will delay sleep by a small amount when hasAggressiveIdling is set.  Don't use          */
/* IdleActivity unless hasAggressiveIdling is set; when hasAggressiveIdling is not set, the use of IdleActivity */
/* is undefined, and well do different things depending on which Power Manager is currently running.            */
enum {
  OverallAct                    = 0,    /* Delays idle sleep by small amount                          */
  UsrActivity                   = 1,    /* Delays idle sleep and dimming by timeout time              */
  NetActivity                   = 2,    /* Delays idle sleep and power cycling by small amount        */
  HDActivity                    = 3,    /* Delays hard drive spindown and idle sleep by small amount  */
  IdleActivity                  = 4     /* Delays idle sleep by timeout time                          */
};

typedef struct SleepQRec                SleepQRec;
typedef SleepQRec *                     SleepQRecPtr;
typedef CALLBACK_API( long , SleepQProcPtr )(long message, SleepQRecPtr qRecPtr);
typedef STACK_UPP_TYPE(SleepQProcPtr)   SleepQUPP;

/*
 *  NewSleepQUPP()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern SleepQUPP
NewSleepQUPP(SleepQProcPtr userRoutine)                       API_DEPRECATED("No longer supported", macos(10.0,10.5) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  DisposeSleepQUPP()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeSleepQUPP(SleepQUPP userUPP)                           API_DEPRECATED("No longer supported", macos(10.0,10.5) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  InvokeSleepQUPP()   *** DEPRECATED ***
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern long
InvokeSleepQUPP(
  long          message,
  SleepQRecPtr  qRecPtr,
  SleepQUPP     userUPP)                                      API_DEPRECATED("No longer supported", macos(10.0,10.5) ) API_UNAVAILABLE( ios, tvos, watchos );


#ifdef __cplusplus
  inline SleepQUPP                                            NewSleepQUPP(SleepQProcPtr userRoutine) { return userRoutine; }
  inline void                                                 DisposeSleepQUPP(SleepQUPP) { }
  inline long                                                 InvokeSleepQUPP(long message, SleepQRecPtr qRecPtr, SleepQUPP userUPP) { return (*userUPP)(message, qRecPtr); }
#else
  #define NewSleepQUPP(userRoutine)                           ((SleepQUPP)userRoutine)
  #define DisposeSleepQUPP(userUPP)
  #define InvokeSleepQUPP(message, qRecPtr, userUPP)          (*userUPP)(message, qRecPtr)
#endif

struct SleepQRec {
  SleepQRecPtr        sleepQLink;             /* pointer to next queue element           */
  short               sleepQType;             /* queue element type (must be SleepQType) */
  SleepQUPP           sleepQProc;             /* pointer to sleep universal proc ptr     */
  short               sleepQFlags;            /* flags                                   */
};

/*
 *  GetCPUSpeed()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use sysctlbyname("hw.cpufrequency"). Found in <sys/sysctl.h>.
 *  
 *  Discussion:
 *    GetCPUSpeed() returns the current effective clock speed of the
 *    CPU in megahertz.
 *  
 *  Result:
 *    the current effective clock speed of the CPU in megahertz.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
GetCPUSpeed(void)                                             API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  SleepQInstall()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use IORegisterForSystemPower(). Found in <IOKit/pwr_mgt/IOPMLib.h>.
 *  
 *  Discussion:
 *    Adds an entry to the sleep queue.
 *  
 *  Parameters:
 *    
 *    qRecPtr:
 *      A pointer to a sleep queue record to be installed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SleepQInstall(SleepQRecPtr qRecPtr)                           API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  SleepQRemove()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use IODeregisterForSystemPower(). Found in <IOKit/pwr_mgt/IOPMLib.h>.
 *  
 *  Discussion:
 *    Remove an entry from the sleep queue.
 *  
 *  Parameters:
 *    
 *    qRecPtr:
 *      A pointer to a sleep queue record to be removed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SleepQRemove(SleepQRecPtr qRecPtr)                            API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  MaximumProcessorSpeed()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use sysctlbyname("hw.cpufrequency_max"). Found in <sys/sysctl.h>.
 *  
 *  Discussion:
 *    MaximumProcessorSpeed() returns the maximum effective clock speed
 *    of the CPU in megahertz.
 *  
 *  Result:
 *    the maximum effective clock speed of the CPU in megahertz.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in PowerMgrLib 1.0 and later
 */
extern short 
MaximumProcessorSpeed(void)                                   API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  MinimumProcessorSpeed()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use sysctlbyname("hw.cpufrequency_min"). Found in <sys/sysctl.h>.
 *  
 *  Discussion:
 *    MinimumProcessorSpeed() returns the minimum effective clock speed
 *    of the CPU in megahertz. Before Mac OS X 10.4, this function
 *    always returns the maximum cpu speed, not the minimum as expected.
 *  
 *  Result:
 *    the minimum effective clock speed of the CPU in megahertz.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in PowerMgrLib 1.0 and later
 */
extern short 
MinimumProcessorSpeed(void)                                   API_DEPRECATED("No longer supported", macos(10.1,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  CurrentProcessorSpeed()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use sysctlbyname("hw.cpufrequency"). Found in <sys/sysctl.h>.
 *  
 *  Discussion:
 *    CurrentProcessorSpeed() returns the current effective clock speed
 *    of the CPU in megahertz. Before Mac OS X 10.4, this function
 *    always returns the maximum cpu speed, not the actual current
 *    speed the processor is running at.  One MHz represents one
 *    million cycles per second.
 *  
 *  Result:
 *    the current effective clock speed of the CPU in megahertz.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in PowerMgrLib 1.0 and later
 */
extern short 
CurrentProcessorSpeed(void)                                   API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  BatteryCount()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use IOPowerSources API. Found in <IOKit/ps/IOPowerSources.h>.
 *  
 *  Summary:
 *    Return the count of batteries installed on this computer.
 *  
 *  Result:
 *    the count of batteries installed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in PowerMgrLib 1.0 and later
 */
extern short 
BatteryCount(void)                                            API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  UpdateSystemActivity()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use IOPMAssertionCreateWithName(). Found in <IOKit/pwr_mgt/IOPMLib.h>.
 *  
 *  Summary:
 *    You can use the UpdateSystemActivity function to notify the Power
 *    Manager that activity has taken place.
 *
 *  Discussion:
 *    The UpdateSystemActivity function is used to notify the Power
 *    Manager that activity has taken place and the timers used to
 *    measure idle time should be updated to the time of this call.
 *    This function can be used by device drivers to prevent the
 *    computer from entering a low-power mode while critical activity
 *    is taking place on a particular device. The function is passed a
 *    parameter indicating the type of activity that has
 *    occurred.
 *
 *    This function is slightly different from DelaySystemIdle, which
 *    should be used to prevent sleep or idle during a critical
 *    section. UpdateSystemActivity simply updates the tick count for
 *    the activity type selected. Conversely, DelaySystemIdle actually
 *    moves the counter to some number of ticks into the future, which
 *    allows the caller to go off and do somethingwithout fear of
 *    idling.
 *
 *    The valid types of activity are:
 *    Value Name       Value        Description
 *    OverallAct       0            general type of activity
 *    UsrActivity      1            user activity (i.e.keyboard or mouse)
 *    NetActivity      2            interaction with network(s)
 *    HDActivity       3            hard disk or storage device in use
 *
 *  Parameters:
 *
 *    activity:
 *      The type of activity which has occurred.
 *
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.8
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in PowerMgrLib 1.0 and later
 */
extern OSErr 
UpdateSystemActivity(UInt8 activity)                          API_DEPRECATED("No longer supported", macos(10.0,10.8) ) API_UNAVAILABLE( ios, tvos, watchos );


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __POWER__ */

