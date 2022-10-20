/*
     File:       HIToolbox/HIButtonViews.h
 
     Contains:   Definitions of the button views provided by HIToolbox.
 
     Copyright:  © 2006-2008 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __HIBUTTONVIEWS__
#define __HIBUTTONVIEWS__

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
 *  HIButtonViews.h
 *  
 *  Discussion:
 *    API definitions for the simple button views provided by
 *    HIToolbox: pushbutton, checkbox, radio button and radio group,
 *    bevel button, and round button.
 */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*   PUSH BUTTON (CDEF 23)                                                              */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  Two new variants of the standard pushbutton have been added to the standard control */
/*  suite that draw a color icon next to the control title. One variant draws the icon  */
/*  on the left side, the other draws it on the right side (when the system justifica-  */
/*  tion is right to left, these are reversed).                                         */
/*                                                                                      */
/*  When either of the icon pushbuttons are created, the contrlMax field of the control */
/*  record is used to determine the ID of the 'cicn' resource drawn in the pushbutton.  */
/*                                                                                      */
/*  In addition, a push button can now be told to draw with a default outline using the */
/*  SetControlData routine with the kControlPushButtonDefaultTag below.                 */
/*                                                                                      */
/*  A push button may also be marked using the kControlPushButtonCancelTag. This has    */
/*  no visible representation, but does cause the button to play the CancelButton theme */
/*  sound instead of the regular pushbutton theme sound when pressed.                   */
/*                                                                                      */
/* Push Button proc IDs */
enum {
  kControlPushButtonProc        = 368,
  kControlPushButLeftIconProc   = 374,  /* Standard pushbutton with left-side icon*/
  kControlPushButRightIconProc  = 375   /* Standard pushbutton with right-side icon*/
};

/* Push Button Icon Alignments */
typedef UInt16 ControlPushButtonIconAlignment;
enum {
  kControlPushButtonIconOnLeft  = 6,
  kControlPushButtonIconOnRight = 7
};

/* Control Kind Tag */
enum {
  kControlKindPushButton        = 'push',
  kControlKindPushIconButton    = 'picn'
};

/* The HIObject class ID for the HIPushButton class. */
#define kHIPushButtonClassID            CFSTR("com.apple.HIPushButton")
/* Creation APIs: Carbon Only */
#if !__LP64__
/*
 *  CreatePushButtonControl()
 *  
 *  Summary:
 *    Creates a push button control.
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
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    title:
 *      The control title. May be NULL.
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
CreatePushButtonControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,            /* can be NULL */
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CreatePushButtonWithIconControl()
 *  
 *  Summary:
 *    Creates a push button control containing an icon or other
 *    graphical content.
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
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    title:
 *      The control title. May be NULL.
 *    
 *    icon:
 *      The control graphic content.
 *    
 *    iconAlignment:
 *      The alignment of the control graphic content.
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
CreatePushButtonWithIconControl(
  WindowRef                        window,
  const Rect *                     boundsRect,
  CFStringRef                      title,               /* can be NULL */
  ControlButtonContentInfo *       icon,
  ControlPushButtonIconAlignment   iconAlignment,
  ControlRef *                     outControl)                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */


/*
 *  Summary:
 *    Tagged data supported by standard buttons
 */
enum {

  /*
   * Data is a Boolean indicating if a push button is a default button.
   * If so, the button will draw with the appropriate appearance.
   * Available in Mac OS 8.5 and later.
   */
  kControlPushButtonDefaultTag  = 'dflt',

  /*
   * Data is a Boolean indicating if a push button is a cancel button.
   * If so, the button will draw with an appropriate appearance.
   * Available in Mac OS 8.5 and later.
   */
  kControlPushButtonCancelTag   = 'cncl',

  /*
   * Data is a ControlButtonContentInfo or HIViewContentInfo structure.
   * May be used to set or retrieve a button's image content. Available
   * in Mac OS X 10.4 and later. In Mac OS X 10.4, the push button
   * supports the kControlContentCIconRes and kControlContentCGImageRef
   * content types. In Mac OS X 10.5 and later, the push button also
   * supports IconRef content.
   */
  kControlPushButtonContentTag  = kControlContentTag,

  /*
   * Data is a ControlPushButtonIconAlignment indicating the desired
   * alignment for the button's image content. Applies to all types of
   * image content that may be associated with the push button, not
   * just icons. Available in Mac OS X 10.4 and later.
   */
  kControlPushButtonIconAlignmentTag = 'cpia',

  /*
	 * Data is a Boolean. On 10.10 and later, getting this value always
	 * returns false, and setting this value is ignored.  Available in Mac
	 * OS X 10.5 and later.
   */
  kControlPushButtonAnimatingTag = 'anim'
};


/*
 *  Summary:
 *    Tagged data supported by push buttons
 *  
 *  Discussion:
 *    This new tag is available on Mac OS X 10.4 and later. The
 *    constant is not in the Mac OS X 10.4 and Mac OS X 10.5 headers,
 *    but the constant value is functional on both releases.
 */
enum {

  /*
   * Data is a Boolean indicating if a push button is a textured push
   * button, to be drawn on a textured window. Textured was previously
   * referred to as "metal". This attribute is only to be set on push
   * buttons being used in composited windows.
   */
  kControlPushButtonIsTexturedTag = 'metl'
};

/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*   CHECKBOX (CDEF 23)                                                             */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  The standard checkbox view supports a "mixed" value that indicates that the         */
/*  current setting contains a mixed set of on and off values. The control value used   */
/*  to display this indication is defined below:                                        */
/*                                                                                      */
/*      kControlCheckBoxMixedValue = 2                                                  */
/*                                                                                      */
/* Check Box proc ID */
enum {
  kControlCheckBoxProc          = 369
};

/* Variants with Appearance 1.1 or later */
enum {
  kControlCheckBoxAutoToggleProc = 371
};

/* Control Kind Tag */
enum {
  kControlKindCheckBox          = 'cbox'
};

/* The HIObject class ID for the HICheckBox class. */
#define kHICheckBoxClassID              CFSTR("com.apple.HICheckBox")
#if !__LP64__
/*
 *  CreateCheckBoxControl()
 *  
 *  Summary:
 *    Creates a checkbox control.
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
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    title:
 *      The control title. May be NULL.
 *    
 *    initialValue:
 *      The initial value of the control. Should be zero (off), one
 *      (on), or two (mixed). The control is automatically given a
 *      minimum value of zero and a maximum value of two.
 *    
 *    autoToggle:
 *      Whether this control should have auto-toggle behavior. If true,
 *      the control will automatically toggle between on and off states
 *      when clicked.
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
CreateCheckBoxControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,              /* can be NULL */
  SInt32        initialValue,
  Boolean       autoToggle,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/* Check Box Values */
#endif  /* !__LP64__ */

enum {
  kControlCheckBoxUncheckedValue = 0,
  kControlCheckBoxCheckedValue  = 1,
  kControlCheckBoxMixedValue    = 2
};


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*   RADIO BUTTON (CDEF 23)                                                         */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  The standard radio button control supports a "mixed" value that indicates that the  */
/*  current setting contains a mixed set of on and off values. The control value used   */
/*  to display this indication is defined below:                                        */
/*                                                                                      */
/*      kControlRadioButtonMixedValue = 2                                               */
/*                                                                                      */
/* Radio Button proc IDs */
enum {
  kControlRadioButtonProc       = 370
};

/* Variants with Appearance 1.1 or later */
enum {
  kControlRadioButtonAutoToggleProc = 372
};

/* Radio Button Values */
enum {
  kControlRadioButtonUncheckedValue = 0,
  kControlRadioButtonCheckedValue = 1,
  kControlRadioButtonMixedValue = 2
};

/* Control Kind Tag */
enum {
  kControlKindRadioButton       = 'rdio'
};

/* The HIObject class ID for the HIRadioButton class. */
#define kHIRadioButtonClassID           CFSTR("com.apple.HIRadioButton")
#if !__LP64__
/*
 *  CreateRadioButtonControl()
 *  
 *  Summary:
 *    Creates a radio button control.
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
 *      The bounds of the control, in local coordinates of the window.
 *    
 *    title:
 *      The control title. May be NULL.
 *    
 *    initialValue:
 *      The initial value of the control. Should be zero (off), one
 *      (on), or two (mixed). The control is automatically given a
 *      minimum value of zero and a maximum value of two.
 *    
 *    autoToggle:
 *      Whether this control should have auto-toggle behavior. If true,
 *      the control will automatically toggle between on and off states
 *      when clicked. This parameter should be false if the control
 *      will be embedded into a radio group control; in that case, the
 *      radio group will handle setting the correct control value in
 *      response to a click.
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
CreateRadioButtonControl(
  WindowRef     window,
  const Rect *  boundsRect,
  CFStringRef   title,              /* can be NULL */
  SInt32        initialValue,
  Boolean       autoToggle,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*   RADIO GROUP (CDEF 26)                                                              */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*  This control implements a radio group. It is an embedding control and can therefore */
/*  only be used when a control hierarchy is established for its owning window. You     */
/*  should only embed controls that have radio button behavior - currently radio buttons*/
/*  and bevel buttons. Specifically, you can embed controls that return the feature bit */
/*  kControlHasRadioBehavior. As controls are embedded into it, the group sets up its   */
/*  value, min, and max to represent the number of embedded items.                      */
/*  The current value of the control is the index of the sub-control that is the current*/
/*  'on' radio button. To get the current radio button control handle, you can use the  */
/*  control manager call GetIndexedSubControl, passing in the value of the radio group. */
/*                                                                                      */
/*  Note that when creating radio buttons for use in a radio group control, you should  */
/*  not use the autoToggle version of the radio button. The radio group control will    */
/*  handling toggling the radio button values itself; auto-toggle radio buttons do not  */
/*  work properly in a radio group control on Mac OS 9.                                 */
/*                                                                                      */
/*  NOTE: This control is only available with Appearance 1.0.1.                         */
/* Radio Group Proc ID */
#endif  /* !__LP64__ */

enum {
  kControlRadioGroupProc        = 416
};

/* Control Kind Tag */
enum {
  kControlKindRadioGroup        = 'rgrp'
};

/* The HIObject class ID for the HIRadioGroup class. */
#define kHIRadioGroupClassID            CFSTR("com.apple.HIRadioGroup")
/* Creation API: Carbon Only */
#if !__LP64__
/*
 *  CreateRadioGroupControl()
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
CreateRadioGroupControl(
  WindowRef     window,
  const Rect *  boundsRect,
  ControlRef *  outControl)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
     BEVEL BUTTON INTERFACE (CDEF 2)
  
    Bevel buttons allow you to control the content type (pict/icon/etc.), the behavior
   (pushbutton/toggle/sticky), and the bevel size. You also have the option of
    attaching a menu to it. When a menu is present, you can specify which way the
    popup arrow is facing (down or right).

    This is all made possible by overloading the Min, Max, and Value parameters for the
    control, as well as adjusting the variant. Here's the breakdown of what goes where:

    Parameter                   What Goes Here
                
    Min                         Hi Byte = Behavior, Lo Byte = content type.
    Max                         ResID for resource-based content types.
    Value                       MenuID to attach, 0 = no menu, please.

    The variant is broken down into two halves. The low 2 bits control the bevel type.
    Bit 2 controls the popup arrow direction (if a menu is present) and bit 3 controls
    whether or not to use the control's owning window's font.

    Constants for all you need to put this together are below. The values for behaviors
    are set up so that you can simply add them to the content type for use in the Min
    parameter of NewControl.

    An example call:

    control = NewControl( window, &bounds, "\p", true, 0, kControlContentIconSuiteRes +
                            kBehaviorToggles, myIconSuiteID, bevelButtonSmallBevelProc,
                            0L );

    Attaching a menu:

    control = NewControl( window, &bounds, "\p", true, kMyMenuID,
            kControlContentIconSuiteRes, myIconSuiteID, bevelButtonSmallBevelProc +
            kBevelButtonMenuOnRight, 0L );

    This will attach menu ID kMyMenuID to the button, with the popup arrow facing right.
    This also puts the menu up to the right of the button. You can also specify that a
    menu can have multiple items checked at once by adding kBehaviorMultiValueMenus
    into the Min parameter. If you do use multivalue menus, the GetBevelButtonMenuValue
    helper function will return the last item chosen from the menu, whether or not it
    was checked.

    NOTE:   Bevel buttons with menus actually have *two* values. The value of the
            button (on/off), and the value of the menu. The menu value can be gotten
            with the GetBevelButtonMenuValue helper function.

    Handle-based Content
    
    You can create your control and then set the content to an existing handle to an
    icon suite, etc. using the macros below. Please keep in mind that resource-based
    content is owned by the control, handle-based content is owned by you. The CDEF will
    not try to dispose of handle-based content. If you are changing the content type of
    the button on the fly, you must make sure that if you are replacing a handle-
    based content with a resource-based content to properly dispose of the handle,
    else a memory leak will ensue.

    Textual Content
    
    Please note that if a bevel button gets its textual content from the title
    of the control. To alter the textual content of a bevel button, use the
    SetControlTitle[WithCFString] API.

    Implicit Menu Arrow Sizing
    
    Bevel buttons can now have implicit popup menu arrow sizes on Mac OS X 10.5 and later. Use
    SetControlData(..., kControlSizeTag, ...), where the default control size is
    kControlSizeAuto. kControlSizeAuto has the bevel button render its popup menu arrow
    at a size that is dependent on the size of the bevel button -- this is the behavior
    on Tiger and earlier. kControlSizeNormal has it render the normal size arrow and
    kControlSizeSmall has it render the arrow small. All other sizes are invalid.
*/
/* Bevel Button Proc IDs */
#endif  /* !__LP64__ */

enum {
  kControlBevelButtonSmallBevelProc = 32,
  kControlBevelButtonNormalBevelProc = 33,
  kControlBevelButtonLargeBevelProc = 34
};

/* Add these variant codes to kBevelButtonSmallBevelProc to change the type of button */
enum {
  kControlBevelButtonSmallBevelVariant = 0,
  kControlBevelButtonNormalBevelVariant = (1 << 0),
  kControlBevelButtonLargeBevelVariant = (1 << 1),
  kControlBevelButtonMenuOnRightVariant = (1 << 2)
};

/* Bevel Thicknesses */
typedef UInt16 ControlBevelThickness;
enum {
  kControlBevelButtonSmallBevel = 0,
  kControlBevelButtonNormalBevel = 1,
  kControlBevelButtonLargeBevel = 2
};

/* Behaviors of bevel buttons. These are set up so you can add  */
/* them together with the content types for use in the Min      */
/* parameter of NewControl. Note that the behavior of a bevel   */
/* button cannot be changed after the button is created.        */
enum {
  kControlBehaviorPushbutton    = 0,
  kControlBehaviorToggles       = 0x0100,
  kControlBehaviorSticky        = 0x0200,
  kControlBehaviorOffsetContents = 0x8000,
  kControlBehaviorSingleValueMenu = 0,
  kControlBehaviorMultiValueMenu = 0x4000 /* only makes sense when a menu is attached.*/
};

/* Behaviors for 1.0.1 or later */
enum {
  kControlBehaviorCommandMenu   = 0x2000 /* menu holds commands, not choices. Overrides multi-value bit.*/
};

typedef UInt16                          ControlBevelButtonBehavior;
typedef UInt16                          ControlBevelButtonMenuBehavior;
/* Bevel Button Menu Placements */
typedef UInt16 ControlBevelButtonMenuPlacement;
enum {
  kControlBevelButtonMenuOnBottom = 0,
  kControlBevelButtonMenuOnRight = (1 << 2)
};

/* Control Kind Tag */
enum {
  kControlKindBevelButton       = 'bevl'
};

/* The HIObject class ID for the HIBevelButton class. */
#define kHIBevelButtonClassID           CFSTR("com.apple.HIBevelButton")
/* Creation API: Carbon Only */
#if !__LP64__
/*
 *  CreateBevelButtonControl()
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
CreateBevelButtonControl(
  WindowRef                         window,
  const Rect *                      boundsRect,
  CFStringRef                       title,
  ControlBevelThickness             thickness,
  ControlBevelButtonBehavior        behavior,
  ControlButtonContentInfoPtr       info,
  MenuID                            menuID,
  ControlBevelButtonMenuBehavior    menuBehavior,
  ControlBevelButtonMenuPlacement   menuPlacement,
  ControlRef *                      outControl)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;




/* Graphic Alignments */
#endif  /* !__LP64__ */

typedef SInt16 ControlButtonGraphicAlignment;
enum {
  kControlBevelButtonAlignSysDirection = -1, /* only left or right*/
  kControlBevelButtonAlignCenter = 0,
  kControlBevelButtonAlignLeft  = 1,
  kControlBevelButtonAlignRight = 2,
  kControlBevelButtonAlignTop   = 3,
  kControlBevelButtonAlignBottom = 4,
  kControlBevelButtonAlignTopLeft = 5,
  kControlBevelButtonAlignBottomLeft = 6,
  kControlBevelButtonAlignTopRight = 7,
  kControlBevelButtonAlignBottomRight = 8
};

/* Text Alignments */
typedef SInt16 ControlButtonTextAlignment;
enum {
  kControlBevelButtonAlignTextSysDirection = teFlushDefault,
  kControlBevelButtonAlignTextCenter = teCenter,
  kControlBevelButtonAlignTextFlushRight = teFlushRight,
  kControlBevelButtonAlignTextFlushLeft = teFlushLeft
};

/* Text Placements */
typedef SInt16 ControlButtonTextPlacement;
enum {
  kControlBevelButtonPlaceSysDirection = -1, /* if graphic on right, then on left*/
  kControlBevelButtonPlaceNormally = 0,
  kControlBevelButtonPlaceToRightOfGraphic = 1,
  kControlBevelButtonPlaceToLeftOfGraphic = 2,
  kControlBevelButtonPlaceBelowGraphic = 3,
  kControlBevelButtonPlaceAboveGraphic = 4
};


/* Data tags supported by the bevel button controls */
enum {
  kControlBevelButtonContentTag = kControlContentTag, /* ControlImageContentInfo*/
  kControlBevelButtonTransformTag = 'tran', /* IconTransformType*/
  kControlBevelButtonTextAlignTag = 'tali', /* ButtonTextAlignment*/
  kControlBevelButtonTextOffsetTag = 'toff', /* SInt16*/
  kControlBevelButtonGraphicAlignTag = 'gali', /* ButtonGraphicAlignment*/
  kControlBevelButtonGraphicOffsetTag = 'goff', /* Point*/
  kControlBevelButtonTextPlaceTag = 'tplc', /* ButtonTextPlacement*/
  kControlBevelButtonMenuValueTag = 'mval', /* SInt16*/
  kControlBevelButtonMenuHandleTag = 'mhnd', /* MenuRef*/
  kControlBevelButtonMenuRefTag = 'mhnd', /* MenuRef*/
  kControlBevelButtonCenterPopupGlyphTag = 'pglc' /* Boolean: true = center, false = bottom right*/
};

/* These are tags in 1.0.1 or later */
enum {
  kControlBevelButtonLastMenuTag = 'lmnu', /* SInt16: menuID of last menu item selected from*/
  kControlBevelButtonMenuDelayTag = 'mdly' /* SInt32: ticks to delay before menu appears*/
};

/* tags available with Appearance 1.1 or later */
enum {
                                        /* Boolean: True = if an icon of the ideal size for*/
                                        /* the button isn't available, scale a larger or*/
                                        /* smaller icon to the ideal size. False = don't*/
                                        /* scale; draw a smaller icon or clip a larger icon.*/
                                        /* Default is false. Only applies to IconSuites and*/
  kControlBevelButtonScaleIconTag = 'scal' /* IconRefs.*/
};

/* tags available in Mac OS X and later */
enum {
  kControlBevelButtonOwnedMenuRefTag = 'omrf', /* MenuRef (control will dispose)*/
  kControlBevelButtonKindTag    = 'bebk' /* ThemeButtonKind ( kTheme[Small,Medium,Large,Rounded]BevelButton )*/
};


/*
 *  Summary:
 *    Tags available with Mac OS X 10.3 or later
 */
enum {

  /*
   * Passed data is an Boolean.  Gets or sets whether or not the
   * associated menu is a multi-value menu or not.  True means that the
   * menu can have multiple selections.
   */
  kControlBevelButtonIsMultiValueMenuTag = 'mult'
};

/* Helper routines are available only thru the shared library/glue. */
#if !__LP64__
/*
 *  GetBevelButtonMenuValue()
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
GetBevelButtonMenuValue(
  ControlRef       inButton,
  MenuItemIndex *  outValue)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonMenuValue()
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
SetBevelButtonMenuValue(
  ControlRef      inButton,
  MenuItemIndex   inValue)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  GetBevelButtonMenuHandle()
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
GetBevelButtonMenuHandle(
  ControlRef    inButton,
  MenuHandle *  outHandle)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#define GetBevelButtonMenuRef GetBevelButtonMenuHandle
/*
 *  GetBevelButtonContentInfo()
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
GetBevelButtonContentInfo(
  ControlRef                    inButton,
  ControlButtonContentInfoPtr   outContent)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonContentInfo()
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
SetBevelButtonContentInfo(
  ControlRef                    inButton,
  ControlButtonContentInfoPtr   inContent)                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonTransform()
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
SetBevelButtonTransform(
  ControlRef          inButton,
  IconTransformType   transform)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonGraphicAlignment()
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
SetBevelButtonGraphicAlignment(
  ControlRef                      inButton,
  ControlButtonGraphicAlignment   inAlign,
  SInt16                          inHOffset,
  SInt16                          inVOffset)                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonTextAlignment()
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
SetBevelButtonTextAlignment(
  ControlRef                   inButton,
  ControlButtonTextAlignment   inAlign,
  SInt16                       inHOffset)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SetBevelButtonTextPlacement()
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
SetBevelButtonTextPlacement(
  ControlRef                   inButton,
  ControlButtonTextPlacement   inWhere)                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*   ROUND BUTTON                                                                       */
/*  (CDEF 31)                                                                           */
/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
#endif  /* !__LP64__ */


/*
 *  ControlRoundButtonSize
 *  
 *  Discussion:
 *    Button Sizes
 */
typedef SInt16 ControlRoundButtonSize;
enum {

  /*
   * A 20 pixel diameter button.
   */
  kControlRoundButtonNormalSize = kControlSizeNormal,

  /*
   * A 25 pixel diameter button.
   */
  kControlRoundButtonLargeSize  = kControlSizeLarge
};

/* Data tags supported by the round button controls */
enum {
  kControlRoundButtonContentTag = kControlContentTag, /* ControlImageContentInfo*/
  kControlRoundButtonSizeTag    = kControlSizeTag /* ControlRoundButtonSize*/
};

/* Control Kind Tag */
enum {
  kControlKindRoundButton       = 'rndb'
};

/* The HIObject class ID for the HIRoundButton class. */
#define kHIRoundButtonClassID           CFSTR("com.apple.HIRoundButton")
#if !__LP64__
/*
 *  CreateRoundButtonControl()
 *  
 *  Summary:
 *    Creates a new instance of the Round Button Control.
 *  
 *  Discussion:
 *    CreateRoundButtonControl is preferred over NewControl because it
 *    allows you to specify the exact set of parameters required to
 *    create the control without overloading parameter semantics.
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Parameters:
 *    
 *    inWindow:
 *      The WindowRef in which to create the control. May be NULL in
 *      10.3 and later.
 *    
 *    inBoundsRect:
 *      The bounding rectangle for the control. The height and width of
 *      the control is fixed (specified by the ControlRoundButtonSize
 *      parameter) and the control will be centered within the
 *      rectangle you specify.
 *    
 *    inSize:
 *      The button size; either kControlRoundButtonNormalSize or
 *      kControlRoundButtonLargeSize.
 *    
 *    inContent:
 *      Any optional content displayed in the button. Currently only
 *      kControlContentIconRef is supported. May be NULL.
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
CreateRoundButtonControl(
  WindowRef                   inWindow,           /* can be NULL */
  const Rect *                inBoundsRect,
  ControlRoundButtonSize      inSize,
  ControlButtonContentInfo *  inContent,          /* can be NULL */
  ControlRef *                outControl)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#endif  /* !__LP64__ */

#if OLDROUTINENAMES
enum {
  kControlCheckboxUncheckedValue = kControlCheckBoxUncheckedValue,
  kControlCheckboxCheckedValue  = kControlCheckBoxCheckedValue,
  kControlCheckboxMixedValue    = kControlCheckBoxMixedValue
};

#endif  /* OLDROUTINENAMES */


#ifdef __cplusplus
}
#endif

#endif /* __HIBUTTONVIEWS__ */

