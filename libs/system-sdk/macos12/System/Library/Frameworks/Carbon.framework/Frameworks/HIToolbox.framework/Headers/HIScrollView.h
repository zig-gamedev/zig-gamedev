/*
     File:       HIToolbox/HIScrollView.h
 
     Contains:   Definition of the scrollbar and scroll views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HISCROLLVIEW__
#define __HISCROLLVIEW__

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
 *  HIScrollView.h
 *  
 *  Discussion:
 *    API definitions for the scrollbar and scroll views.
 */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • SCROLL BAR (CDEF 24)                                                              */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  This is the new Appearance scroll bar.                                              */
/*                                                                                      */
/*  Mac OS X has a "Scroll to here" option in the General pane of System Preferences    */
/*  which allows users to click in the page up/down regions of a scroll bar and have    */
/*  the thumb/indicator jump directly to the clicked position, which alters the value   */
/*  of the scroll bar and moves the scrolled content appropriately. As long as the      */
/*  mouse button is held down, the click is treated as though the user had clicked in   */
/*  the thumb/indicator in the first place.                                             */
/*                                                                                      */
/*  If you want the scroll bars in your application to work with the "Scroll to here"   */
/*  option, you must do the following:                                                  */
/*                                                                                      */
/*  1. Create live-tracking scroll bars, not scroll bars that show a "ghost" thumb      */
/*  when you click on it. You can request live-tracking scroll bars by passing true     */
/*  in the liveTracking parameter to CreateScrollBarControl. If you create scroll bars  */
/*  with NewControl, use the kControlScrollBarLiveProc.                                 */
/*                                                                                      */
/*  2. Write an appropriate ControlActionProc and associate it with your scroll bar     */
/*  via the SetControlAction API. This allows your application to update its content    */
/*  appropriately when the live-tracking scroll bar is clicked.                         */
/*                                                                                      */
/*  3. When calling HandleControlClick or TrackControl, pass -1 in the action proc      */
/*  parameter. This is a request for the Control Manager to use the action proc you     */
/*  associated with your control in step 2. If you rely on the standard window event    */
/*  handler to do your control tracking, this step is handled for you automatically.    */
/*                                                                                      */
/* Theme Scroll Bar proc IDs */
enum {
  kControlScrollBarProc         = 384,  /* normal scroll bar*/
  kControlScrollBarLiveProc     = 386   /* live scrolling variant*/
};

/* Control Kind Tag */
enum {
  kControlKindScrollBar         = 'sbar'
};

/* The HIObject class ID for the HIScrollBar class. */
#define kHIScrollBarClassID             CFSTR("com.apple.HIScrollBar")
#if !__LP64__
/*
 *  CreateScrollBarControl()
 *  
 *  Summary:
 *    Creates a scroll bar control.
 *  
 *  Discussion:
 *    This creation API is available in Carbon only.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window that should contain the control. May be NULL on 10.3
 *      and later.
 *    
 *    boundsRect:
 *      The bounding box of the control.
 *    
 *    value:
 *      The initial value of the control.
 *    
 *    minimum:
 *      The minimum value of the control.
 *    
 *    maximum:
 *      The maximum value of the control.
 *    
 *    viewSize:
 *      The size of the visible area of the scroll bar content.
 *    
 *    liveTracking:
 *      A Boolean indicating whether or not live tracking is enabled
 *      for this scroll bar. If set to true and a valid
 *      liveTrackingProc is also passed in, the callback will be called
 *      repeatedly as the thumb is moved during tracking.  If set to
 *      false, a semi-transparent thumb called a "ghost thumb" will
 *      draw and no live tracking will occur.
 *    
 *    liveTrackingProc:
 *      If liveTracking is on, a ControlActionUPP callback to be called
 *      as the control live tracks.  This callback is called repeatedly
 *      as the scroll thumb is moved during tracking.
 *    
 *    outControl:
 *      On exit, contains the new control.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateScrollBarControl(
  WindowRef          window,
  const Rect *       boundsRect,
  SInt32             value,
  SInt32             minimum,
  SInt32             maximum,
  SInt32             viewSize,
  Boolean            liveTracking,
  ControlActionUPP   liveTrackingProc,
  ControlRef *       outControl)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* These tags are available in Mac OS X or later */
#endif  /* !__LP64__ */

enum {
  kControlScrollBarShowsArrowsTag = 'arro' /* Boolean whether or not to draw the scroll arrows*/
};


/*==============================================================================*/
/*  HIScrollView                                                                */
/*                                                                              */
/* The scroll view is a new view starting in Mac OS 10.2. It can be used in     */
/* the new compositing mode ONLY due to the nature of how it works. Like all    */
/* new HIFoo views, this view is created invisibly. You must show the view      */
/* after creation if you want to, like, see it and stuff.                       */
/* The HIScrollView will set the frame of the contained view when its bounds    */
/* change, so it is not necessary to set up the layout of the embedded view.    */
/*                                                                              */
/* Using an HIScrollView requires a few steps:                                  */
/*                                                                              */
/* 1. Install your scrollable content view into the HIScrollView instance using */
/*    HIViewAddSubview.                                                         */
/*                                                                              */
/* 2. If the scrollable content view doesn't already handle the                 */
/*    kEventScrollableGetInfo and kEventScrollableScrollTo events, you must     */
/*    install handlers on your scrollable content view and handle those events  */
/*    manually. More details on those events can be found below.                */
/*                                                                              */
/* 3. If the scrollable content view doesn't already send out the               */
/*    kEventScrollableInfoChanged event to its parent view, you must send this  */
/*    event to the HIScrollView instance whenever your scrollable content       */
/*    view's size or origin changes. More details on this event can be found    */
/*    below.                                                                    */
/*==============================================================================*/
/* The HIObject class ID for the HIScrollView class. */
#define kHIScrollViewClassID            CFSTR("com.apple.HIScrollView")
/* Control Kind*/
enum {
  kControlKindHIScrollView      = 'scrl'
};

/*
    kEventClassScrollable quick reference:
    
    kEventScrollableGetInfo         = 1,
    kEventScrollableInfoChanged     = 2,
    kEventScrollableScrollTo        = 10
*/
enum {
  kEventClassScrollable         = 'scrl'
};


/*
 */
enum {

  /*
   * The image size is the total size of the scrollable view, including
   * any parts of the view that are not currently visible. For example,
   * a scrollable view that displays a 100-page document would return a
   * horizontal size equal to the width of a page and a vertical image
   * size equal to 100 times the height of a page.
   */
  kEventParamImageSize          = 'imsz', /* typeHISize*/

  /*
   * The view size is the current size of the scrollable view.
   * Typically, this is the same as the view's bounds, and can be
   * acquired by calling HIViewGetBounds.
   */
  kEventParamViewSize           = 'vwsz', /* typeHISize*/

  /*
   * The line size is the distance that the HIScrollView should scroll
   * its subview when the user clicks the scroll bar arrows. For
   * example, this might be 10 pixels vertically and 20 pixels
   * horizontally.
   */
  kEventParamLineSize           = 'lnsz', /* typeHISize*/

  /*
   * The origin is the current view-relative origin with the total
   * scrollable image that is displayed at the top left corner of the
   * view. These coordinates should always be greater than or equal to
   * zero. They should be less than or equal to the view’s image size
   * minus its view size. Typically, a view that implements the
   * kEventScrollableScrollTo event by calling HIViewSetBoundsOrigin
   * will return the current bounds origin for this parameter, and a
   * view that implements the ScrollTo event by storing the origin in
   * its instance data will return its stored origin for this
   * parameter. For example, a scrollable view that is currently
   * displaying page 10 of a 100-page document would return a
   * horizontal origin of zero and a vertical origin equal to 10 times
   * the height of a page.
   */
  kEventParamOrigin             = 'orgn' /* typeHIPoint*/
};

/*
 *  kEventClassScrollable / kEventScrollableGetInfo
 *  
 *  Summary:
 *    Requests information from an HIScrollView’s scrollable view about
 *    its size and origin.
 *  
 *  Discussion:
 *    This event is sent by an HIScrollView to its scrollable view to
 *    determine the current size and origin of the scrollable view. A
 *    scrollable view must implement this event in order to scroll
 *    properly inside an HIScrollView. This event is sent only to the
 *    view, and is not propagated past it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    <-- kEventParamImageSize (out, typeHISize)
 *          On exit, contains the size of the entire scrollable view.
 *    
 *    <-- kEventParamViewSize (out, typeHISize)
 *          On exit, contains the amount of the scrollable view that is
 *          visible.
 *    
 *    <-- kEventParamLineSize (out, typeHISize)
 *          On exit, contains the amount that should be scrolled in
 *          response to a single click on a scrollbar arrow.
 *    
 *    <-- kEventParamOrigin (out, typeHIPoint)
 *          On exit, contains the scrollable view’s current origin.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventScrollableGetInfo       = 1
};

/*
 *  kEventClassScrollable / kEventScrollableInfoChanged
 *  
 *  Summary:
 *    Notification that the size or origin of an HIScrollView’s
 *    scrollable view has changed.
 *  
 *  Discussion:
 *    This event is not sent by HIScrollView itself; rather, it may be
 *    sent to an instance of HIScrollView to notify the scroll view
 *    that the size or origin of its scrollable view have changed. The
 *    HIScrollView responds to this event by sending a
 *    kEventScrollableGetInfo to its scrollable view. It then updates
 *    the scroll bars appropriately to reflect the new reality of the
 *    scrollable view. It does NOT move the origin of the scrollable
 *    view at all. It is just a notification to allow the scroll view
 *    to sync up with its scrollable view.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventScrollableInfoChanged   = 2
};

/*
 *  kEventClassScrollable / kEventScrollableScrollTo
 *  
 *  Summary:
 *    Requests that an HIScrollView’s scrollable view should scroll to
 *    a particular origin.
 *  
 *  Discussion:
 *    This event is sent by an HIScrollView to its scrollable view to
 *    request that the scrollable view update its current origin and
 *    redraw. Typically, a scrollable view scrolls its content either
 *    by setting its bounds origin with HIViewSetBoundsOrigin or by
 *    offsetting its drawing by the scroll origin. If the view embeds
 *    other views, it must use HIViewSetBoundsOrigin to allow the
 *    embedded views to scroll along with their containing view. A view
 *    that uses HIViewSetBoundsOrigin should call that API in response
 *    to this event; a view that offsets its drawing by the scroll
 *    origin should update its current origin in its own instance data
 *    in response to this event. A scrollable view should also use
 *    either HIViewScrollRect to scroll its content, or
 *    HIViewSetNeedsDisplay to cause itself to redraw using the new
 *    origin point. A scrollable view must implement this event in
 *    order to scroll properly inside an HIScrollView.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    --> kEventParamOrigin (in, typeHIPoint)
 *          The new origin for the scrollable view. The origin
 *          coordinates will vary from (0,0) to the scrollable view’s
 *          image size minus its view size.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework
 *    CarbonLib:        not available
 */
enum {
  kEventScrollableScrollTo      = 10
};


/*
 *  Summary:
 *    HIScrollView options
 */
enum {

  /*
   * This indicates that a vertical scroll bar is desired.
   */
  kHIScrollViewOptionsVertScroll = (1 << 0),

  /*
   * This indicates that a horizontal scroll bar is desired.
   */
  kHIScrollViewOptionsHorizScroll = (1 << 1),

  /*
   * This indicates that space for a grow box should be taken into
   * account when laying out scroll bars. On Mac OS X 10.3 and earlier,
   * if both the horizontal and vertical scroll bars are requested,
   * this attribute is assumed. On Mac OS X 10.4 and later, this
   * attribute is *NOT* assumed; this allows the scroll view to support
   * auto-hiding of the two scroll bars independently on Mac OS X 10.4
   * and later. If you want to preserve space for the grow box on all
   * systems, specify this option bit.
   */
  kHIScrollViewOptionsAllowGrow = (1 << 2),

  /*
   * This indicates that if the grow area is visible, then the scroll
   * view should fill it with white. Available in Mac OS X 10.5 and
   * later.
   */
  kHIScrollViewOptionsFillGrowArea = (1 << 3),

  /*
   * Indicates that the scroll view should never use smooth scrolling,
   * overriding the user's preference. Applications should rarely use
   * this option, but it may be appropriate for certain cases.
   */
  kHIScrollViewOptionsDisableSmoothScrolling = 1 << 4,
  kHIScrollViewValidOptions     = (kHIScrollViewOptionsVertScroll | kHIScrollViewOptionsHorizScroll | kHIScrollViewOptionsAllowGrow | kHIScrollViewOptionsFillGrowArea | kHIScrollViewOptionsDisableSmoothScrolling)
};


/*
 *  HIScrollViewAction
 *  
 *  Summary:
 *    HIScrollView navigation actions. See HIScrollViewNavigate for
 *    more information.
 */
typedef UInt32 HIScrollViewAction;
enum {

  /*
   * The scroll view should move to the top of the content.
   */
  kHIScrollViewScrollToTop      = (1 << 0),

  /*
   * The scroll view should move to the bottom of the content.
   */
  kHIScrollViewScrollToBottom   = (1 << 1),

  /*
   * The scroll view should move to the left of the content.
   */
  kHIScrollViewScrollToLeft     = (1 << 2),

  /*
   * The scroll view should move to the right of the content.
   */
  kHIScrollViewScrollToRight    = (1 << 3),

  /*
   * The scroll view should page up.
   */
  kHIScrollViewPageUp           = (1 << 4),

  /*
   * The scroll view should page down.
   */
  kHIScrollViewPageDown         = (1 << 5),

  /*
   * The scroll view should page left.
   */
  kHIScrollViewPageLeft         = (1 << 6),

  /*
   * The scroll view should page right.
   */
  kHIScrollViewPageRight        = (1 << 7)
};

#if !__LP64__
/*
 *  HIScrollViewCreate()
 *  
 *  Discussion:
 *    Creates a scroll view. This view has 3 parts, essentially. It can
 *    have one or two scroll bars (horizontal/vertical), and a view to
 *    be scrolled. The view to be scrolled is merely added via
 *    HIViewAddSubview. The scroll view will automatically connect it
 *    up appropriately. By default, the HIScrollView will not
 *    automatically hide the scroll bars if the content fits within the
 *    scrollable view. Use the HIScrollViewSetScrollBarAutoHide API to
 *    enable that feature.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOptions:
 *      Options for our scroll view. You must specify either a
 *      horizontal or a vertical scroll bar. If neither is passed, an
 *      error is returned.
 *    
 *    outView:
 *      The new scroll view.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIScrollViewCreate(
  OptionBits   inOptions,
  HIViewRef *  outView)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIScrollViewSetScrollBarAutoHide()
 *  
 *  Discussion:
 *    Sets a scroll view's scroll bars to auto-hide when the entire
 *    scrollable view it is managing can be fully displayed in its
 *    bounds. This is similar to the behavior you see in the Preview
 *    application.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The view to affect.
 *    
 *    inAutoHide:
 *      The new auto-hide setting (true == auto-hide).
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIScrollViewSetScrollBarAutoHide(
  HIViewRef   inView,
  Boolean     inAutoHide)                                     AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIScrollViewGetScrollBarAutoHide()
 *  
 *  Discussion:
 *    Gets a scroll view's current scroll bar auto-hide setting.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The view to examine.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIScrollViewGetScrollBarAutoHide(HIViewRef inView)            AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIScrollViewNavigate()
 *  
 *  Discussion:
 *    Allows you to programmatically change what portion of a scroll
 *    view's target you are seeing. For example, you can move to the
 *    beginning or end of a document. You can also page up, down, left
 *    and right. In general, you should not call this from embedded
 *    content (i.e. the scrollable view inside the scroll view). For
 *    those cases, you should instead position yourself appropriately
 *    and tell the scroll view you changed via the
 *    kEventScrollableInfoChanged carbon event. This routine merely is
 *    a programmatic way to scroll as one would by hand using the
 *    scroll bars.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The scroll view to affect.
 *    
 *    inAction:
 *      The action to take.
 *  
 *  Result:
 *    A operating system status code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIScrollViewNavigate(
  HIViewRef            inView,
  HIScrollViewAction   inAction)                              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  HIScrollViewCanNavigate()
 *  
 *  Discussion:
 *    Allows you to tell whether it is currently possible to navigate
 *    somehow in a scroll view. For example, if a scroll view is
 *    already at the top of the scrollable content, it is not possible
 *    to navigate upward, so home and page up actions would not be
 *    possible. You might use this function to help you update the
 *    state of menu items or the like.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inView:
 *      The view to examine.
 *    
 *    inAction:
 *      The action to test.
 *  
 *  Result:
 *    A boolean result.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIScrollViewCanNavigate(
  HIViewRef            inView,
  HIScrollViewAction   inAction)                              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HISCROLLVIEW__ */

