/*
     File:       HIToolbox/HILittleArrows.h
 
     Contains:   Definition of the little arrows view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HILITTLEARROWS__
#define __HILITTLEARROWS__

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
 *  HILittleArrows.h
 *  
 *  Discussion:
 *    API definitions for the little arrows view.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ LITTLE ARROWS (CDEF 6)                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control implements the little up and down arrows that are used in clock views. */
/* Little Arrows proc IDs */
enum {
  kControlLittleArrowsProc      = 96
};

/* Control Kind Tag */
enum {
  kControlKindLittleArrows      = 'larr'
};


/*
 *  Summary:
 *    Tags available with Mac OS X 10.3 or later
 */
enum {

  /*
   * Passed data is an SInt32.  Gets or sets the increment value of the
   * control. Note that the little arrows control does not actually
   * ever change its own value in response to clicks, and so it does
   * not ever use the increment value; you must use an action proc to
   * handle clicks on the control's arrows and change the control value
   * yourself. However, you can store the increment value in the
   * control's data and read it later with GetControlData.
   */
  kControlLittleArrowsIncrementValueTag = 'incr'
};

/* The HIObject class ID for the HILittleArrows class. */
#define kHILittleArrowsClassID          CFSTR("com.apple.HILittleArrows")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateLittleArrowsControl()
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
CreateLittleArrowsControl(
  WindowRef     window,
  const Rect *  boundsRect,
  SInt32        value,
  SInt32        minimum,
  SInt32        maximum,
  SInt32        increment,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HILITTLEARROWS__ */

