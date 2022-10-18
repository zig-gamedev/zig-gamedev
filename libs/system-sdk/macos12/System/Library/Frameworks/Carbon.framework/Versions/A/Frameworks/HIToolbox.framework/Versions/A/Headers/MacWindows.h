/*
     File:       HIToolbox/MacWindows.h
 
     Contains:   Window Manager Interfaces
 
     Copyright:  © 1997-2008 by Apple Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACWINDOWS__
#define __MACWINDOWS__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __APPEARANCE__
#include <HIToolbox/Appearance.h>
#endif

#ifndef __DRAG__
#include <HIToolbox/Drag.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif

#ifndef __HITOOLBAR__
#include <HIToolbox/HIToolbar.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/*                                                                                          */
/* Current documentation for the Mac OS X Window Manager is available on the web:           */
/*  <http://developer.apple.com/documentation/Carbon/Reference/Window_Manager/index.html>   */
/*                                                                                          */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • HIWindowRef                                                                        */
/*——————————————————————————————————————————————————————————————————————————————————————*/
typedef WindowRef                       HIWindowRef;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Property Types                                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
typedef OSType                          PropertyCreator;
typedef OSType                          PropertyTag;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Classes                                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/

/*
 *  WindowClass
 *  
 *  Summary:
 *    The types of windows provided by the Window Manager.
 *  
 *  Discussion:
 *    The class of a window determines several aspects of the window:
 *    its appearance, its initial window attributes, its initial window
 *    group, and its initial modality. All of these except for the
 *    window's appearance may be changed later using other Window
 *    Manager APIs.
 */
typedef UInt32 WindowClass;
enum {

  /*
   * An alert window is used when the application needs the user's
   * attention immediately. On Mac OS 9 and earlier, a visible alert
   * window will prevent the user from switching to any other
   * application. Use kThemeBrushAlertBackgroundActive to draw the
   * background of alert windows. 
   * 
   * Alert windows support compositing mode in Mac OS X 10.5 and later.
   * 
   * 
   * Alert windows are initially placed in the modal window group,
   * given a modality of kWindowModalityAppModal, and given an
   * activation scope of kWindowActivationScopeAll. Available in Mac OS
   * 8.5 and later.
   */
  kAlertWindowClass             = 1,

  /*
   * Similar to kAlertWindowClass, but provides a window that is
   * movable and that allows switching to other applications. Generally
   * you should use this window class rather than kAlertWindowClass.
   * Use kThemeBrushAlertBackgroundActive to draw the background of
   * movable alert windows. 
   * 
   * Movable alert windows support compositing mode in Mac OS X 10.2
   * and later. 
   * 
   * Movable alert windows are initially placed in the modal window
   * group, given a modality of kWindowModalityAppModal, and given an
   * activation scope of kWindowActivationScopeAll. Available in Mac OS
   * 8.5 and later.
   */
  kMovableAlertWindowClass      = 2,

  /*
   * A modal window is used to display a dialog (but not an alert; use
   * kAlertWindowClass for alerts). On Mac OS 9 and earlier, a visible
   * modal window will prevent the user from switching to any other
   * application. Use kThemeBrushDialogBackgroundActive to draw the
   * background of modal windows. 
   * 
   * Modal windows support compositing mode in Mac OS X 10.5 and later.
   * 
   * 
   * Modal windows are initially placed in the modal window group,
   * given a modality of kWindowModalityAppModal, and given an
   * activation scope of kWindowActivationScopeAll. Available in Mac OS
   * 8.5 and later.
   */
  kModalWindowClass             = 3,

  /*
   * Similar to kModalWindowClass, but provides a window that is
   * movable and that allows switching to other applications. Generally
   * you should use this window class rather than kModalWindowClass. If
   * you are using CarbonLib 1.3 or later, or Mac OS X, use
   * kThemeBrushMovableModalBackground to draw the background of
   * movable modal windows; on earlier versions of CarbonLib, or for
   * non-Carbon programming, use kThemeBrushDialogBackgroundActive.
   * 
   * 
   * Movable modal windows support compositing mode in Mac OS X 10.2
   * and later. 
   * 
   * Movable modal windows are initially placed in the modal window
   * group, given a modality of kWindowModalityAppModal, and given an
   * activation scope of kWindowActivationScopeAll. Available in Mac OS
   * 8.5 and later.
   */
  kMovableModalWindowClass      = 4,

  /*
   * A floating window is layered above all application windows except
   * for alert and modal windows. It is commonly used for palette
   * windows, inspectors, and other accessory (non-document) windows.
   * Use kThemeBrushUtilityWindowBackgroundActive or
   * kThemeBrushDocumentWindowBackground to draw the background of
   * floating windows. 
   * 
   * Floating windows support compositing mode in Mac OS X 10.2 and
   * later. 
   * 
   * Floating windows are initially placed in the floating window
   * group, given a modality of kWindowModalityNone, and given an
   * activation scope of kWindowActivationScopeIndependent. Available
   * in Mac OS 8.6 and later.
   */
  kFloatingWindowClass          = 5,

  /*
   * A document window is used for normal application document windows.
   * Use kThemeBrushDocumentWindowBackground or your own custom drawing
   * to draw the background of a document window. 
   * 
   * Document windows support compositing mode in Mac OS X 10.2 and
   * later. 
   * 
   * Document windows are initially placed in the document window
   * group, given a modality of kWindowModalityNone, and given an
   * activation scope of kWindowActivationScopeAll. Available in Mac OS
   * 8.5 and later.
   */
  kDocumentWindowClass          = 6,

  /*
   * A utility window is similar to a floating window, but it floats
   * above the windows of all applications rather than just above the
   * windows of the application that creates it. Use
   * kThemeBrushUtilityWindowBackgroundActive or
   * kThemeBrushDocumentWindowBackground to draw the background of
   * utility windows. 
   * 
   * Utility windows support compositing mode in Mac OS X 10.2 and
   * later. 
   * 
   * Utility windows are initially placed in the utility window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeIndependent. Available in CarbonLib
   * 1.1 and later, and in Mac OS X.
   */
  kUtilityWindowClass           = 8,

  /*
   * A help window is used to display help tags. It has no window
   * frame. Typically you should use the Help Manager to display help
   * tags, rather than creating a help tag window yourself. 
   * 
   * Help windows support compositing mode in Mac OS X 10.3 and later.
   * 
   * 
   * Help windows are initially placed in the help window group, given
   * a modality of kWindowModalityNone, and given an activation scope
   * of kWindowActivationScopeNone. Available in CarbonLib 1.1 and
   * later, and in Mac OS X.
   */
  kHelpWindowClass              = 10,

  /*
   * A sheet window is used to display a dialog that is attached
   * directly to its parent window, rather than being a independent
   * window. A sheet dialog only prevents interaction with its parent
   * window; the user may still interact with other windows in the same
   * application. Use kThemeBrushSheetBackgroundOpaque to draw an
   * opaque background for sheet windows, or
   * kThemeBrushSheetBackgroundTransparent to draw a transparent
   * background (the transparent brush is only available in Mac OS X
   * 10.1 and later). 
   * 
   * Sheet windows support compositing mode in Mac OS X 10.2 and later.
   * 
   * 
   * Sheet windows are initially placed in the document window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeAll. Available in CarbonLib 1.3 and
   * later, and in Mac OS X; in CarbonLib, a sheet window is actually a
   * movable-modal window, which blocks user interaction with all
   * windows of the application.
   */
  kSheetWindowClass             = 11,

  /*
   * A toolbar window is used to display a palette of controls. It is
   * similar to a floating window, and like a floating window is
   * layered above all application windows except for alert and modal
   * windows, but is layered beneath floating windows. Use
   * kThemeBrushToolbarBackground to draw the background of a toolbar
   * window in Mac OS X or later; CarbonLib does not currently support
   * kThemeBrushToolbarBackground. 
   * 
   * Toolbar windows support compositing mode in Mac OS X 10.3 and
   * later. 
   * 
   * Toolbar windows are initially placed in the toolbar window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeNone. Available in CarbonLib 1.1
   * and later, and Mac OS X.
   */
  kToolbarWindowClass           = 12,

  /*
   * A plain window has a single-pixel window frame.
   * kThemeBrushDocumentWindowBackground,
   * kThemeBrushDialogBackgroundActive, and application-specific custom
   * drawing are all commonly used to draw the background of a plain
   * window. 
   * 
   * Plain windows support compositing mode on Mac OS X 10.3 and later.
   * 
   * 
   * Plain windows are initially placed in the document window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeAll. Available in CarbonLib 1.2.5
   * and later, and Mac OS X.
   */
  kPlainWindowClass             = 13,

  /*
   * An overlay window is a completely transparent window positioned
   * above all other windows. Overlay windows are intended as a
   * replacement for the pre-Carbon practice of drawing directly into
   * the window manager port; by creating a full-screen overlay window
   * and drawing into it, you can draw over any window in any
   * application without disturbing the contents of the windows
   * underneath your drawing. 
   * 
   * Overlay windows have a default handler for kEventWindowPaint that
   * uses CGContextClearRect to clear the overlay window's alpha
   * channel to zero. This ensures the initial transparency of the
   * window. You can install your own kEventWindowPaint handler to do
   * your own drawing; typically, you would call through to the default
   * handler with CallNextEventHandler first, and then use
   * QDBeginCGContext to create your own context for drawing. You can
   * use either QuickDraw or CoreGraphics to draw into an overlay
   * window, but you must use CoreGraphics to draw if you need any of
   * your drawing to be non-opaque, since QuickDraw always sets the
   * alpha channel of any pixels that it touches to 1.0. 
   * 
   * Overlay windows support compositing mode on Mac OS X 10.3 and
   * later. You should draw into a compositing overlay window by adding
   * a standard or custom HIView and implementing the kEventControlDraw
   * event for your view; do not use kEventWindowPaint. 
   * 
   * You can also use the standard window event handler together with
   * regular controls in an overlay window. When using the standard
   * window event handler, you will probably want your
   * kEventWindowPaint handler to return eventNotHandledErr (after
   * calling the default handler with CallNextEventHandler first) so
   * that after the Paint handler returns, the Window Manager will send
   * a kEventWindowDrawContent event which the standard window event
   * handler can respond to by drawing the controls in the window.
   * 
   * 
   * Overlay windows are initially placed in the overlay window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeNone. Available in Mac OS X.
   */
  kOverlayWindowClass           = 14,

  /*
   * A sheet alert window is similar to a sheet window, but is intended
   * to display an alert rather than a dialog. On Mac OS X, the
   * appearance of a sheet window and a sheet alert window is currently
   * identical, but in CarbonLib a sheet alert window has a different
   * appearance from a sheet window. Use
   * kThemeBrushSheetBackgroundOpaque to draw an opaque background for
   * sheet alert windows, or kThemeBrushSheetBackgroundTransparent to
   * draw a transparent background (the transparent brush is only
   * available in Mac OS X 10.1 and later). 
   * 
   * Sheet alert windows support compositing mode in Mac OS X 10.2 and
   * later. 
   * 
   * Sheet alert windows are initially placed in the document window
   * group, given a modality of kWindowModalityNone, and given an
   * activation scope of kWindowActivationScopeAll. Available in
   * CarbonLib 1.3 and later, and in Mac OS X 10.1 and later; in
   * CarbonLib, a sheet alert window is actually a movable alert
   * window, which blocks user interaction with all windows of the
   * application.
   */
  kSheetAlertWindowClass        = 15,

  /*
   * A alternate plain window is similar to a plain window but has a
   * solid black shadow on its right and bottom sides. It is rarely
   * used in modern Mac OS applications.
   * kThemeBrushDocumentWindowBackground,
   * kThemeBrushDialogBackgroundActive, and application-specific custom
   * drawing are all commonly used to draw the background of an
   * alternate plain window. 
   * 
   * Alternate plain windows support compositing mode in Mac OS X 10.5
   * and later. 
   * 
   * Alternate plain windows are initially placed in the document
   * window group, given a modality of kWindowModalityNone, and given
   * an activation scope of kWindowActivationScopeAll. Available in
   * CarbonLib 1.3 and later, and Mac OS X 10.1 and later.
   */
  kAltPlainWindowClass          = 16,

  /*
   * A simple window is the simplest possible window; it has no window
   * frame and its entire content is drawn by the application. Use any
   * theme brush or your own custom drawing to draw the background of a
   * simple window. 
   * 
   * Simple windows support compositing mode in Mac OS X 10.3 and
   * later. 
   * 
   * Simple windows are initially placed in the document window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeAll. Available in CarbonLib 1.5 and
   * later, and Mac OS X 10.1 and later.
   */
  kSimpleWindowClass            = 18,

  /*
   * A drawer window is used when implementing a drawer user interface,
   * in which the drawer window slides out from underneath a document
   * window. Use kThemeBrushDrawerBackground or
   * kThemeBrushDocumentWindowBackground to draw the background of
   * drawer windows. 
   * 
   * Drawer windows must always use compositing mode. 
   * 
   * Drawer windows are initially placed in the document window group,
   * given a modality of kWindowModalityNone, and given an activation
   * scope of kWindowActivationScopeAll. Drawer windows must always be
   * created using the Compositing window attribute. Available in Mac
   * OS X 10.2 or later.
   */
  kDrawerWindowClass            = 20,

  /*
   * Not an actual window class, but a meta-constant that is used with
   * GetFrontWindowOfClass, FindWindowOfClass, and GetNextWindowOfClass
   * to indicate that there should be no restriction on the class of
   * the returned window. Also used with GetWindowGroupOfClass to get
   * the root window group.
   */
  kAllWindowClasses             = (UInt32)0xFFFFFFFF
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Attributes                                                                  */
/*——————————————————————————————————————————————————————————————————————————————————————*/

/*
 *  Summary:
 *    Window attribute bits.
 */
enum {

  /*
   * This window has a close box. Available for windows of
   * kDocumentWindowClass, kFloatingWindowClass, and
   * kUtilityWindowClass; in Mac OS X 10.5 and later, also available
   * for windows of kMovableModalWindowClass.
   */
  kHIWindowBitCloseBox          = 1,

  /*
   * This window has a zoom box. Available for windows of
   * kDocumentWindowClass, kFloatingWindowClass, and
   * kUtilityWindowClass; in Mac OS X 10.5 and later, also available
   * for windows of kMovableModalWindowClass. Note that when this
   * attribute is set on a window, both the
   * kWindowHorizontalZoomAttribute and kWindowVerticalZoomAttribute
   * bits are set automatically.
   */
  kHIWindowBitZoomBox           = 2,

  /*
   * This window has a collapse box. Available for windows of
   * kDocumentWindowClass, kFloatingWindowClass, and
   * kUtilityWindowClass. For Floating and Utility window classes on
   * Mac OS X, this attribute must be added to the window using
   * ChangeWindowAttributes after the window is created; it may not be
   * added to the window at creation time.
   */
  kHIWindowBitCollapseBox       = 4,

  /*
   * This window can be resized. Available for windows of
   * kDocumentWindowClass, kMovableModalWindowClass,
   * kFloatingWindowClass, kUtilityWindowClass, and kSheetWindowClass.
   */
  kHIWindowBitResizable         = 5,

  /*
   * This window has a vertical titlebar on the side of the window.
   * Available for windows of kFloatingWindowClass and
   * kUtilityWindowClass.
   */
  kHIWindowBitSideTitlebar      = 6,

  /*
   * This window has a toolbar button. Available for windows of
   * kDocumentWindowClass on Mac OS X.
   */
  kHIWindowBitToolbarButton     = 7,

  /*
   * This window draws its window title and toolbar using a unified
   * appearance that has no separator between the two areas. A window
   * may not have both kHIWindowBitUnifiedTitleAndToolbar and
   * kHIWindowBitTextured. If a window already has the Textured
   * attribute, attempting to set the Unified attribute will cause
   * ChangeWindowAttributes to return an error, and vice versa. This
   * constant was not added to this header file until Mac OS X 10.5,
   * but it is actually available at runtime on Mac OS X 10.4 and later
   * for windows of kDocumentWindowClass. However, on Mac OS X 10.5 and
   * later, kHIWindowBitUnifiedTitleAndToolbar no longer has any
   * effect, since all windows with toolbars now have a unified look.
   */
  kHIWindowBitUnifiedTitleAndToolbar = 8,

  /*
   * This window uses the Textured appearance. Available for document
   * windows on Mac OS X 10.2 and later, and for floating windows on
   * Mac OS X 10.3 and later. Drawers can also be textured, but
   * dynamically adjust their appearance based on their parent window's
   * appearance; it is not necessary to specify this attribute for a
   * textured drawer.
   */
  kHIWindowBitTextured          = 9,

  /*
   * For Document, Floating, and Utility windows, this attribute allows
   * you to hide the title bar of a window. Available in Mac OS X 10.4
   * and later.
   */
  kHIWindowBitNoTitleBar        = 10,

  /*
   * Indicates that a textured window should have square corners. By
   * default, a textured window has round corners. When this attribute
   * is set on a window, the window frame view automatically makes the
   * grow box view opaque, and when this attribute is cleared, the
   * window frame view automatically makes the grow box view
   * transparent. You can change the grow box view transparency after
   * modifying this attribute with HIGrowBoxViewSetTransparent.
   * Relevant only for textured windows; ignored in non-textured
   * windows. Available in Mac OS X 10.5 and later for window classes
   * that support the textured appearance.
   */
  kHIWindowBitTexturedSquareCorners = 11,

  /*
   * Indicates that no border should be drawn between the toolbar and
   * window content. Relevant only in textured windows. Ignored in
   * non-textured windows.  Available in Mac OS X 10.4 and later for
   * window classes that support the textured appearance.
   */
  kHIWindowBitNoTexturedContentSeparator = 12,

  /*
   * Indicates that this window will have rounded bottom corners. By
   * default, a window has squared corners, and the assertion of this
   * bit turns on rounded bottom corners for the window if the window
   * has a bottom bar (set with HIWindowSetContentBorderThickness). If
   * the window does not have a bottom bar, this bit is ignored. If the
   * window has the kHIWindowBitTextured attribute set, the
   * kHIWindowBitRoundBottomBarCorners attribute will be ignored, as
   * that type of window defaults to rounded corners and controls the
   * squareness with the kHIWindowBitTexturedSquareCorners attribute.
   */
  kHIWindowBitRoundBottomBarCorners = 13,

  /*
   * This window does not participate in window cycling invoked by
   * cmd-~ or the "Focus on Window" hotkey defined in the Keyboards
   * preference pane. Available for all windows on Mac OS X 10.2 and
   * later.
   */
  kHIWindowBitDoesNotCycle      = 16,

  /*
   * This window receives no update events. Available for all windows.
   */
  kHIWindowBitNoUpdates         = 17,

  /*
   * This window receives no activate events. Available for all windows.
   */
  kHIWindowBitNoActivates       = 18,

  /*
   * This window receives mouse events even for areas of the window
   * that are transparent (have an alpha channel component of zero).
   * Available for windows of kOverlayWindowClass on Mac OS X 10.0 and
   * 10.1, and available for all windows on Mac OS X 10.2 and later.
   */
  kHIWindowBitOpaqueForEvents   = 19,

  /*
   * This window uses composited drawing. This means that the entire
   * window is comprised of HIViews, and can be treated thusly. This
   * attribute must be specified at window creation, and cannot be
   * changed later with ChangeWindowAttributes. Available on Mac OS X
   * 10.2 and later.
   */
  kHIWindowBitCompositing       = 20,

  /*
   * This window's backing store can be scaled as necessary for high
   * resolution drawing. This attribute is only consulted/respected if
   * the app's Info.plist includes NSHighResolutionCapable with a value
   * of true. This attribute can only be used when
   * kHIWindowBitCompositing is also enabled for the window. When this
   * attribute is enabled, you may not draw with QuickDraw in the
   * window. You may only specify this attribute at window creation
   * time. Available for all windows in 10.7.4 and later.
   */
  kHIWindowBitHighResolutionCapable = 21,

  /*
   * This window has no shadow. Available for all windows on Mac OS X.
   * This attribute is automatically given to windows of
   * kOverlayWindowClass.
   */
  kHIWindowBitNoShadow          = 22,

  /*
   * This window can be made visible prior to user login. By default,
   * in Mac OS X 10.5 and later no windows can be visible before a user
   * logs into the system; this protects the user against certain types
   * of malicious use of insecure applications. However, some software,
   * such as input methods or other accessibility software, may need to
   * deliberately make windows available prior to user login. Such
   * software should add this window attribute to its windows.
   * Available for all windows in Mac OS X 10.5 and later.
   */
  kHIWindowBitCanBeVisibleWithoutLogin = 23,

  /*
   * This window is marked so that the window server will drag the
   * window automatically. Your application should not call DragWindow
   * for this window, else it would confuse the heck out of the drag
   * (it would fight with the window server for control). This
   * attribute is ignored (async drag is not used) if your window is
   * grouped with other windows in a window group that has the
   * kWindowGroupAttrMoveTogether attribute. Available for all windows
   * on Mac OS X 10.3 and later.
   */
  kHIWindowBitAsyncDrag         = 24,

  /*
   * This window is automatically hidden on suspend and shown on
   * resume. Available for all windows. This attribute is automatically
   * given to windows of kFloatingWindowClass, kHelpWindowClass, and
   * kToolbarWindowClass.
   */
  kHIWindowBitHideOnSuspend     = 25,

  /*
   * This window has the standard Carbon window event handler
   * installed. Available for all windows.
   */
  kHIWindowBitStandardHandler   = 26,

  /*
   * This window is automatically hidden during fullscreen mode (when
   * the menubar is invisible) and shown afterwards. Available for all
   * windows. This attribute is automatically given to windows of
   * kUtilityWindowClass.
   */
  kHIWindowBitHideOnFullScreen  = 27,

  /*
   * This window is added to the standard Window menu. Available for
   * windows of kDocumentWindowClass. This attribute is automatically
   * given to windows of kDocumentWindowClass.
   */
  kHIWindowBitInWindowMenu      = 28,

  /*
   * This window supports live resizing. Available for all windows on
   * Mac OS X.
   */
  kHIWindowBitLiveResize        = 29,

  /*
   * This window never receives mouse events, even in areas that are
   * opaque. Instead, clicks on the window will be passed through to
   * windows beneath it. Available for all windows on Mac OS X 10.2 and
   * later.
   */
  kHIWindowBitIgnoreClicks      = 30,

  /*
   * This window will not be repositioned by the default
   * kEventWindowConstrain handler in response to changes in monitor
   * size, Dock position, and so on. Available for all windows on Mac
   * OS X 10.1 and later, and CarbonLib 1.6 and later.
   */
  kHIWindowBitNoConstrain       = 32,

  /*
   * Indicates that this window does not hide when the application is
   * hidden. Available for all windows in Mac OS X 10.5 and later. This
   * attribute is automatically given to windows of kUtilityWindowClass.
   */
  kHIWindowBitDoesNotHide       = 33,

  /*
   * Indicates that this window automatically installs Drag Manager
   * callbacks to detect drag actions, and automatically sends HIView
   * drag Carbon events. Setting this attribute is equivalent to
   * calling the SetAutomaticControlDragTrackingEnabledForWindow API
   * (and calling that API will now set this attribute). Available for
   * all windows in Mac OS X 10.5 and later.
   */
  kHIWindowBitAutoViewDragTracking = 34,

  /*
   * Indicates that this window automatically updates its ColorSync
   * profile to the profile of the display containing the greatest area
   * of the window. After the profile is set,
   * kEventWindowColorSpaceChanged is sent to the window. The standard
   * window handler's response is to repaint the entire window.
   * Available for all windows in Mac OS X 10.6 and later. This
   * attribute is automatically given to all compositing windows.
   */
  kHIWindowBitAutoCalibration   = 36,

  /*
   * Indicates that this window can become fullscreen, and causes the
   * enter fullscreen title bar button to be added to the window's
   * title bar. Including this bit on a window causes HIToolbox to add
   * appropriate Enter/Exit Full Screen menu items to the app's menus,
   * which HIToolbox will update appropriately. Only supported for
   * windows of kDocumentWindowClass. Available in Mac OS X 10.7 and
   * later.
   */
  kHIWindowBitFullScreenPrimary = 45,

  /*
   * Indicates that this window can coexist on a fullscreen space with
   * a fullscreen window of another application, but is not capable of
   * becoming fullscreen itself.
   * This window attribute is used in conjunction with the
   * kHIWindowCanJoinAllSpaces Availability option to allow the window
   * to move across FullScreen workspaces. For applications that are
   * BackgroundOnly or UIElements, this is the default behavior when
   * kHIWindowCanJoinAllSpaces is also set. For other applications,
   * kHIWindowBitFullScreenAuxiliary must be specified, and is
   * supported for window classes including kUtilityWindowClass, and
   * kFloatingWindowClass with a WindowActivationScope of
   * kWindowActivationScopeIndependent or kWindowActivationScopeNone.
   * Available in Mac OS X 10.7 and later.
   */
  kHIWindowBitFullScreenAuxiliary = 46
};


/*
 *  Summary:
 *    Window attributes as bitmasks. In Mac OS X 10.5 and later,
 *    kHIWindowBit constants are preferred.
 *  
 *  Discussion:
 *    Note that the kWindowApplicationScaledAttribute constant provided
 *    in Mac OS X 10.4 has been removed in Mac OS X 10.5 and later.
 *    Application-scaled mode was never fully implemented and all
 *    support for it has been removed from HIToolbox in Mac OS X 10.5.
 */
enum {

  /*
   * A constant with value zero; the lack of any attributes.
   */
  kWindowNoAttributes           = 0,

  /*
   * See kHIWindowBitCloseBox.
   */
  kWindowCloseBoxAttribute      = (1 << (kHIWindowBitCloseBox - 1)),

  /*
   * See kHIWindowBitZoomBox.
   */
  kWindowHorizontalZoomAttribute = (1 << (kHIWindowBitZoomBox - 1)),

  /*
   * See kHIWindowBitZoomBox.
   */
  kWindowVerticalZoomAttribute  = (1 << kHIWindowBitZoomBox),

  /*
   * See kHIWindowBitZoomBox.
   */
  kWindowFullZoomAttribute      = (kWindowVerticalZoomAttribute | kWindowHorizontalZoomAttribute),

  /*
   * See kHIWindowBitCollapseBox.
   */
  kWindowCollapseBoxAttribute   = (1 << (kHIWindowBitCollapseBox - 1)),

  /*
   * See kHIWindowBitResizable.
   */
  kWindowResizableAttribute     = (1 << (kHIWindowBitResizable - 1)),

  /*
   * See kHIWindowBitSideTitlebar.
   */
  kWindowSideTitlebarAttribute  = (1 << (kHIWindowBitSideTitlebar - 1)),

  /*
   * See kHIWindowBitToolbarButton.
   */
  kWindowToolbarButtonAttribute = (1 << (kHIWindowBitToolbarButton - 1)),

  /*
   * See kHIWindowBitUnifiedTitleAndToolbar.
   */
  kWindowUnifiedTitleAndToolbarAttribute = (1 << (kHIWindowBitUnifiedTitleAndToolbar - 1)),

  /*
   * See kHIWindowBitTextured.
   */
  kWindowMetalAttribute         = (1 << (kHIWindowBitTextured - 1)),

  /*
   * See kHIWindowBitNoTitleBar.
   */
  kWindowNoTitleBarAttribute    = (1 << (kHIWindowBitNoTitleBar - 1)),

  /*
   * See kHIWindowBitTexturedSquareCorners.
   */
  kWindowTexturedSquareCornersAttribute = (1 << (kHIWindowBitTexturedSquareCorners - 1)),

  /*
   * See kHIWindowBitNoTexturedContentSeparator.
   */
  kWindowMetalNoContentSeparatorAttribute = (1 << (kHIWindowBitNoTexturedContentSeparator - 1)),
  kWindowHasRoundBottomBarCornersAttribute = (1 << (kHIWindowBitRoundBottomBarCorners - 1)),

  /*
   * See kHIWindowBitDoesNotCycle.
   */
  kWindowDoesNotCycleAttribute  = (1 << (kHIWindowBitDoesNotCycle - 1)),

  /*
   * See kHIWindowBitNoUpdates.
   */
  kWindowNoUpdatesAttribute     = (1 << (kHIWindowBitNoUpdates - 1)),

  /*
   * See kHIWindowBitNoActivates.
   */
  kWindowNoActivatesAttribute   = (1 << (kHIWindowBitNoActivates - 1)),

  /*
   * See kHIWindowBitOpaqueForEvents.
   */
  kWindowOpaqueForEventsAttribute = (1 << (kHIWindowBitOpaqueForEvents - 1)),

  /*
   * See kHIWindowBitCompositing.
   */
  kWindowCompositingAttribute   = (1 << (kHIWindowBitCompositing - 1)),

  /*
   * See kHIWindowBitNoShadow.
   */
  kWindowNoShadowAttribute      = (1 << (kHIWindowBitNoShadow - 1)),

  /*
   * See kHIWindowBitCanBeVisibleWithoutLogin.
   */
  kWindowCanBeVisibleWithoutLoginAttribute = (1 << (kHIWindowBitCanBeVisibleWithoutLogin - 1)),

  /*
   * See kHIWindowBitHideOnSuspend.
   */
  kWindowHideOnSuspendAttribute = (1 << (kHIWindowBitHideOnSuspend - 1)),

  /*
   * See kHIWindowBitAsyncDrag.
   */
  kWindowAsyncDragAttribute     = (1 << (kHIWindowBitAsyncDrag - 1)),

  /*
   * See kHIWindowBitStandardHandler.
   */
  kWindowStandardHandlerAttribute = (1 << (kHIWindowBitStandardHandler - 1)),

  /*
   * See kHIWindowBitHideOnFullScreen.
   */
  kWindowHideOnFullScreenAttribute = (1 << (kHIWindowBitHideOnFullScreen - 1)),

  /*
   * See kHIWindowBitInWindowMenu.
   */
  kWindowInWindowMenuAttribute  = (1 << (kHIWindowBitInWindowMenu - 1)),

  /*
   * See kHIWindowBitLiveResize.
   */
  kWindowLiveResizeAttribute    = (1 << (kHIWindowBitLiveResize - 1)),

  /*
   * See kHIWindowBitIgnoreClicks.
   */
  kWindowIgnoreClicksAttribute  = (1 << (kHIWindowBitIgnoreClicks - 1)),

  /*
   * See kHIWindowBitHighResolutionCapable.
   */
  kWindowHighResolutionCapableAttribute = (1 << (kHIWindowBitHighResolutionCapable - 1)),

  /*
   * The minimum set of window attributes commonly used by document
   * windows.
   */
  kWindowStandardDocumentAttributes = (kWindowCloseBoxAttribute | kWindowFullZoomAttribute | kWindowCollapseBoxAttribute | kWindowResizableAttribute),

  /*
   * The minimum set of window attributes commonly used by floating
   * windows.
   */
  kWindowStandardFloatingAttributes = (kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute)
};


enum
{
  /*
   * See kHIWindowBitNoConstrain.
   */
  kWindowNoConstrainAttribute              = 0x80000000 // force unsigned by specifying the constant explicitly
};
typedef OptionBits                      WindowAttributes;

/*
 *  Summary:
 *    DEPRECATED Window attribute bits.
 */
enum {

  /*
   * DEPRECATED name and functionality. Replaced conceptually by
   * kHIWindowBitHighResolutionCapable.
   */
  kHIWindowBitFrameworkScaled   = kHIWindowBitHighResolutionCapable
};


/*
 *  Summary:
 *    DEPRECATED Window attribute bitmasks.
 */
enum {

  /*
   * DEPRECATED name and functionality. Replaced conceptually by
   * kWindowHighResolutionCapableAttribute.
   */
  kWindowFrameworkScaledAttribute = kWindowHighResolutionCapableAttribute
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Definition Type                                                             */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kWindowDefProcType            = 'WDEF'
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Mac OS 7.5 Window Definition Resource IDs                                          */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kStandardWindowDefinition     = 0,    /* for document windows and dialogs*/
  kRoundWindowDefinition        = 1,    /* old Desk Accessory-style window*/
  kFloatingWindowDefinition     = 124   /* for floating windows*/
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Variant Codes                                                                      */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
                                        /* for use with kStandardWindowDefinition */
  kDocumentWindowVariantCode    = 0,
  kModalDialogVariantCode       = 1,
  kPlainDialogVariantCode       = 2,
  kShadowDialogVariantCode      = 3,
  kMovableModalDialogVariantCode = 5,
  kAlertVariantCode             = 7,
  kMovableAlertVariantCode      = 9,    /* for use with kFloatingWindowDefinition */
  kSideFloaterVariantCode       = 8
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • DefProc IDs                                                                        */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
                                        /* classic ids */
  documentProc                  = 0,
  dBoxProc                      = 1,
  plainDBox                     = 2,
  altDBoxProc                   = 3,
  noGrowDocProc                 = 4,
  movableDBoxProc               = 5,
  zoomDocProc                   = 8,
  zoomNoGrow                    = 12,   /* floating window defproc ids */
  floatProc                     = 1985,
  floatGrowProc                 = 1987,
  floatZoomProc                 = 1989,
  floatZoomGrowProc             = 1991,
  floatSideProc                 = 1993,
  floatSideGrowProc             = 1995,
  floatSideZoomProc             = 1997,
  floatSideZoomGrowProc         = 1999
};

enum {
                                        /* Resource IDs for theme-savvy window defprocs */
  kWindowDocumentDefProcResID   = 64,
  kWindowDialogDefProcResID     = 65,
  kWindowUtilityDefProcResID    = 66,
  kWindowUtilitySideTitleDefProcResID = 67,
  kWindowSheetDefProcResID      = 68,
  kWindowSimpleDefProcResID     = 69,
  kWindowSheetAlertDefProcResID = 70
};

enum {
                                        /* Proc IDs for theme-savvy windows */
  kWindowDocumentProc           = 1024,
  kWindowGrowDocumentProc       = 1025,
  kWindowVertZoomDocumentProc   = 1026,
  kWindowVertZoomGrowDocumentProc = 1027,
  kWindowHorizZoomDocumentProc  = 1028,
  kWindowHorizZoomGrowDocumentProc = 1029,
  kWindowFullZoomDocumentProc   = 1030,
  kWindowFullZoomGrowDocumentProc = 1031
};


enum {
                                        /* Proc IDs for theme-savvy dialogs */
  kWindowPlainDialogProc        = 1040,
  kWindowShadowDialogProc       = 1041,
  kWindowModalDialogProc        = 1042,
  kWindowMovableModalDialogProc = 1043,
  kWindowAlertProc              = 1044,
  kWindowMovableAlertProc       = 1045
};


enum {
                                        /* procIDs available from Mac OS 8.1 (Appearance 1.0.1) forward */
  kWindowMovableModalGrowProc   = 1046
};


enum {
                                        /* Proc IDs for top title bar theme-savvy floating windows */
  kWindowFloatProc              = 1057,
  kWindowFloatGrowProc          = 1059,
  kWindowFloatVertZoomProc      = 1061,
  kWindowFloatVertZoomGrowProc  = 1063,
  kWindowFloatHorizZoomProc     = 1065,
  kWindowFloatHorizZoomGrowProc = 1067,
  kWindowFloatFullZoomProc      = 1069,
  kWindowFloatFullZoomGrowProc  = 1071
};


enum {
                                        /* Proc IDs for side title bar theme-savvy floating windows */
  kWindowFloatSideProc          = 1073,
  kWindowFloatSideGrowProc      = 1075,
  kWindowFloatSideVertZoomProc  = 1077,
  kWindowFloatSideVertZoomGrowProc = 1079,
  kWindowFloatSideHorizZoomProc = 1081,
  kWindowFloatSideHorizZoomGrowProc = 1083,
  kWindowFloatSideFullZoomProc  = 1085,
  kWindowFloatSideFullZoomGrowProc = 1087
};


enum {
                                        /* Proc IDs for sheet windows */
  kWindowSheetProc              = 1088, /* available in Mac OS X and CarbonLib 1.3 */
  kWindowSheetAlertProc         = 1120  /* available in Mac OS X 10.1 and CarbonLib 1.3 */
};



/*
 *  Discussion:
 *    Window defproc IDs for simple windows
 */
enum {
                                        /* Proc IDs for simple windows */

  /*
   * A window that has no structure region; the content covers the
   * entire window.
   */
  kWindowSimpleProc             = 1104,

  /*
   * A window that has a 1-pixel black frame as its structure.
   */
  kWindowSimpleFrameProc        = 1105
};


/*
 *  Discussion:
 *    More window defproc IDs.
 */
enum {

  /*
   * Window defproc ID for overlay-class windows.
   */
  kWindowOverlayProc            = 1136
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • System 7 Window Positioning Constants                                              */
/*                                                                                      */
/* Passed into StandardAlert and used in ‘WIND’, ‘DLOG’, and ‘ALRT’ templates           */
/* StandardAlert uses zero to specify the default position. Other calls use zero to     */
/* specify “no position”.  Do not pass these constants to RepositionWindow.  Do not     */
/* store these constants in the BasicWindowDescription of a ‘wind’ resource.            */
/*——————————————————————————————————————————————————————————————————————————————————————*/

enum {
  kWindowNoPosition             = 0x0000,
  kWindowDefaultPosition        = 0x0000, /* used by StandardAlert*/
  kWindowCenterMainScreen       = 0x280A,
  kWindowAlertPositionMainScreen = 0x300A,
  kWindowStaggerMainScreen      = 0x380A,
  kWindowCenterParentWindow     = 0xA80A,
  kWindowAlertPositionParentWindow = 0xB00A,
  kWindowStaggerParentWindow    = 0xB80A,
  kWindowCenterParentWindowScreen = 0x680A,
  kWindowAlertPositionParentWindowScreen = 0x700A,
  kWindowStaggerParentWindowScreen = 0x780A
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Positioning Methods                                                         */
/*                                                                                      */
/* Positioning methods passed to RepositionWindow.                                      */
/* Do not use them in WIND, ALRT, DLOG templates.                                       */
/* Do not confuse these constants with the constants above                              */
/*——————————————————————————————————————————————————————————————————————————————————————*/

/*
 *  WindowPositionMethod
 *  
 *  Summary:
 *    Positioning methods usable with RepositionWindow.
 *  
 *  Discussion:
 *    These constants are for use solely with the RepositionWindow API.
 *    They should not be used in 'WIND', 'ALRT', or 'DLOG' resources.
 */
typedef UInt32 WindowPositionMethod;
enum {

  /*
   * Centers the window on the main screen.
   */
  kWindowCenterOnMainScreen     = 1,

  /*
   * Centers the window on its parent window. The parent window must be
   * different from the positioned window.
   */
  kWindowCenterOnParentWindow   = 2,

  /*
   * Centers the window on the screen containing the largest portion of
   * its parent window. On Mac OS X 10.3 and later, the parent window
   * may be the same as the positioned window. On CarbonLib and earlier
   * versions of Mac OS X, the parent window must be different from the
   * positioned window.
   */
  kWindowCenterOnParentWindowScreen = 3,

  /*
   * Cascades the window on the main screen.
   */
  kWindowCascadeOnMainScreen    = 4,

  /*
   * Cascades the window on its parent window. The parent window must
   * be different from the positioned window.
   */
  kWindowCascadeOnParentWindow  = 5,

  /*
   * Cascades the window on the screen containing the largest portion
   * of its parent window. On Mac OS X 10.3 and later, the parent
   * window may be the same as the positioned window. On CarbonLib and
   * earlier versions of Mac OS X, the parent window must be different
   * from the positioned window.
   */
  kWindowCascadeOnParentWindowScreen = 6,

  /*
   * Cascades the window on the screen containing the largest portion
   * of its parent window, starting below and to the right of its
   * parent window. The parent window must be different from the
   * positioned window. Available in Mac OS X 10.2 and CarbonLib 1.6
   * and later.
   */
  kWindowCascadeStartAtParentWindowScreen = 10,

  /*
   * Puts the window into the alert position on the main screen.
   */
  kWindowAlertPositionOnMainScreen = 7,

  /*
   * Puts the window into the alert position on its parent window. The
   * parent window must be different from the positioned window.
   */
  kWindowAlertPositionOnParentWindow = 8,

  /*
   * Puts the window into the alert position on the screen containing
   * the largest portion of its parent window. On Mac OS X 10.3 and
   * later, the parent window may be the same as the positioned window.
   * On CarbonLib and earlier versions of Mac OS X, the parent window
   * must be different from the positioned window.
   */
  kWindowAlertPositionOnParentWindowScreen = 9
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • GetWindowRegion Types                                                              */
/*——————————————————————————————————————————————————————————————————————————————————————*/
typedef UInt16                          WindowRegionCode;
enum {
                                        /* Region values to pass into GetWindowRegion & GetWindowBounds. All regions are reported in global coordinates. */
  kWindowTitleBarRgn            = 0,
  kWindowTitleTextRgn           = 1,
  kWindowCloseBoxRgn            = 2,
  kWindowZoomBoxRgn             = 3,
  kWindowDragRgn                = 5,
  kWindowGrowRgn                = 6,
  kWindowCollapseBoxRgn         = 7,
  kWindowTitleProxyIconRgn      = 8,    /* Mac OS 8.5 forward*/
  kWindowStructureRgn           = 32,
  kWindowContentRgn             = 33,   /* Content area of the window; empty when the window is collapsed*/
  kWindowUpdateRgn              = 34,   /* Carbon forward*/
  kWindowOpaqueRgn              = 35,   /* Mac OS X: Area of window considered to be opaque. Only valid for windows with alpha channels.*/
  kWindowGlobalPortRgn          = 40,   /* Carbon forward - bounds of the window’s port in global coordinates; not affected by CollapseWindow*/
  kWindowToolbarButtonRgn       = 41    /* Mac OS X Tiger: the toolbar button area*/
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Standard Window Kinds                                                              */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  dialogKind                    = 2,
  userKind                      = 8,
  kDialogWindowKind             = 2,
  kApplicationWindowKind        = 8
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • FindWindow Result Codes                                                            */
/*——————————————————————————————————————————————————————————————————————————————————————*/
typedef SInt16 WindowPartCode;
enum {
  inDesk                        = 0,
  inNoWindow                    = 0,
  inMenuBar                     = 1,
  inSysWindow                   = 2,
  inContent                     = 3,
  inDrag                        = 4,
  inGrow                        = 5,
  inGoAway                      = 6,
  inZoomIn                      = 7,
  inZoomOut                     = 8,
  inCollapseBox                 = 11,   /* Mac OS 8.0 forward*/
  inProxyIcon                   = 12,   /* Mac OS 8.5 forward*/
  inToolbarButton               = 13,   /* Mac OS X forward*/
  inStructure                   = 15    /* Mac OS X 10.1 forward*/
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Definition Hit Test Result Codes                                            */
/*——————————————————————————————————————————————————————————————————————————————————————*/
typedef SInt16 WindowDefPartCode;
enum {
  wNoHit                        = 0,
  wInContent                    = 1,
  wInDrag                       = 2,
  wInGrow                       = 3,
  wInGoAway                     = 4,
  wInZoomIn                     = 5,
  wInZoomOut                    = 6,
  wInCollapseBox                = 9,    /* Mac OS 8.0 forward*/
  wInProxyIcon                  = 10,   /* Mac OS 8.5 forward*/
  wInToolbarButton              = 11,   /* Mac OS X forward*/
  wInStructure                  = 13    /* Mac OS X 10.1 forward*/
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Feature Bits                                                                */
/*——————————————————————————————————————————————————————————————————————————————————————*/

/*
 *  Summary:
 *    Window feature bits
 *  
 *  Discussion:
 *    These feature bits are supplied by window definition functions in
 *    response to the kWindowMsgGetFeatures message or the
 *    kEventWindowInit Carbon event. A window's feature bits can also
 *    be modified dynamically using the HIWindowChangeFeatures API;
 *    typically, feature bits are only modified by a window definition
 *    or window frame view.
 */
enum {

  /*
   * Indicates whether the window is resizable. Available on Mac OS 8.0
   * and later. Not supported on Mac OS X; replaced by
   * kWindowResizableAttribute.
   */
  kWindowCanGrow                = (1 << 0),

  /*
   * Indicates whether the window can zoom. Available on Mac OS 8.0 and
   * later. Not supported on Mac OS X; replaced by
   * kWindowHorizontal/Vertical/FullZoomAttribute.
   */
  kWindowCanZoom                = (1 << 1),

  /*
   * Indicates whether the window can be minimized. Available on Mac OS
   * 8.0 and later. Not supported on Mac OS X; replaced by
   * kWindowCollapseBoxAttribute.
   */
  kWindowCanCollapse            = (1 << 2),

  /*
   * Indicates whether the window is application-modal. Available on
   * Mac OS 8.0 and later.
   */
  kWindowIsModal                = (1 << 3),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgGetRegion message. Available on Mac OS 8.0 and later.
   * Not supported on Mac OS X. All window definitions that implement
   * the kWindowMsgGetFeatures message automatically get this feature
   * bit on Mac OS X.
   */
  kWindowCanGetWindowRegion     = (1 << 4),

  /*
   * Indicates whether the window is an alert. Available on Mac OS 8.0
   * and later.
   */
  kWindowIsAlert                = (1 << 5),

  /*
   * Indicates whether the window has a title bar. Available on Mac OS
   * 8.0 and later. This feature is required for async dragging to be
   * enabled for a window.
   */
  kWindowHasTitleBar            = (1 << 6),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgDragHilite message. Available on Mac OS 8.5 and later.
   */
  kWindowSupportsDragHilite     = (1 << 7),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgModified message. Available on Mac OS 8.5 and later.
   */
  kWindowSupportsModifiedBit    = (1 << 8),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgDrawInCurrentPort message. Available on Mac OS 8.5 and
   * later. Not supported on Mac OS X.
   */
  kWindowCanDrawInCurrentPort   = (1 << 9),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgSetupProxyDragImage message. Available on Mac OS 8.5 and
   * later.
   */
  kWindowCanSetupProxyDragImage = (1 << 10),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgMeasureTitle message. Available on Mac OS 8.5 and later.
   */
  kWindowCanMeasureTitle        = (1 << 11),

  /*
   * Indicates that the window definition wants to receive a
   * kWindowMsgCleanUp message for each existing window when a process
   * is terminated. Available on Mac OS 8.5 and later. Not supported on
   * Mac OS X.
   */
  kWindowWantsDisposeAtProcessDeath = (1 << 12),

  /*
   * Indicates that the window definition supports the
   * kWindowMsgGetGrowImageRegion message. Available on Mac OS X 10.0
   * and later.
   */
  kWindowSupportsGetGrowImageRegion = (1 << 13),

  /*
   * Indicates that the window is entirely opaque. If this feature bit
   * is set, the window will use less memory because no alpha channel
   * information will be stored for the window's pixels. If this
   * feature bit is not set, the Window Manager will send a
   * kEventWindowGetRegion Carbon event to the window with the
   * kWindowOpaqueRgn constant to get a region that describes the
   * opaque area of the window. Available on Mac OS X 10.1 and later.
   */
  kWindowIsOpaque               = (1 << 14),

  /*
   * Indicates that the window definition does not require that the
   * current port be the classic Window Manager port. Not supported on
   * Mac OS X.
   */
  kWindowDefSupportsColorGrafPort = 0x40000002
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • Region Dragging Constants                                                         */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kMouseUpOutOfSlop             = (int)0x80008000
};


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Class Ordering                                                              */
/*                                                                                      */
/*  Special cases for the “behind” parameter in window creation calls.                  */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#define kFirstWindowOfClass             ((WindowRef)(-1))
#define kLastWindowOfClass              ((WindowRef)0)

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Zoom Information Handle                                                            */
/*——————————————————————————————————————————————————————————————————————————————————————*/
struct WStateData {
  Rect                userState;              /*user zoom state*/
  Rect                stdState;               /*standard zoom state*/
};
typedef struct WStateData               WStateData;
typedef WStateData *                    WStateDataPtr;
typedef WStateDataPtr *                 WStateDataHandle;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window definition function                                                         */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#if __LP64__
typedef void *                          WindowDefUPP;
#else

/*
 *  WindowDefProcPtr
 *  
 *  Summary:
 *    Callback function for a custom window definition.
 */
typedef CALLBACK_API( long , WindowDefProcPtr )(short varCode, WindowRef window, short message, long param);
typedef STACK_UPP_TYPE(WindowDefProcPtr)                        WindowDefUPP;
/*
 *  NewWindowDefUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern WindowDefUPP
NewWindowDefUPP(WindowDefProcPtr userRoutine)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeWindowDefUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeWindowDefUPP(WindowDefUPP userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeWindowDefUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern long
InvokeWindowDefUPP(
  short         varCode,
  WindowRef     window,
  short         message,
  long          param,
  WindowDefUPP  userUPP)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

#if __MACH__
  #ifdef __cplusplus
    inline WindowDefUPP                                         NewWindowDefUPP(WindowDefProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeWindowDefUPP(WindowDefUPP) { }
    inline long                                                 InvokeWindowDefUPP(short varCode, WindowRef window, short message, long param, WindowDefUPP userUPP) { return (*userUPP)(varCode, window, message, param); }
  #else
    #define NewWindowDefUPP(userRoutine)                        ((WindowDefUPP)userRoutine)
    #define DisposeWindowDefUPP(userUPP)
    #define InvokeWindowDefUPP(varCode, window, message, param, userUPP) (*userUPP)(varCode, window, message, param)
  #endif
#endif

#endif  /* __LP64__ */

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Definition Spec.  Used in Carbon to specify the code that defines a window. */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kWindowDefProcPtr             = 0,    /* raw proc-ptr based access*/
  kWindowDefObjectClass         = 1,    /* event-based definition using a ToolboxObjectClassRef (Carbon 1.1 or later)*/
  kWindowDefProcID              = 2,    /* explicit proc ID; overrides the window class default proc ID*/
  kWindowDefHIView              = 3     /* this window uses an HIView as its frame, not a WDEF*/
};

typedef UInt32                          WindowDefType;
struct WindowDefSpec {
  WindowDefType       defType;
  union {
    WindowDefUPP        defProc;
    void *              classRef;             /* ToolboxObjectClassRef*/
    short               procID;
    void *              rootView;             /* HIViewRef*/
  }                       u;
};
typedef struct WindowDefSpec            WindowDefSpec;
typedef WindowDefSpec *                 WindowDefSpecPtr;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Creation & Persistence                                                      */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
    About Window Manager Coordinate Spaces
    
    All Window Manager API that takes a size, position, or rectangle for input, and does not take an
    HICoordinateSpace parameter, expects that its input parameters are given in the 72DPIGlobal coordinate
    space. Likewise, any API that returns a rect or region and that does not take an HICoordinateSpace
    parameter will provide its output in the 72DPIGlobal coordinate space.
    
    We expect that most developers will want to work primarily in the 72DPIGlobal space. Doing so makes
    your code independent of the current user interface scale factor, and eases source compatibility with
    earlier versions of Mac OS X that do not support resolution independence. However, there are also certain
    cases where your application must express your window's bounds in pixel coordinates; primarily when you
    need to position your windows so they exactly align with each other or with some other fixed location,
    such as the edge of the display. For these cases, you should use the ScreenPixel coordinate space.
    
    Note that non-integral pixel coordinates are not available in Mac OS X. If you specify non-integral
    coordinates (in either 72DPIGlobal or ScreenPixel coordinate spaces), they will be rounded to the nearest
    integral value in ScreenPixel space when the window is actually positioned or sized.
*/
#if !__LP64__
/*
 *  HIWindowCreate()
 *  
 *  Summary:
 *    Creates a new window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inClass:
 *      The window class.
 *    
 *    inAttributes:
 *      A zero-terminated array of attribute bit constants, such as
 *      kHIWindowBitCloseBox. Each array entry specifies an attribute
 *      of the window to set. You may pass NULL if you do not wish to
 *      set any attributes.
 *    
 *    inDefSpec:
 *      Contains a custom window proc ID or root view for the window.
 *      May be NULL if you don't need to customize the window.
 *    
 *    inSpace:
 *      The coordinate space of the inBounds parameter. This parameter
 *      must be either kHICoordSpaceScreenPixels or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    inBounds:
 *      The window content bounds, in the coordinate space specified by
 *      the inSpace parameter. ••• consider changing this to structure
 *      bounds?
 *    
 *    outWindow:
 *      On exit, contains the new window.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowCreate(
  WindowClass            inClass,
  const int *            inAttributes,
  const WindowDefSpec *  inDefSpec,
  HICoordinateSpace      inSpace,
  const HIRect *         inBounds,
  WindowRef *            outWindow)                           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  CreateNewWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
CreateNewWindow(
  WindowClass        windowClass,
  WindowAttributes   attributes,
  const Rect *       contentBounds,
  WindowRef *        outWindow)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DisposeWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DisposeWindow(WindowRef window)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Custom Windows                                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* Routines available from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward*/

/*
 *  CreateCustomWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateCustomWindow(
  const WindowDefSpec *  def,
  WindowClass            windowClass,
  WindowAttributes       attributes,
  const Rect *           contentBounds,
  WindowRef *            outWindow)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ReshapeCustomWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReshapeCustomWindow(WindowRef window)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RegisterWindowDefinition()
 *  
 *  Summary:
 *    Registers or unregisters a binding between a resource ID and a
 *    window definition function.
 *  
 *  Discussion:
 *    In the Mac OS 8.x Window Manager, a 'WIND' resource can contain
 *    an embedded WDEF procID that is used by the Window Manager as the
 *    resource ID of an 'WDEF' resource to lay out and draw the window.
 *    The 'WDEF' resource is loaded by the Window Manager when you load
 *    the menu with GetNewWindow. Since WDEFs can no longer be packaged
 *    as code resources on Carbon, the procID can no longer refer
 *    directly to a WDEF resource. However, using
 *    RegisterWindowDefinition you can instead specify a
 *    UniversalProcPtr pointing to code in your application code
 *    fragment. 
 *    
 *    RegisterWindowDefinition is available when linking to CarbonLib
 *    1.1 forward. 
 *    
 *    RegisterWindowDefinition is not available in 64-bit mode because
 *    WDEFs and ToolboxObjects are not supported in 64-bit mode.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inResID:
 *      A WDEF proc ID, as used in a 'WIND' resource.
 *    
 *    inDefSpec:
 *      Specifies the WindowDefUPP or ToolboxObjectClassRef that should
 *      be used for windows with the given WDEF proc ID. Passing NULL
 *      allows you to unregister the window definition that had been
 *      associated with the given WDEF proc ID.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RegisterWindowDefinition(
  SInt16                 inResID,
  const WindowDefSpec *  inDefSpec)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window part tracking                                                               */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  GetWindowWidgetHilite()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowWidgetHilite(
  WindowRef            inWindow,
  WindowDefPartCode *  outHilite)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Metainformation Accessors                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  IsValidWindowClass()
 *  
 *  Summary:
 *    Indicates whether a window class is supported by the Window
 *    Manager.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inClass:
 *      The window class.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsValidWindowClass(WindowClass inClass)                       AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  HIWindowTestAttribute()
 *  
 *  Summary:
 *    Determines whether a window possesses a specified attribute.
 *  
 *  Discussion:
 *    HIWindowTestAttribute takes an attribute bit constant, such as
 *    kHIWindowBitCloseBox. You must not use the older attribute
 *    bitmask constants, such as kWindowCloseBoxAttribute, with this
 *    API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose attributes to test.
 *    
 *    inAttr:
 *      The attribute bit constant to test.
 *  
 *  Result:
 *    True if the window possesses the specified attribute, or false if
 *    not.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowTestAttribute(
  WindowRef   inWindow,
  int         inAttr)                                         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowChangeAttributes()
 *  
 *  Summary:
 *    Changes the attributes of a window.
 *  
 *  Discussion:
 *    HIWindowChangeAttributes takes two arrays of attribute bit
 *    constants, such as kHIWindowBitCloseBox. You must not use the
 *    older attribute bitmask constants, such as
 *    kWindowCloseBoxAttribute, with this API. The first array
 *    specifies the attributes to set, and the second specifies the
 *    attributes to clear. For example, you might call this API as
 *    follows: 
 *    
 *    int setAttr[] = { kHIWindowBitCloseBox, kHIWindowBitZoomBox, 0
 *    };
 *    int clearAttr[] = { kHIWindowBitNoTitleBar, 0 };
 *     HIWindowChangeAttributes( window, setAttr, clearAttr );
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose attributes to change.
 *    
 *    inAttrToSet:
 *      A zero-terminated array of attribute bit constants, such as
 *      kHIWindowBitCloseBox. Each array entry specifies an attribute
 *      of the window to set. You may pass NULL if you do not wish to
 *      set any attributes.
 *    
 *    inAttrToClear:
 *      A zero-terminated array of attribute bit constants, such as
 *      kHIWindowBitCloseBox. Each array entry specifies an attribute
 *      of the window to clear. You may pass NULL if you do not wish to
 *      clear any attributes.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowChangeAttributes(
  WindowRef    inWindow,
  const int *  inAttrToSet,         /* can be NULL */
  const int *  inAttrToClear)       /* can be NULL */         AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowIsAttributeAvailable()
 *  
 *  Summary:
 *    Determines if a window attribute is valid for a specified window
 *    class.
 *  
 *  Discussion:
 *    This API is useful because some window classes support different
 *    attributes on different platforms (for example, floating windows
 *    can have collapse boxes on 9, but not on X), and the Window
 *    Manager will return an error if you attempt to create a window
 *    with attributes that aren't supported for the requested window
 *    class. You can use this API to remove attributes that are not
 *    supported by the current platform before calling CreateNewWindow.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inClass:
 *      The window class.
 *    
 *    inAttr:
 *      The window attribute bit. You must use an attribute bit
 *      constant, such as kHIWindowBitCloseBox. You must not use the
 *      older attribute bitmask constants, such as
 *      kWindowCloseBoxAttribute.
 *  
 *  Result:
 *    True if the attribute is available for windows of the specifed
 *    class, or false if not.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowIsAttributeAvailable(
  WindowClass   inClass,
  int           inAttr)                                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  GetAvailableWindowAttributes()
 *  
 *  Summary:
 *    Returns the window attributes that are valid for a window class.
 *  
 *  Discussion:
 *    This API is useful because some window classes support different
 *    attributes on different platforms (for example, floating windows
 *    can have collapse boxes on 9, but not on X), and the Window
 *    Manager will return an error if you attempt to create a window
 *    with attributes that aren't supported for the requested window
 *    class. You can use this API to remove attributes that are not
 *    supported by the current platform before calling CreateNewWindow.
 *    
 *    
 *    In Mac OS X 10.5 and later, windows may have more than 32 bits of
 *    attributes. GetAvailableWindowAttributes can only return the low
 *    32 bits of window attributes; therefore, we recommend that you
 *    use HIWindowIsAttributeAvailable instead of
 *    GetAvailableWindowAttributes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inClass:
 *      The window class.
 *  
 *  Result:
 *    The window attributes that are supported for the specified window
 *    class.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowAttributes 
GetAvailableWindowAttributes(WindowClass inClass)             AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  GetWindowClass()
 *  
 *  Summary:
 *    Returns the class of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose class to retrieve.
 *    
 *    outClass:
 *      On exit, contains the window's class.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowClass(
  WindowRef      window,
  WindowClass *  outClass)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowAttributes()
 *  
 *  Summary:
 *    Returns the attributes of a window.
 *  
 *  Discussion:
 *    In Mac OS X 10.5 and later, windows may have more than 32 bits of
 *    attributes. GetWindowAttributes can only return the low 32 bits
 *    of window attributes; therefore, we recommend that you use
 *    HIWindowTestAttribute instead of GetWindowAttributes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose attributes to retrieve.
 *    
 *    outAttributes:
 *      On exit, contains the window's attributes.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowAttributes(
  WindowRef           window,
  WindowAttributes *  outAttributes)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ChangeWindowAttributes()
 *  
 *  Summary:
 *    Sets and clears window attributes.
 *  
 *  Discussion:
 *    In Mac OS X 10.5 and later, windows may have more than 32 bits of
 *    attributes. ChangeWindowAttributes can only set or clear the low
 *    32 bits of window attributes; therefore, we recommend that you
 *    use HIWindowChangeAttributes instead of ChangeWindowAttributes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose attributes to modify.
 *    
 *    setTheseAttributes:
 *      The attributes to set.
 *    
 *    clearTheseAttributes:
 *      The attributes to clear.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ChangeWindowAttributes(
  WindowRef          window,
  WindowAttributes   setTheseAttributes,
  WindowAttributes   clearTheseAttributes)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowChangeClass()
 *  
 *  Summary:
 *    Changes the window look and feel of an existing window on the fly.
 *  
 *  Discussion:
 *    HIWindowChangeClass changes the class of a window. Unlike
 *    SetWindowClass, this call effectively changes the look and
 *    behavior of the window all at once. Because SetWindowClass
 *    already existed and had certain behaviour, we could not change it
 *    to behave the way HIWindowChangeClass does. 
 *    
 *    This function can convert a window between kDocumentWindowClass,
 *    kFloatingWindowClass, kUtilityWindowClass, and
 *    kMovableModalWindowClass only. It cannot change a document window
 *    into a plain window, for example. 
 *    
 *    The attributes of the window are adjusted to contain only those
 *    that are allowed for the new class. It is the caller’s
 *    responsibility to adjust them further after HIWindowChangeClass
 *    returns, if necessary.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose class to change.
 *    
 *    inWindowClass:
 *      The new window class.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowChangeClass(
  WindowRef     inWindow,
  WindowClass   inWindowClass)                                AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#endif  /* !__LP64__ */

#if !__LP64__
/*
 *  HIWindowGetCGWindowID()
 *  
 *  Summary:
 *    Returns the CoreGraphics window ID assigned by the window server
 *    when a window was created.
 *  
 *  Discussion:
 *    The window ID is not generally useful with other Carbon API, but
 *    may be usable with other Mac OS X APIs that require window IDs,
 *    such as OpenGL. You may convert a window ID back to a WindowRef
 *    using the HIWindowFromCGWindowID API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose window ID to return.
 *  
 *  Result:
 *    The window's window ID, or zero if the window is invalid.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CGWindowID 
HIWindowGetCGWindowID(WindowRef inWindow)                     AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowFromCGWindowID()
 *  
 *  Summary:
 *    Returns the window in the current process with a specified
 *    CGWindowID, if any.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindowID:
 *      A CoreGraphics window ID, as returned by HIWindowGetCGWindowID
 *      or CGWindowListCopyWindowInfo.
 *  
 *  Result:
 *    The WindowRef with the specified window ID, or NULL if no such
 *    window is found. A valid window ID for a window in another
 *    process will cause a NULL return value, since WindowRefs cannot
 *    be used across processes.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
HIWindowFromCGWindowID(CGWindowID inWindowID)                 AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • Window Flushing                                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowFlush()
 *  
 *  Summary:
 *    Flushes any dirty areas a window might have.
 *  
 *  Discussion:
 *    This routine allows you to manually flush dirty areas of a window
 *    to the screen. If called on a composited window, it will also
 *    cause invalid views in the window to be rendered. This API is
 *    preferred over calling QDFlushPortBuffer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to flush.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowFlush(WindowRef inWindow)                             AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • Window Backing Store Location                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Window backing buffer location constants are used with the
 *    HIWindowGet/SetBackingLocation APIs.
 */
enum {

  /*
   * The window server determines where to locate the window's backing
   * buffer.
   */
  kHIWindowBackingLocationDefault = 0,

  /*
   * The window's backing buffer is stored in video memory. In this
   * case, only Quartz may be used to draw into the window. Use of
   * QuickDraw or direct access will force the window's buffer back
   * into main memory.
   */
  kHIWindowBackingLocationVideoMemory = 1,

  /*
   * The window's backing buffer is stored in main memory. In this
   * case, Quartz, QuickDraw, and direct access are all allowed for
   * drawing into the window.
   */
  kHIWindowBackingLocationMainMemory = 2
};

typedef UInt32                          HIWindowBackingLocation;
#if !__LP64__
/*
 *  HIWindowSetBackingLocation()
 *  
 *  Summary:
 *    Sets the preferred location for a window's backing buffer.
 *  
 *  Discussion:
 *    In Mac OS X 10.5, the Quartz drawing API supports
 *    hardware-accelerated drawing to a window. In order to enable
 *    hardware acceleration, a window's buffer must be stored in video
 *    memory, rather than in main memory. By default, the location of a
 *    window's buffer is managed automatically by the Window Server.
 *    Your application can use this API to explicitly control a
 *    window's buffer location. You might, for example, need to ensure
 *    that the buffer is kept in main memory if you are drawing
 *    directly into the buffer without using Quartz. You might
 *    explicitly force a window's buffer into video memory if, after
 *    comparing performance with and without hardware acceleration, you
 *    find that acceleration is always a performance advantage for your
 *    application. 
 *    
 *    You may change the backing buffer location of a window at any
 *    time, regardless of whether the window is already visible or has
 *    been drawn to. However, changing the buffer location of a visible
 *    window requires a potentially significant amount of memory
 *    traffic between main and video memory, so it is best to set a
 *    window's buffer location once before showing the window, and not
 *    change it again. 
 *    
 *    The Window Server automatically moves a window's backing buffer
 *    into main memory if the buffer is accessed directly. This occurs
 *    if you draw into a window using QuickDraw, or if you acquire a
 *    pointer to the window buffer using the LockPortBits API. For best
 *    performance, if you know that you will be accessing a window's
 *    buffer using either QuickDraw or direct access, you should force
 *    the window's buffer into main memory before showing the window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing buffer location to set.
 *    
 *    inLocation:
 *      The window's new backing buffer location. You may pass
 *      kHIWindowBackingLocationDefault to allow the Window Server to
 *      determine the buffer location automatically.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetBackingLocation(
  WindowRef                 inWindow,
  HIWindowBackingLocation   inLocation)                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowGetBackingLocation()
 *  
 *  Summary:
 *    Retrieves the preferred location of a window's backing buffer,
 *    and its potential location based on hardware and software
 *    configuration.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing buffer location to retrieve.
 *    
 *    outPreferredLocation:
 *      On exit, contains the preferred location for the window's
 *      backing buffer. A newly created window's preferred location is
 *      kHIWindowBackingLocationDefault. If HIWindowSetBackingLocation
 *      has been called on a window, this parameter returns the value
 *      that was set. You may pass NULL if you don't need this
 *      information.
 *  
 *  Result:
 *    The potential location of the window's backing buffer based on
 *    hardware and software configuration. This value indicates whether
 *    the WindowServer is capable of storing the window's buffer in
 *    video or main memory. The determination is based on details of
 *    the current hardware and software configuration, such as whether
 *    the GPU can be used by the WindowServer to store window buffers
 *    and whether the QuartzGL feature is currently enabled for the
 *    entire system or the current application. The return value will
 *    always be either kHIWindowBackingLocationVideoMemory or
 *    kHIWindowBackingLocationMainMemory; it will never be
 *    kHIWindowBackingLocationDefault. Your application can use the
 *    return value to determine if the current system configuration is
 *    fast enough to support your desired level of drawing; if, after
 *    performance measurements, you determine that your software
 *    requires video-memory storage of window buffers for acceptable
 *    performance, you might alert the user if this function returns
 *    kHIWindowBackingLocationMainMemory. 
 *    
 *    Note that the return value does not indicate the current location
 *    of the window's backing buffer, only its potential location. The
 *    current location can change from one moment to the next. Your
 *    application generally only needs to know the capability for
 *    storing the backing buffer in video memory, not the actual
 *    current location.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIWindowBackingLocation 
HIWindowGetBackingLocation(
  WindowRef                  inWindow,
  HIWindowBackingLocation *  outPreferredLocation)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • Window Buffer Visibility To Other Processes                                       */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Window sharing constants are used with the
 *    HIWindowGet/SetSharingType APIs.
 */
enum {

  /*
   * Window contents may not be read by another process.
   */
  kHIWindowSharingNone          = 0,

  /*
   * Window contents may be read but not modified by another process.
   */
  kHIWindowSharingReadOnly      = 1,

  /*
   * Window contents may be read or modified by another process.
   */
  kHIWindowSharingReadWrite     = 2
};

typedef UInt32                          HIWindowSharingType;
#if !__LP64__
/*
 *  HIWindowSetSharingType()
 *  
 *  Summary:
 *    Sets the visibility to other processes of a window's backing
 *    buffer contents.
 *  
 *  Discussion:
 *    In Mac OS X 10.5, a window's backing buffer may be retrieved
 *    using the CGWindowListCreateImage and
 *    CGWindowListCreateImageFromArray APIs. Some windows may have
 *    content that should not be accessible via these APIs, due to
 *    security concerns. Your application can use this API to control
 *    how visible a window's backing buffer should be to other
 *    processes. 
 *    
 *    Note that it is also possible to read the entire contents of a
 *    display using OpenGL; setting a window's sharing type with this
 *    API does not prevent OpenGL from being able to read the window's
 *    content. For example, the built-in screen-sharing software in Mac
 *    OS X Mac OS X 10.5 uses OpenGL and a window's content will still
 *    be visible via screen-sharing even if its sharing type has been
 *    set to None. 
 *    
 *    A window's default sharing state on creation is
 *    kHIWindowSharingReadOnly.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing buffer visibility to set.
 *    
 *    inType:
 *      The window's new backing buffer visibility.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetSharingType(
  WindowRef             inWindow,
  HIWindowSharingType   inType)                               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowGetSharingType()
 *  
 *  Summary:
 *    Retrieves the visibility to other processes of a window's backing
 *    buffer contents.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing buffer visibility to retrieve.
 *  
 *  Result:
 *    The window's backing buffer visibility.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIWindowSharingType 
HIWindowGetSharingType(WindowRef inWindow)                    AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • Window Modality                                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  WindowModality
 *  
 */
typedef UInt32 WindowModality;
enum {

  /*
   * A window does not prevent interaction with any other window in the
   * system.
   */
  kWindowModalityNone           = 0,

  /*
   * A window prevents interaction with all other windows in the
   * system. Only available in CarbonLib. Mapped to
   * kWindowModalityAppModal in Mac OS X.
   */
  kWindowModalitySystemModal    = 1,

  /*
   * A window prevents interaction with other windows in the same
   * process.
   */
  kWindowModalityAppModal       = 2,

  /*
   * A window prevents interaction with a single other window.
   */
  kWindowModalityWindowModal    = 3
};


#if !__LP64__
/*
 *  SetWindowModality()
 *  
 *  Summary:
 *    Sets the modality of a window.
 *  
 *  Discussion:
 *    The modality of a window is used by the Carbon event manager to
 *    automatically determine appropriate event handling.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose modality to set.
 *    
 *    inModalKind:
 *      The new modality for the window.
 *    
 *    inUnavailableWindow:
 *      If the window is becoming window-modal, this parameter
 *      specifies the window to which the inWindow parameter is modal.
 *      The unavailableWindow will not be available while inWindow is
 *      in window-modal state.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowModality(
  WindowRef        inWindow,
  WindowModality   inModalKind,
  WindowRef        inUnavailableWindow)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowModality()
 *  
 *  Summary:
 *    Retrieves the modality of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose modality to retrieve.
 *    
 *    outModalKind:
 *      On exit, contains the modality of the window.
 *    
 *    outUnavailableWindow:
 *      On exit, if the window is window-modal, contains the target
 *      window of the specified window's modality.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowModality(
  WindowRef         inWindow,
  WindowModality *  outModalKind,
  WindowRef *       outUnavailableWindow)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowIsDocumentModalTarget()
 *  
 *  Summary:
 *    Determines if a window is currently the target window of another
 *    document modal window, such as a sheet.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window in question.
 *    
 *    outOwner:
 *      If inWindow is the target of a document modal window, outOwner
 *      is set to the document modal window on exit. If this function
 *      does not return true, outOwner is undefined. You may pass NULL
 *      if you don't want the owner's window ref.
 *  
 *  Result:
 *    A boolean result. A true result indicates that inWindow is
 *    currently the target of a document modal window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowIsDocumentModalTarget(
  WindowRef    inWindow,
  WindowRef *  outOwner)       /* can be NULL */              AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Floating Windows                                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
   Routines available from Mac OS 8.6 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  ShowFloatingWindows()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
ShowFloatingWindows(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HideFloatingWindows()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
HideFloatingWindows(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  AreFloatingWindowsVisible()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
AreFloatingWindowsVisible(void)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Groups                                                                      */
/*                                                                                      */
/* The Window Group API allows the creation and management of groups of windows,        */
/* allowing control of the z-order, activation, and positioning of the windows.         */
/* Internally to the Window Manager, each of the standard window tiers (document,       */
/* toolbar, floating, modal, utility, help, and overlay) is implemented as a window     */
/* group; you can access the window group for a class with GetWindowGroupOfClass.       */
/* You can create your own window groups, if you would like your windows to float,      */
/* for example, above the floating window layer but below the modal layer. It is        */
/* also possible to create more complex hierarchical arrangements of window groups.     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */

typedef struct OpaqueWindowGroupRef*    WindowGroupRef;
/* may be passed as the "behindWindow" parameter to NewCWindow and SendBehind*/
#define kFirstWindowOfGroup             ((WindowRef)(-1))
#define kLastWindowOfGroup              ((WindowRef)NULL)
/* may be passed as the "behindGroup" parameter to SendWindowGroupBehind*/
#define kFirstWindowGroup               ((WindowGroupRef)(-1))
#define kLastWindowGroup                ((WindowGroupRef)NULL)

/*
 *  WindowGroupAttributes
 *  
 *  Summary:
 *    These are attributes that may be applied to a window group.
 */
typedef UInt32 WindowGroupAttributes;
enum {

  /*
   * Makes the group behave somewhat as a layer of windows that move
   * together. When any window in the group is brought to the front of
   * the group, the entire group will also be brought to the front of
   * the containing group's child hierarchy.
   */
  kWindowGroupAttrSelectAsLayer = 1 << 0,

  /*
   * The positions of the contents of this group with respect to each
   * other cannot be changed. When one item moves, all other items are
   * moved simultaneously. Note that when one window in the group is
   * moved, the bounds of the other windows in the group may be updated
   * asynchronously.
   */
  kWindowGroupAttrMoveTogether  = 1 << 1,

  /*
   * The z-order of the contents of this group with respect to each
   * other cannot be changed. When one item changes z-order, all other
   * items are moved simultaneously. For purposes of z-ordering, the
   * group and all its subgroups are effectively treated as if they
   * were a single window in the parent group of this group.
   */
  kWindowGroupAttrLayerTogether = 1 << 2,

  /*
   * The active state of the windows in this group is shared. The
   * windows in the group are activated or deactivated according to the
   * activation scope of the group, but when any window in the group
   * changes activation, all other windows change to match.
   */
  kWindowGroupAttrSharedActivation = 1 << 3,

  /*
   * When any window in this group is collapsed, all other windows in
   * this group are hidden. All subgroups of this group are also
   * examined for the HideOnCollapse attribute, and any windows of any
   * subgroup with this attribute are also hidden. All windows will be
   * shown again when the collapsed window is expanded.
   */
  kWindowGroupAttrHideOnCollapse = 1 << 4,

  /*
   * This window group's window level should be left unchanged. If this
   * attribute is not specified, this window group's window level will
   * be promoted to a value equal to the level of the next FixedLevel
   * window group beneath it in the window group hierarchy.
   */
  kWindowGroupAttrFixedLevel    = 1 << 5
};


/*
 *  WindowActivationScope
 *  
 *  Discussion:
 *    Every window has a WindowActivationScope. It defines how windows
 *    are activated by the Window Manager with respect to other windows
 *    in the window’s group and in the current process.
 */
typedef UInt32 WindowActivationScope;
enum {

  /*
   * Windows with this scope are never activated by the Window Manager.
   * This should be used when the window’s visual state does not change
   * based on activation (for example, tooltip windows), or when the
   * client wishes to manually control all activation. The window owner
   * is free to explicitly activate or deactivate windows using the
   * ActivateWindow API.
   */
  kWindowActivationScopeNone    = 0,

  /*
   * Windows with this scope are always active if visible. Windows with
   * this scope are unaffected by the activation state of other
   * windows. This activation scope is automatically used by floating
   * windows.
   */
  kWindowActivationScopeIndependent = 1,

  /*
   * Windows with this scope are activated relative to other windows
   * with the same scope in the current process. Only one window with
   * this scope can be active in the entire process. This activation
   * scope is automatically used by document and dialog windows.
   */
  kWindowActivationScopeAll     = 2
};


/*
 *  Summary:
 *    These are constants that can be used for the inNextGroup
 *    parameter to GetWindowGroupSibling.
 */
enum {

  /*
   * Indicates that GetWindowGroupSibling should return the next deeper
   * sibling group in the z-order.
   */
  kNextWindowGroup              = true,

  /*
   * Indicates that GetWindowGroupSibling should return the next higher
   * sibling group in the z-order.
   */
  kPreviousWindowGroup          = false
};


/*
 *  WindowGroupContentOptions
 *  
 *  Discussion:
 *    Window group contents options are used to control what group
 *    content is counted or returned by the CountWindowGroupContents
 *    and GetWindowGroupContents APIs.
 */
typedef UInt32 WindowGroupContentOptions;
enum {

  /*
   * Indicates that Count/GetWindowGroupContents should return the
   * windows in a group. If this option is not set, these APIs return
   * the groups in a group.
   */
  kWindowGroupContentsReturnWindows = 1 << 0,

  /*
   * Indicates that Count/GetWindowGroupContents should include the
   * contents of groups contained by the specified group. If this
   * option is not set, these APIs only return information about the
   * specified group’s contents.
   */
  kWindowGroupContentsRecurse   = 1 << 1,

  /*
   * Indicates that Count/GetWindowGroupContents should only include
   * visible windows. Only valid when kWindowGroupContentsReturnWindows
   * is specified.
   */
  kWindowGroupContentsVisible   = 1 << 2
};


/*----------------------------------------------------------------------------------*/
/*  • Group creation, destruction, and refcounting                                  */
/*----------------------------------------------------------------------------------*/
#if !__LP64__
/*
 *  CreateWindowGroup()
 *  
 *  Summary:
 *    Creates a new window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inAttributes:
 *      Attributes for the new window group.
 *    
 *    outGroup:
 *      On exit, contains the new window group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateWindowGroup(
  WindowGroupAttributes   inAttributes,
  WindowGroupRef *        outGroup)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RetainWindowGroup()
 *  
 *  Summary:
 *    Increments the refcount of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose refcount to increment.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RetainWindowGroup(WindowGroupRef inGroup)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ReleaseWindowGroup()
 *  
 *  Summary:
 *    Releases a refcount on a window group. If the refcount goes to
 *    zero, the group is destroyed, and a refcount is released from all
 *    contained objects.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose refcount to decrement.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReleaseWindowGroup(WindowGroupRef inGroup)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupRetainCount()
 *  
 *  Summary:
 *    Returns the refcount of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose refcount to return.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
GetWindowGroupRetainCount(WindowGroupRef inGroup)             AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


/*
 *  GetWindowGroupOfClass()
 *  
 *  Summary:
 *    Gets the window group in which windows of a given class are
 *    placed.
 *  
 *  Discussion:
 *    The Window Manager uses window groups internally to manage the
 *    ordering of windows of different classes. In some cases, multiple
 *    classes are placed within the same group; for example, windows
 *    from all of the modal and alert window classes are placed into
 *    the same modal window group. The refcount of the group returned
 *    by this API is not incremented, and the caller does not need to
 *    release the reference.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    windowClass:
 *      The class whose window group to retrieve. You may pass
 *      kAllWindowClasses to retrieve the root window group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowGroupRef 
GetWindowGroupOfClass(WindowClass windowClass)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*----------------------------------------------------------------------------------*/
/*  • Group name, attributes, and level                                             */
/*----------------------------------------------------------------------------------*/
/*
 *  SetWindowGroupName()
 *  
 *  Summary:
 *    Sets the name of a window group.
 *  
 *  Discussion:
 *    The name of a window group is never displayed to the user.
 *    However, it is displayed by debugging functions such as
 *    DebugPrintWindowGroup. This can be very useful when debugging the
 *    structure of your window groups.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose name to set.
 *    
 *    inName:
 *      The name of the group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroupName(
  WindowGroupRef   inGroup,
  CFStringRef      inName)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyWindowGroupName()
 *  
 *  Summary:
 *    Returns a copy of the name of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose name to retrieve.
 *    
 *    outName:
 *      On exit, contains the name of the group. It is the caller's
 *      responsibility to release the name.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CopyWindowGroupName(
  WindowGroupRef   inGroup,
  CFStringRef *    outName)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupAttributes()
 *  
 *  Summary:
 *    Retrieves the attributes of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose attributes to retrieve.
 *    
 *    outAttributes:
 *      On exit, the group’s attributes.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowGroupAttributes(
  WindowGroupRef           inGroup,
  WindowGroupAttributes *  outAttributes)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ChangeWindowGroupAttributes()
 *  
 *  Summary:
 *    Changes the attributes of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose attributes to change.
 *    
 *    setTheseAttributes:
 *      The attributes to set.
 *    
 *    clearTheseAttributes:
 *      The attributes to clear.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ChangeWindowGroupAttributes(
  WindowGroupRef          inGroup,
  WindowGroupAttributes   setTheseAttributes,
  WindowGroupAttributes   clearTheseAttributes)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowGroupLevel()
 *  
 *  Summary:
 *    Sets the CoreGraphics window group level of windows in a group.
 *    
 *    PLEASE NOTE: Apple does not recommend using this API for most
 *    window-ordering purposes. Rather than explicitly setting the
 *    level of a window group, it is preferable to position the group
 *    in the window group hierarchy so that it is between two standard
 *    groups that bracket the desired window level. Doing so will cause
 *    your group to automatically adopt the window level of the
 *    standard group beneath it. Your code will be more compatible with
 *    future versions of Mac OS X if you avoid setting the window level
 *    of your groups explicitly. 
 *    
 *    The most common case where it is appropriate to explicitly set a
 *    window level for your group is when your group's windows need to
 *    be positioned above or below all windows in the standard window
 *    groups. In these cases, you should position your group in the
 *    group hierarchy above or below all standard groups, and then set
 *    the window group level explicitly, if necessary.
 *  
 *  Discussion:
 *    CoreGraphics windows (used to implement all windows in Carbon and
 *    Cocoa applications on Mac OS X) are divided into layers specified
 *    by a window level. Standard window levels are listed in
 *    <CoreGraphics/CGWindowLevel.h>. By default, a new window group
 *    has a window level of kCGNormalWindowLevel. 
 *    
 *    When a window is placed into a window group, its window level is
 *    determined by the window level of its "base group". This is the
 *    containing group that is a child of the root group. For example,
 *    if group A is a child of the root group, and group B is a child
 *    of group A, and window C is in group B, then window C's base
 *    group is group A, and group A's window level determines the level
 *    of window C. 
 *    
 *    SetWindowGroupLevel only allows changing the window level of
 *    groups that are children of the root group. It returns paramErr
 *    for other groups, since a group that is not a child of the root
 *    group is not a base group and changing its level has no effect.
 *    
 *    
 *    Changing the level of a group also changes the level of all
 *    windows currently contained by the group. 
 *    
 *    In Mac OS X 10.4 and later, SetWindowGroupLevel will set all
 *    three window levels associated with a window group: the Active,
 *    Inactive, and Promoted levels. It will then immediately determine
 *    if the Active level needs to be promoted to a larger value, and
 *    if so, set the Promoted level to that value.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The window group whose level to change.
 *    
 *    inLevel:
 *      The new level for the windows in this group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroupLevel(
  WindowGroupRef   inGroup,
  SInt32           inLevel)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupLevel()
 *  
 *  Summary:
 *    Gets the CoreGraphics window group level of windows in a group.
 *  
 *  Discussion:
 *    In Mac OS X 10.4 and later, GetWindowGroupLevel will return
 *    either the Promoted window level or the Inactive window level,
 *    depending on whether the application is active or inactive.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The window group whose level to return.
 *    
 *    outLevel:
 *      On exit, contains the window level of the windows in this group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowGroupLevel(
  WindowGroupRef   inGroup,
  SInt32 *         outLevel)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Parameters to GetWindowGroupLevelOfType and
 *    SetWindowGroupLevelOfType
 */
enum {

  /*
   * The window level that is nominally used for windows in the group
   * when the application is active. However, if a group with a higher
   * window level is positioned below group in the window group
   * hierarchy, this group's active level will be promoted to match the
   * level of the group in front of it. You can determine the actual
   * window level in use for a group using the
   * kWindowGroupLevelPromoted constant.
   */
  kWindowGroupLevelActive       = 1,

  /*
   * The window level that is used for windows in the group when the
   * application is inactive.
   */
  kWindowGroupLevelInactive     = 2,

  /*
   * The window level that is actually used for windows in the group
   * when the application is active. This level will either be the same
   * as the Active window level, or will be a larger value to match the
   * level of a group below this group. 
   * 
   * We do not recommend setting the Promoted window level explicitly,
   * because the promoted level is reset by the Window Manager whenever
   * the window group hierarchy structure changes, and therefore any
   * changes that you may make to the promoted level can be
   * overwritten. In general you should only use this API to set the
   * Active and Inactive window levels.
   */
  kWindowGroupLevelPromoted     = 3
};

#if !__LP64__
/*
 *  SetWindowGroupLevelOfType()
 *  
 *  Summary:
 *    Sets a CoreGraphics window group level of a window group.
 *    
 *    
 *    PLEASE NOTE: Apple does not recommend using this API for most
 *    window-ordering purposes. Rather than explicitly setting the
 *    level of a window group, it is preferable to position the group
 *    in the window group hierarchy so that it is between two standard
 *    groups that bracket the desired window level. Doing so will cause
 *    your group to automatically adopt the window level of the
 *    standard group beneath it. Your code will be more compatible with
 *    future versions of Mac OS X if you avoid setting the window level
 *    of your groups explicitly. 
 *    
 *    The most common case where it is appropriate to explicitly set a
 *    window level for your group is when your group's windows need to
 *    be positioned above or below all windows in the standard window
 *    groups. In these cases, you should position your group in the
 *    group hierarchy above or below all standard groups, and then set
 *    the window group level explicitly, if necessary.
 *  
 *  Discussion:
 *    See the SetWindowGroupLevel API for a general discussion of
 *    window levels and window groups. 
 *    
 *    In Mac OS X 10.4 and later, a window group may have multiple
 *    window levels associated with it; one level for when the
 *    application is active, and another for when the application is
 *    inactive. The Window Manager automatically switches each group's
 *    level as the application becomes active or inactive. This API can
 *    be used to set each level associated with a group. 
 *    
 *    This API can also be used to set the promoted window level that
 *    is actually used for windows in the group; however, we do not
 *    recommend this usage, because the promoted window level is reset
 *    by the Window Manager whenever the window group hierarchy
 *    structure changes, and therefore any changes that you may make to
 *    the promoted level can be overwritten. In general you should only
 *    use this API to set the Active and Inactive window levels.
 *    
 *    
 *    When setting the Active level of a group with the FixedLevel
 *    window group attribute, this API will automatically also set the
 *    Promoted level to the same value, and also update the Promoted
 *    level of any non-FixedLevel groups above the group being modified.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The window group whose level to change.
 *    
 *    inLevelType:
 *      The level type to change (one of kWindowGroupLevelActive,
 *      kWindowGroupLevelInactive, or kWindowGroupLevelPromoted).
 *    
 *    inLevel:
 *      The new level for the windows in this group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroupLevelOfType(
  WindowGroupRef   inGroup,
  UInt32           inLevelType,
  CGWindowLevel    inLevel)                                   AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  GetWindowGroupLevelOfType()
 *  
 *  Summary:
 *    Gets a CoreGraphics window level of a window group.
 *  
 *  Discussion:
 *    See the SetWindowGroupLevel API for a general discussion of
 *    window levels and window groups. 
 *    
 *    In Mac OS X 10.4 and later, a window group may have multiple
 *    window levels associated with it; one level for when the
 *    application is active, and another for when the application is
 *    inactive. The Window Manager automatically switches each group's
 *    level as the application becomes active or inactive. The
 *    GetWindowGroupLevelOfType API can be used to get each level
 *    associated with a group, including the promoted window level that
 *    is actually in use for windows in the group while the application
 *    is active.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The window group whose level to retrieve.
 *    
 *    inLevelType:
 *      The level type to retrieve (one of kWindowGroupLevelActive,
 *      kWindowGroupLevelInactive, or kWindowGroupLevelPromoted).
 *    
 *    outLevel:
 *      On exit, the level for the windows in this group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowGroupLevelOfType(
  WindowGroupRef   inGroup,
  UInt32           inLevelType,
  CGWindowLevel *  outLevel)                                  AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



/*----------------------------------------------------------------------------------*/
/*  • Group z-ordering                                                              */
/*----------------------------------------------------------------------------------*/
/*
 *  SendWindowGroupBehind()
 *  
 *  Summary:
 *    Changes the z-order of a group, if the group does not have the
 *    kWindowGroupAttributeLayerTogether attribute set.
 *  
 *  Discussion:
 *    SendWindowGroupBehind currently requires that the group being
 *    moved and the behindGroup have the same parent group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose z-order to change.
 *    
 *    behindGroup:
 *      The group behind which to position the specified group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SendWindowGroupBehind(
  WindowGroupRef   inGroup,
  WindowGroupRef   behindGroup)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*----------------------------------------------------------------------------------*/
/*  • Group containment hierarchy manipulation                                      */
/*----------------------------------------------------------------------------------*/
/*
 *  GetWindowGroup()
 *  
 *  Summary:
 *    Gets the window group that contains a window.
 *  
 *  Discussion:
 *    The refcount of the group returned by this API is not
 *    incremented, and the caller does not need to release the
 *    reference.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose containing group to retrieve.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowGroupRef 
GetWindowGroup(WindowRef inWindow)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowGroup()
 *  
 *  Summary:
 *    Sets the window group that contains a window.
 *  
 *  Discussion:
 *    The window’s z-order relative to windows in the current process
 *    may also be changed by this API. If the new window group is
 *    z-ordered above the window’s current group, the window will be
 *    placed at the end of the new group. If the new window group is
 *    z-ordered below the window’s current group, the window will be
 *    placed at the top of the new group. You may not place a window
 *    directly into the root group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose group to change.
 *    
 *    inNewGroup:
 *      The new containing group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroup(
  WindowRef        inWindow,
  WindowGroupRef   inNewGroup)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowContainedInGroup()
 *  
 *  Summary:
 *    Indicates whether a window is contained within a group or any of
 *    its subgroups.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose containment to examine.
 *    
 *    inGroup:
 *      The group that might contain the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsWindowContainedInGroup(
  WindowRef        inWindow,
  WindowGroupRef   inGroup)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupParent()
 *  
 *  Summary:
 *    Gets the window group that contains a group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose containing group to retrieve.
 *    
 *    outGroup:
 *      On exit, the containing window group of the group. The group’s
 *      refcount is not incremented by this API, and the caller does
 *      not need to release the reference.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowGroupRef 
GetWindowGroupParent(WindowGroupRef inGroup)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowGroupParent()
 *  
 *  Summary:
 *    Sets the window group that contains a group.
 *  
 *  Discussion:
 *    SetWindowGroupParent currently requires that the group have no
 *    windows in it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose containing group to change.
 *    
 *    inNewGroup:
 *      The new containing group.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroupParent(
  WindowGroupRef   inGroup,
  WindowGroupRef   inNewGroup)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupSibling()
 *  
 *  Summary:
 *    Returns the next or previous group of a window group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose sibling to return.
 *    
 *    inNextGroup:
 *      True to return the next sibling, false to return the previous
 *      sibling.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowGroupRef 
GetWindowGroupSibling(
  WindowGroupRef   inGroup,
  Boolean          inNextGroup)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupOwner()
 *  
 *  Summary:
 *    Returns the window that owns a window group, or NULL if none.
 *  
 *  Discussion:
 *    A window may own one or more window groups. The windows in an
 *    owned window group will always be z-ordered above the owner
 *    window. Whenever the owner window changes z-order, the windows in
 *    the groups owned by the window will be moved also.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose owner to retrieve.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetWindowGroupOwner(WindowGroupRef inGroup)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowGroupOwner()
 *  
 *  Summary:
 *    Sets the window that owns a window group.
 *  
 *  Discussion:
 *    The group and the window must have the same parent group.
 *    SetWindowGroupOwner currently requires that the group have no
 *    windows in it.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose owner to set.
 *    
 *    inWindow:
 *      The group's new owner.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowGroupOwner(
  WindowGroupRef   inGroup,
  WindowRef        inWindow)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*----------------------------------------------------------------------------------*/
/*  • Inspection of group contents                                                  */
/*----------------------------------------------------------------------------------*/

/*
 *  CountWindowGroupContents()
 *  
 *  Summary:
 *    Counts the windows or groups contained in a group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose contents to count.
 *    
 *    inOptions:
 *      Specifies how to count the group’s contents.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
CountWindowGroupContents(
  WindowGroupRef              inGroup,
  WindowGroupContentOptions   inOptions)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGroupContents()
 *  
 *  Summary:
 *    Retrieves the windows or groups contained in a group.
 *  
 *  Discussion:
 *    The windows or groups returned by this API will be placed into
 *    the output buffer in z-order, from highest to lowest.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group whose contents to retrieve.
 *    
 *    inOptions:
 *      Specifies which content to retrieve.
 *    
 *    inAllowedItems:
 *      The number of items that will fit in the output buffer.
 *    
 *    outNumItems:
 *      On exit, the number of items that were returned. May be NULL.
 *    
 *    outItems:
 *      On entry, points to enough memory to hold inAllowedSize
 *      WindowRefs or WindowGroupRefs. On exit, contains *outNumItems
 *      WindowRefs or WindowGroupRefs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowGroupContents(
  WindowGroupRef              inGroup,
  WindowGroupContentOptions   inOptions,
  ItemCount                   inAllowedItems,
  ItemCount *                 outNumItems,          /* can be NULL */
  void **                     outItems)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetIndexedWindow()
 *  
 *  Summary:
 *    Retrieves a specified window from a group.
 *  
 *  Discussion:
 *    GetIndexedWindow is provided as an easier way to get a particular
 *    window from a group than using GetWindowGroupContents. If you
 *    only need to retrieve, say, the last window in a group, it is
 *    easier and more efficient to use GetIndexedWindow. If you need to
 *    retrieve all the windows in a group, it is more efficient to use
 *    GetWindowGroupContents.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inGroup:
 *      The group containing the window to retrieve.
 *    
 *    inIndex:
 *      The index of the window. This parameter may range from 1 to
 *      CountWindowGroupContents( inGroup,
 *      kWindowGroupContentsReturnWindows | inOptions );
 *    
 *    inOptions:
 *      Indicates how to locate the specified window.
 *      kWindowGroupContentsReturnWindows is implied by this API and
 *      does not need to be explicitly specified.
 *    
 *    outWindow:
 *      On exit, the window at the specified index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetIndexedWindow(
  WindowGroupRef              inGroup,
  ItemCount                   inIndex,
  WindowGroupContentOptions   inOptions,
  WindowRef *                 outWindow)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowIndex()
 *  
 *  Summary:
 *    Retrieves the z-order index of a window inside a group.
 *  
 *  Discussion:
 *    The z-order index of a window is its relative position in z-order
 *    inside a group. The index ranges from 1 to the number of windows
 *    in the group.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose z-order index to retrieve.
 *    
 *    inStartGroup:
 *      The group on which to base the z-order index. This should be
 *      either the containing group of the window, or NULL. If NULL,
 *      this API returns the z-order index of the window across the
 *      entire process.
 *    
 *    inOptions:
 *      Indicates how to enumerate the specified window.
 *      kWindowGroupContentsReturnWindows is implied by this API and
 *      does not need to be explicitly specified.
 *    
 *    outIndex:
 *      On exit, contains the window’s z-order index.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowIndex(
  WindowRef                   inWindow,
  WindowGroupRef              inStartGroup,
  WindowGroupContentOptions   inOptions,
  ItemCount *                 outIndex)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*----------------------------------------------------------------------------------*/
/*  • Window activation                                                             */
/*----------------------------------------------------------------------------------*/
/*
 *  ActiveNonFloatingWindow()
 *  
 *  Summary:
 *    Returns the window, among all windows with activation scope of
 *    kWindowActivationScopeAll, that is considered active.
 *  
 *  Discussion:
 *    The Mac OS 8.5 Window Manager introduced the
 *    FrontNonFloatingWindow API, which was designed to return the
 *    window that should be considered active by the application. With
 *    the advent of window groups, it is now possible to have a window
 *    that looks active (is highlighted, and accepts keyboard input)
 *    but to have other non-floating windows grouped above the active
 *    window. The ActiveNonFloatingWindow API returns the active window
 *    regardless of where it is positioned in the z-order. Most code
 *    that currently uses FrontNonFloatingWindow or
 *    GetFrontWindowOfClass(kDocumentWindowClass) to get the active
 *    window should use ActiveNonFloatingWindow instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
ActiveNonFloatingWindow(void)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowActive()
 *  
 *  Summary:
 *    Indicates whether a window is active.
 *  
 *  Discussion:
 *    Whether a window is considered active is determined by its
 *    activation scope, hiliting, and z-order. For windows that have an
 *    activation scope of kWindowActivationScopeAll, a window is active
 *    if it is the window returned by the ActiveNonFloatingWindow API,
 *    or if it is in the same window group as the window returned by
 *    that API and the window group has the
 *    kWindowGroupAttrSharedActivation attribute. For windows that have
 *    some other activation scope, the window is active if its window
 *    frame is hilited and the window is the frontmost window in its
 *    window group. 
 *    
 *    The IsWindowActive API does not indicate whether the window has
 *    keyboard focus. To get the window with keyboard focus, use
 *    GetUserFocusWindow().
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose active state to retrieve.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsWindowActive(WindowRef inWindow)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ActivateWindow()
 *  
 *  Summary:
 *    Activates or deactivates a window.
 *  
 *  Discussion:
 *    Window activation consists of two steps: hiliting the window
 *    frame and sending an activate event to the window. ActivateWindow
 *    handles both of these steps and also updates internal Window
 *    Manager state. If you just need to hilite the window frame, you
 *    may use HiliteWindow. If you need to send an activate event, you
 *    should always use ActivateWindow rather than creating and sending
 *    the event yourself.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to activate or deactivate.
 *    
 *    inActivate:
 *      Whether to activate or deactivate the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ActivateWindow(
  WindowRef   inWindow,
  Boolean     inActivate)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowActivationScope()
 *  
 *  Summary:
 *    Retrieves a window’s activation scope.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose activation scope to retrieve.
 *    
 *    outScope:
 *      On exit, the window’s activation scope.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowActivationScope(
  WindowRef                inWindow,
  WindowActivationScope *  outScope)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowActivationScope()
 *  
 *  Summary:
 *    Sets a window’s activation scope.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose activation scope to set.
 *    
 *    inScope:
 *      The new activation scope.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowActivationScope(
  WindowRef               inWindow,
  WindowActivationScope   inScope)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*--------------------------------------------------------------------------------------*/
/*  • Window Focus                                                                      */
/*                                                                                      */
/* The 'user focus' is where keyboard input goes. We also use the term 'key' applied    */
/* to windows to mean this. The user focus window is normally the active non-floating   */
/* window or dialog. It is possible to make a floater get the focus, however, by calling*/
/* SetUserFocusWindow. After that call, the event model will automatically route key    */
/* input to the active keyboard focus of that window, if any. Passing kUserFocusAuto    */
/* into the window parameter tells the toolbox to pick what it considers to be the best */
/* candidate for focus. You should call this to restore focus, rather than getting the  */
/* focus, setting it to a special window, and then restoring to the saved focus. There  */
/* are cases, however, when you might want to restore to an explicit window, but the    */
/* typical usage should just be to restore to the kUserFocusAuto focus.                 */
/*                                                                                      */
/* Keep in mind that setting the focus will only last until you restore focus, or the   */
/* user starts clicking in other windows. When that happens, the toolbox will auto-     */
/* redirect the user focus to a newly selected window.                                  */
/*--------------------------------------------------------------------------------------*/
/* pick the most appropriate window for focus*/
#endif  /* !__LP64__ */

#define kUserFocusAuto                  ((WindowRef)(-1))
#if !__LP64__
/*
 *  SetUserFocusWindow()
 *  
 *  Summary:
 *    Sets the window that receives keyboard input.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window that should receive user input. You may also pass
 *      kUserFocusAuto to cause the toolbox to pick a default window
 *      for user focus (typically, this will be the active window).
 *      
 *      
 *      If the focus changes to a new window, the
 *      kEventWindowFocusAcquired Carbon event will be sent to the
 *      newly focused window, and the kEventWindowFocusRelinquish
 *      Carbon event will be sent to the previously focused window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetUserFocusWindow(WindowRef inWindow)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetUserFocusWindow()
 *  
 *  Summary:
 *    Returns the window that receives keyboard input.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The window that is currently receiving keyboard input.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetUserFocusWindow(void)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowShowsFocus()
 *  
 *  Summary:
 *    Returns whether a window's content should show focus indicators,
 *    such as focus rings.
 *  
 *  Discussion:
 *    This API returns true if the window is either the modeless focus
 *    or the effective focus.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose status to return.
 *  
 *  Result:
 *    Whether this window's content should show focus indicators.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowShowsFocus(WindowRef inWindow)                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*--------------------------------------------------------------------------------------*/
/*  • Default/Cancel buttons                                                            */
/*                                                                                      */
/* In our quest to eliminate the need for dialogs when using the new event model, we    */
/* have added the following routines which add dialog-like button control to normal     */
/* windows. With these routines, you can set the default and cancel buttons for a       */
/* window; these work just like the corresponding concepts in dialogs, and when         */
/* present, the standard toolbox handlers will handle keyboard input mapping to these   */
/* buttons. This means that pressing return or enter will 'press' the default button    */
/* and escape or command-period will 'press' the cancel button.                         */
/*--------------------------------------------------------------------------------------*/

/*
 *  SetWindowDefaultButton()
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
SetWindowDefaultButton(
  WindowRef    inWindow,
  ControlRef   inControl)       /* can be NULL */             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowCancelButton()
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
SetWindowCancelButton(
  WindowRef    inWindow,
  ControlRef   inControl)       /* can be NULL */             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowDefaultButton()
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
GetWindowDefaultButton(
  WindowRef     inWindow,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowCancelButton()
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
GetWindowCancelButton(
  WindowRef     inWindow,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*----------------------------------------------------------------------------------*/
/*  • ThemeBrush and ThemeTextColor support                                         */
/*----------------------------------------------------------------------------------*/
/*
 *  SetThemeWindowBackground()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
SetThemeWindowBackground(
  WindowRef    inWindow,
  ThemeBrush   inBrush,
  Boolean      inUpdate)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowGetThemeBackground()
 *  
 *  Summary:
 *    Gets the the window theme background brush.
 *  
 *  Discussion:
 *    Gets the the window theme background brush which may have been
 *    previously set by SetThemeWindowBackground.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window from which to get the brush.
 *    
 *    outThemeBrush:
 *      On return, inWindow's background brush.
 *  
 *  Result:
 *    An operating system result code. If no brush is found
 *    themeNoAppropriateBrushErr will be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetThemeBackground(
  WindowRef     inWindow,
  ThemeBrush *  outThemeBrush)                                AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  SetThemeTextColorForWindow()
 *  
 *  Summary:
 *    Sets a text color which contrasts with a theme brush.
 *  
 *  Discussion:
 *    SetThemeTextColorForWindow sets a text color in the specified
 *    window's port that contrasts with the theme brush associated with
 *    the specified window, and also matches the inActive parameter.
 *    Only a subset of the theme brushes have theme text colors:
 *    currently (as of Mac OS 9 and Mac OS X 10.5), the Alert, Dialog,
 *    ModelessDialog, and Notification brushes have corresponding text
 *    colors. For any other brush, SetThemeTextColorForWindow returns
 *    themeNoAppropriateBrushErr and does not modify the text color.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose text color to change.
 *    
 *    inActive:
 *      Whether the text color should indicate an active or inactive
 *      state.
 *    
 *    inDepth:
 *      The bit depth of the window's port.
 *    
 *    inColorDev:
 *      Whether the window's port is color or black&white.
 *  
 *  Result:
 *    An operating system result code, including
 *    themeNoAppropriateBrushErr if the specified theme brush does not
 *    have a corresponding theme text color.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.1 and later
 */
extern OSStatus 
SetThemeTextColorForWindow(
  WindowRef   inWindow,
  Boolean     inActive,
  SInt16      inDepth,
  Boolean     inColorDev)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Background Image                                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/
/*
 *  SetWindowContentColor()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowContentColor(
  WindowRef         window,
  const RGBColor *  color)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowContentColor()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowContentColor(
  WindowRef   window,
  RGBColor *  color)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Routines available from Mac OS 8.5 forward*/
/*
 *  GetWindowContentPattern()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowContentPattern(
  WindowRef      window,
  PixPatHandle   outPixPat)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowContentPattern()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowContentPattern(
  WindowRef      window,
  PixPatHandle   pixPat)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Colorspace and Depth                                                               */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowSetColorSpace()
 *  
 *  Summary:
 *    Sets the colorspace used for window content that is drawn by the
 *    application with no other specified colorspace.
 *  
 *  Discussion:
 *    Typically this API should be used immediately after a window is
 *    created, before the window is made visible. If the window
 *    colorspace is changed while the window is visible, the window
 *    will be redrawn. 
 *    
 *    Note that setting a colorspace on a window with the
 *    kHIWindowBitAutoCalibration attribute prevents the window from
 *    receiving automatic color space updates. Clearing the colorspace
 *    by passing NULL restores the kHIWindowBitAutoCalibration
 *    functionality.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose colorspace to change.
 *    
 *    inColorSpace:
 *      The window's new colorspace. The colorspace will be retained or
 *      copied by the window. Pass NULL to clear any previously-set
 *      colorspace.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetColorSpace(
  WindowRef         inWindow,
  CGColorSpaceRef   inColorSpace)       /* can be NULL */     AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


/*
 *  HIWindowCopyColorSpace()
 *  
 *  Summary:
 *    Retrieves the colorspace for a window.
 *  
 *  Discussion:
 *    If this API is called before a color space has been explicitly
 *    assigned to a window, the window's default colorspace is returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose colorspace to retrieve.
 *  
 *  Result:
 *    The window's colorspace, or NULL if an error occurs. The
 *    colorspace has been retained and should be released by the caller
 *    when the caller is done with it.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern CGColorSpaceRef 
HIWindowCopyColorSpace(WindowRef inWindow)                    AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Window backing store depth types.
 */
enum {

  /*
   * The window is invalid or its backing store pixel size is unknown.
   */
  kHIWindowDepthInvalid         = 0,

  /*
   * The window's backing store pixel size should be 32 bits.
   */
  kHIWindowDepth32Bit           = 1,

  /*
   * The window's backing store pixel size should be 64 bits.
   */
  kHIWindowDepth64Bit           = 2,

  /*
   * The window's backing store pixel size should be the size of a
   * float.
   */
  kHIWindowDepthFloat           = 3
};

typedef UInt32                          HIWindowDepth;
#if !__LP64__
/*
 *  HIWindowSetDepth()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    This API is not actually functional in Mac OS X 10.6 or Mac OS X
 *    10.7 and should not be used. Applications that need to control
 *    the backing store depth of their windows should use NSWindows and
 *    the [NSWindow setDepthLimit:] API.
 *  
 *  Summary:
 *    Sets the depth of a window's backing store.
 *  
 *  Discussion:
 *    Typically this API should be used immediately after a window is
 *    created, before the window is made visible. If the window depth
 *    is changed while the window is visible, the window will be
 *    redrawn.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing store depth to change.
 *    
 *    inDepth:
 *      The new window depth type. This must be one of
 *      kHIWindowDepth32Bit, 64Bit, or Float.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework [32-bit only] but deprecated in 10.7
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetDepth(
  WindowRef       inWindow,
  HIWindowDepth   inDepth)                                    AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/*
 *  HIWindowGetDepth()
 *  
 *  Summary:
 *    Returns the current depth of the backing store of a window.
 *  
 *  Discussion:
 *    This API returns an HIWindowDepth value corresponding to the
 *    actual depth of the backing store. It will return one of
 *    kHIWindowDepth32Bit, 64Bit, or Float.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose backing store depth to return.
 *  
 *  Result:
 *    The window's backing store depth type (one of
 *    kHIWindowDepth32Bit, 64Bit, or Float), or kHIWindowDepthInvalid
 *    if an error occurs.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIWindowDepth 
HIWindowGetDepth(WindowRef inWindow)                          AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Scrolling Routines                                                                 */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */

typedef UInt32 ScrollWindowOptions;
enum {
  kScrollWindowNoOptions        = 0,
  kScrollWindowInvalidate       = (1 << 0), /* add the exposed area to the window’s update region*/
  kScrollWindowEraseToPortBackground = (1 << 1) /* erase the exposed area using the background color/pattern of the window’s grafport*/
};


/* Routines available from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward*/

#if !__LP64__
/*
 *  ScrollWindowRect()
 *  
 *  Summary:
 *    Moves a rectangular portion of a window by a specified horizontal
 *    and vertical amount.
 *  
 *  Discussion:
 *    This API is only recommended for non-composited windows. For a
 *    composited window, use HIViewScrollRect or simply invalidate the
 *    view and redraw it at its scrolled position.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ScrollWindowRect(
  WindowRef             inWindow,
  const Rect *          inScrollRect,
  SInt16                inHPixels,
  SInt16                inVPixels,
  ScrollWindowOptions   inOptions,
  RgnHandle             outExposedRgn)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ScrollWindowRegion()
 *  
 *  Summary:
 *    Moves a non-rectangular portion of a window by a specified
 *    horizontal and vertical amount.
 *  
 *  Discussion:
 *    This API is only recommended for non-composited windows. For a
 *    composited window, use HIViewScrollRect or simply invalidate the
 *    view and redraw it at its scrolled position.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ScrollWindowRegion(
  WindowRef             inWindow,
  RgnHandle             inScrollRgn,
  SInt16                inHPixels,
  SInt16                inVPixels,
  ScrollWindowOptions   inOptions,
  RgnHandle             outExposedRgn)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window List                                                                        */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Options for the HIWindowFindAtLocation API.
 */
enum {

  /*
   * Requests that HIWindowFindAtLocation ignore windows of other
   * processes which might obscure your process's windows. That is, if
   * a window in your process is at the specified point, it will be
   * passed back even if some other process's window obscures it.
   */
  kHIWindowIgnoreObscuringWindows = 1 << 0
};

#if !__LP64__
/*
 *  HIWindowFindAtLocation()
 *  
 *  Summary:
 *    Finds a window in the current process at a specified location.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inLocation:
 *      The global location. This may be specified in either screen
 *      pixel coordinates or virtual 72DPI coordinates, depending on
 *      the inSpace parameter.
 *    
 *    inSpace:
 *      The coordinate space in which the location is expressed. This
 *      must be kHICoordSpaceScreenPixel or kHICoordSpace72DPIGlobal.
 *    
 *    inStartWindow:
 *      The window at which to start the search, inclusive. Pass
 *      kFirstWindowOfClass (-1) to start the search at the beginning
 *      of the window list. Passing NULL will cause the search to start
 *      at the end of the window list, and therefore no window will be
 *      found.
 *    
 *    inOptions:
 *      Flags allowing you to request optional behavior for this API.
 *      If you want the default behavior, pass zero.
 *    
 *    outWindow:
 *      On exit, contains the window in the current process at the
 *      specified location, if any.
 *    
 *    outWindowPart:
 *      On exit, identifies the window part that was hit. You may pass
 *      NULL if you don't need this information.
 *    
 *    outWindowLocation:
 *      On exit, contains the specified location transformed into
 *      window-relative coordinates. This location will take into
 *      account any window transform or magnification.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowFindAtLocation(
  const HIPoint *     inLocation,
  HICoordinateSpace   inSpace,
  WindowRef           inStartWindow,           /* can be NULL */
  OptionBits          inOptions,
  WindowRef *         outWindow,
  WindowPartCode *    outWindowPart,           /* can be NULL */
  HIPoint *           outWindowLocation)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  [Mac]FindWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
#if TARGET_OS_MAC
    #define MacFindWindow FindWindow
#endif
extern WindowPartCode 
MacFindWindow(
  Point        thePoint,
  WindowRef *  window)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  BringToFront()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
BringToFront(WindowRef window)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SendBehind()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SendBehind(
  WindowRef   window,
  WindowRef   behindWindow)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SelectWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SelectWindow(WindowRef window)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
   Routines available from Mac OS 8.6 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  FrontNonFloatingWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern WindowRef 
FrontNonFloatingWindow(void)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Routines available from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward*/

/*
 *  GetNextWindowOfClass()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetNextWindowOfClass(
  WindowRef     inWindow,
  WindowClass   inWindowClass,
  Boolean       mustBeVisible)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetFrontWindowOfClass()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetFrontWindowOfClass(
  WindowClass   inWindowClass,
  Boolean       mustBeVisible)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  FindWindowOfClass()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
FindWindowOfClass(
  const Point *     where,
  WindowClass       inWindowClass,
  WindowRef *       outWindow,
  WindowPartCode *  outWindowPart)       /* can be NULL */    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Options for the CreateStandardWindowMenu API.
 */
enum {

  /*
   * Requests the standard window menu include a Rotate Windows menu
   * item. Available in Mac OS X 10.2.
   */
  kWindowMenuIncludeRotate      = 1 << 0
};


/*
 *  Summary:
 *    Constants for use with Get/SetMenuItemProperty to access property
 *    data of items in the standard window menu. These constants are
 *    supported on Mac OS X 10.5 and later.
 */
enum {

  /*
   * The property creator for accessing standard window menu item
   * properties.
   */
  kHIWindowMenuCreator          = 'wind',

  /*
   * The property tag for accessing standard window menu item
   * properties that hold WindowRefs. Menu items with the
   * kHICommandSelectWindow command ID will have a property with this
   * tag that contains the WindowRef to be activated when that item is
   * selected. Available in Mac OS X 10.5 and later.
   */
  kHIWindowMenuWindowTag        = 'wind'
};

#if !__LP64__
/*
 *  CreateStandardWindowMenu()
 *  
 *  Discussion:
 *    Creates a standard Window menu for your application. You can call
 *    this to create a window menu for your application. Once you have
 *    the menu, you need to insert it in your menu bar (typically at
 *    the end of your menu list) with a call to InsertMenu. To register
 *    a window to be tracked by this menu, you either create your
 *    window with CreateNewWindow, passing the
 *    kWindowInWindowMenuAttribute, or you can use
 *    ChangeWindowAttributes after the window is created. The Toolbox
 *    takes care of acting on the standard items such as zoom and
 *    minimize, as well as bringing selected windows to the front. All
 *    you need to do is install it and register your windows and the
 *    Toolbox does the rest.
 *    
 *    You may also add your own menu items to the standard window menu.
 *    To do this, we recommend that you use the
 *    GetIndMenuItemWithCommandID API to locate one of the standard
 *    menu items in the menu which is immediately above or below the
 *    location where you wish to insert your items. Then insert your
 *    items relative to the position of the standard menu item. Do not
 *    attempt to search the menu items yourself without calling
 *    GetIndMenuItemWithCommandID; in Mac OS X 10.2,
 *    CreateStandardWindowMenu returns an initially empty menu which is
 *    populated later when the menu is displayed or when
 *    GetIndMenuItemWithCommandID is called, so you will find no items
 *    in the menu unless you first call GetIndMenuItemWithCommandID.
 *    
 *    There is a known bug in all versions of CarbonLib which causes
 *    the Zoom, Collapse, and Uncollapse menu items in the standard
 *    window menu to be non-functional for windows created by the
 *    Dialog Manager. To work around this bug, you can install your own
 *    event handlers on the dialog window for kEventWindowZoom,
 *    kEventWindowCollapse, and kEventWindowExpand, and handle the
 *    event by calling the appropriate Window Manager API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOptions:
 *      Requests optional behavior of the standard window menu. Mac OS
 *      X 10.2 supports the kWindowMenuIncludeRotate option; in earlier
 *      versions of Mac OS X, and in CarbonLib, you must pass zero for
 *      this parameter.
 *    
 *    outMenu:
 *      Receives a new menu reference which contains the standard
 *      window menu items and commands.
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
CreateStandardWindowMenu(
  OptionBits   inOptions,
  MenuRef *    outMenu)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowAlternateTitle()
 *  
 *  Discussion:
 *    This API sets an alternate title for a window. The alternate
 *    title overrides what is displayed in the Window menu. If you do
 *    not set an alternate title, the normal window title is used. You
 *    would normally use this if the window title was not expressive
 *    enough to be used in the Window menu (or similar text-only
 *    situation).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to set the alternate title.
 *    
 *    inTitle:
 *      The alternate title for the window. Passing NULL for this
 *      parameter will remove any alternate title that might be present.
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
SetWindowAlternateTitle(
  WindowRef     inWindow,
  CFStringRef   inTitle)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyWindowAlternateTitle()
 *  
 *  Discussion:
 *    This API gets the alternate title for a window. See the
 *    discussion of SetWindowAlternateTitle for more info.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to get the alternate title from.
 *    
 *    outTitle:
 *      Receives the alternate title for the window. If the window does
 *      not have an alternate title, NULL will be returned in outTitle.
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
CopyWindowAlternateTitle(
  WindowRef      inWindow,
  CFStringRef *  outTitle)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Misc Low-Level stuff                                                               */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/
/*
 *  IsValidWindowPtr()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
IsValidWindowPtr(WindowRef possibleWindow)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#define IsValidWindowRef IsValidWindowPtr

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Various & Sundry Window Accessors                                                  */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HiliteWindow()
 *  
 *  Summary:
 *    Hilites or unhilites a window's frame.
 *  
 *  Discussion:
 *    Hiliting a window's frame is not the same as activating the
 *    window. A window's hilited state determines whether the window
 *    draws its titlebar and associated widgets as if they were the
 *    frontmost window of its window group, whereas a window's active
 *    state determines whether the window really is the frontmost
 *    window of its window group. Activating or deactivating a window
 *    automatically adjusts a window's hilited state to match.
 *    
 *    In general, only very old compatibility code would ever need or
 *    want to manually modify a window's hilited state via the
 *    HiliteWindow API. Modern clients can typically avoid this API
 *    entirely.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose frame you wish to hilite/unhilite.
 *    
 *    fHilite:
 *      Whether to hilite or unhilite the window's frame.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HiliteWindow(
  WindowRef   window,
  Boolean     fHilite)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWRefCon()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetWRefCon(
  WindowRef   window,
  SRefCon     data)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWRefCon()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SRefCon 
GetWRefCon(WindowRef window)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Routines available from Mac OS 8.0 (Appearance 1.0) forward*/
/*
 *  GetWindowFeatures()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetWindowFeatures(
  WindowRef   window,
  UInt32 *    outFeatures)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowCopyShape()
 *  
 *  Summary:
 *    Retrieves a shape describing a specified region of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose shape to return.
 *    
 *    inRegion:
 *      The window region whose shape to return.
 *    
 *    inSpace:
 *      The coordinate space in which the shape should be returned.
 *      This must be kHICoordSpaceWindow, kHICoordSpaceScreenPixel, or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outShape:
 *      On exit, contains a shape describing the specified window
 *      region. If the region is not supported, this parameter is not
 *      modified.
 *  
 *  Result:
 *    An operating system result code. errWindowRegionCodeInvalid is
 *    returned if the window does not support the specified window
 *    region.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowCopyShape(
  WindowRef           inWindow,
  WindowRegionCode    inRegion,
  HICoordinateSpace   inSpace,
  HIShapeRef *        outShape)                               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  GetWindowRegion()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowCopyShape instead.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
GetWindowRegion(
  WindowRef          window,
  WindowRegionCode   inRegionCode,
  RgnHandle          ioWinRgn)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWindowStructureWidths()
 *  
 *  Summary:
 *    Returns the width of the structure region on each edge of a
 *    window.
 *  
 *  Discussion:
 *    The structure widths are the difference between the content
 *    region and the structure region on each edge of a window. For
 *    example, if the left edge of the structure region is at x=100,
 *    and the left edge of the content region is at x=110, then the
 *    structure width for the left side of the window is 10 pixels.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window for which to get structure widths.
 *    
 *    outRect:
 *      On exit, contains the width of the structure region on each
 *      side of the window; the left field of the rectangle contains
 *      the structure width on the left side of the window, the top
 *      field contains the width on the top side, and so on.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowStructureWidths(
  WindowRef   inWindow,
  Rect *      outRect)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  HIWindowChangeFeatures()
 *  
 *  Summary:
 *    Changes the window features on the fly.
 *  
 *  Discussion:
 *    HIWindowChangeFeatures changes the features of a window. This
 *    should only be used by custom window definitions or window frame
 *    views.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to modify.
 *    
 *    inSetThese:
 *      The feature bits to set.
 *    
 *    inClearThese:
 *      The feature bits to clear.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowChangeFeatures(
  WindowRef   inWindow,
  UInt64      inSetThese,
  UInt64      inClearThese)                                   AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Update Events                                                                      */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  BeginUpdate()
 *  
 *  Summary:
 *    Modifies a window's GrafPort state before redrawing the invalid
 *    portion of a non-compositing window.
 *  
 *  Discussion:
 *    BeginUpdate copies the window's update region into the window's
 *    GrafPort's visible region, and then clears the update region. By
 *    setting the visible region to match the update region,
 *    BeginUpdate ensures that any subsequent drawing will be clipped
 *    to the update region. 
 *    
 *    This API should not be used on a compositing window. Composited
 *    windows are drawn automatically by the HIView Manager by sending
 *    kEventControlDraw events to the views contained in the window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to update.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
BeginUpdate(WindowRef window)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  EndUpdate()
 *  
 *  Summary:
 *    Restores the original state of a window's GrafPort after a
 *    non-compositing window has been updated.
 *  
 *  Discussion:
 *    EndUpdate restores the original visible region of a window's
 *    GrafPort. 
 *    
 *    This API should not be used on a compositing window. Composited
 *    windows are drawn automatically by the HIView Manager by sending
 *    kEventControlDraw events to the views contained in the window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window that was updated.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
EndUpdate(WindowRef window)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  InvalWindowRgn()
 *  
 *  Summary:
 *    Invalidates a portion of a non-compositing window.
 *  
 *  Discussion:
 *    This API should not be used on a compositing window. To
 *    invalidate a portion of a compositing window, use
 *    HIViewSetNeedsDisplay or one of its variant APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to invalidate.
 *    
 *    region:
 *      The region to invalidate, in local coordinates of the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
InvalWindowRgn(
  WindowRef   window,
  RgnHandle   region)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  InvalWindowRect()
 *  
 *  Summary:
 *    Invalidates a portion of a non-compositing window.
 *  
 *  Discussion:
 *    This API should not be used on a compositing window. To
 *    invalidate a portion of a compositing window, use
 *    HIViewSetNeedsDisplay or one of its variant APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to invalidate.
 *    
 *    bounds:
 *      The rect to invalidate, in local coordinates of the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
InvalWindowRect(
  WindowRef     window,
  const Rect *  bounds)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ValidWindowRgn()
 *  
 *  Summary:
 *    Validates a portion of a non-compositing window.
 *  
 *  Discussion:
 *    This API should not be used on a compositing window. To validate
 *    a portion of a compositing window, use HIViewSetNeedsDisplay or
 *    one of its variant APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to validate.
 *    
 *    region:
 *      The region to validate, in local coordinates of the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
ValidWindowRgn(
  WindowRef   window,
  RgnHandle   region)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ValidWindowRect()
 *  
 *  Summary:
 *    Validates a portion of a non-compositing window.
 *  
 *  Discussion:
 *    This API should not be used on a compositing window. To validate
 *    a portion of a compositing window, use HIViewSetNeedsDisplay or
 *    one of its variant APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to validate.
 *    
 *    bounds:
 *      The rect to validate, in local coordinates of the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
ValidWindowRect(
  WindowRef     window,
  const Rect *  bounds)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Titles                                                                      */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  SetWindowTitleWithCFString()
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
SetWindowTitleWithCFString(
  WindowRef     inWindow,
  CFStringRef   inString)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CopyWindowTitleAsCFString()
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
CopyWindowTitleAsCFString(
  WindowRef      inWindow,
  CFStringRef *  outString)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Proxies                                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowSetProxyFSRef()
 *  
 *  Summary:
 *    Set the proxy icon for a window using an FSRef to an existing
 *    file system object (volume, folder, or file).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to set.
 *    
 *    inRef:
 *      The file system object that the window represents. The window's
 *      proxy icon is determined by asking Icon Services for the icon
 *      of this object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetProxyFSRef(
  WindowRef      window,
  const FSRef *  inRef)                                       AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIWindowGetProxyFSRef()
 *  
 *  Summary:
 *    Returns the FSRef used to determine the proxy icon for a window.
 *  
 *  Discussion:
 *    This API will return noErr and a valid FSRef if the window's
 *    proxy icon has been specified using the FSRef, FSSpec or alias
 *    SetWindowProxy APIs. If the window has no proxy icon, or if the
 *    icon was specified with SetWindowProxyCreatorAndType or
 *    SetWindowProxyIcon, then an error will be returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window containing the proxy icon to return.
 *    
 *    outRef:
 *      On exit, contains the FSRef to the window's proxy icon.
 *  
 *  Result:
 *    noErr if the window's proxy icon FSRef has been returned;
 *    errWindowDoesNotHaveProxy if the window does not have a proxy
 *    icon, or if the proxy icon was specified by
 *    SetWindowProxyCreatorAndType or SetWindowProxyIcon. Other
 *    operating system error codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetProxyFSRef(
  WindowRef   window,
  FSRef *     outRef)                                         AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  SetWindowProxyAlias()
 *  
 *  Summary:
 *    Sets the proxy icon for a window using an AliasHandle to an
 *    existing file system object (volume, folder, or file).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose proxy icon to set.
 *    
 *    inAlias:
 *      The file system object that the window represents. The window’s
 *      proxy icon is determined by asking Icon Services for the icon
 *      of this object. The Window Manager copies the alias data; the
 *      caller may dispose of the alias after SetWindowProxyAlias
 *      returns.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowProxyAlias(
  WindowRef     inWindow,
  AliasHandle   inAlias)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowProxyAlias()
 *  
 *  Summary:
 *    Returns the alias used to determine the proxy icon for a window.
 *  
 *  Discussion:
 *    This API will return noErr and a valid alias if the window’s
 *    proxy icon has been specified using the SetWindowProxyFSSpec or
 *    SetWindowProxyAlias APIs. If the window has no proxy icon, or if
 *    the icon was specified with another SetWindowProxy API, then an
 *    error will be returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon alias to return.
 *    
 *    alias:
 *      On exit, contains the window’s proxy icon alias.
 *  
 *  Result:
 *    noErr if the window’s proxy icon alias has been returned;
 *    errWindowDoesNotHaveProxy if the window does not have a proxy
 *    icon, or if the proxy icon was specified by IconRef or
 *    type/creator rather than by FSSpec or alias. Other operating
 *    system error codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowProxyAlias(
  WindowRef      window,
  AliasHandle *  alias)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowProxyCreatorAndType()
 *  
 *  Summary:
 *    Sets the proxy icon for a window using a file type and creator.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to set.
 *    
 *    fileCreator:
 *      The creator code for the file system object that the window
 *      represents. The window’s proxy icon is determined by asking
 *      Icon Services for the icon corresponding to the specified
 *      creator code and file type, on the specified volume.
 *    
 *    fileType:
 *      The file type for the file system object that the window
 *      represents.
 *    
 *    vRefNum:
 *      The volume reference number for the volume containing the file
 *      system object that the window represents. You may pass
 *      kOnSystemDisk for this parameter if you don't know which volume
 *      will hold the file system object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowProxyCreatorAndType(
  WindowRef   window,
  OSType      fileCreator,
  OSType      fileType,
  SInt16      vRefNum)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowProxyIcon()
 *  
 *  Summary:
 *    Returns the proxy icon of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to return.
 *    
 *    outIcon:
 *      On exit, contains the window’s proxy icon.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowProxyIcon(
  WindowRef   window,
  IconRef *   outIcon)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowProxyIcon()
 *  
 *  Summary:
 *    Sets a window’s proxy icon.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to set.
 *    
 *    icon:
 *      The proxy icon. The Window Manager retains the IconRef; the
 *      caller may release the IconRef after SetWindowProxyIcon returns.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowProxyIcon(
  WindowRef   window,
  IconRef     icon)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveWindowProxy()
 *  
 *  Summary:
 *    Removes a window’s proxy icon.
 *  
 *  Discussion:
 *    When removing the proxy icon, the Window Manager also releases
 *    the alias or IconRef, if any, that was used to specify the proxy
 *    icon.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to remove.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
RemoveWindowProxy(WindowRef window)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  BeginWindowProxyDrag()
 *  
 *  Summary:
 *    Creates a new drag reference that can be used to drag a window’s
 *    proxy icon.
 *  
 *  Discussion:
 *    This API is used by applications that need to add their own drag
 *    flavors to the drag reference used for a proxy icon drag. Such an
 *    application would call BeginWindowProxyDrag to create the drag
 *    reference, add its own flavors, call
 *    TrackWindowProxyFromExistingDrag to track the proxy icon drag,
 *    and then EndWindowProxyDrag to release the drag
 *    reference.
 *    
 *    An application which does not need to add its own drag flavors to
 *    the drag reference can simply call TrackWindowProxyDrag.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to drag.
 *    
 *    outNewDrag:
 *      On exit, contains the drag reference for the proxy icon.
 *    
 *    outDragOutlineRgn:
 *      On entry, should be a valid RgnHandle; on exit, the region has
 *      been updated with an outline of the proxy icon drag image. This
 *      region should be passed to TrackWindowProxyFromExistingDrag;
 *      the application may modify it first. On Mac OS X 10.5 and
 *      later, you may pass NULL if you don't need the outline region.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
BeginWindowProxyDrag(
  WindowRef   window,
  DragRef *   outNewDrag,
  RgnHandle   outDragOutlineRgn)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  EndWindowProxyDrag()
 *  
 *  Summary:
 *    Releases a drag reference created by BeginWindowProxyDrag.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon drag reference to release.
 *    
 *    theDrag:
 *      The drag reference to release.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
EndWindowProxyDrag(
  WindowRef   window,
  DragRef     theDrag)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowTrackProxyDrag()
 *  
 *  Summary:
 *    Tracks the drag of a window proxy icon.
 *  
 *  Discussion:
 *    This API is used by applications that need to manage the dragging
 *    of the proxy icon in their windows. Applications that use the
 *    standard window event hander and that do not need to customize
 *    the proxy icon drag process may rely on the standard handler to
 *    call this API.
 *    
 *    An application may choose whether to allow the Window Manager to
 *    create the drag reference and populate it with drag flavors, or
 *    to create the drag reference itself and adds its own drag
 *    flavors. In the first case, an application should pass NULL for
 *    the drag reference parameter to this API.In the second case, an
 *    application should call BeginWindowProxyDrag to create the drag
 *    reference, add its own flavors, call HIWindowTrackProxyDrag to
 *    track the proxy icon drag, and then EndWindowProxyDrag to release
 *    the drag reference.
 *    
 *    If you pass a non-NULL drag reference, this routine will not
 *    display an outline region for the drag; you are thus expected to
 *    set up a drag image via SetDragImageWithCGImage or a similar API.
 *    Also, in the non- NULL drag reference case, you should not set
 *    the kDragRegionAndImage drag image flag.
 *    
 *    A proxy icon may only be dragged if the window represented by the
 *    proxy icon is not modifed (as indicated by the IsWindowModified
 *    API). This restriction is imposed because a proxy icon is a
 *    representation of a physical file system object, and dragging the
 *    proxy icon may result in the Finder making a copy of the file
 *    system object. If the window is modified, then it contains user
 *    data that has not yet been saved to disk; making a copy of the
 *    file system object would result in a stale copy that did not
 *    contain the user’s current data.
 *    
 *    By default, all newly created windows are considered to be dirty.
 *    The application must call SetWindowModified( window, false )
 *    before the proxy icon will be draggable.
 *    
 *    In Mac OS X 10.3 and later, the proxy icon is also draggable in
 *    dirty windows if the proxy icon was provided using the
 *    SetWindowProxyIcon or SetWindowProxyCreatorAndType APIs. Dragging
 *    is allowed in this case because the window does not represent an
 *    actual file system object, and therefore there is no risk of user
 *    data loss.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose proxy icon to drag.
 *    
 *    inEvent:
 *      The event that resulted in starting a drag. This will most
 *      commonly be kEventControlTrack, but any event with
 *      kEventParamMouseLocation and kEventParamKeyModifiers parameters
 *      is all that is required.
 *    
 *    inDrag:
 *      The proxy icon drag reference. This parameter may be NULL if
 *      you want the Window Manager to create and populate the drag
 *      reference itself. The Window Manager will add its own drag
 *      flavors to the drag even when the client passes a pre-created
 *      drag reference.
 *  
 *  Result:
 *    errUserWantsToDragWindow if the user’s mouse movements indicated
 *    that the user actually wants to drag the window instead of the
 *    proxy icon (in this case the application should call DragWindow);
 *    windowWrongStateErr if the window was modified and therefore the
 *    proxy icon could not be dragged; noErr if the drag succeeded;
 *    userCanceledErr if the user canceled the drag. Other operating
 *    system result codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowTrackProxyDrag(
  WindowRef   inWindow,
  EventRef    inEvent,
  DragRef     inDrag)         /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  TrackWindowProxyFromExistingDrag()
 *  
 *  Summary:
 *    Tracks the drag of a window proxy icon.
 *  
 *  Discussion:
 *    This API is used by applications that need to add their own drag
 *    flavors to the drag reference used for a proxy icon drag. Such an
 *    application would call BeginWindowProxyDrag to create the drag
 *    reference, add its own flavors, call
 *    TrackWindowProxyFromExistingDrag to track the proxy icon drag,
 *    and then EndWindowProxyDrag to release the drag
 *    reference.
 *    
 *    An application which does not need to add its own drag flavors to
 *    the drag reference can simply call TrackWindowProxyDrag.
 *    
 *    A proxy icon may only be dragged if the window represented by the
 *    proxy icon is not modifed (as indicated by the IsWindowModified
 *    API). This restriction is imposed because a proxy icon is a
 *    representation of a physical file system object, and dragging the
 *    proxy icon may result in the Finder making a copy of the file
 *    system object. If the window is modified, then it contains user
 *    data that has not yet been saved to disk; making a copy of the
 *    file system object would result in a stale copy that did not
 *    contain the user’s current data.
 *    
 *    By default, all newly created windows are considered to be dirty.
 *    The application must call SetWindowModified( window, false )
 *    before the proxy icon will be draggable.
 *    
 *    In Mac OS X 10.3 and later, the proxy icon is also draggable in
 *    dirty windows if the proxy icon was provided using the
 *    SetWindowProxyIcon or SetWindowProxyCreatorAndType APIs. Dragging
 *    is allowed in this case because the window does not represent an
 *    actual file system object, and therefore there is no risk of user
 *    data loss.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to drag.
 *    
 *    startPt:
 *      The point in global coordinates where the drag originated. This
 *      is generally the location of the mouse click in the proxy icon.
 *    
 *    drag:
 *      The proxy icon drag reference.
 *    
 *    inDragOutlineRgn:
 *      The outline of the proxy icon drag image, as returned by
 *      BeginWindowProxyDrag. You may pass NULL on Mac OS X 10.5 and
 *      later.
 *  
 *  Result:
 *    errUserWantsToDragWindow if the user’s mouse movements indicated
 *    that the user actually wants to drag the window instead of the
 *    proxy icon (in this case the application should call DragWindow);
 *    windowWrongStateErr if the window was modified and therefore the
 *    proxy icon could not be dragged; noErr if the drag succeeded;
 *    userCanceledErr if the user canceled the drag. Other operating
 *    system result codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
TrackWindowProxyFromExistingDrag(
  WindowRef   window,
  Point       startPt,
  DragRef     drag,
  RgnHandle   inDragOutlineRgn)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  TrackWindowProxyDrag()
 *  
 *  Summary:
 *    Tracks the drag of a window proxy icon.
 *  
 *  Discussion:
 *    A proxy icon may only be dragged if the window represented by the
 *    proxy icon is not modifed (as indicated by the IsWindowModified
 *    API). This restriction is imposed because a proxy icon is a
 *    representation of a physical file system object, and dragging the
 *    proxy icon may result in the Finder making a copy of the file
 *    system object. If the window is modified, then it contains user
 *    data that has not yet been saved to disk; making a copy of the
 *    file system object would result in a stale copy that did not
 *    contain the user’s current data.
 *    
 *    By default, all newly created windows are considered to be
 *    modified. The application must call SetWindowModified( window,
 *    false ) before the proxy icon will be draggable.
 *    
 *    In Mac OS X 10.3 and later, the proxy icon is also draggable in
 *    dirty windows if the proxy icon was provided using the
 *    SetWindowProxyIcon or SetWindowProxyCreatorAndType APIs. Dragging
 *    is allowed in this case because the window does not represent an
 *    actual file system object, and therefore there is no risk of user
 *    data loss.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to drag.
 *    
 *    startPt:
 *      The point in global coordinates where the drag originated. This
 *      is generally the location of the mouse click in the proxy icon.
 *  
 *  Result:
 *    errUserWantsToDragWindow if the user’s mouse movements indicated
 *    that the user actually wants to drag the window instead of the
 *    proxy icon (in this case the application should call DragWindow);
 *    windowWrongStateErr if the window was modified and therefore the
 *    proxy icon could not be dragged; noErr if the drag succeeded;
 *    userCanceledErr if the user canceled the drag. Other operating
 *    system result codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
TrackWindowProxyDrag(
  WindowRef   window,
  Point       startPt)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowModified()
 *  
 *  Summary:
 *    Returns whether the window is considered to have unsaved content.
 *  
 *  Discussion:
 *    By default, all newly created windows are considered to be
 *    modified. The application must call SetWindowModified( window,
 *    false ) to mark the window as unmodified. Until the window is
 *    marked as unmodified, the proxy icon will not be draggable.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose modified state to return.
 *  
 *  Result:
 *    true if the window has unsaved changes, or false if not.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
IsWindowModified(WindowRef window)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowModified()
 *  
 *  Summary:
 *    Sets whether the window is considered to have unsaved content.
 *  
 *  Discussion:
 *    By default, all newly created windows are considered to be
 *    modified. The application must call SetWindowModified( window,
 *    false ) to mark the window as unmodified. Until the window is
 *    marked as unmodified, the proxy icon will not be draggable.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose modified state to return.
 *    
 *    modified:
 *      Whether the window has unsaved changes.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowModified(
  WindowRef   window,
  Boolean     modified)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowPathSelectEvent()
 *  
 *  Summary:
 *    Indicates whether a Carbon event describing a click on a window’s
 *    title should cause a path selection menu to be displayed.
 *  
 *  Discussion:
 *    Windows that have a proxy icon provided using an FSSpec or alias
 *    can support a path selection menu, which displays the file system
 *    path to the object, one menu item per directory. Making a
 *    selection from this item will automatically open the
 *    corresponding object in the Finder.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window on which the click occurred.
 *    
 *    inEvent:
 *      The event. In CarbonLib and in Mac OS X 10.2 and earlier,
 *      IsWindowPathSelectEvent only returned true for
 *      kEventClassMouse/ kEventMouseDown events. In Mac OS X 10.3 and
 *      later, IsWindowPathSelectEvent returns true for any event that
 *      has suitable kEventParamMouseLocation and
 *      kEventParamKeyModifiers parameters.
 *  
 *  Result:
 *    true if the click should cause a path selection menu to be
 *    displayed, or false if not. If this API returns true, the
 *    application should call the WindowPathSelect API.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsWindowPathSelectEvent(
  WindowRef   window,
  EventRef    inEvent)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  WindowPathSelect()
 *  
 *  Summary:
 *    Displays a path selection menu for a window that has a proxy icon.
 *  
 *  Discussion:
 *    If the application passes NULL for the menu parameter, and the
 *    user selects an item from the path selection menu, the Window
 *    Manager will automatically request the Finder to display that
 *    item, and in CarbonLib 1.3.1 and later and Mac OS X, will also
 *    make the Finder be the active application.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window for which to display the path selection menu.
 *    
 *    menu:
 *      The menu to display. If you pass NULL, the Window Manager
 *      automatically creates a suitable menu based on the window’s
 *      proxy icon.
 *    
 *    outMenuResult:
 *      On exit, the menu ID and menu item index of the selected item;
 *      the menu ID is in the high 16 bits of the result, and the menu
 *      item index is in the low 16 bits.
 *  
 *  Result:
 *    noErr if the user selected an item from the menu; userCanceledErr
 *    if the user closed the menu without making a selection. Other
 *    operating system result codes may be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
WindowPathSelect(
  WindowRef   window,
  MenuRef     menu,                /* can be NULL */
  SInt32 *    outMenuResult)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • HiliteWindowFrameForDrag                                                          */
/*                                                                                      */
/*  If you call ShowDragHilite and HideDragHilite, you don’t need to use this routine.  */
/*  If you implement custom drag hiliting, you should call HiliteWindowFrameForDrag     */
/*  when the drag is tracking inside a window with drag-hilited content.                */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* Routines available from Mac OS 8.5 forward*/

/*
 *  HiliteWindowFrameForDrag()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
HiliteWindowFrameForDrag(
  WindowRef   window,
  Boolean     hilited)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Transitions                                                                 */
/*                                                                                      */
/*  TransitionWindow displays a window with accompanying animation and sound.           */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  WindowTransitionEffect
 *  
 *  Summary:
 *    Visual effects that are provided by TransitionWindow
 */
typedef UInt32 WindowTransitionEffect;
enum {

  /*
   * Finder-like zoom rectangles. Use with TransitionWindow and Show or
   * Hide transition actions. In Mac OS X 10.7 and later, this effect
   * can also be used with the Resize transition action to show the
   * standard animation effect for zooming a window to a new size. The
   * combination of the Zoom effect and the Resize action is only
   * recommended for use with compositing windows.
   */
  kWindowZoomTransitionEffect   = 1,

  /*
   * Zoom in/out from parent. Use with TransitionWindowAndParent and
   * Show or Hide transition actions. Available in Mac OS X, and in
   * CarbonLib 1.5 and later.
   */
  kWindowSheetTransitionEffect  = 2,

  /*
   * Slide the window into its new position. Use with TransitionWindow
   * and Move or Resize transition actions. Available in Mac OS X, and
   * in CarbonLib 1.5 and later.
   */
  kWindowSlideTransitionEffect  = 3,

  /*
   * Fade the window into or out of visibility. Use with the Show or
   * Hide transition actions. Available in Mac OS X 10.3 and later.
   */
  kWindowFadeTransitionEffect   = 4,

  /*
   * Use the Genie effect that the Dock uses to minimize or maximize a
   * window to show or hide the window. Use with the Show or Hide
   * transition actions. Available in Mac OS X 10.3 and later.
   */
  kWindowGenieTransitionEffect  = 5
};


/*
 *  WindowTransitionAction
 *  
 *  Summary:
 *    Modifications to window state that are provided by
 *    TransitionWindow
 */
typedef UInt32 WindowTransitionAction;
enum {

  /*
   * Shows the window. Use with the Zoom, Sheet, Fade, or Genie
   * transition effects. For the Zoom, Sheet, and Genie effects, the
   * inRect parameter is the global coordinates from which to start the
   * animation; inRect may be NULL for the Zoom and Sheet effects, and
   * in that case, the animation begins at the center of the window.
   * The Genie effect requires a non-NULL inRect parameter. The Fade
   * effect does not use the inRect parameter.
   */
  kWindowShowTransitionAction   = 1,

  /*
   * Hides the window. Use with the Zoom, Sheet, Fade, or Genie
   * transition effects. For the Zoom, Sheet, and Genie effects, the
   * inRect parameter is the global coordinates at which to end the
   * animation; inRect may be NULL for the Zoom and Sheet effects, and
   * in that case, the animation ends at the center of the window. The
   * Genie effect requires a non-NULL inRect parameter. The Fade effect
   * does not use the inRect parameter.
   */
  kWindowHideTransitionAction   = 2,

  /*
   * Moves the window. Use with the Slide transition effect. The inRect
   * parameter is the global coordinates of the window's new structure
   * bounds; inRect must be non-NULL. Available in Mac OS X, and in
   * CarbonLib 1.5 and later.
   */
  kWindowMoveTransitionAction   = 3,

  /*
   * Resizes the window. Use with the Slide transition effect. In Mac
   * OS X 10.7 and later, the Zoom transition effect is also supported
   * with this action, for use by applications that respond to a zoom
   * box click by resizing the window programmatically without calling
   * the ZoomWindow or ZoomWindowIdeal APIs. The inRect parameter is
   * the global coordinates of the window's new structure bounds;
   * inRect must be non-NULL. Available in Mac OS X, and in CarbonLib
   * 1.5 and later.
   */
  kWindowResizeTransitionAction = 4
};

#if !__LP64__
/*
 *  TransitionWindow()
 *  
 *  Summary:
 *    Shows, hides, moves, or resizes a window with appropriate
 *    animation and sound.
 *  
 *  Discussion:
 *    In Mac OS X 10.3 and later, this API sends
 *    kEventWindowTransitionStarted and kEventWindowTransitionCompleted
 *    Carbon events to the transitioning window at the start and end of
 *    the transition.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window on which to act.
 *    
 *    inEffect:
 *      The type of visual effect to use. TransitionWindow supports the
 *      Zoom, Slide, Fade, and Genie transition effects. The Slide
 *      effect is supported on Mac OS X and in CarbonLib 1.5 and later.
 *      The Fade and Genie effects are supported on Mac OS X 10.3 and
 *      later.
 *    
 *    inAction:
 *      The action to take on the window. TransitionWindow supports the
 *      Show, Hide, Move, and Resize actions. The Move and Resize
 *      actions are supported on Mac OS X and in CarbonLib 1.5 and
 *      later.
 *    
 *    inRect:
 *      A screen rect in global coordinates. The interpretation of the
 *      rect is dependent on the transition action; see the
 *      documentation for each action for details. May be NULL for some
 *      transition actions.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
TransitionWindow(
  WindowRef                inWindow,
  WindowTransitionEffect   inEffect,
  WindowTransitionAction   inAction,
  const Rect *             inRect)         /* can be NULL */  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  TransitionWindowAndParent()
 *  
 *  Summary:
 *    Shows or hides a window, potentially also moving a second window,
 *    with appropriate animation and sound.
 *  
 *  Discussion:
 *    In Mac OS X 10.3 and later, this API sends
 *    kEventWindowTransitionStarted and kEventWindowTransitionCompleted
 *    Carbon events to the transitioning window at the start and end of
 *    the transition.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window on which to act.
 *    
 *    inParentWindow:
 *      The window to which the primary window is related. For the
 *      Sheet effect, this parameter must specify a valid WindowRef; it
 *      should be NULL for other effects.
 *    
 *    inEffect:
 *      The type of visual effect to use. TransitionWindowAndParent is
 *      most commonly used to perform the Sheet transition effect, but
 *      also supports the Zoom, Slide, Fade, and Genie effects.
 *    
 *    inAction:
 *      The action to take on the window. TransitionWindowAndParent
 *      supports the Show, Hide, Move, and Resize actions.
 *    
 *    inRect:
 *      A screen rect in global coordinates. The interpretation of the
 *      rect is dependent on the transition action; see the
 *      documentation for each action for details. May be NULL for some
 *      transition actions.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TransitionWindowAndParent(
  WindowRef                inWindow,
  WindowRef                inParentWindow,
  WindowTransitionEffect   inEffect,
  WindowTransitionAction   inAction,
  const Rect *             inRect)               /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  TransitionWindowOptions
 *  
 *  Summary:
 *    Extensible parameter block for the TransitionWindowWithOptions
 *    API.
 */
struct TransitionWindowOptions {

  /*
   * The structure version. You must put 0 in this field.
   */
  UInt32              version;

  /*
   * The duration of the fade, in seconds. For use with the Sheet,
   * Slide, Fade, and Genie transition effects; ignored for other
   * effects. You may pass 0 to use the default duration. The effect is
   * not guaranteed to last precisely this long, but should be a close
   * approximation.
   */
  EventTime           duration;

  /*
   * For use with kWindowSheetTransitionEffect. This is the parent
   * window of the sheet.
   */
  WindowRef           window;

  /*
   * This value will be sent as the kEventParamUserData parameter for
   * the kEventWindowTransitionStarted and
   * kEventWindowTransitionCompleted events.
   */
  void *              userData;
};
typedef struct TransitionWindowOptions  TransitionWindowOptions;
#if !__LP64__
/*
 *  TransitionWindowWithOptions()
 *  
 *  Summary:
 *    Transitions a window from one state to another with appropriate
 *    animation and sound.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window that should be transitioned.
 *    
 *    inEffect:
 *      The type of visual effect to use.
 *    
 *    inAction:
 *      The action to take on the window.
 *    
 *    inBounds:
 *      A screen rect in global coordinates. The interpretation of the
 *      rect is dependent on the transition action; see the
 *      documentation for each action for details. May be NULL for some
 *      transition actions.
 *    
 *    inAsync:
 *      Whether the transition should run synchronously or
 *      asynchronously. If inAsync is true, TransitionWindow will
 *      return immediately, and the transition will run using an event
 *      loop timer. You must run your event loop for the transition to
 *      occur. If inAsync is false, TransitionWindow will block until
 *      the transition is completed. In either case, the
 *      kEventWindowTransitionStarted and
 *      kEventWindowTransitionCompleted Carbon events will be sent to
 *      the transitioning window at the start and end of the transition.
 *    
 *    inOptions:
 *      Extra options that are required for some transitions. This
 *      parameter may be NULL if the specific transition effect does
 *      not require extra information.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
TransitionWindowWithOptions(
  WindowRef                        inWindow,
  WindowTransitionEffect           inEffect,
  WindowTransitionAction           inAction,
  const HIRect *                   inBounds,        /* can be NULL */
  Boolean                          inAsync,
  const TransitionWindowOptions *  inOptions)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Positioning                                                                 */
/*——————————————————————————————————————————————————————————————————————————————————————*/

/*
 *  [Mac]MoveWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
#if TARGET_OS_MAC
    #define MacMoveWindow MoveWindow
#endif
extern void 
MacMoveWindow(
  WindowRef   window,
  short       hGlobal,
  short       vGlobal,
  Boolean     front)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SizeWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SizeWindow(
  WindowRef   window,
  short       w,
  short       h,
  Boolean     fUpdate)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ResizeWindow()
 *  
 *  Summary:
 *    Handles all user interaction while a window is being resized.
 *  
 *  Discussion:
 *    The ResizeWindow function moves either an outline of the window's
 *    edges (on Mac OS 9.x and earlier) or the actual window (on Mac OS
 *    X) around the screen, following the user’s cursor movements, and
 *    handles all user interaction until the mouse button is released.
 *    Unlike the GrowWindow API, there is no need to follow this call
 *    with a call to the function SizeWindow, because once the mouse
 *    button is released, ResizeWindow resizes the window if the user
 *    has changed the window size. Once the resizing is complete,
 *    ResizeWindow draws the window in the new size. Your application
 *    should call the ResizeWindow function instead of the earlier
 *    Window Manager APIs SizeWindow and GrowWindow. Some windows may
 *    allow themselves to be resized from any corner or edge, not just
 *    the bottom right, and as a result, when the user resizes the
 *    window, the window may move on the screen and not merely change
 *    size. ResizeWindow informs your application of the new window
 *    bounds, so that your application can respond to any changes in
 *    the window’s position.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to be resized.
 *    
 *    inStartPoint:
 *      The point, in global coordinates, at which the original
 *      mouse-down occurred.
 *    
 *    inSizeConstraints:
 *      The limits on the vertical and horizontal measurements of the
 *      content rectangle, in pixels. Although this parameter is typed
 *      as a Rect, the four numbers in the structure represent limits,
 *      not screen coordinates. The top, left, bottom, and right fields
 *      of the structure specify the minimum vertical measurement
 *      (top), the minimum horizontal measurement (left), the maximum
 *      vertical measurement (bottom), and the maximum horizontal
 *      measurement (right). The minimum dimensions should be large
 *      enough to allow a manageable rectangle; 64 pixels on a side is
 *      typical. The maximum dimensions can be no greater than 32,767.
 *      You can pass NULL to allow the user to resize the window to any
 *      size that is contained onscreen.
 *    
 *    outNewContentRect:
 *      On exit, the structure contains the new dimensions of the
 *      window’s content region, in global coordinates. On Mac OS 9.x
 *      and earlier, you must pass a non-NULL value in this parameter;
 *      in Carbon, you may pass NULL if you do not need the window’s
 *      new dimensions.
 *  
 *  Result:
 *    Returns true if the window changed size, or false if not.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
ResizeWindow(
  WindowRef     inWindow,
  Point         inStartPoint,
  const Rect *  inSizeConstraints,       /* can be NULL */
  Rect *        outNewContentRect)       /* can be NULL */    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  DragWindow()
 *  
 *  Summary:
 *    Allows the user to drag a window to a different position.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to drag.
 *    
 *    startPt:
 *      The point in global coordinates where the mouse was clicked on
 *      the window.
 *    
 *    boundsRect:
 *      A rect in global coordinates outside of which the window cannot
 *      move. May be NULL in CarbonLib and Mac OS X, to indicate that
 *      there are no restrictions on window movement. This parameter is
 *      ignored by CarbonLib and Mac OS X 10.0 through 10.2; it is
 *      obeyed in Mac OS X 10.3 and later.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DragWindow(
  WindowRef     window,
  Point         startPt,
  const Rect *  boundsRect)       /* can be NULL */           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ZoomWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ZoomWindow(
  WindowRef        window,
  WindowPartCode   partCode,
  Boolean          front)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Collapsing/Expanding                                                        */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* Routines available from Mac OS 8.0 (Appearance 1.0) forward*/
/*
 *  IsWindowCollapsable()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern Boolean 
IsWindowCollapsable(WindowRef window)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowCollapsed()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern Boolean 
IsWindowCollapsed(WindowRef window)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CollapseWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
CollapseWindow(
  WindowRef   window,
  Boolean     collapse)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CollapseAllWindows()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSStatus 
CollapseAllWindows(Boolean collapse)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowCreateCollapsedDockTileContext()
 *  
 *  Summary:
 *    Creates a CGContextRef that can be used to draw into a collapsed
 *    window's Dock tile.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The collapsed window.
 *    
 *    outContext:
 *      On exit, returns a CGContextRef for drawing into the window's
 *      Dock tile. You should use
 *      HIWindowReleaseCollapsedDockTileContext to release this
 *      context. To ensure that drawing to the context appears
 *      onscreen, make sure to call CGContextFlush before releasing the
 *      context.
 *    
 *    outContextSize:
 *      On exit, returns the size of the context in which the
 *      application should draw.
 *  
 *  Result:
 *    An operating system result code. windowWrongStateErr is returned
 *    if the window is not collapsed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowCreateCollapsedDockTileContext(
  WindowRef       inWindow,
  CGContextRef *  outContext,
  HISize *        outContextSize)                             AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowReleaseCollapsedDockTileContext()
 *  
 *  Discussion:
 *    Releases a CGContextRef and other state created by the
 *    HIWindowCreateCollapsedDockTileContext API. You MUST call this
 *    instead of CFRelease/CGContextRelease directly, or you may leak
 *    system resources.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The collapsed window for which you created the CGContextRef.
 *    
 *    inContext:
 *      The context to release. The context is invalid after this call
 *      and should no longer be used. To ensure that drawing to the
 *      context appears onscreen, make sure to call CGContextFlush
 *      before releasing the context.
 *  
 *  Result:
 *    An operating system result code. windowWrongStateErr is returned
 *    if the window is not collapsed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowReleaseCollapsedDockTileContext(
  WindowRef      inWindow,
  CGContextRef   inContext)                                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  CreateQDContextForCollapsedWindowDockTile()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowCreateCollapsedDockTileContext instead.
 *  
 *  Discussion:
 *    Creates and returns a CGrafPtr for a collapsed window's tile in
 *    the dock. You can use this port to draw into your window's dock
 *    tile with Quickdraw. You MUST call
 *    ReleaseQDContextForCollapsedWindowDockTile and NOT DisposePort
 *    when using this API, as it maintains more state than just the
 *    port. If you call DisposePort, you may leak system resources.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to create the dock tile port for. If this window is
 *      not collapsed, an error is returned.
 *    
 *    outContext:
 *      The Quickdraw port for you to use to draw into. If you wish to
 *      use CoreGraphics (Quartz) drawing, call CreateCGContextForPort
 *      with this port to obtain a CGContext.
 *  
 *  Result:
 *    An operating system result code. windowWrongStateErr is returned
 *    if the window is not collapsed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateQDContextForCollapsedWindowDockTile(
  WindowRef   inWindow,
  CGrafPtr *  outContext)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  ReleaseQDContextForCollapsedWindowDockTile()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowReleaseCollapsedDockTileContext instead.
 *  
 *  Discussion:
 *    Releases a port and other state created by the
 *    CreateQDContextForCollapsedWindowDockTile API. You MUST call this
 *    instead of DisposePort directly, or you may leak system resources.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window you created the port for. If this window is not
 *      collapsed, an error is returned.
 *    
 *    inContext:
 *      The Quickdraw context to dispose.
 *  
 *  Result:
 *    An operating system result code. windowWrongStateErr is returned
 *    if the window is not collapsed.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReleaseQDContextForCollapsedWindowDockTile(
  WindowRef   inWindow,
  CGrafPtr    inContext)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  UpdateCollapsedWindowDockTile()
 *  
 *  Discussion:
 *    Automatically updates the image of a particular window in the
 *    dock to the current contents of the window. Use this for periodic
 *    updates, etc. Do not use this for animation purposes, if you want
 *    animation, use the above create/release drawing context APIs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to update the dock tile for. If this window is not
 *      collapsed, an error is returned.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
UpdateCollapsedWindowDockTile(WindowRef inWindow)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowDockTileMenu()
 *  
 *  Summary:
 *    Sets the menu that is displayed by a window's dock tile.
 *  
 *  Discussion:
 *    When a window's dock tile menu is right-clicked or
 *    control-clicked, the Dock will always automatically display a
 *    menu containing a Close menu item that closes the window. If the
 *    application wants to add other additional menu items, it can use
 *    the SetWindowDockTileMenu API to provide those items. The items
 *    in the specified menu will be combined with the standard items
 *    provided by the Dock.
 *    
 *    Before the menu is actually displayed, it will receive
 *    kEventMenuPopulate, kEventMenuOpening, and kEventMenuEnableItems
 *    Carbon events, so any event handlers for these events may update
 *    the menu appropriately for the current state of the
 *    application.
 *    
 *    The application should set a command ID for each menu item in the
 *    dock tile menu, and when that item is chosen, a
 *    kEventCommandProcess Carbon event containing the item's command
 *    ID will be sent to the window's event target (and from there to
 *    the application, if the window does not handle the
 *    event).
 *    
 *    This API increments the refcount of the specified menu.
 *    
 *    The toolbox provides a default handler for the
 *    kEventWindowGetDockTileMenu event that returns the menu specified
 *    by the SetWindowDockTileMenu API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window for which to set the dock tile menu.
 *    
 *    inMenu:
 *      The menu to display, or NULL to remove the current dock tile
 *      menu.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowDockTileMenu(
  WindowRef   inWindow,
  MenuRef     inMenu)         /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetWindowDockTileMenu()
 *  
 *  Summary:
 *    Returns the menu that is displayed by a window's dock tile.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The application's dock tile menu, or NULL if none.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern MenuRef 
GetWindowDockTileMenu(WindowRef inWindow)                     AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIWindowGetBounds()
 *  
 *  Summary:
 *    Retrieves the bounds of a specified region of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose bounds to return.
 *    
 *    inRegion:
 *      The window region whose bounds to return.
 *    
 *    inSpace:
 *      The coordinate space in which the bounds should be returned.
 *      This must be kHICoordSpaceWindow, kHICoordSpaceScreenPixel, or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outBounds:
 *      On exit, contains the bounds of the specified window region. If
 *      the region is not supported, this parameter is not modified.
 *  
 *  Result:
 *    An operating system result code. errWindowRegionCodeInvalid is
 *    returned if the window does not support the specified window
 *    region.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetBounds(
  WindowRef           inWindow,
  WindowRegionCode    inRegion,
  HICoordinateSpace   inSpace,
  HIRect *            outBounds)                              AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowSetBounds()
 *  
 *  Summary:
 *    Sets the bounds of a window based on either the structure or
 *    content region of the window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose bounds to set.
 *    
 *    inRegion:
 *      The window region on which to base the window's new bounds.
 *      This parameter must be either kWindowStructureRgn or
 *      kWindowContentRgn.
 *    
 *    inSpace:
 *      The coordinate space in which the bounds are expressed. This
 *      must be kHICoordSpaceWindow, kHICoordSpaceScreenPixel, or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    inBounds:
 *      The new bounds of the specified window region. If the
 *      coordinate space is kHICoordSpaceWindow, then the origin of the
 *      bounds is a window-relative value. Therefore, you can use the
 *      Window coordinate space to resize a window without first
 *      getting its current bounds by setting the origin to (0,0), or
 *      you can offset a window from its current position by setting
 *      the origin to the offset amount and the size to the window's
 *      current size.
 *  
 *  Result:
 *    An operating system result code. errWindowRegionCodeInvalid is
 *    returned if the inRegion parameter is any value other than
 *    kWindowStructureRgn or kWindowContentRgn.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetBounds(
  WindowRef           inWindow,
  WindowRegionCode    inRegion,
  HICoordinateSpace   inSpace,
  const HIRect *      inBounds)                               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  GetWindowBounds()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowBounds(
  WindowRef          window,
  WindowRegionCode   regionCode,
  Rect *             globalBounds)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowResizeLimits()
 *  
 *  Summary:
 *    Sets the minimum and maximum content sizes for a window.
 *  
 *  Discussion:
 *    The minimum and maximum sizes are obeyed by ResizeWindow and
 *    GrowWindow. The default Carbon event handler installed for all
 *    windows will return these sizes in response to
 *    kEventWindowGetMinimumSize and kEventWindowGetMaximumSize events.
 *    When a window is first created, its minimum and maximum sizes are
 *    set to reasonable values (which may change from one system
 *    release to the next).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose minimum and maximum sizes to set.
 *    
 *    inMinLimits:
 *      The new minimum size for the content region of the window. Pass
 *      NULL if you don't want to set a minimum size.
 *    
 *    inMaxLimits:
 *      The new maximum size for the content region of the window. Pass
 *      NULL if you don't want to set a maximum size.
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
SetWindowResizeLimits(
  WindowRef       inWindow,
  const HISize *  inMinLimits,       /* can be NULL */
  const HISize *  inMaxLimits)       /* can be NULL */        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetWindowResizeLimits()
 *  
 *  Summary:
 *    Returns the minimum and maximum content sizes for a window.
 *  
 *  Discussion:
 *    GetWindowResizeLimits returns the minimum and maximum sizes that
 *    were set by SetWindowResizeLimits. Note that it does not send
 *    kEventWindowGetMinimumSize or kEventWindowGetMaximumSize Carbon
 *    events to get these sizes; it simply retrieves the sizes from the
 *    WindowRef. It is entirely possible (and quite likely) that a
 *    given window will have event handlers for the
 *    kEventWindowGetMinimum/MaximumSize events that will modify or
 *    override the sizes in the WindowRef; therefore, to accurately
 *    determine the desired minimum and maximum sizes, you should send
 *    kEventWindowGetMinimum/MaximumSize Carbon events rather than
 *    using this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose minimum and maximum sizes to retrieve.
 *    
 *    outMinLimits:
 *      On exit, contains the minimum size of the window's content
 *      region. Pass NULL if you don't want this information.
 *    
 *    outMaxLimits:
 *      On exit, contains the maximum size of the window's content
 *      region. Pass NULL if you don't want this information.
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
GetWindowResizeLimits(
  WindowRef   inWindow,
  HISize *    outMinLimits,       /* can be NULL */
  HISize *    outMaxLimits)       /* can be NULL */           AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
   Routines available from Mac OS 8.5 forward,
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0.2 forward
*/

/*
 *  SetWindowBounds()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowBounds(
  WindowRef          window,
  WindowRegionCode   regionCode,
  const Rect *       globalBounds)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RepositionWindow()
 *  
 *  Summary:
 *    Positions a window according in one of the standard window
 *    locations.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window to position.
 *    
 *    parentWindow:
 *      For some positioning methods, the new location for the
 *      positioned window will be calculated based on the parent
 *      window’s position. On Mac OS X 10.3 and later, some positioning
 *      methods allow this parameter to be the same as the window
 *      parameter; CarbonLib and earlier versions of Mac OS X require
 *      that it be different from the window parameter. See the
 *      WindowPositionMethod documentation for details on which methods
 *      allow the parent to be the same as the positioned window.
 *    
 *    method:
 *      The window positioning method. This should be one of the
 *      WindowPositionMethod constants.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
RepositionWindow(
  WindowRef              window,
  WindowRef              parentWindow,
  WindowPositionMethod   method)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  MoveWindowStructure()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
MoveWindowStructure(
  WindowRef   window,
  short       hGlobal,
  short       vGlobal)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
   Routines available from Mac OS 8.5 forward,
   or from Mac OS 8.6 forward when linking to CarbonLib 1.1 forward
*/

/* Routines available from Mac OS 8.5 forward*/

/*
 *  HIWindowIsInStandardState()
 *  
 *  Summary:
 *    Determines whether a window is currently zoomed in to the user
 *    state or zoomed out to the standard state.
 *  
 *  Discussion:
 *    HIWindowIsInStandardState compares the window’s current
 *    dimensions to those given by the inIdealSize parameter to
 *    determine if the window is currently in its standard (zoomed-out)
 *    state. Your application may use HIWindowIsInStandardState to
 *    decide whether a user’s click of the zoom box is a request to
 *    zoom in to the user state or out to the standard state. Your
 *    application may also use HIWindowIsInStandardState to determine
 *    the size and position of the standard state that the Window
 *    Manager would calculate for a window, given a specified ideal
 *    size; this value is returned in the outIdealStandardState
 *    parameter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window for which to determine the zoom state.
 *    
 *    inIdealSize:
 *      The ideal width and height of the window’s content region,
 *      regardless of the actual screen device dimensions. If you set
 *      idealSize to NULL, HIWindowIsInStandardState examines the
 *      dimensions stored in the stdState field of the WStateData
 *      structure attached to the window.
 *    
 *    inSpace:
 *      The coordinate space in which the ideal size is expressed and
 *      in which the standard state bounds should be returned. This
 *      must be kHICoordSpaceScreenPixel or kHICoordSpace72DPIGlobal.
 *    
 *    outIdealStandardState:
 *      On exit, contains the global coordinates for the content region
 *      of the window in its standard state, based on the data supplied
 *      in the inIdealSize parameter. The coordinates are in either
 *      screen pixels or 72DPI virtual coordinates. You may pass NULL
 *      if you do not need this information.
 *  
 *  Result:
 *    Returns true if the window is currently in its standard
 *    (zoomed-out) state, or false if the window is a non-zoomed-out
 *    state.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowIsInStandardState(
  WindowRef           inWindow,
  const HISize *      inIdealSize,                 /* can be NULL */
  HICoordinateSpace   inSpace,
  HIRect *            outIdealStandardState)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  IsWindowInStandardState()
 *  
 *  Summary:
 *    Determines whether a window is currently zoomed in to the user
 *    state or zoomed out to the standard state.
 *  
 *  Discussion:
 *    IsWindowInStandardState compares the window’s current dimensions
 *    to those given by the inIdealSize parameter to determine if the
 *    window is currently in its standard (zoomed-out) state. Your
 *    application may use IsWindowInStandardState to decide whether a
 *    user’s click of the zoom box is a request to zoom in to the user
 *    state or out to the standard state. Your application may also use
 *    IsWindowInStandardState to determine the size and position of the
 *    standard state that the Window Manager would calculate for a
 *    window, given a specified ideal size; this value is returned in
 *    the outIdealStandardState parameter.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window for which to determine the zoom state.
 *    
 *    inIdealSize:
 *      The ideal width and height of the window’s content region,
 *      regardless of the actual screen device dimensions. If you set
 *      idealSize to NULL, IsWindowInStandardState examines the
 *      dimensions stored in the stdState field of the WStateData
 *      structure attached to the window.
 *    
 *    outIdealStandardState:
 *      On exit, contains the global coordinates for the content region
 *      of the window in its standard state, based on the data supplied
 *      in the inIdealSize parameter. You may pass NULL if you do not
 *      need this information.
 *  
 *  Result:
 *    Returns true if the window is currently in its standard
 *    (zoomed-out) state, or false if the window is a non-zoomed-out
 *    state.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
IsWindowInStandardState(
  WindowRef      inWindow,
  const Point *  inIdealSize,                 /* can be NULL */
  Rect *         outIdealStandardState)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ZoomWindowIdeal()
 *  
 *  Summary:
 *    Zooms a window in accordance with human interface guidelines.
 *  
 *  Discussion:
 *    Applications should use the ZoomWindowIdeal API instead of the
 *    older ZoomWindow API. When your application calls
 *    ZoomWindowIdeal, it automatically conforms to the human interface
 *    guidelines for determining a window’s standard state: the window
 *    is moved as little as possible when switching between user and
 *    standard states, the window is zoomed to the screen that contains
 *    the largest portion of the window, and the window is positioned
 *    in its zoomed-out size to avoid the Dock on Mac OS X. 
 *    
 *    The ZoomWindowIdeal API calculates a window’s ideal standard
 *    state and updates a window’s ideal user state independently of
 *    the WStateData structure. Previously, the window definition
 *    function was responsible for updating the user state, but because
 *    it relies upon the WStateData structure, the window definition
 *    function is unaware of the ideal standard state and can no longer
 *    track the window’s zoom state reliably. The Window Manager
 *    provides the GetWindowIdealUserState and SetWindowIdealUserState
 *    APIs to access a window's current ideal user state, previously
 *    recorded by ZoomWindowIdeal. 
 *    
 *    In Mac OS X 10.7 and later, the ZoomWindowIdeal API automatically
 *    uses an animation to resize the window to its new size, if the
 *    window uses composited mode.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to be zoomed.
 *    
 *    inPartCode:
 *      The direction of the zoom, either inZoomIn or inZoomOut. The
 *      value passed in this parameter should generally be determined
 *      by calling IsWindowInStandardState; if IsWindowInStandardState
 *      returns true, pass inZoomIn, otherwise pass inZoomOut.
 *    
 *    ioIdealSize:
 *      When you specify inZoomIn in the inPartCode parameter, this
 *      parameter is unused on entry; you pass a pointer to a Point
 *      structure, but do not fill the structure with data. On exit,
 *      the Point contains the new height and width of the window’s
 *      content region, and ZoomWindowIdeal restores the previous ideal
 *      user state. When you specify inZoomOut in the inPartCode
 *      parameter, you pass the ideal height and width of the window’s
 *      content region in the Point structure. On return, the Point
 *      contains the new height and width of the window’s content
 *      region; ZoomWindowIdeal saves the current user state of the
 *      window and zooms the window to its ideal size for the standard
 *      state. 
 *      
 *      Prior to Mac OS X Mac OS X 10.4, the ZoomWindowIdeal API
 *      malfunctioned if passed an idealSize that was large enough to
 *      cause 16-bit integer overflow when added to the window's
 *      current position. Therefore, when specifying the ideal size
 *      parameter, you should generally not use values that are close
 *      to 32767. It is better to limit your ideal size to, say, 16K.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
ZoomWindowIdeal(
  WindowRef        inWindow,
  WindowPartCode   inPartCode,
  Point *          ioIdealSize)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowGetIdealUserState()
 *  
 *  Summary:
 *    Returns the bounds of a window’s content region in its user
 *    (zoomed-in) state.
 *  
 *  Discussion:
 *    The HIWindowGetIdealUserState API returns the window’s user state
 *    most recently recorded by ZoomWindowIdeal.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose ideal user state you wish to retrieve.
 *    
 *    inSpace:
 *      The coordinate space in which the user state bounds should be
 *      returned. This must be kHICoordSpaceScreenPixel or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outUserState:
 *      On exit, contains the global coordinates of the window’s
 *      content region when zoomed in, in either screen pixels or 72DPI
 *      virtual coordinates. HIWindowGetIdealUserState automatically
 *      returns the window’s current content bounds if the window has
 *      not yet been zoomed.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetIdealUserState(
  WindowRef           inWindow,
  HICoordinateSpace   inSpace,
  HIRect *            outUserState)                           AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowSetIdealUserState()
 *  
 *  Summary:
 *    Sets the bounds of a window’s content region in its user
 *    (zoomed-in) state.
 *  
 *  Discussion:
 *    The window’s ideal user state is used by the ZoomWindowIdeal API
 *    when zooming in. The ideal user state is ignored by the
 *    ZoomWindow API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose ideal user state to set.
 *    
 *    inSpace:
 *      The coordinate space in which the user state bounds are
 *      expressed. This must be kHICoordSpaceScreenPixel or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    inUserState:
 *      The global coordinates of the window’s content region in its
 *      ideal user state, in either screen pixels or 72DPI virtual
 *      coordinates.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetIdealUserState(
  WindowRef           inWindow,
  HICoordinateSpace   inSpace,
  const HIRect *      inUserState)                            AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  GetWindowIdealUserState()
 *  
 *  Summary:
 *    Returns the bounds of a window’s content region in its user
 *    (zoomed-in) state.
 *  
 *  Discussion:
 *    Traditionally, the user (zoomed-in) state of a window has been
 *    stored in the WStateData structure attached to a window. This
 *    field is updated by the window definition when the user clicks on
 *    the zoom box and the window definition determines that the window
 *    is currently not zoomed out. When determining whether the window
 *    is zoomed out, however, the window definition can only look at
 *    the standard state stored in the WStateData field. With the
 *    introduction of the ZoomWindowIdeal API, there is a new basis for
 *    determining whether a window is zoomed out: the window’s ideal
 *    size. The window definition does not have access to the window’s
 *    ideal size, and therefore cannot accurately determine whether a
 *    window that is zoomed with ZoomWindowIdeal is in its user state
 *    or standard state; therefore, the user state that the window
 *    definition stores in the WStateData is also unreliable. The
 *    ZoomWindowIdeal API therefore stores the window’s user state
 *    before zooming out in a new location, which is accessed using the
 *    GetWindowIdealUserState API. The GetWindowIdealUserState API
 *    returns the window’s user state most recently recorded by
 *    ZoomWindowIdeal.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose ideal user state you wish to retrieve.
 *    
 *    outUserState:
 *      On exit, contains the global coordinates of the window’s
 *      content region when zoomed in. On Mac OS X 10.1 and earlier,
 *      and CarbonLib 1.5 and earlier, the window’s ideal user state is
 *      an empty rect with coordinates (0,0,0,0) if the window has
 *      never been zoomed out; you should check for an empty rect and
 *      use GetWindowBounds with the kWindowContentRgn selector to
 *      determine the window’s current content region. On later
 *      versions of Mac OS X and CarbonLib, GetWindowIdealUserState
 *      automatically returns the window’s current content bounds if
 *      the window has not yet been zoomed.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowIdealUserState(
  WindowRef   inWindow,
  Rect *      outUserState)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowIdealUserState()
 *  
 *  Summary:
 *    Sets the bounds of a window’s content region in its user
 *    (zoomed-in) state.
 *  
 *  Discussion:
 *    The window’s ideal user state is used by the ZoomWindowIdeal API
 *    when zooming in. The ideal user state is ignored by the
 *    ZoomWindow API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose ideal user state to set.
 *    
 *    inUserState:
 *      The global coordinates of the window’s content region in its
 *      ideal user state.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowIdealUserState(
  WindowRef     inWindow,
  const Rect *  inUserState)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowGreatestAreaDevice()
 *  
 *  Summary:
 *    Returns the graphics device with the greatest area of
 *    intersection with a specified window region.
 *  
 *  Discussion:
 *    HIWindowGetGreatestAreaDisplay is recommended for use instead of
 *    this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to compare against.
 *    
 *    inRegion:
 *      The window region to compare against.
 *    
 *    outGreatestDevice:
 *      On exit, the graphics device with the greatest intersection.
 *      May be NULL.
 *    
 *    outGreatestDeviceRect:
 *      On exit, the bounds of the graphics device with the greatest
 *      intersection. May be NULL. If the device with the greatest
 *      intersection also contains the menu bar, the device rect will
 *      exclude the menu bar area.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowGreatestAreaDevice(
  WindowRef          inWindow,
  WindowRegionCode   inRegion,
  GDHandle *         outGreatestDevice,           /* can be NULL */
  Rect *             outGreatestDeviceRect)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowGetGreatestAreaDisplay()
 *  
 *  Summary:
 *    Returns the display with the greatest area of intersection with a
 *    specified window region.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to compare against.
 *    
 *    inRegion:
 *      The window region to compare against.
 *    
 *    inSpace:
 *      The coordinate space in which the display bounds should be
 *      returned. This must be either kHICoordSpaceScreenPixel or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outGreatestDisplay:
 *      On exit, the display with the greatest intersection. May be
 *      NULL if you don't need this information.
 *    
 *    outGreatestDisplayRect:
 *      On exit, the bounds of the display with the greatest
 *      intersection. May be NULL if you don't need this information.
 *      If the device with the greatest intersection also contains the
 *      menu bar, the display rect will exclude the menu bar area. This
 *      rectangle is returned in the specified coordinate space.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetGreatestAreaDisplay(
  WindowRef            inWindow,
  WindowRegionCode     inRegion,
  HICoordinateSpace    inSpace,
  CGDirectDisplayID *  outGreatestDisplay,           /* can be NULL */
  HIRect *             outGreatestDisplayRect)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  WindowConstrainOptions
 *  
 */
typedef UInt32 WindowConstrainOptions;
enum {

  /*
   * The window may be resized if necessary to make it fit onscreen.
   */
  kWindowConstrainMayResize     = (1 << 0),

  /*
   * The window will be moved even if it doesn't fit entirely onscreen.
   */
  kWindowConstrainMoveRegardlessOfFit = (1 << 1),

  /*
   * Allow partial intersection of the specified window region with the
   * screen, instead of requiring total intersection.
   */
  kWindowConstrainAllowPartial  = (1 << 2),

  /*
   * Only calculate the new window bounds; don't actually move the
   * window.
   */
  kWindowConstrainCalcOnly      = (1 << 3),

  /*
   * Use TransitionWindow with kWindowSlideTransitionEffect to move
   * windows onscreen. Available in Mac OS X 10.2.
   */
  kWindowConstrainUseTransitionWindow = (1 << 4),

  /*
   * Requests that the window be moved the minimum amount necessary to
   * be onscreen. This option applies if a partial fit is not allowed
   * (kWindowConstrainAllowPartial is not specified) or a partial fit
   * is allowed, but the window is not even partially visible. In
   * either case, the window will be moved just enough to be slightly
   * onscreen. You may customize the minimum amount that is required to
   * be visible by passing the desired dimensions in the inMinimumSize
   * parameter to HIWindowConstrain.
   */
  kWindowConstrainMoveMinimum   = (1 << 6),

  /*
   * Requests that when determining the bounds of the window region to
   * be constrained, rather than getting the current bounds of the
   * specified region, the Window Manager should use the bounds
   * provided in the ioBounds parameter to HIWindowConstrain. This
   * allows your application to constrain a window to a hypothetical
   * location; for example, if you plan to move your window such that
   * its content rect is at a certain location, and you want to know in
   * advance before moving the window whether the window would be
   * offscreen at that location, you can use this option.
   */
  kWindowConstrainUseSpecifiedBounds = (1 << 8),

  /*
   * The most common options: don't resize the window, move the window
   * regardless of fit to the screen, require total intersection of the
   * specified window region with the screen, and do actually move the
   * window.
   */
  kWindowConstrainStandardOptions = kWindowConstrainMoveRegardlessOfFit
};

#if !__LP64__
/*
 *  HIWindowConstrain()
 *  
 *  Summary:
 *    Moves and resizes a window to be within a specified bounding
 *    rectangle.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to constrain.
 *    
 *    inRegionCode:
 *      The window region to constrain.
 *    
 *    inOptions:
 *      Flags controlling how the window is constrained.
 *    
 *    inSpace:
 *      The coordinate space in which the inScreenBounds,
 *      inMinimumSize, and ioBounds parameters are expressed. This
 *      parameter must be either kHICoordSpaceScreenPixels or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    inScreenBounds:
 *      A rectangle, in either screen pixel coordinates or 72DPI
 *      virtual coordinates, within which to constrain the window. You
 *      may pass NULL if you don't need to specify a screen bounds. If
 *      NULL, the window is constrained to the screen that has the
 *      greatest intersection with the specified window region.
 *    
 *    inMinimumSize:
 *      A minimum size, in either screen pixel dimensions or 72DPI
 *      virtual dimensions, that should be kept within the specified
 *      screen bounds. This parameter is ignored if the
 *      kWindowConstrainMoveMinimum option is not set. Even if that
 *      option is set, you may still pass NULL if you don't need to
 *      customize the minimum dimensions. If NULL, the minimum size is
 *      currently seven 72DPI units in both x and y dimensions, but
 *      these values may change in different versions of Mac OS X.
 *    
 *    ioBounds:
 *      On entry, if the inOptions parameter contains
 *      kWindowConstrainUseSpecifiedBounds, then this parameter should
 *      be a bounding rect of the specified window region, in either
 *      screen pixel coordinates or 72DPI global coordinates. The
 *      bounding rect does not have to match the actual current bounds
 *      of the specified region; it may be a hypothetical bounds that
 *      you would like to constrain without actually moving the window
 *      to that location. 
 *      
 *      On exit, contains the new structure bounds of the window, in
 *      either screen pixel coordinates or 72DPI virtual coordinates.
 *      You may pass NULL if you don't need the window bounds returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowConstrain(
  WindowRef                inWindow,
  WindowRegionCode         inRegionCode,
  WindowConstrainOptions   inOptions,
  HICoordinateSpace        inSpace,
  const HIRect *           inScreenBounds,       /* can be NULL */
  const HISize *           inMinimumSize,        /* can be NULL */
  HIRect *                 ioBounds)             /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  ConstrainWindowToScreen()
 *  
 *  Summary:
 *    Moves and resizes a window so that it's contained entirely on a
 *    single screen.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindowRef:
 *      The window to constrain.
 *    
 *    inRegionCode:
 *      The window region to constrain.
 *    
 *    inOptions:
 *      Flags controlling how the window is constrained.
 *    
 *    inScreenRect:
 *      A rectangle, in global coordinates, in which to constrain the
 *      window. May be NULL. If NULL, the window is constrained to the
 *      screen with the greatest intersection with the specified window
 *      region.
 *    
 *    outStructure:
 *      On exit, contains the new structure bounds of the window, in
 *      global coordinates. May be NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ConstrainWindowToScreen(
  WindowRef                inWindowRef,
  WindowRegionCode         inRegionCode,
  WindowConstrainOptions   inOptions,
  const Rect *             inScreenRect,       /* can be NULL */
  Rect *                   outStructure)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetAvailableWindowPositioningBounds()
 *  
 *  Summary:
 *    Returns the available window positioning bounds on the given
 *    screen (i.e., the screen rect minus the MenuBar and Dock if
 *    located on that screen).
 *  
 *  Discussion:
 *    HIWindowGetAvailablePositioningBounds is recommended for use
 *    instead of this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDevice:
 *      The device for which to find the available bounds. May be NULL
 *      to request the bounds of the main device.
 *    
 *    outAvailableRect:
 *      On exit, contains the available bounds for the given device.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetAvailableWindowPositioningBounds(
  GDHandle   inDevice,
  Rect *     outAvailableRect)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetAvailableWindowPositioningRegion()
 *  
 *  Summary:
 *    Returns the available window positioning region on the given
 *    screen (i.e., the screen rect minus the MenuBar and Dock if
 *    located on that screen).
 *  
 *  Discussion:
 *    GetAvailableWindowPositionRegion differs from
 *    GetAvailableWindowPositioningBounds in that the Bounds API
 *    removes the entire area that may theoretically be covered by the
 *    Dock, even if the the Dock does not currently reach from edge to
 *    edge of the device on which it is positioned. The Region API
 *    includes the area at the sides of the Dock that is not covered by
 *    the Dock in the available region. 
 *    
 *    HIWindowCopyAvailablePositioningShape is recommended for use
 *    instead of this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDevice:
 *      The device for which to find the available bounds. May be NULL
 *      to request the bounds of the main device.
 *    
 *    ioRgn:
 *      On entry, contains a preallocated RgnHandle. On exit, the
 *      RgnHandle has been modified to contain the available region for
 *      the given device.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetAvailableWindowPositioningRegion(
  GDHandle    inDevice,
  RgnHandle   ioRgn)                                          AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIWindowGetAvailablePositioningBounds()
 *  
 *  Summary:
 *    Returns the available window positioning bounds on the given
 *    display (the display bounds minus the menu bar and Dock if
 *    located on that display).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDisplay:
 *      The display for which to find the available bounds. May be
 *      kCGNullDirectDisplay to request the bounds of the main display.
 *    
 *    inSpace:
 *      The coordinate space in which the positioning bounds should be
 *      returned. This must be either kHICoordSpaceScreenPixel or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outAvailableRect:
 *      On exit, contains the available bounds for the given display.
 *      This rectangle is returned in the specified coordinate space.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetAvailablePositioningBounds(
  CGDirectDisplayID   inDisplay,
  HICoordinateSpace   inSpace,
  HIRect *            outAvailableRect)                       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowCopyAvailablePositioningShape()
 *  
 *  Summary:
 *    Returns the available window positioning bounds on the given
 *    display (the display bounds minus the menu bar and Dock if
 *    located on that display).
 *  
 *  Discussion:
 *    HIWindowCopyAvailablePositioningShape differs from
 *    HIWindowGetAvailablePositioningBounds in that the Bounds API
 *    removes the entire area that may theoretically be covered by the
 *    Dock, even if the the Dock does not currently reach from edge to
 *    edge of the device on which it is positioned. The Shape API
 *    includes the area at the sides of the Dock that is not covered by
 *    the Dock in the available shape.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDisplay:
 *      The display for which to find the available shape. May be
 *      kCGNullDirectDisplay to request the shape of the main display.
 *    
 *    inSpace:
 *      The coordinate space in which the positioning shape should be
 *      returned. This must be either kHICoordSpaceScreenPixel or
 *      kHICoordSpace72DPIGlobal.
 *    
 *    outShape:
 *      On exit, contains the available shape for the given display.
 *      This shape is returned in the specified coordinate space.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowCopyAvailablePositioningShape(
  CGDirectDisplayID   inDisplay,
  HICoordinateSpace   inSpace,
  HIShapeRef *        outShape)                               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Visibility                                                                  */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HideWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HideWindow(WindowRef window)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  [Mac]ShowWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
#if TARGET_OS_MAC
    #define MacShowWindow ShowWindow
#endif
extern void 
MacShowWindow(WindowRef window)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ShowHide()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ShowHide(
  WindowRef   window,
  Boolean     showFlag)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  [Mac]IsWindowVisible()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
#if TARGET_OS_MAC
    #define MacIsWindowVisible IsWindowVisible
#endif
extern Boolean 
MacIsWindowVisible(WindowRef window)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HIWindowIsOnActiveSpace()
 *  
 *  Summary:
 *    Returns whether a window is attached to the currently active
 *    space.
 *  
 *  Discussion:
 *    For visible windows, this API indicates whether the window is
 *    currently visible on the active space; for invisible windows, it
 *    indicates whether the window would be visible on the active space
 *    if it were visible at this moment. The window's visible state
 *    still controls whether the window is actually visible or not.
 *    
 *    
 *    Windows that have availability attributes of either
 *    kHIWindowCanJoinAllSpaces or kHIWindowMoveToActiveSpace will
 *    always return true from this API.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to examine.
 *  
 *  Result:
 *    True if the window is attached to the current space or to all
 *    spaces, or false if the window is attached solely to some other
 *    space.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.6 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowIsOnActiveSpace(WindowRef inWindow)                   AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


/*
  ——————————————————————————————————————————————————————————————————————————————————————
    • Latent window visibility
  ——————————————————————————————————————————————————————————————————————————————————————
*/

#endif  /* !__LP64__ */


/*
 *  WindowLatentVisibility
 *  
 *  Summary:
 *    Reasons why a window is currently invisible when ordinarily it
 *    would be visible.
 */
typedef UInt32 WindowLatentVisibility;
enum {

  /*
   * Window is a floater and floating windows are hidden
   */
  kWindowLatentVisibleFloater   = 1 << 0,

  /*
   * Window has HideOnSuspend and we are suspended
   */
  kWindowLatentVisibleSuspend   = 1 << 1,

  /*
   * Window has HideOnFullScreen and we are full-screen
   */
  kWindowLatentVisibleFullScreen = 1 << 2,

  /*
   * Window's process is hidden
   */
  kWindowLatentVisibleAppHidden = 1 << 3,

  /*
   * Window is in an owned group and the owner was collapsed
   */
  kWindowLatentVisibleCollapsedOwner = 1 << 4,

  /*
   * Window is in a HideOnCollapse group and another window in the
   * group was collapsed
   */
  kWindowLatentVisibleCollapsedGroup = 1 << 5
};

#if !__LP64__
/*
 *  IsWindowLatentVisible()
 *  
 *  Summary:
 *    Indicates whether a window is visible onscreen and also whether
 *    it is latently visible but not currently onscreen.
 *  
 *  Discussion:
 *    All windows are either onscreen or offscreen. A window that is
 *    offscreen may still be latently visible; this occurs, for
 *    example, when a floating window is hidden as an application is
 *    suspended. The floating window is not visible onscreen, but it is
 *    latently visible and is only hidden due to the suspended state of
 *    the application; when the application becomes active again, the
 *    floating window will be placed back onscreen.
 *    IsWindowLatentVisible may be used to determine both the window's
 *    onscreen/offscreen status and its latent visibility (if the
 *    window is offscreen).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose visibility to return.
 *    
 *    outLatentVisible:
 *      If the window is onscreen, the latent visibility is zero. If
 *      the window is offscreen, this parameter is used to return the
 *      latent visibility flags of the window. If any of the flags are
 *      set, then the window is latently visible.
 *  
 *  Result:
 *    Indicates whether the window is currently onscreen.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsWindowLatentVisible(
  WindowRef                 inWindow,
  WindowLatentVisibility *  outLatentVisible)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;



/*
  ——————————————————————————————————————————————————————————————————————————————————————
    • Window Availability for Exposé
  ——————————————————————————————————————————————————————————————————————————————————————
*/

#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Window availability options for Exposé and Spaces.
 *  
 *  Discussion:
 *    These options are used with the HIWindowGet/ChangeAvailability
 *    APIs to override the default behavior of a window in Exposé and
 *    Spaces. Most applications should not override the default
 *    behavior; these options should only be used in special cases. By
 *    default, newly created windows of class kDocumentWindowClass are
 *    given an availability of zero (meaning that they are available
 *    during Exposé, and are only visible in the Space in which they
 *    were created), and windows from all other window classes are
 *    given an availability of kHIWindowExposeHidden.
 */
enum {

  /*
   * This window is hidden during Exposé’s “All windows” and
   * “Application windows” modes, and during the Spaces overview mode.
   * It moves to the current space automatically after a space switch,
   * and does not trigger a space switch when dragged to a space
   * boundary. If this bit is not set, the window is visible during
   * Exposé and Spaces.
   */
  kHIWindowBehaviorTransient    = 1 << 0,

  /*
   * This window is visible during Exposé’s “All windows” and
   * “Application windows” modes, and does not move. It remains in its
   * original position and, when clicked in "Show desktop" mode,
   * receives the mouse event.
   */
  kHIWindowBehaviorStationary   = 1 << 1,

  /*
   * This window is visible in all window sets managed by Spaces. If
   * this bit is not set, the window is only visible in the Space where
   * it was created. This bit and the kHIWindowMoveToActiveSpace bit
   * should not both be set.
   */
  kHIWindowCanJoinAllSpaces     = 1 << 8,

  /*
   * When made visible, this window is always shown in the current
   * Space, rather than the space in which it was last visible. When
   * activated, this window moves to the active space, rather than
   * forcing a switch to the Space on which it was previously located.
   * This option is typically used with modeless dialog windows such as
   * the Quick Search window in BBEdit. This bit and the
   * kHIWindowCanJoinAllSpaces bit should not both be set.
   */
  kHIWindowMoveToActiveSpace    = 1 << 9,

  /*
   * The original name for the constant kHIWindowBehaviorTransient.
   * Please do not use this name.
   */
  kHIWindowExposeHidden         = kHIWindowBehaviorTransient,

  /*
   * The original name for the constant kHIWindowCanJoinAllSpaces.
   * Please do not use this name.
   */
  kHIWindowVisibleInAllSpaces   = kHIWindowCanJoinAllSpaces
};


typedef OptionBits                      HIWindowAvailability;
#if !__LP64__
/*
 *  HIWindowGetAvailability()
 *  
 *  Summary:
 *    Returns the availability of a window during Exposé.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose availability to return.
 *    
 *    outAvailability:
 *      On exit, contains the window availability.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetAvailability(
  WindowRef               inWindow,
  HIWindowAvailability *  outAvailability)                    AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*
 *  HIWindowChangeAvailability()
 *  
 *  Summary:
 *    Alters the availability of a window during Exposé.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose availability to change.
 *    
 *    inSetAvailability:
 *      The availability bits to set.
 *    
 *    inClearAvailability:
 *      The availability bits to clear.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowChangeAvailability(
  WindowRef              inWindow,
  HIWindowAvailability   inSetAvailability,
  HIWindowAvailability   inClearAvailability)                 AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
    • Sheets
    
    Sheets are a new user interface object in Mac OS X. A sheet is a modal alert or dialog,
    but unlike a traditional alert or dialog window (which is visually separate from the
    frontmost document window), a sheet appears to be attached to its parent window; it
    moves and z-orders simultaneously with its parent. Furthermore, sheets on Mac OS X
    use a new type of modality called window modality. A traditional alert or dialog is
    app-modal; it prevents user interaction with all other windows in the current application.
    A sheet is window-modal; it only prevents user interaction with its parent window, and
    events continue to flow to other windows in the application.
    
    • Sheet Event Handling
    
    Implementing a sheet window in your application generally requires some modifications
    to your event-handling code. A traditional app-modal window is implemented using a modal
    event loop; your application starts a new event loop (either by processing events itself,
    or by calling ModalDialog), which does not return back to your application's main event
    loop until the app-modal window has closed.
    
    Starting a new event loop doesn't work with sheets, because typically the modal event loop
    will only handle events destined for the sheet, and not events for other windows, but
    a sheet only blocks events for its parent window, and your application must still handle
    events for the rest of its windows as normal. Therefore, you will usually not use a modal
    event loop to handle events in a sheet. Rather, you will show the sheet window, and then
    return directly back to your main event loop. The Carbon Event Manager automatically 
    prevents events from reaching the sheet's parent window; events in your application's
    other windows are still returned to you via WaitNextEvent or your application's Carbon
    event handlers, where you can process them as normal.
    
    You have several choices for handling events in the sheet itself. A sheet is, at the most
    basic level, simply another window in your application, and you can use any of the standard
    event-handling APIs to receive events in the sheet. For example, you can:
    
        -   receive events in the sheet via WaitNextEvent, and handle them directly in your
            main event loop
            
        -   create the sheet using Dialog Manager APIs, and use IsDialogEvent and DialogSelect
            to handle events in the sheet
            
        -   install Carbon event handlers on the sheet, and respond to events in your handlers
    
    Which approach you choose is up to you.
    
    • Sheets in CarbonLib
    
    The sheet window class, sheet WDEF procIDs, and ShowSheetWindow, HideSheetWindow, and
    GetSheetWindowParent APIs are implemented in CarbonLib starting with version 1.3. However,
    since Mac OS 8 and 9 do not traditionally support a window-modal user interface, sheet
    windows are displayed as app-modal windows by CarbonLib. From your application's perspective,
    event handling for a sheet in CarbonLib is the same as event handling for a sheet on X;
    ShowSheetWindow still returns immediately, and your application should still return back
    to its main event loop and be prepared to handle events in other windows. On CarbonLib,
    your application will simply never receive any user input in any of your other windows;
    since the sheet has application modality, the Carbon Event Manager will discard events
    in any windows other than the sheet.
    
    • Creating a Sheet
    
    A sheet is just a normal window with a special window class: kSheetWindowClass or
    kSheetAlertWindowClass. As such, it can be created in any of the ways you might create
    a window: NewWindow, NewCWindow, CreateNewWindow, GetNewWindow, GetNewCWindow, 
    CreateWindowFromCollection, CreateWindowFromResource, CreateWindowFromNib, NewDialog,
    NewColorDialog, NewFeaturesDialog, or GetNewDialog.
    
    The Window Manager defines two window classes and two WDEF procIDs for sheets:
        
        -   kSheetWindowClass and kSheetAlertWindowClass
        -   kWindowSheetProc and kWindowSheetAlertProc
        
    The window classes may be used with CreateNewWindow, CreateWindowFromCollection, and
    CreateWindowFromResource; the WDEF procIDs may be used with NewWindow, NewCWindow, NewDialog,
    NewColorDialog, NewFeaturesDialog, and in 'WDEF' and 'DLOG' resources.
    
    Mac OS X 10.0 only supports kSheetWindowClass and kWindowSheetProc;
    it does not support kSheetAlertWindowClass or kWindowSheetAlertProc. The latter window
    class and procID were added in CarbonLib 1.3 and Mac OS X 10.1. A new window class and
    procID were necessary for CarbonLib support because
    sheets can be used for both alerts ("Do you want to save changes before closing this
    window?") and dialogs (a Navigation Services PutFile dialog). On Mac OS X, sheet windows
    have the same appearance when used for either an alert or a dialog, but on Mac OS 8 and 9,
    alert windows have a different appearance from dialog windows. Two separate window classes
    are necessary for CarbonLib to know whether to display a sheet using a movable alert or a
    movable dialog window. Therefore, it is recommended that you use kSheetAlertWindowClass when
    creating a sheet window that will be used to display an alert, although this is not required.
    
    • Displaying a Sheet
    
    A sheet is made visible by calling the ShowSheetWindow API. This API shows the sheet,
    using whatever visual effects are appropriate for the platform, and then returns immediately.
    On Mac OS X, it creates a window group and places the sheet and its parent window into the
    group; it also marks the sheet as window-modal. On CarbonLib, it marks the sheet as app-modal
    but does not create a window group.
    
    On Mac OS X, before the sheet window is actually made visible, ShowSheetWindow sends a 
    kEventWindowDrawContent event to the sheet window, asking it to draw its content into the
    window's offscreen buffer. The sheet must handle this event, or its content area will be
    blank after the sheet becomes visible.
    
    In some cases, this handler is automatically provided for you:
    
        -   If you create your sheet window using the Dialog Manager, the Dialog Manager
            automatically installs a handler for this event that calls DrawDialog, so you
            don't need to install the handler yourself.
            
        -   If you install the standard Carbon window event handler on your sheet window
            (using kWindowStandardHandlerAttribute or InstallStandardEventHandler), the
            standard handler automatically handles this event and calls DrawControls.
            
    Typically, your event handling code (whether it uses WaitNextEvent, the Dialog Manager,
    or Carbon event handlers) will receive and respond to events in the sheet until the
    user does something that should cause the sheet to close. This might be clicking in an
    OK or Cancel button, for example. At that time, your event handling code must call either
    HideSheetWindow or DetachSheetWindow. The sheet window will hide, but will not be destroyed,
    so you can use it again later if you want.
    
    • Closing a sheet
    
    A sheet is normally hidden by calling the HideSheetWindow API. HideSheetWindow provides the
    visual effects of zooming the sheet back into the parent window's title bar and moving the
    parent window back to its original position. In Mac OS X 10.3 and later, the DetachSheetWindow
    API is also available. DetachSheetWindow ungroups the sheet from its parent, releases the
    retain count acquired by ShowSheetWindow on the parent window, and removes all event handlers
    installed by ShowSheetWindow, but does not hide the sheet window; an application would typically
    call DetachSheetWindow and then HideWindow to hide the sheet window without the sheet closing
    animation. This may be useful if, for example, the sheet were being used to ask if changes to
    a modified document should be saved; if the user chooses "Don’t Save", then the application
    could use DetachSheetWindow and then hide both the sheet and the document immediately, so that
    the document closes as quickly as possible without taking time for the closing animation.
    The Navigation Services Save Changes dialog does this automatically.
    
    You _must_ call either HideSheetWindow or DetachSheetWindow before destroying a sheet that has
    been shown with ShowSheetWindow. You may not simply dispose of a sheet window without first using
    Hide or DetachSheetWindow; doing so will leave an extra refcount on the parent window, and will
    leave the parent window still embedded in the sheet window group.
    
    • Sheet Transparency
    
    Sheets should be transparent so that the user can see some of the document content behind
    the sheet and remember the context in which the sheet was displayed. In Mac OS X 10.1, 
    a sheet is made transparent by using the kThemeBrushSheetBackgroundTransparent constant for
    the sheet window’s theme background brush. In Mac OS X 10.2 and later, sheets are only
    transparent if this brush is used and if the sheet window uses compositing mode (enabled by
    setting the kWindowCompositingAttribute window attribute when the sheet window is created).
*/
/*
 *  ShowSheetWindow()
 *  
 *  Summary:
 *    Shows a sheet window using appropriate visual effects.
 *  
 *  Discussion:
 *    ShowSheetWindow is implemented in both CarbonLib 1.3 and Mac OS
 *    X. Since Mac OS 9 does not use a window-modal user interface for
 *    alerts and dialogs, ShowSheetWindow in CarbonLib does not bind
 *    the sheet to the parent window in the same way that it does on
 *    Mac OS X; instead, it shows the sheet like a standard
 *    movable-modal dialog window. Sheet windows must use the window
 *    classes kSheetWindowClass or kSheetAlertWindowClass to get the
 *    right appearance on both platforms. 
 *    
 *    Note that ShowSheetWindow will increment the retain count of the
 *    parent window. The retain count is decremented by HideSheetWindow
 *    and DetachSheetWindow. You must call one of those APIs before
 *    destroying the sheet window. 
 *    
 *    On Mac OS X, ShowSheetWindow sets the modality of the sheet
 *    window to kWindowModalityWindowModal.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSheet:
 *      The sheet window to show.
 *    
 *    inParentWindow:
 *      The sheet's parent window.
 *  
 *  Result:
 *    An operating system result code. ShowSheetWindow checks for the
 *    following error conditions, and returns paramErr if any occur:
 *    the sheet window must have a window class of kSheetWindowClass or
 *    kSheetAlertWindowClass; the sheet and parent windows must not be
 *    the same window; the sheet must not have a parent window already;
 *    and the parent window must not already be the target of a sheet.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ShowSheetWindow(
  WindowRef   inSheet,
  WindowRef   inParentWindow)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HideSheetWindow()
 *  
 *  Summary:
 *    Hides a sheet window using appropriate visual effects.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSheet:
 *      The sheet window to hide.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HideSheetWindow(WindowRef inSheet)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  DetachSheetWindow()
 *  
 *  Summary:
 *    Detaches a sheet window from its parent window without affecting
 *    the visibility or position of the sheet or its parent.
 *  
 *  Discussion:
 *    This API may be useful if you want to hide a sheet window without
 *    an animation effect. To do that, use DetachSheetWindow to detach
 *    the sheet from the parent, and then use HideWindow to hide the
 *    sheet, or DisposeWindow to destroy the sheet.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSheet:
 *      The sheet to detach.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.3 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DetachSheetWindow(WindowRef inSheet)                          AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


/*
 *  GetSheetWindowParent()
 *  
 *  Summary:
 *    Returns the parent window of a sheet.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inSheet:
 *      The sheet window whose parent to retrieve.
 *    
 *    outParentWindow:
 *      On exit, contains the parent window of the sheet.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.3 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetSheetWindowParent(
  WindowRef    inSheet,
  WindowRef *  outParentWindow)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
    • Drawers
    
    Drawers are supported in Carbon by Mac OS X 10.2 and later. To create and use a drawer:
    
        -   Create a window using kDrawerWindowClass. You must also use compositing mode with
            drawers; this mode is enabled by passing kWindowCompositingAttribute to CreateNewWindow.
            
        -   Either install the standard window event handler on the drawer window, or use
            your own Carbon event or WaitNextEvent code to handle user interaction with the
            drawer. If you do not use the standard window event handler, you should call
            ResizeWindow in response to clicks on the grow region of the drawer if you want
            the drawer to be resizable.
            
        -   Set the drawer's parent window with the SetDrawerParent function. The parent is
            the window on which the drawer will open.
            
        -   Optionally, install a Carbon event handler on the drawer or the drawer's parent
            window for the kEventWindowDrawerOpening/Opened/Closing/Closed events, to be notified
            when the drawer has started or finished opening or closing.
        
        -   Optionally, set the drawer's preferred edge on the parent window with the
            SetDrawerPreferredEdge function. If you do not set a preferred edge, the drawer
            opens on the parent's left side on a left-to-right system, or on the parent's right
            side on a right-to-left system.
            
        -   Optionally, set the drawer's offsets with the SetDrawerOffsets function. The offsets
            control the amount of inset between the edge of the parent's content area and the edge
            of the drawer's structure. If you do not set any offsets, the drawer's edges are flush
            with the edges of the parent's content.
            
        -   Optionally, set the drawer's minimum and maximum sizes with the SetWindowResizeLimits
            function. Or, install a Carbon event handler on the drawer to handle the kEventWindow-
            GetMinimumSize and kEventWindowGetMaximumSize events. The drawer's minimum and maximum
            sizes control how small or large it will resize together with the parent. If you do not
            set resize limits, the drawer will be capable of resizing to default small and large
            limits.
        
        -   Call ToggleDrawer to open or close the drawer, or use OpenDrawer or CloseDrawer if
            you require more control over how the drawer opens or closes.
*/
#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Indicates the parent window edge on which a drawer will be shown.
 */
enum {

  /*
   * This constant is typically used with the OpenDrawer API; it
   * indicates that the drawer should be opened on whatever edge of the
   * parent window has previously been set as the drawer's preferred
   * edge.
   */
  kWindowEdgeDefault            = 0,

  /*
   * The drawer should open on the top edge of the parent window.
   */
  kWindowEdgeTop                = 1 << 0,

  /*
   * The drawer should open on the left edge of the parent window.
   */
  kWindowEdgeLeft               = 1 << 1,

  /*
   * The drawer should open on the bottom edge of the parent window.
   */
  kWindowEdgeBottom             = 1 << 2,

  /*
   * The drawer should open on the right edge of the parent window.
   */
  kWindowEdgeRight              = 1 << 3
};


/*
 *  Summary:
 *    Indicates the current state of a drawer window.
 */
enum {

  /*
   * The drawer is opening, but is not yet fully open.
   */
  kWindowDrawerOpening          = 1,

  /*
   * The drawer is fully open.
   */
  kWindowDrawerOpen             = 2,

  /*
   * The drawer is closing, but is not yet fully closed.
   */
  kWindowDrawerClosing          = 3,

  /*
   * The drawer is fully closed.
   */
  kWindowDrawerClosed           = 4
};

typedef UInt32                          WindowDrawerState;

#if !__LP64__
/*
 *  GetDrawerPreferredEdge()
 *  
 *  Summary:
 *    Returns the preferred parent window edge of a drawer.
 *  
 *  Discussion:
 *    Drawers start out with a preferred parent window edge of
 *    kWindowEdgeDefault. On left-to-right systems, the default edge is
 *    the left edge of the parent window; on right-to-left systems, the
 *    default edge is the right edge. You can set the preferred edge
 *    with SetDrawerPreferredEdge. If there's not enough room on the
 *    preferred edge, the drawer will automatically switch to the
 *    opposite edge.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose preferred edge to retrieve.
 *  
 *  Result:
 *    The preferred edge of the drawer window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OptionBits 
GetDrawerPreferredEdge(WindowRef inDrawerWindow)              AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  SetDrawerPreferredEdge()
 *  
 *  Summary:
 *    Sets the preferred parent window edge of a drawer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose preferred edge to set.
 *    
 *    inEdge:
 *      The preferred edge of the drawer window. Note that although the
 *      WindowEdge enumeration has values appropriate for a bitfield,
 *      the current implementation does not support receiving more than
 *      one edge bit in this parameter. You can also pass
 *      kWindowEdgeDefault to allow the Window Manager to pick an edge.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetDrawerPreferredEdge(
  WindowRef    inDrawerWindow,
  OptionBits   inEdge)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetDrawerCurrentEdge()
 *  
 *  Summary:
 *    Returns the current parent window edge on which a drawer is
 *    displayed.
 *  
 *  Discussion:
 *    If the drawer window is currently visible, this API returns the
 *    parent window edge on which the drawer is displayed. If the
 *    drawer is not visible, this API determines on which edge of the
 *    parent window the drawer should be displayed, given the current
 *    size of the drawer, position of the parent, and preferred edge
 *    for the drawer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose current edge to retrieve.
 *  
 *  Result:
 *    The current edge of the drawer window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OptionBits 
GetDrawerCurrentEdge(WindowRef inDrawerWindow)                AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetDrawerState()
 *  
 *  Summary:
 *    Returns the current state of a drawer: opening, open, closing, or
 *    closed.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose state to retrieve.
 *  
 *  Result:
 *    The current state of the drawer window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowDrawerState 
GetDrawerState(WindowRef inDrawerWindow)                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetDrawerParent()
 *  
 *  Summary:
 *    Returns the parent window of a drawer.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose parent window to retrieve.
 *  
 *  Result:
 *    The drawer's parent window, or NULL if the drawer has no assigned
 *    parent.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetDrawerParent(WindowRef inDrawerWindow)                     AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  SetDrawerParent()
 *  
 *  Summary:
 *    Sets the parent window of a drawer.
 *  
 *  Discussion:
 *    In Mac OS X 10.4 and 10.5, this API incorrectly increments the
 *    refcount of the drawer window, and you must either destroy the
 *    drawer parent window or use SetDrawerParent( drawer, NULL ) on
 *    the drawer window before destroying the drawer. This extra step
 *    is not required in Mac OS X 10.2, 10.3, or 10.6 and later; in
 *    those versions you may simply release the drawer window when
 *    you're done with it and it will be automatically removed from the
 *    parent.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose parent window to set.
 *    
 *    inParent:
 *      The drawer's new parent window, or NULL if the drawer should
 *      have no parent.
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
SetDrawerParent(
  WindowRef   inDrawerWindow,
  WindowRef   inParent)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  kWindowOffsetUnchanged
 *  
 *  Discussion:
 *    Pass this value to SetDrawerOffsets to indicate that an existing
 *    offset should not be changed.
 */
#define kWindowOffsetUnchanged          ((float)(-1.0))
#if !__LP64__
/*
 *  SetDrawerOffsets()
 *  
 *  Summary:
 *    Sets the offsets from the beginning and end of the parent window
 *    to the beginning and end of the drawer window.
 *  
 *  Discussion:
 *    The drawer offsets control the positioning of the drawer relative
 *    to its parent window. When a drawer is first created, its offsets
 *    are zero. When a drawer is positioned, it is initially given a
 *    height or width equal to the height or width of the content area
 *    of the parent to which it is attached. If a drawer is opening on
 *    the left side of its parent, for example, the drawer's height
 *    will be the height of the parent's content area. In this case,
 *    the top side of the drawer window is called the leading edge of
 *    the drawer, and the bottom side of the drawer window is called
 *    the trailing edge of the drawer. The drawer's size is then
 *    adjusted by the leading and trailing offsets. The leading edge of
 *    the drawer is moved inward by an amount equal to the leading
 *    offset, and the trailing edge is moved inward by an amount equal
 *    to the trailing offset. For example, if the leading and trailing
 *    offsets are five and fifteen, then the top edge of a left-opening
 *    drawer will be five pixels inside the top edge of the parent
 *    window's content area, and the bottom edge of the drawer will be
 *    fifteen pixels inside the bottom edge of the parent's content
 *    area.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose offsets to change.
 *    
 *    inLeadingOffset:
 *      The new leading offset, in pixels. Pass kWindowOffsetUnchanged
 *      if you don't want to change the leading offset.
 *    
 *    inTrailingOffset:
 *      The new trailing offset, in pixels. Pass kWindowOffsetUnchanged
 *      if you don't want to change the trailing offset.
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
SetDrawerOffsets(
  WindowRef   inDrawerWindow,
  CGFloat     inLeadingOffset,
  CGFloat     inTrailingOffset)                               AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetDrawerOffsets()
 *  
 *  Summary:
 *    Returns the offsets from the beginning and end of the parent
 *    window to the beginning and end of the drawer window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window whose offsets to retrieve.
 *    
 *    outLeadingOffset:
 *      On exit, contains the drawer's leading offset. Pass NULL if you
 *      don't need this information returned.
 *    
 *    outTrailingOffset:
 *      On exit, contains the drawer's trailing offset. Pass NULL if
 *      you don't need this information returned.
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
GetDrawerOffsets(
  WindowRef   inDrawerWindow,
  CGFloat *   outLeadingOffset,        /* can be NULL */
  CGFloat *   outTrailingOffset)       /* can be NULL */      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  ToggleDrawer()
 *  
 *  Summary:
 *    Toggles a drawer from open to closed, or vice versa.
 *  
 *  Discussion:
 *    ToggleDrawer opens the drawer if it is closed, opening, or
 *    closing. If the drawer is open, it closes the drawer.
 *    
 *    ToggleDrawer attempts to open the drawer on its preferred edge,
 *    but if there is not enough room on that edge, it will try the
 *    opposite edge instead. If there is insufficient room on either
 *    edge, the drawer will open on the preferred edge but may extend
 *    offscreen, under the Dock, or under the menubar.
 *    
 *    The opening or closing of the drawer is performed asynchronously;
 *    ToggleDrawer installs an event loop timer that opens or closes
 *    the drawer after ToggleDrawer returns to the caller. Therefore,
 *    the caller must be running its event loop for the drawer to open
 *    or close. To open or close the drawer synchronously, use the
 *    OpenDrawer or CloseDrawer APIs.
 *    
 *    ToggleDrawer retains the drawer window while the drawer is
 *    opening or closing, and releases it when the drawer is fully
 *    opened or closed.
 *    
 *    ToggleDrawer sends the kEventWindowDrawerOpening,
 *    kEventWindowDrawerOpened, kEventWindowDrawerClosing, and
 *    kEventWindowDrawerClosed events as the drawer opens or closes.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window to open or close.
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
ToggleDrawer(WindowRef inDrawerWindow)                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  OpenDrawer()
 *  
 *  Summary:
 *    Opens a drawer on a specified parent window edge.
 *  
 *  Discussion:
 *    OpenDrawer may open the drawer either synchronously or
 *    asynchronously, depending on the value of the inAsync parameter.
 *    If inAsync is true, OpenDrawer installs an event loop timer that
 *    opens the drawer after OpenDrawer returns to the caller;
 *    therefore, the caller must be running its event loop for the
 *    drawer to open. If inAsync is false, OpenDrawer opens the drawer
 *    completely before returning to the caller.
 *    
 *    OpenDrawer retains the drawer window while the drawer is opening,
 *    and releases it when the drawer is fully open.
 *    
 *    OpenDrawer sends the kEventWindowDrawerOpening event to the
 *    drawer, the drawer's parent, and the application before opening
 *    the drawer. If an event handler for this event returns
 *    userCanceledErr, OpenDrawer will return immediately without
 *    opening the drawer. OpenDrawer sends the kEventWindowDrawerOpened
 *    event to the drawer, the drawer's parent, and the application
 *    after the drawer has finished opening.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window to open.
 *    
 *    inEdge:
 *      The parent window edge on which to open the drawer. Pass
 *      kWindowEdgeDefault to use the drawer's preferred edge. If there
 *      is not enough room on the preferred edge, OpenDrawer will try
 *      the opposite edge instead. If there is insufficient room on
 *      either edge, the drawer will open on the preferred edge but may
 *      extend offscreen, under the Dock, or under the menubar.
 *    
 *    inAsync:
 *      Whether to open the drawer synchronously (the drawer is
 *      entirely opened before the function call returns) or
 *      asynchronously (the drawer opens using an event loop timer
 *      after the function call returns).
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
OpenDrawer(
  WindowRef    inDrawerWindow,
  OptionBits   inEdge,
  Boolean      inAsync)                                       AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  CloseDrawer()
 *  
 *  Summary:
 *    Closes a drawer.
 *  
 *  Discussion:
 *    CloseDrawer may close the drawer either synchronously or
 *    asynchronously, depending on the value of the inAsync parameter.
 *    If inAsync is true, CloseDrawer installs an event loop timer that
 *    closes the drawer after CloseDrawer returns to the caller;
 *    therefore, the caller must be running its event loop for the
 *    drawer to close. If inAsync is false, CloseDrawer closes the
 *    drawer completely before returning to the caller.
 *    
 *    CloseDrawer retains the drawer window while the drawer is
 *    closing, and releases it when the drawer is fully closed.
 *    
 *    CloseDrawer sends the kEventWindowDrawerClosing event to the
 *    drawer, the drawer's parent, and the application before closing
 *    the drawer. If an event handler for this event returns
 *    userCanceledErr, CloseDrawer will return immediately without
 *    closing the drawer. CloseDrawer sends the
 *    kEventWindowDrawerClosed event to the drawer, the drawer's
 *    parent, and the application after the drawer has finished closing.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inDrawerWindow:
 *      The drawer window to close.
 *    
 *    inAsync:
 *      Whether to close the drawer synchronously (the drawer is
 *      entirely closed before the function call returns) or
 *      asynchronously (the drawer closes using an event loop timer
 *      after the function call returns).
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
CloseDrawer(
  WindowRef   inDrawerWindow,
  Boolean     inAsync)                                        AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  HIWindowCopyDrawers()
 *  
 *  Summary:
 *    Returns an array of the drawers that are attached to a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose drawers to return.
 *    
 *    outDrawers:
 *      On exit, an array containing WindowRefs. Each array entry is a
 *      drawer attached to the specified window. The array will be
 *      valid, but empty, if the window has no drawers.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowCopyDrawers(
  WindowRef     inWindow,
  CFArrayRef *  outDrawers)                                   AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Disabling Screen Redraw                                                            */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  DisableScreenUpdates()
 *  
 *  Summary:
 *    Temporarily disables redraw of the screen after changes to window
 *    geometry or window contents.
 *  
 *  Discussion:
 *    It is appropriate to disable updates if you are moving or
 *    resizing multiple windows and you want all of the geometry
 *    changes to appear onscreen simulataneously. In most other cases,
 *    you should strive to avoid disabling screen updates. The window
 *    server will automatically re-enable updates (and print a message
 *    to the Console log) if you leave updates disabled for a
 *    sufficiently long period of time, currently about 1 second.
 *    
 *    
 *    The window server records the number of calls to
 *    DisableScreenUpdates and does not re-enable updates until a
 *    matching number of calls to EnableScreenUpdates have been made.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An operating system result code. In practice, this API always
 *    returns noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
DisableScreenUpdates(void)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  EnableScreenUpdates()
 *  
 *  Summary:
 *    Re-enables redraw of the screen after changes to window geometry
 *    or window contents.
 *  
 *  Discussion:
 *    Screen redraw is not actually enabled until the number of calls
 *    to EnableScreenUpdates matches the number of calls to
 *    DisableScreenUpdates.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An operating system result code. In practice, this API always
 *    returns noErr.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
EnableScreenUpdates(void)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Toolbars                                                                    */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  SetWindowToolbar()
 *  
 *  Discussion:
 *    Sets the toolbar for a window. If any other toolbar is currently
 *    bound to the window, it is released. This API does NOT add the
 *    toolbar button to the window, your application must set the
 *    attribute itself.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to add the toolbar to.
 *    
 *    inToolbar:
 *      The toolbar to add.
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
SetWindowToolbar(
  WindowRef      inWindow,
  HIToolbarRef   inToolbar)                                   AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  GetWindowToolbar()
 *  
 *  Discussion:
 *    Gets the toolbar of a window, if any.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to add the toolbar to.
 *    
 *    outToolbar:
 *      The toolbar. You do not own the toolbar reference returned by
 *      this function. Do not release it! It is possible for the
 *      toolbar returned to be NULL, indicating there is no toolbar
 *      associated with this window. ••• NOTE: May need to change the
 *      release strategy here.
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
GetWindowToolbar(
  WindowRef       inWindow,
  HIToolbarRef *  outToolbar)                                 AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  ShowHideWindowToolbar()
 *  
 *  Discussion:
 *    Shows or hides a window's toolbar. Optionally, you can ask for
 *    the transition to be animated or not. Typically, you would not
 *    need to call this other than to set up your window accordingly.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to show or hide the toolbar for.
 *    
 *    inShow:
 *      Pass true to show the toolbar, false to hide it.
 *    
 *    inAnimate:
 *      Pass true to animate the transition, false to do it quickly and
 *      without fanfare.
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
ShowHideWindowToolbar(
  WindowRef   inWindow,
  Boolean     inShow,
  Boolean     inAnimate)                                      AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
 *  IsWindowToolbarVisible()
 *  
 *  Discussion:
 *    Returns whether the toolbar (if any) attached to a window is
 *    visible. If the window has no toolbar, false is returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window test the toolbar visiblity for.
 *  
 *  Result:
 *    A boolean result indicating whether the toolbar is visible (true)
 *    or not (false).
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.2 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IsWindowToolbarVisible(WindowRef inWindow)                    AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;


/*
    About Custom Toolbar Views
    
    A window with a custom toolbar view does not have an HIToolbarRef. No API that takes an HIToolbarRef will work.
    
    When a custom toolbar view is provided for a window, the Window Manager will:
        - set the view's HIViewID to kHIViewWindowToolbarID
        - embed the toolbar view in the root view of the window
        - make the toolbar view invisible
        
    These aspects of the standard toolbar support also work with custom toolbar views:
        - unmodified clicks on the toolbar button to show and hide the toolbar
        - ShowHideWindowToolbar and IsWindowToolbarVisible
        - kHICommandShow/HideToolbar and kHICommandToggleToolbar
        
    These aspects of the standard toolbar support do _not_ work with custom toolbar views:
        - option-click on toolbar button to toggle all windows with the same toolbar
        - command-click and command-shift-click on toolbar button to change display mode and size
        - command-option click on toolbar button to display config sheet
        
    A custom toolbar view must handle these events:
        - kEventControlGetOptimalBounds
        - kEventControlSetData with kHIToolbarViewDrawBackgroundTag
        - kEventControlDraw and draw, or not, as requested by the background tag
        
    A custom toolbar view may optionally handle:
        - kEventWindowAttributesChanged to be notified when window style changes
        - kHICommandCustomizeToolbar to present its own toolbar customization dialog
        - kHICommandToggleAllToolbars to implement multi-window toggling
        - kHICommandCycleToolbarModeSmaller/Larger to change display mode and size
        
    A custom toolbar view may need to make itself taller or shorter while the view is visible. To ensure
    correct window redraw when this happens, the window frame view must be involved or notified of the
    change in toolbar size. The correct way to do this depends on the Mac OS X version:
    
        - for Mac OS X 10.4.x, the toolbar view should use this code:
        
            WindowRef window = HIViewGetWindow( view );
            DisableScreenUpdates();
            ShowHideWindowToolbar( window, false, false );
            HIViewSetFrame( view, &newBounds );
            ShowHideWindowToolbar( window, true, false );
            EnableScreenUpdates();
        
        - for Mac OS X 10.5 and later, the toolbar view may either use the above code or may instead
          send a kEventControlOptimalBoundsChanged event to itself. The window frame view will observe
          this event and resize the toolbar view automatically. Note that in this case, the view should
          not resize itself; instead, it should be prepared to receive a kEventControlGetOptimalBounds
          event and return its new optimal size in response.
*/
#endif  /* !__LP64__ */


/*
 */
enum {

  /*
   * A SetControlData tag that is used by the standard window frame
   * view to inform the toolbar view whether the view should draw its
   * background or leave its background transparent. The data for this
   * tag is a Boolean. If the data value is true, the toolbar view
   * should draw its background as it desires. If the data value is
   * false, the toolbar view should leave its background transparent so
   * that the window's root view can show through the toolbar view.
   * Currently, the toolbar view will be asked to leave its background
   * transparent for windows with the textured or unified appearance.
   */
  kHIToolbarViewDrawBackgroundTag = 'back'
};

#if !__LP64__
/*
 *  HIWindowSetToolbarView()
 *  
 *  Summary:
 *    Sets a custom toolbar view for a window.
 *  
 *  Discussion:
 *    This API is provided for use by applications that cannot use the
 *    HIToolbarRef API. For best compatibility with future versions of
 *    Mac OS X, we highly recommend that you use the HIToolbar API if
 *    possible. However, if HIToolbar is not sufficient for your needs,
 *    you can provide a custom toolbar view that will be placed at the
 *    standard location inside the window frame. You are responsible
 *    for defining the appearance and behavior of the view. You cannot
 *    use this API to customize the view that is associated with an
 *    HIToolbarRef; a window with an HIToolbarRef uses a standard
 *    HIToolbox-provided view that cannot be customized. When using a
 *    custom toolbar view, no API that takes an HIToolbarRef will work
 *    with that window. 
 *    
 *    The HIWIndowSetToolbarView API is available in Mac OS X 10.4.9
 *    and later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose toolbar view to set.
 *    
 *    inView:
 *      The custom toolbar view for the window. You may pass NULL to
 *      remove the custom view from the window. Setting a custom view
 *      will also remove any HIToolbarRef that is associated with the
 *      window. 
 *      
 *      After a custom toolbar view has been set, the window owns the
 *      view and will release it automatically when the window is
 *      destroyed, or when a different custom view or standard
 *      HIToolbar is set for the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetToolbarView(
  WindowRef   inWindow,
  HIViewRef   inView)         /* can be NULL */               AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Transparency                                                                */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  SetWindowAlpha()
 *  
 *  Discussion:
 *    Alters the overall alpha of a window, making the entire window
 *    (including window frame) either more or less transparent. The
 *    alpha is expressed as a floating point value from 0.0 (completely
 *    transparent) to 1.0 (completely opaque).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose alpha to modify.
 *    
 *    inAlpha:
 *      The new alpha value.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowAlpha(
  WindowRef   inWindow,
  CGFloat     inAlpha)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetWindowAlpha()
 *  
 *  Discussion:
 *    Returns the current overall alpha value for a window. The alpha
 *    is expressed as a floating point value from 0.0 (completely
 *    transparent) to 1.0 (completely opaque).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose alpha to return.
 *    
 *    outAlpha:
 *      On exit, contains the window's current alpha value.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowAlpha(
  WindowRef   inWindow,
  CGFloat *   outAlpha)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Shadows                                                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowInvalidateShadow()
 *  
 *  Summary:
 *    This API causes a window's shadow to be recalculated.
 *  
 *  Discussion:
 *    HIWindowInvalidateShadow is not typically used by applications.
 *    However, it may be useful for applications with customized window
 *    frames that change shape dynamically; in this case, after the
 *    application has drawn the new window shape, the window shadow
 *    must be recalculated to follow the new window shape. 
 *    
 *    This API causes the window shadow to be immediately recalculated
 *    and redrawn based on the current contents of the window's back
 *    buffer. For best performance and visual appearance, you should
 *    follow these steps when invalidating a window shadow: disable
 *    updates with DisableScreenUpdates, draw, flush, invalidate the
 *    shadow, and enable updates. For a compositing window, after
 *    invalidating any views that should be redrawn, you will need to
 *    explicitly draw and flush using HIWindowFlush, rather than
 *    waiting for the event loop to draw and flush the window, because
 *    you cannot disable updates or invalidate the window shadow if
 *    drawing is done via the event loop.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowInvalidateShadow(WindowRef inWindow)                  AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Scaling for Resolution Independence                                         */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowGetBackingScaleFactor()
 *  
 *  Summary:
 *    Returns the scale factor representing the number of backing store
 *    pixels corresponding to each linear unit in window space on this
 *    WindowRef.
 *  
 *  Discussion:
 *    This is generally only necessary when building a bitmap context
 *    or image whose resolution needs to match that of a particular
 *    WindowRef. Note that a WindowRef's backing scale factor can
 *    change over time, such as when the window moves from one display
 *    to another, or when a display's resolution changes, so clients
 *    should not cache the value returned by this function.
 *    HIWindowGetBackingScaleFactor is only available on Mac OS X
 *    Version 10.7.3 and later.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef whose backing scale factor to provide.
 *  
 *  Result:
 *    The backing scale factor of the window.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.7 and later
 *    Non-Carbon CFM:   not available
 */
extern CGFloat 
HIWindowGetBackingScaleFactor(WindowRef inWindow)             AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  HIWindowScaleMode
 *  
 *  Discussion:
 *    A window's scale mode indicates in which resolution-independent
 *    scale mode it is operating.
 */
typedef UInt32 HIWindowScaleMode;
enum {

  /*
   * The window is not scaled at all because the display scale factor
   * is 1.0.
   */
  kHIWindowScaleModeUnscaled    = 0,

  /*
   * The window's backing store is being magnified by the window server
   * because the display scale factor != 1.0, and because the window
   * was created without kWindowFrameworkScaledAttribute.
   */
  kHIWindowScaleModeMagnified   = 1,

  /*
   * The window's context has been scaled to match the display scale
   * factor because the display scale factor != 1.0 and because the
   * window was created with kWindowFrameworkScaledAttribute.
   */
  kHIWindowScaleModeFrameworkScaled = 2
};

#if !__LP64__
/*
 *  HIWindowGetScaleMode()   *** DEPRECATED ***
 *  
 *  Discussion:
 *    This function is deprecated and should not be used by
 *    applications targeting Mac OS X 10.7 or later. Please use an
 *    appropriate AppKit API instead. Clients desiring high resolution
 *    windows should switch to use NSWindows instead of Carbon
 *    WindowRefs.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef whose scale mode to provide.
 *    
 *    outMode:
 *      On exit, this is always kHIWindowScaleModeUnscaled.
 *    
 *    outScaleFactor:
 *      On exit, this is always 1.0.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only] but deprecated in 10.7
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.4 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowGetScaleMode(
  WindowRef            inWindow,
  HIWindowScaleMode *  outMode,
  CGFloat *            outScaleFactor)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window FullScreen Transition                                                       */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  HIWindowToggleFullScreen()
 *  
 *  Summary:
 *    Causes a window to enter or exit fullscreen.
 *  
 *  Discussion:
 *    If the window is not already fullscreen and the window is capable
 *    of becoming fullscreen -- see kHIWindowBitFullScreenPrimary --
 *    the window will enter fullscreen. Otherwise, if the window is
 *    already fullscreen, the window will exit fullscreen.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose fullscreen state to toggle.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowToggleFullScreen(WindowRef inWindow)                  AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;


/*
 *  HIWindowIsFullScreen()
 *  
 *  Summary:
 *    Returns whether the window is fullscreen.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose fullscreen state to query.
 *  
 *  Result:
 *    A Boolean indicating whether the window is fullscreen: True means
 *    fullscreen, and false means not fullscreen.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.7 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HIWindowIsFullScreen(WindowRef inWindow)                      AVAILABLE_MAC_OS_X_VERSION_10_7_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Content Border                                                              */
/*——————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */


/*
 *  HIContentBorderMetrics
 *  
 *  Summary:
 *    Describes the border of the content area of the window that will
 *    contain the window frame gradient.
 */
struct HIContentBorderMetrics {

  /*
   * Height of the top of the content border area.
   */
  CGFloat             top;

  /*
   * Width of the left of the content border area.
   */
  CGFloat             left;

  /*
   * Height of the bottom of the content border area.
   */
  CGFloat             bottom;

  /*
   * Width of the right of the content border area.
   */
  CGFloat             right;
};
typedef struct HIContentBorderMetrics   HIContentBorderMetrics;
#if !__LP64__
/*
 *  HIWindowSetContentBorderThickness()
 *  
 *  Summary:
 *    Sets the thickness of the window border that extends into the
 *    content area of the window.
 *  
 *  Discussion:
 *    In Mac OS X 10.5, windows can have a gradient on the top and
 *    bottom section of the window which have the appearance and
 *    behavior of being part of the window frame. The window frame
 *    gradient is drawn or extended into this border and the window is
 *    draggable from this area. Functionally, the area actually extends
 *    into the content of the window where the client can embed
 *    controls or print status messages a la iTunes. This can be set on
 *    all non-floating windows.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose content border thickness is being set.
 *    
 *    inBorderThickness:
 *      A pointer to a structure that indicates how much of the content
 *      area is used for the frame gradient drawing. Currently only the
 *      top and bottom fields are allowed. If any value other than 0 is
 *      used for the left or right fields, this function will return
 *      paramErr.
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HIWindowSetContentBorderThickness(
  HIWindowRef                     inWindow,
  const HIContentBorderMetrics *  inBorderThickness)          AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*
 *  HIWindowGetContentBorderThickness()
 *  
 *  Summary:
 *    Retrieves the border thickness of the content area of the window.
 *  
 *  Discussion:
 *    In Mac OS X 10.5, windows can have a gradient on the top and
 *    bottom section of the window which have the appearance and
 *    behavior of being part of the window frame. This is actually the
 *    border of the content area of the window, and this API returns
 *    the metrics of this content border.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window to be queried.
 *  
 *  Result:
 *    A structure describing the metrics of the content view that is
 *    drawn with the window frame appearance.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIContentBorderMetrics 
HIWindowGetContentBorderThickness(HIWindowRef inWindow)       AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Properties                                                                  */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
   Routines available from Mac OS 8.5 forward
   or from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward
*/

/*
 *  GetWindowProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowProperty(
  WindowRef         window,
  PropertyCreator   propertyCreator,
  PropertyTag       propertyTag,
  ByteCount         bufferSize,
  ByteCount *       actualSize,            /* can be NULL */
  void *            propertyBuffer)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowPropertySize()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowPropertySize(
  WindowRef         window,
  PropertyCreator   creator,
  PropertyTag       tag,
  ByteCount *       size)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetWindowProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowProperty(
  WindowRef         window,
  PropertyCreator   propertyCreator,
  PropertyTag       propertyTag,
  ByteCount         propertySize,
  const void *      propertyBuffer)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RemoveWindowProperty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
RemoveWindowProperty(
  WindowRef         window,
  PropertyCreator   propertyCreator,
  PropertyTag       propertyTag)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* Routines available from Mac OS 8.1 forward when linking to CarbonLib 1.0 forward*/

#endif  /* !__LP64__ */

enum {
  kWindowPropertyPersistent     = 0x00000001 /* whether this property is preserved when the system causes your application to be quit and relaunched */
};

#if !__LP64__
/*
 *  GetWindowPropertyAttributes()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
GetWindowPropertyAttributes(
  WindowRef     window,
  OSType        propertyCreator,
  OSType        propertyTag,
  OptionBits *  attributes)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  ChangeWindowPropertyAttributes()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ChangeWindowPropertyAttributes(
  WindowRef    window,
  OSType       propertyCreator,
  OSType       propertyTag,
  OptionBits   attributesToSet,
  OptionBits   attributesToClear)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Utilities                                                                          */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  PinRect()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
PinRect(
  const Rect *  theRect,
  Point         thePt)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Part Tracking                                                               */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  TrackBox()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
TrackBox(
  WindowRef        window,
  Point            thePt,
  WindowPartCode   partCode)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  TrackGoAway()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
TrackGoAway(
  WindowRef   window,
  Point       thePt)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Accessors                                                                   */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
 *  GetWindowGoAwayFlag()
 *  
 *  Discussion:
 *    use GetWindowAttributes in Carbon
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */


/*
 *  GetWindowSpareFlag()
 *  
 *  Discussion:
 *    use GetWindowAttributes in Carbon
 *  
 *  Availability:
 *    Mac OS X:         not available [32-bit only]
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */


/*
 *  GetWindowList()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later
 */
extern WindowRef 
GetWindowList(void)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetWindowPort()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern CGrafPtr 
GetWindowPort(WindowRef window)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetWindowStructurePort()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.5 and later
 *    Non-Carbon CFM:   not available
 */
extern CGrafPtr 
GetWindowStructurePort(WindowRef inWindow)                    AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;



/*
 *  GetWindowKind()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern short 
GetWindowKind(WindowRef window)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  IsWindowHilited()
 *  
 *  Summary:
 *    Indicates whether a window's frame is hilited.
 *  
 *  Discussion:
 *    See HiliteWindow for a disucssion on the meaning of a window's
 *    hilited state.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose hilited state you wish to retrieve.
 *  
 *  Result:
 *    A Boolean indicating whether the window's frame is hilited.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern Boolean 
IsWindowHilited(WindowRef window)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  IsWindowUpdatePending()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern Boolean 
IsWindowUpdatePending(WindowRef window)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  [Mac]GetNextWindow()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0.2 and later or as macro/inline
 */
#if TARGET_OS_MAC
    #define MacGetNextWindow GetNextWindow
#endif
extern WindowRef 
MacGetNextWindow(WindowRef window)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetPreviousWindow()
 *  
 *  Summary:
 *    Returns the window above a given window in the window list.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window above this window is returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.2 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.6 and later
 *    Non-Carbon CFM:   not available
 */
extern WindowRef 
GetPreviousWindow(WindowRef inWindow)                         AVAILABLE_MAC_OS_X_VERSION_10_2_AND_LATER;



/*
 *  GetWindowStandardState()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern Rect * 
GetWindowStandardState(
  WindowRef   window,
  Rect *      rect)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetWindowUserState()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern Rect * 
GetWindowUserState(
  WindowRef   window,
  Rect *      rect)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetWindowKind()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern void 
SetWindowKind(
  WindowRef   window,
  short       kind)                                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetWindowStandardState()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern void 
SetWindowStandardState(
  WindowRef     window,
  const Rect *  rect)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/*
 *  SetWindowUserState()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern void 
SetWindowUserState(
  WindowRef     window,
  const Rect *  rect)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  SetPortWindowPort()
 *  
 *  Discussion:
 *    set the current QuickDraw port to the port associated with the
 *    window
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern void 
SetPortWindowPort(WindowRef window)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/*
 *  GetWindowPortBounds()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern Rect * 
GetWindowPortBounds(
  WindowRef   window,
  Rect *      bounds)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
 *  GetWindowFromPort()
 *  
 *  Discussion:
 *    Needed to ‘cast up’ to a WindowRef from a GrafPtr
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in CarbonAccessors.o 1.0 and later or as macro/inline
 */
extern WindowRef 
GetWindowFromPort(CGrafPtr port)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* old accessors*/

#endif  /* !__LP64__ */

/*
 *  GetWindowDataHandle()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  SetWindowDataHandle()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  GetWindowZoomFlag()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  GetWindowStructureRgn()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  GetWindowContentRgn()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  GetWindowUpdateRgn()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*
 *  GetWindowTitleWidth()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */



/*--------------------------------------------------------------------------------------*/
/*  • DEPRECATED                                                                        */
/*                                                                                      */
/*  All functions below this point are either deprecated (they continue to function     */
/*  but are not the most modern nor most efficient solution to a problem), or they are  */
/*  completely unavailable on Mac OS X.                                                 */
/*--------------------------------------------------------------------------------------*/
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Definition Messages                                                         */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kWindowMsgDraw                = 0,
  kWindowMsgHitTest             = 1,
  kWindowMsgCalculateShape      = 2,
  kWindowMsgInitialize          = 3,
  kWindowMsgCleanUp             = 4,
  kWindowMsgDrawGrowOutline     = 5,
  kWindowMsgDrawGrowBox         = 6
};

/* Messages available from Mac OS 8.0 forward*/
enum {
  kWindowMsgGetFeatures         = 7,
  kWindowMsgGetRegion           = 8
};

/* Messages available from Mac OS 8.5 forward*/
enum {
  kWindowMsgDragHilite          = 9,    /* parameter boolean indicating on or off*/
  kWindowMsgModified            = 10,   /* parameter boolean indicating saved (false) or modified (true)*/
  kWindowMsgDrawInCurrentPort   = 11,   /* same as kWindowMsgDraw, but must draw in current port*/
  kWindowMsgSetupProxyDragImage = 12,   /* parameter pointer to SetupWindowProxyDragImageRec*/
  kWindowMsgStateChanged        = 13,   /* something about the window's state has changed*/
  kWindowMsgMeasureTitle        = 14    /* measure and return the ideal title width*/
};

/* Messages only available in Carbon*/
enum {
  kWindowMsgGetGrowImageRegion  = 19    /* get region to xor during grow/resize. parameter pointer to GetGrowImageRegionRec.*/
};

/* old names*/
enum {
  wDraw                         = kWindowMsgDraw,
  wHit                          = kWindowMsgHitTest,
  wCalcRgns                     = kWindowMsgCalculateShape,
  wNew                          = kWindowMsgInitialize,
  wDispose                      = kWindowMsgCleanUp,
  wGrow                         = kWindowMsgDrawGrowOutline,
  wDrawGIcon                    = kWindowMsgDrawGrowBox
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • State-changed Flags for kWindowMsgStateChanged                                     */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  kWindowStateTitleChanged      = (1 << 0)
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • WDEF Message Types                                                                 */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*
   SetupWindowProxyDragImageRec - setup the proxy icon drag image
   Both regions are allocated and disposed by the Window Manager.
   The GWorld is disposed of by the Window Manager, but the WDEF must allocate
   it.  See Technote on Drag Manager 1.1 additions for more information and sample code for
   setting up drag images.
*/

struct SetupWindowProxyDragImageRec {
  GWorldPtr           imageGWorld;            /* locked GWorld containing the drag image - output - can be NULL*/
  RgnHandle           imageRgn;               /* image clip region, contains the portion of the image which gets blitted to screen - preallocated output - if imageGWorld is NULL, this is ignored*/
  RgnHandle           outlineRgn;             /* the outline region used on shallow monitors - preallocated output - must always be non-empty*/
};
typedef struct SetupWindowProxyDragImageRec SetupWindowProxyDragImageRec;
/* MeasureWindowTitleRec - a pointer to this is passed in WDEF param for kWindowMsgMeasureTitle*/
struct MeasureWindowTitleRec {
                                              /* output parameters (filled in by the WDEF)*/
  SInt16              fullTitleWidth;         /* text + proxy icon width*/
  SInt16              titleTextWidth;         /* text width*/

                                              /* input parameters*/
  Boolean             isUnicodeTitle;
  Boolean             unused;                 /* future use*/
};
typedef struct MeasureWindowTitleRec    MeasureWindowTitleRec;
typedef MeasureWindowTitleRec *         MeasureWindowTitleRecPtr;
/*
   GetGrowImageRegionRec - generate a region to be xored during GrowWindow and ResizeWindow.
   This is passed along with a kWindowMsgGetGrowImageRegion message. On input, the growRect
   parameter is the window's new bounds in global coordinates. The growImageRegion parameter
   will be allocated and disposed automatically; the window definition should alter the 
   region appropriately.
*/
struct GetGrowImageRegionRec {
  Rect                growRect;
  RgnHandle           growImageRegion;
};
typedef struct GetGrowImageRegionRec    GetGrowImageRegionRec;
/* GetWindowRegionRec - a pointer to this is passed in WDEF param for kWindowMsgGetRegion*/
struct GetWindowRegionRec {
  RgnHandle           winRgn;
  WindowRegionCode    regionCode;
};
typedef struct GetWindowRegionRec       GetWindowRegionRec;
typedef GetWindowRegionRec *            GetWindowRegionPtr;
typedef GetWindowRegionRec *            GetWindowRegionRecPtr;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Desktop Pattern Resource ID                                                        */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  deskPatID                     = 16
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Color Part Codes                                                            */
/*——————————————————————————————————————————————————————————————————————————————————————*/
enum {
  wContentColor                 = 0,
  wFrameColor                   = 1,
  wTextColor                    = 2,
  wHiliteColor                  = 3,
  wTitleBarColor                = 4
};

/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • Window Color Table                                                                 */
/*——————————————————————————————————————————————————————————————————————————————————————*/
struct WinCTab {
  long                wCSeed;                 /* reserved */
  short               wCReserved;             /* reserved */
  short               ctSize;                 /* usually 4 for windows */
  ColorSpec           ctTable[5];
};
typedef struct WinCTab                  WinCTab;
typedef WinCTab *                       WCTabPtr;
typedef WCTabPtr *                      WCTabHandle;
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • WindowRecords                                                                      */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* • AuxWinHandle                                                                       */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*——————————————————————————————————————————————————————————————————————————————————————*/
/*  • BasicWindowDescription                                                            */
/*                                                                                      */
/*  Contains statically-sized basic attributes of the window, for storage in a          */
/*  collection item.                                                                    */
/*——————————————————————————————————————————————————————————————————————————————————————*/
/* constants for the version field*/
enum {
  kWindowDefinitionVersionOne   = 1,
  kWindowDefinitionVersionTwo   = 2
};

/* constants for the stateflags bit field */
enum {
  kWindowIsCollapsedState       = (1 << 0L)
};

struct BasicWindowDescription {
  UInt32              descriptionSize;        /* sizeof(BasicWindowDescription)*/

  Rect                windowContentRect;      /* location on screen*/
  Rect                windowZoomRect;         /* location on screen when zoomed out*/
  URefCon             windowRefCon;           /* the refcon - __avoid saving stale pointers here__  */
  UInt32              windowStateFlags;       /* window state bit flags*/
  WindowPositionMethod  windowPositionMethod; /* method last used by RepositionWindow to position the window (if any)*/

  UInt32              windowDefinitionVersion;
  union {
    struct {
      SInt16              windowDefProc;      /* defProc and variant*/
      Boolean             windowHasCloseBox;
    }                       versionOne;

    struct {
      WindowClass         windowClass;        /* the class*/
      WindowAttributes    windowAttributes;   /* the attributes*/
    }                       versionTwo;

  }                       windowDefinition;
};
typedef struct BasicWindowDescription   BasicWindowDescription;
/*  the window manager stores the default collection items using these IDs*/
enum {
  kStoredWindowSystemTag        = 'appl', /* Only Apple collection items will be of this tag*/
  kStoredBasicWindowDescriptionID = 'sbas', /* BasicWindowDescription*/
  kStoredWindowPascalTitleID    = 's255', /* pascal title string*/
  kStoredWindowTitleCFStringID  = 'cfst' /* CFString title string*/
};


/*
 *  DeskHookProcPtr
 *  
 *  Summary:
 *    Callback function that handles mouse clicks on the desktop.
 */
typedef CALLBACK_API( void , DeskHookProcPtr )(Boolean mouseClick, EventRecord *theEvent);

/*
 *  WindowPaintProcPtr
 *  
 *  Summary:
 *    Callback function that paints a window's content area.
 */
typedef CALLBACK_API( OSStatus , WindowPaintProcPtr )(GDHandle device, GrafPtr qdContext, WindowRef window, RgnHandle inClientPaintRgn, RgnHandle outSystemPaintRgn, void *refCon);
typedef STACK_UPP_TYPE(DeskHookProcPtr)                         DeskHookUPP;
typedef STACK_UPP_TYPE(WindowPaintProcPtr)                      WindowPaintUPP;
/*
 *  NewDeskHookUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  NewWindowPaintUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern WindowPaintUPP
NewWindowPaintUPP(WindowPaintProcPtr userRoutine)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  DisposeDeskHookUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  DisposeWindowPaintUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeWindowPaintUPP(WindowPaintUPP userUPP)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

/*
 *  InvokeDeskHookUPP()
 *  
 *  Availability:
 *    Mac OS X:         not available
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   available as macro/inline
 */

/*
 *  InvokeWindowPaintUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeWindowPaintUPP(
  GDHandle        device,
  GrafPtr         qdContext,
  WindowRef       window,
  RgnHandle       inClientPaintRgn,
  RgnHandle       outSystemPaintRgn,
  void *          refCon,
  WindowPaintUPP  userUPP)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;

#if __MACH__
  #ifdef __cplusplus
    inline WindowPaintUPP                                       NewWindowPaintUPP(WindowPaintProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeWindowPaintUPP(WindowPaintUPP) { }
    inline OSStatus                                             InvokeWindowPaintUPP(GDHandle device, GrafPtr qdContext, WindowRef window, RgnHandle inClientPaintRgn, RgnHandle outSystemPaintRgn, void * refCon, WindowPaintUPP userUPP) { return (*userUPP)(device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon); }
  #else
    #define NewWindowPaintUPP(userRoutine)                      ((WindowPaintUPP)userRoutine)
    #define DisposeWindowPaintUPP(userUPP)
    #define InvokeWindowPaintUPP(device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon, userUPP) (*userUPP)(device, qdContext, window, inClientPaintRgn, outSystemPaintRgn, refCon)
  #endif
#endif

#if !__LP64__
/*
 *  NewWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CreateNewWindow instead of NewWindow.
 *  
 *  Summary:
 *    Creates a new window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern WindowRef 
NewWindow(
  void *             wStorage,
  const Rect *       boundsRect,
  ConstStr255Param   title,
  Boolean            visible,
  short              theProc,
  WindowRef          behind,
  Boolean            goAwayFlag,
  SRefCon            refCon)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  NewCWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CreateNewWindow instead of NewCWindow.
 *  
 *  Summary:
 *    Creates a new window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern WindowRef 
NewCWindow(
  void *             wStorage,
  const Rect *       boundsRect,
  ConstStr255Param   title,
  Boolean            visible,
  short              procID,
  WindowRef          behind,
  Boolean            goAwayFlag,
  SRefCon            refCon)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetNewCWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use nib files and CreateWindowFromNib instead of GetNewCWindow.
 *  
 *  Summary:
 *    Loads a window from a 'WIND' resource.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern WindowRef 
GetNewCWindow(
  short       windowID,
  void *      wStorage,
  WindowRef   behind)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetNewWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use nib files and CreateWindowFromNib instead of GetNewWindow.
 *  
 *  Summary:
 *    Loads a window from a 'WIND' resource.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern WindowRef 
GetNewWindow(
  short       windowID,
  void *      wStorage,
  WindowRef   behind)                                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CreateWindowFromResource()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use nib files and CreateWindowFromNib instead of
 *    CreateWindowFromResource.
 *  
 *  Summary:
 *    Loads a window from a 'wind' resource.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
CreateWindowFromResource(
  SInt16       resID,
  WindowRef *  outWindow)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  StoreWindowIntoCollection()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIArchiveEncodeCFType to encode a window to an archive
 *    instead of StoreWindowIntoCollection.
 *  
 *  Summary:
 *    Stores a description of a window into a Collection Manager
 *    collection.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
StoreWindowIntoCollection(
  WindowRef    window,
  Collection   collection)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CreateWindowFromCollection()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIArchiveCopyDecodedCFType to decode a window from an archive
 *    instead of CreateWindowFromCollection.
 *  
 *  Summary:
 *    Creates a window from a window description stored in a Collection
 *    Manager collection.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
CreateWindowFromCollection(
  Collection   collection,
  WindowRef *  outWindow)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWindowOwnerCount()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.2 and later, use CFGetRetainCount instead of
 *    GetWindowOwnerCount.
 *  
 *  Summary:
 *    Returns the retain count of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowOwnerCount(
  WindowRef    window,
  ItemCount *  outCount)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CloneWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.2 and later, use CFRetain instead of CloneWindow.
 *  
 *  Summary:
 *    Increments the retain count of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
CloneWindow(WindowRef window)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWindowRetainCount()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    In Mac OS X 10.2 and later, use CFGetRetainCount instead of
 *    GetWindowRetainCount.
 *  
 *  Summary:
 *    Returns the retain count of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern ItemCount 
GetWindowRetainCount(WindowRef inWindow)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  RetainWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.2 and later, use CFRetain instead of RetainWindow.
 *  
 *  Summary:
 *    Increments the retain count of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
RetainWindow(WindowRef inWindow)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  ReleaseWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    On Mac OS X 10.2 and later, use CFRelease instead of
 *    ReleaseWindow.
 *  
 *  Summary:
 *    Decrements the retain count of a window, and destroys the window
 *    if the retain count falls to zero.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
ReleaseWindow(WindowRef inWindow)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWVariant()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use GetWindowAttributes to determine aspects of a window's
 *    appearance or behavior.
 *  
 *  Summary:
 *    Retrieves the window variant code for a window.
 *  
 *  Discussion:
 *    The window variant code is the low four bits of a window's
 *    procID. This API is no longer recommended for use.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern short 
GetWVariant(WindowRef window)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetWindowClass()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowChangeClass or SetWindowGroup or
 *    ChangeWindowAttributes (or all three) instead of SetWindowClass.
 *  
 *  Summary:
 *    Changes the window class of a window.
 *  
 *  Discussion:
 *    SetWindowClass changes the class of a window. It also changes the
 *    window's z-order so that it is grouped with other windows of the
 *    same class. It does not change the visual appearance of the
 *    window. In CarbonLib, SetWindowClass may not be used to change a
 *    non-utility window to have utility window class, or to make a
 *    utility window have non-utility class. SetWindowClass is
 *    available from CarbonLib 1.1 forward.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The window whose class to change.
 *    
 *    inWindowClass:
 *      The new window class.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
SetWindowClass(
  WindowRef     inWindow,
  WindowClass   inWindowClass)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


#endif  /* !__LP64__ */

enum {
  kWindowPaintProcOptionsNone   = 0
};


typedef OptionBits                      WindowPaintProcOptions;
#if !__LP64__
/*
 *  InstallWindowContentPaintProc()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use an event handler for kEventControlDraw on a window's content
 *    view instead of installing a window content paint proc.
 *  
 *  Summary:
 *    Installs a callback function that is used by non-compositing
 *    windows to erase the window background.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 9.0 and later
 */
extern OSStatus 
InstallWindowContentPaintProc(
  WindowRef                window,
  WindowPaintUPP           paintProc,
  WindowPaintProcOptions   options,
  void *                   refCon)          /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  ClipAbove()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Do not use this function on Mac OS X; it does nothing.
 *  
 *  Summary:
 *    Sets the clip region of the Window Manager port to be the desktop
 *    minus the structure regions of the windows in front of the
 *    specified window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
ClipAbove(WindowRef window)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PaintOne()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Applications never need to call PaintOne. Use InvalWindowRect,
 *    InvalWindowRgn, or HIViewSetNeedsDisplay to invalidate a portion
 *    of a window.
 *  
 *  Summary:
 *    Repaints a portion of a window's structure and content.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PaintOne(
  WindowRef   window,             /* can be NULL */
  RgnHandle   clobberedRgn)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PaintBehind()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Applications never need to call PaintBehind. Use InvalWindowRect,
 *    InvalWindowRgn, or HIViewSetNeedsDisplay to invalidate a portion
 *    of a window.
 *  
 *  Summary:
 *    Repaints the windows that intersect with a specified region.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PaintBehind(
  WindowRef   startWindow,        /* can be NULL */
  RgnHandle   clobberedRgn)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CalcVis()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    The visible region of a window on Mac OS X is managed by the
 *    window server. Applications never need to call this function.
 *  
 *  Summary:
 *    Recalculates the visible region of a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
CalcVis(WindowRef window)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CalcVisBehind()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    The visible region of a window on Mac OS X is managed by the
 *    window server. Applications never need to call this function.
 *  
 *  Summary:
 *    Recalculates the visible region of windows that intersect with a
 *    specified region.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
CalcVisBehind(
  WindowRef   startWindow,        /* can be NULL */
  RgnHandle   clobberedRgn)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  CheckUpdate()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use AcquireFirstMatchingEventInQueue instead of CheckUpdate.
 *  
 *  Summary:
 *    Searches the event queue for the next available update event.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
CheckUpdate(EventRecord * theEvent)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  FrontWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use ActiveNonFloatingWindow, FrontNonFloatingWindow, or
 *    GetFrontWindowOfClass instead of FrontWindow.
 *  
 *  Summary:
 *    Returns the frontmost visible window in the window list.
 *  
 *  Discussion:
 *    The frontmost visible window is not necessarily a document or
 *    dialog window, or even a window created by your application. For
 *    example, it may be a menu window, a Text Services Manager
 *    bottom-line input window, a help tag, or a floating window. If
 *    your code needs the frontmost document or dialog window, use the
 *    ActiveNonFloatingWindow or FrontNonFloatingWindow APIs instead of
 *    FrontWindow. For compatibility with existing applications,
 *    FrontWindow ignores all windows of class kMenuBarWindowClass and
 *    instead returns the frontmost visible window behind the menubar.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    The frontmost visible window, or NULL if no windows are visible.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern WindowRef 
FrontWindow(void)                                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetWindowPic()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use an HIImageView placed in the window instead of SetWindowPic.
 *  
 *  Summary:
 *    Causes a picture to be drawn in a window's content area.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetWindowPic(
  WindowRef   window,
  PicHandle   pic)                                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWindowPic()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    If using an HIImageView to draw a window's content, ask the image
 *    view for its image instead of calling GetWindowPic.
 *  
 *  Summary:
 *    Retrieves the picture being drawn in a window's content area.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern PicHandle 
GetWindowPic(WindowRef window)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetWindowProxyFSSpec()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowSetProxyFSRef instead of SetWindowProxyFSSpec.
 *  
 *  Summary:
 *    Set the proxy icon for a window using an FSSpec to an existing
 *    file system object (volume, folder, or file).
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon to set.
 *    
 *    inFile:
 *      The file system object that the window represents. The window’s
 *      proxy icon is determined by asking Icon Services for the icon
 *      of this object.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
SetWindowProxyFSSpec(
  WindowRef       window,
  const FSSpec *  inFile)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWindowProxyFSSpec()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use HIWindowGetProxyFSRef instead of GetWindowProxyFSSpec.
 *  
 *  Summary:
 *    Returns the FSSpec used to determine the proxy icon for a window.
 *  
 *  Discussion:
 *    This API will return noErr and a valid FSSpec if the window’s
 *    proxy icon has been specified using the SetWindowProxyFSSpec or
 *    SetWindowProxyAlias APIs. If the window has no proxy icon, or if
 *    the icon was specified with another SetWindowProxy API, then an
 *    error will be returned.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window whose proxy icon FSSpec to return.
 *    
 *    outFile:
 *      On exit, contains the window’s proxy icon FSSpec.
 *  
 *  Result:
 *    noErr if the window’s proxy icon FSSpec has been returned;
 *    errWindowDoesNotHaveProxy if the window does not have a proxy
 *    icon, or if the proxy icon was specified by IconRef or
 *    type/creator rather than by FSSpec or alias. Other operating
 *    system error codes may also be returned.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern OSStatus 
GetWindowProxyFSSpec(
  WindowRef   window,
  FSSpec *    outFile)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IsWindowPathSelectClick()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use IsWindowPathSelectEvent instead of IsWindowPathSelectClick.
 *  
 *  Summary:
 *    Indicates whether an EventRecord describing a click on a window’s
 *    title should cause a path selection menu to be displayed.
 *  
 *  Discussion:
 *    Windows that have a proxy icon provided using an FSSpec or alias
 *    can support a path selection menu, which displays the file system
 *    path to the object, one menu item per directory. Making a
 *    selection from this item will automatically open the
 *    corresponding object in the Finder.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window on which the click occurred.
 *    
 *    event:
 *      The event. IsWindowPathSelectClick will only return true for
 *      mouseDown events.
 *  
 *  Result:
 *    true if the click should cause a path selection menu to be
 *    displayed, or false if not. If this API returns true, the
 *    application should call the WindowPathSelect API.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in WindowsLib 8.5 and later
 */
extern Boolean 
IsWindowPathSelectClick(
  WindowRef            window,
  const EventRecord *  event)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetWTitle()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use SetWindowTitleWithCFString instead of SetWTitle.
 *  
 *  Summary:
 *    Sets the title of a window using a Pascal string.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
SetWTitle(
  WindowRef          window,
  ConstStr255Param   title)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetWTitle()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CopyWindowTitleAsCFString instead of GetWTitle.
 *  
 *  Summary:
 *    Retrieves the title of a window as a Pascal string.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
GetWTitle(
  WindowRef   window,
  Str255      title)                                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DrawGrowIcon()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    DrawGrowIcon is deprecated from Mac OS 8.0 forward. Theme-savvy
 *    window defprocs automatically draw the grow box in the window
 *    frame.
 *  
 *  Summary:
 *    Draws the grow icon in a window.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DrawGrowIcon(WindowRef window)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GrowWindow()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use ResizeWindow instead of GrowWindow.
 *  
 *  Summary:
 *    Tracks the mouse while the user resizes a window, and returns the
 *    new width and height.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
GrowWindow(
  WindowRef     window,
  Point         startPt,
  const Rect *  bBox)          /* can be NULL */              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetGrayRgn()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    To determine the area in which a window may be positioned, use
 *    HIWindowCopyAvailablePositioningShape.
 *  
 *  Summary:
 *    Returns a region that covers the desktop area of all active
 *    displays.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern RgnHandle 
GetGrayRgn(void)                                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DragGrayRgn()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use a overlay window or other custom drawing instead of
 *    DragGrayRgn.
 *  
 *  Summary:
 *    Tracks the mouse as the user drags a gray outline of a region.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
DragGrayRgn(
  RgnHandle        theRgn,
  Point            startPt,
  const Rect *     limitRect,
  const Rect *     slopRect,
  short            axis,
  DragGrayRgnUPP   actionProc)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DragTheRgn()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use a overlay window or other custom drawing instead of
 *    DragTheRgn.
 *  
 *  Summary:
 *    Tracks the mouse as the user drags the outline of a region. The
 *    outline is drawn using the pattern specified in the DragPattern
 *    low-memory global.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern long 
DragTheRgn(
  RgnHandle        theRgn,
  Point            startPt,
  const Rect *     limitRect,
  const Rect *     slopRect,
  short            axis,
  DragGrayRgnUPP   actionProc)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*——————————————————————————————————————————————————————————————————————————————————————————————————*/
/* Obsolete symbolic names                                                                          */
/*——————————————————————————————————————————————————————————————————————————————————————————————————*/
#endif  /* !__LP64__ */

enum {
  kWindowGroupAttrSelectable    = kWindowGroupAttrSelectAsLayer,
  kWindowGroupAttrPositionFixed = kWindowGroupAttrMoveTogether,
  kWindowGroupAttrZOrderFixed   = kWindowGroupAttrLayerTogether
};










#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __MACWINDOWS__ */

