/*
     File:       HIToolbox/HIContainerViews.h
 
     Contains:   Definition of the container views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HICONTAINERVIEWS__
#define __HICONTAINERVIEWS__

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
 *  HIContainerViews.h
 *  
 *  Discussion:
 *    API definitions for the views that can contain other views: group
 *    box, placard, window header, and user pane.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ GROUP BOX (CDEF 10)                                                               */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  The group box CDEF can be use in several ways. It can have no title, a text title,  */
/*  a check box as the title, or a popup button as a title. There are two versions of   */
/*  group boxes, primary and secondary, which look slightly different.                  */
/* Group Box proc IDs */
enum {
  kControlGroupBoxTextTitleProc = 160,
  kControlGroupBoxCheckBoxProc  = 161,
  kControlGroupBoxPopupButtonProc = 162,
  kControlGroupBoxSecondaryTextTitleProc = 164,
  kControlGroupBoxSecondaryCheckBoxProc = 165,
  kControlGroupBoxSecondaryPopupButtonProc = 166
};

/* Control Kind Tag */
enum {
  kControlKindGroupBox          = 'grpb',
  kControlKindCheckGroupBox     = 'cgrp',
  kControlKindPopupGroupBox     = 'pgrp'
};

/* The HIObject class ID for the HIGroupBox class. */
#define kHIGroupBoxClassID              CFSTR("com.apple.HIGroupBox")
/* The HIObject class ID for the HICheckBoxGroup class. */
#define kHICheckBoxGroupClassID         CFSTR("com.apple.HICheckBoxGroup")
/* Creation APIs: Carbon only */
#if !__LP64__
/*
 *  CreateGroupBoxControl()
 *  
 *  Summary:
 *    Creates a group box control.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window that should contain the control.
 *    
 *    boundsRect:
 *      The bounding box of the control.
 *    
 *    title:
 *      The title of the control.
 *    
 *    primary:
 *      Whether to create a primary or secondary group box.
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
CreateGroupBoxControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,
  Boolean       primary,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreateCheckGroupBoxControl()
 *  
 *  Summary:
 *    Creates a group box control that has a check box as its title.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window that should contain the control.
 *    
 *    boundsRect:
 *      The bounding box of the control.
 *    
 *    title:
 *      The title of the control (used as the title of the check box).
 *    
 *    initialValue:
 *      The initial value of the check box.
 *    
 *    primary:
 *      Whether to create a primary or secondary group box.
 *    
 *    autoToggle:
 *      Whether to create an auto-toggling check box. Auto-toggling
 *      check box titles are only supported on Mac OS X; this parameter
 *      must be false when used with CarbonLib.
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
CreateCheckGroupBoxControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,
  SInt32        initialValue,
  Boolean       primary,
  Boolean       autoToggle,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreatePopupGroupBoxControl()
 *  
 *  Summary:
 *    Creates a group box control that has a popup button as its title.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window that should contain the control.
 *    
 *    boundsRect:
 *      The bounding box of the control.
 *    
 *    title:
 *      The title of the control (used as the title of the popup
 *      button).
 *    
 *    primary:
 *      Whether to create a primary or secondary group box.
 *    
 *    menuID:
 *      The menu ID of the menu to be displayed by the popup button.
 *    
 *    variableWidth:
 *      Whether the popup button should have a variable-width title.
 *      Fixed-width titles are only supported by Mac OS X; this
 *      parameter must be true when used with CarbonLib.
 *    
 *    titleWidth:
 *      The width in pixels of the popup button title.
 *    
 *    titleJustification:
 *      The justification of the popup button title. Use one of the
 *      TextEdit justification constants here (teFlushDefault,
 *      teCenter, teFlushRight, or teFlushLeft).
 *    
 *    titleStyle:
 *      The QuickDraw text style of the popup button title.
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
CreatePopupGroupBoxControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,
  Boolean       primary,
  MenuID        menuID,
  Boolean       variableWidth,
  SInt16        titleWidth,
  SInt16        titleJustification,
  Style         titleStyle,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* Tagged data supported by group box */
#endif  /* !__LP64__ */

enum {
  kControlGroupBoxMenuHandleTag = 'mhan', /* MenuRef (popup title only)*/
  kControlGroupBoxMenuRefTag    = 'mhan', /* MenuRef (popup title only)*/
  kControlGroupBoxFontStyleTag  = kControlFontStyleTag /* ControlFontStyleRec*/
};

/* tags available with Appearance 1.1 or later */
enum {
  kControlGroupBoxTitleRectTag  = 'trec' /* Rect. Rectangle that the title text/control is drawn in. (get only)*/
};


/*
 *  Summary:
 *    Tags available with Mac OS X 10.3 or later
 */
enum {

  /*
   * Passed data is a Rect.  Returns the full rectangle that content is
   * drawn in (get only). This is slightly different than the content
   * region, as it also includes the frame drawn around the content.
   */
  kControlGroupBoxFrameRectTag  = 'frec'
};


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ PLACARD (CDEF 14)                                                                 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Placard proc IDs */
enum {
  kControlPlacardProc           = 224
};

/* Control Kind Tag */
enum {
  kControlKindPlacard           = 'plac'
};

/* The HIObject class ID for the HIPlacardView class. */
#define kHIPlacardViewClassID           CFSTR("com.apple.HIPlacardView")
#if !__LP64__
/*
 *  CreatePlacardControl()
 *  
 *  Summary:
 *    Creates a placard control.
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
 *    outControl:
 *      On exit, contains the new control.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreatePlacardControl(
  WindowRef     window,           /* can be NULL */
  const Rect *  boundsRect,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ WINDOW HEADER (CDEF 21)                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Window Header proc IDs */
#endif  /* !__LP64__ */

enum {
  kControlWindowHeaderProc      = 336,  /* normal header*/
  kControlWindowListViewHeaderProc = 337 /* variant for list views - no bottom line*/
};

/* Control Kind Tag */
enum {
  kControlKindWindowHeader      = 'whed'
};


/*
 *  Summary:
 *    Tags available with Mac OS X 10.3 or later
 */
enum {

  /*
   * Passed data is a Boolean.  Set to true if the control is to draw
   * as a list header.
   */
  kControlWindowHeaderIsListHeaderTag = 'islh'
};

/* The HIObject class ID for the HIWindowHeaderView class. */
#define kHIWindowHeaderViewClassID      CFSTR("com.apple.HIWindowHeaderView")
/* Creation API: Carbon Only */
#if !__LP64__
/*
 *  CreateWindowHeaderControl()
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
CreateWindowHeaderControl(
  WindowRef     window,
  const Rect *  boundsRect,
  Boolean       isListHeader,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ USER PANE (CDEF 16)                                                               */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  User panes have two primary purposes: to allow easy implementation of a custom      */
/*  control by the developer, and to provide a generic container for embedding other    */
/*  controls.                                                                           */
/*                                                                                      */
/*  In Carbon, with the advent of Carbon-event-based controls, you may find it easier   */
/*  to simply write a new control from scratch than to customize a user pane control.   */
/*  The set of callbacks provided by the user pane will not be extended to support      */
/*  new Control Manager features; instead, you should just write a real control.        */
/*                                                                                      */
/*  User panes do not, by default, support embedding. If you try to embed a control     */
/*  into a user pane, you will get back errControlIsNotEmbedder. You can make a user    */
/*  pane support embedding by passing the kControlSupportsEmbedding flag in the 'value' */
/*  parameter when you create the control.                                              */
/*                                                                                      */
/*  User panes support the following overloaded control initialization options:         */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ         ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ      */
/*  Value                       Control feature flags                                   */

/* User Pane proc IDs */
#endif  /* !__LP64__ */

enum {
  kControlUserPaneProc          = 256
};

/* Control Kind Tag */
enum {
  kControlKindUserPane          = 'upan'
};

/* The HIObject class ID for the HIUserPane class. Valid in Mac OS X 10.4 and later. */
#define kHIUserPaneClassID              CFSTR("com.apple.HIUserPane")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateUserPaneControl()
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
CreateUserPaneControl(
  WindowRef     window,
  const Rect *  boundsRect,
  UInt32        features,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by user panes */
/* Currently, they are all proc ptrs for doing things like drawing and hit testing, etc. */
#endif  /* !__LP64__ */

enum {
  kControlUserItemDrawProcTag   = 'uidp', /* UserItemUPP*/
  kControlUserPaneDrawProcTag   = 'draw', /* ControlUserPaneDrawUPP*/
  kControlUserPaneHitTestProcTag = 'hitt', /* ControlUserPaneHitTestUPP*/
  kControlUserPaneTrackingProcTag = 'trak', /* ControlUserPaneTrackingUPP*/
  kControlUserPaneIdleProcTag   = 'idle', /* ControlUserPaneIdleUPP*/
  kControlUserPaneKeyDownProcTag = 'keyd', /* ControlUserPaneKeyDownUPP*/
  kControlUserPaneActivateProcTag = 'acti', /* ControlUserPaneActivateUPP*/
  kControlUserPaneFocusProcTag  = 'foci', /* ControlUserPaneFocusUPP*/
  kControlUserPaneBackgroundProcTag = 'back' /* ControlUserPaneBackgroundUPP (32-bit only)*/
};

typedef CALLBACK_API( void , ControlUserPaneDrawProcPtr )(ControlRef control, ControlPartCode part);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneHitTestProcPtr )(ControlRef control, Point where);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneTrackingProcPtr )(ControlRef control, Point startPt, ControlActionUPP actionProc);
typedef CALLBACK_API( void , ControlUserPaneIdleProcPtr )(ControlRef control);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneKeyDownProcPtr )(ControlRef control, SInt16 keyCode, SInt16 charCode, SInt16 modifiers);
typedef CALLBACK_API( void , ControlUserPaneActivateProcPtr )(ControlRef control, Boolean activating);
typedef CALLBACK_API( ControlPartCode , ControlUserPaneFocusProcPtr )(ControlRef control, ControlFocusPart action);
typedef STACK_UPP_TYPE(ControlUserPaneDrawProcPtr)              ControlUserPaneDrawUPP;
typedef STACK_UPP_TYPE(ControlUserPaneHitTestProcPtr)           ControlUserPaneHitTestUPP;
typedef STACK_UPP_TYPE(ControlUserPaneTrackingProcPtr)          ControlUserPaneTrackingUPP;
typedef STACK_UPP_TYPE(ControlUserPaneIdleProcPtr)              ControlUserPaneIdleUPP;
typedef STACK_UPP_TYPE(ControlUserPaneKeyDownProcPtr)           ControlUserPaneKeyDownUPP;
typedef STACK_UPP_TYPE(ControlUserPaneActivateProcPtr)          ControlUserPaneActivateUPP;
typedef STACK_UPP_TYPE(ControlUserPaneFocusProcPtr)             ControlUserPaneFocusUPP;
/*
 *  NewControlUserPaneDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneDrawUPP
NewControlUserPaneDrawUPP(ControlUserPaneDrawProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneHitTestUPP
NewControlUserPaneHitTestUPP(ControlUserPaneHitTestProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneTrackingUPP
NewControlUserPaneTrackingUPP(ControlUserPaneTrackingProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneIdleUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneIdleUPP
NewControlUserPaneIdleUPP(ControlUserPaneIdleProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneKeyDownUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneKeyDownUPP
NewControlUserPaneKeyDownUPP(ControlUserPaneKeyDownProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneActivateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneActivateUPP
NewControlUserPaneActivateUPP(ControlUserPaneActivateProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewControlUserPaneFocusUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneFocusUPP
NewControlUserPaneFocusUPP(ControlUserPaneFocusProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneDrawUPP(ControlUserPaneDrawUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneHitTestUPP(ControlUserPaneHitTestUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneTrackingUPP(ControlUserPaneTrackingUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneIdleUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneIdleUPP(ControlUserPaneIdleUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneKeyDownUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneKeyDownUPP(ControlUserPaneKeyDownUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneActivateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneActivateUPP(ControlUserPaneActivateUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneFocusUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneFocusUPP(ControlUserPaneFocusUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneDrawUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeControlUserPaneDrawUPP(
  ControlRef              control,
  ControlPartCode         part,
  ControlUserPaneDrawUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneHitTestUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlPartCode
InvokeControlUserPaneHitTestUPP(
  ControlRef                 control,
  Point                      where,
  ControlUserPaneHitTestUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneTrackingUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlPartCode
InvokeControlUserPaneTrackingUPP(
  ControlRef                  control,
  Point                       startPt,
  ControlActionUPP            actionProc,
  ControlUserPaneTrackingUPP  userUPP)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneIdleUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeControlUserPaneIdleUPP(
  ControlRef              control,
  ControlUserPaneIdleUPP  userUPP)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneKeyDownUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlPartCode
InvokeControlUserPaneKeyDownUPP(
  ControlRef                 control,
  SInt16                     keyCode,
  SInt16                     charCode,
  SInt16                     modifiers,
  ControlUserPaneKeyDownUPP  userUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneActivateUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeControlUserPaneActivateUPP(
  ControlRef                  control,
  Boolean                     activating,
  ControlUserPaneActivateUPP  userUPP)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneFocusUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlPartCode
InvokeControlUserPaneFocusUPP(
  ControlRef               control,
  ControlFocusPart         action,
  ControlUserPaneFocusUPP  userUPP)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline ControlUserPaneDrawUPP                               NewControlUserPaneDrawUPP(ControlUserPaneDrawProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneHitTestUPP                            NewControlUserPaneHitTestUPP(ControlUserPaneHitTestProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneTrackingUPP                           NewControlUserPaneTrackingUPP(ControlUserPaneTrackingProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneIdleUPP                               NewControlUserPaneIdleUPP(ControlUserPaneIdleProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneKeyDownUPP                            NewControlUserPaneKeyDownUPP(ControlUserPaneKeyDownProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneActivateUPP                           NewControlUserPaneActivateUPP(ControlUserPaneActivateProcPtr userRoutine) { return userRoutine; }
    inline ControlUserPaneFocusUPP                              NewControlUserPaneFocusUPP(ControlUserPaneFocusProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeControlUserPaneDrawUPP(ControlUserPaneDrawUPP) { }
    inline void                                                 DisposeControlUserPaneHitTestUPP(ControlUserPaneHitTestUPP) { }
    inline void                                                 DisposeControlUserPaneTrackingUPP(ControlUserPaneTrackingUPP) { }
    inline void                                                 DisposeControlUserPaneIdleUPP(ControlUserPaneIdleUPP) { }
    inline void                                                 DisposeControlUserPaneKeyDownUPP(ControlUserPaneKeyDownUPP) { }
    inline void                                                 DisposeControlUserPaneActivateUPP(ControlUserPaneActivateUPP) { }
    inline void                                                 DisposeControlUserPaneFocusUPP(ControlUserPaneFocusUPP) { }
    inline void                                                 InvokeControlUserPaneDrawUPP(ControlRef control, ControlPartCode part, ControlUserPaneDrawUPP userUPP) { (*userUPP)(control, part); }
    inline ControlPartCode                                      InvokeControlUserPaneHitTestUPP(ControlRef control, Point where, ControlUserPaneHitTestUPP userUPP) { return (*userUPP)(control, where); }
    inline ControlPartCode                                      InvokeControlUserPaneTrackingUPP(ControlRef control, Point startPt, ControlActionUPP actionProc, ControlUserPaneTrackingUPP userUPP) { return (*userUPP)(control, startPt, actionProc); }
    inline void                                                 InvokeControlUserPaneIdleUPP(ControlRef control, ControlUserPaneIdleUPP userUPP) { (*userUPP)(control); }
    inline ControlPartCode                                      InvokeControlUserPaneKeyDownUPP(ControlRef control, SInt16 keyCode, SInt16 charCode, SInt16 modifiers, ControlUserPaneKeyDownUPP userUPP) { return (*userUPP)(control, keyCode, charCode, modifiers); }
    inline void                                                 InvokeControlUserPaneActivateUPP(ControlRef control, Boolean activating, ControlUserPaneActivateUPP userUPP) { (*userUPP)(control, activating); }
    inline ControlPartCode                                      InvokeControlUserPaneFocusUPP(ControlRef control, ControlFocusPart action, ControlUserPaneFocusUPP userUPP) { return (*userUPP)(control, action); }
  #else
    #define NewControlUserPaneDrawUPP(userRoutine)              ((ControlUserPaneDrawUPP)userRoutine)
    #define NewControlUserPaneHitTestUPP(userRoutine)           ((ControlUserPaneHitTestUPP)userRoutine)
    #define NewControlUserPaneTrackingUPP(userRoutine)          ((ControlUserPaneTrackingUPP)userRoutine)
    #define NewControlUserPaneIdleUPP(userRoutine)              ((ControlUserPaneIdleUPP)userRoutine)
    #define NewControlUserPaneKeyDownUPP(userRoutine)           ((ControlUserPaneKeyDownUPP)userRoutine)
    #define NewControlUserPaneActivateUPP(userRoutine)          ((ControlUserPaneActivateUPP)userRoutine)
    #define NewControlUserPaneFocusUPP(userRoutine)             ((ControlUserPaneFocusUPP)userRoutine)
    #define DisposeControlUserPaneDrawUPP(userUPP)
    #define DisposeControlUserPaneHitTestUPP(userUPP)
    #define DisposeControlUserPaneTrackingUPP(userUPP)
    #define DisposeControlUserPaneIdleUPP(userUPP)
    #define DisposeControlUserPaneKeyDownUPP(userUPP)
    #define DisposeControlUserPaneActivateUPP(userUPP)
    #define DisposeControlUserPaneFocusUPP(userUPP)
    #define InvokeControlUserPaneDrawUPP(control, part, userUPP) (*userUPP)(control, part)
    #define InvokeControlUserPaneHitTestUPP(control, where, userUPP) (*userUPP)(control, where)
    #define InvokeControlUserPaneTrackingUPP(control, startPt, actionProc, userUPP) (*userUPP)(control, startPt, actionProc)
    #define InvokeControlUserPaneIdleUPP(control, userUPP)      (*userUPP)(control)
    #define InvokeControlUserPaneKeyDownUPP(control, keyCode, charCode, modifiers, userUPP) (*userUPP)(control, keyCode, charCode, modifiers)
    #define InvokeControlUserPaneActivateUPP(control, activating, userUPP) (*userUPP)(control, activating)
    #define InvokeControlUserPaneFocusUPP(control, action, userUPP) (*userUPP)(control, action)
  #endif
#endif

#if !__LP64__
typedef CALLBACK_API( void , ControlUserPaneBackgroundProcPtr )(ControlRef control, ControlBackgroundPtr info);
typedef STACK_UPP_TYPE(ControlUserPaneBackgroundProcPtr)        ControlUserPaneBackgroundUPP;
/*
 *  NewControlUserPaneBackgroundUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ControlUserPaneBackgroundUPP
NewControlUserPaneBackgroundUPP(ControlUserPaneBackgroundProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeControlUserPaneBackgroundUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeControlUserPaneBackgroundUPP(ControlUserPaneBackgroundUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeControlUserPaneBackgroundUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeControlUserPaneBackgroundUPP(
  ControlRef                    control,
  ControlBackgroundPtr          info,
  ControlUserPaneBackgroundUPP  userUPP)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline ControlUserPaneBackgroundUPP                         NewControlUserPaneBackgroundUPP(ControlUserPaneBackgroundProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeControlUserPaneBackgroundUPP(ControlUserPaneBackgroundUPP) { }
    inline void                                                 InvokeControlUserPaneBackgroundUPP(ControlRef control, ControlBackgroundPtr info, ControlUserPaneBackgroundUPP userUPP) { (*userUPP)(control, info); }
  #else
    #define NewControlUserPaneBackgroundUPP(userRoutine)        ((ControlUserPaneBackgroundUPP)userRoutine)
    #define DisposeControlUserPaneBackgroundUPP(userUPP)
    #define InvokeControlUserPaneBackgroundUPP(control, info, userUPP) (*userUPP)(control, info)
  #endif
#endif

#endif  /* !__LP64__ */


#ifdef __cplusplus
}
#endif

#endif /* __HICONTAINERVIEWS__ */

