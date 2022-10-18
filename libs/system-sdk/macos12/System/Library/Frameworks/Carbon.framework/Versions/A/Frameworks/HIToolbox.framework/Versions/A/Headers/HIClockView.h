/*
     File:       HIToolbox/HIClockView.h
 
     Contains:   Definition of the clock view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HICLOCKVIEW__
#define __HICLOCKVIEW__

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
 *  HIClockView.h
 *  
 *  Discussion:
 *    API definitions for the clock view.
 */
/*==============================================================================*/
/*  Clock view events                                                           */
/*==============================================================================*/
enum {
  kEventClassClockView          = 'cloc'
};

/*
 *  kEventClassClockView / kEventClockDateOrTimeChanged
 *  
 *  Summary:
 *    Allows clients to determine when the user has changed the date or
 *    time in the clock view.
 *  
 *  Discussion:
 *    This event is sent by the clock view when the user has changed
 *    the date or time. Clients could register for this notification in
 *    order to update some state based on the date or time in the
 *    clock, for instance. This event is sent to the view only, it will
 *    not propagate. It is sent to all handlers installed on the view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamDirectObject (in, typeControlRef)
 *          The view whose date has changed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventClockDateOrTimeChanged  = 1
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ CLOCK (CDEF 15)                                                                   */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  NOTE:   You can specify more options in the Value paramter when creating the clock. */
/*          See below.                                                                  */
/*                                                                                      */
/*  NOTE:   Under Appearance 1.1, the clock control knows and returns more part codes.  */
/*          The new clock-specific part codes are defined with the other control parts. */
/*          Besides these clock-specific parts, we also return kControlUpButtonPart     */
/*          and kControlDownButtonPart when they hit the up and down arrows.            */
/*          The new part codes give you more flexibility for focusing and hit testing.  */
/*                                                                                      */
/*          The original kControlClockPart is still valid. When hit testing, it means   */
/*          that some non-editable area of the clock's whitespace has been clicked.     */
/*          When focusing a currently unfocused clock, it changes the focus to the      */
/*          first part; it is the same as passing kControlFocusNextPart. When           */
/*          re-focusing a focused clock, it will not change the focus at all.           */
/* Clock proc IDs */
enum {
  kControlClockTimeProc         = 240,
  kControlClockTimeSecondsProc  = 241,
  kControlClockDateProc         = 242,
  kControlClockMonthYearProc    = 243
};

/* Clock Types */
typedef UInt16 ControlClockType;
enum {
  kControlClockTypeHourMinute   = 0,
  kControlClockTypeHourMinuteSecond = 1,
  kControlClockTypeMonthDayYear = 2,
  kControlClockTypeMonthYear    = 3
};

/* Clock Flags */
/*  These flags can be passed into 'value' field on creation of the control.            */
/*  Value is set to 0 after control is created.                                         */
typedef UInt32 ControlClockFlags;
enum {
  kControlClockFlagStandard     = 0,    /* editable, non-live*/
  kControlClockNoFlags          = 0,
  kControlClockFlagDisplayOnly  = 1,    /* add this to become non-editable*/
  kControlClockIsDisplayOnly    = 1,
  kControlClockFlagLive         = 2,    /* automatically shows current time on idle. only valid with display only.*/
  kControlClockIsLive           = 2
};

/* Control Kind Tag */
enum {
  kControlKindClock             = 'clck'
};

/* The HIObject class ID for the HIClock class. */
#define kHIClockViewClassID             CFSTR("com.apple.HIClock")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateClockControl()
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
CreateClockControl(
  WindowRef           window,
  const Rect *        boundsRect,
  ControlClockType    clockType,
  ControlClockFlags   clockFlags,
  ControlRef *        outControl)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by clocks */
#endif  /* !__LP64__ */

enum {
  kControlClockLongDateTag      = 'date', /* LongDateRec, 32-bit only*/
  kControlClockAbsoluteTimeTag  = 'abst', /* CFAbsoluteTime; available in Leopard and later*/
  kControlClockFontStyleTag     = kControlFontStyleTag, /* ControlFontStyleRec*/
  kControlClockAnimatingTag     = 'anim' /* Boolean*/
};


#ifdef __cplusplus
}
#endif

#endif /* __HICLOCKVIEW__ */

