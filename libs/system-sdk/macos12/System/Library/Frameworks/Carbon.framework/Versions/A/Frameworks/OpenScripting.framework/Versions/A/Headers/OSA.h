/*
     File:       OpenScripting/OSA.h
 
     Contains:   Open Scripting Architecture Client Interfaces.
 
     Version:    OpenScripting
 
     Copyright:  © 1992-2008 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OSA__
#define __OSA__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __AEINTERACTION__
#include <HIToolbox/AEInteraction.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
    Types and Constants
**************************************************************************/

/*    The componenent manager type code for components that
        support the OSA interface defined here. */
/* 0x6f736120 */
enum {
  kOSAComponentType             = 'osa '
};

/* 0x73637074 */
enum {
  kOSAGenericScriptingComponentSubtype = 'scpt'
};

/*  Type of script document files.  */
/* 0x6f736173 */
enum {
  kOSAFileType                  = 'osas'
};

/*
        Suite and event code of the RecordedText event. 
        (See OSAStartRecording, below.)
    */
/* 0x61736372 */
enum {
  kOSASuite                     = 'ascr'
};

/* 0x72656364 */
enum {
  kOSARecordedText              = 'recd'
};

/* Selector returns boolean */
/* 0x6d6f6469 */
enum {
  kOSAScriptIsModified          = 'modi'
};

/* Selector returns boolean */
/* 0x63736372 */
enum {
  kOSAScriptIsTypeCompiledScript = 'cscr'
};

/* Selector returns boolean */
/* 0x76616c75 */
enum {
  kOSAScriptIsTypeScriptValue   = 'valu'
};

/* Selector returns boolean */
/* 0x636e7478 */
enum {
  kOSAScriptIsTypeScriptContext = 'cntx'
};

/* Selector returns a DescType which may be passed to OSACoerceToDesc */
/* 0x62657374 */
enum {
  kOSAScriptBestType            = 'best'
};

/*
        This selector is used to determine whether a script has source 
        associated with it that when given to OSAGetSource, the call will not
        fail.  The selector returns a boolean.
    */
/* 0x67737263 */
enum {
  kOSACanGetSource              = 'gsrc'
};


enum {
  typeOSADialectInfo            = 'difo', /*  0x6469666f   */
  keyOSADialectName             = 'dnam', /*  0x646e616d   */
  keyOSADialectCode             = 'dcod', /*  0x64636f64   */
  keyOSADialectLangCode         = 'dlcd', /*  0x646c6364   */
  keyOSADialectScriptCode       = 'dscd' /*  0x64736364   */
};

typedef ComponentResult                 OSAError;
/* Under the Open Scripting Architecture all error results are longs */
typedef UInt32                          OSAID;
/*
        OSAIDs allow transparent manipulation of scripts associated with
        various scripting systems.
    */
enum {
  kOSANullScript                = 0
};

/* No -script constant. */
enum {
  kOSANullMode                  = 0,    /* sounds better */
  kOSAModeNull                  = 0     /* tastes consistent */
};

/*
        Some routines take flags that control their execution.  This constant
        declares default mode settings are used.
    */
typedef CALLBACK_API( OSErr , OSACreateAppleEventProcPtr )(AEEventClass theAEEventClass, AEEventID theAEEventID, const AEAddressDesc *target, short returnID, SInt32 transactionID, AppleEvent *result, SRefCon refCon);
typedef CALLBACK_API( OSErr , OSASendProcPtr )(const AppleEvent *theAppleEvent, AppleEvent *reply, AESendMode sendMode, AESendPriority sendPriority, SInt32 timeOutInTicks, AEIdleUPP idleProc, AEFilterUPP filterProc, SRefCon refCon);
typedef STACK_UPP_TYPE(OSACreateAppleEventProcPtr)              OSACreateAppleEventUPP;
typedef STACK_UPP_TYPE(OSASendProcPtr)                          OSASendUPP;

#if !__LP64__
/*
 *  NewOSACreateAppleEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSACreateAppleEventUPP
NewOSACreateAppleEventUPP(OSACreateAppleEventProcPtr userRoutine) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  NewOSASendUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSASendUPP
NewOSASendUPP(OSASendProcPtr userRoutine)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  DisposeOSACreateAppleEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOSACreateAppleEventUPP(OSACreateAppleEventUPP userUPP) __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  DisposeOSASendUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOSASendUPP(OSASendUPP userUPP)                         __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  InvokeOSACreateAppleEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeOSACreateAppleEventUPP(
  AEEventClass            theAEEventClass,
  AEEventID               theAEEventID,
  const AEAddressDesc *   target,
  short                   returnID,
  SInt32                  transactionID,
  AppleEvent *            result,
  SRefCon                 refCon,
  OSACreateAppleEventUPP  userUPP)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  InvokeOSASendUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeOSASendUPP(
  const AppleEvent *  theAppleEvent,
  AppleEvent *        reply,
  AESendMode          sendMode,
  AESendPriority      sendPriority,
  SInt32              timeOutInTicks,
  AEIdleUPP           idleProc,
  AEFilterUPP         filterProc,
  SRefCon             refCon,
  OSASendUPP          userUPP)                                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);
#endif

#if __MACH__
  #ifdef __cplusplus
    inline OSACreateAppleEventUPP                               NewOSACreateAppleEventUPP(OSACreateAppleEventProcPtr userRoutine) { return userRoutine; }
    inline OSASendUPP                                           NewOSASendUPP(OSASendProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeOSACreateAppleEventUPP(OSACreateAppleEventUPP) { }
    inline void                                                 DisposeOSASendUPP(OSASendUPP) { }
    inline OSErr                                                InvokeOSACreateAppleEventUPP(AEEventClass theAEEventClass, AEEventID theAEEventID, const AEAddressDesc * target, short returnID, SInt32 transactionID, AppleEvent * result, SRefCon refCon, OSACreateAppleEventUPP userUPP) { return (*userUPP)(theAEEventClass, theAEEventID, target, returnID, transactionID, result, refCon); }
    inline OSErr                                                InvokeOSASendUPP(const AppleEvent * theAppleEvent, AppleEvent * reply, AESendMode sendMode, AESendPriority sendPriority, SInt32 timeOutInTicks, AEIdleUPP idleProc, AEFilterUPP filterProc, SRefCon refCon, OSASendUPP userUPP) { return (*userUPP)(theAppleEvent, reply, sendMode, sendPriority, timeOutInTicks, idleProc, filterProc, refCon); }
  #else
    #define NewOSACreateAppleEventUPP(userRoutine)              ((OSACreateAppleEventUPP)userRoutine)
    #define NewOSASendUPP(userRoutine)                          ((OSASendUPP)userRoutine)
    #define DisposeOSACreateAppleEventUPP(userUPP)
    #define DisposeOSASendUPP(userUPP)
    #define InvokeOSACreateAppleEventUPP(theAEEventClass, theAEEventID, target, returnID, transactionID, result, refCon, userUPP) (*userUPP)(theAEEventClass, theAEEventID, target, returnID, transactionID, result, refCon)
    #define InvokeOSASendUPP(theAppleEvent, reply, sendMode, sendPriority, timeOutInTicks, idleProc, filterProc, refCon, userUPP) (*userUPP)(theAppleEvent, reply, sendMode, sendPriority, timeOutInTicks, idleProc, filterProc, refCon)
  #endif
#endif

/**************************************************************************
    OSA Interface Descriptions
**************************************************************************
    The OSA Interface is broken down into a required interface, and several
    optional interfaces to support additional functionality.  A given scripting
    component may choose to support only some of the optional interfaces in
    addition to the basic interface.  The OSA Component Flags may be used to 
    query the Component Manager to find a scripting component with a particular
    capability, or determine if a particular scripting component supports a 
    particular capability.
**************************************************************************/
/* OSA Component Flags: */
enum {
  kOSASupportsCompiling         = 0x0002,
  kOSASupportsGetSource         = 0x0004,
  kOSASupportsAECoercion        = 0x0008,
  kOSASupportsAESending         = 0x0010,
  kOSASupportsRecording         = 0x0020,
  kOSASupportsConvenience       = 0x0040,
  kOSASupportsDialects          = 0x0080,
  kOSASupportsEventHandling     = 0x0100
};

/* Component Selectors: */
enum {
  kOSASelectLoad                = 0x0001,
  kOSASelectStore               = 0x0002,
  kOSASelectExecute             = 0x0003,
  kOSASelectDisplay             = 0x0004,
  kOSASelectScriptError         = 0x0005,
  kOSASelectDispose             = 0x0006,
  kOSASelectSetScriptInfo       = 0x0007,
  kOSASelectGetScriptInfo       = 0x0008,
  kOSASelectSetActiveProc       = 0x0009,
  kOSASelectGetActiveProc       = 0x000A,
  kOSASelectCopyDisplayString   = 0x000B
};

/* Compiling: */
enum {
  kOSASelectScriptingComponentName = 0x0102,
  kOSASelectCompile             = 0x0103,
  kOSASelectCopyID              = 0x0104,
  kOSASelectCopyScript          = 0x0105
};

/* GetSource: */
enum {
  kOSASelectGetSource           = 0x0201,
  kOSASelectCopySourceString    = 0x0202
};

/* AECoercion: */
enum {
  kOSASelectCoerceFromDesc      = 0x0301,
  kOSASelectCoerceToDesc        = 0x0302
};

/* AESending: */
enum {
  kOSASelectSetSendProc         = 0x0401,
  kOSASelectGetSendProc         = 0x0402,
  kOSASelectSetCreateProc       = 0x0403,
  kOSASelectGetCreateProc       = 0x0404,
  kOSASelectSetDefaultTarget    = 0x0405
};

/* Recording: */
enum {
  kOSASelectStartRecording      = 0x0501,
  kOSASelectStopRecording       = 0x0502
};

/* Convenience: */
enum {
  kOSASelectLoadExecute         = 0x0601,
  kOSASelectCompileExecute      = 0x0602,
  kOSASelectDoScript            = 0x0603
};

/* Dialects: */
enum {
  kOSASelectSetCurrentDialect   = 0x0701,
  kOSASelectGetCurrentDialect   = 0x0702,
  kOSASelectAvailableDialects   = 0x0703,
  kOSASelectGetDialectInfo      = 0x0704,
  kOSASelectAvailableDialectCodeList = 0x0705
};

/* Event Handling: */
enum {
  kOSASelectSetResumeDispatchProc = 0x0801,
  kOSASelectGetResumeDispatchProc = 0x0802,
  kOSASelectExecuteEvent        = 0x0803,
  kOSASelectDoEvent             = 0x0804,
  kOSASelectMakeContext         = 0x0805
};


/* scripting component specific selectors are added beginning with this value  */
enum {
  kOSASelectComponentSpecificStart = 0x1001
};


/*        Mode Flags:

    Warning: These should not conflict with the AESend mode flags in
    AppleEvents.h, because we may want to use them as OSA mode flags too.
*/

/*
        This mode flag may be passed to OSALoad, OSAStore or OSACompile to
        instruct the scripting component to not retain the "source" of an
        expression.  This will cause the OSAGetSource call to return the error
        errOSASourceNotAvailable if used.  However, some scripting components
        may not retain the source anyway.  This is mainly used when either space
        efficiency is desired, or a script is to be "locked" so that its
        implementation may not be viewed.
    */
enum {
  kOSAModePreventGetSource      = 0x00000001
};

/*
        These mode flags may be passed to OSACompile, OSAExecute, OSALoadExecute
        OSACompileExecute, OSADoScript, OSAExecuteEvent, or OSADoEvent to
        indicate whether or not the script may interact with the user, switch
        layer or reconnect if necessary.  Any AppleEvents will be sent with the
        corresponding AESend mode supplied.
    */
enum {
  kOSAModeNeverInteract         = kAENeverInteract,
  kOSAModeCanInteract           = kAECanInteract,
  kOSAModeAlwaysInteract        = kAEAlwaysInteract,
  kOSAModeDontReconnect         = kAEDontReconnect
};

/*
        This mode flag may be passed to OSACompile, OSAExecute, OSALoadExecute
        OSACompileExecute, OSADoScript, OSAExecuteEvent, or OSADoEvent to
        indicate whether or not AppleEvents should be sent with the
        kAECanSwitchLayer mode flag sent or not. NOTE: This flag is exactly the
        opposite sense of the AppleEvent flag kAECanSwitchLayer.  This is to
        provide a more convenient default, i.e. not supplying any mode
        (kOSAModeNull) means to send events with kAECanSwitchLayer.  Supplying
        the kOSAModeCantSwitchLayer mode flag will cause AESend to be called
        without kAECanSwitchLayer.
    */
enum {
  kOSAModeCantSwitchLayer       = 0x00000040
};

/*
        This mode flag may be passed to OSACompile, OSAExecute, OSALoadExecute
        OSACompileExecute, OSADoScript, OSAExecuteEvent, or OSADoEvent to
        indicate whether or not AppleEvents should be sent with the kAEDontRecord
        mode flag sent or not. NOTE: This flag is exactly the opposite sense of
        the AppleEvent flag kAEDontRecord.  This is to provide a more convenient
        default, i.e. not supplying any mode (kOSAModeNull) means to send events
        with kAEDontRecord.  Supplying the kOSAModeDoRecord mode flag will 
        cause AESend to be called without kAEDontRecord.
    */
enum {
  kOSAModeDoRecord              = 0x00001000
};

/*
        This is a mode flag for OSACompile that indicates that a context should
        be created as the result of compilation. All handler definitions are
        inserted into the new context, and variables are initialized by
        evaluating their initial values in a null context (i.e. they must be
        constant expressions).
    */
enum {
  kOSAModeCompileIntoContext    = 0x00000002
};

/*
        This is a mode flag for OSACompile that indicates that the previous
        script ID (input to OSACompile) should be augmented with any new
        definitions in the sourceData rather than replaced with a new script.
        This means that the previous script ID must designate a context.
        The presence of this flag causes the kOSAModeCompileIntoContext flag
        to be implicitly used, causing any new definitions to be initialized
        in a null context.
    */
enum {
  kOSAModeAugmentContext        = 0x00000004
};

/*
        This mode flag may be passed to OSADisplay or OSADoScript to indicate
        that output only need be human-readable, not re-compilable by OSACompile.
        If used, output may be arbitrarily "beautified", e.g. quotes may be left
        off of string values, long lists may have elipses, etc.
    */
enum {
  kOSAModeDisplayForHumans      = 0x00000008
};

/*
        This mode flag may be passed to OSAStore in the case where the scriptID
        is a context.  This causes the context to be saved, but not the context's
        parent context.  When the stored context is loaded back in, the parent
        will be kOSANullScript.
    */
enum {
  kOSAModeDontStoreParent       = 0x00010000
};

/*
        This mode flag may be passed to OSAExecuteEvent to cause the event to
        be dispatched to the direct object of the event. The direct object (or
        subject attribute if the direct object is a non-object specifier) will
        be resolved, and the resulting script object will be the recipient of
        the message. The context argument to OSAExecuteEvent will serve as the
        root of the lookup/resolution process.
    */
enum {
  kOSAModeDispatchToDirectObject = 0x00020000
};

/*
        This mode flag may be passed to OSAExecuteEvent to indicate that
        components do not have to get the data of object specifier arguments.
    */
enum {
  kOSAModeDontGetDataForArguments = 0x00040000
};

/*
        This mode flag may be passed to OSACoerceToDesc to indicate that
        the resulting descriptor should be fully qualified (i.e. should
        include the root application reference).
    */
enum {
  kOSAModeFullyQualifyDescriptors = 0x00080000
};

/**************************************************************************
    OSA Basic Scripting Interface
**************************************************************************
    Scripting components must at least support the Basic Scripting interface.
**************************************************************************/
/*        Loading and Storing Scripts:

    These routines allow scripts to be loaded and stored in their internal
    (possibly compiled, non-text) representation.
*/

/* Resource type for scripts */
enum {
  kOSAScriptResourceType        = kOSAGenericScriptingComponentSubtype
};

/*
        Default type given to OSAStore which creates "generic" loadable script
        data descriptors.
    */
enum {
  typeOSAGenericStorage         = kOSAScriptResourceType
};

/*
 *  OSALoad()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSALoad(
  ComponentInstance   scriptingComponent,
  const AEDesc *      scriptData,
  SInt32              modeFlags,
  OSAID *             resultingScriptID)                      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectLoad, 12);
    
        Errors:
            badComponentInstance        invalid scripting component instance
            errOSASystemError
            errOSABadStorageType:       scriptData not for this scripting component
            errOSACorruptData:          data seems to be corrupt
            errOSADataFormatObsolete    script data format is no longer supported
            errOSADataFormatTooNew      script data format is from a newer version
        
        ModeFlags:
            kOSAModePreventGetSource
    */
/*
 *  OSAStore()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAStore(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  DescType            desiredType,
  SInt32              modeFlags,
  AEDesc *            resultingScriptData)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectStore, 16);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSABadStorageType:   desiredType not for this scripting component
        
        ModeFlags:
            kOSAModePreventGetSource
            kOSAModeDontStoreParent
    */
/* Executing Scripts: */
/*
 *  OSAExecute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAExecute(
  ComponentInstance   scriptingComponent,
  OSAID               compiledScriptID,
  OSAID               contextID,
  SInt32              modeFlags,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectExecute, 16);
        This call runs a script.  The contextID represents the environment
        with which global variables in the script are resolved.  The constant
        kOSANullScript may be used for the contextID if the application wishes
        to not deal with context directly (a default one is associated with each
        scripting component instance).  The resultingScriptValueID is the 
        result of evaluation, and contains a value which may be displayed using
        the OSAGetSource call.  The modeFlags convey scripting component
        specific information.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSAScriptError:      the executing script got an error
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/* Displaying results: */
/*
 *  OSADisplay()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSADisplay(
  ComponentInstance   scriptingComponent,
  OSAID               scriptValueID,
  DescType            desiredType,
  SInt32              modeFlags,
  AEDesc *            resultingText)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectDisplay, 16);
        This call is used to convert results (script value IDs) into displayable
        text. The desiredType should be at least typeChar, and modeFlags are
        scripting system specific flags to control the formatting of the
        resulting text. This call differs from OSAGetSource in that (1) it
        always produces at least typeChar, (2) is only works on script values,
        (3) it may display it's output in non-compilable form (e.g. without
        string quotes, elipses inserted in long and/or circular lists, etc.) and
        (4) it is required by the basic scripting interface.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errAECoercionFail:      desiredType not supported by scripting component
    
        ModeFlags:
            kOSAModeDisplayForHumans
    */
/*
    @function   OSACopyDisplayString

    @discussion Similar to OSADisplay, but returns the text as a CFAttributedStringRef.
*/
/*
 *  OSACopyDisplayString()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSACopyDisplayString(
  ComponentInstance        scriptingComponent,
  OSAID                    scriptID,
  SInt32                   modeFlags,
  CFAttributedStringRef *  result)                            __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);


/* Getting Error Information: */
/*
 *  OSAScriptError()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAScriptError(
  ComponentInstance   scriptingComponent,
  OSType              selector,
  DescType            desiredType,
  AEDesc *            resultingErrorDescription)              __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectScriptError, 12);
        Whenever script execution returns errOSAExecutionError, this routine
        may be used to get information about that error.  The selector describes
        the type of information desired about the error (various selectors are
        listed below).  The desiredType indicates the data type of the result
        desired for that selector.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSABadSelector:      selector not supported by scripting component
            errAECoercionFail:      desiredType not supported by scripting component
    */
/* OSAScriptError selectors: */
/*
        This selector is used to determine the error number of a script error.
        These error numbers may be either system error numbers, or error numbers
        that are scripting component specific.
        Required desiredTypes:  
            typeSInt32
    */
enum {
  kOSAErrorNumber               = keyErrorNumber
};

/*
        This selector is used to determine the full error message associated
        with the error number.  It should include the name of the application
        which caused the error, as well as the specific error that occurred.
        This selector is sufficient for simple error reporting (but see
        kOSAErrorBriefMessage, below).
        Required desiredTypes:
            typeChar                    error message string
    */
enum {
  kOSAErrorMessage              = keyErrorString
};

/*
        This selector is used to determine a brief error message associated with
        the error number.  This message and should not mention the name of the
        application which caused the error, any partial results or offending
        object (see kOSAErrorApp, kOSAErrorPartialResult and
        kOSAErrorOffendingObject, below).
        Required desiredTypes:
            typeChar                    brief error message string
    */
/*  0x65727262  */
enum {
  kOSAErrorBriefMessage         = 'errb'
};

/*
        This selector is used to determine which application actually got the
        error (if it was the result of an AESend), or the current application
        if ....
        Required desiredTypes:
            typeProcessSerialNumber     PSN of the errant application
            typeChar                    name of the errant application
    */
/*  0x65726170  */
enum {
  kOSAErrorApp                  = 'erap'
};

/*
        This selector is used to determine any partial result returned by an 
        operation. If an AESend call failed, but a partial result was returned,
        then the partial result may be returned as an AEDesc.
        Required desiredTypes:
            typeBest                    AEDesc of any partial result
    */
/*  0x70746c72   */
enum {
  kOSAErrorPartialResult        = 'ptlr'
};

/*
        This selector is used to determine any object which caused the error
        that may have been indicated by an application.  The result is an 
        AEDesc.
        Required desiredTypes:
            typeBest                    AEDesc of any offending object
    */
/*  0x65726f62   */
enum {
  kOSAErrorOffendingObject      = 'erob'
};

/*
        This selector is used to determine the type expected by a coercion 
        operation if a type error occurred.
    */
/*  0x65727274   */
enum {
  kOSAErrorExpectedType         = 'errt'
};

/*
        This selector is used to determine the source text range (start and 
        end positions) of where the error occurred.
        Required desiredTypes:
            typeOSAErrorRange
    */
/*  0x65726e67  */
enum {
  kOSAErrorRange                = 'erng'
};

/*
        An AERecord type containing keyOSASourceStart and keyOSASourceEnd fields
        of type short.
    */
/*  0x65726e67   */
enum {
  typeOSAErrorRange             = 'erng'
};

/* Field of a typeOSAErrorRange record of typeSInt16 */
/*  0x73726373    */
enum {
  keyOSASourceStart             = 'srcs'
};

/* Field of a typeOSAErrorRange record of typeSInt16 */
/*  0x73726365   */
enum {
  keyOSASourceEnd               = 'srce'
};

/* Disposing Script IDs: */
/*
 *  OSADispose()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSADispose(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID)                               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectDispose, 4);
        Disposes a script or context.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
    */
/* Getting and Setting Script Information: */
/*
 *  OSASetScriptInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetScriptInfo(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  OSType              selector,
  long                value)                                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetScriptInfo, 12);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSABadSelector:      selector not supported by scripting component
                                    or selector not for this scriptID
    */
/*
 *  OSAGetScriptInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetScriptInfo(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  OSType              selector,
  long *              result)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetScriptInfo, 12);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSABadSelector:      selector not supported by scripting component
                                    or selector not for this scriptID
    */
/* Manipulating the ActiveProc:

    Scripting systems will supply default values for these procedures if they
    are not set by the client:
*/
typedef CALLBACK_API( OSErr , OSAActiveProcPtr )(SRefCon refCon);
typedef STACK_UPP_TYPE(OSAActiveProcPtr)                        OSAActiveUPP;
#if !__LP64__
/*
 *  NewOSAActiveUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSAActiveUPP
NewOSAActiveUPP(OSAActiveProcPtr userRoutine)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  DisposeOSAActiveUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeOSAActiveUPP(OSAActiveUPP userUPP)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/*
 *  InvokeOSAActiveUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeOSAActiveUPP(
  SRefCon       refCon,
  OSAActiveUPP  userUPP)                                      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);
#endif

#if __MACH__
  #ifdef __cplusplus
    inline OSAActiveUPP                                         NewOSAActiveUPP(OSAActiveProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeOSAActiveUPP(OSAActiveUPP) { }
    inline OSErr                                                InvokeOSAActiveUPP(SRefCon refCon, OSAActiveUPP userUPP) { return (*userUPP)(refCon); }
  #else
    #define NewOSAActiveUPP(userRoutine)                        ((OSAActiveUPP)userRoutine)
    #define DisposeOSAActiveUPP(userUPP)
    #define InvokeOSAActiveUPP(refCon, userUPP)                 (*userUPP)(refCon)
  #endif
#endif

/*
 *  OSASetActiveProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetActiveProc(
  ComponentInstance   scriptingComponent,
  OSAActiveUPP        activeProc,
  SRefCon             refCon)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetActiveProc, 8);
        If activeProc is nil, the default activeProc is used.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAGetActiveProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetActiveProc(
  ComponentInstance   scriptingComponent,
  OSAActiveUPP *      activeProc,
  SRefCon *           refCon)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetActiveProc, 8);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/**************************************************************************
    OSA Optional Compiling Interface
**************************************************************************
    Scripting components that support the Compiling interface have the 
    kOSASupportsCompiling bit set in it's ComponentDescription.
**************************************************************************/
/*
 *  OSAScriptingComponentName()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAScriptingComponentName(
  ComponentInstance   scriptingComponent,
  AEDesc *            resultingScriptingComponentName)        __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectScriptingComponentName, 4);
        Given a scripting component, this routine returns the name of that
        scripting component in a type that is coercable to text (typeChar).
        The generic scripting component returns the name of the default
        scripting component.  This name should be sufficient to convey to the
        user the kind of script (syntax) he is expected to write.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSACompile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSACompile(
  ComponentInstance   scriptingComponent,
  const AEDesc *      sourceData,
  SInt32              modeFlags,
  OSAID *             previousAndResultingScriptID)           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCompile, 12);
        Coerces input desc (possibly text) into a script's internal format.
        Once compiled, the script is ready to run.  The modeFlags convey
        scripting component specific information.  The previous script ID
        (result parameter) is made to refer to the newly compiled script,
        unless it was originally kOSANullScript.  In this case a new script
        ID is created and used.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errAECoercionFail:      sourceData is not compilable
            errOSAScriptError:      sourceData was a bad script (syntax error)
            errOSAInvalidID:        previousAndResultingCompiledScriptID was not
                                    valid on input
    
        ModeFlags:
            kOSAModePreventGetSource
            kOSAModeCompileIntoContext
            kOSAModeAugmentContext
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSACopyID()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSACopyID(
  ComponentInstance   scriptingComponent,
  OSAID               fromID,
  OSAID *             toID)                                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCopyID, 8);
        If toID is a reference to kOSANullScript then it is updated to have a
        new scriptID value.  This call can be used to perform undo or revert
        operations on scripts. 
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
    */
/*
 *  OSACopyScript()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in AppleScriptLib 1.5 and later
 */
extern OSAError 
OSACopyScript(
  ComponentInstance   scriptingComponent,
  OSAID               fromID,
  OSAID *             toID)                                   __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCopyScript, 8);
        Creates a duplicate copy of the script with the given OSAID and returns
        a new OSAID for it.  Can be used by script editors or debuggers. 
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
    */
/**************************************************************************
    OSA Optional GetSource Interface
**************************************************************************
    Scripting components that support the GetSource interface have the 
    kOSASupportsGetSource bit set in their ComponentDescription.
**************************************************************************/
/*
 *  OSAGetSource()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetSource(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  DescType            desiredType,
  AEDesc *            resultingSourceData)                    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetSource, 12);
        This routine causes a compiled script to be output in a form (possibly
        text) such that it is suitable to be passed back to OSACompile.

        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSASourceNotAvailable    can't get source for this scriptID
    */
/*
    @function   OSACopySourceString

    @discussion Similar to OSAGetSource, but returns the text as a CFAttributedStringRef.
*/
/*
 *  OSACopySourceString()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSACopySourceString(
  ComponentInstance        scriptingComponent,
  OSAID                    scriptID,
  SInt32                   modeFlags,
  CFAttributedStringRef *  result)                            __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);


/**************************************************************************
    OSA Optional AECoercion Interface
**************************************************************************
    Scripting components that support the AECoercion interface have the 
    kOSASupportsAECoercion bit set in their ComponentDescription.
**************************************************************************/
/*
 *  OSACoerceFromDesc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSACoerceFromDesc(
  ComponentInstance   scriptingComponent,
  const AEDesc *      scriptData,
  SInt32              modeFlags,
  OSAID *             resultingScriptID)                      __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCoerceFromDesc, 12);
        This routine causes script data to be coerced into a script value.
        If the scriptData is an AppleEvent, then the resultingScriptID is a
        compiled script ID (mode flags for OSACompile may be used in this case).
        Other scriptData descriptors create script value IDs.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    
        ModeFlags:
            kOSAModePreventGetSource
            kOSAModeCompileIntoContext
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSACoerceToDesc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSACoerceToDesc(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  DescType            desiredType,
  SInt32              modeFlags,
  AEDesc *            result)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCoerceToDesc, 16);
        This routine causes a script value to be coerced into any desired form.
        If the scriptID denotes a compiled script, then it may be coerced to 
        typeAppleEvent.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
    */
/**************************************************************************
    OSA Optional AESending Interface
**************************************************************************
    Scripting components that support the AESending interface have the 
    kOSASupportsAESending bit set in their ComponentDescription.
**************************************************************************/
/*
    Scripting systems will supply default values for these procedures if they
    are not set by the client:
*/
/*
 *  OSASetSendProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetSendProc(
  ComponentInstance   scriptingComponent,
  OSASendUPP          sendProc,
  SRefCon             refCon)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetSendProc, 8);
        If sendProc is nil, the default sendProc is used.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAGetSendProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetSendProc(
  ComponentInstance   scriptingComponent,
  OSASendUPP *        sendProc,
  SRefCon *           refCon)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetSendProc, 8);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSASetCreateProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetCreateProc(
  ComponentInstance        scriptingComponent,
  OSACreateAppleEventUPP   createProc,
  SRefCon                  refCon)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetCreateProc, 8);
        If createProc is nil, the default createProc is used.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAGetCreateProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetCreateProc(
  ComponentInstance         scriptingComponent,
  OSACreateAppleEventUPP *  createProc,
  SRefCon *                 refCon)                           __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetCreateProc, 8);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSASetDefaultTarget()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetDefaultTarget(
  ComponentInstance      scriptingComponent,
  const AEAddressDesc *  target)                              __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetDefaultTarget, 4);
        This routine sets the default target application for AE sending.
        It also establishes the default target from which terminologies come.
        It is effectively like having an AppleScript "tell" statement around
        the entire program.  If this routine is not called, or if the target 
        is a null AEDesc, then the current application is the default target.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/**************************************************************************
    OSA Optional Recording Interface
**************************************************************************
    Scripting components that support the Recording interface have the 
    kOSASupportsRecording bit set in their ComponentDescription.
**************************************************************************/
/*
 *  OSAStartRecording()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAStartRecording(
  ComponentInstance   scriptingComponent,
  OSAID *             compiledScriptToModifyID)               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectStartRecording, 4);
        Starts recording.  If compiledScriptToModifyID is kOSANullScript, a
        new script ID is created and returned.  If the current application has
        a handler for the kOSARecordedText event, then kOSARecordedText events
        are sent to the application containing the text of each AppleEvent 
        recorded.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSARecordingIsAlreadyOn
    */
/*
 *  OSAStopRecording()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAStopRecording(
  ComponentInstance   scriptingComponent,
  OSAID               compiledScriptID)                       __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectStopRecording, 4);
        If compiledScriptID is not being recorded into or recording is not
        currently on, no error is returned.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
    */
/**************************************************************************
    OSA Optional Convenience Interface
**************************************************************************
    Scripting components that support the Convenience interface have the 
    kOSASupportsConvenience bit set in their ComponentDescription.
**************************************************************************/
/*
 *  OSALoadExecute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSALoadExecute(
  ComponentInstance   scriptingComponent,
  const AEDesc *      scriptData,
  OSAID               contextID,
  SInt32              modeFlags,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectLoadExecute, 16);
        This routine is effectively equivalent to calling OSALoad followed by
        OSAExecute.  After execution, the compiled source is disposed.  Only the
        resulting value ID is retained.
    
        Errors:
            badComponentInstance        invalid scripting component instance
            errOSASystemError
            errOSABadStorageType:       scriptData not for this scripting component
            errOSACorruptData:          data seems to be corrupt
            errOSADataFormatObsolete    script data format is no longer supported
            errOSADataFormatTooNew      script data format is from a newer version
            errOSAInvalidID
            errOSAScriptError:          the executing script got an error
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSACompileExecute()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSACompileExecute(
  ComponentInstance   scriptingComponent,
  const AEDesc *      sourceData,
  OSAID               contextID,
  SInt32              modeFlags,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectCompileExecute, 16);
        This routine is effectively equivalent to calling OSACompile followed by
        OSAExecute.  After execution, the compiled source is disposed.  Only the
        resulting value ID is retained.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errAECoercionFail:      sourceData is not compilable
            errOSAScriptError:      sourceData was a bad script (syntax error)
            errOSAInvalidID:        previousAndResultingCompiledScriptID was not
                                    valid on input
            errOSAScriptError:      the executing script got an error
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSADoScript()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSADoScript(
  ComponentInstance   scriptingComponent,
  const AEDesc *      sourceData,
  OSAID               contextID,
  DescType            desiredType,
  SInt32              modeFlags,
  AEDesc *            resultingText)                          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectDoScript, 20);
        This routine is effectively equivalent to calling OSACompile followed by
        OSAExecute and then OSADisplay.  After execution, the compiled source
        and the resulting value are is disposed.  Only the resultingText
        descriptor is retained.  If a script error occur during processing, the 
        resultingText gets the error message of the error, and errOSAScriptError
        is returned.  OSAScriptError may still be used to extract more 
        information about the particular error.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errAECoercionFail:      sourceData is not compilable or 
                                    desiredType not supported by scripting component
            errOSAScriptError:      sourceData was a bad script (syntax error)
            errOSAInvalidID:        previousAndResultingCompiledScriptID was not
                                    valid on input
            errOSAScriptError:      the executing script got an error
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
            kOSAModeDisplayForHumans
    */
/**************************************************************************
    OSA Optional Dialects Interface
**************************************************************************
    Scripting components that support the Dialects interface have the 
    kOSASupportsDialects bit set in their ComponentDescription.
**************************************************************************/
/*
    These calls allows an scripting component that supports different dialects
    to dynamically switch between those dialects.  Although this interface is
    specified, the particular dialect codes are scripting component dependent.
*/
/*
 *  OSASetCurrentDialect()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetCurrentDialect(
  ComponentInstance   scriptingComponent,
  short               dialectCode)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetCurrentDialect, 2);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSANoSuchDialect:    invalid dialectCode
    */
/*
 *  OSAGetCurrentDialect()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetCurrentDialect(
  ComponentInstance   scriptingComponent,
  short *             resultingDialectCode)                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetCurrentDialect, 4);
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAAvailableDialects()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAAvailableDialects(
  ComponentInstance   scriptingComponent,
  AEDesc *            resultingDialectInfoList)               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectAvailableDialects, 4);
        This call return an AEList containing information about each of the
        currently available dialects of a scripting component.  Each item
        is an AERecord of typeOSADialectInfo that contains at least the fields
        keyOSADialectName, keyOSADialectCode, KeyOSADialectLangCode and 
        keyOSADialectScriptCode.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAGetDialectInfo()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetDialectInfo(
  ComponentInstance   scriptingComponent,
  short               dialectCode,
  OSType              selector,
  AEDesc *            resultingDialectInfo)                   __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetDialectInfo, 10);
        This call gives information about the specified dialect of a scripting
        component. It returns an AEDesc whose type depends on the selector 
        specified. Available selectors are the same as the field keys for a
        dialect info record. The type of AEDesc returned is the same as the 
        type of the field that has same key as the selector.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSABadSelector
            errOSANoSuchDialect:    invalid dialectCode
    */
/*
 *  OSAAvailableDialectCodeList()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAAvailableDialectCodeList(
  ComponentInstance   scriptingComponent,
  AEDesc *            resultingDialectCodeList)               __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectAvailableDialectCodeList, 4);
        This is alternative to OSAGetAvailableDialectCodeList. Use this call
        and  OSAGetDialectInfo to get information on dialects.
        This call return an AEList containing dialect code for each of the
        currently available dialects of a scripting component. Each dialect
        code is a short integer of type typeSInt16.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError

        Type of a dialect info record containing at least keyOSADialectName
        and keyOSADialectCode fields.

        keys for dialect info record, also used as selectors to OSAGetDialectInfo.

        Field of a typeOSADialectInfo record of typeChar.
        Field of a typeOSADialectInfo record of typeSInt16.
        Field of a typeOSADialectInfo record of typeSInt16.
        Field of a typeOSADialectInfo record of typeSInt16.
    */
/**************************************************************************
    OSA Optional Event Handling Interface
**************************************************************************
    Scripting components that support the Event Handling interface have the 
    kOSASupportsEventHandling bit set in their ComponentDescription.
**************************************************************************/
/*
 *  OSASetResumeDispatchProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSASetResumeDispatchProc(
  ComponentInstance   scriptingComponent,
  AEEventHandlerUPP   resumeDispatchProc,
  SRefCon             refCon)                                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectSetResumeDispatchProc, 8);
        This function is used to set the ResumeDispatchProc that will be used
        by OSAExecuteEvent and OSADoEvent if either no event handler can be
        found in the context, or the context event hander "continues" control
        onward. The two constants kOSAUseStandardDispatch and kOSANoDispatch
        may also be passed to this routine indicating that the handler registered
        in the application with AEInstallEventHandler should be used, or no
        dispatch should occur, respectively.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
enum {
  kOSAUseStandardDispatch       = kAEUseStandardDispatch
};

/*
        Special ResumeDispatchProc constant which may be passed to 
        OSASetResumeDispatchProc indicating that the handler registered
        in the application with AEInstallEventHandler should be used.
        
        NOTE:   Had to remove the cast (AEEventHandlerUPP).  The C compiler
                doesn't allow pointer types to be assigned to an enum.  All
                constants must be assigned as enums to translate properly to
                Pascal.
    */
enum {
  kOSANoDispatch                = kAENoDispatch
};

/*
        Special ResumeDispatchProc constant which may be passed to 
        OSASetResumeDispatchProc indicating that no dispatch should occur.
        
        NOTE:   Had to remove the cast (AEEventHandlerUPP).  The C compiler
                doesn't allow pointer types to be assigned to an enum.  All
                constants must be assigned as enums to translate properly to
                Pascal.
    */
enum {
  kOSADontUsePhac               = 0x0001
};

/*
        Special refCon constant that may be given to OSASetResumeDispatchProc
        only when kOSAUseStandardDispatch is used as the ResumeDispatchProc.
        This causes the standard dispatch to be performed, except the phac
        handler is not called.  This is useful during tinkerability, when
        the phac handler is used to lookup a context associated with an event's 
        direct parameter, and call OSAExecuteEvent or OSADoEvent.  Failure to
        bypass the phac handler would result in an infinite loop.
    */
/*
 *  OSAGetResumeDispatchProc()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAGetResumeDispatchProc(
  ComponentInstance    scriptingComponent,
  AEEventHandlerUPP *  resumeDispatchProc,
  SRefCon *            refCon)                                __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectGetResumeDispatchProc, 8);
        Returns the registered ResumeDispatchProc.  If no ResumeDispatchProc has
        been registered, then kOSAUseStandardDispatch (the default) is returned.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
    */
/*
 *  OSAExecuteEvent()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAExecuteEvent(
  ComponentInstance   scriptingComponent,
  const AppleEvent *  theAppleEvent,
  OSAID               contextID,
  SInt32              modeFlags,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectExecuteEvent, 16);
        This call is similar to OSAExecute except the initial command to
        execute comes in the form of an AppleEvent.  If the contextID
        defines any event handlers for that event, they are used to process
        the event.  If no event handler can be found in the context
        errAEEventNotHandled is returned.  If an event handler is found and
        the hander "continues" control onward, the ResumeDispatchProc
        (registered with OSASetResumeDispatchProc, above) is called given the
        AppleEvent.  The result is returned as a scriptValueID.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errOSAScriptError:      the executing script got an error
            errAEEventNotHandled:   no handler for event in contextID
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSADoEvent()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSADoEvent(
  ComponentInstance   scriptingComponent,
  const AppleEvent *  theAppleEvent,
  OSAID               contextID,
  SInt32              modeFlags,
  AppleEvent *        reply)                                  __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectDoEvent, 16);
        This call is similar to OSADoScript except the initial command to
        execute comes in the form of an AppleEvent, and the result is an 
        AppleEvent reply record.  If the contextID defines any event handlers
        for that event, they are used to process the event.  If no event handler
        can be found in the context errAEEventNotHandled is returned.  If an
        event handler is found and the hander "continues" control onward, the
        ResumeDispatchProc (registered with OSASetResumeDispatchProc, above) is
        called given the AppleEvent.  The result is returned in the form of an
        AppleEvent reply descriptor. If at any time the script gets an error, or
        if the ResumeDispatchProc returns a reply event indicating an error,
        then the OSADoEvent call itself returns an error reply (i.e. OSADoEvent
        should never return errOSAScriptError).  Any error result returned by
        the ResumeDispatchProc will be returned by OSADoEvent.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errAEEventNotHandled:   no handler for event in contextID
    
        ModeFlags:
            kOSAModeNeverInteract
            kOSAModeCanInteract
            kOSAModeAlwaysInteract
            kOSAModeCantSwitchLayer
            kOSAModeDontReconnect
            kOSAModeDoRecord
    */
/*
 *  OSAMakeContext()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
OSAMakeContext(
  ComponentInstance   scriptingComponent,
  const AEDesc *      contextName,
  OSAID               parentContext,
  OSAID *             resultingContextID)                     __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        OSAComponentFunctionInline(kOSASelectMakeContext, 12);
        Makes a new empty context which may be passed to OSAExecute or 
        OSAExecuteEvent.  If contextName is typeNull, an unnamed context is
        created. If parentContext is kOSANullScript then the resulting context
        does not inherit bindings from any other context.
    
        Errors:
            badComponentInstance    invalid scripting component instance
            errOSASystemError
            errOSAInvalidID
            errAECoercionFail:      contextName is invalid
    */


/**************************************************************************
    OSA Script File Interface
**************************************************************************/
/*
 *  OSAGetScriptDataFromURL()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSAGetScriptDataFromURL(
  CFURLRef   scriptURL,
  Boolean *  storable,                  /* can be NULL */
  SInt32     modeFlags,
  AEDesc *   resultingScriptData)                             __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_NA);


/*
        This routine reads script data from a URL into a descriptor. The
        URL may refer to a compiled script, script application, or source
        text file. If "scriptURL" refers to a text file, the resulting data
        is the source text. If "storable" is non-NULL, it will be set to
        indicate whether a script can be stored into the script file using
        OSAStoreFile().
     
        You may use OSALoadScriptData() with the resulting descriptor to
        load the script into a component instance. Doing this in two steps
        affords the opportunity to examine the script data with
        OSAGetStorageType() and select a component instance.
    
        Errors:
            errOSASystemError
            File system errors.
            
        ModeFlags:
            No mode flags are supported at this time.
    */
/*
 *  OSALoadScriptData()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSALoadScriptData(
  ComponentInstance   scriptingComponent,
  const AEDesc *      scriptData,
  CFURLRef            fromURL,                  /* can be NULL */
  SInt32              modeFlags,
  OSAID *             resultingScriptID)                      __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_NA);


/*
        This routine loads script data from a descriptor into the specified
        scripting component. If "scriptData" is source, it will be compiled.
        If "fromURL" is non-NULL, it indicates the file from which the data
        was read.
    
        Errors:
            See OSALoad() and OSACompile().
            
        ModeFlags:
            See OSALoad() and OSACompile().
    */
/*
 *  OSALoadFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSALoadFile(
  ComponentInstance   scriptingComponent,
  const FSRef *       scriptFile,
  Boolean *           storable,                 /* can be NULL */
  SInt32              modeFlags,
  OSAID *             resultingScriptID)                      __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/*
        This routine loads a script into the specified scripting component.
        If "scriptFile" is a text file, the script will be compiled. If
        "storable" is non-NULL, it will be set to indicate whether a
        script can be stored into the script file using OSAStoreFile().
    
        Errors:
            See OSALoad() and OSACompile().
            
        ModeFlags:
            See OSALoad() and OSACompile().
    */
/*
 *  OSAStoreFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSAStoreFile(
  ComponentInstance   scriptingComponent,
  OSAID               scriptID,
  DescType            desiredType,
  SInt32              modeFlags,
  const FSRef *       scriptFile)                             __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/*
        This routine stores a script into the specified file.
    
        Errors:
            See OSAStore().
        
        ModeFlags:
            See OSAStore().
    */
/*
 *  OSALoadExecuteFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSALoadExecuteFile(
  ComponentInstance   scriptingComponent,
  const FSRef *       scriptFile,
  OSAID               contextID,
  SInt32              modeFlags,
  OSAID *             resultingScriptValueID)                 __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/*
        This routine is effectively equivalent to calling OSALoadFile followed by
        OSAExecute.  After execution, the compiled source is disposed.  Only the
        resulting value ID is retained.
    
        Errors:
            See OSALoadExecute() and OSACompileExecute().
    
        ModeFlags:
            See OSALoadExecute() and OSACompileExecute().
    */
/*
 *  OSADoScriptFile()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
OSADoScriptFile(
  ComponentInstance   scriptingComponent,
  const FSRef *       scriptFile,
  OSAID               contextID,
  DescType            desiredType,
  SInt32              modeFlags,
  AEDesc *            resultingText)                          __OSX_AVAILABLE_STARTING(__MAC_10_3, __IPHONE_NA);


/*
        This routine is effectively equivalent to calling OSALoadFile, followed by 
        OSAExecute, OSADisplay, and then OSAStoreFile if the script has persistent 
        properties.  After execution, the compiled source and the resulting value are 
        disposed.  Only the resultingText descriptor is retained.  If a script error 
        occurs during processing, the resultingText gets the error message of the error, 
        and errOSAScriptError is returned.  OSAScriptError may still be used to extract 
        more information about the particular error.
    
        Errors:
            See OSALoad(), OSACompile(), OSAExecute(), OSADisplay(), and OSAStore().
    
        ModeFlags:
            See OSALoad(), OSACompile(), OSAExecute(), and OSADisplay().
    */

#ifdef __cplusplus
}
#endif

#endif /* __OSA__ */

