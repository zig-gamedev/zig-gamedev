/*
     File:       OpenScripting/AppleScript.h
 
     Contains:   AppleScript Specific Interfaces.
 
     Version:    OpenScripting
 
     Copyright:  © 1992-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLESCRIPT__
#define __APPLESCRIPT__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __OSA__
#include <OpenScripting/OSA.h>
#endif

#ifndef __TEXTEDIT__
#include <HIToolbox/TextEdit.h>
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
/*
    The specific type for the AppleScript instance of the
    Open Scripting Architecture type.
*/
enum {
  typeAppleScript               = 'ascr',
  kAppleScriptSubtype           = typeAppleScript,
  typeASStorage                 = typeAppleScript
};

/**************************************************************************
    Component Selectors
**************************************************************************/

enum {
  kASSelectInit                 = 0x1001,
  kASSelectSetSourceStyles      = 0x1002,
  kASSelectGetSourceStyles      = 0x1003,
  kASSelectGetSourceStyleNames  = 0x1004,
  kASSelectCopySourceAttributes = 0x1005,
  kASSelectSetSourceAttributes  = 0x1006
};


/**************************************************************************
    OSAGetScriptInfo Selectors
**************************************************************************/
enum {
  kASHasOpenHandler             = 'hsod'
};

/*
        This selector is used to query a context as to whether it contains
        a handler for the kAEOpenDocuments event. This allows "applets" to be 
        distinguished from "droplets."  OSAGetScriptInfo returns false if
        there is no kAEOpenDocuments handler, and returns the error value 
        errOSAInvalidAccess if the input is not a context.
    */
/**************************************************************************
    Initialization
**************************************************************************/
/*
 *  ASInit()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASInit(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  UInt32              minStackSize,
  UInt32              preferredStackSize,
  UInt32              maxStackSize,
  UInt32              minHeapSize,
  UInt32              preferredHeapSize,
  UInt32              maxHeapSize)                            __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);


/*
        ComponentCallNow(kASSelectInit, 28);
        This call can be used to explicitly initialize AppleScript.  If it is
        not called, the a scripting size resource is looked for and used. If
        there is no scripting size resource, then the constants listed below
        are used.  If at any stage (the init call, the size resource, the 
        defaults) any of these parameters are zero, then parameters from the
        next stage are used.  ModeFlags are not currently used.
        Errors:
        errOSASystemError       initialization failed
    */
/*
    These values will be used if ASInit is not called explicitly, or if any
    of ASInit's parameters are zero:
*/
enum {
  kASDefaultMinStackSize        = 4 * 1024,
  kASDefaultPreferredStackSize  = 16 * 1024,
  kASDefaultMaxStackSize        = 16 * 1024,
  kASDefaultMinHeapSize         = 4 * 1024,
  kASDefaultPreferredHeapSize   = 16 * 1024,
  kASDefaultMaxHeapSize         = 32L * 1024 * 1024
};

/**************************************************************************
    Source Styles
**************************************************************************/
/*
 *  ASSetSourceStyles()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use ASSetSourceAttributes instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASSetSourceStyles(
  ComponentInstance   scriptingComponent,
  STHandle            sourceStyles)                           __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  ASGetSourceStyles()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    use ASCopySourceAttributes instead.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASGetSourceStyles(
  ComponentInstance   scriptingComponent,
  STHandle *          resultingSourceStyles)                  __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0, __MAC_10_5, __IPHONE_NA, __IPHONE_NA);


/*
 *  ASCopySourceAttributes()
 *  
 *  Summary:
 *    Get the current AppleScript source style attributes.
 *  
 *  Discussion:
 *    A "style attribute" will typically be something meaningful to a
 *    CFAttributedString, but clients may add any attributes they like
 *    using ASSetSourceAttributes.
 *  
 *  Parameters:
 *    
 *    scriptingComponent:
 *      A valid AppleScript component instance.
 *    
 *    resultingSourceAttributes:
 *      If successful, *resultingSourceAttributes will be set to a
 *      CFArray of CFDictionaries of text attributes.  The order of the
 *      array elements corresponds to the source style constants below,
 *      and therefore also to the names returned by
 *      ASGetSourceStyleNames. The caller is responsible for releasing
 *      this array.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
ASCopySourceAttributes(
  ComponentInstance   scriptingComponent,
  CFArrayRef *        resultingSourceAttributes)              __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);


/*
 *  ASSetSourceAttributes()
 *  
 *  Summary:
 *    Set the AppleScript source style attributes.
 *  
 *  Discussion:
 *    A "style attribute" will typically be something meaningful to a
 *    CFAttributedString, but clients may add any attributes they like.
 *     Because of this, you should generally call ASSetSourceAttributes
 *    with a modified copy of the result from ASCopySourceAttributes,
 *    not a built-from-scratch set of attributes.
 *  
 *  Parameters:
 *    
 *    scriptingComponent:
 *      A valid AppleScript component instance.
 *    
 *    sourceAttributes:
 *      A CFArray of CFDictionaries of text attributes.  The order of
 *      the array elements corresponds to the source style constants
 *      below, and therefore also to the names returned by
 *      ASGetSourceStyleNames.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSAError 
ASSetSourceAttributes(
  ComponentInstance   scriptingComponent,
  CFArrayRef          sourceAttributes)                       __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_NA);


/*
 *  ASGetSourceStyleNames()
 *  
 *  Summary:
 *    Returns the AppleScript source style names.
 *  
 *  Discussion:
 *    This call returns an AEList of text descriptors with the names of
 *    the source styles.  The order of the names corresponds to the
 *    order of the source style constants, below.  The precise type of
 *    the text descriptors is not defined; you should coerce them to
 *    the type you want to handle.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppleScriptLib 1.1 and later
 */
extern OSAError 
ASGetSourceStyleNames(
  ComponentInstance   scriptingComponent,
  SInt32              modeFlags,
  AEDescList *        resultingSourceStyleNamesList)          __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);



/*
    Elements of STHandle correspond to following categories of tokens, and
    accessed through following index constants:
*/
enum {
  kASSourceStyleUncompiledText  = 0,
  kASSourceStyleNormalText      = 1,
  kASSourceStyleLanguageKeyword = 2,
  kASSourceStyleApplicationKeyword = 3,
  kASSourceStyleComment         = 4,
  kASSourceStyleLiteral         = 5,
  kASSourceStyleUserSymbol      = 6,
  kASSourceStyleObjectSpecifier = 7,    /* Obsolete. Has been reused for strings. */
  kASSourceStyleString          = 7,    /* This and after were added in Mac OS X 10.6. */
  kASSourceStyleEventName       = 8,
  kASSourceStyleParameterName   = 9,
  kASSourceStyleClass           = 10,
  kASSourceStyleProperty        = 11,
  kASSourceStyleEnumValue       = 12,
  kASSourceStyleDynamicEventName = 13,
  kASSourceStyleDynamicParameterName = 14,
  kASSourceStyleDynamicClass    = 15,
  kASSourceStyleDynamicProperty = 16,
  kASSourceStyleDynamicEnumValue = 17,
  kASNumberOfSourceStyles       = 18    /* Deprecated. Don't assume the array length is fixed, and don't assume all indices are in the array. Always get its size at runtime. */
};



#ifdef __cplusplus
}
#endif

#endif /* __APPLESCRIPT__ */

