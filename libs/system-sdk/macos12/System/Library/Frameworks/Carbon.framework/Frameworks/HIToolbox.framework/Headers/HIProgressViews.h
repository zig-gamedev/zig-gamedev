/*
     File:       HIToolbox/HIProgressViews.h
 
     Contains:   Definition of the progress-display views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIPROGRESSVIEWS__
#define __HIPROGRESSVIEWS__

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
 *  HIProgressViews.h
 *  
 *  Discussion:
 *    API definitions for the views that display progress: the progress
 *    bar and the chasing arrows.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ PROGRESS INDICATOR (CDEF 5)                                                       */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This CDEF implements both determinate and indeterminate progress bars. To switch,   */
/*  just use SetControlData to set the indeterminate flag to make it indeterminate. Any */
/*  animation will automatically be handled by the toolbox on an event timer.           */
/* Progress Bar proc IDs */
enum {
  kControlProgressBarProc       = 80,
  kControlRelevanceBarProc      = 81
};

/* Control Kind Tag */
enum {
  kControlKindProgressBar       = 'prgb',
  kControlKindRelevanceBar      = 'relb'
};

/* The HIObject class ID for the HIProgressBar class. */
#define kHIProgressBarClassID           CFSTR("com.apple.HIProgressBar")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateProgressBarControl()
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
CreateProgressBarControl(
  WindowRef     window,
  const Rect *  boundsRect,
  SInt32        value,
  SInt32        minimum,
  SInt32        maximum,
  Boolean       indeterminate,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by progress bars */
#endif  /* !__LP64__ */

enum {
  kControlProgressBarIndeterminateTag = 'inde', /* Boolean*/
  kControlProgressBarAnimatingTag = 'anim' /* Boolean*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ CHASING ARROWS (CDEF 7)                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control will automatically animate via an event loop timer.                    */
/*                                                                                      */
/* Chasing Arrows proc IDs */
enum {
  kControlChasingArrowsProc     = 112
};

/* Control Kind Tag */
enum {
  kControlKindChasingArrows     = 'carr'
};

/* The HIObject class ID for the HIChasingArrows class. */
#define kHIChasingArrowsClassID         CFSTR("com.apple.HIChasingArrows")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateChasingArrowsControl()
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
CreateChasingArrowsControl(
  WindowRef     window,
  const Rect *  boundsRect,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by the Chasing Arrows control */
#endif  /* !__LP64__ */

enum {
  kControlChasingArrowsAnimatingTag = 'anim' /* Boolean*/
};



#ifdef __cplusplus
}
#endif

#endif /* __HIPROGRESSVIEWS__ */

