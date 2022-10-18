/*
     File:       HIToolbox/HIDisclosureViews.h
 
     Contains:   API and type definitions related to disclosure views.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIDISCLOSUREVIEWS__
#define __HIDISCLOSUREVIEWS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ DISCLOSURE BUTTON                                                                 */
/*  (CDEF 30)                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* The HIObject class ID for the HIDisclosureButton class. */
#define kHIDisclosureButtonClassID      CFSTR("com.apple.HIDisclosureButton")
#if !__LP64__
/*
 *  CreateDisclosureButtonControl()
 *  
 *  Summary:
 *    Creates a new instance of the Disclosure Button Control.
 *  
 *  Discussion:
 *    CreateDisclosureButtonControl is preferred over NewControl
 *    because it allows you to specify the exact set of parameters
 *    required to create the control without overloading parameter
 *    semantics. The initial minimum of the Disclosure Button will be
 *    kControlDisclosureButtonClosed, and the maximum will be
 *    kControlDisclosureButtonDisclosed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef in which to create the control.
 *    
 *    inBoundsRect:
 *      The bounding rectangle for the control. The height of the
 *      control is fixed and the control will be centered vertically
 *      within the rectangle you specify.
 *    
 *    inValue:
 *      The initial value; either kControlDisclosureButtonClosed or
 *      kControlDisclosureButtonDisclosed.
 *    
 *    inAutoToggles:
 *      A boolean value indicating whether its value should change
 *      automatically after tracking the mouse.
 *    
 *    outControl:
 *      On successful exit, this will contain the new control.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateDisclosureButtonControl(
  WindowRef     inWindow,
  const Rect *  inBoundsRect,
  SInt32        inValue,
  Boolean       inAutoToggles,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* Control Kind Tag */
#endif  /* !__LP64__ */

enum {
  kControlKindDisclosureButton  = 'disb'
};


/*
 *  Discussion:
 *    Disclosure Button Values
 */
enum {

  /*
   * The control be drawn suggesting a closed state.
   */
  kControlDisclosureButtonClosed = 0,

  /*
   * The control will be drawn suggesting an open state.
   */
  kControlDisclosureButtonDisclosed = 1
};


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ DISCLOSURE TRIANGLE (CDEF 4)                                                      */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control can be used as either left or right facing. It can also handle its own */
/*  tracking if you wish. This means that when the 'autotoggle' variant is used, if the */
/*  user clicks the control, its state will change automatically from open to closed    */
/*  and vice-versa depending on its initial state. After a successful call to Track-    */
/*  Control, you can just check the current value to see what state it was switched to. */


/*
 *  Summary:
 *    Triangle proc IDs
 *  
 *  Discussion:
 *    This control can be used as either left or right facing. It can
 *    also handle its own tracking if you wish. This means that when
 *    the 'autotoggle' variant is used, if the user clicks the control,
 *    its state will change automatically from open to closed and
 *    vice-versa depending on its initial state. After a successful
 *    call to TrackControl, you can just check the current value to see
 *    what state it was switched to.
 */
enum {
  kControlTriangleProc          = 64,
  kControlTriangleLeftFacingProc = 65,
  kControlTriangleAutoToggleProc = 66,
  kControlTriangleLeftFacingAutoToggleProc = 67
};


/*
 *  ControlDisclosureTriangleOrientation
 *  
 *  Summary:
 *    The orientations available to a disclosure triangle control.
 */
typedef UInt16 ControlDisclosureTriangleOrientation;
enum {

  /*
   * Points right on a left-to-right script system (Mac OS X and later
   * or CarbonLib 1.5 and later only)
   */
  kControlDisclosureTrianglePointDefault = 0,
  kControlDisclosureTrianglePointRight = 1,
  kControlDisclosureTrianglePointLeft = 2
};

/* Control Kind Tag */
enum {
  kControlKindDisclosureTriangle = 'dist'
};

/* The HIObject class ID for the HIDisclosureTriangle class. */
#define kHIDisclosureTriangleClassID    CFSTR("com.apple.HIDisclosureTriangle")
#if !__LP64__
/*
 *  CreateDisclosureTriangleControl()
 *  
 *  Summary:
 *    Creates a Disclosure Triangle control at a specific position in
 *    the specified window.
 *  
 *  Discussion:
 *    Disclosure Triangles are small controls that give the user a way
 *    to toggle the visibility of information or other user interface.
 *    When information is in a hidden state, a Disclosure Triangle is
 *    considered "closed" and should point to the right (or sometimes
 *    to the left). When the user clicks on it, the Disclosure Triangle
 *    rotates downwards into the "open" state. The application should
 *    repond by revealing the appropriate information or interface. On
 *    Mac OS X, a root control will be created for the window if one
 *    does not already exist. If a root control exists for the window,
 *    the Disclosure Triangle control will be embedded into it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef into which the Disclosure Triangle will be
 *      created.
 *    
 *    inBoundsRect:
 *      The desired position (in coordinates local to the window's
 *      port) for the Disclosure Triangle.
 *    
 *    inOrientation:
 *      The direction the Disclosure Triangle should point when it is
 *      "closed". Passing kControlDisclosureTrianglePointDefault is
 *      only legal as of Mac OS X and CarbonLib 1.5.
 *    
 *    inTitle:
 *      The title for the Disclosure Triangle. The title will only be
 *      displayed if the inDrawTitle parameter is true. Title display
 *      only works on Mac OS X.
 *    
 *    inInitialValue:
 *      The starting value determines whether the Disclosure Triangle
 *      is initially in its "open" or "closed" state. The value 0
 *      represents the "closed" state and 1 represents the "open" state.
 *    
 *    inDrawTitle:
 *      A Boolean indicating whether the Disclosure Triangle should
 *      draw its title next to the widget. Title display only works on
 *      Mac OS X.
 *    
 *    inAutoToggles:
 *      A Boolean indicating whether the Disclosure Triangle should
 *      change its own value (from "open" to "closed" and vice-versa)
 *      automatically when it is clicked on.
 *    
 *    outControl:
 *      On successful output, outControl will contain a reference to
 *      the Disclosure Triangle control.
 *  
 *  Result:
 *    An OSStatus code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateDisclosureTriangleControl(
  WindowRef                              inWindow,
  const Rect *                           inBoundsRect,
  ControlDisclosureTriangleOrientation   inOrientation,
  CFStringRef                            inTitle,
  SInt32                                 inInitialValue,
  Boolean                                inDrawTitle,
  Boolean                                inAutoToggles,
  ControlRef *                           outControl)          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Tagged data supported by disclosure triangles
 */
enum {

  /*
   * An SInt16 for the last value that the control had.
   */
  kControlTriangleLastValueTag  = 'last'
};

#if !__LP64__
/*
 *  SetDisclosureTriangleLastValue()
 *  
 *  Summary:
 *    Set the last value that the control had.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDisclosureTriangle:
 *      The disclosure triangle to affect.
 *    
 *    inValue:
 *      The last value.
 *  
 *  Result:
 *    An OSErr code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
SetDisclosureTriangleLastValue(
  HIViewRef   inDisclosureTriangle,
  SInt16      inValue)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIDisclosureTriangleSetDisplaysTitle()
 *  
 *  Summary:
 *    Sets whether a disclosure triangle displays its title.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDisclosureTriangle:
 *      The disclosure triangle to affect.
 *    
 *    inDisplaysTitle:
 *      A Boolean where true means "display the title" and false means
 *      "don't display the title".
 *  
 *  Result:
 *    An OSStatus code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIDisclosureTriangleSetDisplaysTitle(
  HIViewRef   inDisclosureTriangle,
  Boolean     inDisplaysTitle)                                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIDisclosureTriangleGetDisplaysTitle()
 *  
 *  Summary:
 *    Determines whether a disclosure triangle displays its title.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDisclosureTriangle:
 *      The disclosure triangle to test.
 *  
 *  Result:
 *    A Boolean indicating if the view will display the title. The
 *    incoming disclosure triangle reference must be valid or the
 *    return value is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIDisclosureTriangleGetDisplaysTitle(HIViewRef inDisclosureTriangle) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIDISCLOSUREVIEWS__ */

