/*
     File:       HIToolbox/HITabbedView.h
 
     Contains:   Definition of the tab view provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HITABBEDVIEW__
#define __HITABBEDVIEW__

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

#pragma pack(push, 2)


/*
 *  HITabbedView.h
 *  
 *  Discussion:
 *    API definitions for the tab view.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ TABS (CDEF 8)                                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Tabs use an auxiliary resource (tab#) to hold tab information such as the tab name  */
/*  and an icon suite ID for each tab.                                                  */
/*                                                                                      */
/*  The ID of the tab# resource that you wish to associate with a tab control should    */
/*  be passed in as the Value parameter of the control. If you are using GetNewControl, */
/*  then the Value slot in the CNTL resource should have the ID of the 'tab#' resource  */
/*  on creation.                                                                        */
/*                                                                                      */
/*  Passing zero in for the tab# resource tells the control not to read in a tab# res.  */
/*  You can then use SetControlMaximum to add tabs, followed by a call to SetControlData*/
/*  with the kControlTabInfoTag, passing in a pointer to a ControlTabInfoRec. This sets */
/*  the name and optionally an icon for a tab. Pass the 1-based tab index as the part   */
/*  code parameter to SetControlData to indicate the tab that you want to modify.       */
/*                                                                                      */
/*  Accessibility Notes: Those of you who wish to customize the accessibility           */
/*  information provided for individual tabs of a tabs control -- by handling various   */
/*  kEventClassAccessibility Carbon Events, by calling                                  */
/*  HIObjectSetAuxiliaryAccessibilityAttribute, etc. -- need to know how to interpret   */
/*  and/or build AXUIElementRefs that represent individual tabs. The AXUIElement        */
/*  representing an individual tab will/must be constructed using the tab control's     */
/*  ControlRef and the UInt64 identifier of the one-based index of the tab the element  */
/*  refers to. As usual, a UInt64 identifier of zero represents the tabs control as a   */
/*  whole. You must neither interpret nor create tab control elements whose identifiers */
/*  are greater than the count of tabs in the tabs control.                             */
/* Tabs proc IDs */
enum {
  kControlTabLargeProc          = 128,  /* Large tab size, north facing   */
  kControlTabSmallProc          = 129,  /* Small tab size, north facing   */
  kControlTabLargeNorthProc     = 128,  /* Large tab size, north facing   */
  kControlTabSmallNorthProc     = 129,  /* Small tab size, north facing   */
  kControlTabLargeSouthProc     = 130,  /* Large tab size, south facing   */
  kControlTabSmallSouthProc     = 131,  /* Small tab size, south facing   */
  kControlTabLargeEastProc      = 132,  /* Large tab size, east facing    */
  kControlTabSmallEastProc      = 133,  /* Small tab size, east facing    */
  kControlTabLargeWestProc      = 134,  /* Large tab size, west facing    */
  kControlTabSmallWestProc      = 135   /* Small tab size, west facing    */
};

/* Tab Directions */
typedef UInt16 ControlTabDirection;
enum {
  kControlTabDirectionNorth     = 0,
  kControlTabDirectionSouth     = 1,
  kControlTabDirectionEast      = 2,
  kControlTabDirectionWest      = 3
};

/* Tab Sizes */
typedef UInt16 ControlTabSize;
enum {
  kControlTabSizeLarge          = kControlSizeNormal,
  kControlTabSizeSmall          = kControlSizeSmall,
  kControlTabSizeMini           = kControlSizeMini
};

/* Control Tab Entry - used during creation                             */
/* Note that the client is responsible for allocating/providing         */
/* the ControlButtonContentInfo and string storage for this             */
/* structure.                                                           */
struct ControlTabEntry {
  ControlButtonContentInfo * icon;
  CFStringRef         name;
  Boolean             enabled;
};
typedef struct ControlTabEntry          ControlTabEntry;
/* Control Kind Tag */
enum {
  kControlKindTabs              = 'tabs'
};

/* The HIObject class ID for the HITabbedView class. */
#define kHITabbedViewClassID            CFSTR("com.apple.HITabbedView")
/* Creation API: Carbon only */
#if !__LP64__
/*
 *  CreateTabsControl()
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
CreateTabsControl(
  WindowRef                window,
  const Rect *             boundsRect,
  ControlTabSize           size,
  ControlTabDirection      direction,
  UInt16                   numTabs,
  const ControlTabEntry *  tabArray,
  ControlRef *             outControl)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Tagged data supported by the Tabs control.
 */
enum {

  /*
   * Used to get the bounds of the control area benath the tabs. Cannot
   * be used with SetControlData. Data is of type Rect.
   */
  kControlTabContentRectTag     = 'rect',

  /*
   * Used to get or set the enable state of a specific tab. The part
   * code parameter to Get/SetControlData must be a 1-based tab index.
   * Data is of type Boolean.
   */
  kControlTabEnabledFlagTag     = 'enab',

  /*
   * Used to get or set the font and style attributes of a specific
   * tab. The part code parameter to Get/SetControlData must be a
   * 1-based tab index. Data is of type ControlFontStyleRec.
   */
  kControlTabFontStyleTag       = kControlFontStyleTag,

  /*
   * Used to get or set the name and image of a specified tab. The part
   * code parameter to Get/SetControlData must be a 1-based tab index.
   * Data is of type ControlTabInfoRec. Available in Appearance Manager
   * 1.0.1 and later.
   */
  kControlTabInfoTag            = 'tabi',

  /*
   * Used to get or set the image of a specified tab. The part code
   * parameter to Get/SetControlData must be a 1-based tab index. When
   * used with GetControlData, if the tab image is an IconRef or
   * CGImageRef, the Tabs control will automatically retain the icon or
   * image, and the caller of GetControlData must release it. Data is
   * of type ControlImageContentInfo, or HIViewContentInfo. Available
   * in Mac OS X 10.1 and later.
   */
  kControlTabImageContentTag    = kControlContentTag
};

enum {
  kControlTabInfoVersionZero    = 0,    /* ControlTabInfoRec*/
  kControlTabInfoVersionOne     = 1     /* ControlTabInfoRecV1*/
};

struct ControlTabInfoRec {
  SInt16              version;                /* version of this structure.*/
  SInt16              iconSuiteID;            /* icon suite to use. Zero indicates no icon*/
  Str255              name;                   /* name to be displayed on the tab*/
};
typedef struct ControlTabInfoRec        ControlTabInfoRec;
struct ControlTabInfoRecV1 {
  SInt16              version;                /* version of this structure. == kControlTabInfoVersionOne*/
  SInt16              iconSuiteID;            /* icon suite to use. Zero indicates no icon*/
  CFStringRef         name;                   /* name to be displayed on the tab. Will be retained so caller*/
                                              /* should always release it.*/
};
typedef struct ControlTabInfoRecV1      ControlTabInfoRecV1;
/* Helper routines are available only thru the shared library/glue. */
#if !__LP64__
/*
 *  GetTabContentRect()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
GetTabContentRect(
  ControlRef   inTabControl,
  Rect *       outContentRect)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetTabEnabled()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in AppearanceLib 1.0 and later
 */
extern OSErr 
SetTabEnabled(
  ControlRef   inTabControl,
  SInt16       inTabToHilite,
  Boolean      inEnabled)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ Resource Types                                                                                    */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
#endif  /* !__LP64__ */

enum {
  kControlTabListResType        = 'tab#' /* used for tab control (Appearance 1.0 and later)*/
};


#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __HITABBEDVIEW__ */

