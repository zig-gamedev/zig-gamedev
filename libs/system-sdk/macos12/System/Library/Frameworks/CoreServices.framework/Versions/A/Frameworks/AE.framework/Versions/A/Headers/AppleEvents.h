/*
     File:       AE/AppleEvents.h
 
     Contains:   AppleEvent Package Interfaces.
 
    
 
     Copyright:  � 1989-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLEEVENTS__
#define __APPLEEVENTS__

#ifndef __COREFOUNDATION__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifndef __CARBONCORE__
#include <CarbonCore/CarbonCore.h>
#endif

/*
    Note:   The functions and types for the building and parsing AppleEvent  
            messages has moved to AEDataModel.h
*/
#ifndef __AEDATAMODEL__
#include <AE/AEDataModel.h>
#endif



#include <os/availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

CF_ENUM(AEKeyword) {
                                        /* Keywords for Apple event parameters */
  keyDirectObject               = '----',
  keyErrorNumber                = 'errn',
  keyErrorString                = 'errs',
  keyProcessSerialNumber        = 'psn ', /* Keywords for special handlers */
  keyPreDispatch                = 'phac', /* preHandler accessor call */
  keySelectProc                 = 'selh', /* more selector call */
                                        /* Keyword for recording */
  keyAERecorderCount            = 'recr', /* available only in vers 1.0.1 and greater */
                                        /* Keyword for version information */
  keyAEVersion                  = 'vers' /* available only in vers 1.0.1 and greater */
};

/* Event Class */
CF_ENUM(DescType) {
  kCoreEventClass               = 'aevt'
};

/* Event ID's */
CF_ENUM(AEEventID) {
  kAEOpenApplication            = 'oapp',
  kAEOpenDocuments              = 'odoc',
  kAEPrintDocuments             = 'pdoc',
  kAEOpenContents               = 'ocon',
  kAEQuitApplication            = 'quit', /* may include a property kAEQuitReason indicating what lead to the quit being sent. */
  kAEAnswer                     = 'ansr',
  kAEApplicationDied            = 'obit',
  kAEShowPreferences            = 'pref' /* sent by Mac OS X when the user chooses the Preferences item */
};

/* Constants for recording */
CF_ENUM(AEEventID) {
  kAEStartRecording             = 'reca', /* available only in vers 1.0.1 and greater */
  kAEStopRecording              = 'recc', /* available only in vers 1.0.1 and greater */
  kAENotifyStartRecording       = 'rec1', /* available only in vers 1.0.1 and greater */
  kAENotifyStopRecording        = 'rec0', /* available only in vers 1.0.1 and greater */
  kAENotifyRecording            = 'recr' /* available only in vers 1.0.1 and greater */
};


/**
 * AEEventSource is defined as an SInt8 for compatability with pascal.
 * Important note: keyEventSourceAttr is returned by AttributePtr as a typeShortInteger.
 * Be sure to pass at least two bytes of storage to AEGetAttributePtr - the result can be
 * compared directly against the following enums.
 */
typedef SInt8 AEEventSource;
enum {
  kAEUnknownSource              = 0,
  kAEDirectCall                 = 1,
  kAESameProcess                = 2,
  kAELocalProcess               = 3,
  kAERemoteProcess              = 4
};

	
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= 1080
	enum {
		errAETargetAddressNotPermitted	 = 	-1742,	/* Mac OS X 10.8 and later, the target of an AppleEvent is not accessible to this process, perhaps due to sandboxing */
		errAEEventNotPermitted = -1743,			/* Mac OS X 10.8 and later, the target of the AppleEvent does not allow this sender to execute this event */
	};
#endif
	
	
/**************************************************************************
  These calls are used to set up and modify the event dispatch table.D
**************************************************************************/
/*
 *  AEInstallEventHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEInstallEventHandler(
  AEEventClass        theAEEventClass,
  AEEventID           theAEEventID,
  AEEventHandlerUPP   handler,
  SRefCon             handlerRefcon,
  Boolean             isSysHandler)                           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AERemoveEventHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AERemoveEventHandler(
  AEEventClass        theAEEventClass,
  AEEventID           theAEEventID,
  AEEventHandlerUPP   handler,
  Boolean             isSysHandler)                           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetEventHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetEventHandler(
  AEEventClass         theAEEventClass,
  AEEventID            theAEEventID,
  AEEventHandlerUPP *  handler,
  SRefCon *            handlerRefcon,
  Boolean              isSysHandler)                          API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );




/**************************************************************************
  These calls are used to set up and modify special hooks into the
  AppleEvent manager.
**************************************************************************/
/*
 *  AEInstallSpecialHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEInstallSpecialHandler(
  AEKeyword           functionClass,
  AEEventHandlerUPP   handler,
  Boolean             isSysHandler)                           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AERemoveSpecialHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AERemoveSpecialHandler(
  AEKeyword           functionClass,
  AEEventHandlerUPP   handler,
  Boolean             isSysHandler)                           API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEGetSpecialHandler()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEGetSpecialHandler(
  AEKeyword            functionClass,
  AEEventHandlerUPP *  handler,
  Boolean              isSysHandler)                          API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/**************************************************************************
  This call was added in version 1.0.1. If called with the keyword
  keyAERecorderCount ('recr'), the number of recorders that are
  currently active is returned in 'result'
  (available only in vers 1.0.1 and greater).
**************************************************************************/
/*
 *  AEManagerInfo()
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.2
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AEManagerInfo(
  AEKeyword   keyWord,
  long *      result)                                         API_AVAILABLE( macos(10.0) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
  AERemoteProcessResolver:
  
  These calls subsume the functionality of using the PPCToolbox on Mac
  OS 9 to locate processes on remote computers.  (PPCToolbox is not
  part of Carbon.)  These calls are supported on Mac OS X 10.3 or
  later.
  
  The model is to create a resolver for a particular URL and schedule
  it on a CFRunLoop to retrieve the results asynchronously.  If
  synchronous behavior is desired, just call
  AERemoteProcessResolverGetProcesses to get the array; the call will
  block until the request is completed.
  
  A resolver can only be used once; once it has fetched the data or
  gotten an error it can no longer be scheduled.
  
  The data obtained from the resolver is a CFArrayRef of
  CFDictionaryRef objects.  Each dictionary contains the URL of the
  remote application and its human readable name.
*/

/*
 *  kAERemoteProcessURLKey
 *  
 *  Discussion:
 *    the full URL to this application, a CFURLRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kAERemoteProcessURLKey                      API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
 *  kAERemoteProcessNameKey
 *  
 *  Discussion:
 *    the visible name to this application, in the localization
 *    supplied by the server, a CFStringRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kAERemoteProcessNameKey                     API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
 *  kAERemoteProcessUserIDKey
 *  
 *  Discussion:
 *    the userid of this application, if available.  If present, a
 *    CFNumberRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kAERemoteProcessUserIDKey                   API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );
/*
 *  kAERemoteProcessProcessIDKey
 *  
 *  Discussion:
 *    the process id of this application, if available.  If present, a
 *    CFNumberRef.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const CFStringRef kAERemoteProcessProcessIDKey                API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );

/*
 *  AERemoteProcessResolverContext
 *  
 *  Discussion:
 *    An optional context parameter for asynchronous resolution.  The
 *    context is copied and the info pointer retained.  When the
 *    callback is made, the info pointer is passed to the callback.
 */
struct AERemoteProcessResolverContext {

  /*
   * set to zero (0)
   */
  CFIndex             version;

  /*
   * info pointer to be passed to the callback
   */
  void *              info;

  /*
   * callback made on the info pointer. This field may be NULL.
   */
  CFAllocatorRetainCallBack  retain;

  /*
   * callback made on the info pointer. This field may be NULL.
   */
  CFAllocatorReleaseCallBack  release;

  /*
   * callback made on the info pointer. This field may be NULL.
   */
  CFAllocatorCopyDescriptionCallBack  copyDescription;
};
typedef struct AERemoteProcessResolverContext AERemoteProcessResolverContext;

/*
 *  AERemoteProcessResolverRef
 *  
 *  Discussion:
 *    An opaque reference to an object that encapsulates the mechnanism
 *    by which a list of processes running on a remote machine are
 *    obtained.  Created by AECreateRemoteProcessResolver, and must be
 *    disposed of by AEDisposeRemoteProcessResolver. A
 *    AERemoteProcessResolverRef is not a CFType.
 */
typedef struct AERemoteProcessResolver*  AERemoteProcessResolverRef;
/*
 *  AECreateRemoteProcessResolver()
 *  
 *  Discussion:
 *    Create a Remote Process List Resolver object.  The allocator is
 *    used for any CoreFoundation types created or returned by this
 *    API.  The resulting object can be scheduled on a run loop, or
 *    queried synchronously.  Once the object has retreived results
 *    from the server, or got an error doing so, it will not re-fetch
 *    the data.  To retrieve a new list of processes, create a new
 *    instance of this object.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    allocator:
 *      a CFAllocatorRef to use when creating CFTypes
 *    
 *    url:
 *      a CFURL identifying the remote host and port.
 *  
 *  Result:
 *    a AECreateRemoteProcessResolverRef, which must be disposed of
 *    with AEDisposeRemoteProcessResolver.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern AERemoteProcessResolverRef 
AECreateRemoteProcessResolver(
  CFAllocatorRef   allocator,
  CFURLRef         url)                                       API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AEDisposeRemoteProcessResolver()
 *  
 *  Discussion:
 *    Disposes of a AERemoteProcessResolverRef.  If this resolver is
 *    currently scheduled on a run loop, it is unscheduled.  In this
 *    case, the asynchronous callback will not be executed.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    ref:
 *      The AERemoteProcessResolverRef to dispose
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
AEDisposeRemoteProcessResolver(AERemoteProcessResolverRef ref) API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );


/*
 *  AERemoteProcessResolverGetProcesses()
 *  
 *  Discussion:
 *    Returns a CFArrayRef containing CFDictionary objects containing
 *    information about processses running on a remote machine.  If the
 *    result array is NULL, the query failed and the error out
 *    parameter will contain information about the failure.  If the
 *    resolver had not been previously scheduled for execution, this
 *    call will block until the resulting array is available or an
 *    error occurs.  If the resolver had been scheduled but had not yet
 *    completed fetching the array, this call will block until the
 *    resolver does complete.  The array returned is owned by the
 *    resolver, so callers must retain it before disposing of the
 *    resolver object itself.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    ref:
 *      The AERemoteProcessResolverRef to query
 *    
 *    outError:
 *      If the result is NULL, outError will contain a CFStreamError
 *      with information about the type of failure
 *  
 *  Result:
 *    a CFArray of CFDictionary objects containing information about
 *    the remote applications.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CFArrayRef 
AERemoteProcessResolverGetProcesses(
  AERemoteProcessResolverRef   ref,
  CFStreamError *              outError)                      API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );



/*
 *  AERemoteProcessResolverCallback
 *  
 *  Discussion:
 *    A callback made when the asynchronous execution of a resolver
 *    completes, either due to success or failure. The data itself
 *    should be obtained with AERemoteProcessResolverGetProcesses.
 */
typedef CALLBACK_API( void , AERemoteProcessResolverCallback )(AERemoteProcessResolverRef ref, void *info);
/*
 *  AERemoteProcessResolverScheduleWithRunLoop()
 *
 *  Discussion:
 *    Schedules a resolver for execution on a given runloop in a given
 *    mode.   The resolver will move through various internal states as
 *    long as the specified run loop is run.  When the resolver
 *    completes, either with success or an error condition, the
 *    callback is executed.  There is no explicit unschedule of the
 *    resolver; you must dispose of it to remove it from the run loop.
 *
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *
 *  Parameters:
 *
 *    ref:
 *      The AERemoteProcessResolverRef to scheduile
 *
 *    runLoop:
 *      a CFRunLoop
 *
 *    runLoopMode:
 *      a CFString specifying the run loop mode
 *
 *    callback:
 *      a callback to be executed when the reolver completes
 *
 *    ctx:
 *      a AERemoteProcessResolverContext.  If this parameter is not
 *      NULL, the info field of this structure will be passed to the
 *      callback (otherwise, the callback info parameter will
 *      explicitly be NULL.)
 *
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern void 
AERemoteProcessResolverScheduleWithRunLoop(
  AERemoteProcessResolverRef              ref,
  CFRunLoopRef                            runLoop,
  CFStringRef                             runLoopMode,
  AERemoteProcessResolverCallback         callback,
  const AERemoteProcessResolverContext *  ctx)               /* can be NULL */ API_AVAILABLE( macos(10.3) ) API_UNAVAILABLE( ios, tvos, watchos );

/**
 Determines whether the current application is able to send an AppleEvent with the given eventClass and eventID to the application described as targetAddressDesc.

 Mac OS 10.14 and later impose additional requirements on applications when they send AppleEvents to other applications in order
 to insure that users are aware of and consent to allowing such control or information exchange.  Generally this involves
 the user being prompted in a secure fashion the first time an application attempts to send an AppleEvent to another application.
 If the user consents then this application can send events to the target.  If the user does not consent then any future
 attempts to send AppleEvents will result in a failure with errAEEventNotPermitted being returned.

 Certain AppleEvents are allowed to be sent without prompting the user.  Pass typeWildCard for the eventClass and eventID
 to determine if every event is allowed to be sent from this application to the target.

 Applications can determine, without sending an AppleEvent to a target application, whether they are allowed to send AppleEvents
 to the target with this function.  If askUserIfNeeded is true, and this application does not yet have permission to send
 AppleEvents to the target, then the user will be asked if permission can be granted; if askUserIfNeeded is false and permission
 has not been granted, then errAEEventWouldRequireUserConsent will be returned.

 The target AEAddressDesc must refer to an already running application.

 @subsection macOS Threading
 Thread safe since version 10.14.  Do not call this function on your main thread because it may take arbitrarily long
 to return if the user needs to be prompted for consent.

 @param target
    A pointer to an address descriptor. Before calling AEDeterminePermissionToAutomateTarget, you set the descriptor to identify
    the target application for the Apple event.  The target address descriptor must refer to a running application.  If
    the target application is on another machine, then Remote AppleEvents must be enabled on that machine for the user.

 @param theAEEventClass
    The event class of the Apple event to determine permission for.

 @param theAEEventID
    The event ID of the Apple event to determine permission for.

 @param askUserIfNeeded
    a Boolean; if true, and if this application does not yet have permission to send events to the target application, then
	prompt the user to obtain permission.  If false, do not prompt the user.

 @returns If the current application is permitted to send the given AppleEvent to the target, then noErr will be returned.  If the
 current application is not permitted to send the event, errAEEventNotPermitted will be returned.  If the target application
 is not running, then procNotFound will be returned.  If askUserIfNeeded is false, and this application is not yet permitted
 to send AppleEvents to the target, then errAEEventWouldRequireUserConsent will be returned.
 */
extern OSStatus AEDeterminePermissionToAutomateTarget( const AEAddressDesc* target, AEEventClass theAEEventClass, AEEventID theAEEventID, Boolean askUserIfNeeded ) API_AVAILABLE( macos(10.14) ) API_UNAVAILABLE( ios, tvos, watchos );

#if defined(__MAC_10_14) && __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_14

	enum {
		errAEEventWouldRequireUserConsent = -1744, ///< Determining whether this can be sent would require prompting the user, and the AppleEvent was sent with kAEDoNotPromptForPermission
	};
	
	/*
	 *	AESendMode flag
	 *
	 *	In AESend(), when sending an AppleEvent, if this is masked into AESendMode and if the AppleEvent would require user consent, then AESend() will return errAEEventWouldRequireUserConsent.
	 *
	 */
	enum {
		kAEDoNotPromptForUserConsent = 0x00020000, /* If set, and the AppleEvent requires user consent, do not prompt and instead return errAEEventWouldRequireUserConsent */
	};
#endif


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __APPLEEVENTS__ */

