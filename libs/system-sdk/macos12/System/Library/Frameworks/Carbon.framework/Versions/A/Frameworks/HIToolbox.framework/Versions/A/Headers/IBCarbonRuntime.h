/*
     File:       HIToolbox/IBCarbonRuntime.h
 
     Contains:   Nib support for Carbon
 
     Copyright:  © 2000-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __IBCARBONRUNTIME__
#define __IBCARBONRUNTIME__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif

#ifndef __CONTROLDEFINITIONS__
#include <HIToolbox/ControlDefinitions.h>
#endif




#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum {
  kIBCarbonRuntimeCantFindNibFile = -10960,
  kIBCarbonRuntimeObjectNotOfRequestedType = -10961,
  kIBCarbonRuntimeCantFindObject = -10962
};

/* ----- typedef ------ */
typedef struct OpaqueIBNibRef*          IBNibRef;
/* ----- Create & Dispose NIB References ------ */
#if !__LP64__
/*
 *  CreateNibReference()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateNibReference(
  CFStringRef   inNibName,
  IBNibRef *    outNibRef)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreateNibReferenceWithCFBundle()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateNibReferenceWithCFBundle(
  CFBundleRef   inBundle,
  CFStringRef   inNibName,
  IBNibRef *    outNibRef)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DisposeNibReference()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern void 
DisposeNibReference(IBNibRef inNibRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* ----- Window ------ */
/*
 *  CreateWindowFromNib()
 *  
 *  Summary:
 *    Creates a window from a description stored in a nib file.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inNibRef:
 *      The identifier for the nib file containing the window
 *      description.
 *    
 *    inName:
 *      The name of the window description.
 *    
 *    outWindow:
 *      On exit, contains the window, if creation of the window was
 *      successful.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateWindowFromNib(
  IBNibRef      inNibRef,
  CFStringRef   inName,
  WindowRef *   outWindow)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* ----- Menu -----*/

/*
 *  CreateMenuFromNib()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateMenuFromNib(
  IBNibRef      inNibRef,
  CFStringRef   inName,
  MenuRef *     outMenuRef)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* ----- MenuBar ------*/

/*
 *  CreateMenuBarFromNib()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateMenuBarFromNib(
  IBNibRef      inNibRef,
  CFStringRef   inName,
  Handle *      outMenuBar)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetMenuBarFromNib()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetMenuBarFromNib(
  IBNibRef      inNibRef,
  CFStringRef   inName)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __IBCARBONRUNTIME__ */

