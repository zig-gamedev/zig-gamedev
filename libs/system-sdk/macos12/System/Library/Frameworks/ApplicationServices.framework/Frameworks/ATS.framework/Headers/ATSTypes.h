/*
     File:       ATS/ATSTypes.h
 
     Contains:   Public interfaces for Apple Type Services components.
 
     Version:    ATS
 
     Copyright:  Copyright 1997-2019 by Apple Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ATSTYPES__
#define __ATSTYPES__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __CGGEOMETRY__
#include <CoreGraphics/CGGeometry.h>
#endif

#include <AvailabilityMacros.h>

#ifndef __ATSDEFINES__
#include <ATS/ATSDefines.h>
#endif

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#ifndef CGFLOAT_DEFINED
/* For the time being CGFLOAT_DEFINED will serve to indicate the presence 
       of new RefCon types in CoreServices. If new headers and associated typedefs
       are not present then URefCon is declared here.
    */
#ifdef __LP64__
typedef void *                          URefCon;
#else
typedef unsigned long                   URefCon;
#endif  /* defined(__LP64__) */

#endif  /* !defined(CGFLOAT_DEFINED) */

#ifdef __LP64__
typedef CGPoint                         ATSPoint;
#else
typedef Float32Point                    ATSPoint;
#endif  /* defined(__LP64__) */

#if ATS_LEGACY_API
#ifdef __LP64__
/*
   ATSFSSpec serves as a temporary place holder for the FSSpec data type which is deprecated for 64-bit. 
   A 64-bit replacement for FSSpec based APIs will be introduced.  
*/
struct ATSFSSpec {
  FSVolumeRefNum      vRefNum;
  SInt32              parID;
  StrFileName         name;
};
typedef struct ATSFSSpec                ATSFSSpec;
#else
typedef FSSpec                          ATSFSSpec;
#endif  /* defined(__LP64__) */
#endif // ATS_LEGACY_API

/* FMGeneration */
typedef UInt32                          FMGeneration;
/* The FMFontFamily reference represents a collection of fonts with the same design
   characteristics. It replaces the standard QuickDraw font identifer and may be used
   with all QuickDraw functions including GetFontName and TextFont. It cannot be used
   with the Resource Manager to access information from a FOND resource handle. A font
   reference does not imply a particular script system, nor is the character encoding
   of a font family determined by an arithmetic mapping of its value.
*/
typedef SInt16                          FMFontFamily;
typedef SInt16                          FMFontStyle;
typedef SInt16                          FMFontSize;
/* 
   The font family is a collection of fonts, each of which is identified
   by an FMFont reference that maps to a single object registered with
   the font database. The font references associated with the font
   family consist of individual outline and bitmapped fonts that may be
   used with the font access routines of the Font Manager and ATS.
*/
typedef UInt32                          FMFont;
struct FMFontFamilyInstance {
  FMFontFamily        fontFamily;
  FMFontStyle         fontStyle;
};
typedef struct FMFontFamilyInstance     FMFontFamilyInstance;
struct FMFontFamilyIterator {
  UInt32              reserved[16];
};
typedef struct FMFontFamilyIterator     FMFontFamilyIterator;
struct FMFontIterator {
  UInt32              reserved[16];
};
typedef struct FMFontIterator           FMFontIterator;
struct FMFontFamilyInstanceIterator {
  UInt32              reserved[16];
};
typedef struct FMFontFamilyInstanceIterator FMFontFamilyInstanceIterator;
enum {
  kInvalidGeneration            = 0,
  kInvalidFontFamily            = -1,
  kInvalidFont                  = 0
};

enum {
  kFMCurrentFilterFormat        = 0
};

typedef UInt32 FMFilterSelector;
enum {
  kFMFontTechnologyFilterSelector = 1,
  kFMFontContainerFilterSelector = 2,
  kFMGenerationFilterSelector   = 3,
  kFMFontFamilyCallbackFilterSelector = 4,
  kFMFontCallbackFilterSelector = 5,
  kFMFontDirectoryFilterSelector = 6,
  kFMFontFileRefFilterSelector  = 10
};

enum {
  kFMTrueTypeFontTechnology     = 'true',
  kFMPostScriptFontTechnology   = 'typ1'
};

#if ATS_LEGACY_API

typedef CALLBACK_API( OSStatus , FMFontFamilyCallbackFilterProcPtr )(FMFontFamily iFontFamily, void *iRefCon);
typedef CALLBACK_API( OSStatus , FMFontCallbackFilterProcPtr )(FMFont iFont, void *iRefCon);
typedef STACK_UPP_TYPE(FMFontFamilyCallbackFilterProcPtr)       FMFontFamilyCallbackFilterUPP;
typedef STACK_UPP_TYPE(FMFontCallbackFilterProcPtr)             FMFontCallbackFilterUPP;
/*
 *  NewFMFontFamilyCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern FMFontFamilyCallbackFilterUPP
NewFMFontFamilyCallbackFilterUPP(FMFontFamilyCallbackFilterProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

/*
 *  NewFMFontCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern FMFontCallbackFilterUPP
NewFMFontCallbackFilterUPP(FMFontCallbackFilterProcPtr userRoutine) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

/*
 *  DisposeFMFontFamilyCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern void
DisposeFMFontFamilyCallbackFilterUPP(FMFontFamilyCallbackFilterUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

/*
 *  DisposeFMFontCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern void
DisposeFMFontCallbackFilterUPP(FMFontCallbackFilterUPP userUPP) AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

/*
 *  InvokeFMFontFamilyCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern OSStatus
InvokeFMFontFamilyCallbackFilterUPP(
  FMFontFamily                   iFontFamily,
  void *                         iRefCon,
  FMFontFamilyCallbackFilterUPP  userUPP)                     AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

/*
 *  InvokeFMFontCallbackFilterUPP()
 *  
 *  Availability:
 *    Mac OS X:         in version 10.0 and later in ApplicationServices.framework
 *    CarbonLib:        in CarbonLib 1.0 and later
 *    Non-Carbon CFM:   available as macro/inline
 */
#if !__LP64__
extern OSStatus
InvokeFMFontCallbackFilterUPP(
  FMFont                   iFont,
  void *                   iRefCon,
  FMFontCallbackFilterUPP  userUPP)                           AVAILABLE_MAC_OS_X_VERSION_10_0_AND_LATER_BUT_DEPRECATED_IN_MAC_OS_X_VERSION_10_7;
#endif // !__LP64

#if __MACH__
  #ifdef __cplusplus
    inline FMFontFamilyCallbackFilterUPP                        NewFMFontFamilyCallbackFilterUPP(FMFontFamilyCallbackFilterProcPtr userRoutine) { return userRoutine; }
    inline FMFontCallbackFilterUPP                              NewFMFontCallbackFilterUPP(FMFontCallbackFilterProcPtr userRoutine) { return userRoutine; }
    inline void                                                 DisposeFMFontFamilyCallbackFilterUPP(FMFontFamilyCallbackFilterUPP) { }
    inline void                                                 DisposeFMFontCallbackFilterUPP(FMFontCallbackFilterUPP) { }
    inline OSStatus                                             InvokeFMFontFamilyCallbackFilterUPP(FMFontFamily iFontFamily, void * iRefCon, FMFontFamilyCallbackFilterUPP userUPP) { return (*userUPP)(iFontFamily, iRefCon); }
    inline OSStatus                                             InvokeFMFontCallbackFilterUPP(FMFont iFont, void * iRefCon, FMFontCallbackFilterUPP userUPP) { return (*userUPP)(iFont, iRefCon); }
  #else
    #define NewFMFontFamilyCallbackFilterUPP(userRoutine)       ((FMFontFamilyCallbackFilterUPP)userRoutine)
    #define NewFMFontCallbackFilterUPP(userRoutine)             ((FMFontCallbackFilterUPP)userRoutine)
    #define DisposeFMFontFamilyCallbackFilterUPP(userUPP)
    #define DisposeFMFontCallbackFilterUPP(userUPP)
    #define InvokeFMFontFamilyCallbackFilterUPP(iFontFamily, iRefCon, userUPP) (*userUPP)(iFontFamily, iRefCon)
    #define InvokeFMFontCallbackFilterUPP(iFont, iRefCon, userUPP) (*userUPP)(iFont, iRefCon)
  #endif
#endif

struct FMFontDirectoryFilter {
  SInt16              fontFolderDomain;
  UInt32              reserved[2];
};
typedef struct FMFontDirectoryFilter    FMFontDirectoryFilter;
/*
   Note: The fontContainerFilter member is not available in 64-bit. Use fontFileRefFilter
   and the kFMFontFileRefFilterSelector enum instead.
*/
struct FMFilter {
  UInt32              format;
  FMFilterSelector    selector;
  union {
    FourCharCode        fontTechnologyFilter;
    ATSFSSpec           fontContainerFilter;
    FMGeneration        generationFilter;
    FMFontFamilyCallbackFilterUPP  fontFamilyCallbackFilter;
    FMFontCallbackFilterUPP  fontCallbackFilter;
    FMFontDirectoryFilter  fontDirectoryFilter;
    const FSRef *       fontFileRefFilter;
  }                       filter;
};
typedef struct FMFilter                 FMFilter;

#endif // ATS_LEGACY_API

typedef OptionBits                      ATSOptionFlags;
typedef UInt32                          ATSGeneration;
typedef UInt32                          ATSFontContainerRef;
typedef UInt32                          ATSFontFamilyRef;
#ifndef ATSFONTREF_DEFINED
typedef UInt32                          ATSFontRef;
#define ATSFONTREF_DEFINED 1
#endif
typedef UInt16                          ATSGlyphRef;
typedef CGFloat                         ATSFontSize;
typedef UInt32                          ATSFontFormat;
enum {
  kATSFontFormatUnspecified     = 0
};

enum {
  kATSGenerationUnspecified     = 0,
  kATSFontContainerRefUnspecified = 0,
  kATSFontFamilyRefUnspecified  = 0,
  kATSFontRefUnspecified        = 0
};

/*
    ATSFontMetrics measurements are relative to a font's point size.
    For example, when a font with an ATSFontMetrics ascent of 0.6 is drawn at 18 points, its actual ascent is (0.6 * 18) = 10.8 points.
*/
struct ATSFontMetrics {
  UInt32              version;
  CGFloat             ascent;                 /* Maximum height above baseline reached by the glyphs in the font */
                                              /* or maximum distance to the right of the centerline reached by the glyphs in the font */
  CGFloat             descent;                /* Maximum depth below baseline reached by the glyphs in the font */
                                              /* or maximum distance to the left of the centerline reached by the glyphs in the font */
  CGFloat             leading;                /* Desired spacing between lines of text */
  CGFloat             avgAdvanceWidth;
  CGFloat             maxAdvanceWidth;        /* Maximum advance width or height of the glyphs in the font */
  CGFloat             minLeftSideBearing;     /* Minimum left or top side bearing */
  CGFloat             minRightSideBearing;    /* Minimum right or bottom side bearing */
  CGFloat             stemWidth;              /* Width of the dominant vertical stems of the glyphs in the font */
  CGFloat             stemHeight;             /* Vertical width of the dominant horizontal stems of glyphs in the font */
  CGFloat             capHeight;              /* Height of a capital letter from the baseline to the top of the letter */
  CGFloat             xHeight;                /* Height of lowercase characters in a font, specifically the letter x, excluding ascenders and descenders */
  CGFloat             italicAngle;            /* Angle in degrees counterclockwise from the vertical of the dominant vertical strokes of the glyphs in the font */
  CGFloat             underlinePosition;      /* Distance from the baseline for positioning underlining strokes */
  CGFloat             underlineThickness;     /* Stroke width for underlining */
};
typedef struct ATSFontMetrics           ATSFontMetrics;
enum {
  kATSItalicQDSkew              = (1 << 16) / 4, /* fixed value of 0.25 */
  kATSBoldQDStretch             = (1 << 16) * 3 / 2, /* fixed value of 1.50 */
  kATSRadiansFactor             = 1144  /* fixed value of approx. pi/180 (0.0174560546875) */
};

/* Glyph outline path constants used in ATSFontGetNativeCurveType. */
typedef UInt16 ATSCurveType;
enum {
  kATSCubicCurveType            = 0x0001,
  kATSQuadCurveType             = 0x0002,
  kATSOtherCurveType            = 0x0003
};

/* 
    This is what the ATSGlyphRef is set to when the glyph is deleted -
    that is, when the glyph is set to no longer appear when the layout
    is actually drawn
*/
enum {
  kATSDeletedGlyphcode          = 0xFFFF
};

struct ATSUCurvePath {
  UInt32              vectors;
  UInt32              controlBits[1];
  ATSPoint            vector[1];
};
typedef struct ATSUCurvePath            ATSUCurvePath;
struct ATSUCurvePaths {
  UInt32              contours;
  ATSUCurvePath       contour[1];
};
typedef struct ATSUCurvePaths           ATSUCurvePaths;
/* Glyph ideal metrics */
struct ATSGlyphIdealMetrics {
  ATSPoint            advance;
  ATSPoint            sideBearing;
  ATSPoint            otherSideBearing;
};
typedef struct ATSGlyphIdealMetrics     ATSGlyphIdealMetrics;
/* Glyph screen metrics */
struct ATSGlyphScreenMetrics {
  ATSPoint            deviceAdvance;
  ATSPoint            topLeft;
  UInt32              height;
  UInt32              width;
  ATSPoint            sideBearing;
  ATSPoint            otherSideBearing;
};
typedef struct ATSGlyphScreenMetrics    ATSGlyphScreenMetrics;
/* Glyph References */

typedef ATSGlyphRef                     GlyphID;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __ATSTYPES__ */

