/*
     File:       CarbonCore/Gestalt.h
 
     Contains:   Gestalt Interfaces.
                 The contents of this header file are deprecated.
                 Use sysctl(3) instead.
 
     Copyright:  © 1988-2011 by Apple Inc. All rights reserved.
*/
#ifndef __GESTALT__
#define __GESTALT__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MIXEDMODE__
#include <CarbonCore/MixedMode.h>
#endif



#include <Availability.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif



typedef CALLBACK_API( OSErr , SelectorFunctionProcPtr )(OSType selector, SInt32 *response);
typedef STACK_UPP_TYPE(SelectorFunctionProcPtr)                 SelectorFunctionUPP;
/*
 *  Gestalt()
 *  
 *  Summary:
 *    Gestalt returns information about the operating environment.
 *  
 *  Discussion:
 *    The Gestalt function places the information requested by the
 *    selector parameter in the variable parameter response . Note that
 *    Gestalt returns the response from all selectors in an SInt32,
 *    which occupies 4 bytes. When not all 4 bytes are needed, the
 *    significant information appears in the low-order byte or bytes.
 *    Although the response parameter is declared as a variable
 *    parameter, you cannot use it to pass information to Gestalt or to
 *    a Gestalt selector function. Gestalt interprets the response
 *    parameter as an address at which it is to place the result
 *    returned by the selector function specified by the selector
 *    parameter. Gestalt ignores any information already at that
 *    address.
 *    
 *    The Apple-defined selector codes fall into two categories:
 *    environmental selectors, which supply specific environmental
 *    information you can use to control the behavior of your
 *    application, and informational selectors, which supply
 *    information you can't use to determine what hardware or software
 *    features are available. You can use one of the selector codes
 *    defined by Apple (listed in the "Constants" section beginning on
 *    page 1-14 ) or a selector code defined by a third-party
 *    product.
 *    
 *    The meaning of the value that Gestalt returns in the response
 *    parameter depends on the selector code with which it was called.
 *    For example, if you call Gestalt using the gestaltTimeMgrVersion
 *    selector, it returns a version code in the response parameter. In
 *    this case, a returned value of 3 indicates that the extended Time
 *    Manager is available.
 *    
 *    In most cases, the last few characters in the selector's symbolic
 *    name form a suffix that indicates what type of value you can
 *    expect Gestalt to place in the response parameter. For example,
 *    if the suffix in a Gestalt selector is Size , then Gestalt
 *    returns a size in the response parameter.
 *    
 *    Attr:  A range of 32 bits, the meanings of which are defined by a
 *    list of constants. Bit 0 is the least significant bit of the
 *    SInt32.
 *    Count: A number indicating how many of the indicated type of item
 *    exist.
 *    Size: A size, usually in bytes.
 *    Table: The base address of a table.
 *    Type: An index to a list of feature descriptions.
 *    Version: A version number, which can be either a constant with a
 *    defined meaning or an actual version number, usually stored as
 *    four hexadecimal digits in the low-order word of the return
 *    value. Implied decimal points may separate digits. The value
 *    $0701, for example, returned in response to the
 *    gestaltSystemVersion selector, represents system software version
 *    7.0.1.
 *    
 *    Selectors that have the suffix Attr deserve special attention.
 *    They cause Gestalt to return a bit field that your application
 *    must interpret to determine whether a desired feature is present.
 *    For example, the application-defined sample function
 *    MyGetSoundAttr , defined in Listing 1-2 on page 1-6 , returns a
 *    LongInt that contains the Sound Manager attributes field
 *    retrieved from Gestalt . To determine whether a particular
 *    feature is available, you need to look at the designated bit.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      The selector to return information for
 *    
 *    response:
 *      On exit, the requested information whose format depends on the
 *      selector specified.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
Gestalt(
  OSType    selector,
  SInt32 *  response)                                         __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


#if !__LP64__
/*
 *  ReplaceGestalt()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use NewGestaltValue instead wherever possible.
 *  
 *  Summary:
 *    Replaces the gestalt function associated with a selector.
 *  
 *  Discussion:
 *    The ReplaceGestalt function replaces the selector function
 *    associated with an existing selector code.
 *    
 *    So that your function can call the function previously associated
 *    with the selector, ReplaceGestalt places the address of the old
 *    selector function in the oldGestaltFunction parameter. If
 *    ReplaceGestalt returns an error of any type, then the value of
 *    oldGestaltFunction is undefined.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to replace
 *    
 *    gestaltFunction:
 *      a UPP for the new selector function
 *    
 *    oldGestaltFunction:
 *      on exit, a pointer to the UPP of the previously associated with
 *      the specified selector
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ReplaceGestalt(
  OSType                 selector,
  SelectorFunctionUPP    gestaltFunction,
  SelectorFunctionUPP *  oldGestaltFunction)                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  NewGestalt()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use NewGestaltValue instead wherever possible.
 *  
 *  Summary:
 *    Adds a selector code to those already recognized by Gestalt.
 *  
 *  Discussion:
 *    The NewGestalt function registers a specified selector code with
 *    the Gestalt Manager so that when the Gestalt function is called
 *    with that selector code, the specified selector function is
 *    executed. Before calling NewGestalt, you must define a selector
 *    function callback. See SelectorFunctionProcPtr for a description
 *    of how to define your selector function.
 *    
 *    Registering with the Gestalt Manager is a way for software such
 *    as system extensions to make their presence known to potential
 *    users of their services.
 *    
 *    In Mac OS X, the selector and replacement value are on a
 *    per-context basis. That means they are available only to the
 *    application or other code that installs them. You cannot use this
 *    function to make information available to another process.
 *     
 *    A Gestalt selector registered with NewGestalt() can not be
 *    deleted.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to create
 *    
 *    gestaltFunction:
 *      a UPP for the new selector function, which Gestalt executes
 *      when it receives the new selector code. See
 *      SelectorFunctionProcPtr for more information on the callback
 *      you need to provide.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NewGestalt(
  OSType                selector,
  SelectorFunctionUPP   gestaltFunction)                      __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_3, __IPHONE_NA, __IPHONE_NA);


#endif  /* !__LP64__ */

/*
 *  NewGestaltValue()
 *  
 *  Summary:
 *    Adds a selector code to those already recognized by Gestalt.
 *  
 *  Discussion:
 *    The NewGestalt function registers a specified selector code with
 *    the Gestalt Manager so that when the Gestalt function is called
 *    with that selector code, the specified selector function is
 *    executed. Before calling NewGestalt, you must define a selector
 *    function callback. See SelectorFunctionProcPtr for a description
 *    of how to define your selector function.
 *    
 *    Registering with the Gestalt Manager is a way for software such
 *    as system extensions to make their presence known to potential
 *    users of their services.
 *    
 *    In Mac OS X, the selector and replacement value are on a
 *    per-context basis. That means they are available only to the
 *    application or other code that installs them. You cannot use this
 *    function to make information available to another process.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to create
 *    
 *    newValue:
 *      the value to return for the new selector code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.5 and later
 */
extern OSErr 
NewGestaltValue(
  OSType   selector,
  SInt32   newValue)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  ReplaceGestaltValue()
 *  
 *  Summary:
 *    Replaces the value that the function Gestalt returns for a
 *    specified selector code with the value provided to the function.
 *  
 *  Discussion:
 *    You use the function ReplaceGestaltValue to replace an existing
 *    value. You should not call this function to introduce a value
 *    that doesn't already exist; instead call the function
 *    NewGestaltValue.
 *    
 *    In Mac OS X, the selector and replacement value are on a
 *    per-context basis. That means they are available only to the
 *    application or other code that installs them. You cannot use this
 *    function to make information available to another process.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to create
 *    
 *    replacementValue:
 *      the new value to return for the new selector code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.5 and later
 */
extern OSErr 
ReplaceGestaltValue(
  OSType   selector,
  SInt32   replacementValue)                                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  SetGestaltValue()
 *  
 *  Summary:
 *    Sets the value the function Gestalt will return for a specified
 *    selector code, installing the selector if it was not already
 *    installed.
 *  
 *  Discussion:
 *    You use SetGestaltValue to establish a value for a selector,
 *    without regard to whether the selector was already installed.
 *     
 *    In Mac OS X, the selector and replacement value are on a
 *    per-context basis. That means they are available only to the
 *    application or other code that installs them. You cannot use this
 *    function to make information available to another process.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to create
 *    
 *    newValue:
 *      the new value to return for the new selector code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.5 and later
 */
extern OSErr 
SetGestaltValue(
  OSType   selector,
  SInt32   newValue)                                          __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);


/*
 *  DeleteGestaltValue()
 *  
 *  Summary:
 *    Deletes a Gestalt selector code so that it is no longer
 *    recognized by Gestalt.
 *  
 *  Discussion:
 *    After calling this function, subsequent query or replacement
 *    calls for the selector code will fail as if the selector had
 *    never been installed 
 *    
 *    In Mac OS X, the selector and replacement value are on a
 *    per-context basis. That means they are available only to the
 *    application or other code that installs them.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Parameters:
 *    
 *    selector:
 *      the selector to delete
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.5 and later
 */
extern OSErr 
DeleteGestaltValue(OSType selector)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);



/*
 *  NewSelectorFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern SelectorFunctionUPP
NewSelectorFunctionUPP(SelectorFunctionProcPtr userRoutine)   __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  DisposeSelectorFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeSelectorFunctionUPP(SelectorFunctionUPP userUPP)       __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

/*
 *  InvokeSelectorFunctionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in CoreServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeSelectorFunctionUPP(
  OSType               selector,
  SInt32 *             response,
  SelectorFunctionUPP  userUPP)                               __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA);

#if __MACH__
  #ifdef __cplusplus
    inline SelectorFunctionUPP                                  NewSelectorFunctionUPP(SelectorFunctionProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeSelectorFunctionUPP(SelectorFunctionUPP) { }
    inline OSErr                                                InvokeSelectorFunctionUPP(OSType selector, SInt32 * response, SelectorFunctionUPP userUPP) { return (*userUPP)(selector, response); }
  #else
    #define NewSelectorFunctionUPP(userRoutine)                 ((SelectorFunctionUPP)userRoutine)
    #define DisposeSelectorFunctionUPP(userUPP)
    #define InvokeSelectorFunctionUPP(selector, response, userUPP) (*userUPP)(selector, response)
  #endif
#endif


/* Environment Selectors */
enum {
  gestaltAddressingModeAttr     = 'addr', /* addressing mode attributes */
  gestalt32BitAddressing        = 0,    /* using 32-bit addressing mode */
  gestalt32BitSysZone           = 1,    /* 32-bit compatible system zone */
  gestalt32BitCapable           = 2     /* Machine is 32-bit capable */
};

enum {
  gestaltAFPClient              = 'afps',
  gestaltAFPClientVersionMask   = 0x0000FFFF, /* low word of SInt32 is the */
                                        /* client version 0x0001 -> 0x0007*/
  gestaltAFPClient3_5           = 0x0001,
  gestaltAFPClient3_6           = 0x0002,
  gestaltAFPClient3_6_1         = 0x0003,
  gestaltAFPClient3_6_2         = 0x0004,
  gestaltAFPClient3_6_3         = 0x0005, /* including 3.6.4, 3.6.5*/
  gestaltAFPClient3_7           = 0x0006, /* including 3.7.1*/
  gestaltAFPClient3_7_2         = 0x0007, /* including 3.7.3, 3.7.4*/
  gestaltAFPClient3_8           = 0x0008,
  gestaltAFPClient3_8_1         = 0x0009, /* including 3.8.2 */
  gestaltAFPClient3_8_3         = 0x000A,
  gestaltAFPClient3_8_4         = 0x000B, /* including 3.8.5, 3.8.6 */
  gestaltAFPClientAttributeMask = (int)0xFFFF0000, /* high word of response is a */
                                        /* set of attribute bits*/
  gestaltAFPClientCfgRsrc       = 16,   /* Client uses config resources*/
  gestaltAFPClientSupportsIP    = 29,   /* Client supports AFP over TCP/IP*/
  gestaltAFPClientVMUI          = 30,   /* Client can put up UI from the PBVolMount trap*/
  gestaltAFPClientMultiReq      = 31    /* Client supports multiple outstanding requests*/
};


enum {
  gestaltAliasMgrAttr           = 'alis', /* Alias Mgr Attributes */
  gestaltAliasMgrPresent        = 0,    /* True if the Alias Mgr is present */
  gestaltAliasMgrSupportsRemoteAppletalk = 1, /* True if the Alias Mgr knows about Remote Appletalk */
  gestaltAliasMgrSupportsAOCEKeychain = 2, /* True if the Alias Mgr knows about the AOCE Keychain */
  gestaltAliasMgrResolveAliasFileWithMountOptions = 3, /* True if the Alias Mgr implements gestaltAliasMgrResolveAliasFileWithMountOptions() and IsAliasFile() */
  gestaltAliasMgrFollowsAliasesWhenResolving = 4,
  gestaltAliasMgrSupportsExtendedCalls = 5,
  gestaltAliasMgrSupportsFSCalls = 6,   /* true if Alias Mgr supports HFS+ Calls */
  gestaltAliasMgrPrefersPath    = 7,    /* True if the Alias Mgr prioritizes the path over file id during resolution by default */
  gestaltAliasMgrRequiresAccessors = 8  /* Set if Alias Manager requires accessors for size and usertype */
};

/* Gestalt selector and values for the Appearance Manager */
enum {
  gestaltAppearanceAttr         = 'appr',
  gestaltAppearanceExists       = 0,
  gestaltAppearanceCompatMode   = 1
};

/* Gestalt selector for determining Appearance Manager version   */
/* If this selector does not exist, but gestaltAppearanceAttr    */
/* does, it indicates that the 1.0 version is installed. This    */
/* gestalt returns a BCD number representing the version of the  */
/* Appearance Manager that is currently running, e.g. 0x0101 for */
/* version 1.0.1.                                                */
enum {
  gestaltAppearanceVersion      = 'apvr'
};

enum {
  gestaltArbitorAttr            = 'arb ',
  gestaltSerialArbitrationExists = 0    /* this bit if the serial port arbitrator exists*/
};

enum {
  gestaltAppleScriptVersion     = 'ascv' /* AppleScript version*/
};

enum {
  gestaltAppleScriptAttr        = 'ascr', /* AppleScript attributes*/
  gestaltAppleScriptPresent     = 0,
  gestaltAppleScriptPowerPCSupport = 1
};

enum {
  gestaltATAAttr                = 'ata ', /* ATA is the driver to support IDE hard disks */
  gestaltATAPresent             = 0     /* if set, ATA Manager is present */
};

enum {
  gestaltATalkVersion           = 'atkv' /* Detailed AppleTalk version; see comment above for format */
};

enum {
  gestaltAppleTalkVersion       = 'atlk' /* appletalk version */
};

/*
    FORMAT OF gestaltATalkVersion RESPONSE
    --------------------------------------
    The version is stored in the high three bytes of the response value.  Let us number
    the bytes in the response value from 0 to 3, where 0 is the least-significant byte.

        Byte#:     3 2 1 0
        Value:  0xMMNNRR00

    Byte 3 (MM) contains the major revision number, byte 2 (NN) contains the minor
    revision number, and byte 1 (RR) contains a constant that represents the release
    stage.  Byte 0 always contains 0x00.  The constants for the release stages are:

        development = 0x20
        alpha       = 0x40
        beta        = 0x60
        final       = 0x80
        release     = 0x80

    For example, if you call Gestalt with the 'atkv' selector when AppleTalk version 57
    is loaded, you receive the integer response value 0x39008000.
*/
enum {
  gestaltAUXVersion             = 'a/ux' /* a/ux version, if present */
};

enum {
  gestaltMacOSCompatibilityBoxAttr = 'bbox', /* Classic presence and features */
  gestaltMacOSCompatibilityBoxPresent = 0, /* True if running under the Classic */
  gestaltMacOSCompatibilityBoxHasSerial = 1, /* True if Classic serial support is implemented. */
  gestaltMacOSCompatibilityBoxless = 2  /* True if we're Boxless (screen shared with Carbon/Cocoa) */
};

enum {
  gestaltBusClkSpeed            = 'bclk' /* main I/O bus clock speed in hertz */
};

enum {
  gestaltBusClkSpeedMHz         = 'bclm' /* main I/O bus clock speed in megahertz ( a UInt32 ) */
};

enum {
  gestaltCloseViewAttr          = 'BSDa', /* CloseView attributes */
  gestaltCloseViewEnabled       = 0,    /* Closeview enabled (dynamic bit - returns current state) */
  gestaltCloseViewDisplayMgrFriendly = 1 /* Closeview compatible with Display Manager (FUTURE) */
};

enum {
  gestaltCarbonVersion          = 'cbon' /* version of Carbon API present in system */
};

enum {
  gestaltCFMAttr                = 'cfrg', /* Selector for information about the Code Fragment Manager */
  gestaltCFMPresent             = 0,    /* True if the Code Fragment Manager is present */
  gestaltCFMPresentMask         = 0x0001,
  gestaltCFM99Present           = 2,    /* True if the CFM-99 features are present. */
  gestaltCFM99PresentMask       = 0x0004
};

enum {
  gestaltProcessorCacheLineSize = 'csiz' /* The size, in bytes, of the processor cache line. */
};

enum {
  gestaltCollectionMgrVersion   = 'cltn' /* Collection Manager version */
};

enum {
  gestaltColorMatchingAttr      = 'cmta', /* ColorSync attributes */
  gestaltHighLevelMatching      = 0,
  gestaltColorMatchingLibLoaded = 1
};

enum {
  gestaltColorMatchingVersion   = 'cmtc',
  gestaltColorSync10            = 0x0100, /* 0x0100 & 0x0110 _Gestalt versions for 1.0-1.0.3 product */
  gestaltColorSync11            = 0x0110, /*   0x0100 == low-level matching only */
  gestaltColorSync104           = 0x0104, /* Real version, by popular demand */
  gestaltColorSync105           = 0x0105,
  gestaltColorSync20            = 0x0200, /* ColorSync 2.0 */
  gestaltColorSync21            = 0x0210,
  gestaltColorSync211           = 0x0211,
  gestaltColorSync212           = 0x0212,
  gestaltColorSync213           = 0x0213,
  gestaltColorSync25            = 0x0250,
  gestaltColorSync26            = 0x0260,
  gestaltColorSync261           = 0x0261,
  gestaltColorSync30            = 0x0300
};

enum {
  gestaltControlMgrVersion      = 'cmvr' /* NOTE: The first version we return is 3.0.1, on Mac OS X plus update 2*/
};

enum {
  gestaltControlMgrAttr         = 'cntl', /* Control Mgr*/
  gestaltControlMgrPresent      = (1L << 0), /* NOTE: this is a bit mask, whereas all other Gestalt constants of*/
                                        /* this type are bit index values.   Universal Interfaces 3.2 slipped*/
                                        /* out the door with this mistake.*/
  gestaltControlMgrPresentBit   = 0,    /* bit number*/
  gestaltControlMsgPresentMask  = (1L << gestaltControlMgrPresentBit)
};

enum {
  gestaltConnMgrAttr            = 'conn', /* connection mgr attributes    */
  gestaltConnMgrPresent         = 0,
  gestaltConnMgrCMSearchFix     = 1,    /* Fix to CMAddSearch?     */
  gestaltConnMgrErrorString     = 2,    /* has CMGetErrorString() */
  gestaltConnMgrMultiAsyncIO    = 3     /* CMNewIOPB, CMDisposeIOPB, CMPBRead, CMPBWrite, CMPBIOKill */
};

enum {
  gestaltColorPickerVersion     = 'cpkr', /* returns version of ColorPicker */
  gestaltColorPicker            = 'cpkr' /* gestaltColorPicker is old name for gestaltColorPickerVersion */
};

enum {
  gestaltComponentMgr           = 'cpnt', /* Component Mgr version */
  gestaltComponentPlatform      = 'copl' /* Component Platform number */
};

/*
    The gestaltNativeCPUtype ('cput') selector can be used to determine the
    native CPU type for all Macs running System 7.5 or later.

    However, the use of these selectors for pretty much anything is discouraged.
    If you are trying to determine if you can use a particular processor or
    operating system feature, it would be much better to check directly for that
    feature using one of the apis for doing so -- like, sysctl() or sysctlbyname().
    Those apis return information directly from the operating system and kernel.  By
    using those apis you may be able to avoid linking against Frameworks which you
    otherwise don't need, and may lessen the memory and code footprint of your
    applications.
    
    The gestaltNativeCPUfamily ('cpuf') selector can be used to determine the
    general family the native CPU is in.

    gestaltNativeCPUfamily uses the same results as gestaltNativeCPUtype, but
    will only return certain CPU values.
    
    IMPORTANT NOTE: gestaltNativeCPUFamily may no longer be updated for any new
                    processor families introduced after the 970.  If there are
                    processor specific features you need to be checking for in
                    your code, use one of the appropriate apis to get for those
                    exact features instead of assuming that all members of a given
                    cpu family exhibit the same behaviour.  The most appropriate api
                    to look at is sysctl() and sysctlbyname(), which return information
                    direct from the kernel about the system.
*/
enum {
  gestaltNativeCPUtype          = 'cput', /* Native CPU type                          */
  gestaltNativeCPUfamily        = 'cpuf', /* Native CPU family                      */
  gestaltCPU68000               = 0,    /* Various 68k CPUs...    */
  gestaltCPU68010               = 1,
  gestaltCPU68020               = 2,
  gestaltCPU68030               = 3,
  gestaltCPU68040               = 4,
  gestaltCPU601                 = 0x0101, /* IBM 601                               */
  gestaltCPU603                 = 0x0103,
  gestaltCPU604                 = 0x0104,
  gestaltCPU603e                = 0x0106,
  gestaltCPU603ev               = 0x0107,
  gestaltCPU750                 = 0x0108, /* Also 740 - "G3" */
  gestaltCPU604e                = 0x0109,
  gestaltCPU604ev               = 0x010A, /* Mach 5, 250Mhz and up */
  gestaltCPUG4                  = 0x010C, /* Max */
  gestaltCPUG47450              = 0x0110 /* Vger , Altivec */
};

enum {
  gestaltCPUApollo              = 0x0111, /* Apollo , Altivec, G4 7455 */
  gestaltCPUG47447              = 0x0112,
  gestaltCPU750FX               = 0x0120, /* Sahara,G3 like thing */
  gestaltCPU970                 = 0x0139, /* G5 */
  gestaltCPU970FX               = 0x013C, /* another G5 */
  gestaltCPU970MP               = 0x0144
};

enum {
                                        /* x86 CPUs all start with 'i' in the high nybble */
  gestaltCPU486                 = 'i486',
  gestaltCPUPentium             = 'i586',
  gestaltCPUPentiumPro          = 'i5pr',
  gestaltCPUPentiumII           = 'i5ii',
  gestaltCPUX86                 = 'ixxx',
  gestaltCPUPentium4            = 'i5iv',
};

enum {
	gestaltCPUARMFamily			= 'arm ',
	gestaltCPUARM64				= 'ax64'
};

enum {
  gestaltCRMAttr                = 'crm ', /* comm resource mgr attributes */
  gestaltCRMPresent             = 0,
  gestaltCRMPersistentFix       = 1,    /* fix for persistent tools */
  gestaltCRMToolRsrcCalls       = 2     /* has CRMGetToolResource/ReleaseToolResource */
};

enum {
  gestaltControlStripVersion    = 'csvr' /* Control Strip version (was 'sdvr') */
};

enum {
  gestaltCountOfCPUs            = 'cpus' /* the number of CPUs on the computer, Mac OS X 10.4 and later */
};

enum {
  gestaltCTBVersion             = 'ctbv' /* CommToolbox version */
};

enum {
  gestaltDBAccessMgrAttr        = 'dbac', /* Database Access Mgr attributes */
  gestaltDBAccessMgrPresent     = 0     /* True if Database Access Mgr present */
};

enum {
  gestaltDiskCacheSize          = 'dcsz' /* Size of disk cache's buffers, in bytes */
};

enum {
  gestaltSDPFindVersion         = 'dfnd' /* OCE Standard Directory Panel*/
};

enum {
  gestaltDictionaryMgrAttr      = 'dict', /* Dictionary Manager attributes */
  gestaltDictionaryMgrPresent   = 0     /* Dictionary Manager attributes */
};

enum {
  gestaltDITLExtAttr            = 'ditl', /* AppenDITL, etc. calls from CTB */
  gestaltDITLExtPresent         = 0,    /* True if calls are present */
  gestaltDITLExtSupportsIctb    = 1     /* True if AppendDITL, ShortenDITL support 'ictb's */
};

enum {
  gestaltDialogMgrAttr          = 'dlog', /* Dialog Mgr*/
  gestaltDialogMgrPresent       = (1L << 0), /* NOTE: this is a bit mask, whereas all other Gestalt constants of*/
                                        /* this type are bit index values.   Universal Interfaces 3.2 slipped*/
                                        /* out the door with this mistake.*/
  gestaltDialogMgrPresentBit    = 0,    /* bit number*/
  gestaltDialogMgrHasAquaAlertBit = 2,  /* bit number*/
  gestaltDialogMgrPresentMask   = (1L << gestaltDialogMgrPresentBit),
  gestaltDialogMgrHasAquaAlertMask = (1L << gestaltDialogMgrHasAquaAlertBit),
  gestaltDialogMsgPresentMask   = gestaltDialogMgrPresentMask /* compatibility mask*/
};

enum {
  gestaltDesktopPicturesAttr    = 'dkpx', /* Desktop Pictures attributes */
  gestaltDesktopPicturesInstalled = 0,  /* True if control panel is installed */
  gestaltDesktopPicturesDisplayed = 1   /* True if a picture is currently displayed */
};

enum {
  gestaltDisplayMgrVers         = 'dplv' /* Display Manager version */
};

enum {
  gestaltDisplayMgrAttr         = 'dply', /* Display Manager attributes */
  gestaltDisplayMgrPresent      = 0,    /* True if Display Mgr is present */
  gestaltDisplayMgrCanSwitchMirrored = 2, /* True if Display Mgr can switch modes on mirrored displays */
  gestaltDisplayMgrSetDepthNotifies = 3, /* True SetDepth generates displays mgr notification */
  gestaltDisplayMgrCanConfirm   = 4,    /* True Display Manager supports DMConfirmConfiguration */
  gestaltDisplayMgrColorSyncAware = 5,  /* True if Display Manager supports profiles for displays */
  gestaltDisplayMgrGeneratesProfiles = 6, /* True if Display Manager will automatically generate profiles for displays */
  gestaltDisplayMgrSleepNotifies = 7    /* True if Display Mgr generates "displayWillSleep", "displayDidWake" notifications */
};

enum {
  gestaltDragMgrAttr            = 'drag', /* Drag Manager attributes */
  gestaltDragMgrPresent         = 0,    /* Drag Manager is present */
  gestaltDragMgrFloatingWind    = 1,    /* Drag Manager supports floating windows */
  gestaltPPCDragLibPresent      = 2,    /* Drag Manager PPC DragLib is present */
  gestaltDragMgrHasImageSupport = 3,    /* Drag Manager allows SetDragImage call */
  gestaltCanStartDragInFloatWindow = 4, /* Drag Manager supports starting a drag in a floating window */
  gestaltSetDragImageUpdates    = 5     /* Drag Manager supports drag image updating via SetDragImage */
};

enum {
  gestaltDrawSprocketVersion    = 'dspv' /* Draw Sprocket version (as a NumVersion) */
};

enum {
  gestaltDigitalSignatureVersion = 'dsig' /* returns Digital Signature Toolbox version in low-order word*/
};

/*
   Desktop Printing Feature Gestalt
   Use this gestalt to check if third-party printer driver support is available
*/
enum {
  gestaltDTPFeatures            = 'dtpf',
  kDTPThirdPartySupported       = 0x00000004 /* mask for checking if third-party drivers are supported*/
};


/*
   Desktop Printer Info Gestalt
   Use this gestalt to get a hold of information for all of the active desktop printers
*/
enum {
  gestaltDTPInfo                = 'dtpx' /* returns GestaltDTPInfoHdle*/
};

enum {
  gestaltEasyAccessAttr         = 'easy', /* Easy Access attributes */
  gestaltEasyAccessOff          = 0,    /* if Easy Access present, but off (no icon) */
  gestaltEasyAccessOn           = 1,    /* if Easy Access "On" */
  gestaltEasyAccessSticky       = 2,    /* if Easy Access "Sticky" */
  gestaltEasyAccessLocked       = 3     /* if Easy Access "Locked" */
};

enum {
  gestaltEditionMgrAttr         = 'edtn', /* Edition Mgr attributes */
  gestaltEditionMgrPresent      = 0,    /* True if Edition Mgr present */
  gestaltEditionMgrTranslationAware = 1 /* True if edition manager is translation manager aware */
};

enum {
  gestaltAppleEventsAttr        = 'evnt', /* Apple Events attributes */
  gestaltAppleEventsPresent     = 0,    /* True if Apple Events present */
  gestaltScriptingSupport       = 1,
  gestaltOSLInSystem            = 2,    /* OSL is in system so donÕt use the one linked in to app */
  gestaltSupportsApplicationURL = 4     /* Supports the typeApplicationURL addressing mode */
};

enum {
  gestaltExtensionTableVersion  = 'etbl' /* ExtensionTable version */
};


enum {
  gestaltFBCIndexingState       = 'fbci', /* Find By Content indexing state*/
  gestaltFBCindexingSafe        = 0,    /* any search will result in synchronous wait*/
  gestaltFBCindexingCritical    = 1     /* any search will execute immediately*/
};

enum {
  gestaltFBCVersion             = 'fbcv', /* Find By Content version*/
  gestaltFBCCurrentVersion      = 0x0011, /* First release for OS 8/9*/
  gestaltOSXFBCCurrentVersion   = 0x0100 /* First release for OS X*/
};


enum {
  gestaltFileMappingAttr        = 'flmp', /* File mapping attributes*/
  gestaltFileMappingPresent     = 0,    /* bit is set if file mapping APIs are present*/
  gestaltFileMappingMultipleFilesFix = 1 /* bit is set if multiple files per volume can be mapped*/
};

enum {
  gestaltFloppyAttr             = 'flpy', /* Floppy disk drive/driver attributes */
  gestaltFloppyIsMFMOnly        = 0,    /* Floppy driver only supports MFM disk formats */
  gestaltFloppyIsManualEject    = 1,    /* Floppy drive, driver, and file system are in manual-eject mode */
  gestaltFloppyUsesDiskInPlace  = 2     /* Floppy drive must have special DISK-IN-PLACE output; standard DISK-CHANGED not used */
};

enum {
  gestaltFinderAttr             = 'fndr', /* Finder attributes */
  gestaltFinderDropEvent        = 0,    /* Finder recognizes drop event */
  gestaltFinderMagicPlacement   = 1,    /* Finder supports magic icon placement */
  gestaltFinderCallsAEProcess   = 2,    /* Finder calls AEProcessAppleEvent */
  gestaltOSLCompliantFinder     = 3,    /* Finder is scriptable and recordable */
  gestaltFinderSupports4GBVolumes = 4,  /* Finder correctly handles 4GB volumes */
  gestaltFinderHasClippings     = 6,    /* Finder supports Drag Manager clipping files */
  gestaltFinderFullDragManagerSupport = 7, /* Finder accepts 'hfs ' flavors properly */
  gestaltFinderFloppyRootComments = 8,  /* in MacOS 8 and later, will be set if Finder ever supports comments on Floppy icons */
  gestaltFinderLargeAndNotSavedFlavorsOK = 9, /* in MacOS 8 and later, this bit is set if drags with >1024-byte flavors and flavorNotSaved flavors work reliably */
  gestaltFinderUsesExtensibleFolderManager = 10, /* Finder uses Extensible Folder Manager (for example, for Magic Routing) */
  gestaltFinderUnderstandsRedirectedDesktopFolder = 11 /* Finder deals with startup disk's desktop folder residing on another disk */
};

enum {
  gestaltFindFolderAttr         = 'fold', /* Folder Mgr attributes */
  gestaltFindFolderPresent      = 0,    /* True if Folder Mgr present */
  gestaltFolderDescSupport      = 1,    /* True if Folder Mgr has FolderDesc calls */
  gestaltFolderMgrFollowsAliasesWhenResolving = 2, /* True if Folder Mgr follows folder aliases */
  gestaltFolderMgrSupportsExtendedCalls = 3, /* True if Folder Mgr supports the Extended calls */
  gestaltFolderMgrSupportsDomains = 4,  /* True if Folder Mgr supports domains for the first parameter to FindFolder */
  gestaltFolderMgrSupportsFSCalls = 5   /* True if FOlder manager supports __FindFolderFSRef & __FindFolderExtendedFSRef */
};

enum {
  gestaltFindFolderRedirectionAttr = 'fole'
};


enum {
  gestaltFontMgrAttr            = 'font', /* Font Mgr attributes */
  gestaltOutlineFonts           = 0     /* True if Outline Fonts supported */
};

enum {
  gestaltFPUType                = 'fpu ', /* fpu type */
  gestaltNoFPU                  = 0,    /* no FPU */
  gestalt68881                  = 1,    /* 68881 FPU */
  gestalt68882                  = 2,    /* 68882 FPU */
  gestalt68040FPU               = 3     /* 68040 built-in FPU */
};

enum {
  gestaltFSAttr                 = 'fs  ', /* file system attributes */
  gestaltFullExtFSDispatching   = 0,    /* has really cool new HFSDispatch dispatcher */
  gestaltHasFSSpecCalls         = 1,    /* has FSSpec calls */
  gestaltHasFileSystemManager   = 2,    /* has a file system manager */
  gestaltFSMDoesDynamicLoad     = 3,    /* file system manager supports dynamic loading */
  gestaltFSSupports4GBVols      = 4,    /* file system supports 4 gigabyte volumes */
  gestaltFSSupports2TBVols      = 5,    /* file system supports 2 terabyte volumes */
  gestaltHasExtendedDiskInit    = 6,    /* has extended Disk Initialization calls */
  gestaltDTMgrSupportsFSM       = 7,    /* Desktop Manager support FSM-based foreign file systems */
  gestaltFSNoMFSVols            = 8,    /* file system doesn't supports MFS volumes */
  gestaltFSSupportsHFSPlusVols  = 9,    /* file system supports HFS Plus volumes */
  gestaltFSIncompatibleDFA82    = 10    /* VCB and FCB structures changed; DFA 8.2 is incompatible */
};

enum {
  gestaltFSSupportsDirectIO     = 11    /* file system supports DirectIO */
};

enum {
  gestaltHasHFSPlusAPIs         = 12,   /* file system supports HFS Plus APIs */
  gestaltMustUseFCBAccessors    = 13,   /* FCBSPtr and FSFCBLen are invalid - must use FSM FCB accessor functions*/
  gestaltFSUsesPOSIXPathsForConversion = 14, /* The path interchange routines operate on POSIX paths instead of HFS paths */
  gestaltFSSupportsExclusiveLocks = 15, /* File system uses POSIX O_EXLOCK for opens */
  gestaltFSSupportsHardLinkDetection = 16, /* File system returns if an item is a hard link */
  gestaltFSAllowsConcurrentAsyncIO = 17 /* File Manager supports concurrent async reads and writes */
};

enum {
  gestaltAdminFeaturesFlagsAttr = 'fred', /* a set of admin flags, mostly useful internally. */
  gestaltFinderUsesSpecialOpenFoldersFile = 0 /* the Finder uses a special file to store the list of open folders */
};

enum {
  gestaltFSMVersion             = 'fsm ' /* returns version of HFS External File Systems Manager (FSM) */
};

enum {
  gestaltFXfrMgrAttr            = 'fxfr', /* file transfer manager attributes */
  gestaltFXfrMgrPresent         = 0,
  gestaltFXfrMgrMultiFile       = 1,    /* supports FTSend and FTReceive */
  gestaltFXfrMgrErrorString     = 2,    /* supports FTGetErrorString */
  gestaltFXfrMgrAsync           = 3     /*supports FTSendAsync, FTReceiveAsync, FTCompletionAsync*/
};

enum {
  gestaltGraphicsAttr           = 'gfxa', /* Quickdraw GX attributes selector */
  gestaltGraphicsIsDebugging    = 0x00000001,
  gestaltGraphicsIsLoaded       = 0x00000002,
  gestaltGraphicsIsPowerPC      = 0x00000004
};

enum {
  gestaltGraphicsVersion        = 'grfx', /* Quickdraw GX version selector */
  gestaltCurrentGraphicsVersion = 0x00010200 /* the version described in this set of headers */
};

enum {
  gestaltHardwareAttr           = 'hdwr', /* hardware attributes */
  gestaltHasVIA1                = 0,    /* VIA1 exists */
  gestaltHasVIA2                = 1,    /* VIA2 exists */
  gestaltHasASC                 = 3,    /* Apple Sound Chip exists */
  gestaltHasSCC                 = 4,    /* SCC exists */
  gestaltHasSCSI                = 7,    /* SCSI exists */
  gestaltHasSoftPowerOff        = 19,   /* Capable of software power off */
  gestaltHasSCSI961             = 21,   /* 53C96 SCSI controller on internal bus */
  gestaltHasSCSI962             = 22,   /* 53C96 SCSI controller on external bus */
  gestaltHasUniversalROM        = 24,   /* Do we have a Universal ROM? */
  gestaltHasEnhancedLtalk       = 30    /* Do we have Enhanced LocalTalk? */
};

enum {
  gestaltHelpMgrAttr            = 'help', /* Help Mgr Attributes */
  gestaltHelpMgrPresent         = 0,    /* true if help mgr is present */
  gestaltHelpMgrExtensions      = 1,    /* true if help mgr extensions are installed */
  gestaltAppleGuideIsDebug      = 30,
  gestaltAppleGuidePresent      = 31    /* true if AppleGuide is installed */
};

enum {
  gestaltHardwareVendorCode     = 'hrad', /* Returns hardware vendor information */
  gestaltHardwareVendorApple    = 'Appl' /* Hardware built by Apple */
};

enum {
  gestaltCompressionMgr         = 'icmp' /* returns version of the Image Compression Manager */
};

enum {
  gestaltIconUtilitiesAttr      = 'icon', /* Icon Utilities attributes  (Note: available in System 7.0, despite gestalt) */
  gestaltIconUtilitiesPresent   = 0,    /* true if icon utilities are present */
  gestaltIconUtilitiesHas48PixelIcons = 1, /* true if 48x48 icons are supported by IconUtilities */
  gestaltIconUtilitiesHas32BitIcons = 2, /* true if 32-bit deep icons are supported */
  gestaltIconUtilitiesHas8BitDeepMasks = 3, /* true if 8-bit deep masks are supported */
  gestaltIconUtilitiesHasIconServices = 4 /* true if IconServices is present */
};

enum {
  gestaltInternalDisplay        = 'idsp' /* slot number of internal display location */
};

/*
    To obtain information about the connected keyboard(s), one should
    use the ADB Manager API.  See Technical Note OV16 for details.
*/
enum {
  gestaltKeyboardType           = 'kbd ', /* keyboard type */
  gestaltMacKbd                 = 1,
  gestaltMacAndPad              = 2,
  gestaltMacPlusKbd             = 3,    /* OBSOLETE: This pre-ADB keyboard is not supported by any Mac OS X hardware and this value now means gestaltUnknownThirdPartyKbd */
  gestaltUnknownThirdPartyKbd   = 3,    /* Unknown 3rd party keyboard. */
  gestaltExtADBKbd              = 4,
  gestaltStdADBKbd              = 5,
  gestaltPrtblADBKbd            = 6,
  gestaltPrtblISOKbd            = 7,
  gestaltStdISOADBKbd           = 8,
  gestaltExtISOADBKbd           = 9,
  gestaltADBKbdII               = 10,
  gestaltADBISOKbdII            = 11,
  gestaltPwrBookADBKbd          = 12,
  gestaltPwrBookISOADBKbd       = 13,
  gestaltAppleAdjustKeypad      = 14,
  gestaltAppleAdjustADBKbd      = 15,
  gestaltAppleAdjustISOKbd      = 16,
  gestaltJapanAdjustADBKbd      = 17,   /* Japan Adjustable Keyboard */
  gestaltPwrBkExtISOKbd         = 20,   /* PowerBook Extended International Keyboard with function keys */
  gestaltPwrBkExtJISKbd         = 21,   /* PowerBook Extended Japanese Keyboard with function keys      */
  gestaltPwrBkExtADBKbd         = 24,   /* PowerBook Extended Domestic Keyboard with function keys      */
  gestaltPS2Keyboard            = 27,   /* PS2 keyboard */
  gestaltPwrBkSubDomKbd         = 28,   /* PowerBook Subnote Domestic Keyboard with function keys w/  inverted T  */
  gestaltPwrBkSubISOKbd         = 29,   /* PowerBook Subnote International Keyboard with function keys w/  inverted T     */
  gestaltPwrBkSubJISKbd         = 30,   /* PowerBook Subnote Japanese Keyboard with function keys w/ inverted T    */
  gestaltPortableUSBANSIKbd     = 37,   /* Powerbook USB-based internal keyboard, ANSI layout */
  gestaltPortableUSBISOKbd      = 38,   /* Powerbook USB-based internal keyboard, ISO layout */
  gestaltPortableUSBJISKbd      = 39,   /* Powerbook USB-based internal keyboard, JIS layout */
  gestaltThirdPartyANSIKbd      = 40,   /* Third party keyboard, ANSI layout.  Returned in Mac OS X Tiger and later. */
  gestaltThirdPartyISOKbd       = 41,   /* Third party keyboard, ISO layout. Returned in Mac OS X Tiger and later. */
  gestaltThirdPartyJISKbd       = 42,   /* Third party keyboard, JIS layout. Returned in Mac OS X Tiger and later. */
  gestaltPwrBkEKDomKbd          = 195,  /* (0xC3) PowerBook Domestic Keyboard with Embedded Keypad, function keys & inverted T    */
  gestaltPwrBkEKISOKbd          = 196,  /* (0xC4) PowerBook International Keyboard with Embedded Keypad, function keys & inverted T   */
  gestaltPwrBkEKJISKbd          = 197,  /* (0xC5) PowerBook Japanese Keyboard with Embedded Keypad, function keys & inverted T      */
  gestaltUSBCosmoANSIKbd        = 198,  /* (0xC6) original USB Domestic (ANSI) Keyboard */
  gestaltUSBCosmoISOKbd         = 199,  /* (0xC7) original USB International (ISO) Keyboard */
  gestaltUSBCosmoJISKbd         = 200,  /* (0xC8) original USB Japanese (JIS) Keyboard */
  gestaltPwrBk99JISKbd          = 201,  /* (0xC9) '99 PowerBook JIS Keyboard with Embedded Keypad, function keys & inverted T               */
  gestaltUSBAndyANSIKbd         = 204,  /* (0xCC) USB Pro Keyboard Domestic (ANSI) Keyboard                                 */
  gestaltUSBAndyISOKbd          = 205,  /* (0xCD) USB Pro Keyboard International (ISO) Keyboard                               */
  gestaltUSBAndyJISKbd          = 206   /* (0xCE) USB Pro Keyboard Japanese (JIS) Keyboard                                    */
};


enum {
  gestaltPortable2001ANSIKbd    = 202,  /* (0xCA) PowerBook and iBook Domestic (ANSI) Keyboard with 2nd cmd key right & function key moves.     */
  gestaltPortable2001ISOKbd     = 203,  /* (0xCB) PowerBook and iBook International (ISO) Keyboard with 2nd cmd key right & function key moves.   */
  gestaltPortable2001JISKbd     = 207   /* (0xCF) PowerBook and iBook Japanese (JIS) Keyboard with function key moves.                   */
};

enum {
  gestaltUSBProF16ANSIKbd       = 34,   /* (0x22) USB Pro Keyboard w/ F16 key Domestic (ANSI) Keyboard */
  gestaltUSBProF16ISOKbd        = 35,   /* (0x23) USB Pro Keyboard w/ F16 key International (ISO) Keyboard */
  gestaltUSBProF16JISKbd        = 36,   /* (0x24) USB Pro Keyboard w/ F16 key Japanese (JIS) Keyboard */
  gestaltProF16ANSIKbd          = 31,   /* (0x1F) Pro Keyboard w/F16 key Domestic (ANSI) Keyboard */
  gestaltProF16ISOKbd           = 32,   /* (0x20) Pro Keyboard w/F16 key International (ISO) Keyboard */
  gestaltProF16JISKbd           = 33    /* (0x21) Pro Keyboard w/F16 key Japanese (JIS) Keyboard */
};

/*
    This gestalt indicates the highest UDF version that the active UDF implementation supports.
    The value should be assembled from a read version (upper word) and a write version (lower word)
*/
enum {
  gestaltUDFSupport             = 'kudf' /*    Used for communication between UDF implementations*/
};

enum {
  gestaltLowMemorySize          = 'lmem' /* size of low memory area */
};

enum {
  gestaltLogicalRAMSize         = 'lram' /* logical ram size */
};

/*
    MACHINE TYPE CONSTANTS NAMING CONVENTION

        All future machine type constant names take the following form:

            gestalt<lineName><modelNumber>

    Line Names

        The following table contains the lines currently produced by Apple and the
        lineName substrings associated with them:

            Line                        lineName
            -------------------------   ------------
            Macintosh LC                "MacLC"
            Macintosh Performa          "Performa"
            Macintosh PowerBook         "PowerBook"
            Macintosh PowerBook Duo     "PowerBookDuo"
            Power Macintosh             "PowerMac"
            Apple Workgroup Server      "AWS"

        The following table contains lineNames for some discontinued lines:

            Line                        lineName
            -------------------------   ------------
            Macintosh Quadra            "MacQuadra" (preferred)
                                        "Quadra" (also used, but not preferred)
            Macintosh Centris           "MacCentris"

    Model Numbers

        The modelNumber is a string representing the specific model of the machine
        within its particular line.  For example, for the Power Macintosh 8100/80,
        the modelNumber is "8100".

        Some Performa & LC model numbers contain variations in the rightmost 1 or 2
        digits to indicate different RAM and Hard Disk configurations.  A single
        machine type is assigned for all variations of a specific model number.  In
        this case, the modelNumber string consists of the constant leftmost part
        of the model number with 0s for the variant digits.  For example, the
        Performa 6115 and Performa 6116 are both return the same machine type
        constant:  gestaltPerforma6100.


    OLD NAMING CONVENTIONS

    The "Underscore Speed" suffix

        In the past, Apple differentiated between machines that had the same model
        number but different speeds.  For example, the Power Macintosh 8100/80 and
        Power Macintosh 8100/100 return different machine type constants.  This is
        why some existing machine type constant names take the form:

            gestalt<lineName><modelNumber>_<speed>

        e.g.

            gestaltPowerMac8100_110
            gestaltPowerMac7100_80
            gestaltPowerMac7100_66

        It is no longer necessary to use the "underscore speed" suffix.  Starting with
        the Power Surge machines (Power Macintosh 7200, 7500, 8500 and 9500), speed is
        no longer used to differentiate between machine types.  This is why a Power
        Macintosh 7200/75 and a Power Macintosh 7200/90 return the same machine type
        constant:  gestaltPowerMac7200.

    The "Screen Type" suffix

        All PowerBook models prior to the PowerBook 190, and all PowerBook Duo models
        before the PowerBook Duo 2300 take the form:

            gestalt<lineName><modelNumber><screenType>

        Where <screenType> is "c" or the empty string.

        e.g.

            gestaltPowerBook100
            gestaltPowerBookDuo280
            gestaltPowerBookDuo280c
            gestaltPowerBook180
            gestaltPowerBook180c

        Starting with the PowerBook 190 series and the PowerBook Duo 2300 series, machine
        types are no longer differentiated based on screen type.  This is why a PowerBook
        5300cs/100 and a PowerBook 5300c/100 both return the same machine type constant:
        gestaltPowerBook5300.

        Macintosh LC 630                gestaltMacLC630
        Macintosh Performa 6200         gestaltPerforma6200
        Macintosh Quadra 700            gestaltQuadra700
        Macintosh PowerBook 5300        gestaltPowerBook5300
        Macintosh PowerBook Duo 2300    gestaltPowerBookDuo2300
        Power Macintosh 8500            gestaltPowerMac8500
*/

enum {
  gestaltMachineType            = 'mach', /* machine type */
  gestaltClassic                = 1,
  gestaltMacXL                  = 2,
  gestaltMac512KE               = 3,
  gestaltMacPlus                = 4,
  gestaltMacSE                  = 5,
  gestaltMacII                  = 6,
  gestaltMacIIx                 = 7,
  gestaltMacIIcx                = 8,
  gestaltMacSE030               = 9,
  gestaltPortable               = 10,
  gestaltMacIIci                = 11,
  gestaltPowerMac8100_120       = 12,
  gestaltMacIIfx                = 13,
  gestaltMacClassic             = 17,
  gestaltMacIIsi                = 18,
  gestaltMacLC                  = 19,
  gestaltMacQuadra900           = 20,
  gestaltPowerBook170           = 21,
  gestaltMacQuadra700           = 22,
  gestaltClassicII              = 23,
  gestaltPowerBook100           = 24,
  gestaltPowerBook140           = 25,
  gestaltMacQuadra950           = 26,
  gestaltMacLCIII               = 27,
  gestaltPerforma450            = gestaltMacLCIII,
  gestaltPowerBookDuo210        = 29,
  gestaltMacCentris650          = 30,
  gestaltPowerBookDuo230        = 32,
  gestaltPowerBook180           = 33,
  gestaltPowerBook160           = 34,
  gestaltMacQuadra800           = 35,
  gestaltMacQuadra650           = 36,
  gestaltMacLCII                = 37,
  gestaltPowerBookDuo250        = 38,
  gestaltAWS9150_80             = 39,
  gestaltPowerMac8100_110       = 40,
  gestaltAWS8150_110            = gestaltPowerMac8100_110,
  gestaltPowerMac5200           = 41,
  gestaltPowerMac5260           = gestaltPowerMac5200,
  gestaltPerforma5300           = gestaltPowerMac5200,
  gestaltPowerMac6200           = 42,
  gestaltPerforma6300           = gestaltPowerMac6200,
  gestaltMacIIvi                = 44,
  gestaltMacIIvm                = 45,
  gestaltPerforma600            = gestaltMacIIvm,
  gestaltPowerMac7100_80        = 47,
  gestaltMacIIvx                = 48,
  gestaltMacColorClassic        = 49,
  gestaltPerforma250            = gestaltMacColorClassic,
  gestaltPowerBook165c          = 50,
  gestaltMacCentris610          = 52,
  gestaltMacQuadra610           = 53,
  gestaltPowerBook145           = 54,
  gestaltPowerMac8100_100       = 55,
  gestaltMacLC520               = 56,
  gestaltAWS9150_120            = 57,
  gestaltPowerMac6400           = 58,
  gestaltPerforma6400           = gestaltPowerMac6400,
  gestaltPerforma6360           = gestaltPerforma6400,
  gestaltMacCentris660AV        = 60,
  gestaltMacQuadra660AV         = gestaltMacCentris660AV,
  gestaltPerforma46x            = 62,
  gestaltPowerMac8100_80        = 65,
  gestaltAWS8150_80             = gestaltPowerMac8100_80,
  gestaltPowerMac9500           = 67,
  gestaltPowerMac9600           = gestaltPowerMac9500,
  gestaltPowerMac7500           = 68,
  gestaltPowerMac7600           = gestaltPowerMac7500,
  gestaltPowerMac8500           = 69,
  gestaltPowerMac8600           = gestaltPowerMac8500,
  gestaltAWS8550                = gestaltPowerMac7500,
  gestaltPowerBook180c          = 71,
  gestaltPowerBook520           = 72,
  gestaltPowerBook520c          = gestaltPowerBook520,
  gestaltPowerBook540           = gestaltPowerBook520,
  gestaltPowerBook540c          = gestaltPowerBook520,
  gestaltPowerMac5400           = 74,
  gestaltPowerMac6100_60        = 75,
  gestaltAWS6150_60             = gestaltPowerMac6100_60,
  gestaltPowerBookDuo270c       = 77,
  gestaltMacQuadra840AV         = 78,
  gestaltPerforma550            = 80,
  gestaltPowerBook165           = 84,
  gestaltPowerBook190           = 85,
  gestaltMacTV                  = 88,
  gestaltMacLC475               = 89,
  gestaltPerforma47x            = gestaltMacLC475,
  gestaltMacLC575               = 92,
  gestaltMacQuadra605           = 94,
  gestaltMacQuadra630           = 98,
  gestaltMacLC580               = 99,
  gestaltPerforma580            = gestaltMacLC580,
  gestaltPowerMac6100_66        = 100,
  gestaltAWS6150_66             = gestaltPowerMac6100_66,
  gestaltPowerBookDuo280        = 102,
  gestaltPowerBookDuo280c       = 103,
  gestaltPowerMacLC475          = 104,  /* Mac LC 475 & PPC Processor Upgrade Card*/
  gestaltPowerMacPerforma47x    = gestaltPowerMacLC475,
  gestaltPowerMacLC575          = 105,  /* Mac LC 575 & PPC Processor Upgrade Card */
  gestaltPowerMacPerforma57x    = gestaltPowerMacLC575,
  gestaltPowerMacQuadra630      = 106,  /* Quadra 630 & PPC Processor Upgrade Card*/
  gestaltPowerMacLC630          = gestaltPowerMacQuadra630, /* Mac LC 630 & PPC Processor Upgrade Card*/
  gestaltPowerMacPerforma63x    = gestaltPowerMacQuadra630, /* Performa 63x & PPC Processor Upgrade Card*/
  gestaltPowerMac7200           = 108,
  gestaltPowerMac7300           = 109,
  gestaltPowerMac7100_66        = 112,
  gestaltPowerBook150           = 115,
  gestaltPowerMacQuadra700      = 116,  /* Quadra 700 & Power PC Upgrade Card*/
  gestaltPowerMacQuadra900      = 117,  /* Quadra 900 & Power PC Upgrade Card */
  gestaltPowerMacQuadra950      = 118,  /* Quadra 950 & Power PC Upgrade Card */
  gestaltPowerMacCentris610     = 119,  /* Centris 610 & Power PC Upgrade Card */
  gestaltPowerMacCentris650     = 120,  /* Centris 650 & Power PC Upgrade Card */
  gestaltPowerMacQuadra610      = 121,  /* Quadra 610 & Power PC Upgrade Card */
  gestaltPowerMacQuadra650      = 122,  /* Quadra 650 & Power PC Upgrade Card */
  gestaltPowerMacQuadra800      = 123,  /* Quadra 800 & Power PC Upgrade Card */
  gestaltPowerBookDuo2300       = 124,
  gestaltPowerBook500PPCUpgrade = 126,
  gestaltPowerBook5300          = 128,
  gestaltPowerBook1400          = 310,
  gestaltPowerBook3400          = 306,
  gestaltPowerBook2400          = 307,
  gestaltPowerBookG3Series      = 312,
  gestaltPowerBookG3            = 313,
  gestaltPowerBookG3Series2     = 314,
  gestaltPowerMacNewWorld       = 406,  /* All NewWorld architecture Macs (iMac, blue G3, etc.)*/
  gestaltPowerMacG3             = 510,
  gestaltPowerMac5500           = 512,
  gestalt20thAnniversary        = gestaltPowerMac5500,
  gestaltPowerMac6500           = 513,
  gestaltPowerMac4400_160       = 514,  /* slower machine has different machine ID*/
  gestaltPowerMac4400           = 515,
  gestaltMacOSCompatibility     = 1206  /*    Mac OS Compatibility on Mac OS X (Classic)*/
};


enum {
  gestaltQuadra605              = gestaltMacQuadra605,
  gestaltQuadra610              = gestaltMacQuadra610,
  gestaltQuadra630              = gestaltMacQuadra630,
  gestaltQuadra650              = gestaltMacQuadra650,
  gestaltQuadra660AV            = gestaltMacQuadra660AV,
  gestaltQuadra700              = gestaltMacQuadra700,
  gestaltQuadra800              = gestaltMacQuadra800,
  gestaltQuadra840AV            = gestaltMacQuadra840AV,
  gestaltQuadra900              = gestaltMacQuadra900,
  gestaltQuadra950              = gestaltMacQuadra950
};

enum {
  kMachineNameStrID             = -16395
};

enum {
  gestaltSMPMailerVersion       = 'malr' /* OCE StandardMail*/
};

enum {
  gestaltMediaBay               = 'mbeh', /* media bay driver type */
  gestaltMBLegacy               = 0,    /* media bay support in PCCard 2.0 */
  gestaltMBSingleBay            = 1,    /* single bay media bay driver */
  gestaltMBMultipleBays         = 2     /* multi-bay media bay driver */
};

enum {
  gestaltMessageMgrVersion      = 'mess' /* GX Printing Message Manager Gestalt Selector */
};


/*  Menu Manager Gestalt (Mac OS 8.5 and later)*/
enum {
  gestaltMenuMgrAttr            = 'menu', /* If this Gestalt exists, the Mac OS 8.5 Menu Manager is installed */
  gestaltMenuMgrPresent         = (1L << 0), /* NOTE: this is a bit mask, whereas all other Gestalt constants of this nature */
                                        /* are bit index values. 3.2 interfaces slipped out with this mistake unnoticed. */
                                        /* Sincere apologies for any inconvenience.*/
  gestaltMenuMgrPresentBit      = 0,    /* bit number */
  gestaltMenuMgrAquaLayoutBit   = 1,    /* menus have the Aqua 1.0 layout*/
  gestaltMenuMgrMultipleItemsWithCommandIDBit = 2, /* CountMenuItemsWithCommandID/GetIndMenuItemWithCommandID support multiple items with the same command ID*/
  gestaltMenuMgrRetainsIconRefBit = 3,  /* SetMenuItemIconHandle, when passed an IconRef, calls AcquireIconRef*/
  gestaltMenuMgrSendsMenuBoundsToDefProcBit = 4, /* kMenuSizeMsg and kMenuPopUpMsg have menu bounding rect information*/
  gestaltMenuMgrMoreThanFiveMenusDeepBit = 5, /* the Menu Manager supports hierarchical menus more than five deep*/
  gestaltMenuMgrCGImageMenuTitleBit = 6, /* SetMenuTitleIcon supports CGImageRefs*/
                                        /* masks for the above bits*/
  gestaltMenuMgrPresentMask     = (1L << gestaltMenuMgrPresentBit),
  gestaltMenuMgrAquaLayoutMask  = (1L << gestaltMenuMgrAquaLayoutBit),
  gestaltMenuMgrMultipleItemsWithCommandIDMask = (1L << gestaltMenuMgrMultipleItemsWithCommandIDBit),
  gestaltMenuMgrRetainsIconRefMask = (1L << gestaltMenuMgrRetainsIconRefBit),
  gestaltMenuMgrSendsMenuBoundsToDefProcMask = (1L << gestaltMenuMgrSendsMenuBoundsToDefProcBit),
  gestaltMenuMgrMoreThanFiveMenusDeepMask = (1L << gestaltMenuMgrMoreThanFiveMenusDeepBit),
  gestaltMenuMgrCGImageMenuTitleMask = (1L << gestaltMenuMgrCGImageMenuTitleBit)
};


enum {
  gestaltMultipleUsersState     = 'mfdr' /* Gestalt selector returns MultiUserGestaltHandle (in Folders.h)*/
};


enum {
  gestaltMachineIcon            = 'micn' /* machine icon */
};

enum {
  gestaltMiscAttr               = 'misc', /* miscellaneous attributes */
  gestaltScrollingThrottle      = 0,    /* true if scrolling throttle on */
  gestaltSquareMenuBar          = 2     /* true if menu bar is square */
};


/*
    The name gestaltMixedModeVersion for the 'mixd' selector is semantically incorrect.
    The same selector has been renamed gestaltMixedModeAttr to properly reflect the
    Inside Mac: PowerPC System Software documentation.  The gestaltMixedModeVersion
    symbol has been preserved only for backwards compatibility.

    Developers are forewarned that gestaltMixedModeVersion has a limited lifespan and
    will be removed in a future release of the Interfaces.

    For the first version of Mixed Mode, both meanings of the 'mixd' selector are
    functionally identical.  They both return 0x00000001.  In subsequent versions
    of Mixed Mode, however, the 'mixd' selector will not respond with an increasing
    version number, but rather, with 32 attribute bits with various meanings.
*/
enum {
  gestaltMixedModeVersion       = 'mixd' /* returns version of Mixed Mode */
};

enum {
  gestaltMixedModeAttr          = 'mixd', /* returns Mixed Mode attributes */
  gestaltMixedModePowerPC       = 0,    /* true if Mixed Mode supports PowerPC ABI calling conventions */
  gestaltPowerPCAware           = 0,    /* old name for gestaltMixedModePowerPC */
  gestaltMixedModeCFM68K        = 1,    /* true if Mixed Mode supports CFM-68K calling conventions */
  gestaltMixedModeCFM68KHasTrap = 2,    /* true if CFM-68K Mixed Mode implements _MixedModeDispatch (versions 1.0.1 and prior did not) */
  gestaltMixedModeCFM68KHasState = 3    /* true if CFM-68K Mixed Mode exports Save/RestoreMixedModeState */
};

enum {
  gestaltQuickTimeConferencing  = 'mtlk' /* returns QuickTime Conferencing version */
};

enum {
  gestaltMemoryMapAttr          = 'mmap', /* Memory map type */
  gestaltMemoryMapSparse        = 0     /* Sparse memory is on */
};

enum {
  gestaltMMUType                = 'mmu ', /* mmu type */
  gestaltNoMMU                  = 0,    /* no MMU */
  gestaltAMU                    = 1,    /* address management unit */
  gestalt68851                  = 2,    /* 68851 PMMU */
  gestalt68030MMU               = 3,    /* 68030 built-in MMU */
  gestalt68040MMU               = 4,    /* 68040 built-in MMU */
  gestaltEMMU1                  = 5     /* Emulated MMU type 1  */
};

enum {
                                        /*    On Mac OS X, the user visible machine name may something like "PowerMac3,4", which is*/
                                        /*    a unique string for each signifigant Macintosh computer which Apple creates, but is*/
                                        /*    not terribly useful as a user visible string.*/
  gestaltUserVisibleMachineName = 'mnam' /* Coerce response into a StringPtr to get a user visible machine name */
};

enum {
  gestaltMPCallableAPIsAttr     = 'mpsc', /* Bitmap of toolbox/OS managers that can be called from MPLibrary MPTasks */
  gestaltMPFileManager          = 0,    /* True if File Manager calls can be made from MPTasks */
  gestaltMPDeviceManager        = 1,    /* True if synchronous Device Manager calls can be made from MPTasks */
  gestaltMPTrapCalls            = 2     /* True if most trap-based calls can be made from MPTasks */
};

enum {
  gestaltStdNBPAttr             = 'nlup', /* standard nbp attributes */
  gestaltStdNBPPresent          = 0,
  gestaltStdNBPSupportsAutoPosition = 1 /* StandardNBP takes (-1,-1) to mean alert position main screen */
};

enum {
  gestaltNotificationMgrAttr    = 'nmgr', /* notification manager attributes */
  gestaltNotificationPresent    = 0     /* notification manager exists */
};

enum {
  gestaltNameRegistryVersion    = 'nreg' /* NameRegistryLib version number, for System 7.5.2+ usage */
};

enum {
  gestaltNuBusSlotCount         = 'nubs' /* count of logical NuBus slots present */
};

enum {
  gestaltOCEToolboxVersion      = 'ocet', /* OCE Toolbox version */
  gestaltOCETB                  = 0x0102, /* OCE Toolbox version 1.02 */
  gestaltSFServer               = 0x0100 /* S&F Server version 1.0 */
};

enum {
  gestaltOCEToolboxAttr         = 'oceu', /* OCE Toolbox attributes */
  gestaltOCETBPresent           = 0x01, /* OCE toolbox is present, not running */
  gestaltOCETBAvailable         = 0x02, /* OCE toolbox is running and available */
  gestaltOCESFServerAvailable   = 0x04, /* S&F Server is running and available */
  gestaltOCETBNativeGlueAvailable = 0x10 /* Native PowerPC Glue routines are availible */
};

enum {
  gestaltOpenFirmwareInfo       = 'opfw' /* Open Firmware info */
};

enum {
  gestaltOSAttr                 = 'os  ', /* o/s attributes */
  gestaltSysZoneGrowable        = 0,    /* system heap is growable */
  gestaltLaunchCanReturn        = 1,    /* can return from launch */
  gestaltLaunchFullFileSpec     = 2,    /* can launch from full file spec */
  gestaltLaunchControl          = 3,    /* launch control support available */
  gestaltTempMemSupport         = 4,    /* temp memory support */
  gestaltRealTempMemory         = 5,    /* temp memory handles are real */
  gestaltTempMemTracked         = 6,    /* temporary memory handles are tracked */
  gestaltIPCSupport             = 7,    /* IPC support is present */
  gestaltSysDebuggerSupport     = 8,    /* system debugger support is present */
  gestaltNativeProcessMgrBit    = 19,   /* the process manager itself is native */
  gestaltAltivecRegistersSwappedCorrectlyBit = 20 /* Altivec registers are saved correctly on process switches */
};

enum {
  gestaltOSTable                = 'ostt' /*  OS trap table base  */
};


/*******************************************************************************
*   Gestalt Selectors for Open Transport Network Setup
*
*   Note: possible values for the version "stage" byte are:
*   development = 0x20, alpha = 0x40, beta = 0x60, final & release = 0x80
********************************************************************************/
enum {
  gestaltOpenTptNetworkSetup    = 'otcf',
  gestaltOpenTptNetworkSetupLegacyImport = 0,
  gestaltOpenTptNetworkSetupLegacyExport = 1,
  gestaltOpenTptNetworkSetupSupportsMultihoming = 2
};

enum {
  gestaltOpenTptNetworkSetupVersion = 'otcv'
};

/*******************************************************************************
*   Gestalt Selectors for Open Transport-based Remote Access/PPP
*
*   Note: possible values for the version "stage" byte are:
*   development = 0x20, alpha = 0x40, beta = 0x60, final & release = 0x80
********************************************************************************/
enum {
  gestaltOpenTptRemoteAccess    = 'otra',
  gestaltOpenTptRemoteAccessPresent = 0,
  gestaltOpenTptRemoteAccessLoaded = 1,
  gestaltOpenTptRemoteAccessClientOnly = 2,
  gestaltOpenTptRemoteAccessPServer = 3,
  gestaltOpenTptRemoteAccessMPServer = 4,
  gestaltOpenTptPPPPresent      = 5,
  gestaltOpenTptARAPPresent     = 6
};

enum {
  gestaltOpenTptRemoteAccessVersion = 'otrv'
};


/* ***** Open Transport Gestalt ******/


enum {
  gestaltOpenTptVersions        = 'otvr' /* Defined by OT 1.1 and higher, response is NumVersion.*/
};

enum {
  gestaltOpenTpt                = 'otan', /* Defined by all versions, response is defined below.*/
  gestaltOpenTptPresentMask     = 0x00000001,
  gestaltOpenTptLoadedMask      = 0x00000002,
  gestaltOpenTptAppleTalkPresentMask = 0x00000004,
  gestaltOpenTptAppleTalkLoadedMask = 0x00000008,
  gestaltOpenTptTCPPresentMask  = 0x00000010,
  gestaltOpenTptTCPLoadedMask   = 0x00000020,
  gestaltOpenTptIPXSPXPresentMask = 0x00000040,
  gestaltOpenTptIPXSPXLoadedMask = 0x00000080,
  gestaltOpenTptPresentBit      = 0,
  gestaltOpenTptLoadedBit       = 1,
  gestaltOpenTptAppleTalkPresentBit = 2,
  gestaltOpenTptAppleTalkLoadedBit = 3,
  gestaltOpenTptTCPPresentBit   = 4,
  gestaltOpenTptTCPLoadedBit    = 5,
  gestaltOpenTptIPXSPXPresentBit = 6,
  gestaltOpenTptIPXSPXLoadedBit = 7
};


enum {
  gestaltPCCard                 = 'pccd', /*    PC Card attributes*/
  gestaltCardServicesPresent    = 0,    /*    PC Card 2.0 (68K) API is present*/
  gestaltPCCardFamilyPresent    = 1,    /*    PC Card 3.x (PowerPC) API is present*/
  gestaltPCCardHasPowerControl  = 2,    /*    PCCardSetPowerLevel is supported*/
  gestaltPCCardSupportsCardBus  = 3     /*    CardBus is supported*/
};

enum {
  gestaltProcClkSpeed           = 'pclk' /* processor clock speed in hertz (a UInt32) */
};

enum {
  gestaltProcClkSpeedMHz        = 'mclk' /* processor clock speed in megahertz (a UInt32) */
};

enum {
  gestaltPCXAttr                = 'pcxg', /* PC Exchange attributes */
  gestaltPCXHas8and16BitFAT     = 0,    /* PC Exchange supports both 8 and 16 bit FATs */
  gestaltPCXHasProDOS           = 1,    /* PC Exchange supports ProDOS */
  gestaltPCXNewUI               = 2,
  gestaltPCXUseICMapping        = 3     /* PC Exchange uses InternetConfig for file mappings */
};

enum {
  gestaltLogicalPageSize        = 'pgsz' /* logical page size */
};

/*    System 7.6 and later.  If gestaltScreenCaptureMain is not implemented,
    PictWhap proceeds with screen capture in the usual way.

    The high word of gestaltScreenCaptureMain is reserved (use 0).

    To disable screen capture to disk, put zero in the low word.  To
    specify a folder for captured pictures, put the vRefNum in the
    low word of gestaltScreenCaptureMain, and put the directory ID in
    gestaltScreenCaptureDir.
*/
enum {
  gestaltScreenCaptureMain      = 'pic1', /* Zero, or vRefNum of disk to hold picture */
  gestaltScreenCaptureDir       = 'pic2' /* Directory ID of folder to hold picture */
};

enum {
  gestaltGXPrintingMgrVersion   = 'pmgr' /* QuickDraw GX Printing Manager Version*/
};

enum {
  gestaltPopupAttr              = 'pop!', /* popup cdef attributes */
  gestaltPopupPresent           = 0
};

enum {
  gestaltPowerMgrAttr           = 'powr', /* power manager attributes */
  gestaltPMgrExists             = 0,
  gestaltPMgrCPUIdle            = 1,
  gestaltPMgrSCC                = 2,
  gestaltPMgrSound              = 3,
  gestaltPMgrDispatchExists     = 4,
  gestaltPMgrSupportsAVPowerStateAtSleepWake = 5
};

enum {
  gestaltPowerMgrVers           = 'pwrv' /* power manager version */
};

/*
 * PPC will return the combination of following bit fields.
 * e.g. gestaltPPCSupportsRealTime +gestaltPPCSupportsIncoming + gestaltPPCSupportsOutGoing
 * indicates PPC is cuurently is only supports real time delivery
 * and both incoming and outgoing network sessions are allowed.
 * By default local real time delivery is supported as long as PPCInit has been called.*/
enum {
  gestaltPPCToolboxAttr         = 'ppc ', /* PPC toolbox attributes */
  gestaltPPCToolboxPresent      = 0x0000, /* PPC Toolbox is present  Requires PPCInit to be called */
  gestaltPPCSupportsRealTime    = 0x1000, /* PPC Supports real-time delivery */
  gestaltPPCSupportsIncoming    = 0x0001, /* PPC will allow incoming network requests */
  gestaltPPCSupportsOutGoing    = 0x0002, /* PPC will allow outgoing network requests */
  gestaltPPCSupportsTCP_IP      = 0x0004, /* PPC supports TCP/IP transport  */
  gestaltPPCSupportsIncomingAppleTalk = 0x0010,
  gestaltPPCSupportsIncomingTCP_IP = 0x0020,
  gestaltPPCSupportsOutgoingAppleTalk = 0x0100,
  gestaltPPCSupportsOutgoingTCP_IP = 0x0200
};

/*
    Programs which need to know information about particular features of the processor should
    migrate to using sysctl() and sysctlbyname() to get this kind of information.  No new
    information will be added to the 'ppcf' selector going forward.
*/
enum {
  gestaltPowerPCProcessorFeatures = 'ppcf', /* Optional PowerPC processor features */
  gestaltPowerPCHasGraphicsInstructions = 0, /* has fres, frsqrte, and fsel instructions */
  gestaltPowerPCHasSTFIWXInstruction = 1, /* has stfiwx instruction */
  gestaltPowerPCHasSquareRootInstructions = 2, /* has fsqrt and fsqrts instructions */
  gestaltPowerPCHasDCBAInstruction = 3, /* has dcba instruction */
  gestaltPowerPCHasVectorInstructions = 4, /* has vector instructions */
  gestaltPowerPCHasDataStreams  = 5,    /* has dst, dstt, dstst, dss, and dssall instructions */
  gestaltPowerPCHas64BitSupport = 6,    /* double word LSU/ALU, etc. */
  gestaltPowerPCHasDCBTStreams  = 7,    /* TH field of DCBT recognized */
  gestaltPowerPCASArchitecture  = 8,    /* chip uses new 'A/S' architecture */
  gestaltPowerPCIgnoresDCBST    = 9     /* */
};

enum {
  gestaltProcessorType          = 'proc', /* processor type */
  gestalt68000                  = 1,
  gestalt68010                  = 2,
  gestalt68020                  = 3,
  gestalt68030                  = 4,
  gestalt68040                  = 5
};

enum {
  gestaltSDPPromptVersion       = 'prpv' /* OCE Standard Directory Panel*/
};

enum {
  gestaltParityAttr             = 'prty', /* parity attributes */
  gestaltHasParityCapability    = 0,    /* has ability to check parity */
  gestaltParityEnabled          = 1     /* parity checking enabled */
};

enum {
  gestaltQD3DVersion            = 'q3v ' /* Quickdraw 3D version in pack BCD*/
};

enum {
  gestaltQD3DViewer             = 'q3vc', /* Quickdraw 3D viewer attributes*/
  gestaltQD3DViewerPresent      = 0     /* bit 0 set if QD3D Viewer is available*/
};

#if OLDROUTINENAMES
enum {
  gestaltQD3DViewerNotPresent   = (0 << gestaltQD3DViewerPresent),
  gestaltQD3DViewerAvailable    = (1 << gestaltQD3DViewerPresent)
};

#endif  /* OLDROUTINENAMES */

enum {
  gestaltQuickdrawVersion       = 'qd  ', /* quickdraw version */
  gestaltOriginalQD             = 0x0000, /* original 1-bit QD */
  gestalt8BitQD                 = 0x0100, /* 8-bit color QD */
  gestalt32BitQD                = 0x0200, /* 32-bit color QD */
  gestalt32BitQD11              = 0x0201, /* 32-bit color QDv1.1 */
  gestalt32BitQD12              = 0x0220, /* 32-bit color QDv1.2 */
  gestalt32BitQD13              = 0x0230, /* 32-bit color QDv1.3 */
  gestaltAllegroQD              = 0x0250, /* Allegro QD OS 8.5 */
  gestaltMacOSXQD               = 0x0300 /* 0x310, 0x320 etc. for 10.x.y */
};

enum {
  gestaltQD3D                   = 'qd3d', /* Quickdraw 3D attributes*/
  gestaltQD3DPresent            = 0     /* bit 0 set if QD3D available*/
};

#if OLDROUTINENAMES
enum {
  gestaltQD3DNotPresent         = (0 << gestaltQD3DPresent),
  gestaltQD3DAvailable          = (1 << gestaltQD3DPresent)
};

#endif  /* OLDROUTINENAMES */

enum {
  gestaltGXVersion              = 'qdgx' /* Overall QuickDraw GX Version*/
};

enum {
  gestaltQuickdrawFeatures      = 'qdrw', /* quickdraw features */
  gestaltHasColor               = 0,    /* color quickdraw present */
  gestaltHasDeepGWorlds         = 1,    /* GWorlds can be deeper than 1-bit */
  gestaltHasDirectPixMaps       = 2,    /* PixMaps can be direct (16 or 32 bit) */
  gestaltHasGrayishTextOr       = 3,    /* supports text mode grayishTextOr */
  gestaltSupportsMirroring      = 4,    /* Supports video mirroring via the Display Manager. */
  gestaltQDHasLongRowBytes      = 5     /* Long rowBytes supported in GWorlds */
};

enum {
  gestaltQDTextVersion          = 'qdtx', /* QuickdrawText version */
  gestaltOriginalQDText         = 0x0000, /* up to and including 8.1 */
  gestaltAllegroQDText          = 0x0100, /* starting with 8.5 */
  gestaltMacOSXQDText           = 0x0200 /* we are in Mac OS X */
};

enum {
  gestaltQDTextFeatures         = 'qdtf', /* QuickdrawText features */
  gestaltWSIISupport            = 0,    /* bit 0: WSII support included */
  gestaltSbitFontSupport        = 1,    /* sbit-only fonts supported */
  gestaltAntiAliasedTextAvailable = 2,  /* capable of antialiased text */
  gestaltOFA2available          = 3,    /* OFA2 available */
  gestaltCreatesAliasFontRsrc   = 4,    /* "real" datafork font support */
  gestaltNativeType1FontSupport = 5,    /* we have scaler for Type1 fonts */
  gestaltCanUseCGTextRendering  = 6
};


enum {
  gestaltQuickTimeConferencingInfo = 'qtci' /* returns pointer to QuickTime Conferencing information */
};

enum {
  gestaltQuickTimeVersion       = 'qtim', /* returns version of QuickTime */
  gestaltQuickTime              = 'qtim' /* gestaltQuickTime is old name for gestaltQuickTimeVersion */
};

enum {
  gestaltQuickTimeFeatures      = 'qtrs',
  gestaltPPCQuickTimeLibPresent = 0     /* PowerPC QuickTime glue library is present */
};

enum {
  gestaltQuickTimeStreamingFeatures = 'qtsf'
};

enum {
  gestaltQuickTimeStreamingVersion = 'qtst'
};

enum {
  gestaltQuickTimeThreadSafeFeaturesAttr = 'qtth', /* Quicktime thread safety attributes */
  gestaltQuickTimeThreadSafeICM = 0,
  gestaltQuickTimeThreadSafeMovieToolbox = 1,
  gestaltQuickTimeThreadSafeMovieImport = 2,
  gestaltQuickTimeThreadSafeMovieExport = 3,
  gestaltQuickTimeThreadSafeGraphicsImport = 4,
  gestaltQuickTimeThreadSafeGraphicsExport = 5,
  gestaltQuickTimeThreadSafeMoviePlayback = 6
};

enum {
  gestaltQTVRMgrAttr            = 'qtvr', /* QuickTime VR attributes                               */
  gestaltQTVRMgrPresent         = 0,    /* QTVR API is present                                   */
  gestaltQTVRObjMoviesPresent   = 1,    /* QTVR runtime knows about object movies                */
  gestaltQTVRCylinderPanosPresent = 2,  /* QTVR runtime knows about cylindrical panoramic movies */
  gestaltQTVRCubicPanosPresent  = 3     /* QTVR runtime knows about cubic panoramic movies       */
};

enum {
  gestaltQTVRMgrVers            = 'qtvv' /* QuickTime VR version                                  */
};

/*    
    Because some PowerPC machines now support very large physical memory capacities, including
    some above the maximum value which can held in a 32 bit quantity, there is now a new selector,
    gestaltPhysicalRAMSizeInMegabytes, which returns the size of physical memory scaled
    in megabytes.  It is recommended that code transition to using this new selector if
    it wants to get a useful value for the amount of physical memory on the system.  Code can
    also use the sysctl() and sysctlbyname() BSD calls to get these kinds of values.
    
    For compatability with code which assumed that the value in returned by the
    gestaltPhysicalRAMSize selector would be a signed quantity of bytes, this selector will
    now return 2 gigabytes-1 ( INT_MAX ) if the system has 2 gigabytes of physical memory or more.
*/
enum {
  gestaltPhysicalRAMSize        = 'ram ' /* physical RAM size, in bytes */
};

enum {
  gestaltPhysicalRAMSizeInMegabytes = 'ramm' /* physical RAM size, scaled in megabytes */
};

enum {
  gestaltRBVAddr                = 'rbv ' /* RBV base address  */
};

enum {
  gestaltROMSize                = 'rom ' /* rom size */
};

enum {
  gestaltROMVersion             = 'romv' /* rom version */
};

enum {
  gestaltResourceMgrAttr        = 'rsrc', /* Resource Mgr attributes */
  gestaltPartialRsrcs           = 0,    /* True if partial resources exist */
  gestaltHasResourceOverrides   = 1     /* Appears in the ROM; so put it here. */
};

enum {
  gestaltResourceMgrBugFixesAttrs = 'rmbg', /* Resource Mgr bug fixes */
  gestaltRMForceSysHeapRolledIn = 0,
  gestaltRMFakeAppleMenuItemsRolledIn = 1,
  gestaltSanityCheckResourceFiles = 2,  /* Resource manager does sanity checking on resource files before opening them */
  gestaltSupportsFSpResourceFileAlreadyOpenBit = 3, /* The resource manager supports GetResFileRefNum and FSpGetResFileRefNum and FSpResourceFileAlreadyOpen */
  gestaltRMSupportsFSCalls      = 4,    /* The resource manager supports OpenResFileFSRef, CreateResFileFSRef and  ResourceFileAlreadyOpenFSRef */
  gestaltRMTypeIndexOrderingReverse = 8 /* GetIndType() calls return resource types in opposite order to original 68k resource manager */
};


enum {
  gestaltRealtimeMgrAttr        = 'rtmr', /* Realtime manager attributes         */
  gestaltRealtimeMgrPresent     = 0     /* true if the Realtime manager is present    */
};


enum {
  gestaltSafeOFAttr             = 'safe',
  gestaltVMZerosPagesBit        = 0,
  gestaltInitHeapZerosOutHeapsBit = 1,
  gestaltNewHandleReturnsZeroedMemoryBit = 2,
  gestaltNewPtrReturnsZeroedMemoryBit = 3,
  gestaltFileAllocationZeroedBlocksBit = 4
};


enum {
  gestaltSCCReadAddr            = 'sccr' /* scc read base address  */
};

enum {
  gestaltSCCWriteAddr           = 'sccw' /* scc read base address  */
};

enum {
  gestaltScrapMgrAttr           = 'scra', /* Scrap Manager attributes */
  gestaltScrapMgrTranslationAware = 0   /* True if scrap manager is translation aware */
};

enum {
  gestaltScriptMgrVersion       = 'scri' /* Script Manager version number     */
};

enum {
  gestaltScriptCount            = 'scr#' /* number of active script systems   */
};

enum {
  gestaltSCSI                   = 'scsi', /* SCSI Manager attributes */
  gestaltAsyncSCSI              = 0,    /* Supports Asynchronous SCSI */
  gestaltAsyncSCSIINROM         = 1,    /* Async scsi is in ROM (available for booting) */
  gestaltSCSISlotBoot           = 2,    /* ROM supports Slot-style PRAM for SCSI boots (PDM and later) */
  gestaltSCSIPollSIH            = 3     /* SCSI Manager will poll for interrupts if Secondary Interrupts are busy. */
};

enum {
  gestaltControlStripAttr       = 'sdev', /* Control Strip attributes */
  gestaltControlStripExists     = 0,    /* Control Strip is installed */
  gestaltControlStripVersionFixed = 1,  /* Control Strip version Gestalt selector was fixed */
  gestaltControlStripUserFont   = 2,    /* supports user-selectable font/size */
  gestaltControlStripUserHotKey = 3     /* support user-selectable hot key to show/hide the window */
};

enum {
  gestaltSDPStandardDirectoryVersion = 'sdvr' /* OCE Standard Directory Panel*/
};

enum {
  gestaltSerialAttr             = 'ser ', /* Serial attributes */
  gestaltHasGPIaToDCDa          = 0,    /* GPIa connected to DCDa*/
  gestaltHasGPIaToRTxCa         = 1,    /* GPIa connected to RTxCa clock input*/
  gestaltHasGPIbToDCDb          = 2,    /* GPIb connected to DCDb */
  gestaltHidePortA              = 3,    /* Modem port (A) should be hidden from users */
  gestaltHidePortB              = 4,    /* Printer port (B) should be hidden from users */
  gestaltPortADisabled          = 5,    /* Modem port (A) disabled and should not be used by SW */
  gestaltPortBDisabled          = 6     /* Printer port (B) disabled and should not be used by SW */
};

enum {
  gestaltShutdownAttributes     = 'shut', /* ShutDown Manager Attributes */
  gestaltShutdownHassdOnBootVolUnmount = 0 /* True if ShutDown Manager unmounts boot & VM volume at shutdown time. */
};

enum {
  gestaltNuBusConnectors        = 'sltc' /* bitmap of NuBus connectors*/
};

enum {
  gestaltSlotAttr               = 'slot', /* slot attributes  */
  gestaltSlotMgrExists          = 0,    /* true is slot mgr exists  */
  gestaltNuBusPresent           = 1,    /* NuBus slots are present  */
  gestaltSESlotPresent          = 2,    /* SE PDS slot present  */
  gestaltSE30SlotPresent        = 3,    /* SE/30 slot present  */
  gestaltPortableSlotPresent    = 4     /* PortableÕs slot present  */
};

enum {
  gestaltFirstSlotNumber        = 'slt1' /* returns first physical slot */
};

enum {
  gestaltSoundAttr              = 'snd ', /* sound attributes */
  gestaltStereoCapability       = 0,    /* sound hardware has stereo capability */
  gestaltStereoMixing           = 1,    /* stereo mixing on external speaker */
  gestaltSoundIOMgrPresent      = 3,    /* The Sound I/O Manager is present */
  gestaltBuiltInSoundInput      = 4,    /* built-in Sound Input hardware is present */
  gestaltHasSoundInputDevice    = 5,    /* Sound Input device available */
  gestaltPlayAndRecord          = 6,    /* built-in hardware can play and record simultaneously */
  gestalt16BitSoundIO           = 7,    /* sound hardware can play and record 16-bit samples */
  gestaltStereoInput            = 8,    /* sound hardware can record stereo */
  gestaltLineLevelInput         = 9,    /* sound input port requires line level */
                                        /* the following bits are not defined prior to Sound Mgr 3.0 */
  gestaltSndPlayDoubleBuffer    = 10,   /* SndPlayDoubleBuffer available, set by Sound Mgr 3.0 and later */
  gestaltMultiChannels          = 11,   /* multiple channel support, set by Sound Mgr 3.0 and later */
  gestalt16BitAudioSupport      = 12    /* 16 bit audio data supported, set by Sound Mgr 3.0 and later */
};

enum {
  gestaltSplitOSAttr            = 'spos',
  gestaltSplitOSBootDriveIsNetworkVolume = 0, /* the boot disk is a network 'disk', from the .LANDisk drive. */
  gestaltSplitOSAware           = 1,    /* the system includes the code to deal with a split os situation. */
  gestaltSplitOSEnablerVolumeIsDifferentFromBootVolume = 2, /* the active enabler is on a different volume than the system file. */
  gestaltSplitOSMachineNameSetToNetworkNameTemp = 3, /* The machine name was set to the value given us from the BootP server */
  gestaltSplitOSMachineNameStartupDiskIsNonPersistent = 5 /* The startup disk ( vRefNum == -1 ) is non-persistent, meaning changes won't persist across a restart. */
};

enum {
  gestaltSMPSPSendLetterVersion = 'spsl' /* OCE StandardMail*/
};

enum {
  gestaltSpeechRecognitionAttr  = 'srta', /* speech recognition attributes */
  gestaltDesktopSpeechRecognition = 1,  /* recognition thru the desktop microphone is available */
  gestaltTelephoneSpeechRecognition = 2 /* recognition thru the telephone is available */
};

enum {
  gestaltSpeechRecognitionVersion = 'srtb' /* speech recognition version (0x0150 is the first version that fully supports the API) */
};

enum {
  gestaltSoftwareVendorCode     = 'srad', /* Returns system software vendor information */
  gestaltSoftwareVendorApple    = 'Appl', /* System software sold by Apple */
  gestaltSoftwareVendorLicensee = 'Lcns' /* System software sold by licensee */
};

enum {
  gestaltStandardFileAttr       = 'stdf', /* Standard File attributes */
  gestaltStandardFile58         = 0,    /* True if selectors 5-8 (StandardPutFile-CustomGetFile) are supported */
  gestaltStandardFileTranslationAware = 1, /* True if standard file is translation manager aware */
  gestaltStandardFileHasColorIcons = 2, /* True if standard file has 16x16 color icons */
  gestaltStandardFileUseGenericIcons = 3, /* Standard file LDEF to use only the system generic icons if true */
  gestaltStandardFileHasDynamicVolumeAllocation = 4 /* True if standard file supports more than 20 volumes */
};

enum {
  gestaltSysArchitecture        = 'sysa', /* Native System Architecture */
  gestalt68k                    = 1,    /* Motorola MC68k architecture */
  gestaltPowerPC                = 2,    /* IBM PowerPC architecture */
  gestaltIntel                  = 10,   /* Intel x86 architecture */
  gestaltArm                    = 20    /* Arm architecture */
};

enum {
  gestaltSystemUpdateVersion    = 'sysu' /* System Update version */
};

/*  
    Returns the system version as a 32 bit packed BCD ( binary coded decimal )
    version representation.  Bits 0 through 3 are the "bug fix" revision number.
    Bits 4 through 7 are the minor revision, and bits 8 through 31 are the bcd
    decimal digits of the major release version.
    
      Value:  0xMMMMMMRB = M.R.B            Example: 0x00001023 = 10.2.3
                ^^^^^^     major rev                   ^^^^^^   major rev   = 10
                      ^    minor rev                         ^  minor rev   =  2
                       ^   bug fix rev                        ^ bug fix rev =  3
    
    If the values of the minor or bug fix revision are larger than 9, then
    gestaltSystemVersion will substitute the value 9 for them.  For example,
    Mac OS X 10.3.15 will be returned as 0x1039, and Mac OS X 10.10.5 will
    return 0x1095.
    
    A better way to get version information with the deprecated Gestalt API
	would be to use the gestaltSystemVersionMajor, gestaltSystemVersionMinor,
    and gestaltSystemVersionBugFix selectors, which don't have arbitrary limits
    on the values returned. However, because Gestalt is deprecated, you should
	use NSProcessInfo's operatingSystemVersion property instead.
 
    If you want to know the product build version string, product name, or
    the user visible version string you should read in the system version
    information from the file /System/Library/CoreServices/SystemVersion.plist.
 
*/
enum {
  gestaltSystemVersion          = 'sysv', /* system version*/
  gestaltSystemVersionMajor     = 'sys1', /* The major system version number; in 10.4.17 this would be the decimal value 10 */
  gestaltSystemVersionMinor     = 'sys2', /* The minor system version number; in 10.4.17 this would be the decimal value 4 */
  gestaltSystemVersionBugFix    = 'sys3' /* The bug fix system version number; in 10.4.17 this would be the decimal value 17 */
} __OSX_AVAILABLE_BUT_DEPRECATED_MSG(__MAC_10_0, __MAC_10_8, __IPHONE_NA, __IPHONE_NA, "Use NSProcessInfo's operatingSystemVersion property instead.");

enum {
  gestaltToolboxTable           = 'tbtt' /*  OS trap table base  */
};

enum {
  gestaltTextEditVersion        = 'te  ', /* TextEdit version number */
  gestaltTE1                    = 1,    /* TextEdit in MacIIci ROM */
  gestaltTE2                    = 2,    /* TextEdit with 6.0.4 Script Systems on MacIIci (Script bug fixes for MacIIci) */
  gestaltTE3                    = 3,    /* TextEdit with 6.0.4 Script Systems all but MacIIci */
  gestaltTE4                    = 4,    /* TextEdit in System 7.0 */
  gestaltTE5                    = 5     /* TextWidthHook available in TextEdit */
};

enum {
  gestaltTE6                    = 6     /* TextEdit with integrated TSMTE and improved UI */
};

enum {
  gestaltTEAttr                 = 'teat', /* TextEdit attributes */
  gestaltTEHasGetHiliteRgn      = 0,    /* TextEdit has TEGetHiliteRgn */
  gestaltTESupportsInlineInput  = 1,    /* TextEdit does Inline Input */
  gestaltTESupportsTextObjects  = 2,    /* TextEdit does Text Objects */
  gestaltTEHasWhiteBackground   = 3     /* TextEdit supports overriding the TERec's background to white */
};

enum {
  gestaltTeleMgrAttr            = 'tele', /* Telephone manager attributes */
  gestaltTeleMgrPresent         = 0,
  gestaltTeleMgrPowerPCSupport  = 1,
  gestaltTeleMgrSoundStreams    = 2,
  gestaltTeleMgrAutoAnswer      = 3,
  gestaltTeleMgrIndHandset      = 4,
  gestaltTeleMgrSilenceDetect   = 5,
  gestaltTeleMgrNewTELNewSupport = 6
};

enum {
  gestaltTermMgrAttr            = 'term', /* terminal mgr attributes */
  gestaltTermMgrPresent         = 0,
  gestaltTermMgrErrorString     = 2
};

enum {
  gestaltThreadMgrAttr          = 'thds', /* Thread Manager attributes */
  gestaltThreadMgrPresent       = 0,    /* bit true if Thread Mgr is present */
  gestaltSpecificMatchSupport   = 1,    /* bit true if Thread Mgr supports exact match creation option */
  gestaltThreadsLibraryPresent  = 2     /* bit true if Thread Mgr shared library is present */
};

enum {
  gestaltTimeMgrVersion         = 'tmgr', /* time mgr version */
  gestaltStandardTimeMgr        = 1,    /* standard time mgr is present */
  gestaltRevisedTimeMgr         = 2,    /* revised time mgr is present */
  gestaltExtendedTimeMgr        = 3,    /* extended time mgr is present */
  gestaltNativeTimeMgr          = 4     /* PowerPC native TimeMgr is present */
};

enum {
  gestaltTSMTEVersion           = 'tmTV',
  gestaltTSMTE1                 = 0x0100, /* Original version of TSMTE */
  gestaltTSMTE15                = 0x0150, /* System 8.0 */
  gestaltTSMTE152               = 0x0152 /* System 8.2 */
};

enum {
  gestaltTSMTEAttr              = 'tmTE',
  gestaltTSMTEPresent           = 0,
  gestaltTSMTE                  = 0     /* gestaltTSMTE is old name for gestaltTSMTEPresent */
};

enum {
  gestaltAVLTreeAttr            = 'tree', /* AVLTree utility routines attributes. */
  gestaltAVLTreePresentBit      = 0,    /* if set, then the AVL Tree routines are available. */
  gestaltAVLTreeSupportsHandleBasedTreeBit = 1, /* if set, then the AVL Tree routines can store tree data in a single handle */
  gestaltAVLTreeSupportsTreeLockingBit = 2 /* if set, the AVLLockTree() and AVLUnlockTree() routines are available. */
};

enum {
  gestaltALMAttr                = 'trip', /* Settings Manager attributes (see also gestaltALMVers) */
  gestaltALMPresent             = 0,    /* bit true if ALM is available */
  gestaltALMHasSFGroup          = 1,    /* bit true if Put/Get/Merge Group calls are implmented */
  gestaltALMHasCFMSupport       = 2,    /* bit true if CFM-based modules are supported */
  gestaltALMHasRescanNotifiers  = 3     /* bit true if Rescan notifications/events will be sent to clients */
};

enum {
  gestaltALMHasSFLocation       = gestaltALMHasSFGroup
};

enum {
  gestaltTSMgrVersion           = 'tsmv', /* Text Services Mgr version, if present */
  gestaltTSMgr15                = 0x0150,
  gestaltTSMgr20                = 0x0200, /* Version 2.0 as of MacOSX 10.0 */
  gestaltTSMgr22                = 0x0220, /* Version 2.2 as of MacOSX 10.3 */
  gestaltTSMgr23                = 0x0230 /* Version 2.3 as of MacOSX 10.4 */
};

enum {
  gestaltTSMgrAttr              = 'tsma', /* Text Services Mgr attributes, if present */
  gestaltTSMDisplayMgrAwareBit  = 0,    /* TSM knows about display manager */
  gestaltTSMdoesTSMTEBit        = 1     /* TSM has integrated TSMTE */
};

enum {
  gestaltSpeechAttr             = 'ttsc', /* Speech Manager attributes */
  gestaltSpeechMgrPresent       = 0,    /* bit set indicates that Speech Manager exists */
  gestaltSpeechHasPPCGlue       = 1     /* bit set indicates that native PPC glue for Speech Manager API exists */
};

enum {
  gestaltTVAttr                 = 'tv  ', /* TV version */
  gestaltHasTVTuner             = 0,    /* supports Philips FL1236F video tuner */
  gestaltHasSoundFader          = 1,    /* supports Philips TEA6330 Sound Fader chip */
  gestaltHasHWClosedCaptioning  = 2,    /* supports Philips SAA5252 Closed Captioning */
  gestaltHasIRRemote            = 3,    /* supports CyclopsII Infra Red Remote control */
  gestaltHasVidDecoderScaler    = 4,    /* supports Philips SAA7194 Video Decoder/Scaler */
  gestaltHasStereoDecoder       = 5,    /* supports Sony SBX1637A-01 stereo decoder */
  gestaltHasSerialFader         = 6,    /* has fader audio in serial with system audio */
  gestaltHasFMTuner             = 7,    /* has FM Tuner from donnybrook card */
  gestaltHasSystemIRFunction    = 8,    /* Infra Red button function is set up by system and not by Video Startup */
  gestaltIRDisabled             = 9,    /* Infra Red remote is not disabled. */
  gestaltINeedIRPowerOffConfirm = 10,   /* Need IR power off confirm dialog. */
  gestaltHasZoomedVideo         = 11    /* Has Zoomed Video PC Card video input. */
};


enum {
  gestaltATSUVersion            = 'uisv',
  gestaltOriginalATSUVersion    = (1 << 16), /* ATSUI version 1.0 */
  gestaltATSUUpdate1            = (2 << 16), /* ATSUI version 1.1 */
  gestaltATSUUpdate2            = (3 << 16), /* ATSUI version 1.2 */
  gestaltATSUUpdate3            = (4 << 16), /* ATSUI version 2.0 */
  gestaltATSUUpdate4            = (5 << 16), /* ATSUI version in Mac OS X - SoftwareUpdate 1-4 for Mac OS 10.0.1 - 10.0.4 */
  gestaltATSUUpdate5            = (6 << 16), /* ATSUI version 2.3 in MacOS 10.1 */
  gestaltATSUUpdate6            = (7 << 16), /* ATSUI version 2.4 in MacOS 10.2 */
  gestaltATSUUpdate7            = (8 << 16) /* ATSUI version 2.5 in MacOS 10.3 */
};

enum {
  gestaltATSUFeatures           = 'uisf',
  gestaltATSUTrackingFeature    = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSUMemoryFeature      = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSUFallbacksFeature   = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSUGlyphBoundsFeature = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSULineControlFeature = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSULayoutCreateAndCopyFeature = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSULayoutCacheClearFeature = 0x00000001, /* feature introduced in ATSUI version 1.1 */
  gestaltATSUTextLocatorUsageFeature = 0x00000002, /* feature introduced in ATSUI version 1.2 */
  gestaltATSULowLevelOrigFeatures = 0x00000004, /* first low-level features introduced in ATSUI version 2.0 */
  gestaltATSUFallbacksObjFeatures = 0x00000008, /* feature introduced - ATSUFontFallbacks objects introduced in ATSUI version 2.3 */
  gestaltATSUIgnoreLeadingFeature = 0x00000008, /* feature introduced - kATSLineIgnoreFontLeading LineLayoutOption introduced in ATSUI version 2.3 */
  gestaltATSUByCharacterClusterFeature = 0x00000010, /* ATSUCursorMovementTypes introduced in ATSUI version 2.4 */
  gestaltATSUAscentDescentControlsFeature = 0x00000010, /* attributes introduced in ATSUI version 2.4 */
  gestaltATSUHighlightInactiveTextFeature = 0x00000010, /* feature introduced in ATSUI version 2.4 */
  gestaltATSUPositionToCursorFeature = 0x00000010, /* features introduced in ATSUI version 2.4 */
  gestaltATSUBatchBreakLinesFeature = 0x00000010, /* feature introduced in ATSUI version 2.4 */
  gestaltATSUTabSupportFeature  = 0x00000010, /* features introduced in ATSUI version 2.4 */
  gestaltATSUDirectAccess       = 0x00000010, /* features introduced in ATSUI version 2.4 */
  gestaltATSUDecimalTabFeature  = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUBiDiCursorPositionFeature = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUNearestCharLineBreakFeature = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUHighlightColorControlFeature = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUUnderlineOptionsStyleFeature = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUStrikeThroughStyleFeature = 0x00000020, /* feature introduced in ATSUI version 2.5 */
  gestaltATSUDropShadowStyleFeature = 0x00000020 /* feature introduced in ATSUI version 2.5 */
};

enum {
  gestaltUSBAttr                = 'usb ', /* USB Attributes */
  gestaltUSBPresent             = 0,    /* USB Support available */
  gestaltUSBHasIsoch            = 1     /* USB Isochronous features available */
};

enum {
  gestaltUSBVersion             = 'usbv' /* USB version */
};

enum {
  gestaltVersion                = 'vers', /* gestalt version */
  gestaltValueImplementedVers   = 5     /* version of gestalt where gestaltValue is implemented. */
};

enum {
  gestaltVIA1Addr               = 'via1' /* via 1 base address  */
};

enum {
  gestaltVIA2Addr               = 'via2' /* via 2 base address  */
};

enum {
  gestaltVMAttr                 = 'vm  ', /* virtual memory attributes */
  gestaltVMPresent              = 0,    /* true if virtual memory is present */
  gestaltVMHasLockMemoryForOutput = 1,  /* true if LockMemoryForOutput is available */
  gestaltVMFilemappingOn        = 3,    /* true if filemapping is available */
  gestaltVMHasPagingControl     = 4     /* true if MakeMemoryResident, MakeMemoryNonResident, FlushMemory, and ReleaseMemoryData are available */
};

enum {
  gestaltVMInfoType             = 'vmin', /* Indicates how the Finder should display information about VM in */
                                        /* the Finder about box. */
  gestaltVMInfoSizeStorageType  = 0,    /* Display VM on/off, backing store size and name */
  gestaltVMInfoSizeType         = 1,    /* Display whether VM is on or off and the size of the backing store */
  gestaltVMInfoSimpleType       = 2,    /* Display whether VM is on or off */
  gestaltVMInfoNoneType         = 3     /* Display no VM information */
};

enum {
  gestaltVMBackingStoreFileRefNum = 'vmbs' /* file refNum of virtual memory's main backing store file (returned in low word of result) */
};



enum {
  gestaltALMVers                = 'walk' /* Settings Manager version (see also gestaltALMAttr) */
};

enum {
  gestaltWindowMgrAttr          = 'wind', /* If this Gestalt exists, the Mac OS 8.5 Window Manager is installed*/
  gestaltWindowMgrPresent       = (1L << 0), /* NOTE: this is a bit mask, whereas all other Gestalt constants of*/
                                        /* this type are bit index values.   Universal Interfaces 3.2 slipped*/
                                        /* out the door with this mistake.*/
  gestaltWindowMgrPresentBit    = 0,    /* bit number*/
  gestaltExtendedWindowAttributes = 1,  /* Has ChangeWindowAttributes; GetWindowAttributes works for all windows*/
  gestaltExtendedWindowAttributesBit = 1, /* Has ChangeWindowAttributes; GetWindowAttributes works for all windows*/
  gestaltHasFloatingWindows     = 2,    /* Floating window APIs work*/
  gestaltHasFloatingWindowsBit  = 2,    /* Floating window APIs work*/
  gestaltHasWindowBuffering     = 3,    /* This system has buffering available*/
  gestaltHasWindowBufferingBit  = 3,    /* This system has buffering available*/
  gestaltWindowLiveResizeBit    = 4,    /* live resizing of windows is available*/
  gestaltWindowMinimizeToDockBit = 5,   /* windows minimize to the dock and do not windowshade (Mac OS X)*/
  gestaltHasWindowShadowsBit    = 6,    /* windows have shadows*/
  gestaltSheetsAreWindowModalBit = 7,   /* sheet windows are modal only to their parent window*/
  gestaltFrontWindowMayBeHiddenBit = 8, /* FrontWindow and related APIs will return the front window even when the app is hidden*/
                                        /* masks for the above bits*/
  gestaltWindowMgrPresentMask   = (1L << gestaltWindowMgrPresentBit),
  gestaltExtendedWindowAttributesMask = (1L << gestaltExtendedWindowAttributesBit),
  gestaltHasFloatingWindowsMask = (1L << gestaltHasFloatingWindowsBit),
  gestaltHasWindowBufferingMask = (1L << gestaltHasWindowBufferingBit),
  gestaltWindowLiveResizeMask   = (1L << gestaltWindowLiveResizeBit),
  gestaltWindowMinimizeToDockMask = (1L << gestaltWindowMinimizeToDockBit),
  gestaltHasWindowShadowsMask   = (1L << gestaltHasWindowShadowsBit),
  gestaltSheetsAreWindowModalMask = (1L << gestaltSheetsAreWindowModalBit),
  gestaltFrontWindowMayBeHiddenMask = (1L << gestaltFrontWindowMayBeHiddenBit)
};

enum {
  gestaltHasSingleWindowModeBit = 8,    /* This system supports single window mode*/
  gestaltHasSingleWindowModeMask = (1L << gestaltHasSingleWindowModeBit)
};


/* gestaltX86Features is a convenience for 'cpuid' instruction.  Note
   that even though the processor may support a specific feature, the
   OS may not support all of these features.  These bitfields
   correspond directly to the bits returned by cpuid */
enum {
  gestaltX86Features            = 'x86f',
  gestaltX86HasFPU              = 0,    /* has an FPU that supports the 387 instructions*/
  gestaltX86HasVME              = 1,    /* supports Virtual-8086 Mode Extensions*/
  gestaltX86HasDE               = 2,    /* supports I/O breakpoints (Debug Extensions)*/
  gestaltX86HasPSE              = 3,    /* supports 4-Mbyte pages (Page Size Extension)*/
  gestaltX86HasTSC              = 4,    /* supports RTDSC instruction (Time Stamp Counter)*/
  gestaltX86HasMSR              = 5,    /* supports Model Specific Registers*/
  gestaltX86HasPAE              = 6,    /* supports physical addresses > 32 bits (Physical Address Extension)*/
  gestaltX86HasMCE              = 7,    /* supports Machine Check Exception*/
  gestaltX86HasCX8              = 8,    /* supports CMPXCHG8 instructions (Compare Exchange 8 bytes)*/
  gestaltX86HasAPIC             = 9,    /* contains local APIC*/
  gestaltX86HasSEP              = 11,   /* supports fast system call (SysEnter Present)*/
  gestaltX86HasMTRR             = 12,   /* supports Memory Type Range Registers*/
  gestaltX86HasPGE              = 13,   /* supports Page Global Enable*/
  gestaltX86HasMCA              = 14,   /* supports Machine Check Architecture*/
  gestaltX86HasCMOV             = 15,   /* supports CMOVcc instruction (Conditional Move).*/
                                        /* If FPU bit is also set, supports FCMOVcc and FCOMI, too*/
  gestaltX86HasPAT              = 16,   /* supports Page Attribute Table*/
  gestaltX86HasPSE36            = 17,   /* supports 36-bit Page Size Extension*/
  gestaltX86HasPSN              = 18,   /* Processor Serial Number*/
  gestaltX86HasCLFSH            = 19,   /* CLFLUSH Instruction supported*/
  gestaltX86Serviced20          = 20,   /* do not count on this bit value*/
  gestaltX86HasDS               = 21,   /* Debug Store*/
  gestaltX86ResACPI             = 22,   /* Thermal Monitor, SW-controlled clock*/
  gestaltX86HasMMX              = 23,   /* supports MMX instructions*/
  gestaltX86HasFXSR             = 24,   /* Supports FXSAVE and FXRSTOR instructions (fast FP save/restore)*/
  gestaltX86HasSSE              = 25,   /* Streaming SIMD extensions*/
  gestaltX86HasSSE2             = 26,   /* Streaming SIMD extensions 2*/
  gestaltX86HasSS               = 27,   /* Self-Snoop*/
  gestaltX86HasHTT              = 28,   /* Hyper-Threading Technology*/
  gestaltX86HasTM               = 29    /* Thermal Monitor*/
};

/* 'cpuid' now returns a 64 bit value, and the following 
    gestalt selector and field definitions apply
    to the extended form of this instruction */
enum {
  gestaltX86AdditionalFeatures  = 'x86a',
  gestaltX86HasSSE3             = 0,    /* Prescott New Inst.*/
  gestaltX86HasMONITOR          = 3,    /* Monitor/mwait*/
  gestaltX86HasDSCPL            = 4,    /* Debug Store CPL*/
  gestaltX86HasVMX              = 5,    /* VMX*/
  gestaltX86HasSMX              = 6,    /* SMX*/
  gestaltX86HasEST              = 7,    /* Enhanced SpeedsTep (GV3)*/
  gestaltX86HasTM2              = 8,    /* Thermal Monitor 2*/
  gestaltX86HasSupplementalSSE3 = 9,    /* Supplemental SSE3 instructions*/
  gestaltX86HasCID              = 10,   /* L1 Context ID*/
  gestaltX86HasCX16             = 13,   /* CmpXchg16b instruction*/
  gestaltX86HasxTPR             = 14    /* Send Task PRiority msgs*/
};

enum {
  gestaltTranslationAttr        = 'xlat', /* Translation Manager attributes */
  gestaltTranslationMgrExists   = 0,    /* True if translation manager exists */
  gestaltTranslationMgrHintOrder = 1,   /* True if hint order reversal in effect */
  gestaltTranslationPPCAvail    = 2,
  gestaltTranslationGetPathAPIAvail = 3
};

enum {
  gestaltExtToolboxTable        = 'xttt' /* Extended Toolbox trap table base */
};

enum {
  gestaltUSBPrinterSharingVersion = 'zak ', /* USB Printer Sharing Version*/
  gestaltUSBPrinterSharingVersionMask = 0x0000FFFF, /* mask for bits in version*/
  gestaltUSBPrinterSharingAttr  = 'zak ', /* USB Printer Sharing Attributes*/
  gestaltUSBPrinterSharingAttrMask = (int)0xFFFF0000, /*  mask for attribute bits*/
  gestaltUSBPrinterSharingAttrRunning = (int)0x80000000, /* printer sharing is running*/
  gestaltUSBPrinterSharingAttrBooted = 0x40000000 /* printer sharing was installed at boot time*/
};

/*WorldScript settings;*/
enum {
  gestaltWorldScriptIIVersion   = 'doub',
  gestaltWorldScriptIIAttr      = 'wsat',
  gestaltWSIICanPrintWithoutPrGeneralBit = 0 /* bit 0 is on if WS II knows about PrinterStatus callback */
};





#ifdef __cplusplus
}
#endif

#endif /* __GESTALT__ */

