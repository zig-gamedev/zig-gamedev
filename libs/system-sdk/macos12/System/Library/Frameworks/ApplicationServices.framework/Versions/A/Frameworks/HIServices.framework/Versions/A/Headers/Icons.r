/*
     File:       Icons.r
 
     Contains:   Icon Utilities and Icon Services Interfaces.
 
     Copyright:  © 1990-2012 by Apple Computer, Inc. All rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/

#ifndef __ICONS_R__
#define __ICONS_R__

#ifndef __QUICKDRAW_R__
#include <QD/Quickdraw.r>
#endif

#ifndef __CORESERVICES_R__
#include <CoreServices/CoreServices.r>
#endif

#define kAlignNone 						0x00
#define kAlignVerticalCenter 			0x01
#define kAlignTop 						0x02
#define kAlignBottom 					0x03
#define kAlignHorizontalCenter 			0x04
#define kAlignAbsoluteCenter 			0x05
#define kAlignCenterTop 				0x06
#define kAlignCenterBottom 				0x07
#define kAlignLeft 						0x08
#define kAlignCenterLeft 				0x09
#define kAlignTopLeft 					0x0A
#define kAlignBottomLeft 				0x0B
#define kAlignRight 					0x0C
#define kAlignCenterRight 				0x0D
#define kAlignTopRight 					0x0E
#define kAlignBottomRight 				0x0F

#define atNone 							0x00
#define atVerticalCenter 				0x01
#define atTop 							0x02
#define atBottom 						0x03
#define atHorizontalCenter 				0x04
#define atAbsoluteCenter 				0x05
#define atCenterTop 					0x06
#define atCenterBottom 					0x07
#define atLeft 							0x08
#define atCenterLeft 					0x09
#define atTopLeft 						0x0A
#define atBottomLeft 					0x0B
#define atRight 						0x0C
#define atCenterRight 					0x0D
#define atTopRight 						0x0E
#define atBottomRight 					0x0F

#define kTransformNone 					0x00
#define kTransformDisabled 				0x01
#define kTransformOffline 				0x02
#define kTransformOpen 					0x03
#define kTransformLabel1 				0x0100
#define kTransformLabel2 				0x0200
#define kTransformLabel3 				0x0300
#define kTransformLabel4 				0x0400
#define kTransformLabel5 				0x0500
#define kTransformLabel6 				0x0600
#define kTransformLabel7 				0x0700
#define kTransformSelected 				0x4000
#define kTransformSelectedDisabled 		0x4001
#define kTransformSelectedOffline 		0x4002
#define kTransformSelectedOpen 			0x4003

#define ttNone 							0x00
#define ttDisabled 						0x01
#define ttOffline 						0x02
#define ttOpen 							0x03
#define ttLabel1 						0x0100
#define ttLabel2 						0x0200
#define ttLabel3 						0x0300
#define ttLabel4 						0x0400
#define ttLabel5 						0x0500
#define ttLabel6 						0x0600
#define ttLabel7 						0x0700
#define ttSelected 						0x4000
#define ttSelectedDisabled 				0x4001
#define ttSelectedOffline 				0x4002
#define ttSelectedOpen 					0x4003

#define kSelectorLarge1Bit 				0x00000001
#define kSelectorLarge4Bit 				0x00000002
#define kSelectorLarge8Bit 				0x00000004
#define kSelectorLarge32Bit 			0x00000008
#define kSelectorLarge8BitMask 			0x00000010
#define kSelectorSmall1Bit 				0x00000100
#define kSelectorSmall4Bit 				0x00000200
#define kSelectorSmall8Bit 				0x00000400
#define kSelectorSmall32Bit 			0x00000800
#define kSelectorSmall8BitMask 			0x00001000
#define kSelectorMini1Bit 				0x00010000
#define kSelectorMini4Bit 				0x00020000
#define kSelectorMini8Bit 				0x00040000
#define kSelectorHuge1Bit 				0x01000000
#define kSelectorHuge4Bit 				0x02000000
#define kSelectorHuge8Bit 				0x04000000
#define kSelectorHuge32Bit 				0x08000000
#define kSelectorHuge8BitMask 			0x10000000
#define kSelectorAllLargeData 			0x000000FF
#define kSelectorAllSmallData 			0x0000FF00
#define kSelectorAllMiniData 			0x00FF0000
#define kSelectorAllHugeData 			0xFF000000
#define kSelectorAll1BitData 			0x01010101
#define kSelectorAll4BitData 			0x02020202
#define kSelectorAll8BitData 			0x04040404
#define kSelectorAll32BitData 			0x08000808
#define kSelectorAllAvailableData 		0xFFFFFFFF

#define svLarge1Bit 					0x00000001
#define svLarge4Bit 					0x00000002
#define svLarge8Bit 					0x00000004
#define svSmall1Bit 					0x00000100
#define svSmall4Bit 					0x00000200
#define svSmall8Bit 					0x00000400
#define svMini1Bit 						0x00010000
#define svMini4Bit 						0x00020000
#define svMini8Bit 						0x00040000
#define svAllLargeData 					0x000000FF
#define svAllSmallData 					0x0000FF00
#define svAllMiniData 					0x00FF0000
#define svAll1BitData 					0x01010101
#define svAll4BitData 					0x02020202
#define svAll8BitData 					0x04040404
#define svAllAvailableData 				0xFFFFFFFF

#define kPlotIconRefNormalFlags 		0
#define kPlotIconRefNoImage 			0x02
#define kPlotIconRefNoMask 				0x04


/*-----------------------------------cicn ¥ Color Icon -------------------------------------------*/
/*
   cicn_RezTemplateVersion:
       0 - original 
      1 - auto calculate much of internal data                <-- default
*/
#ifndef cicn_RezTemplateVersion
 #ifdef oldTemp                          /* grandfather in use of ÒoldTempÓ */
      #define cicn_RezTemplateVersion 0
  #else
      #define cicn_RezTemplateVersion 1
  #endif
#endif


type 'cicn' {
     /* IconPMap (pixMap) record */
     fill long;                                              /* Base address         */
     unsigned bitstring[1] = 1;                              /* New pixMap flag      */
     unsigned bitstring[2] = 0;                              /* Must be 0            */
#if cicn_RezTemplateVersion == 0
    pMapRowBytes:
      unsigned bitstring[13];                                 /* Offset to next row   */
 Bounds:
        rect;                                                   /* Bitmap bounds        */
     integer;                                                /* pixMap vers number   */
     integer unpacked;                                       /* Packing format       */
     unsigned longint;                                       /* Size of pixel data   */
     unsigned hex longint;                                   /* h. resolution (ppi) (fixed) */
      unsigned hex longint;                                   /* v. resolution (ppi) (fixed) */
      integer         chunky, chunkyPlanar, planar;           /* Pixel storage format */
     integer;                                                /* # bits in pixel      */
     integer;                                                /* # components in pixel*/
     integer;                                                /* # bits per field     */
     unsigned longint;                                       /* Offset to next plane */
     unsigned longint;                                       /* Offset to color table*/
     fill long;                                              /* Reserved             */

        /* IconMask (bitMap) record */
     fill long;                                              /* Base address         */
 maskRowBytes:
      integer;                                                /* Row bytes            */
     rect;                                                   /* Bitmap bounds        */


       /* IconBMap (bitMap) record */
     fill long;                                              /* Base address         */
 iconBMapRowBytes:
      integer;                                                /* Row bytes            */
     rect;                                                   /* Bitmap bounds        */

        fill long;                                              /* Handle placeholder   */

        /* Mask data */
        hex string [$$Word(maskRowBytes) * PIXMAPHEIGHT];

     /* BitMap data */
      hex string [$$Word(iconBMapRowBytes) * PIXMAPHEIGHT];

     /* Color Table */
      unsigned hex longint;                                   /* ctSeed               */
     integer;                                                /* ctFlags              */
     integer = $$Countof(ColorSpec) - 1;                     /* ctSize               */
     wide array ColorSpec {
             integer;                                        /* value                */
             unsigned integer;                               /* RGB: red             */
             unsigned integer;                               /*      green           */
             unsigned integer;                               /*      blue            */
     };

        /* PixelMap data */
        hex string [$$BitField(pMapRowBytes, 0, 13) * PIXMAPHEIGHT];


#else
 pMapRowBytes:
      unsigned bitstring[13] k32x32x4 = 16;                   /* Offset to next row   */
 Bounds: 
       rect;                                                   /* Bitmap bounds        */
     integer = 0;                                            /* pixMap vers number   */
     integer = 0;                                            /* Packing format       */
     fill long;                                              /* Size of pixel data   */
     unsigned hex longint = $00480000;                       /* h. resolution (ppi)  */
     unsigned hex longint = $00480000;                       /* v. resolution (ppi)  */
     integer = 0 /* chunky */;                               /* Pixel storage format */
 PixelSize:
     integer;                                                /* # bits in pixel      */
     integer = 1;                                            /* # components in pixel*/
     integer = $$Word(PixelSize);                            /* # bits per field     */
     fill long;
     fill long;
     fill long;

        /* IconMask (bitMap) record */
     fill long;                                              /* Base address         */
     integer = BitMapRowBytes;                               /* Row bytes            */
     PixMapBounds;                                           /* Bitmap bounds        */

        /* IconBMap (bitMap) record */
     fill long;                                              /* Base address         */
     integer = BitMapRowBytes;                               /* Row bytes            */
     PixMapBounds;                                           /* Bitmap bounds        */

        fill long;                                              /* Handle placeholder   */

        /* Mask data */
        hex string [BitMapRowBytes * PixMapHeight];

       /* BitMap data */
      hex string [BitMapRowBytes * PixMapHeight];

   ColorTable:
        unsigned hex longint = 0;                               /* ctSeed               */
     integer = 0;                                            /* ctFlags              */
     integer = $$Countof(ColorSpec) - 1;                     /* ctSize               */
     wide array ColorSpec {
             integer = $$ArrayIndex(ColorSpec) - 1;          /* value                */
             unsigned integer;                               /* RGB: red             */
             unsigned integer;                               /*      green           */
             unsigned integer;                               /*      blue            */
     };

    PixelData:
     hex string [PixMapRowBytes * PixMapHeight];             /* more of the pixmap   */
#endif
};


/*----------------------------ICON ¥ Icon-----------------------------------------------*/
type 'ICON' {
      hex string[128];                                        /* Icon data            */
};


/*----------------------------ICN# ¥ Icon List------------------------------------------*/
type 'ICN#' {
      array {
                hex string[128];                                /* Icon data            */
     };
};


/*----------------------------SICN ¥ Small Icon-----------------------------------------*/
type 'SICN' {
      array {
                hex string[32];                                 /* SICN data            */
     };
};


/*--------------------------ics# ¥ small icons with masks-------------------------------*/
type 'ics#' {
  array [2] {
        hex string[32];
    };
};


/*--------------------------icm# ¥ mini icons with masks--------------------------------*/
type 'icm#' {
  array [2] {
        hex string[24];
    };
};


/*--------------------------icm8 ¥ 8-bit mini icon no mask------------------------------*/
type 'icm8' {
  hex string[192];
};


/*--------------------------icm4 ¥ 4-bit mini icon no mask------------------------------*/
type 'icm4' {
  hex string[96];
};


/*--------------------------icl8 ¥ 8-bit large icon no mask-----------------------------*/
type 'icl8' {
  hex string[1024];
};


/*--------------------------icl4 ¥ 4-bit large icon no mask-----------------------------*/
type 'icl4' {
  hex string[512];
};


/*--------------------------ics8 ¥ 8-bit small icon no mask-----------------------------*/
type 'ics8' {
  hex string[256];
};


/*--------------------------ics4 ¥ 4-bit small icon no mask-----------------------------*/
type 'ics4' {
  hex string[128];
};


/*-------------------------------¥ Keyboard Icons---------------------------------------*/
type 'KCN#' as 'ICN#';                      
type 'kcs#' as 'ics#';                             /* Keyboard small icon */
type 'kcl8' as 'icl8';                                /* Keyboard 8-bit color icon */
type 'kcl4' as 'icl4';                              /* Keyboard 4-bit color icon */
type 'kcs8' as 'ics8';                              /* Keyboard 8-bit small color icon */
type 'kcs4' as 'ics4';                                /* Keyboard 4-bit small color icon */


/*--------------------------icns ¥ icon family-----------------------------*/
type 'icns' {
iconFamilyBegin:
  literal longint = 'icns';
  unsigned longint = (iconFamilyEnd - iconFamilyBegin) >> 3;
 array elementArray {
       literal longint;
#if derez
len:
        fill long;
     hex string[$$Long(len[$$ArrayIndex(elementArray)]) - 8];
#else
elementBegin:
       longint =   (elementEnd[$$ArrayIndex(elementArray)] -
                  elementBegin[$$ArrayIndex(elementArray)] >> 3) + 4;
        hex string;
elementEnd:
#endif
 };
iconFamilyEnd:
};


#endif /* __ICONS_R__ */

