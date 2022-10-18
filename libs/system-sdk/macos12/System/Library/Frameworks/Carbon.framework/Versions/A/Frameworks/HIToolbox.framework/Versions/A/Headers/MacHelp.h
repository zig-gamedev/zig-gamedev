/*
     File:       HIToolbox/MacHelp.h
 
     Contains:   Carbon Help Manager Interfaces.
 
     Copyright:  © 1998-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __MACHELP__
#define __MACHELP__

#ifndef __CONTROLS__
#include <HIToolbox/Controls.h>
#endif

#ifndef __DIALOGS__
#include <HIToolbox/Dialogs.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif

#ifndef __MACWINDOWS__
#include <HIToolbox/MacWindows.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
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

#pragma pack(push, 2)

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Help Manager constants, etc.                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kMacHelpVersion               = 0x0003
};

enum {
  kHMHelpMenuID                 = -16490 /* Resource ID and menu ID of help menu */
};

typedef SInt16 HMContentRequest;
enum {
  kHMSupplyContent              = 0,
  kHMDisposeContent             = 1
};


/*
 *  HMContentType
 *  
 *  Summary:
 *    These constants should be used in the contentType field of the
 *    HMHelpContent structure.
 */
typedef UInt32 HMContentType;
enum {

  /*
   * No help content is provided.
   */
  kHMNoContent                  = 'none',

  /*
   * Help content is a CFStringRef in the HMHelpContent.u.tagCFString
   * field. 
   * 
   * The string is automatically retained by the Help Manager when
   * provided to the HMSetControl/Window/MenuItemHelpContent APIs, and
   * automatically released when the control/window/menu is destroyed.
   * When this type of help content is provided by a
   * control/window/menu title/menu item help content proc, then the
   * string is not automatically released, but the content proc will be
   * called again later with the kHMDisposeContent message to allow
   * your application to release the string, if appropriate.
   */
  kHMCFStringContent            = 'cfst',

  /*
   * Help content is a localized string loaded automatically by the
   * Help Manager using CFCopyLocalizedString. The key that identifies
   * the localized string should be placed in the
   * HMHelpContent.u.tagCFString field. 
   * 
   * The string is automatically retained by the Help Manager when
   * provided to the HMSetControl/Window/MenuItemHelpContent APIs, and
   * automatically released when the control/window/menu is destroyed.
   * When this type of help content is provided by a
   * control/window/menu title/menu item help content proc, then the
   * string is not automatically released, but the content proc will be
   * called again later with the kHMDisposeContent message to allow
   * your application to release the string, if appropriate.
   */
  kHMCFStringLocalizedContent   = 'cfsl',

  /*
   * Help content is a Pascal string in the application's text encoding
   * in the HMHelpContent.u.tagString field.
   */
  kHMPascalStrContent           = 'pstr',

  /*
   * Help content is text in the application's text encoding loaded
   * from a 'TEXT' resource. The 'TEXT' resource ID should be placed in
   * the HMHelpContent.u.tagTextRes field. This help content type is
   * available in Mac OS X 10.2 and later.
   */
  kHMTextResContent             = 'text',

  /*
   * Help content is a TEHandle in the HMHelpContent.u.tagTEHandle.
   * Note that Mac OS X ignores all style information contained in the
   * TEHandle and only uses the text. This help content type is
   * available in Mac OS X 10.2 and later. It is not supported in
   * 64-bit mode. 
   * 
   * When this type of help content is provided to the
   * HMSetControl/Window/MenuItemHelpContent APIs, the TEHandle is
   * _not_ released when the control/window/menu is destroyed. When
   * this type of help content is provided by a control/window/menu
   * title/menu item help content proc, then the content proc will be
   * called again later with the kHMDisposeContent message to allow
   * your application to release the TEHandle, if appropriate.
   */
  kHMTEHandleContent            = 'txth',

  /*
   * Help content is a Pascal string in the application's text encoding
   * loaded from a 'STR#' resource. The 'STR#' resource ID and string
   * index should be placed in the HMHelpContent.u.tagStringRes field.
   * This help content type is not supported in 64-bit mode.
   */
  kHMStringResContent           = 'str#',

  /*
   * Help content is a Pascal string in the application's text encoding
   * loaded from a 'STR ' resource. The 'STR ' resource ID should be
   * placed in the HMHelpContent.u.tagStrRes field. This help content
   * type is not supported in 64-bit mode.
   */
  kHMStrResContent              = 'str '
};


/*
 *  HMTagDisplaySide
 *  
 *  Discussion:
 *    Help tag display locations relative to absolute hot rect
 */
typedef SInt16 HMTagDisplaySide;
enum {

  /*
   * System default location
   */
  kHMDefaultSide                = 0,

  /*
   * Above, aligned with left or right depending on system script
   */
  kHMOutsideTopScriptAligned    = 1,

  /*
   * To the left, centered vertically
   */
  kHMOutsideLeftCenterAligned   = 2,

  /*
   * Below, aligned with left or right depending on system script
   */
  kHMOutsideBottomScriptAligned = 3,

  /*
   * To the right, centered vertically
   */
  kHMOutsideRightCenterAligned  = 4,

  /*
   * Above, aligned with left
   */
  kHMOutsideTopLeftAligned      = 5,

  /*
   * Above, aligned with right
   */
  kHMOutsideTopRightAligned     = 6,

  /*
   * To the left, aligned with top
   */
  kHMOutsideLeftTopAligned      = 7,

  /*
   * To the left, aligned with bottom
   */
  kHMOutsideLeftBottomAligned   = 8,

  /*
   * Below, aligned with left
   */
  kHMOutsideBottomLeftAligned   = 9,

  /*
   * Below, aligned with right
   */
  kHMOutsideBottomRightAligned  = 10,

  /*
   * To the right, aligned with top
   */
  kHMOutsideRightTopAligned     = 11,

  /*
   * To the right, aligned with bottom
   */
  kHMOutsideRightBottomAligned  = 12,

  /*
   * Above, centered horizontally
   */
  kHMOutsideTopCenterAligned    = 13,

  /*
   * Below, centered horizontally
   */
  kHMOutsideBottomCenterAligned = 14,

  /*
   * Inside, aligned with right, centered vertically
   */
  kHMInsideRightCenterAligned   = 15,

  /*
   * Inside, aligned with left, centered vertically
   */
  kHMInsideLeftCenterAligned    = 16,

  /*
   * Inside, aligned with bottom, centered horizontally
   */
  kHMInsideBottomCenterAligned  = 17,

  /*
   * Inside, aligned with top, centered horizontally
   */
  kHMInsideTopCenterAligned     = 18,

  /*
   * Inside, aligned with top and left
   */
  kHMInsideTopLeftCorner        = 19,

  /*
   * Inside, aligned with top and right
   */
  kHMInsideTopRightCorner       = 20,

  /*
   * Inside, aligned with bottom and left
   */
  kHMInsideBottomLeftCorner     = 21,

  /*
   * Inside, aligned with bottom and right
   */
  kHMInsideBottomRightCorner    = 22,

  /*
   * Centered vertically and horizontally
   */
  kHMAbsoluteCenterAligned      = 23
};

/* Obsoleted constants HMTagDisplaySides, use the new ones, please */
enum {
  kHMTopSide                    = kHMOutsideTopScriptAligned,
  kHMLeftSide                   = kHMOutsideLeftCenterAligned,
  kHMBottomSide                 = kHMOutsideBottomScriptAligned,
  kHMRightSide                  = kHMOutsideRightCenterAligned,
  kHMTopLeftCorner              = kHMOutsideTopLeftAligned,
  kHMTopRightCorner             = kHMOutsideTopRightAligned,
  kHMLeftTopCorner              = kHMOutsideLeftTopAligned,
  kHMLeftBottomCorner           = kHMOutsideLeftBottomAligned,
  kHMBottomLeftCorner           = kHMOutsideBottomLeftAligned,
  kHMBottomRightCorner          = kHMOutsideBottomRightAligned,
  kHMRightTopCorner             = kHMOutsideRightTopAligned,
  kHMRightBottomCorner          = kHMOutsideRightBottomAligned
};

typedef SInt16 HMContentProvidedType;
enum {
  kHMContentProvided            = 0,
  kHMContentNotProvided         = 1,
  kHMContentNotProvidedDontPropagate = 2
};

enum {
  kHMMinimumContentIndex        = 0,    /* first entry in HMHelpContentRec.content is the minimum content */
  kHMMaximumContentIndex        = 1     /* second entry in HMHelpContentRec.content is the maximum content */
};

enum {
  errHMIllegalContentForMinimumState = -10980, /* unrecognized content type for minimum content */
  errHMIllegalContentForMaximumState = -10981 /* unrecognized content type for maximum content */
};

/* obsolete names; will be removed*/
enum {
  kHMIllegalContentForMinimumState = errHMIllegalContentForMinimumState
};

enum {
  kHelpTagEventHandlerTag       = 'hevt'
};

struct HMStringResType {
  short               hmmResID;
  short               hmmIndex;
};
typedef struct HMStringResType          HMStringResType;
struct HMHelpContent {
  HMContentType       contentType;
  union {
    CFStringRef         tagCFString;          /* CFStringRef*/
    Str255              tagString;            /* Pascal String*/
    HMStringResType     tagStringRes;         /* STR# resource ID and index*/
    TEHandle            tagTEHandle;          /* TextEdit handle (NOT SUPPORTED ON MAC OS X)*/
    SInt16              tagTextRes;           /* TEXT/styl resource ID (NOT SUPPORTED ON MAC OS X)*/
    SInt16              tagStrRes;            /* STR resource ID*/
  }                       u;
};
typedef struct HMHelpContent            HMHelpContent;
struct HMHelpContentRec {
  SInt32              version;
  Rect                absHotRect;
  HMTagDisplaySide    tagSide;
  HMHelpContent       content[2];
};
typedef struct HMHelpContentRec         HMHelpContentRec;
typedef HMHelpContentRec *              HMHelpContentPtr;
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Callback procs                                       */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
typedef CALLBACK_API( OSStatus , HMControlContentProcPtr )(ControlRef inControl, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentRec *ioHelpContent);
typedef CALLBACK_API( OSStatus , HMWindowContentProcPtr )(WindowRef inWindow, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentRec *ioHelpContent);
typedef CALLBACK_API( OSStatus , HMMenuTitleContentProcPtr )(MenuRef inMenu, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentRec *ioHelpContent);
typedef CALLBACK_API( OSStatus , HMMenuItemContentProcPtr )(const MenuTrackingData *inTrackingData, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentRec *ioHelpContent);
typedef STACK_UPP_TYPE(HMControlContentProcPtr)                 HMControlContentUPP;
typedef STACK_UPP_TYPE(HMWindowContentProcPtr)                  HMWindowContentUPP;
typedef STACK_UPP_TYPE(HMMenuTitleContentProcPtr)               HMMenuTitleContentUPP;
typedef STACK_UPP_TYPE(HMMenuItemContentProcPtr)                HMMenuItemContentUPP;
/*
 *  NewHMControlContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern HMControlContentUPP
NewHMControlContentUPP(HMControlContentProcPtr userRoutine)   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewHMWindowContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern HMWindowContentUPP
NewHMWindowContentUPP(HMWindowContentProcPtr userRoutine)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewHMMenuTitleContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern HMMenuTitleContentUPP
NewHMMenuTitleContentUPP(HMMenuTitleContentProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewHMMenuItemContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern HMMenuItemContentUPP
NewHMMenuItemContentUPP(HMMenuItemContentProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeHMControlContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeHMControlContentUPP(HMControlContentUPP userUPP)       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeHMWindowContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeHMWindowContentUPP(HMWindowContentUPP userUPP)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeHMMenuTitleContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeHMMenuTitleContentUPP(HMMenuTitleContentUPP userUPP)   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeHMMenuItemContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeHMMenuItemContentUPP(HMMenuItemContentUPP userUPP)     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeHMControlContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeHMControlContentUPP(
  ControlRef               inControl,
  Point                    inGlobalMouse,
  HMContentRequest         inRequest,
  HMContentProvidedType *  outContentProvided,
  HMHelpContentRec *       ioHelpContent,
  HMControlContentUPP      userUPP)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeHMWindowContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeHMWindowContentUPP(
  WindowRef                inWindow,
  Point                    inGlobalMouse,
  HMContentRequest         inRequest,
  HMContentProvidedType *  outContentProvided,
  HMHelpContentRec *       ioHelpContent,
  HMWindowContentUPP       userUPP)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeHMMenuTitleContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeHMMenuTitleContentUPP(
  MenuRef                  inMenu,
  HMContentRequest         inRequest,
  HMContentProvidedType *  outContentProvided,
  HMHelpContentRec *       ioHelpContent,
  HMMenuTitleContentUPP    userUPP)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeHMMenuItemContentUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSStatus
InvokeHMMenuItemContentUPP(
  const MenuTrackingData *  inTrackingData,
  HMContentRequest          inRequest,
  HMContentProvidedType *   outContentProvided,
  HMHelpContentRec *        ioHelpContent,
  HMMenuItemContentUPP      userUPP)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline HMControlContentUPP                                  NewHMControlContentUPP(HMControlContentProcPtr userRoutine) { return userRoutine; }
    inline HMWindowContentUPP                                   NewHMWindowContentUPP(HMWindowContentProcPtr userRoutine) { return userRoutine; }
    inline HMMenuTitleContentUPP                                NewHMMenuTitleContentUPP(HMMenuTitleContentProcPtr userRoutine) { return userRoutine; }
    inline HMMenuItemContentUPP                                 NewHMMenuItemContentUPP(HMMenuItemContentProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeHMControlContentUPP(HMControlContentUPP) { }
    inline void                                                 DisposeHMWindowContentUPP(HMWindowContentUPP) { }
    inline void                                                 DisposeHMMenuTitleContentUPP(HMMenuTitleContentUPP) { }
    inline void                                                 DisposeHMMenuItemContentUPP(HMMenuItemContentUPP) { }
    inline OSStatus                                             InvokeHMControlContentUPP(ControlRef inControl, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType * outContentProvided, HMHelpContentRec * ioHelpContent, HMControlContentUPP userUPP) { return (*userUPP)(inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent); }
    inline OSStatus                                             InvokeHMWindowContentUPP(WindowRef inWindow, Point inGlobalMouse, HMContentRequest inRequest, HMContentProvidedType * outContentProvided, HMHelpContentRec * ioHelpContent, HMWindowContentUPP userUPP) { return (*userUPP)(inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent); }
    inline OSStatus                                             InvokeHMMenuTitleContentUPP(MenuRef inMenu, HMContentRequest inRequest, HMContentProvidedType * outContentProvided, HMHelpContentRec * ioHelpContent, HMMenuTitleContentUPP userUPP) { return (*userUPP)(inMenu, inRequest, outContentProvided, ioHelpContent); }
    inline OSStatus                                             InvokeHMMenuItemContentUPP(const MenuTrackingData * inTrackingData, HMContentRequest inRequest, HMContentProvidedType * outContentProvided, HMHelpContentRec * ioHelpContent, HMMenuItemContentUPP userUPP) { return (*userUPP)(inTrackingData, inRequest, outContentProvided, ioHelpContent); }
  #else
    #define NewHMControlContentUPP(userRoutine)                 ((HMControlContentUPP)userRoutine)
    #define NewHMWindowContentUPP(userRoutine)                  ((HMWindowContentUPP)userRoutine)
    #define NewHMMenuTitleContentUPP(userRoutine)               ((HMMenuTitleContentUPP)userRoutine)
    #define NewHMMenuItemContentUPP(userRoutine)                ((HMMenuItemContentUPP)userRoutine)
    #define DisposeHMControlContentUPP(userUPP)
    #define DisposeHMWindowContentUPP(userUPP)
    #define DisposeHMMenuTitleContentUPP(userUPP)
    #define DisposeHMMenuItemContentUPP(userUPP)
    #define InvokeHMControlContentUPP(inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userUPP) (*userUPP)(inControl, inGlobalMouse, inRequest, outContentProvided, ioHelpContent)
    #define InvokeHMWindowContentUPP(inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent, userUPP) (*userUPP)(inWindow, inGlobalMouse, inRequest, outContentProvided, ioHelpContent)
    #define InvokeHMMenuTitleContentUPP(inMenu, inRequest, outContentProvided, ioHelpContent, userUPP) (*userUPP)(inMenu, inRequest, outContentProvided, ioHelpContent)
    #define InvokeHMMenuItemContentUPP(inTrackingData, inRequest, outContentProvided, ioHelpContent, userUPP) (*userUPP)(inTrackingData, inRequest, outContentProvided, ioHelpContent)
  #endif
#endif

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* API                                                                                      */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Help Menu */
#if !__LP64__
/*
 *  HMGetHelpMenu()
 *  
 *  Summary:
 *    Returns a menu to which applications may add their own help items.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    outHelpMenu:
 *      On exit, contains the help menu.
 *    
 *    outFirstCustomItemIndex:
 *      On exit, contains the menu item index that will be used by the
 *      first item added by the application. This parameter may be NULL.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMGetHelpMenu(
  MenuRef *        outHelpMenu,
  MenuItemIndex *  outFirstCustomItemIndex)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Installing/Retrieving Content */
/* Menu title and item help tags are not supported by CarbonLib. They are fully supported on Mac OS X. */
/* Pass NULL for the inContent parameter of these APIs to remove help content from a control, window, or menu. */
/*
 *  HMSetControlHelpContent()
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
HMSetControlHelpContent(
  ControlRef                inControl,
  const HMHelpContentRec *  inContent)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetControlHelpContent()
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
HMGetControlHelpContent(
  ControlRef          inControl,
  HMHelpContentRec *  outContent)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMSetWindowHelpContent()
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
HMSetWindowHelpContent(
  WindowRef                 inWindow,
  const HMHelpContentRec *  inContent)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetWindowHelpContent()
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
HMGetWindowHelpContent(
  WindowRef           inWindow,
  HMHelpContentRec *  outContent)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMSetMenuItemHelpContent()
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
HMSetMenuItemHelpContent(
  MenuRef                   inMenu,
  MenuItemIndex             inItem,
  const HMHelpContentRec *  inContent)       /* can be NULL */ AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetMenuItemHelpContent()
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
HMGetMenuItemHelpContent(
  MenuRef             inMenu,
  MenuItemIndex       inItem,
  HMHelpContentRec *  outContent)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Installing/Retrieving Content Callbacks */
/*
 *  HMInstallControlContentCallback()
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
HMInstallControlContentCallback(
  ControlRef            inControl,
  HMControlContentUPP   inContentUPP)                         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMInstallWindowContentCallback()
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
HMInstallWindowContentCallback(
  WindowRef            inWindow,
  HMWindowContentUPP   inContentUPP)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMInstallMenuTitleContentCallback()
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
HMInstallMenuTitleContentCallback(
  MenuRef                 inMenu,
  HMMenuTitleContentUPP   inContentUPP)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMInstallMenuItemContentCallback()
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
HMInstallMenuItemContentCallback(
  MenuRef                inMenu,
  HMMenuItemContentUPP   inContentUPP)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetControlContentCallback()
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
HMGetControlContentCallback(
  ControlRef             inControl,
  HMControlContentUPP *  outContentUPP)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetWindowContentCallback()
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
HMGetWindowContentCallback(
  WindowRef             inWindow,
  HMWindowContentUPP *  outContentUPP)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetMenuTitleContentCallback()
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
HMGetMenuTitleContentCallback(
  MenuRef                  inMenu,
  HMMenuTitleContentUPP *  outContentUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetMenuItemContentCallback()
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
HMGetMenuItemContentCallback(
  MenuRef                 inMenu,
  HMMenuItemContentUPP *  outContentUPP)                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Enabling and Disabling Help Tags */
/*
 *  HMAreHelpTagsDisplayed()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
HMAreHelpTagsDisplayed(void)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMSetHelpTagsDisplayed()
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
HMSetHelpTagsDisplayed(Boolean inDisplayTags)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMSetTagDelay()
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
HMSetTagDelay(Duration inDelay)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMGetTagDelay()
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
HMGetTagDelay(Duration * outDelay)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Displaying tags */
/*
 *  HMDisplayTag()
 *  
 *  Summary:
 *    Displays a help tag at a user-defined location.
 *  
 *  Discussion:
 *    Prior to Mac OS X 10.4, HMDisplayTag does not retain the help
 *    content that is passed to it, nor release it when the tag is
 *    closed. Your application must ensure that the help content
 *    remains valid as long as the tag may be visible (which
 *    effectively means that your application should never dispose of
 *    help content that is passed to HMDisplayTag). In Mac OS X 10.4
 *    and later, HMDisplayTag makes a copy of the content and releases
 *    the copy when the tag closes, so you can release the content
 *    after HMDisplayTag returns.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inContent:
 *      HMHelpContentRec describing the help tag to be displayed.
 *  
 *  Result:
 *    An OSStatus code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMDisplayTag(const HMHelpContentRec * inContent)              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HMHideTag()
 *  
 *  Summary:
 *    HMHideTag hides the currently visible help tag.  If there is no
 *    current help tag, this call does nothing.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Result:
 *    An OSStatus code indicating success or failure.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.2 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMHideTag(void)                                               AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;


#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Values for the inOptions parameter to HMHideTagWithOptions.
 */
enum {

  /*
   * Causes the tag to fade out when hidden. If this flag is not
   * specified, the tag is hidden without fading.
   */
  kHMHideTagFade                = 1 << 0,

  /*
   * Causes the tag to begin hiding immediately. If this flag is not
   * specified, the tag is hidden after a short delay (currently 0.75
   * second).
   */
  kHMHideTagImmediately         = 1 << 1
};

#if !__LP64__
/*
 *  HMHideTagWithOptions()
 *  
 *  Summary:
 *    Hides the current help tag, with various options to control how
 *    the tag is hidden.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inOptions:
 *      Options for how the tag should be hidden. kHMHideTagFade and
 *      kHMHideTagImmediately are the only available options.
 *  
 *  Result:
 *    An operating system result code. noErr is returned if there is no
 *    tag currently visible.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.4 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        not available in CarbonLib 1.x
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMHideTagWithOptions(OptionBits inOptions)                    AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;


/*--------------------------------------------------------------------------------------*/
/*  ¥ DEPRECATED                                                                        */
/*                                                                                      */
/*  All functions below this point are either deprecated (they continue to function     */
/*  but are not the most modern nor most efficient solution to a problem), or they are  */
/*  completely unavailable on Mac OS X.                                                 */
/*--------------------------------------------------------------------------------------*/
/*
 *  HMSetMenuHelpFromBalloonRsrc()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Though this API is exported from CarbonLib and Mac OS X, it is
 *    completely non-functional.
 *  
 *  Summary:
 *    Not really implemented.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMSetMenuHelpFromBalloonRsrc(
  MenuRef   inMenu,
  SInt16    inHmnuRsrcID)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  HMSetDialogHelpFromBalloonRsrc()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Though this API is exported from CarbonLib and Mac OS X, it is
 *    completely non-functional.
 *  
 *  Summary:
 *    Not really implemented.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
HMSetDialogHelpFromBalloonRsrc(
  DialogRef   inDialog,
  SInt16      inHdlgRsrcID,
  SInt16      inItemStart)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;




#endif  /* !__LP64__ */


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __MACHELP__ */

