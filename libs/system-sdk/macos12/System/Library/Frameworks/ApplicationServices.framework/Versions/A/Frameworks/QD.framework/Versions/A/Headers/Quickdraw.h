/*
     File:       QD/Quickdraw.h
 
     Contains:   Interface to Quickdraw Graphics
 
     Version:    Quickdraw-291~1
 
     Copyright:  © 1985-2008 by Apple, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QUICKDRAW__
#define __QUICKDRAW__

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#ifndef __COREGRAPHICS__
#include <CoreGraphics/CoreGraphics.h>
#endif



#include <AvailabilityMacros.h>

#include <QD/ColorSyncDeprecated.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

#define QD_HEADERS_ARE_PRIVATE 1
typedef struct OpaqueGrafPtr*           GrafPtr;
typedef GrafPtr                         CGrafPtr;
typedef CGrafPtr                        GWorldPtr;
typedef short                           QDErr;
typedef unsigned long                   GWorldFlags;

/* For the convenience of QuickTime */
enum {
  srcCopy                       = 0
};

/* needed in ImageCompression.h */
/* Needed in HIServices/Icons.i */
struct BitMap {
  Ptr                 baseAddr;
  short               rowBytes;
  Rect                bounds;
};
typedef struct BitMap                   BitMap;
typedef BitMap *                        BitMapPtr;
typedef BitMapPtr *                     BitMapHandle;
struct RGBColor {
  unsigned short      red;
  unsigned short      green;
  unsigned short      blue;
};
typedef struct RGBColor                 RGBColor;
struct ColorSpec {
  short               value;
  RGBColor            rgb;
};
typedef struct ColorSpec                ColorSpec;
typedef ColorSpec *                     ColorSpecPtr;
typedef ColorSpec                       CSpecArray[1];
struct ColorTable {
  SInt32              ctSeed;
  short               ctFlags;
  short               ctSize;
  CSpecArray          ctTable;
};
typedef struct ColorTable               ColorTable;
typedef ColorTable *                    CTabPtr;
typedef CTabPtr *                       CTabHandle;
struct PixMap {
  Ptr                 baseAddr;               /* pointer to pixels */
  short               rowBytes;               /* offset to next line */
  Rect                bounds;                 /* encloses bitmap */
  short               pmVersion;              /* pixMap version number */
  short               packType;               /* defines packing format */
  SInt32              packSize;               /* length of pixel data */
  Fixed               hRes;                   /* horiz. resolution (ppi) */
  Fixed               vRes;                   /* vert. resolution (ppi) */
  short               pixelType;              /* defines pixel type */
  short               pixelSize;              /* # bits in pixel */
  short               cmpCount;               /* # components in pixel */
  short               cmpSize;                /* # bits per component */
  OSType              pixelFormat;            /* fourCharCode representation */
  CTabHandle          pmTable;                /* color map for this pixMap */
  void *              pmExt;                  /* Handle to pixMap extension */
};
typedef struct PixMap                   PixMap;
typedef PixMap *                        PixMapPtr;
typedef PixMapPtr *                     PixMapHandle;

#define GETPIXMAPPIXELFORMAT(pm)   ( ((pm)->pixelFormat != 0) ? (UInt32)(pm)->pixelFormat : (UInt32)(pm)->pixelSize )

/* Values for PixMap.pixelFormat (BE = Big Endian, LE = Little Endian)*/
enum {
                                        /* The original pixel formats supported by 32-bit Color Quickdraw*/
  k1MonochromePixelFormat       = 0x00000001, /* 1 bit indexed*/
  k2IndexedPixelFormat          = 0x00000002, /* 2 bit indexed*/
  k4IndexedPixelFormat          = 0x00000004, /* 4 bit indexed*/
  k8IndexedPixelFormat          = 0x00000008, /* 8 bit indexed*/
  k16BE555PixelFormat           = 0x00000010, /* 16 bit BE rgb 555 (Mac)*/
  k24RGBPixelFormat             = 0x00000018, /* 24 bit rgb */
  k32ARGBPixelFormat            = 0x00000020, /* 32 bit argb    (Mac)*/
                                        /* QuickTime additions*/
  k1IndexedGrayPixelFormat      = 0x00000021, /* 1 bit indexed gray*/
  k2IndexedGrayPixelFormat      = 0x00000022, /* 2 bit indexed gray*/
  k4IndexedGrayPixelFormat      = 0x00000024, /* 4 bit indexed gray*/
  k8IndexedGrayPixelFormat      = 0x00000028, /* 8 bit indexed gray*/
  k16LE555PixelFormat           = 'L555', /* 16 bit LE rgb 555 (PC)*/
  k16LE5551PixelFormat          = '5551', /* 16 bit LE rgb 5551*/
  k16BE565PixelFormat           = 'B565', /* 16 bit BE rgb 565*/
  k16LE565PixelFormat           = 'L565', /* 16 bit LE rgb 565*/
  k24BGRPixelFormat             = '24BG', /* 24 bit bgr */
  k32BGRAPixelFormat            = 'BGRA', /* 32 bit bgra    (Matrox)*/
  k32ABGRPixelFormat            = 'ABGR', /* 32 bit abgr    */
  k32RGBAPixelFormat            = 'RGBA', /* 32 bit rgba    */
  kYUVSPixelFormat              = 'yuvs', /* YUV 4:2:2 byte ordering 16-unsigned = 'YUY2'*/
  kYUVUPixelFormat              = 'yuvu', /* YUV 4:2:2 byte ordering 16-signed*/
  kYVU9PixelFormat              = 'YVU9', /* YVU9 Planar    9*/
  kYUV411PixelFormat            = 'Y411', /* YUV 4:1:1 Interleaved  16*/
  kYVYU422PixelFormat           = 'YVYU', /* YVYU 4:2:2 byte ordering   16*/
  kUYVY422PixelFormat           = 'UYVY', /* UYVY 4:2:2 byte ordering   16*/
  kYUV211PixelFormat            = 'Y211', /* YUV 2:1:1 Packed   8*/
  k2vuyPixelFormat              = '2vuy' /* UYVY 4:2:2 byte ordering   16*/
};


struct Pattern {
  UInt8               pat[8];
};
typedef struct Pattern                  Pattern;
typedef Pattern *                       PatPtr;
typedef PatPtr *                        PatHandle;
struct PixPat {
  short               patType;                /*type of pattern*/
  PixMapHandle        patMap;                 /*the pattern's pixMap*/
  Handle              patData;                /*pixmap's data*/
  Handle              patXData;               /*expanded Pattern data*/
  short               patXValid;              /*flags whether expanded Pattern valid*/
  Handle              patXMap;                /*Handle to expanded Pattern data*/
  Pattern             pat1Data;               /*old-Style pattern/RGB color*/
};
typedef struct PixPat                   PixPat;
typedef PixPat *                        PixPatPtr;
typedef PixPatPtr *                     PixPatHandle;
typedef struct OpaqueRgnHandle*         RgnHandle;
typedef struct GDevice                  GDevice;
typedef GDevice *                       GDPtr;
typedef GDPtr *                         GDHandle;
struct GDevice {
  short               gdRefNum;               /*driver's unit number*/
  short               gdID;                   /*client ID for search procs*/
  short               gdType;                 /*fixed/CLUT/direct*/
  Handle              gdITable;               /*Handle to inverse lookup table*/
  short               gdResPref;              /*preferred resolution of GDITable*/
  Handle              gdSearchProc;           /*search proc list head*/
  Handle              gdCompProc;             /*complement proc list*/
  short               gdFlags;                /*grafDevice flags word*/
  PixMapHandle        gdPMap;                 /*describing pixMap*/
  SInt32              gdRefCon;               /*reference value*/
  GDHandle            gdNextGD;               /*GDHandle Handle of next gDevice*/
  Rect                gdRect;                 /* device's bounds in global coordinates*/
  SInt32              gdMode;                 /*device's current mode*/
  short               gdCCBytes;              /*depth of expanded cursor data*/
  short               gdCCDepth;              /*depth of expanded cursor data*/
  Handle              gdCCXData;              /*Handle to cursor's expanded data*/
  Handle              gdCCXMask;              /*Handle to cursor's expanded mask*/
  Handle              gdExt;                  /*QuickTime 3.0 private info*/
};

struct Picture {
  short               picSize;
  Rect                picFrame;
};
typedef struct Picture                  Picture;
typedef Picture *                       PicPtr;
typedef PicPtr *                        PicHandle;
struct OpenCPicParams {
  Rect                srcRect;
  Fixed               hRes;
  Fixed               vRes;
  short               version;
  short               reserved1;
  SInt32              reserved2;
};
typedef struct OpenCPicParams           OpenCPicParams;
struct FontInfo {
  short               ascent;
  short               descent;
  short               widMax;
  short               leading;
};
typedef struct FontInfo                 FontInfo;

typedef SInt32                          QDRegionParseDirection;
typedef CALLBACK_API_C( OSStatus , RegionToRectsProcPtr )(UInt16 message, RgnHandle rgn, const Rect *rect, void *refCon);
typedef STACK_UPP_TYPE(RegionToRectsProcPtr)                    RegionToRectsUPP;


typedef short                           TruncCode;
/* DragConstraint is needed in Controls.h */
typedef UInt16                          DragConstraint;
enum {
  kNoConstraint                 = 0,
  kVerticalConstraint           = 1,
  kHorizontalConstraint         = 2
};



typedef CALLBACK_API( void , DragGrayRgnProcPtr )(void);
typedef CALLBACK_API( Boolean , ColorSearchProcPtr )(RGBColor *rgb, long *position);
typedef CALLBACK_API( Boolean , ColorComplementProcPtr )(RGBColor * rgb);
typedef STACK_UPP_TYPE(DragGrayRgnProcPtr)                      DragGrayRgnUPP;
typedef STACK_UPP_TYPE(ColorSearchProcPtr)                      ColorSearchUPP;
typedef STACK_UPP_TYPE(ColorComplementProcPtr)                  ColorComplementUPP;

/*
 *  This set of definitions "belongs" in MacWindows.
 *  But, there is a circularity in the headers where MacWindows includes Controls
 *  and Controls includes MacWindows. To break the circle, the information
 *  needed by Controls is moved from MacWindows to Quickdraw.
 */
typedef struct OpaqueWindowPtr*         WindowPtr;
typedef struct OpaqueDialogPtr*         DialogPtr;
typedef WindowPtr                       WindowRef;
/*
 * Something for Quickdraw.r (needed by Icons.r etc. in HIToolbox):
 */



/* Temporary, until QuickTime resolves rdar://7359491 */
struct VDGammaRecord {
  Ptr                 csGTable;               /*pointer to gamma table*/
};
typedef struct VDGammaRecord            VDGammaRecord;
typedef VDGammaRecord *                 VDGamRecPtr;
/* CQDProcs needed by QTKit.framework/.../QTUtilities_Private.h */
/* CQDProcs requires all the following */
struct MacPolygon {
  short               polySize;
  Rect                polyBBox;
  Point               polyPoints[1];
};
typedef struct MacPolygon               MacPolygon;
/*
   The type name "Polygon" has a name space collision on Win32.
   Use MacPolygon to be cross-platfrom safe.
*/
typedef MacPolygon                      Polygon;
typedef MacPolygon *                    PolyPtr;
typedef PolyPtr *                       PolyHandle;
typedef SInt8                           GrafVerb;
typedef SInt32                          PrinterStatusOpcode;
typedef CALLBACK_API( void , QDTextProcPtr )(short byteCount, const void *textBuf, Point numer, Point denom);
typedef CALLBACK_API( void , QDLineProcPtr )(Point newPt);
typedef CALLBACK_API( void , QDRectProcPtr )(GrafVerb verb, const Rect *r);
typedef CALLBACK_API( void , QDRRectProcPtr )(GrafVerb verb, const Rect *r, short ovalWidth, short ovalHeight);
typedef CALLBACK_API( void , QDOvalProcPtr )(GrafVerb verb, const Rect *r);
typedef CALLBACK_API( void , QDArcProcPtr )(GrafVerb verb, const Rect *r, short startAngle, short arcAngle);
typedef CALLBACK_API( void , QDPolyProcPtr )(GrafVerb verb, PolyHandle poly);
typedef CALLBACK_API( void , QDRgnProcPtr )(GrafVerb verb, RgnHandle rgn);
typedef CALLBACK_API( void , QDBitsProcPtr )(const BitMap *srcBits, const Rect *srcRect, const Rect *dstRect, short mode, RgnHandle maskRgn);
typedef CALLBACK_API( void , QDCommentProcPtr )(short kind, short dataSize, Handle dataHandle);
typedef CALLBACK_API( short , QDTxMeasProcPtr )(short byteCount, const void *textAddr, Point *numer, Point *denom, FontInfo *info);
typedef CALLBACK_API( void , QDGetPicProcPtr )(void *dataPtr, short byteCount);
typedef CALLBACK_API( void , QDPutPicProcPtr )(const void *dataPtr, short byteCount);
typedef CALLBACK_API( void , QDOpcodeProcPtr )(const Rect *fromRect, const Rect *toRect, UInt16 opcode, SInt16 version);
/* The following is unused on Mac OS X - ignore it! */
typedef CALLBACK_API_C( OSStatus , QDStdGlyphsProcPtr )(void *dataStream, ByteCount size);
typedef CALLBACK_API( void , QDJShieldCursorProcPtr )(short left, short top, short right, short bottom);
typedef CALLBACK_API_C( OSStatus , QDPrinterStatusProcPtr )(PrinterStatusOpcode opcode, CGrafPtr currentPort, void *printerStatus);
typedef STACK_UPP_TYPE(QDTextProcPtr)                           QDTextUPP;
typedef STACK_UPP_TYPE(QDLineProcPtr)                           QDLineUPP;
typedef STACK_UPP_TYPE(QDRectProcPtr)                           QDRectUPP;
typedef STACK_UPP_TYPE(QDRRectProcPtr)                          QDRRectUPP;
typedef STACK_UPP_TYPE(QDOvalProcPtr)                           QDOvalUPP;
typedef STACK_UPP_TYPE(QDArcProcPtr)                            QDArcUPP;
typedef STACK_UPP_TYPE(QDPolyProcPtr)                           QDPolyUPP;
typedef STACK_UPP_TYPE(QDRgnProcPtr)                            QDRgnUPP;
typedef STACK_UPP_TYPE(QDBitsProcPtr)                           QDBitsUPP;
typedef STACK_UPP_TYPE(QDCommentProcPtr)                        QDCommentUPP;
typedef STACK_UPP_TYPE(QDTxMeasProcPtr)                         QDTxMeasUPP;
typedef STACK_UPP_TYPE(QDGetPicProcPtr)                         QDGetPicUPP;
typedef STACK_UPP_TYPE(QDPutPicProcPtr)                         QDPutPicUPP;
typedef STACK_UPP_TYPE(QDOpcodeProcPtr)                         QDOpcodeUPP;
typedef STACK_UPP_TYPE(QDStdGlyphsProcPtr)                      QDStdGlyphsUPP;
typedef STACK_UPP_TYPE(QDJShieldCursorProcPtr)                  QDJShieldCursorUPP;
typedef STACK_UPP_TYPE(QDPrinterStatusProcPtr)                  QDPrinterStatusUPP;


struct CQDProcs {
  QDTextUPP           textProc;
  QDLineUPP           lineProc;
  QDRectUPP           rectProc;
  QDRRectUPP          rRectProc;
  QDOvalUPP           ovalProc;
  QDArcUPP            arcProc;
  QDPolyUPP           polyProc;
  QDRgnUPP            rgnProc;
  QDBitsUPP           bitsProc;
  QDCommentUPP        commentProc;
  QDTxMeasUPP         txMeasProc;
  QDGetPicUPP         getPicProc;
  QDPutPicUPP         putPicProc;
  QDOpcodeUPP         opcodeProc;
  UniversalProcPtr    newProc1;               /* this is the StdPix bottleneck -- see ImageCompression.h */
  QDStdGlyphsUPP      glyphsProc;             /* unused on Mac OS X */
  QDPrinterStatusUPP  printerStatusProc;      /* was newProc3;  now used to communicate status between Printing code and System imaging code */
  UniversalProcPtr    newProc4;
  UniversalProcPtr    newProc5;
  UniversalProcPtr    newProc6;
};
typedef struct CQDProcs                 CQDProcs;
typedef CQDProcs *                      CQDProcsPtr;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Temporary until HIToolbox gets fixed */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct GrafPort {
  short               whatever[87];
};
typedef struct GrafPort                 GrafPort;
typedef GrafPort                        CGrafPort;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __QUICKDRAW__ */

