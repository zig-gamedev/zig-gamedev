/*
     File:       HIToolbox/ControlDefinitions.h
 
     Contains:   Definitions of controls provided by the Control Manager
 
     Copyright:  © 1999-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONTROLDEFINITIONS__
#define __CONTROLDEFINITIONS__

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

#ifndef __LISTS__
#include <HIToolbox/Lists.h>
#endif

#ifndef __MACHELP__
#include <HIToolbox/MacHelp.h>
#endif

#ifndef __MENUS__
#include <HIToolbox/Menus.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ControlDefinitions.h
 *  
 *  Discussion:
 *    System software supplies a variety of controls for your
 *    applications to use. They are described herein.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ Control Definition IDÕs                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/* Standard System 7 procIDs*/

enum {
  pushButProc                   = 0,
  checkBoxProc                  = 1,
  radioButProc                  = 2,
  scrollBarProc                 = 16,
  popupMenuProc                 = 1008
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ Control Part Codes                                                                */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  kControlLabelPart             = 1,
  kControlMenuPart              = 2,
  kControlTrianglePart          = 4,
  kControlEditTextPart          = 5,    /* Appearance 1.0 and later*/
  kControlPicturePart           = 6,    /* Appearance 1.0 and later*/
  kControlIconPart              = 7,    /* Appearance 1.0 and later*/
  kControlClockPart             = 8,    /* Appearance 1.0 and later*/
  kControlListBoxPart           = 24,   /* Appearance 1.0 and later*/
  kControlListBoxDoubleClickPart = 25,  /* Appearance 1.0 and later*/
  kControlImageWellPart         = 26,   /* Appearance 1.0 and later*/
  kControlRadioGroupPart        = 27,   /* Appearance 1.0.2 and later*/
  kControlButtonPart            = 10,
  kControlCheckBoxPart          = 11,
  kControlRadioButtonPart       = 11,
  kControlUpButtonPart          = kAppearancePartUpButton,
  kControlDownButtonPart        = kAppearancePartDownButton,
  kControlPageUpPart            = kAppearancePartPageUpArea,
  kControlPageDownPart          = kAppearancePartPageDownArea,
  kControlClockHourDayPart      = 9,    /* Appearance 1.1 and later*/
  kControlClockMinuteMonthPart  = 10,   /* Appearance 1.1 and later*/
  kControlClockSecondYearPart   = 11,   /* Appearance 1.1 and later*/
  kControlClockAMPMPart         = 12,   /* Appearance 1.1 and later*/
  kControlDataBrowserPart       = 24,   /* CarbonLib 1.0 and later*/
  kControlDataBrowserDraggedPart = 25   /* CarbonLib 1.0 and later*/
};

/*--------------------------------------------------------------------------------------*/
/*  ¥ DEPRECATED                                                                        */
/*                                                                                      */
/*  All functions below this point are either deprecated (they continue to function     */
/*  but are not the most modern nor most efficient solution to a problem), or they are  */
/*  completely unavailable on Mac OS X.                                                 */
/*--------------------------------------------------------------------------------------*/
/*
  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
    ¥ EDIT TEXT (CDEF 17)
  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
    Use the EditUnicodeText control to replace the Edit Text control. It uses the same
    data tags as the Edit Text control.
*/

/* Edit Text proc IDs */
enum {
  kControlEditTextProc          = 272,
  kControlEditTextPasswordProc  = 274
};

/* proc IDs available with Appearance 1.1 or later */
enum {
  kControlEditTextInlineInputProc = 276 /* Can't combine with the other variants*/
};

/* Control Kind Tag */
enum {
  kControlKindEditText          = 'etxt'
};

#if !__LP64__
/*
 *  CreateEditTextControl()   *** DEPRECATED ***
 *  
 *  Deprecated:
 *    Use CreateEditUnicodeTextControl API instead.
 *  
 *  Summary:
 *    Creates a new edit text control.
 *  
 *  Discussion:
 *    This control is a legacy control. It is deprecated in favor of
 *    the EditUnicodeText control, which handles Unicode and draws its
 *    text using antialiasing.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    window:
 *      The window in which the control should be placed. May be NULL
 *      in 10.3 and later.
 *    
 *    boundsRect:
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    text:
 *      The text of the control. May be NULL.
 *    
 *    isPassword:
 *      A Boolean indicating whether the field is to be used as a
 *      password field. Passing false indicates that the field is to
 *      display entered text normally. True means that the field will
 *      be used as a password field and any text typed into the field
 *      will be displayed only as bullets.
 *    
 *    useInlineInput:
 *      A Boolean indicating whether or not the control is to accept
 *      inline input. Pass true to to accept inline input, otherwise
 *      pass false.
 *    
 *    style:
 *      The control's font style, size, color, and so on. May be NULL.
 *    
 *    outControl:
 *      On exit, contains the new control (if noErr is returned as the
 *      result code).
 *  
 *  Result:
 *    An operating system result code.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only] but deprecated in 10.4
 *    CarbonLib:        in CarbonLib 1.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
CreateEditTextControl(
  WindowRef                    window,
  const Rect *                 boundsRect,
  CFStringRef                  text,                 /* can be NULL */
  Boolean                      isPassword,
  Boolean                      useInlineInput,
  const ControlFontStyleRec *  style,                /* can be NULL */
  ControlRef *                 outControl)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_4;


/* ControlData tags supported only by the classic EditText control*/
#endif  /* !__LP64__ */

enum {
  kControlEditTextTEHandleTag   = 'than', /* The TEHandle of the text edit record*/
  kControlEditTextInlinePreUpdateProcTag = 'prup', /* TSMTEPreUpdateUPP and TSMTEPostUpdateUpp. For use with inline input variant...*/
  kControlEditTextInlinePostUpdateProcTag = 'poup' /* ...The refCon parameter will contain the ControlRef.*/
};

/*
    The classic EditText control also supports these tags defined for the EditUnicodeText control:
    
        kControlEditTextLockedTag
        kControlEditTextStyleTag
        kControlEditTextFixedTextTag
        kControlEditTextTextTag
        kControlEditTextKeyFilterTag
        kControlEditTextValidationProcTag
        kControlEditTextSelectionTag
        kControlEditTextKeyScriptBehaviorTag
        kControlEditTextCFStringTag
        kControlEditTextPasswordTag
        kControlEditTextPasswordCFStringTag
*/

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ PICTURE CONTROL (CDEF 19)                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Value parameter should contain the ID of the picture you wish to display when       */
/*  creating controls of this type. If you don't want the control tracked at all, use   */
/*  the 'no track' variant.                                                             */
/* Picture control proc IDs */
enum {
  kControlPictureProc           = 304,
  kControlPictureNoTrackProc    = 305   /* immediately returns kControlPicturePart*/
};

/* Control Kind Tag */
enum {
  kControlKindPicture           = 'pict'
};

/* The HIObject class ID for the HIPictureView class. */
#define kHIPictureViewClassID           CFSTR("com.apple.HIPictureView")
#if !__LP64__
/*
 *  CreatePictureControl()
 *  
 *  Summary:
 *    Creates a picture control.
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
 *    content:
 *      The descriptor for the picture you want the control to display.
 *    
 *    dontTrack:
 *      A Boolean value indicating whether the control should hilite
 *      when it is clicked on. False means hilite and track the mouse.
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
CreatePictureControl(
  WindowRef                         window,           /* can be NULL */
  const Rect *                      boundsRect,
  const ControlButtonContentInfo *  content,
  Boolean                           dontTrack,
  ControlRef *                      outControl)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by picture controls */
#endif  /* !__LP64__ */

enum {
  kControlPictureHandleTag      = 'pich' /* PicHandle*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ LIST BOX (CDEF 22)                                                                */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Lists use an auxiliary resource to define their format. The resource type used is   */
/*  'ldes' and a definition for it can be found in Appearance.r. The resource ID for    */
/*  the ldes is passed in the 'value' parameter when creating the control. You may pass */
/*  zero in value. This tells the List Box control to not use a resource. The list will */
/*  be created with default values, and will use the standard LDEF (0). You can change  */
/*  the list by getting the list handle. You can set the LDEF to use by using the tag   */
/*  below (kControlListBoxLDEFTag)                                                      */
/* List Box proc IDs */
enum {
  kControlListBoxProc           = 352,
  kControlListBoxAutoSizeProc   = 353
};

/* Control Kind Tag */
enum {
  kControlKindListBox           = 'lbox'
};

#if !__LP64__
/*
 *  CreateListBoxControl()
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
CreateListBoxControl(
  WindowRef            window,
  const Rect *         boundsRect,
  Boolean              autoSize,
  SInt16               numRows,
  SInt16               numColumns,
  Boolean              horizScroll,
  Boolean              vertScroll,
  SInt16               cellHeight,
  SInt16               cellWidth,
  Boolean              hasGrowSpace,
  const ListDefSpec *  listDef,
  ControlRef *         outControl)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by list box */
#endif  /* !__LP64__ */

enum {
  kControlListBoxListHandleTag  = 'lhan', /* ListHandle*/
  kControlListBoxKeyFilterTag   = kControlKeyFilterTag, /* ControlKeyFilterUPP*/
  kControlListBoxFontStyleTag   = kControlFontStyleTag /* ControlFontStyleRec*/
};

/* New tags in 1.0.1 or later */
enum {
  kControlListBoxDoubleClickTag = 'dblc', /* Boolean. Was last click a double-click?*/
  kControlListBoxLDEFTag        = 'ldef' /* SInt16. ID of LDEF to use.*/
};

/* Resource Types */
enum {
  kControlListDescResType       = 'ldes' /* used for list box control (Appearance 1.0 and later)*/
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ SCROLL TEXT BOX (CDEF 27)                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control implements a scrolling box of (non-editable) text. This is useful for  */
/*  credits in about boxes, etc.                                                        */
/*  The standard version of this control has a scroll bar, but the autoscrolling        */
/*  variant does not. The autoscrolling variant needs two pieces of information to      */
/*  work: delay (in ticks) before the scrolling starts, and time (in ticks) between     */
/*  scrolls. It will scroll one pixel at a time, unless changed via SetControlData.     */
/*                                                                                      */
/*  Parameter                   What Goes Here                                          */
/*  ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ         ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ    */
/*  Value                       Resource ID of 'TEXT'/'styl' content.                   */
/*  Min                         Scroll start delay (in ticks)                       .   */
/*  Max                         Delay (in ticks) between scrolls.                       */
/*                                                                                      */
/*  NOTE: This control is only available with Appearance 1.1.                           */
/* Scroll Text Box Proc IDs */
enum {
  kControlScrollTextBoxProc     = 432,
  kControlScrollTextBoxAutoScrollProc = 433
};

/* Control Kind Tag */
enum {
  kControlKindScrollingTextBox  = 'stbx'
};

#if !__LP64__
/*
 *  CreateScrollingTextBoxControl()
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
CreateScrollingTextBoxControl(
  WindowRef     window,
  const Rect *  boundsRect,
  SInt16        contentResID,
  Boolean       autoScroll,
  UInt32        delayBeforeAutoScroll,
  UInt32        delayBetweenAutoScroll,
  UInt16        autoScrollAmount,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/* Tagged data supported by Scroll Text Box */
#endif  /* !__LP64__ */

enum {
  kControlScrollTextBoxDelayBeforeAutoScrollTag = 'stdl', /* UInt32 (ticks until autoscrolling starts)*/
  kControlScrollTextBoxDelayBetweenAutoScrollTag = 'scdl', /* UInt32 (ticks between scrolls)*/
  kControlScrollTextBoxAutoScrollAmountTag = 'samt', /* UInt16 (pixels per scroll) -- defaults to 1*/
  kControlScrollTextBoxContentsTag = 'tres', /* SInt16 (resource ID of 'TEXT'/'styl') -- write only!*/
  kControlScrollTextBoxAnimatingTag = 'anim' /* Boolean (whether the text box should auto-scroll)*/
};

#if OLDROUTINENAMES
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  ¥ OLDROUTINENAMES                                                                   */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
enum {
  inLabel                       = kControlLabelPart,
  inMenu                        = kControlMenuPart,
  inTriangle                    = kControlTrianglePart,
  inButton                      = kControlButtonPart,
  inCheckBox                    = kControlCheckBoxPart,
  inUpButton                    = kControlUpButtonPart,
  inDownButton                  = kControlDownButtonPart,
  inPageUp                      = kControlPageUpPart,
  inPageDown                    = kControlPageDownPart
};

enum {
  kInLabelControlPart           = kControlLabelPart,
  kInMenuControlPart            = kControlMenuPart,
  kInTriangleControlPart        = kControlTrianglePart,
  kInButtonControlPart          = kControlButtonPart,
  kInCheckBoxControlPart        = kControlCheckBoxPart,
  kInUpButtonControlPart        = kControlUpButtonPart,
  kInDownButtonControlPart      = kControlDownButtonPart,
  kInPageUpControlPart          = kControlPageUpPart,
  kInPageDownControlPart        = kControlPageDownPart
};


#endif  /* OLDROUTINENAMES */


#ifdef __cplusplus
}
#endif

#endif /* __CONTROLDEFINITIONS__ */

