/*
     File:       HIServices/Icons.h
 
     Contains:   Icon Utilities and Icon Services Interfaces.
 
     Copyright:  © 1990-2012 by Apple Computer, Inc. All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ICONS__
#define __ICONS__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __QUICKDRAW__
#include <QD/Quickdraw.h>
#endif

#ifndef __CGCONTEXT__
#include <CoreGraphics/CGContext.h>
#endif

#ifndef __HISHAPE__
#include <HIServices/HIShape.h>
#endif


#include <AvailabilityMacros.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

/* Alignment type values. */
enum {
  kAlignNone                    = 0x00,
  kAlignVerticalCenter          = 0x01,
  kAlignTop                     = 0x02,
  kAlignBottom                  = 0x03,
  kAlignHorizontalCenter        = 0x04,
  kAlignAbsoluteCenter          = kAlignVerticalCenter | kAlignHorizontalCenter,
  kAlignCenterTop               = kAlignTop | kAlignHorizontalCenter,
  kAlignCenterBottom            = kAlignBottom | kAlignHorizontalCenter,
  kAlignLeft                    = 0x08,
  kAlignCenterLeft              = kAlignVerticalCenter | kAlignLeft,
  kAlignTopLeft                 = kAlignTop | kAlignLeft,
  kAlignBottomLeft              = kAlignBottom | kAlignLeft,
  kAlignRight                   = 0x0C,
  kAlignCenterRight             = kAlignVerticalCenter | kAlignRight,
  kAlignTopRight                = kAlignTop | kAlignRight,
  kAlignBottomRight             = kAlignBottom | kAlignRight
};

/* Obsolete. Use names defined above. */
enum {
  atNone                        = kAlignNone,
  atVerticalCenter              = kAlignVerticalCenter,
  atTop                         = kAlignTop,
  atBottom                      = kAlignBottom,
  atHorizontalCenter            = kAlignHorizontalCenter,
  atAbsoluteCenter              = kAlignAbsoluteCenter,
  atCenterTop                   = kAlignCenterTop,
  atCenterBottom                = kAlignCenterBottom,
  atLeft                        = kAlignLeft,
  atCenterLeft                  = kAlignCenterLeft,
  atTopLeft                     = kAlignTopLeft,
  atBottomLeft                  = kAlignBottomLeft,
  atRight                       = kAlignRight,
  atCenterRight                 = kAlignCenterRight,
  atTopRight                    = kAlignTopRight,
  atBottomRight                 = kAlignBottomRight
};

typedef SInt16                          IconAlignmentType;
/* Transform type values. */
enum {
  kTransformNone                = 0x00,
  kTransformDisabled            = 0x01,
  kTransformOffline             = 0x02,
  kTransformOpen                = 0x03,
  kTransformLabel1              = 0x0100,
  kTransformLabel2              = 0x0200,
  kTransformLabel3              = 0x0300,
  kTransformLabel4              = 0x0400,
  kTransformLabel5              = 0x0500,
  kTransformLabel6              = 0x0600,
  kTransformLabel7              = 0x0700,
  kTransformSelected            = 0x4000,
  kTransformSelectedDisabled    = kTransformSelected | kTransformDisabled,
  kTransformSelectedOffline     = kTransformSelected | kTransformOffline,
  kTransformSelectedOpen        = kTransformSelected | kTransformOpen
};

/* Obsolete. Use names defined above. */
enum {
  ttNone                        = kTransformNone,
  ttDisabled                    = kTransformDisabled,
  ttOffline                     = kTransformOffline,
  ttOpen                        = kTransformOpen,
  ttLabel1                      = kTransformLabel1,
  ttLabel2                      = kTransformLabel2,
  ttLabel3                      = kTransformLabel3,
  ttLabel4                      = kTransformLabel4,
  ttLabel5                      = kTransformLabel5,
  ttLabel6                      = kTransformLabel6,
  ttLabel7                      = kTransformLabel7,
  ttSelected                    = kTransformSelected,
  ttSelectedDisabled            = kTransformSelectedDisabled,
  ttSelectedOffline             = kTransformSelectedOffline,
  ttSelectedOpen                = kTransformSelectedOpen
};

typedef SInt16                          IconTransformType;
/* Selector mask values. */
enum {
  kSelectorLarge1Bit            = 0x00000001,
  kSelectorLarge4Bit            = 0x00000002,
  kSelectorLarge8Bit            = 0x00000004,
  kSelectorLarge32Bit           = 0x00000008,
  kSelectorLarge8BitMask        = 0x00000010,
  kSelectorSmall1Bit            = 0x00000100,
  kSelectorSmall4Bit            = 0x00000200,
  kSelectorSmall8Bit            = 0x00000400,
  kSelectorSmall32Bit           = 0x00000800,
  kSelectorSmall8BitMask        = 0x00001000,
  kSelectorMini1Bit             = 0x00010000,
  kSelectorMini4Bit             = 0x00020000,
  kSelectorMini8Bit             = 0x00040000,
  kSelectorHuge1Bit             = 0x01000000,
  kSelectorHuge4Bit             = 0x02000000,
  kSelectorHuge8Bit             = 0x04000000,
  kSelectorHuge32Bit            = 0x08000000,
  kSelectorHuge8BitMask         = 0x10000000,
  kSelectorAllLargeData         = 0x000000FF,
  kSelectorAllSmallData         = 0x0000FF00,
  kSelectorAllMiniData          = 0x00FF0000,
  kSelectorAllHugeData          = (UInt32)0xFF000000,
  kSelectorAll1BitData          = kSelectorLarge1Bit | kSelectorSmall1Bit | kSelectorMini1Bit | kSelectorHuge1Bit,
  kSelectorAll4BitData          = kSelectorLarge4Bit | kSelectorSmall4Bit | kSelectorMini4Bit | kSelectorHuge4Bit,
  kSelectorAll8BitData          = kSelectorLarge8Bit | kSelectorSmall8Bit | kSelectorMini8Bit | kSelectorHuge8Bit,
  kSelectorAll32BitData         = kSelectorLarge32Bit | kSelectorSmall32Bit | kSelectorHuge32Bit,
  kSelectorAllAvailableData     = (UInt32)0xFFFFFFFF
};

typedef UInt32                          IconSelectorValue;
/* Obsolete. Use names defined above. */
enum {
  svLarge1Bit                   = kSelectorLarge1Bit,
  svLarge4Bit                   = kSelectorLarge4Bit,
  svLarge8Bit                   = kSelectorLarge8Bit,
  svSmall1Bit                   = kSelectorSmall1Bit,
  svSmall4Bit                   = kSelectorSmall4Bit,
  svSmall8Bit                   = kSelectorSmall8Bit,
  svMini1Bit                    = kSelectorMini1Bit,
  svMini4Bit                    = kSelectorMini4Bit,
  svMini8Bit                    = kSelectorMini8Bit,
  svAllLargeData                = kSelectorAllLargeData,
  svAllSmallData                = kSelectorAllSmallData,
  svAllMiniData                 = kSelectorAllMiniData,
  svAll1BitData                 = kSelectorAll1BitData,
  svAll4BitData                 = kSelectorAll4BitData,
  svAll8BitData                 = kSelectorAll8BitData,
  svAllAvailableData            = kSelectorAllAvailableData
};

typedef CALLBACK_API( OSErr , IconActionProcPtr )(ResType theType, Handle *theIcon, void *yourDataPtr);
typedef CALLBACK_API( Handle , IconGetterProcPtr )(ResType theType, void *yourDataPtr);
typedef STACK_UPP_TYPE(IconActionProcPtr)                       IconActionUPP;
typedef STACK_UPP_TYPE(IconGetterProcPtr)                       IconGetterUPP;
/*
 *  NewIconActionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern IconActionUPP
NewIconActionUPP(IconActionProcPtr userRoutine)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  NewIconGetterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern IconGetterUPP
NewIconGetterUPP(IconGetterProcPtr userRoutine)               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeIconActionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeIconActionUPP(IconActionUPP userUPP)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  DisposeIconGetterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern void
DisposeIconGetterUPP(IconGetterUPP userUPP)                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeIconActionUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern OSErr
InvokeIconActionUPP(
  ResType        theType,
  Handle *       theIcon,
  void *         yourDataPtr,
  IconActionUPP  userUPP)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

/*
 *  InvokeIconGetterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
extern Handle
InvokeIconGetterUPP(
  ResType        theType,
  void *         yourDataPtr,
  IconGetterUPP  userUPP)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;

#if __MACH__
  #ifdef __cplusplus
    inline IconActionUPP                                        NewIconActionUPP(IconActionProcPtr userRoutine) { return userRoutine; }
    inline IconGetterUPP                                        NewIconGetterUPP(IconGetterProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeIconActionUPP(IconActionUPP) { }
    inline void                                                 DisposeIconGetterUPP(IconGetterUPP) { }
    inline OSErr                                                InvokeIconActionUPP(ResType theType, Handle * theIcon, void * yourDataPtr, IconActionUPP userUPP) { return (*userUPP)(theType, theIcon, yourDataPtr); }
    inline Handle                                               InvokeIconGetterUPP(ResType theType, void * yourDataPtr, IconGetterUPP userUPP) { return (*userUPP)(theType, yourDataPtr); }
  #else
    #define NewIconActionUPP(userRoutine)                       ((IconActionUPP)userRoutine)
    #define NewIconGetterUPP(userRoutine)                       ((IconGetterUPP)userRoutine)
    #define DisposeIconActionUPP(userUPP)
    #define DisposeIconGetterUPP(userUPP)
    #define InvokeIconActionUPP(theType, theIcon, yourDataPtr, userUPP) (*userUPP)(theType, theIcon, yourDataPtr)
    #define InvokeIconGetterUPP(theType, yourDataPtr, userUPP)  (*userUPP)(theType, yourDataPtr)
  #endif
#endif

#if !__LP64__
typedef IconGetterProcPtr               IconGetter;
typedef IconActionProcPtr               IconAction;
struct CIcon {
  PixMap              iconPMap;               /*the icon's pixMap*/
  BitMap              iconMask;               /*the icon's mask*/
  BitMap              iconBMap;               /*the icon's bitMap*/
  Handle              iconData;               /*the icon's data*/
  SInt16              iconMaskData[1];        /*icon's mask and BitMap data*/
};
typedef struct CIcon                    CIcon;
typedef CIcon *                         CIconPtr;
typedef CIconPtr *                      CIconHandle;
/*
    Note:   IconSuiteRef and IconCacheRef should be an abstract types, 
            but too much source code already relies on them being of type Handle.
*/
typedef Handle                          IconSuiteRef;
typedef Handle                          IconCacheRef;
#endif  /* !__LP64__ */
#if !__LP64__
/*
 *  GetCIcon()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern CIconHandle 
GetCIcon(SInt16 iconID)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotCIcon()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PlotCIcon(
  const Rect *  theRect,
  CIconHandle   theIcon)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DisposeCIcon()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
DisposeCIcon(CIconHandle theIcon)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetIcon()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Handle 
GetIcon(SInt16 iconID)                                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotIcon()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern void 
PlotIcon(
  const Rect *  theRect,
  Handle        theIcon)                                      AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotIconID()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotIconID(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  SInt16              theResID)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  NewIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
NewIconSuite(IconSuiteRef * theIconSuite)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  AddIconToSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
AddIconToSuite(
  Handle         theIconData,
  IconSuiteRef   theSuite,
  ResType        theType)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetIconFromSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetIconFromSuite(
  Handle *       theIconData,
  IconSuiteRef   theSuite,
  ResType        theType)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  ForEachIconDo()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
ForEachIconDo(
  IconSuiteRef        theSuite,
  IconSelectorValue   selector,
  IconActionUPP       action,
  void *              yourDataPtr)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetIconSuite(
  IconSuiteRef *      theIconSuite,
  SInt16              theResID,
  IconSelectorValue   selector)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  DisposeIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
DisposeIconSuite(
  IconSuiteRef   theIconSuite,
  Boolean        disposeData)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotIconSuite(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  IconSuiteRef        theIconSuite)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  MakeIconCache()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
MakeIconCache(
  IconCacheRef *  theCache,
  IconGetterUPP   makeIcon,
  void *          yourDataPtr)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  LoadIconCache()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
LoadIconCache(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  IconCacheRef        theIconCache)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotIconMethod()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotIconMethod(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  IconGetterUPP       theMethod,
  void *              yourDataPtr)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetLabel()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.3
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetLabel(
  SInt16      labelNumber,
  RGBColor *  labelColor,
  Str255      labelString)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PtInIconID()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
PtInIconID(
  Point               testPt,
  const Rect *        iconRect,
  IconAlignmentType   align,
  SInt16              iconID)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PtInIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
PtInIconSuite(
  Point               testPt,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconSuiteRef        theIconSuite)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PtInIconMethod()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
PtInIconMethod(
  Point               testPt,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconGetterUPP       theMethod,
  void *              yourDataPtr)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  RectInIconID()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
RectInIconID(
  const Rect *        testRect,
  const Rect *        iconRect,
  IconAlignmentType   align,
  SInt16              iconID)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  RectInIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
RectInIconSuite(
  const Rect *        testRect,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconSuiteRef        theIconSuite)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  RectInIconMethod()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern Boolean 
RectInIconMethod(
  const Rect *        testRect,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconGetterUPP       theMethod,
  void *              yourDataPtr)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IconIDToRgn()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
IconIDToRgn(
  RgnHandle           theRgn,
  const Rect *        iconRect,
  IconAlignmentType   align,
  SInt16              iconID)                                 AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IconSuiteToRgn()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
IconSuiteToRgn(
  RgnHandle           theRgn,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconSuiteRef        theIconSuite)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  IconMethodToRgn()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
IconMethodToRgn(
  RgnHandle           theRgn,
  const Rect *        iconRect,
  IconAlignmentType   align,
  IconGetterUPP       theMethod,
  void *              yourDataPtr)                            AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetSuiteLabel()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetSuiteLabel(
  IconSuiteRef   theSuite,
  SInt16         theLabel)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetSuiteLabel()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern SInt16 
GetSuiteLabel(IconSuiteRef theSuite)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetIconCacheData()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetIconCacheData(
  IconCacheRef   theCache,
  void **        theData)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetIconCacheData()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetIconCacheData(
  IconCacheRef   theCache,
  void *         theData)                                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  GetIconCacheProc()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
GetIconCacheProc(
  IconCacheRef     theCache,
  IconGetterUPP *  theProc)                                   AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  SetIconCacheProc()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
SetIconCacheProc(
  IconCacheRef    theCache,
  IconGetterUPP   theProc)                                    AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotIconHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotIconHandle(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  Handle              theIcon)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotSICNHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotSICNHandle(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  Handle              theSICN)                                AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


/*
 *  PlotCIconHandle()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in InterfaceLib 7.1 and later
 */
extern OSErr 
PlotCIconHandle(
  const Rect *        theRect,
  IconAlignmentType   align,
  IconTransformType   transform,
  CIconHandle         theCIcon)                               AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;


#endif  /* !__LP64__ */


/*
 *  PlotIconRefFlags
 *  
 *  Discussion:
 *    Flags that can be passed to the PlotIconRefInContext routine.
 */
typedef UInt32 PlotIconRefFlags;
enum {

  /*
   * Draw the icon image and alpha channel.
   */
  kPlotIconRefNormalFlags       = 0,

  /*
   * Only draw the alpha channel.
   */
  kPlotIconRefNoImage           = (1 << 1),

  /*
   * Only draw the image.
   */
  kPlotIconRefNoMask            = (1 << 2)
};



/*
  ==============================================================================
   Converting data structures
  ==============================================================================
*/


/*
   IconRefToIconFamily
   This routines returns a new IconFamily that contains the data corresponding
   to the specified IconRef.
*/

/*
 *  IconRefToIconFamily()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
IconRefToIconFamily(
  IconRef             theIconRef,
  IconSelectorValue   whichIcons,
  IconFamilyHandle *  iconFamily)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
   IconFamilyToIconSuite
   This routine transfers the data from an icon family handle into an icon suite.
*/

#if !__LP64__
/*
 *  IconFamilyToIconSuite()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
IconFamilyToIconSuite(
  IconFamilyHandle    iconFamily,
  IconSelectorValue   whichIcons,
  IconSuiteRef *      iconSuite)                              AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*
   IconSuiteToIconFamily
   This routine transfers the data in an icon suite into an icon family.
*/

/*
 *  IconSuiteToIconFamily()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
IconSuiteToIconFamily(
  IconSuiteRef        iconSuite,
  IconSelectorValue   whichIcons,
  IconFamilyHandle *  iconFamily)                             AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*
   SetIconFamilyData
   Change the data of an icon family. The data is copied.
   The type can be one of the icon type, or 'PICT'.
   The data will be compressed if needed.
*/

#endif  /* !__LP64__ */

/*
 *  SetIconFamilyData()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
SetIconFamilyData(
  IconFamilyHandle   iconFamily,
  OSType             iconType,
  Handle             h)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
   GetIconFamilyData
   Return a copy of the data in the icon family.
   The type can be one of the icon type, or 'PICT'
   The data will be returned uncompressed.
   The handle (h) will be resized as appropriate. If no data of the 
   requested type is present, the handle size will be set to 0.
*/

/*
 *  GetIconFamilyData()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
GetIconFamilyData(
  IconFamilyHandle   iconFamily,
  OSType             iconType,
  Handle             h)                                       AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
   PlotIconRef
   
   This routine plots the IconRef.  It mostly takes the same parameters as 
   PlotIconSuite. Pass kIconServicesNormalUsageFlag as a default value for 
   IconServicesUsageFlags.
*/

#if !__LP64__
/*
 *  PlotIconRef()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
PlotIconRef(
  const Rect *             theRect,
  IconAlignmentType        align,
  IconTransformType        transform,
  IconServicesUsageFlags   theIconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/* PlotIconRefInContext*/
#endif  /* !__LP64__ */

/*
 *  PlotIconRefInContext()
 *  
 *  Discussion:
 *    This routines plots an IconRef using Quartz/CoreGraphics.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.4
 *  
 *  Parameters:
 *    
 *    inContext:
 *      The graphics context to use.
 *    
 *    inRect:
 *      The rect to plot the icon in.
 *    
 *    inAlign:
 *      The icon alignment.
 *    
 *    inTransform:
 *      The icon transform.
 *    
 *    inLabelColor:
 *      The icon label color.
 *    
 *    inFlags:
 *      The drawing flags to use (usually kPlotIconRefNormalFlags).
 *    
 *    inIconRef:
 *      The IconRef to plot.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.1 and later in ApplicationServices.framework
 *    CarbonLib:        not available in CarbonLib 1.x, is available on Mac OS X version 10.1 and later
 *    Non-Carbon CFM:   not available
 */
extern OSStatus 
PlotIconRefInContext(
  CGContextRef        inContext,
  const CGRect *      inRect,
  IconAlignmentType   inAlign,
  IconTransformType   inTransform,
  const RGBColor *    inLabelColor,
  PlotIconRefFlags    inFlags,
  IconRef             inIconRef)                              AVAILABLE_MAC_OS_X_VERSION_10_1_AND_LATER;



/*
   PtInIconRef
   
   This routine indicates if testPt would hit the icon designated by iconRef.
   It mostly takes the same parameters as PtInIconSuite.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
   This call is deprecated. Please use IconRefContainsCGPoint() instead.
*/


#if !__LP64__
/*
 *  PtInIconRef()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern Boolean 
PtInIconRef(
  const Point *            testPt,
  const Rect *             iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   theIconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*
   RectInIconRef
   
   This routine indicates if testRect would intersect the icon designated by iconRef.
   It mostly takes the same parameters as RectInIconSuite.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
   This call is deprecated. Please use IconRefIntersectsCGRect() instead.
*/


/*
 *  RectInIconRef()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern Boolean 
RectInIconRef(
  const Rect *             testRect,
  const Rect *             iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



/*
   IconRefToRgn
   
   This routine returns a region for the icon.
   It mostly takes the same parameters as IconSuiteToRgn.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
   This call is deprecated. Please use IconRefToHIShape() instead.
*/

/*
 *  IconRefToRgn()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.5
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
IconRefToRgn(
  RgnHandle                theRgn,
  const Rect *             iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_5;



#endif  /* !__LP64__ */

/*
 *  IconRefContainsCGPoint()
 *  
 *  Discussion:
 *    Tests whether a specified point falls within an iconÕs mask.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    testPt:
 *      A pointer to the location that IconServices tests to see
 *      whether is falls within the mask of indicated icon.
 *    
 *    iconRect:
 *      A pointer to the rectangle defining the area that Icon Services
 *      uses to determine which icon is hit-tested. Use the same CGRect
 *      value as when the icon was last drawn.
 *    
 *    align:
 *      A value that specifies how the indicated icon is aligned within
 *      the rectangle specified in the iconRect parameter. Use the same
 *      IconAlignmentType value as when the icon was last drawn.
 *    
 *    iconServicesUsageFlags:
 *      Reserved for future use. Pass the kIconServicesNormalUsageFlag
 *      constant in this parameter.
 *    
 *    theIconRef:
 *      The icon to be tested.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IconRefContainsCGPoint(
  const CGPoint *          testPt,
  const CGRect *           iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*
 *  IconRefIntersectsCGRect()
 *  
 *  Discussion:
 *    Tests whether a specified rectangle falls within an iconÕs mask.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    testRect:
 *      A pointer to the rectangle that IconServices tests to see
 *      whether is falls within the mask of indicated icon.
 *    
 *    iconRect:
 *      A pointer to the rectangle defining the area that Icon Services
 *      uses to determine which icon is hit-tested. Use the same CGRect
 *      value as when the icon was last drawn.
 *    
 *    align:
 *      A value that specifies how the indicated icon is aligned within
 *      the rectangle specified in the iconRect parameter. Use the same
 *      IconAlignmentType value as when the icon was last drawn.
 *    
 *    iconServicesUsageFlags:
 *      Reserved for future use. Pass the kIconServicesNormalUsageFlag
 *      constant in this parameter.
 *    
 *    theIconRef:
 *      The icon to be tested.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern Boolean 
IconRefIntersectsCGRect(
  const CGRect *           testRect,
  const CGRect *           iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*
 *  IconRefToHIShape()
 *  
 *  Discussion:
 *    Converts an IconRef-derived icon into an HIShape.
 *  
 *  Mac OS X threading:
 *    Thread safe since version 10.5
 *  
 *  Parameters:
 *    
 *    iconRect:
 *      A pointer to the rectangle defining the area that Icon Services
 *      uses as the bounding box of the region.
 *    
 *    align:
 *      The value which determines how Icon Services aligns the region
 *      within the rectangle.
 *    
 *    iconServicesUsageFlags:
 *      Reserved for future use. Pass the kIconServicesNormalUsageFlag
 *      constant in this parameter.
 *    
 *    theIconRef:
 *      The icon to be tested.
 *  
 *  Availability:
 *    Mac OS X:         in version 10.5 and later in ApplicationServices.framework
 *    CarbonLib:        not available
 *    Non-Carbon CFM:   not available
 */
extern HIShapeRef 
IconRefToHIShape(
  const CGRect *           iconRect,
  IconAlignmentType        align,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;



/*
   GetIconSizesFromIconRef
   
   This routine returns an IconSelectorValue indicating the depths and sizes of 
   icon data which are actually available.  It takes an IconSelectorValue 
   indicating which sizes/depths the caller is interested and returns an 
   IconSelectorValue indicating which sizes/depths exist.
   
   Caution:
   
   This is potentially an extremely expensive call as IconServices may be forced 
   to attempt fetching the data for the IconRef's sizes/depths which may result 
   in hitting the local disk or even the network to obtain the data to determine 
   which sizes/depths actually exist.
   Pass kIconServicesNormalUsageFlag as a default value for IconServicesUsageFlags.
   
   This call is deprecated. Please use IsDataAvailableInIconRef() instead.
*/

#if !__LP64__
/*
 *  GetIconSizesFromIconRef()   *** DEPRECATED ***
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework [32-bit only] but deprecated in 10.3
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern OSErr 
GetIconSizesFromIconRef(
  IconSelectorValue        iconSelectorInput,
  IconSelectorValue *      iconSelectorOutputPtr,
  IconServicesUsageFlags   iconServicesUsageFlags,
  IconRef                  theIconRef)                        AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_3;



/*
   IsIconRefMaskEmpty
   Returns true if the mask for this icon is blank
*/

#endif  /* !__LP64__ */

/*
 *  IsIconRefMaskEmpty()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 8.5 and later
 */
extern Boolean 
IsIconRefMaskEmpty(IconRef iconRef)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;



/*
   GetIconRefVariant
   Icon variants allows different images to be used with different icon state.
   For example, the 'open' variant for a folder could be represented with
   an open folder.
   Given an icon ref and a desired variant, this routine returns an icon
   ref (which may be the same as the input icon ref) and a transformation 
   which should be used with PlotIconRef() to render the icon appropriately.
   The returned icon ref should be used to do hit-testing.
*/

/*
 *  GetIconRefVariant()
 *  
 *  Mac OS X threading:
 *    Not thread safe
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   in IconServicesLib 9.0 and later
 */
extern IconRef 
GetIconRefVariant(
  IconRef              inIconRef,
  OSType               inVariant,
  IconTransformType *  outTransform)                          AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER;















#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __ICONS__ */

