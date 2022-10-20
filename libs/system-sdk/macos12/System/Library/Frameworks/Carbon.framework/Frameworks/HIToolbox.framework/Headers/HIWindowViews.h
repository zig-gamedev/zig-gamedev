/*
     File:       HIToolbox/HIWindowViews.h
 
     Contains:   Definition of the window-related views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIWINDOWVIEWS__
#define __HIWINDOWVIEWS__

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

#ifndef __HIVIEW__
#include <HIToolbox/HIView.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  HIWindowViews.h
 *  
 *  Discussion:
 *    API definitions for the window-related views.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* ¥ HIViewIDs for subviews of a window frame view                                      */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*
 *  kHIViewWindowContentID
 *  
 *  Discussion:
 *    The standard view ID for the content view of a window. Note that
 *    a window's content view should not be moved from its original
 *    container window into any other window; this will cause the
 *    content view to behave incorrectly. If you need to move all of
 *    the views from one window into another, embed a user pane into
 *    the content view and them embed all other views into the user
 *    pane, and move the user pane from one window to another.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowContentID                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;
/*
 *  kHIViewWindowCloseBoxID
 *  
 *  Discussion:
 *    The standard view ID for the close box view of a window. Not all
 *    windows have close boxes, so be aware that you might not find
 *    this view if you look for it. This variable is only exported on
 *    Mac OS X 10.5 and later, but the close box view uses this view ID
 *    on Mac OS X 10.2 and later. If you need to access the view on an
 *    earlier version of Mac OS X, you can safely initialize your own
 *    view ID with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowCloseBoxID                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowCollapseBoxID
 *  
 *  Discussion:
 *    The standard view ID for the collapse box view of a window. Not
 *    all windows have collapse boxes, so be aware that you might not
 *    find this view if you look for it. This variable is only exported
 *    on Mac OS X 10.5 and later, but the collapse box view uses this
 *    view ID on Mac OS X 10.2 and later. If you need to access the
 *    view on an earlier version of Mac OS X, you can safely initialize
 *    your own view ID with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowCollapseBoxID                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowZoomBoxID
 *  
 *  Discussion:
 *    The standard view ID for the zoom box view of a window. Not all
 *    windows have zoom boxes, so be aware that you might not find this
 *    view if you look for it. This variable is only exported on Mac OS
 *    X 10.5 and later, but the zoom box view uses this view ID on Mac
 *    OS X 10.2 and later. If you need to access the view on an earlier
 *    version of Mac OS X, you can safely initialize your own view ID
 *    with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowZoomBoxID                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowToolbarButtonID
 *  
 *  Discussion:
 *    The standard view ID for the toolbar button view of a window. Not
 *    all windows have toolbar buttons, so be aware that you might not
 *    find this view if you look for it. This variable is only exported
 *    on Mac OS X 10.5 and later, but the toolbar button view uses this
 *    view ID on Mac OS X 10.2 and later. If you need to access the
 *    view on an earlier version of Mac OS X, you can safely initialize
 *    your own view ID with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowToolbarButtonID                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowTitleID
 *  
 *  Discussion:
 *    The standard view ID for the title view of a window. Not all
 *    windows have titles, so be aware that you might not find this
 *    view if you look for it. This variable is only exported on Mac OS
 *    X 10.5 and later, but the title view uses this view ID on Mac OS
 *    X 10.2 and later. If you need to access the view on an earlier
 *    version of Mac OS X, you can safely initialize your own view ID
 *    with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowTitleID                           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowToolbarID
 *  
 *  Discussion:
 *    The standard view ID for the toolbar view of a window. Not all
 *    windows have toolbars, so be aware that you might not find this
 *    view if you look for it. This variable is only exported on Mac OS
 *    X 10.5 and later, but the toolbar view uses this view ID on Mac
 *    OS X 10.2 and later. If you need to access the view on an earlier
 *    version of Mac OS X, you can safely initialize your own view ID
 *    with the same contents as this constant.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowToolbarID                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
/*
 *  kHIViewWindowGrowBoxID
 *  
 *  Discussion:
 *    The standard view ID for the grow box view of a window. Not all
 *    windows have grow boxes, so be aware that you might not find this
 *    view if you look for it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern const HIViewID kHIViewWindowGrowBoxID                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* ¥ HIView part codes for window frame views                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/

/*
 *  Summary:
 *    HIView part codes used by window frame views.
 *  
 *  Discussion:
 *    These part codes are used by an HIView that implements the frame
 *    of a window. They may be used with the standard document windows
 *    provided by the Window Manager. A custom window frame view may
 *    optionally (but is not required to) implement these part codes in
 *    its event handlers for kEventControlGetPartRegion/Bounds.
 */
enum {

  /*
   * Identifies the titlebar part of a window frame view. This partcode
   * is used by GetWindowBounds and GetWindowRegion when called with
   * kWindowTitleBarRgn.
   */
  kHIWindowTitleBarPart         = 2,

  /*
   * Identifies the draggable part of a window frame view. This
   * partcode is used by GetWindowBounds and GetWindowRegion when
   * called with kWindowDragRgn.
   */
  kHIWindowDragPart             = 3,

  /*
   * Identifies the proxy icon part of a window frame title view. The
   * title view is a subview of the window frame view and is identified
   * by an HIViewID of kHIViewWindowTitleID. This partcode is _not_
   * used by the window frame view itself, but only by the title view.
   * This partcode is used by GetWindowBounds and GetWindowRegion when
   * called with kWindowTitleProxyIconRgn.
   */
  kHIWindowTitleProxyIconPart   = 2
};

/*==============================================================================*/
/* HIGrowBoxView                                                                */
/*                                                                              */
/* The grow box view is a new view starting in Mac OS 10.2. It can be used in   */
/* both the new compositing mode, as well as the traditional Control Manager    */
/* mode. Like all new HIFoo views, this view is created invisibly. You must     */
/* show the view after creation if you want to, like, see it and stuff.         */
/*==============================================================================*/
/* The HIObject class ID for the HIGrowBoxView class. */
#define kHIGrowBoxViewClassID           CFSTR("com.apple.higrowboxview")
/* Control Kind*/
enum {
  kControlKindHIGrowBoxView     = 'grow'
};

/* Currently there is no direct creation API for the grow box, so you must use  */
/* HIObjectCreate if you wish to create one directly. Normally, a window will   */
/* create one for you, so you should generally never need to do this.           */
#if !__LP64__
/*
 *  HIGrowBoxViewSetTransparent()
 *  
 *  Discussion:
 *    Sets a grow box view as transparent, meaning it will draw the
 *    grow box lines over any content below it. When not transparent,
 *    it's an opaque white square with the grow lines.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGrowBoxView:
 *      The grow box view reference.
 *    
 *    inTransparent:
 *      Pass true to make the grow view use its transparent look, false
 *      to give it the opaque look.
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
HIGrowBoxViewSetTransparent(
  HIViewRef   inGrowBoxView,
  Boolean     inTransparent)                                  AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIGrowBoxViewIsTransparent()
 *  
 *  Discussion:
 *    Returns true if a grow box view is set to be transparent.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGrowBoxView:
 *      The grow box view reference.
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
HIGrowBoxViewIsTransparent(HIViewRef inGrowBoxView)           AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HIWINDOWVIEWS__ */

