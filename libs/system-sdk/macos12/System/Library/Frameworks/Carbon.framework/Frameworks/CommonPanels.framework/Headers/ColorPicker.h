/*
     File:       CommonPanels/ColorPicker.h
 
     Contains:   Color Picker package Interfaces.
 
     Version:    CommonPanels-94~674
 
     Copyright:  © 1987-2008 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __COLORPICKER__
#define __COLORPICKER__

#ifndef __APPLICATIONSERVICES__
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifndef __EVENTS__
#include <HIToolbox/Events.h>
#endif



#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#if !__LP64__
enum {
                                        /*Maximum small fract value, as long*/
  kMaximumSmallFract            = 0x0000FFFF
};

#endif  /* !__LP64__ */

/* These are legacy constants. The Color Picker on OS X uses the Cocoa NSColorPanel. */
enum {
  kDefaultColorPickerWidth      = 383,
  kDefaultColorPickerHeight     = 238
};

typedef SInt16 DialogPlacementSpec;
enum {
  kAtSpecifiedOrigin            = 0,
  kDeepestColorScreen           = 1,
  kCenterOnMainScreen           = 2
};

/* Since OS X uses the Cocoa NSColorPanel, the flags below are no longer used. */
enum {
  kColorPickerDialogIsMoveable  = 1,
  kColorPickerDialogIsModal     = 2,
  kColorPickerCanModifyPalette  = 4,
  kColorPickerCanAnimatePalette = 8,
  kColorPickerAppIsColorSyncAware = 16,
  kColorPickerInSystemDialog    = 32,
  kColorPickerInApplicationDialog = 64,
  kColorPickerInPickerDialog    = 128,
  kColorPickerDetachedFromChoices = 256,
  kColorPickerCallColorProcLive = 512
};

#if OLDROUTINENAMES
#if !__LP64__
enum {
                                        /*Maximum small fract value, as long*/
  MaxSmallFract                 = 0x0000FFFF
};

#endif  /* !__LP64__ */

enum {
  kDefaultWidth                 = 383,
  kDefaultHeight                = 238
};

/* Since OS X uses the Cocoa NSColorPanel, the flags below are no longer used. */
enum {
  DialogIsMoveable              = 1,
  DialogIsModal                 = 2,
  CanModifyPalette              = 4,
  CanAnimatePalette             = 8,
  AppIsColorSyncAware           = 16,
  InSystemDialog                = 32,
  InApplicationDialog           = 64,
  InPickerDialog                = 128,
  DetachedFromChoices           = 256,
  CallColorProcLive             = 512
};

#endif  /* OLDROUTINENAMES */

#if !__LP64__
/* A SmallFract value is just the fractional part of a Fixed number,
which is the low order word.  They can be
assigned directly to and from INTEGERs. */
/* Unsigned fraction between 0 and 1 */
typedef unsigned short                  SmallFract;
struct HSVColor {
  SmallFract          hue;                    /* Fraction of circle, red at 0 */
  SmallFract          saturation;             /* 0-1, 0 for gray, 1 for pure color */
  SmallFract          value;                  /* 0-1, 0 for black, 1 for max intensity */
};
typedef struct HSVColor                 HSVColor;
struct HSLColor {
  SmallFract          hue;                    /* Fraction of circle, red at 0 */
  SmallFract          saturation;             /* 0-1, 0 for gray, 1 for pure color */
  SmallFract          lightness;              /* 0-1, 0 for black, 1 for white */
};
typedef struct HSLColor                 HSLColor;
struct CMYColor {
  SmallFract          cyan;
  SmallFract          magenta;
  SmallFract          yellow;
};
typedef struct CMYColor                 CMYColor;
struct PMColor {
  CMProfileHandle     profile;
  CMColor             color;
};
typedef struct PMColor                  PMColor;
typedef PMColor *                       PMColorPtr;
#endif  /* !__LP64__ */

struct NPMColor {
  CMProfileRef        profile;
  CMColor             color;
};
typedef struct NPMColor                 NPMColor;
typedef NPMColor *                      NPMColorPtr;
typedef struct OpaquePicker*            Picker;
typedef Picker picker;
/* Since OS X uses the Cocoa NSColorPanel, the struct below is no longer used. */
struct PickerMenuItemInfo {
  SInt16              editMenuID;
  SInt16              cutItem;
  SInt16              copyItem;
  SInt16              pasteItem;
  SInt16              clearItem;
  SInt16              undoItem;
};
typedef struct PickerMenuItemInfo       PickerMenuItemInfo;
/* The following proc ptr is the only supported way to communicate with the Cocoa NSColorPanel on OS X. */
typedef CALLBACK_API( void , NColorChangedProcPtr )(SRefCon userData, NPMColor *newColor);
typedef STACK_UPP_TYPE(NColorChangedProcPtr)                    NColorChangedUPP;
/*
 *  NewNColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern NColorChangedUPP
NewNColorChangedUPP(NColorChangedProcPtr userRoutine)         AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeNColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeNColorChangedUPP(NColorChangedUPP userUPP)             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeNColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeNColorChangedUPP(
  SRefCon           userData,
  NPMColor *        newColor,
  NColorChangedUPP  userUPP)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline NColorChangedUPP                                     NewNColorChangedUPP(NColorChangedProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeNColorChangedUPP(NColorChangedUPP) { }
    inline void                                                 InvokeNColorChangedUPP(SRefCon userData, NPMColor * newColor, NColorChangedUPP userUPP) { (*userUPP)(userData, newColor); }
  #else
    #define NewNColorChangedUPP(userRoutine)                    ((NColorChangedUPP)userRoutine)
    #define DisposeNColorChangedUPP(userUPP)
    #define InvokeNColorChangedUPP(userData, newColor, userUPP) (*userUPP)(userData, newColor)
  #endif
#endif

#if __LP64__

typedef void *                          ColorChangedUPP;
typedef void *                          UserEventUPP;
#else
typedef CALLBACK_API( void , ColorChangedProcPtr )(SInt32 userData, PMColor *newColor);
typedef CALLBACK_API( Boolean , UserEventProcPtr )(EventRecord * event);
typedef STACK_UPP_TYPE(ColorChangedProcPtr)                     ColorChangedUPP;
typedef STACK_UPP_TYPE(UserEventProcPtr)                        UserEventUPP;
/*
 *  NewColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern ColorChangedUPP
NewColorChangedUPP(ColorChangedProcPtr userRoutine)           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewUserEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern UserEventUPP
NewUserEventUPP(UserEventProcPtr userRoutine)                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeColorChangedUPP(ColorChangedUPP userUPP)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeUserEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeUserEventUPP(UserEventUPP userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeColorChangedUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
InvokeColorChangedUPP(
  SInt32           userData,
  PMColor *        newColor,
  ColorChangedUPP  userUPP)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeUserEventUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Boolean
InvokeUserEventUPP(
  EventRecord *  event,
  UserEventUPP   userUPP)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline ColorChangedUPP                                      NewColorChangedUPP(ColorChangedProcPtr userRoutine) { return userRoutine; }
    inline UserEventUPP                                         NewUserEventUPP(UserEventProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeColorChangedUPP(ColorChangedUPP) { }
    inline void                                                 DisposeUserEventUPP(UserEventUPP) { }
    inline void                                                 InvokeColorChangedUPP(SInt32 userData, PMColor * newColor, ColorChangedUPP userUPP) { (*userUPP)(userData, newColor); }
    inline Boolean                                              InvokeUserEventUPP(EventRecord * event, UserEventUPP userUPP) { return (*userUPP)(event); }
  #else
    #define NewColorChangedUPP(userRoutine)                     ((ColorChangedUPP)userRoutine)
    #define NewUserEventUPP(userRoutine)                        ((UserEventUPP)userRoutine)
    #define DisposeColorChangedUPP(userUPP)
    #define DisposeUserEventUPP(userUPP)
    #define InvokeColorChangedUPP(userData, newColor, userUPP)  (*userUPP)(userData, newColor)
    #define InvokeUserEventUPP(event, userUPP)                  (*userUPP)(event)
  #endif
#endif

#endif  /* __LP64__ */

#if !__LP64__
struct ColorPickerInfo {
  PMColor             theColor;
  CMProfileHandle     dstProfile;
  UInt32              flags;
  DialogPlacementSpec  placeWhere;
  Point               dialogOrigin;
  OSType              pickerType;
  UserEventUPP        eventProc;
  ColorChangedUPP     colorProc;
  UInt32              colorProcData;
  Str255              prompt;
  PickerMenuItemInfo  mInfo;
  Boolean             newColorChosen;
  SInt8               filler;
};
typedef struct ColorPickerInfo          ColorPickerInfo;
#endif  /* !__LP64__ */

struct NColorPickerInfo {
  NPMColor            theColor;
  CMProfileRef        dstProfile;             /* Currently ignored */
  UInt32              flags;                  /* Currently ignored */
  DialogPlacementSpec  placeWhere;            /* Currently ignored */
  Point               dialogOrigin;           /* Currently ignored */
  OSType              pickerType;             /* Currently ignored */
#if !__LP64__

  UserEventUPP        eventProc;              /* Ignored */
#endif

  NColorChangedUPP    colorProc;
  URefCon             colorProcData;
  Str255              prompt;                 /* Currently ignored */
  PickerMenuItemInfo  mInfo;                  /* Ignored */
  Boolean             newColorChosen;
  UInt8               reserved;               /* Must be 0 */
};
typedef struct NColorPickerInfo         NColorPickerInfo;

#if !__LP64__
/*
 *  Fix2SmallFract()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SmallFract 
Fix2SmallFract(Fixed f)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  SmallFract2Fix()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Fixed 
SmallFract2Fix(SmallFract s)                                  AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  CMY2RGB()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
CMY2RGB(
  const CMYColor *  cColor,
  RGBColor *        rColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RGB2CMY()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
RGB2CMY(
  const RGBColor *  rColor,
  CMYColor *        cColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HSL2RGB()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HSL2RGB(
  const HSLColor *  hColor,
  RGBColor *        rColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RGB2HSL()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
RGB2HSL(
  const RGBColor *  rColor,
  HSLColor *        hColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  HSV2RGB()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
HSV2RGB(
  const HSVColor *  hColor,
  RGBColor *        rColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


/*
 *  RGB2HSV()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
RGB2HSV(
  const RGBColor *  rColor,
  HSVColor *        hColor)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

/*
 *  GetColor()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
GetColor(
  Point              where,
  ConstStr255Param   prompt,
  const RGBColor *   inColor,
  RGBColor *         outColor)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#if !__LP64__
/*
 *  PickColor()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework [32-bit only]
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in ColorPickerLib 2.0 and later
 */
extern OSErr 
PickColor(ColorPickerInfo * theColorInfo)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;


#endif  /* !__LP64__ */

/*
 *  NPickColor()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in Carbon.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in ColorPickerLib 2.1 and later
 */
extern OSErr 
NPickColor(NColorPickerInfo * theColorInfo)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __COLORPICKER__ */

