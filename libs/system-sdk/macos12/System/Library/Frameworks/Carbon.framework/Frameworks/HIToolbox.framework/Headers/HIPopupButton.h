/*
     File:       HIToolbox/HIPopupButton.h
 
     Contains:   Definitions of the popup button and popup arrow views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIPOPUPBUTTON__
#define __HIPOPUPBUTTON__

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
 *  HIPopupButton.h
 *  
 *  Discussion:
 *    API definitions for the popup button and popup arrow views.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ POPUP BUTTON (CDEF 25)                                                            */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This is the new Appearance Popup Button. It takes the same variants and does the    */
/*  same overloading as the previous popup menu control. There are some differences:    */
/*                                                                                      */
/*  Passing in a menu ID of -12345 causes the popup not to try and get the menu from a  */
/*  resource. Instead, you can build the menu and later stuff the MenuRef field in      */
/*  the popup data information.                                                         */
/*                                                                                      */
/*  You can pass -1 in the Max parameter to have the control calculate the width of the */
/*  title on its own instead of guessing and then tweaking to get it right. It adds the */
/*  appropriate amount of space between the title and the popup.                        */
/*                                                                                      */
/* Theme Popup Button proc IDs */
enum {
  kControlPopupButtonProc       = 400,
  kControlPopupFixedWidthVariant = 1 << 0,
  kControlPopupVariableWidthVariant = 1 << 1,
  kControlPopupUseAddResMenuVariant = 1 << 2,
  kControlPopupUseWFontVariant  = kControlUsesOwningWindowsFontVariant
};

/* Control Kind Tag */
enum {
  kControlKindPopupButton       = 'popb'
};

/* The HIObject class ID for the HIPopupButton class. */
#define kHIPopupButtonClassID           CFSTR("com.apple.HIPopupButton")
#if !__LP64__
/*
 *  CreatePopupButtonControl()
 *  
 *  Summary:
 *    Creates a popup button control.
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
 *    title:
 *      The title of the control.
 *    
 *    menuID:
 *      The ID of a menu that should be used by the control. A menu
 *      with this ID should be inserted into the menubar with
 *      InsertMenu(menu, kInsertHierarchicalMenu). You can also pass
 *      -12345 to have the control delay its acquisition of a menu; in
 *      this case, you can build the menu and later provide it to the
 *      control with SetControlData and kControlPopupButtonMenuRefTag
 *      or kControlPopupButtonOwnedMenuRefTag.
 *    
 *    variableWidth:
 *      Whether the width of the control is allowed to vary according
 *      to the width of the selected menu item text, or should remain
 *      fixed to the original control bounds width.
 *    
 *    titleWidth:
 *      The width of the title.
 *    
 *    titleJustification:
 *      The justification of the title.
 *    
 *    titleStyle:
 *      A QuickDraw style bitfield indicating the font style of the
 *      title.
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
CreatePopupButtonControl(
  WindowRef     window,                   /* can be NULL */
  const Rect *  boundsRect,
  CFStringRef   title,
  MenuID        menuID,
  Boolean       variableWidth,
  SInt16        titleWidth,
  SInt16        titleJustification,
  Style         titleStyle,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* These tags are available in 1.0.1 or later of Appearance */
#endif  /* !__LP64__ */

enum {
  kControlPopupButtonMenuHandleTag = 'mhan', /* MenuRef*/
  kControlPopupButtonMenuRefTag = 'mhan', /* MenuRef*/
  kControlPopupButtonMenuIDTag  = 'mnid' /* SInt16*/
};

/* These tags are available in 1.1 or later of Appearance */
enum {
  kControlPopupButtonExtraHeightTag = 'exht', /* SInt16 - extra vertical whitespace within the button*/
  kControlPopupButtonOwnedMenuRefTag = 'omrf' /* MenuRef*/
};

/* These tags are available in Mac OS X */
enum {
  kControlPopupButtonCheckCurrentTag = 'chck' /* Boolean    - whether the popup puts a checkmark next to the current item (defaults to true)*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ POPUP ARROW (CDEF 12)                                                             */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  The popup arrow CDEF is used to draw the small arrow normally associated with a     */
/*  popup control. The arrow can point in four directions, and a small or large version */
/*  can be used. This control is provided to allow clients to draw the arrow in a       */
/*  normalized fashion which will take advantage of themes automatically.               */
/*                                                                                      */
/* Popup Arrow proc IDs */
enum {
  kControlPopupArrowEastProc    = 192,
  kControlPopupArrowWestProc    = 193,
  kControlPopupArrowNorthProc   = 194,
  kControlPopupArrowSouthProc   = 195,
  kControlPopupArrowSmallEastProc = 196,
  kControlPopupArrowSmallWestProc = 197,
  kControlPopupArrowSmallNorthProc = 198,
  kControlPopupArrowSmallSouthProc = 199
};

/* Popup Arrow Orientations */
enum {
  kControlPopupArrowOrientationEast = 0,
  kControlPopupArrowOrientationWest = 1,
  kControlPopupArrowOrientationNorth = 2,
  kControlPopupArrowOrientationSouth = 3
};

typedef UInt16                          ControlPopupArrowOrientation;
/* Popup Arrow Size */
enum {
  kControlPopupArrowSizeNormal  = 0,
  kControlPopupArrowSizeSmall   = 1
};

typedef UInt16                          ControlPopupArrowSize;
/* Control Kind Tag */
enum {
  kControlKindPopupArrow        = 'parr'
};

/* The HIObject class ID for the HIPopupArrow class. */
#define kHIPopupArrowClassID            CFSTR("com.apple.hipopuparrow")
#if !__LP64__
/*
 *  CreatePopupArrowControl()
 *  
 *  Summary:
 *    Creates a popup arrow control.
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
 *    orientation:
 *      The orientation of the control.
 *    
 *    size:
 *      The size of the control.
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
CreatePopupArrowControl(
  WindowRef                      window,            /* can be NULL */
  const Rect *                   boundsRect,
  ControlPopupArrowOrientation   orientation,
  ControlPopupArrowSize          size,
  ControlRef *                   outControl)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*--------------------------------------------------------------------------------------*/
/*  ¥ DEPRECATED                                                                        */
/*                                                                                      */
/*  All declarations below this point are either deprecated (they continue to function  */
/*  but are not the most modern nor most efficient solution to a problem), or they are  */
/*  completely unavailable on Mac OS X.                                                 */
/*--------------------------------------------------------------------------------------*/
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ Pop-Up Menu Control Constants                                                     */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Variant codes for the System 7 pop-up menu*/
#endif  /* !__LP64__ */

enum {
  popupFixedWidth               = 1 << 0,
  popupVariableWidth            = 1 << 1,
  popupUseAddResMenu            = 1 << 2,
  popupUseWFont                 = 1 << 3
};

/* Menu label styles for the System 7 pop-up menu*/
enum {
  popupTitleBold                = 1 << 8,
  popupTitleItalic              = 1 << 9,
  popupTitleUnderline           = 1 << 10,
  popupTitleOutline             = 1 << 11,
  popupTitleShadow              = 1 << 12,
  popupTitleCondense            = 1 << 13,
  popupTitleExtend              = 1 << 14,
  popupTitleNoStyle             = 1 << 15
};

/* Menu label justifications for the System 7 pop-up menu*/
enum {
  popupTitleLeftJust            = 0x00000000,
  popupTitleCenterJust          = 0x00000001,
  popupTitleRightJust           = 0x000000FF
};



#ifdef __cplusplus
}
#endif

#endif /* __HIPOPUPBUTTON__ */

