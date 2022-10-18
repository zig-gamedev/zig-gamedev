/*
     File:       CarbonCore/Timer.h
 
     Contains:   Time Manager interfaces.
                 The contents of this header file are deprecated.
 
     Copyright:  © 1985-2011 by Apple Inc. All rights reserved.
*/
#ifndef __TIMER__
#define __TIMER__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __OSUTILS__
#include <CarbonCore/OSUtils.h>
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
 *  Microseconds()
 *  
 *  Summary:
 *    Determines the number of microseconds that have elapsed since
 *    system startup time.
 *  
 *  Discussion:
 *    Return a value representing the number of microseconds since some
 *    point in time, usually since the system was booted.  One
 *    microsecond is 1 * 10^-6 seconds, and so there are one million
 *    ( 1,000,000 ) microseconds per second.  For reference, in
 *    one microsecond light can travel about 850 feet in a vacuum.
 *     
 *    Microseconds() doesn't necessarily advance while the computer is
 *    asleep, so it should not be used for long duration timings.
 *  
 *  Parameters:
 *    
 *    microTickCount:
 *      The number of microseconds elapsed since system startup.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
Microseconds(UnsignedWide * microTickCount)                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/****************************************************************************

    The remaining functions in this file have all been deprecated on Mac
    OS X 10.4.  There are other solutions which perform better, and consume
    fewer system resources which are recommended.  The Time Manager on
    Mac OS X does not have exactly the same behavior as it did on Mac OS 9
    and earlier, especially in a multithreaded process.
    
    Instead of using the Time Manager functions, you should consider
    the following:

    1.  If you want a function to be called periodically from an
        application, then look at using a CFRunLoopTimer ( in CFRunLoop.h ).

    2.  In a Cocoa application, you can use the NSTimer object to get
        both absolute and interval-based periodic callbacks.
    
    3.  If you need something to happen periodically, and don't have a
        CFRunLoop in your application, you can create a thread which sleeps
        for whatever interval you want, then does something and sleeps again
        ( or exits, or whatever. )
        Conceptually, this is all the current Time Manager does -- when you
        prime a task, a thread is created which sleeps on a semaphore for the
        amount of time remaining before the next earliest Time Manager task,
        then calls the tmTask function for that task and then calculates the
        time until the next task, ad nauseum.
        
    4. If you just want to delay for some period of time, and don't have
        other threads or data which may require synchronization, you can call
        Delay(), sleep(), usleep(), or nanosleep().
        
    5. 
    - 
 ****************************************************************************/
enum {
                                        /* high bit of qType is set if task is active */
  kTMTaskActive                 = (1L << 15)
};

typedef struct TMTask                   TMTask;
typedef TMTask *                        TMTaskPtr;
typedef CALLBACK_API( void , TimerProcPtr )(TMTaskPtr tmTaskPtr);
typedef STACK_UPP_TYPE(TimerProcPtr)                            TimerUPP;
struct TMTask {
  QElemPtr            qLink;
  short               qType;
  TimerUPP            tmAddr;
  long                tmCount;
  long                tmWakeUp;
  long                tmReserved;
};

/*
 *  InsTime()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Use InstallTimeTask() instead of this function.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
InsTime(QElemPtr tmTaskPtr)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  InsXTime()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Use InstallXTimeTask() instead of this function.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
InsXTime(QElemPtr tmTaskPtr)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PrimeTime()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Use PrimeTimeTask() instead of this function.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PrimeTime(
  QElemPtr   tmTaskPtr,
  long       count)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  RmvTime()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    Use RemoveTimeTask() instead of this function.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
RmvTime(QElemPtr tmTaskPtr)                                   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  InstallTimeTask()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Installs a task, taking advantage of the drift-free,
 *    fixed-frequency timing services of the extended Time Manager.
 *  
 *  Discussion:
 *    The InstallXTimeTask function adds the Time Manager task
 *    structure specified by tmTaskPtr to the Time Manager queue. Use
 *    InstallXTimeTask only if you wish to use the drift-free,
 *    fixed-frequency timing services of the extended Time Manager; use
 *    InstallTimeTask in all other cases. The tmTaskPtr parameter must
 *    point to an extended Time Manager task structure. Your
 *    application must fill in the tmAddr field of that task. You
 *    should set the tmWakeUp and tmReserved fields to 0 the first time
 *    you call InsXTime.
 *    With the extended Time Manager, you can set tmAddr to NULL if you
 *    do not want a task to execute when the delay passed to PrimeTime
 *    expires. Also, InsXTime resets the high-order bit of the qType
 *    field to 0.
 *    The InstallXTimeTask function, which returns a value of type
 *    OSErr, takes the place of InsXTime.
 *  
 *  Parameters:
 *    
 *    tmTaskPtr:
 *      A pointer to an extended task structure to be installed in the
 *      queue.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
InstallTimeTask(QElemPtr tmTaskPtr)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  InstallXTimeTask()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Installs a task, taking advantage of the drift-free,
 *    fixed-frequency timing services of the extended Time Manager.
 *  
 *  Discussion:
 *    The InstallXTimeTask function adds the Time Manager task
 *    structure specified by tmTaskPtr to the Time Manager queue. Use
 *    InstallXTimeTask only if you wish to use the drift-free,
 *    fixed-frequency timing services of the extended Time Manager; use
 *    InstallTimeTask in all other cases. The tmTaskPtr parameter must
 *    point to an extended Time Manager task structure. Your
 *    application must fill in the tmAddr field of that task. You
 *    should set the tmWakeUp and tmReserved fields to 0 the first time
 *    you call InsXTime.
 *    With the extended Time Manager, you can set tmAddr to NULL if you
 *    do not want a task to execute when the delay passed to PrimeTime
 *    expires. Also, InsXTime resets the high-order bit of the qType
 *    field to 0.
 *    The InstallXTimeTask function, which returns a value of type
 *    OSErr, takes the place of InsXTime.
 *    
 *    
 *    
 *    
 *    
 *    
 *    *************************************************************
 *     See the discussion at the top of this file for information about
 *    why this call is deprecated on Mac OS X, and what other system
 *    facilities you can use in your code instead of the Time Manager.
 *    *****************************************************************
 *  
 *  Parameters:
 *    
 *    tmTaskPtr:
 *      A pointer to an extended task structure to be installed in the
 *      queue.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
InstallXTimeTask(QElemPtr tmTaskPtr)                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  PrimeTimeTask()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Activates a task in the Time Manager queue.
 *  
 *  Discussion:
 *    The PrimeTimeTask function schedules the task specified by the
 *    tmAddr field of the structure pointed to by the tmTaskPtr
 *    parameter for execution after the delay specified by the count
 *    parameter has elapsed.
 *    
 *    If the count parameter is a positive value, it is interpreted as
 *    milliseconds. If count is a negative value, it is interpreted in
 *    negated microseconds. Microsecond delays are allowable only in
 *    the revised and extended Time Managers.
 *    
 *    The task record specified by the tmTaskPtr parameter must already
 *    be installed in the queue (by a previous call to the functions
 *    InstallTimeTask or InstallXTimeTask) before your application
 *    calls the PrimeTimeTask function. The PrimeTimeTask function
 *    returns immediately, and the specified task is executed after the
 *    specified delay has elapsed. If you call the PrimeTimeTask
 *    function with a time delay of 0, the task runs as soon as
 *    interrupts are enabled.
 *    
 *    In the revised and extended Time Managers, the PrimeTimeTask
 *    function sets the high-order bit of the qType field to 1. In
 *    addition, any value of the count parameter that exceeds the
 *    maximum millisecond delay is reduced to the maximum. If you stop
 *    an unexpired task (by calling the function RemoveTimeTask) and
 *    then reinstall it (by calling the InstallXTimeTask function), you
 *    can continue the previous delay by calling the PrimeTimeTask
 *    function with the count parameter set to 0.
 *    
 *    
 *    
 *    
 *    
 *    
 *    *************************************************************
 *     See the discussion at the top of this file for information about
 *    why this call is deprecated on Mac OS X, and what other system
 *    facilities you can use in your code instead of the Time Manager.
 *    *****************************************************************
 *  
 *  Parameters:
 *    
 *    tmTaskPtr:
 *      A pointer to a task structure already installed in the queue.
 *    
 *    count:
 *      The desired delay before execution of the task.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
PrimeTimeTask(
  QElemPtr   tmTaskPtr,
  long       count)                                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);


/*
 *  RemoveTimeTask()   *** DEPRECATED ***
 *  
 *  Summary:
 *    Removes a task from the Time Manager queue.
 *  
 *  Discussion:
 *    The RemoveTimeTask function removes the Time Manager task
 *    structure specified by the tmTaskPtr parameter from the Time
 *    Manager queue. In both the revised and extended Time Managers, if
 *    the specified task record is active (that is, if it has been
 *    activated but the specified time has not yet elapsed), the
 *    tmCount field of the task structure returns the amount of time
 *    remaining. To provide the greatest accuracy, the unused time is
 *    reported as negated microseconds if that value is small enough to
 *    fit into the tmCount field (even if the delay was originally
 *    specified in milliseconds); otherwise, the unused time is
 *    reported in positive milliseconds. If the time has already
 *    expired, the tmCount field contains 0.
 *    
 *    In the revised and extended Time Managers, the RemoveTimeTask
 *    function sets the high-order bit of the qType field to 0.
 *     
 *    
 *    
 *    
 *    
 *    
 *    *************************************************************
 *     See the discussion at the top of this file for information about
 *    why this call is deprecated on Mac OS X, and what other system
 *    facilities you can use in your code instead of the Time Manager.
 *    *****************************************************************
 *  
 *  Parameters:
 *    
 *    tmTaskPtr:
 *      A pointer to a task structure to be removed from the queue.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.0.2 and later
 *    Non-Carbon CFM:   in InterfaceLib 9.1 and later
 */
extern OSErr 
RemoveTimeTask(QElemPtr tmTaskPtr)                            __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_4, __IPHONE_NA, __IPHONE_NA);



/*
 *  NewTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern TimerUPP
NewTimerUPP(TimerProcPtr userRoutine)                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeTimerUPP(TimerUPP userUPP)                             __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeTimerUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeTimerUPP(
  TMTaskPtr  tmTaskPtr,
  TimerUPP   userUPP)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline TimerUPP                                             NewTimerUPP(TimerProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeTimerUPP(TimerUPP) { }
    inline void                                                 InvokeTimerUPP(TMTaskPtr tmTaskPtr, TimerUPP userUPP) { (*userUPP)(tmTaskPtr); }
  #else
    #define NewTimerUPP(userRoutine)                            ((TimerUPP)userRoutine)
    #define DisposeTimerUPP(userUPP)
    #define InvokeTimerUPP(tmTaskPtr, userUPP)                  (*userUPP)(tmTaskPtr)
  #endif
#endif




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __TIMER__ */

