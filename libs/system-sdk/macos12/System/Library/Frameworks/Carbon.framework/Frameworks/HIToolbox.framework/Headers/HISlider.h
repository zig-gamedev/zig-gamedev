/*
     File:       HIToolbox/HISlider.h
 
     Contains:   API and type definitions related to HISlider.
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISLIDER__
#define __HISLIDER__

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


/*
 *  Discussion:
 *    There are several variants that control the behavior of the
 *    slider control. Any combination of the following three constants
 *    can be added to the basic CDEF ID (kSliderProc). 
 *    
 *    Mac OS X has a "Scroll to here" option in the General pane of
 *    System Preferences which allows users to click in the page
 *    up/down regions of a slider and have the thumb/indicator jump
 *    directly to the clicked position, which alters the value of the
 *    slider and moves any associated content appropriately. As long as
 *    the mouse button is held down, the click is treated as though the
 *    user had clicked in the thumb/indicator in the first place.
 *    
 *    
 *    If you want the sliders in your application to work with the
 *    "Scroll to here" option, you must do the following: 
 *    
 *    1. Create live-tracking sliders, not sliders that show a "ghost"
 *    thumb when you click on it. You can request live-tracking sliders
 *    by passing true in the liveTracking parameter to
 *    CreateSliderControl. If you create sliders with NewControl, use
 *    the kControlSliderLiveFeedback variant. 
 *    
 *    2. Write an appropriate ControlActionProc and associate it with
 *    your slider via the SetControlAction API. This allows your
 *    application to update its content appropriately when the
 *    live-tracking slider is clicked. 
 *    
 *    3. When calling HandleControlClick or TrackControl, pass -1 in
 *    the action proc parameter. This is a request for the Control
 *    Manager to use the action proc you associated with your control
 *    in step 2. If you rely on the standard window event handler to do
 *    your control tracking, this step is handled for you automatically.
 */
enum {
  kControlSliderProc            = 48,
  kControlSliderLiveFeedback    = (1 << 0),
  kControlSliderHasTickMarks    = (1 << 1),
  kControlSliderReverseDirection = (1 << 2),
  kControlSliderNonDirectional  = (1 << 3)
};


/*
 *  ControlSliderOrientation
 *  
 *  Discussion:
 *    Slider Orientation
 */
typedef UInt16 ControlSliderOrientation;
enum {

  /*
   * If horizontal, the thumb of the slider points downwards. If
   * vertical, the thumb of the slider points to the right. This
   * orientation of slider can show tick marks.
   */
  kControlSliderPointsDownOrRight = 0,

  /*
   * If horizontal, the thumb of the slider points upwards. If
   * vertical, the thumb of the slider points to the left. This
   * orientation of slider can show tick marks.
   */
  kControlSliderPointsUpOrLeft  = 1,

  /*
   * The slider thumb is non-directional. This orientation of slider is
   * not capable of showing tick marks.
   */
  kControlSliderDoesNotPoint    = 2
};

/* Control Kind Tag */
enum {
  kControlKindSlider            = 'sldr'
};

/* The HIObject class ID for the HISlider class. */
#define kHISliderClassID                CFSTR("com.apple.HISlider")
#if !__LP64__
/*
 *  CreateSliderControl()
 *  
 *  Summary:
 *    Creates a slider control with the specified settings.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window in which to create the slider.
 *    
 *    boundsRect:
 *      The bounds with which to create the slider.
 *    
 *    value:
 *      The initial value of the slider. Should be in the range of the
 *      minimum and maximum values.
 *    
 *    minimum:
 *      The minimum value of the slider. Should be less than the
 *      maximum.
 *    
 *    maximum:
 *      The maximum value of the slider. Should be greater than the
 *      minimum.
 *    
 *    orientation:
 *      The orientation of the slider.
 *    
 *    numTickMarks:
 *      The number of tick marks of the slider. The orientation must
 *      not support tick marks if this value is non-zero.
 *    
 *    liveTracking:
 *    
 *    liveTrackingProc:
 *    
 *    outControl:
 *      A pointer to a ControlRef in which to store a reference to the
 *      created slider.
 *  
 *  Result:
 *    An operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateSliderControl(
  WindowRef                  window,
  const Rect *               boundsRect,
  SInt32                     value,
  SInt32                     minimum,
  SInt32                     maximum,
  ControlSliderOrientation   orientation,
  UInt16                     numTickMarks,
  Boolean                    liveTracking,
  ControlActionUPP           liveTrackingProc,
  ControlRef *               outControl)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HISliderGetThumbOrientation()
 *  
 *  Summary:
 *    Get the orientation of the thumb of a slider.
 *  
 *  Discussion:
 *    Available only on Mac OS X 10.5 and later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSlider:
 *      A reference to the slider from which to retrieve the thumb
 *      orientation.
 *  
 *  Result:
 *    The current thumb orientation of the specified slider. The
 *    incoming slider reference must be valid or the return value is
 *    undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern ControlSliderOrientation 
HISliderGetThumbOrientation(HIViewRef inSlider)               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HISliderSetThumbOrientation()
 *  
 *  Summary:
 *    Set the orientation of the thumb of a slider.
 *  
 *  Discussion:
 *    Available only on Mac OS X 10.5 and later. This call may change
 *    the bounds needed to draw the slider fully. If changing the
 *    orientation so that tick marks will appear/disappear, be sure to
 *    resize the control to its optimal size if necessary.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSlider:
 *      A reference to the slider from which to retrieve the thumb
 *      orientation.
 *    
 *    inOrientation:
 *      A ControlSliderOrientation for the desired thumb orientation.
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
HISliderSetThumbOrientation(
  HIViewRef                  inSlider,
  ControlSliderOrientation   inOrientation)                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HISliderGetTickMarkCount()
 *  
 *  Summary:
 *    Get the tick mark count of a slider.
 *  
 *  Discussion:
 *    Available only on Mac OS X 10.5 and later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSlider:
 *      A reference to the slider for which to get the tick mark count.
 *  
 *  Result:
 *    The current tick mark count of the specified slider. The incoming
 *    slider reference must be valid or the return value is undefined.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
HISliderGetTickMarkCount(HIViewRef inSlider)                  AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HISliderSetTickMarkCount()
 *  
 *  Summary:
 *    Set the tick mark count of a slider. Tick marks will not appear
 *    on a slider unless its orientation is pointy (i.e.
 *    kControlSliderPointsDownOrRight/UpOrLeft) This call may change
 *    the bounds needed to draw the slider fully. If changing the tick
 *    mark count so that tick marks will appear/disappear, be sure to
 *    resize the control to its optimal size if necessary.
 *  
 *  Discussion:
 *    Available only on Mac OS X 10.5 and later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSlider:
 *      A reference to the slider for which to set the tick mark count.
 *    
 *    inNumTicks:
 *      The desired number of tick marks.
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
HISliderSetTickMarkCount(
  HIViewRef   inSlider,
  ItemCount   inNumTicks)                                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HISLIDER__ */

