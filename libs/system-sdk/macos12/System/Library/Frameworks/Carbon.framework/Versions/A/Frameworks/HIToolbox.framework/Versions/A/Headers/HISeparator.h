/*
     File:       HIToolbox/HISeparator.h
 
     Contains:   Definition of the separator view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISEPARATOR__
#define __HISEPARATOR__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __APPEARANCE__
#include <HIToolbox/Appearance.h>
#endif

#ifndef __CARBONEVENTS__
#include <HIToolbox/CarbonEvents.h>
#endif

#ifndef __CONTROLS__
#include <HIToolbox/Controls.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HISeparator.h
 *  
 *  Discussion:
 *    API definitions for the separator view.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ VISUAL SEPARATOR (CDEF 9)                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Separator lines determine their orientation (horizontal or vertical) automatically  */
/*  based on the relative height and width of their contrlRect.                         */
/* Visual separator proc IDs */
enum {
  kControlSeparatorLineProc     = 144
};

/* Control Kind Tag */
enum {
  kControlKindSeparator         = 'sepa'
};

/* The HIObject class ID for the HIVisualSeparator class. */
#define kHIVisualSeparatorClassID       CFSTR("com.apple.HIVisualSeparator")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateSeparatorControl()
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
CreateSeparatorControl(
  WindowRef     window,
  const Rect *  boundsRect,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HISEPARATOR__ */

