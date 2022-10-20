/* ColorSync- ColorSyncDeprecated.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
*/


#ifndef __COLORSYNCDEPRECATED__
#define __COLORSYNCDEPRECATED__

#include <stddef.h>
#include <sys/types.h>

#ifndef __CORESERVICES__
#include <CoreServices/CoreServices.h>
#endif

#if TARGET_OS_OSX

#pragma mark --- CMBase.h ---

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(CSEXTERN)
    #if defined(_MSC_VER)
        #if defined(CG_BUILDING_CG)
            #define CSEXTERN __declspec(dllexport) extern
        #else
            #define CSEXTERN __declspec(dllimport) extern
        #endif
    #else
        #define CSEXTERN    __attribute__((visibility("default"))) extern
    #endif
#endif

#if !defined(CSEXPORT)
    #if defined(_MSC_VER)
        #if defined(CG_BUILDING_CG)
            #define CSEXPORT __declspec(dllexport)
        #else
            #define CSEXPORT __declspec(dllimport)
        #endif
    #else
        #define CSEXPORT    __attribute__((visibility("default")))
    #endif
#endif

#ifdef __cplusplus
}
#endif


#include <AvailabilityMacros.h>

#ifdef BUILDING_COLORSYNC
#ifdef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER
#undef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER
#endif
#define DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER
#endif

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark --- CMICCProfile.h ----

#pragma pack(push, 2)

/* ICC Profile version constants  */
enum {
  cmICCProfileVersion4          = 0x04000000,
  cmICCProfileVersion2          = 0x02000000,
  cmICCProfileVersion21         = 0x02100000,
  cmCS2ProfileVersion           = cmICCProfileVersion2,
  cmCS1ProfileVersion           = 0x00000100 /* ColorSync 1.0 profile version */
};

/* Current Major version number */
enum {
  cmProfileMajorVersionMask     = (int)0xFF000000,
  cmCurrentProfileMajorVersion  = 0x02000000
};

/* magic cookie number for anonymous file ID */
enum {
  cmMagicNumber                 = 'acsp'
};


/************************************************************************/
/*************** ColorSync 2.0 profile specification ********************/
/************************************************************************/
/**** flags field  ****/
enum {
  cmICCReservedFlagsMask        = 0x0000FFFF, /* these bits of the flags field are defined and reserved by ICC */
  cmEmbeddedMask                = 0x00000001, /* if bit 0 is 0 then not embedded profile, if 1 then embedded profile */
  cmEmbeddedUseMask             = 0x00000002, /* if bit 1 is 0 then ok to use anywhere, if 1 then ok to use as embedded profile only */
  cmBlackPointCompensationMask  = 0x00000004, /* if bit 2 is 1 then CMM will enable Black Point Compensation if applicable*/
  cmCMSReservedFlagsMask        = (int)0xFFFF0000, /* these bits of the flags field are defined and reserved by CMS vendor */
  cmQualityMask                 = 0x00030000, /* if bits 16-17 is 0 then normal, if 1 then draft, if 2 then best */
  cmInterpolationMask           = 0x00040000, /* if bit 18 is 0 then interpolation, if 1 then lookup only */
  cmGamutCheckingMask           = 0x00080000 /* if bit 19 is 0 then create gamut checking info, if 1 then no gamut checking info */
};

/* copyright-protection flag options */
enum {
  cmEmbeddedProfile             = 0,    /* 0 is not embedded profile, 1 is embedded profile */
  cmEmbeddedUse                 = 1     /* 0 is to use anywhere, 1 is to use as embedded profile only */
};

/* speed and quality flag options */
enum {
  cmNormalMode                  = 0,    /* it uses the least significent two bits in the high word of flag */
  cmDraftMode                   = 1,    /* it should be evaulated like this: right shift 16 bits first, mask off the */
  cmBestMode                    = 2     /* high 14 bits, and then compare with the enum to determine the option value */
};

/* black point compensation flag option */
enum {
  cmBlackPointCompensation      = 1     /* 0 do not apply Black Point Compensation, 1 apply */
};


/**** deviceAttributes fields ****/
/* deviceAttributes[0] is defined by and reserved for device vendors */
/* deviceAttributes[1] is defined by and reserved for ICC */
/* The following bits of deviceAttributes[1] are currently defined */
enum {
  cmReflectiveTransparentMask   = 0x00000001, /* if bit 0 is 0 then reflective media, if 1 then transparency media */
  cmGlossyMatteMask             = 0x00000002 /* if bit 1 is 0 then glossy, if 1 then matte */
};

/* device/media attributes element values  */
enum {
  cmReflective                  = 0,    /* if bit 0 is 0 then reflective media, if 1 then transparency media */
  cmGlossy                      = 1     /* if bit 1 is 0 then glossy, if 1 then matte */
};


/**** renderingIntent field ****/
enum {
  cmPerceptual                  = 0,    /* Photographic images */
  cmRelativeColorimetric        = 1,    /* Logo Colors */
  cmSaturation                  = 2,    /* Business graphics */
  cmAbsoluteColorimetric        = 3     /* Logo Colors */
};



/* data type element values */
enum {
  cmAsciiData                   = 0,
  cmBinaryData                  = 1
};

/* screen encodings  */
enum {
  cmPrtrDefaultScreens          = 0,    /* Use printer default screens.  0 is false, 1 is ture */
  cmLinesPer                    = 1     /* 0 is LinesPerCm, 1 is LinesPerInch */
};

/* 2.0 tag type information */
enum {
  cmNumHeaderElements           = 10
};

/* public tags */
enum {
  cmAToB0Tag                    = 'A2B0',
  cmAToB1Tag                    = 'A2B1',
  cmAToB2Tag                    = 'A2B2',
  cmBlueColorantTag             = 'bXYZ',
  cmBlueTRCTag                  = 'bTRC',
  cmBToA0Tag                    = 'B2A0',
  cmBToA1Tag                    = 'B2A1',
  cmBToA2Tag                    = 'B2A2',
  cmCalibrationDateTimeTag      = 'calt',
  cmChromaticAdaptationTag      = 'chad',
  cmCharTargetTag               = 'targ',
  cmCopyrightTag                = 'cprt',
  cmDeviceMfgDescTag            = 'dmnd',
  cmDeviceModelDescTag          = 'dmdd',
  cmGamutTag                    = 'gamt',
  cmGrayTRCTag                  = 'kTRC',
  cmGreenColorantTag            = 'gXYZ',
  cmGreenTRCTag                 = 'gTRC',
  cmLuminanceTag                = 'lumi',
  cmMeasurementTag              = 'meas',
  cmMediaBlackPointTag          = 'bkpt',
  cmMediaWhitePointTag          = 'wtpt',
  cmNamedColorTag               = 'ncol',
  cmNamedColor2Tag              = 'ncl2',
  cmPreview0Tag                 = 'pre0',
  cmPreview1Tag                 = 'pre1',
  cmPreview2Tag                 = 'pre2',
  cmProfileDescriptionTag       = 'desc',
  cmProfileSequenceDescTag      = 'pseq',
  cmPS2CRD0Tag                  = 'psd0',
  cmPS2CRD1Tag                  = 'psd1',
  cmPS2CRD2Tag                  = 'psd2',
  cmPS2CRD3Tag                  = 'psd3',
  cmPS2CSATag                   = 'ps2s',
  cmPS2RenderingIntentTag       = 'ps2i',
  cmRedColorantTag              = 'rXYZ',
  cmRedTRCTag                   = 'rTRC',
  cmScreeningDescTag            = 'scrd',
  cmScreeningTag                = 'scrn',
  cmTechnologyTag               = 'tech',
  cmUcrBgTag                    = 'bfd ',
  cmViewingConditionsDescTag    = 'vued',
  cmViewingConditionsTag        = 'view'
};

/* custom tags */
enum {
  cmPS2CRDVMSizeTag             = 'psvm',
  cmVideoCardGammaTag           = 'vcgt',
  cmMakeAndModelTag             = 'mmod',
  cmProfileDescriptionMLTag     = 'dscm',
  cmNativeDisplayInfoTag        = 'ndin'
};

/* public type signatures */
enum {
  cmSigCrdInfoType              = 'crdi',
  cmSigCurveType                = 'curv',
  cmSigDataType                 = 'data',
  cmSigDateTimeType             = 'dtim',
  cmSigLut16Type                = 'mft2',
  cmSigLut8Type                 = 'mft1',
  cmSigMeasurementType          = 'meas',
  cmSigMultiFunctA2BType        = 'mAB ',
  cmSigMultiFunctB2AType        = 'mBA ',
  cmSigNamedColorType           = 'ncol',
  cmSigNamedColor2Type          = 'ncl2',
  cmSigParametricCurveType      = 'para',
  cmSigProfileDescriptionType   = 'desc',
  cmSigProfileSequenceDescType  = 'pseq',
  cmSigScreeningType            = 'scrn',
  cmSigS15Fixed16Type           = 'sf32',
  cmSigSignatureType            = 'sig ',
  cmSigTextType                 = 'text',
  cmSigU16Fixed16Type           = 'uf32',
  cmSigU1Fixed15Type            = 'uf16',
  cmSigUInt8Type                = 'ui08',
  cmSigUInt16Type               = 'ui16',
  cmSigUInt32Type               = 'ui32',
  cmSigUInt64Type               = 'ui64',
  cmSigUcrBgType                = 'bfd ',
  cmSigUnicodeTextType          = 'utxt',
  cmSigViewingConditionsType    = 'view',
  cmSigXYZType                  = 'XYZ '
};

/* custom type signatures */
enum {
  cmSigPS2CRDVMSizeType         = 'psvm',
  cmSigVideoCardGammaType       = 'vcgt',
  cmSigMakeAndModelType         = 'mmod',
  cmSigNativeDisplayInfoType    = 'ndin',
  cmSigMultiLocalizedUniCodeType = 'mluc'
};


/* technology tag descriptions */
enum {
  cmTechnologyDigitalCamera     = 'dcam',
  cmTechnologyFilmScanner       = 'fscn',
  cmTechnologyReflectiveScanner = 'rscn',
  cmTechnologyInkJetPrinter     = 'ijet',
  cmTechnologyThermalWaxPrinter = 'twax',
  cmTechnologyElectrophotographicPrinter = 'epho',
  cmTechnologyElectrostaticPrinter = 'esta',
  cmTechnologyDyeSublimationPrinter = 'dsub',
  cmTechnologyPhotographicPaperPrinter = 'rpho',
  cmTechnologyFilmWriter        = 'fprn',
  cmTechnologyVideoMonitor      = 'vidm',
  cmTechnologyVideoCamera       = 'vidc',
  cmTechnologyProjectionTelevision = 'pjtv',
  cmTechnologyCRTDisplay        = 'CRT ',
  cmTechnologyPMDisplay         = 'PMD ',
  cmTechnologyAMDisplay         = 'AMD ',
  cmTechnologyPhotoCD           = 'KPCD',
  cmTechnologyPhotoImageSetter  = 'imgs',
  cmTechnologyGravure           = 'grav',
  cmTechnologyOffsetLithography = 'offs',
  cmTechnologySilkscreen        = 'silk',
  cmTechnologyFlexography       = 'flex'
};


/* Measurement type encodings */
/* Measurement Flare */
enum {
  cmFlare0                      = 0x00000000,
  cmFlare100                    = 0x00000001
};

/* Measurement Geometry */
enum {
  cmGeometryUnknown             = 0x00000000,
  cmGeometry045or450            = 0x00000001,
  cmGeometry0dord0              = 0x00000002
};

/* Standard Observer    */
enum {
  cmStdobsUnknown               = 0x00000000,
  cmStdobs1931TwoDegrees        = 0x00000001,
  cmStdobs1964TenDegrees        = 0x00000002
};

/* Standard Illuminant */
enum {
  cmIlluminantUnknown           = 0x00000000,
  cmIlluminantD50               = 0x00000001,
  cmIlluminantD65               = 0x00000002,
  cmIlluminantD93               = 0x00000003,
  cmIlluminantF2                = 0x00000004,
  cmIlluminantD55               = 0x00000005,
  cmIlluminantA                 = 0x00000006,
  cmIlluminantEquiPower         = 0x00000007,
  cmIlluminantF8                = 0x00000008
};

/* Spot Function Value */
enum {
  cmSpotFunctionUnknown         = 0,
  cmSpotFunctionDefault         = 1,
  cmSpotFunctionRound           = 2,
  cmSpotFunctionDiamond         = 3,
  cmSpotFunctionEllipse         = 4,
  cmSpotFunctionLine            = 5,
  cmSpotFunctionSquare          = 6,
  cmSpotFunctionCross           = 7
};

/* Color Space Signatures */
enum {
  cmXYZData                     = 'XYZ ',
  cmLabData                     = 'Lab ',
  cmLuvData                     = 'Luv ',
  cmYCbCrData                   = 'YCbr',
  cmYxyData                     = 'Yxy ',
  cmRGBData                     = 'RGB ',
  cmSRGBData                    = 'sRGB',
  cmGrayData                    = 'GRAY',
  cmHSVData                     = 'HSV ',
  cmHLSData                     = 'HLS ',
  cmCMYKData                    = 'CMYK',
  cmCMYData                     = 'CMY ',
  cmMCH5Data                    = 'MCH5',
  cmMCH6Data                    = 'MCH6',
  cmMCH7Data                    = 'MCH7',
  cmMCH8Data                    = 'MCH8',
  cm3CLRData                    = '3CLR',
  cm4CLRData                    = '4CLR',
  cm5CLRData                    = '5CLR',
  cm6CLRData                    = '6CLR',
  cm7CLRData                    = '7CLR',
  cm8CLRData                    = '8CLR',
  cm9CLRData                    = '9CLR',
  cm10CLRData                   = 'ACLR',
  cm11CLRData                   = 'BCLR',
  cm12CLRData                   = 'CCLR',
  cm13CLRData                   = 'DCLR',
  cm14CLRData                   = 'ECLR',
  cm15CLRData                   = 'FCLR',
  cmNamedData                   = 'NAME'
};

/* profileClass enumerations */
enum {
  cmInputClass                  = 'scnr',
  cmDisplayClass                = 'mntr',
  cmOutputClass                 = 'prtr',
  cmLinkClass                   = 'link',
  cmAbstractClass               = 'abst',
  cmColorSpaceClass             = 'spac',
  cmNamedColorClass             = 'nmcl'
};

/* platform enumerations */
enum {
  cmMacintosh                   = 'APPL',
  cmMicrosoft                   = 'MSFT',
  cmSolaris                     = 'SUNW',
  cmSiliconGraphics             = 'SGI ',
  cmTaligent                    = 'TGNT'
};

/* parametric curve type enumerations */
enum {
  cmParametricType0             = 0,    /* Y = X^gamma */
  cmParametricType1             = 1,    /* Y = (aX+b)^gamma     [X>=-b/a],  Y = 0    [X<-b/a] */
  cmParametricType2             = 2,    /* Y = (aX+b)^gamma + c [X>=-b/a],  Y = c    [X<-b/a] */
  cmParametricType3             = 3,    /* Y = (aX+b)^gamma     [X>=d],     Y = cX   [X<d]    */
  cmParametricType4             = 4     /* Y = (aX+b)^gamma + e [X>=d],     Y = cX+f [X<d]    */
};


/* ColorSync 1.0 elements */
enum {
  cmCS1ChromTag                 = 'chrm',
  cmCS1TRCTag                   = 'trc ',
  cmCS1NameTag                  = 'name',
  cmCS1CustTag                  = 'cust'
};

/* General element data types */
typedef struct CMDateTime {
  UInt16              year;
  UInt16              month;
  UInt16              dayOfTheMonth;
  UInt16              hours;
  UInt16              minutes;
  UInt16              seconds;
} CMDateTime DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMFixedXYColor {
  Fixed               x;
  Fixed               y;
} CMFixedXYColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMFixedXYZColor {
  Fixed               X;
  Fixed               Y;
  Fixed               Z;
} CMFixedXYZColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef UInt16                          CMXYZComponent;

typedef struct CMXYZColor {
  CMXYZComponent      X;
  CMXYZComponent      Y;
  CMXYZComponent      Z;
} CMXYZColor;

/* Typedef for Profile MD5 message digest */
/* Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm */

typedef unsigned char                   CMProfileMD5[16] DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
typedef CMProfileMD5 *                  CMProfileMD5Ptr  DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
 *  CMProfileMD5AreEqual()
 *  
 *  Availability:       available as macro/inline
 */
#ifdef __cplusplus
  inline Boolean CMProfileMD5AreEqual(CMProfileMD5 a, CMProfileMD5 b)
  {
    return  ((UInt32*)a)[0]==((UInt32*)b)[0] && ((UInt32*)a)[1]==((UInt32*)b)[1] &&
            ((UInt32*)a)[2]==((UInt32*)b)[2] && ((UInt32*)a)[3]==((UInt32*)b)[3];
  }
#else
  #define CMProfileMD5AreEqual(a, b) (\
    ((UInt32*)a)[0]==((UInt32*)b)[0] && ((UInt32*)a)[1]==((UInt32*)b)[1] && \
    ((UInt32*)a)[2]==((UInt32*)b)[2] && ((UInt32*)a)[3]==((UInt32*)b)[3])
#endif

typedef struct CM2Header {
  UInt32              size;                   /* This is the total size of the Profile */
  OSType              CMMType;                /* CMM signature,  Registered with CS2 consortium  */
  UInt32              profileVersion;         /* Version of CMProfile format */
  OSType              profileClass;           /* input, display, output, devicelink, abstract, or color conversion profile type */
  OSType              dataColorSpace;         /* color space of data */
  OSType              profileConnectionSpace; /* profile connection color space */
  CMDateTime          dateTime;               /* date and time of profile creation */
  OSType              CS2profileSignature;    /* 'acsp' constant ColorSync 2.0 file ID */
  OSType              platform;               /* primary profile platform, Registered with CS2 consortium */
  UInt32              flags;                  /* profile flags */
  OSType              deviceManufacturer;     /* Registered with ICC consortium */
  UInt32              deviceModel;            /* Registered with ICC consortium */
  UInt32              deviceAttributes[2];    /* Attributes[0] is for device vendors, [1] is for ICC */
  UInt32              renderingIntent;        /* preferred rendering intent of tagged object */
  CMFixedXYZColor     white;                  /* profile illuminant */
  OSType              creator;                /* profile creator */
  char                reserved[44];           /* reserved for future use */
} CM2Header DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CM4Header {
  UInt32              size;                   /* This is the total size of the Profile */
  OSType              CMMType;                /* CMM signature,  Registered with CS2 consortium  */
  UInt32              profileVersion;         /* Version of CMProfile format */
  OSType              profileClass;           /* input, display, output, devicelink, abstract, or color conversion profile type */
  OSType              dataColorSpace;         /* color space of data */
  OSType              profileConnectionSpace; /* profile connection color space */
  CMDateTime          dateTime;               /* date and time of profile creation */
  OSType              CS2profileSignature;    /* 'acsp' constant ColorSync 2.0 file ID */
  OSType              platform;               /* primary profile platform, Registered with CS2 consortium */
  UInt32              flags;                  /* profile flags */
  OSType              deviceManufacturer;     /* Registered with ICC consortium */
  UInt32              deviceModel;            /* Registered with ICC consortium */
  UInt32              deviceAttributes[2];    /* Attributes[0] is for device vendors, [1] is for ICC */
  UInt32              renderingIntent;        /* preferred rendering intent of tagged object */
  CMFixedXYZColor     white;                  /* profile illuminant */
  OSType              creator;                /* profile creator */
  CMProfileMD5        digest;                 /* Profile message digest */
  char                reserved[28];           /* reserved for future use */
} CM4Header DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMTagRecord {
  OSType              tag;                    /* Registered with CS2 consortium */
  UInt32              elementOffset;          /* Relative to start of CMProfile */
  UInt32              elementSize;
} CMTagRecord DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMTagElemTable {
  UInt32              count;
  CMTagRecord         tagList[1];             /* variable size, determined by count */
} CMTagElemTable DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CM2Profile {
  CM2Header           header;
  CMTagElemTable      tagTable;
  char                elemData[1];            /* variable size data for tagged element storage */
} CM2Profile, *CM2ProfilePtr, **CM2ProfileHandle DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Tag Type Definitions */
typedef struct CMAdaptationMatrixType {
  OSType              typeDescriptor;         /* 'sf32' = cmSigS15Fixed16Type */
  UInt32              reserved;               /* fill with 0x00 */
  Fixed               adaptationMatrix[9];    /* fixed size of nine matrix entries */
} CMAdaptationMatrixType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMCurveType {
  OSType              typeDescriptor;         /* 'curv' = cmSigCurveType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              countValue;             /* number of entries in table that follows */
  UInt16              data[1];                /* variable size, determined by countValue */
} CMCurveType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMDataType {
  OSType              typeDescriptor;         /* 'data' = cmSigDataType*/
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              dataFlag;               /* 0 = ASCII, 1 = binary */
  char                data[1];                /* variable size, determined by tag element size */
} CMDataType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMDateTimeType {
  OSType              typeDescriptor;         /* 'dtim' = cmSigDateTimeType */
  UInt32              reserved;               /* fill with 0x00 */
  CMDateTime          dateTime;               /* */
} CMDateTimeType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMLut16Type {
  OSType              typeDescriptor;         /* 'mft2' = cmSigLut16Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt8               inputChannels;          /* Number of input channels */
  UInt8               outputChannels;         /* Number of output channels */
  UInt8               gridPoints;             /* Number of clutTable grid points */
  UInt8               reserved2;              /* fill with 0x00 */
  Fixed               matrix[3][3];           /* */
  UInt16              inputTableEntries;      /* Number of entries in 1-D input luts */
  UInt16              outputTableEntries;     /* Number of entries in 1-D output luts */
  UInt16              inputTable[1];          /* variable size, determined by inputChannels*inputTableEntries */
#if 0 /* NOTE: Field offsets are variable from here on. */
  /* In order to correctly reflect the actual format of this tag, some of the fields in */
  /* this structure have been removed because they follow an array field of variable size. */
  /* As a result, the size of this structure has changed from previous versions of this interface. */
  /* Code that relies on sizeof(CMLut16Type) should be changed. */
  UInt16              CLUT[];                 /* variable size, determined by (gridPoints^inputChannels)*outputChannels */
  UInt16              outputTable[];          /* variable size, determined by outputChannels*outputTableEntries */
#endif
} CMLut16Type DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMLut8Type {
  OSType              typeDescriptor;         /* 'mft1' = cmSigLut8Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt8               inputChannels;          /* Number of input channels */
  UInt8               outputChannels;         /* Number of output channels */
  UInt8               gridPoints;             /* Number of clutTable grid points */
  UInt8               reserved2;              /* fill with 0x00 */
  Fixed               matrix[3][3];           /* */
  UInt8               inputTable[1];          /* variable size, determined by inputChannels*256 */
#if 0 /* NOTE: Field offsets are variable from here on. */
  /* In order to correctly reflect the actual format of this tag, some of the fields in */
  /* this structure have been removed because they follow an array field of variable size. */
  /* As a result, the size of this structure has changed from previous versions of this interface. */
  /* Code that relies on sizeof(CMLut8Type) should be changed. */
  UInt8               CLUT[];                 /* variable size, determined by (gridPoints^inputChannels)*outputChannels */
  UInt8               outputTable[];          /* variable size, determined by outputChannels*256 */
#endif
} CMLut8Type DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultiFunctLutType {
  OSType              typeDescriptor;         /* 'mAB ' = cmSigMultiFunctA2BType or 'mBA ' = cmSigMultiFunctB2AType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt8               inputChannels;          /* Number of input channels */
  UInt8               outputChannels;         /* Number of output channels */
  UInt16              reserved2;              /* fill with 0x00 */
  UInt32              offsetBcurves;          /* offset to first "B" curve */
  UInt32              offsetMatrix;           /* offset to 3x4 matrix */
  UInt32              offsetMcurves;          /* offset to first "M" curve */
  UInt32              offsetCLUT;             /* offset to multi-dimensional LUT of type CMMultiFunctCLUTType */
  UInt32              offsetAcurves;          /* offset to first "A" curve */
  UInt8               data[1];                /* variable size */
} CMMultiFunctLutType, CMMultiFunctLutA2BType, CMMultiFunctLutB2AType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultiFunctCLUTType {
  UInt8               gridPoints[16];         /* grigpoints for each input channel dimension (remaining are 0) */
  UInt8               entrySize;              /* bytes per lut enrty (1 or 2) */
  UInt8               reserved[3];            /* fill with 0x00 */
  UInt8               data[2];                /* variable size, determined by above */
} CMMultiFunctCLUTType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMeasurementType {
  OSType              typeDescriptor;         /* 'meas' = cmSigMeasurementType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              standardObserver;       /* cmStdobsUnknown, cmStdobs1931TwoDegrees, cmStdobs1964TenDegrees */
  CMFixedXYZColor     backingXYZ;             /* absolute XYZ values of backing */
  UInt32              geometry;               /* cmGeometryUnknown, cmGeometry045or450 (0/45), cmGeometry0dord0 (0/d or d/0) */
  UInt32              flare;                  /* cmFlare0, cmFlare100 */
  UInt32              illuminant;             /* cmIlluminantUnknown, cmIlluminantD50, ... */
} CMMeasurementType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNamedColorType {
  OSType              typeDescriptor;         /* 'ncol' = cmSigNamedColorType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              vendorFlag;             /* */
  UInt32              count;                  /* count of named colors in array that follows */
  UInt8               prefixName[1];          /* variable size, max = 32 */
#if 0 /* NOTE: Field offsets are variable from here on. */
  /* In order to correctly reflect the actual format of this tag, some of the fields in */
  /* this structure have been removed because they follow an array field of variable size. */
  /* As a result, the size of this structure has changed from previous versions of this interface. */
  /* Code that relies on sizeof(CMNamedColorType) should be changed. */
  UInt8               suffixName[];           /* variable size, max = 32 */
  struct {
    UInt8             rootName[];             /* variable size, max = 32 */
    UInt8             colorCoords[];          /* variable size */
  } colorName[];                              /* variable size */
#endif
} CMNamedColorType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNamedColor2EntryType {
  UInt8               rootName[32];           /* 32 byte field.  7 bit ASCII null terminated */
  UInt16              PCSColorCoords[3];      /* Lab or XYZ color */
  UInt16              DeviceColorCoords[1];   /* variable size */
} CMNamedColor2EntryType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNamedColor2Type {
  OSType              typeDescriptor;         /* 'ncl2' = cmSigNamedColor2Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              vendorFlag;             /* lower 16 bits reserved for ICC use */
  UInt32              count;                  /* count of named colors in array that follows */
  UInt32              deviceChannelCount;     /* number of device channels, 0 indicates no device value available */
  UInt8               prefixName[32];         /* Fixed 32 byte size.  7 bit ASCII null terminated */
  UInt8               suffixName[32];         /* Fixed 32 byte size.  7 bit ASCII null terminated */
  char                data[1];                /* variable size data for CMNamedColor2EntryType */
} CMNamedColor2Type DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNativeDisplayInfo {
  UInt32              dataSize;               /* Size of this structure */
  CMFixedXYColor      redPhosphor;            /* Phosphors - native cromaticity values of the display  */
  CMFixedXYColor      greenPhosphor;
  CMFixedXYColor      bluePhosphor;
  CMFixedXYColor      whitePoint;
  Fixed               redGammaValue;          /* Gammas - native gamma values of the display */
  Fixed               greenGammaValue;
  Fixed               blueGammaValue;
                                              /*  Gamma tables - if if gammaChannels is not zero, */
                                              /*  native gamma tables are preferred over values */
                                              /*  redGammaValue, greenGammaValue, blueGammaValue */
  UInt16              gammaChannels;          /* # of gamma channels (1 or 3) */
  UInt16              gammaEntryCount;        /* 1-based number of entries per channel */
  UInt16              gammaEntrySize;         /* size in bytes of each entry */
  char                gammaData[1];           /* variable size, determined by channels*entryCount*entrySize */
} CMNativeDisplayInfo DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNativeDisplayInfoType {
  OSType              typeDescriptor;         /* 'ndin' = cmSigNativeDisplayInfoType */
  UInt32              reserved;               /* fill with 0x00 */
  CMNativeDisplayInfo nativeDisplayInfo;      /* data of type CMNativeDisplayInfo */
} CMNativeDisplayInfoType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMParametricCurveType {
  OSType              typeDescriptor;         /* 'para' = cmSigParametricCurveType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt16              functionType;           /* cmParametricType0, cmParametricType1, etc. */
  UInt16              reserved2;              /* fill with 0x00 */
  Fixed               value[1];               /* variable size, determined by functionType */
} CMParametricCurveType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMTextDescriptionType {
  OSType              typeDescriptor;         /* 'desc' = cmSigProfileDescriptionType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              ASCIICount;             /* Count of bytes (including null terminator)  */
  UInt8               ASCIIName[2];           /* variable size, determined by ASCIICount.  7 bit ASCII null terminated */
#if 0 /* NOTE: Field offsets are variable from here on. */
  /* In order to correctly reflect the actual format of this tag, some of the fields in */
  /* this structure have been removed because they follow an array field of variable size. */
  /* As a result, the size of this structure has changed from previous versions of this interface. */
  /* Code that relies on sizeof(CMTextDescriptionType) should be changed. */
  UInt32              UniCodeCode;            /* Unused */
  UInt32              UniCodeCount;           /* Count of 2-byte characters (including null terminator) */
  UInt8               UniCodeName[];          /* variable size, determined by UniCodeCount */
  SInt16              ScriptCodeCode;         /* Mac-defined script code */
  UInt8               ScriptCodeCount;        /* Count of bytes (including null terminator) */
  UInt8               ScriptCodeName[];       /* variable size, determined by ScriptCodeCount */
#endif
} CMTextDescriptionType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMTextType {
  OSType              typeDescriptor;         /* 'text' = cmSigTextType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt8               text[1];                /* variable size, determined by tag element size */
} CMTextType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUnicodeTextType {
  OSType              typeDescriptor;         /* 'utxt' = cmSigUnicodeTextType */
  UInt32              reserved;               /* fill with 0x00 */
  UniChar             text[1];                /* variable size, determined by tag element size  */
} CMUnicodeTextType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMScreeningChannelRec {
  Fixed               frequency;
  Fixed               angle;
  UInt32              spotFunction;
} CMScreeningChannelRec DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMScreeningType {
  OSType              typeDescriptor;         /* 'scrn' = cmSigScreeningType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              screeningFlag;          /* bit 0 : use printer default screens, bit 1 : inch/cm */
  UInt32              channelCount;           /* */
  CMScreeningChannelRec  channelInfo[1];      /* variable size, determined by channelCount */
} CMScreeningType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMSignatureType {
  OSType              typeDescriptor;         /* 'sig ' = cmSigSignatureType */
  UInt32              reserved;               /* fill with 0x00 */
  OSType              signature;
} CMSignatureType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMS15Fixed16ArrayType {
  OSType              typeDescriptor;         /* 'sf32' = cmSigS15Fixed16Type */
  UInt32              reserved;               /* fill with 0x00 */
  Fixed               value[1];               /* variable size, determined by tag element size */
} CMS15Fixed16ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMU16Fixed16ArrayType {
  OSType              typeDescriptor;         /* 'uf32' = cmSigU16Fixed16Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              value[1];               /* variable size, determined by tag element size */
} CMU16Fixed16ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUInt8ArrayType {
  OSType              typeDescriptor;         /* 'ui08' = cmSigUInt8Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt8               value[1];               /* variable size, determined by tag element size */
} CMUInt8ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUInt16ArrayType {
  OSType              typeDescriptor;         /* 'ui16' = cmSigUInt16Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt16              value[1];               /* variable size, determined by tag element size */
} CMUInt16ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUInt32ArrayType {
  OSType              typeDescriptor;         /* 'ui32' = cmSigUInt32Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              value[1];               /* variable size, determined by tag element size */
} CMUInt32ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUInt64ArrayType {
  OSType              typeDescriptor;         /* 'ui64' = cmSigUInt64Type */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              value[1];               /* variable size, determined by tag element size */
} CMUInt64ArrayType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMViewingConditionsType {
  OSType              typeDescriptor;         /* 'view' = cmSigViewingConditionsType */
  UInt32              reserved;               /* fill with 0x00 */
  CMFixedXYZColor     illuminant;             /* absolute XYZs of illuminant  in cd/m^2 */
  CMFixedXYZColor     surround;               /* absolute XYZs of surround in cd/m^2 */
  UInt32              stdIlluminant;          /* see definitions of std illuminants */
} CMViewingConditionsType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMXYZType {
  OSType              typeDescriptor;         /* 'XYZ ' = cmSigXYZType */
  UInt32              reserved;               /* fill with 0x00 */
  CMFixedXYZColor     XYZ[1];                 /* variable size, determined by tag element size */
} CMXYZType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMProfileSequenceDescType {
  OSType              typeDescriptor;         /* 'pseq' = cmProfileSequenceDescTag */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              count;                  /* Number of descriptions */
  char                data[1];                /* variable size data explained in ICC spec */
} CMProfileSequenceDescType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMUcrBgType {
  OSType              typeDescriptor;         /* 'bfd ' = cmSigUcrBgType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              ucrCount;               /* Number of UCR entries */
  UInt16              ucrValues[1];           /* variable size, determined by ucrCount */
#if 0 /* NOTE: Field offsets are variable from here on. */
  /* In order to correctly reflect the actual format of this tag, some of the fields in */
  /* this structure have been removed because they follow an array field of variable size. */
  /* As a result, the size of this structure has changed from previous versions of this interface. */
  /* Code that relies on sizeof(CMUcrBgType) should be changed. */
  UInt32              bgCount;                /* Number of BG entries */
  UInt16              bgValues[];             /* variable size, determined by bgCount */
  UInt8               ucrbgASCII[];           /* null terminated ASCII string */
#endif
} CMUcrBgType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Private Tag Type Definitions */


typedef struct CMIntentCRDVMSize {
  UInt32              renderingIntent;        /* rendering intent */
  UInt32              VMSize;                 /* VM size taken up by the CRD */
} CMIntentCRDVMSize DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMPS2CRDVMSizeType {
  OSType              typeDescriptor;         /* 'psvm' = cmSigPS2CRDVMSizeType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              count;                  /* number of intent entries */
  CMIntentCRDVMSize   intentCRD[1];           /* variable size, determined by count */
} CMPS2CRDVMSizeType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


enum {
  cmVideoCardGammaTableType     = 0,
  cmVideoCardGammaFormulaType   = 1
};

typedef struct CMVideoCardGammaTable {
  UInt16              channels;               /* # of gamma channels (1 or 3) */
  UInt16              entryCount;             /* 1-based number of entries per channel */
  UInt16              entrySize;              /* size in bytes of each entry */
  char                data[1];                /* variable size, determined by channels*entryCount*entrySize */
} CMVideoCardGammaTable DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMVideoCardGammaFormula {
  Fixed               redGamma;               /* must be > 0.0 */
  Fixed               redMin;                 /* must be > 0.0 and < 1.0 */
  Fixed               redMax;                 /* must be > 0.0 and < 1.0 */
  Fixed               greenGamma;             /* must be > 0.0 */
  Fixed               greenMin;               /* must be > 0.0 and < 1.0 */
  Fixed               greenMax;               /* must be > 0.0 and < 1.0 */
  Fixed               blueGamma;              /* must be > 0.0 */
  Fixed               blueMin;                /* must be > 0.0 and < 1.0 */
  Fixed               blueMax;                /* must be > 0.0 and < 1.0 */
} CMVideoCardGammaFormula DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMVideoCardGamma {
  UInt32              tagType;
  union {
    CMVideoCardGammaTable    table;
    CMVideoCardGammaFormula  formula;
  }                       u;
} CMVideoCardGamma DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMVideoCardGammaType {
  OSType              typeDescriptor;         /* 'vcgt' = cmSigVideoCardGammaType */
  UInt32              reserved;               /* fill with 0x00 */
  CMVideoCardGamma    gamma;
} CMVideoCardGammaType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMakeAndModel {
  OSType              manufacturer;
  UInt32              model;
  UInt32              serialNumber;
  UInt32              manufactureDate;
  UInt32              reserved1;              /* fill with 0x00 */
  UInt32              reserved2;              /* fill with 0x00 */
  UInt32              reserved3;              /* fill with 0x00 */
  UInt32              reserved4;              /* fill with 0x00 */
} CMMakeAndModel DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMakeAndModelType {
  OSType              typeDescriptor;         /* 'mmod' = cmSigMakeAndModelType */
  UInt32              reserved;               /* fill with 0x00 */
  CMMakeAndModel      makeAndModel;
} CMMakeAndModelType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultiLocalizedUniCodeEntryRec {
  char                languageCode[2];        /* language code from ISO-639 */
  char                regionCode[2];          /* region code from ISO-3166 */
  UInt32              textLength;             /* the length in bytes of the string */
  UInt32              textOffset;             /* the offset from the start of tag in bytes */
} CMMultiLocalizedUniCodeEntryRec DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultiLocalizedUniCodeType {
  OSType              typeDescriptor;         /* 'mluc' = cmSigMultiLocalizedUniCodeType */
  UInt32              reserved;               /* fill with 0x00 */
  UInt32              entryCount;             /* 1-based number of name records that follow */
  UInt32              entrySize;              /* size in bytes of name records that follow */
                                              /* variable-length data for storage of CMMultiLocalizedUniCodeEntryRec */
                                              /* variable-length data for storage of Unicode strings*/
} CMMultiLocalizedUniCodeType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


#if !__LP64__
/************************************************************************/
/*************** ColorSync 1.0 profile specification ********************/
/************************************************************************/
enum {
  cmGrayResponse                = 0,
  cmRedResponse                 = 1,
  cmGreenResponse               = 2,
  cmBlueResponse                = 3,
  cmCyanResponse                = 4,
  cmMagentaResponse             = 5,
  cmYellowResponse              = 6,
  cmUcrResponse                 = 7,
  cmBgResponse                  = 8,
  cmOnePlusLastResponse         = 9
};


/* Device types */
enum {
  cmMonitorDevice               = 'mntr',
  cmScannerDevice               = 'scnr',
  cmPrinterDevice               = 'prtr'
};


typedef struct CMIString {
  ScriptCode          theScript;
  Str63               theString;
} CMIString DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Profile options */
enum {
  cmPerceptualMatch             = 0x0000, /* Default. For photographic images */
  cmColorimetricMatch           = 0x0001, /* Exact matching when possible */
  cmSaturationMatch             = 0x0002 /* For solid colors */
};

/* Profile flags */
enum {
  cmNativeMatchingPreferred     = 0x00000001, /* Default to native not preferred */
  cmTurnOffCache                = 0x00000002 /* Default to turn on CMM cache */
};


typedef UInt32 CMMatchOption DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
typedef UInt32 CMMatchFlag DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMHeader {
  UInt32              size;
  OSType              CMMType;
  UInt32              applProfileVersion;
  OSType              dataType;
  OSType              deviceType;
  OSType              deviceManufacturer;
  UInt32              deviceModel;
  UInt32              deviceAttributes[2];
  UInt32              profileNameOffset;
  UInt32              customDataOffset;
  CMMatchFlag         flags;
  CMMatchOption       options;
  CMXYZColor          white;
  CMXYZColor          black;
} CMHeader DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMProfileChromaticities {
  CMXYZColor          red;
  CMXYZColor          green;
  CMXYZColor          blue;
  CMXYZColor          cyan;
  CMXYZColor          magenta;
  CMXYZColor          yellow;
} CMProfileChromaticities DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMProfileResponse {
  UInt16              counts[9];
  UInt16              data[1];                /* Variable size */
} CMProfileResponse DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMProfile {
  CMHeader            header;
  CMProfileChromaticities  profile;
  CMProfileResponse   response;
  CMIString           profileName;
  char                customData[1];          /* Variable size */
} CMProfile, *CMProfilePtr, **CMProfileHandle DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#endif // !__LP64__


#pragma pack(pop)

#pragma mark --- CMTypes.h ----

/* Abstract data type for memory-based Profile */
typedef struct OpaqueCMProfileRef*      CMProfileRef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#if !__LP64__
/* Abstract data type for Profile search result */
typedef struct OpaqueCMProfileSearchRef*  CMProfileSearchRef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Abstract data type for BeginMatching(‚Ä¶) reference */
typedef struct OpaqueCMMatchRef*        CMMatchRef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif // !__LP64__

/* Abstract data type for ColorWorld reference */
typedef struct OpaqueCMWorldRef*        CMWorldRef DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Data type for ColorSync DisplayID reference */
/* On 8 & 9 this is a AVIDType */
/* On X this is a CGSDisplayID */
typedef UInt32                          CMDisplayIDType DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
typedef UInt32                          CMChromaticAdaptation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
enum {
  cmUseDefaultChromaticAdaptation = 0,
  cmLinearChromaticAdaptation   = 1,
  cmVonKriesChromaticAdaptation = 2,
  cmBradfordChromaticAdaptation = 3
};



/* Caller-supplied flatten function */
typedef CALLBACK_API( OSErr , CMFlattenProcPtr )(SInt32 command, long *size, void *data, void *refCon);

typedef STACK_UPP_TYPE(CMFlattenProcPtr) CMFlattenUPP;




#if !_DECLARE_CS_QD_API_

#define CMBITMAPCALLBACKPROCPTR_DEFINED 1

/* Caller-supplied progress function for Bitmap & PixMap matching routines */
typedef CALLBACK_API( Boolean , CMBitmapCallBackProcPtr )(SInt32 progress, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMBitmapCallBackProcPtr) CMBitmapCallBackUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#endif /* _DECLARE_CS_QD_API_ */

/* Caller-supplied progress function for NCMMConcatInit & NCMMNewLinkProfile routines */
typedef CALLBACK_API( Boolean , CMConcatCallBackProcPtr )(SInt32 progress, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMConcatCallBackProcPtr) CMConcatCallBackUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


#if !__LP64__

/* Caller-supplied filter function for Profile search*/
typedef CALLBACK_API( Boolean , CMProfileFilterProcPtr )(CMProfileRef prof, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMProfileFilterProcPtr) CMProfileFilterUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Caller-supplied function for profile access*/
typedef CALLBACK_API( OSErr , CMProfileAccessProcPtr )(SInt32 command, SInt32 offset, SInt32 *size, void *data, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMProfileAccessProcPtr) CMProfileAccessUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#endif // !__LP64__

#ifdef __cplusplus
}
#endif

#pragma mark --- CMApplication.h ----

#ifdef __cplusplus
extern "C" {
#endif

#if !__LP64__
#if defined(_MSC_VER)
#pragma pack(push, CMApplication_h, 2)
#else
#pragma options align=mac68k
#endif
#endif

enum {
  kDefaultCMMSignature          = 'appl'
};

#if !__LP64__ && !TARGET_OS_WIN32
/* PicComment IDs */
enum {
  cmBeginProfile                = 220,
  cmEndProfile                  = 221,
  cmEnableMatching              = 222,
  cmDisableMatching             = 223,
  cmComment                     = 224
};

/* PicComment selectors for cmComment */
enum {
  cmBeginProfileSel             = 0,
  cmContinueProfileSel          = 1,
  cmEndProfileSel               = 2,
  cmProfileIdentifierSel        = 3
};

/* Defines for version 1.0 CMProfileSearchRecord.fieldMask */
enum {
  cmMatchCMMType                = 0x00000001,
  cmMatchApplProfileVersion     = 0x00000002,
  cmMatchDataType               = 0x00000004,
  cmMatchDeviceType             = 0x00000008,
  cmMatchDeviceManufacturer     = 0x00000010,
  cmMatchDeviceModel            = 0x00000020,
  cmMatchDeviceAttributes       = 0x00000040,
  cmMatchFlags                  = 0x00000080,
  cmMatchOptions                = 0x00000100,
  cmMatchWhite                  = 0x00000200,
  cmMatchBlack                  = 0x00000400
};

/* Defines for version 2.0 CMSearchRecord.searchMask */
enum {
  cmMatchAnyProfile             = 0x00000000,
  cmMatchProfileCMMType         = 0x00000001,
  cmMatchProfileClass           = 0x00000002,
  cmMatchDataColorSpace         = 0x00000004,
  cmMatchProfileConnectionSpace = 0x00000008,
  cmMatchManufacturer           = 0x00000010,
  cmMatchModel                  = 0x00000020,
  cmMatchAttributes             = 0x00000040,
  cmMatchProfileFlags           = 0x00000080
};

#endif // !__LP64__ && !TARGET_OS_WIN32

/* Flags for PostScript-related functions */
enum {
  cmPS7bit                      = 1,
  cmPS8bit                      = 2
};

#if !__LP64__ && !TARGET_OS_WIN32
/* Flags for profile embedding functions */
enum {
  cmEmbedWholeProfile           = 0x00000000,
  cmEmbedProfileIdentifier      = 0x00000001
};
#endif // !__LP64__ && !TARGET_OS_WIN32

/* Commands for CMFlattenUPP() */
enum {
  cmOpenReadSpool               = 1,
  cmOpenWriteSpool              = 2,
  cmReadSpool                   = 3,
  cmWriteSpool                  = 4,
  cmCloseSpool                  = 5
};

/* Commands for CMAccessUPP() */
enum {
  cmOpenReadAccess              = 1,
  cmOpenWriteAccess             = 2,
  cmReadAccess                  = 3,
  cmWriteAccess                 = 4,
  cmCloseAccess                 = 5,
  cmCreateNewAccess             = 6,
  cmAbortWriteAccess            = 7,
  cmBeginAccess                 = 8,
  cmEndAccess                   = 9
};

/* Use types for CMGet/SetDefaultProfileByUse() */
enum {
  cmInputUse                    = 'inpt',
  cmOutputUse                   = 'outp',
  cmDisplayUse                  = 'dply',
  cmProofUse                    = 'pruf'
};


/* Union of 1.0 2.0, and 4.0 profile header variants */
union CMAppleProfileHeader {
#if !__LP64__ && !TARGET_OS_WIN32
  CMHeader            cm1;
#endif // !__LP64__ && !TARGET_OS_WIN32
  CM2Header           cm2;
  CM4Header           cm4;
};
typedef union CMAppleProfileHeader      CMAppleProfileHeader DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* CWConcatColorWorld() definitions */
typedef struct CMConcatProfileSet {
  UInt16              keyIndex;               /* Zero-based */
  UInt16              count;                  /* Min 1 */
  CMProfileRef        profileSet[1];          /* Variable. Ordered from Source -> Dest */
} CMConcatProfileSet DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* NCWConcatColorWorld() definitions */
typedef struct NCMConcatProfileSpec {
  UInt32              renderingIntent;        /* renderingIntent override */
  UInt32              transformTag;           /* transform enumerations defined below */
  CMProfileRef        profile;                /* profile */
} NCMConcatProfileSpec DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct NCMConcatProfileSet {
  OSType              cmm;                    /* e.g. 'KCMS', 'appl', ...  uniquely ids the cmm, or 0000 */
  UInt32              flags;                  /* specify quality, lookup only, no gamut checking ... */
  UInt32              flagsMask;              /* which bits of 'flags' to use to override profile */
  UInt32              profileCount;           /* how many ProfileSpecs in the following set */
  NCMConcatProfileSpec  profileSpecs[1];      /* Variable. Ordered from Source -> Dest */
} NCMConcatProfileSet DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

enum {
  kNoTransform                  = 0,    /* Not used */
  kUseAtoB                      = 1,    /* Use 'A2B*' tag from this profile or equivalent */
  kUseBtoA                      = 2,    /* Use 'B2A*' tag from this profile or equivalent */
  kUseBtoB                      = 3,    /* Use 'pre*' tag from this profile or equivalent */
                                        /* For typical device profiles the following synonyms may be useful */
  kDeviceToPCS                  = kUseAtoB, /* Device Dependent to Device Independent */
  kPCSToDevice                  = kUseBtoA, /* Device Independent to Device Dependent */
  kPCSToPCS                     = kUseBtoB, /* Independent, through device's gamut */
  kUseProfileIntent             = (int)0xFFFFFFFF /* For renderingIntent in NCMConcatProfileSpec    */
};


/* ColorSync color data types */
typedef struct CMRGBColor {
  UInt16              red;                    /* 0..65535 */
  UInt16              green;
  UInt16              blue;
} CMRGBColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMCMYKColor {
  UInt16              cyan;                   /* 0..65535 */
  UInt16              magenta;
  UInt16              yellow;
  UInt16              black;
} CMCMYKColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMCMYColor {
  UInt16              cyan;                   /* 0..65535 */
  UInt16              magenta;
  UInt16              yellow;
} CMCMYColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMHLSColor {
  UInt16              hue;                    /* 0..65535. Fraction of circle. Red at 0 */
  UInt16              lightness;              /* 0..65535 */
  UInt16              saturation;             /* 0..65535 */
} CMHLSColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMHSVColor {
  UInt16              hue;                    /* 0..65535. Fraction of circle. Red at 0 */
  UInt16              saturation;             /* 0..65535 */
  UInt16              value;                  /* 0..65535 */
} CMHSVColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMLabColor {
  UInt16              L;                      /* 0..65535 maps to 0..100 */
  UInt16              a;                      /* 0..65535 maps to -128..127.996 */
  UInt16              b;                      /* 0..65535 maps to -128..127.996 */
} CMLabColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMLuvColor {
  UInt16              L;                      /* 0..65535 maps to 0..100 */
  UInt16              u;                      /* 0..65535 maps to -128..127.996 */
  UInt16              v;                      /* 0..65535 maps to -128..127.996 */
} CMLuvColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMYxyColor {
  UInt16              capY;                   /* 0..65535 maps to 0..1 */
  UInt16              x;                      /* 0..65535 maps to 0..1 */
  UInt16              y;                      /* 0..65535 maps to 0..1 */
} CMYxyColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMGrayColor {
  UInt16              gray;                   /* 0..65535 */
} CMGrayColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultichannel5Color {
  UInt8               components[5];          /* 0..255 */
} CMMultichannel5Color DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultichannel6Color {
  UInt8               components[6];          /* 0..255 */
} CMMultichannel6Color DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultichannel7Color {
  UInt8               components[7];          /* 0..255 */
} CMMultichannel7Color DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMMultichannel8Color {
  UInt8               components[8];          /* 0..255 */
} CMMultichannel8Color DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMNamedColor {
  UInt32              namedColorIndex;        /* 0..a lot */
} CMNamedColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef union CMColor {
  CMRGBColor          rgb;
  CMHSVColor          hsv;
  CMHLSColor          hls;
  CMXYZColor          XYZ;
  CMLabColor          Lab;
  CMLuvColor          Luv;
  CMYxyColor          Yxy;
  CMCMYKColor         cmyk;
  CMCMYColor          cmy;
  CMGrayColor         gray;
  CMMultichannel5Color  mc5;
  CMMultichannel6Color  mc6;
  CMMultichannel7Color  mc7;
  CMMultichannel8Color  mc8;
  CMNamedColor        namedColor;
} CMColor DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#if !__LP64__ && !TARGET_OS_WIN32
/* GetIndexedProfile() search definition*/
typedef struct CMProfileSearchRecord {
  CMHeader            header;
  UInt32              fieldMask;
  UInt32              reserved[2];
} CMProfileSearchRecord, *CMProfileSearchRecordPtr, **CMProfileSearchRecordHandle DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* CMNewProfileSearch() search definition */
typedef struct CMSearchRecord {
  OSType              CMMType;
  OSType              profileClass;
  OSType              dataColorSpace;
  OSType              profileConnectionSpace;
  UInt32              deviceManufacturer;
  UInt32              deviceModel;
  UInt32              deviceAttributes[2];
  UInt32              profileFlags;
  UInt32              searchMask;
  CMProfileFilterUPP  filter;
} CMSearchRecord DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif // !__LP64__ && !TARGET_OS_WIN32

/* CMMIterateUPP() structure */
typedef struct CMMInfo {
  size_t              dataSize;               /* Size of this structure - compatibility*/
  OSType              CMMType;                /* Signature, e.g. 'appl', 'HDM ' or 'KCMS'*/
  OSType              CMMMfr;                 /* Vendor, e.g. 'appl'*/
  UInt32              CMMVersion;             /* CMM version number*/
  unsigned char       ASCIIName[32];          /* pascal string - name*/
  unsigned char       ASCIIDesc[256];         /* pascal string - description or copyright*/
  UniCharCount        UniCodeNameCount;       /* count of UniChars in following array*/
  UniChar             UniCodeName[32];        /* the name in UniCode chars*/
  UniCharCount        UniCodeDescCount;       /* count of UniChars in following array*/
  UniChar             UniCodeDesc[256];       /* the description in UniCode chars*/
} CMMInfo DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* GetCWInfo() structures */

#if !__LP64__ && !TARGET_OS_WIN32

typedef struct CMMInfoRecord {
  OSType              CMMType;
  long                CMMVersion;
} CMMInfoRecord DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMCWInfoRecord {
  UInt32              cmmCount;
  CMMInfoRecord       cmmInfo[2];
} CMCWInfoRecord;

/* profile identifier structures */
typedef struct CMProfileIdentifier {
  CM2Header           profileHeader;
  CMDateTime          calibrationDate;
  UInt32              ASCIIProfileDescriptionLen;
  char                ASCIIProfileDescription[1]; /* variable length */
} CMProfileIdentifier, *CMProfileIdentifierPtr DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif // !__LP64__ && !TARGET_OS_WIN32

/* colorspace masks */
enum {
  cmColorSpaceSpaceMask         = 0x0000003F,
  cmColorSpacePremulAlphaMask   = 0x00000040,
  cmColorSpaceAlphaMask         = 0x00000080,
  cmColorSpaceSpaceAndAlphaMask = 0x000000FF,
  cmColorSpacePackingMask       = 0x0000FF00,
  cmColorSpaceEncodingMask      = 0x000F0000,
  cmColorSpaceReservedMask      = (int)0xFFF00000
};

/* packing formats */
enum {
  cmNoColorPacking              = 0x0000,
  cmWord5ColorPacking           = 0x0500,
  cmWord565ColorPacking         = 0x0600,
  cmLong8ColorPacking           = 0x0800,
  cmLong10ColorPacking          = 0x0A00,
  cmAlphaFirstPacking           = 0x1000,
  cmOneBitDirectPacking         = 0x0B00,
  cmAlphaLastPacking            = 0x0000,
  cm8_8ColorPacking             = 0x2800,
  cm16_8ColorPacking            = 0x2000,
  cm24_8ColorPacking            = 0x2100,
  cm32_8ColorPacking            = cmLong8ColorPacking,
  cm40_8ColorPacking            = 0x2200,
  cm48_8ColorPacking            = 0x2300,
  cm56_8ColorPacking            = 0x2400,
  cm64_8ColorPacking            = 0x2500,
  cm32_16ColorPacking           = 0x2600,
  cm48_16ColorPacking           = 0x2900,
  cm64_16ColorPacking           = 0x2A00,
  cm32_32ColorPacking           = 0x2700,
  cmLittleEndianPacking         = 0x4000,
  cmReverseChannelPacking       = 0x8000
};

/* channel encoding format */
enum {
  cmSRGB16ChannelEncoding       = 0x00010000 /* used for sRGB64 encoding ( ±3.12 format)*/
};

/* general colorspaces */
enum {
  cmNoSpace                     = 0x0000,
  cmRGBSpace                    = 0x0001,
  cmCMYKSpace                   = 0x0002,
  cmHSVSpace                    = 0x0003,
  cmHLSSpace                    = 0x0004,
  cmYXYSpace                    = 0x0005,
  cmXYZSpace                    = 0x0006,
  cmLUVSpace                    = 0x0007,
  cmLABSpace                    = 0x0008,
  cmReservedSpace1              = 0x0009,
  cmGraySpace                   = 0x000A,
  cmReservedSpace2              = 0x000B,
  cmGamutResultSpace            = 0x000C,
  cmNamedIndexedSpace           = 0x0010,
  cmMCFiveSpace                 = 0x0011,
  cmMCSixSpace                  = 0x0012,
  cmMCSevenSpace                = 0x0013,
  cmMCEightSpace                = 0x0014,
  cmAlphaPmulSpace              = 0x0040,
  cmAlphaSpace                  = 0x0080,
  cmRGBASpace                   = cmRGBSpace + cmAlphaSpace,
  cmGrayASpace                  = cmGraySpace + cmAlphaSpace,
  cmRGBAPmulSpace               = cmRGBASpace + cmAlphaPmulSpace,
  cmGrayAPmulSpace              = cmGrayASpace + cmAlphaPmulSpace
};

/* supported CMBitmapColorSpaces - Each of the following is a */
/* combination of a general colospace and a packing formats. */
/* Each can also be or'd with cmReverseChannelPacking. */
enum {
  cmGray8Space                  = cmGraySpace + cm8_8ColorPacking,
  cmGray16Space                 = cmGraySpace,
  cmGray16LSpace                = cmGraySpace + cmLittleEndianPacking,
  cmGrayA16Space                = cmGrayASpace + cm16_8ColorPacking,
  cmGrayA32Space                = cmGrayASpace,
  cmGrayA32LSpace               = cmGrayASpace + cmLittleEndianPacking,
  cmGrayA16PmulSpace            = cmGrayAPmulSpace + cm16_8ColorPacking,
  cmGrayA32PmulSpace            = cmGrayAPmulSpace,
  cmGrayA32LPmulSpace           = cmGrayAPmulSpace + cmLittleEndianPacking,
  cmRGB16Space                  = cmRGBSpace + cmWord5ColorPacking,
  cmRGB16LSpace                 = cmRGBSpace + cmWord5ColorPacking + cmLittleEndianPacking,
  cmRGB565Space                 = cmRGBSpace + cmWord565ColorPacking,
  cmRGB565LSpace                = cmRGBSpace + cmWord565ColorPacking + cmLittleEndianPacking,
  cmRGB24Space                  = cmRGBSpace + cm24_8ColorPacking,
  cmRGB32Space                  = cmRGBSpace + cm32_8ColorPacking,
  cmRGB48Space                  = cmRGBSpace + cm48_16ColorPacking,
  cmRGB48LSpace                 = cmRGBSpace + cm48_16ColorPacking + cmLittleEndianPacking,
  cmARGB32Space                 = cmRGBASpace + cm32_8ColorPacking + cmAlphaFirstPacking,
  cmARGB64Space                 = cmRGBASpace + cm64_16ColorPacking + cmAlphaFirstPacking,
  cmARGB64LSpace                = cmRGBASpace + cm64_16ColorPacking + cmAlphaFirstPacking + cmLittleEndianPacking,
  cmRGBA32Space                 = cmRGBASpace + cm32_8ColorPacking + cmAlphaLastPacking,
  cmRGBA64Space                 = cmRGBASpace + cm64_16ColorPacking + cmAlphaLastPacking,
  cmRGBA64LSpace                = cmRGBASpace + cm64_16ColorPacking + cmAlphaLastPacking + cmLittleEndianPacking,
  cmARGB32PmulSpace             = cmRGBAPmulSpace + cm32_8ColorPacking + cmAlphaFirstPacking,
  cmARGB64PmulSpace             = cmRGBAPmulSpace + cm64_16ColorPacking + cmAlphaFirstPacking,
  cmARGB64LPmulSpace            = cmRGBAPmulSpace + cm64_16ColorPacking + cmAlphaFirstPacking + cmLittleEndianPacking,
  cmRGBA32PmulSpace             = cmRGBAPmulSpace + cm32_8ColorPacking + cmAlphaLastPacking,
  cmRGBA64PmulSpace             = cmRGBAPmulSpace + cm64_16ColorPacking + cmAlphaLastPacking,
  cmRGBA64LPmulSpace            = cmRGBAPmulSpace + cm64_16ColorPacking + cmAlphaLastPacking + cmLittleEndianPacking,
  cmCMYK32Space                 = cmCMYKSpace + cm32_8ColorPacking,
  cmCMYK64Space                 = cmCMYKSpace + cm64_16ColorPacking,
  cmCMYK64LSpace                = cmCMYKSpace + cm64_16ColorPacking + cmLittleEndianPacking,
  cmHSV32Space                  = cmHSVSpace + cmLong10ColorPacking,
  cmHLS32Space                  = cmHLSSpace + cmLong10ColorPacking,
  cmYXY32Space                  = cmYXYSpace + cmLong10ColorPacking,
  cmXYZ24Space                  = cmXYZSpace + cm24_8ColorPacking,
  cmXYZ32Space                  = cmXYZSpace + cmLong10ColorPacking,
  cmXYZ48Space                  = cmXYZSpace + cm48_16ColorPacking,
  cmXYZ48LSpace                 = cmXYZSpace + cm48_16ColorPacking + cmLittleEndianPacking,
  cmLUV32Space                  = cmLUVSpace + cmLong10ColorPacking,
  cmLAB24Space                  = cmLABSpace + cm24_8ColorPacking,
  cmLAB32Space                  = cmLABSpace + cmLong10ColorPacking,
  cmLAB48Space                  = cmLABSpace + cm48_16ColorPacking,
  cmLAB48LSpace                 = cmLABSpace + cm48_16ColorPacking + cmLittleEndianPacking,
  cmGamutResult1Space           = cmOneBitDirectPacking + cmGamutResultSpace,
  cmNamedIndexed32Space         = cm32_32ColorPacking + cmNamedIndexedSpace,
  cmNamedIndexed32LSpace        = cm32_32ColorPacking + cmNamedIndexedSpace + cmLittleEndianPacking,
  cmMCFive8Space                = cm40_8ColorPacking + cmMCFiveSpace,
  cmMCSix8Space                 = cm48_8ColorPacking + cmMCSixSpace,
  cmMCSeven8Space               = cm56_8ColorPacking + cmMCSevenSpace,
  cmMCEight8Space               = cm64_8ColorPacking + cmMCEightSpace
};


typedef UInt32                          CMBitmapColorSpace DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMBitmap {
  char *              image;
  size_t              width;
  size_t              height;
  size_t              rowBytes;
  size_t              pixelSize;
  CMBitmapColorSpace  space;
  UInt32              user1;
  UInt32              user2;
} CMBitmap DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Profile Locations */

enum {
#if !__LP64__ && !TARGET_OS_WIN32
  CS_MAX_PATH                   = 256
#elif TARGET_OS_WIN32
  CS_MAX_PATH                   = 1024,
  WIN_CS_MAX_PATH               = CS_MAX_PATH / sizeof(TCHAR)
#else
  CS_MAX_PATH                   = 1024
#endif // !__LP64__ && !TARGET_OS_WIN32
};


enum {
  cmNoProfileBase               = 0,
#if !__LP64__ && !TARGET_OS_WIN32
  cmFileBasedProfile            = 1,
  cmHandleBasedProfile          = 2,
  cmPtrBasedProfile             = 3,
  cmProcedureBasedProfile       = 4,
#endif // !__LP64__ && !TARGET_OS_WIN32
  cmPathBasedProfile            = 5,
  cmBufferBasedProfile          = 6
};


#if !__LP64__ && !TARGET_OS_WIN32
/* This structure is deprecated in Mac OS X 10.5. Use CMPathLocation instead.*/
typedef struct CMFileLocation {
  FSSpec              spec;
} CMFileLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif // !__LP64__ && !TARGET_OS_WIN32

typedef struct CMHandleLocation {
  Handle              h;
} CMHandleLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

#if !__LP64__ && !TARGET_OS_WIN32
/* This structure is deprecated in Mac OS X 10.5. Use CMBufferLocation instead.*/
typedef struct CMPtrLocation {
  Ptr                 p;
} CMPtrLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* This structure is deprecated in Mac OS X 10.5.*/
typedef struct CMProcedureLocation {
  CMProfileAccessUPP  proc;
  void *              refCon;
} CMProcedureLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif // !__LP64__ && !TARGET_OS_WIN32

typedef struct CMPathLocation {
#if TARGET_OS_WIN32
  TCHAR               path[WIN_CS_MAX_PATH];
#else
  char                path[CS_MAX_PATH];
#endif
} CMPathLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMBufferLocation {
  void *              buffer;
  UInt32              size;
} CMBufferLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef union CMProfLoc {
#if !__LP64__ && !TARGET_OS_WIN32
  CMFileLocation      fileLoc;
#endif // !__LP64__ && !TARGET_OS_WIN32
  CMHandleLocation    handleLoc;
#if !__LP64__ && !TARGET_OS_WIN32
  CMPtrLocation       ptrLoc;
  CMProcedureLocation procLoc;
#endif // !__LP64__ && !TARGET_OS_WIN32
  CMPathLocation      pathLoc;
  CMBufferLocation    bufferLoc;
} CMProfLoc DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct CMProfileLocation {
  short               locType;
  CMProfLoc           u;
} CMProfileLocation DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

enum {
  cmOriginalProfileLocationSize = 72,
  cmCurrentProfileLocationSize  = sizeof(CMProfileLocation)
};


/* Struct and enums used for Profile iteration */

enum {
  cmProfileIterateDataVersion1  = 0x00010000,
  cmProfileIterateDataVersion2  = 0x00020000, /* Added makeAndModel*/
  cmProfileIterateDataVersion3  = 0x00030000, /* Added MD5 digest*/
  cmProfileIterateDataVersion4  = 0x00040000  /* Only path based locations*/
};

typedef struct CMProfileIterateData {
  UInt32              dataVersion;            /* cmProfileIterateDataVersion2 */
  CM2Header           header;
  ScriptCode          code;
  Str255              name;
  CMProfileLocation   location;
  UniCharCount        uniCodeNameCount;
  UniChar *           uniCodeName;
  unsigned char *     asciiName;
  CMMakeAndModel *    makeAndModel;
  CMProfileMD5 *      digest;                 /* Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm */
} CMProfileIterateData DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Caller-supplied callback function for Profile iteration */

typedef CALLBACK_API( OSErr , CMProfileIterateProcPtr )(CMProfileIterateData *iterateData, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMProfileIterateProcPtr)                 CMProfileIterateUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Caller-supplied callback function for CMM iteration */

typedef CALLBACK_API( OSErr , CMMIterateProcPtr )(CMMInfo *iterateData, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef STACK_UPP_TYPE(CMMIterateProcPtr)                       CMMIterateUPP DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


typedef CALLBACK_API_C( void , CMLabToLabProcPtr )(float *L, float *a, float *b, void *refcon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;





/* Notifications */

/*
 *  Clients can register for notifications of ColorSync preference changes by
 *  using the kCMPrefsChangedNotification key. This notification will be sent
 *  if the user changes ColorSync preferences such as:
 *      the default profile by colors space, (CMSetDefaultProfileBySpace)
 *      the default profile by device useage, (CMSetDefaultProfileByUse)
 *      or the preferred CMM.
 *  See <CMDeviceIntegration.h> for more notifications that can be sent.
 */
#define    kCMPrefsChangedNotification     CFSTR("AppleColorSyncPreferencesChangedNotification")



#if !__LP64__
#if defined(_MSC_VER)
#pragma pack(pop, CMApplication_h)
#else
#pragma options align=reset
#endif
#endif

#ifdef __cplusplus
}
#endif

#pragma mark --- CMFloatBitmap.h ----

#ifdef __cplusplus
extern "C" {
#endif


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint on
#endif

enum CMFloatBitmapFlags {
  kCMFloatBitmapFlagsNone       = 0,
  kCMFloatBitmapFlagsAlpha      = 1,
  kCMFloatBitmapFlagsAlphaPremul = 2,
  kCMFloatBitmapFlagsRangeClipped = 4
};
typedef enum CMFloatBitmapFlags CMFloatBitmapFlags;

/*!
    @struct     CMFloatBitmap
    @abstract       A new struture that defines and arbritrary map of float color values.
    @discussion     The struture defines a pixel array of dimensions [height][width][chans] 
                    where 'chans' is the number of channels in the colorspace plus an optional one for alpha.
                    The actual memory pointed to by the structure can contain a variety of possible arrangements. 
                    The actual data values can be chuncky or planar. The channels can by in any order.
<PRE>

Examples:
a) float* p contains a 640w by 480h bitmap of chunky RGB data
    CMFloatBitmap map = { 0,         // version
                {p, p+1, p+2},       // base addrs of R,G,B
                480, 640,            // height, width
                640*3,               // rowStride
                3,                   // colStride
                cmRGBData,
                kCMFloatBitmapFlagsNone};
b) float* p contains a 640w by 480h bitmap of chunky BGRA data
    CMFloatBitmap map = { 0,         // version
                {p+2, p+1, p, p+3},  // base addrs of R,G,B,A
                480, 640,            // height, width
                640*4,               // rowStride
                3,                   // colStride
                cmRGBData,
                kCMFloatBitmapFlagsAlpha};
c) float* p contains a 640w by 480h bitmap of planar CMYK data
    CMFloatBitmap map = { 0,        // version
                {p, p+640*480 , p+2*640*480 , p+3*640*480},
                480, 640,           // height, width
                640,                // rowStride
                1,                  // colStride
                cmCMYKData,
                kCMFloatBitmapFlagsNone};
</PRE>
        
    @field      version     The version number of this structure to allow for future expansion.
                            Should contain 0 for now.
    
    @field      buffers     The base address for each channel in canonical order.
                            The canonical order for RGB is R,G,B. CMYK is C,M,Y,K etc.
                            A maximum of sixteen channels is supported.
                            Another way to think of this is 
                                buffers[c] = &(pixelArray[0][0][c])
                                
    @field      height      The height (in pixels) of the bitmap.

    @field      width       The width (in pixels) of the bitmap.

    @field      rowStride   The number of floats to skip to move from one row to the next.
                            This is typically (width*chans) for chunky pixel buffers or (width) for planar.
                            Can be negative if the image is vertically flipped.

    @field      colStride   The number of floats to skip to move from one column to the next.
                            This is typically (chans) for chunky pixel buffers or (1) for planar.
                            Can be negative if the image is horizontally flipped.

    @field      space       The colorspace of the data (e.g cmRGBdata cmCMYKData)

    @field      flags       Holds bits to specify the alpha type of the data.
                            The remaining bits are reserved for future use.

*/
typedef struct CMFloatBitmap {
  unsigned long       version;
  float *             buffers[16];
  size_t              height;
  size_t              width;
  ptrdiff_t           rowStride;
  ptrdiff_t           colStride;
  OSType              space;
  CMFloatBitmapFlags  flags;
} CMFloatBitmap DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
 
 
 

#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
#endif

#ifdef __cplusplus
}
#endif

#pragma mark --- CMMComponent.h ----

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#pragma mark --- CMScriptingPlugin.h ----

#if !__LP64__ && !TARGET_OS_WIN32

#ifdef __cplusplus
extern "C" {
#endif

enum {
                                        /* ColorSync Scripting Errors */
  cmspInvalidImageFile          = -4220, /* Plugin cannot handle this image file type */
  cmspInvalidImageSpace         = -4221, /* Plugin cannot create an image file of this colorspace */
  cmspInvalidProfileEmbed       = -4222, /* Specific invalid profile errors */
  cmspInvalidProfileSource      = -4223,
  cmspInvalidProfileDest        = -4224,
  cmspInvalidProfileProof       = -4225,
  cmspInvalidProfileLink        = -4226
};


/**** embedFlags field  ****/
/* reserved for future use: currently 0 */

/**** matchFlags field  ****/
enum {
  cmspFavorEmbeddedMask         = 0x00000001 /* if bit 0 is 0 then use srcProf profile, if 1 then use profile embedded in image if present*/
};





#ifdef __cplusplus
}
#endif

#endif // !__LP64__ && !TARGET_OS_WIN32

#pragma mark -- CMDeviceIntegration.h

#ifdef __cplusplus
extern "C" {
#endif

#if !__LP64__ && !TARGET_OS_WIN32
#pragma options align=mac68k
#endif
/*
    The current versions of the data structure
    containing information on registered devices.
*/
enum {
  cmDeviceInfoVersion1          = 0x00010000,
  cmDeviceProfileInfoVersion1   = 0x00010000,
  cmDeviceProfileInfoVersion2   = 0x00020000
};

enum {
  cmCurrentDeviceInfoVersion    = cmDeviceInfoVersion1,
  cmCurrentProfileInfoVersion   = cmDeviceProfileInfoVersion1
};

/*
    Certain APIs require a device ID or profile ID.  
    In some cases, a "default ID" can be used.
*/
enum {
  cmDefaultDeviceID             = 0,
  cmDefaultProfileID            = 0
};

/*
    Possible values for device states accessible by the
    CMGetDeviceState() and CMSetDeviceState() APIs.
*/
enum {
  cmDeviceStateDefault          = 0x00000000,
  cmDeviceStateOffline          = 0x00000001,
  cmDeviceStateBusy             = 0x00000002,
  cmDeviceStateForceNotify      = (int)0x80000000,
  cmDeviceStateDeviceRsvdBits   = 0x00FF0000,
  cmDeviceStateAppleRsvdBits    = (int)0xFF00FFFF
};

/*
    Possible values for flags passed to the
    CMIterateDeviceProfiles() API.
    
    "Factory" profiles are registered via the
    CMSetDeviceFactoryProfiles() API.
    
    "Custom" profiles are those which are meant to take
    the place of the factory profiles, as a result of
    customization or calibration.  These profiles are
    registered via the CMSetDeviceProfiles() API.
    
    To retrieve all of the the former for all devices,
    use cmIterateFactoryDeviceProfiles as the flags
    value when calling CMIterateDeviceProfiles().
    
    To retrieve only the latter for all devices, use
    the cmIterateCustomDeviceProfiles, as the flags
    value when calling CMIterateDeviceProfiles().
    
    To get the profiles in use for all devices, use
    cmIterateCurrentDeviceProfiles as the flags value.
    This will replace the factory profiles with any
    overrides, yielding the currently used set.
    
    To get all profiles, without replacement, use
    cmIterateAllDeviceProfiles. 
*/
enum {
  cmIterateFactoryDeviceProfiles = 0x00000001,
  cmIterateCustomDeviceProfiles = 0x00000002,
  cmIterateCurrentDeviceProfiles = 0x00000003,
  cmIterateAllDeviceProfiles    = 0x00000004,
  cmIterateDeviceProfilesMask   = 0x0000000F
};

/*
    Errors returned by CMDeviceIntegration APIs
*/
enum {
  cmDeviceDBNotFoundErr         = -4227, /* Prefs not found/loaded */
  cmDeviceAlreadyRegistered     = -4228, /* Re-registration of device */
  cmDeviceNotRegistered         = -4229, /* Device not found */
  cmDeviceProfilesNotFound      = -4230, /* Profiles not found */
  cmInternalCFErr               = -4231, /* CoreFoundation failure */
  cmPrefsSynchError             = -4232  /* CFPreferencesSynchronize failed */
};


/*
   Clients can register for notifications of device changes:
      Notification         Description                           Sent by API
    ----------------      -----------                           -----------
      DeviceRegistered      a device was registered               CMRegisterColorDevice()  
      DeviceUnregistered    a device was unregistered             CMUnregisterColorDevice()
      DeviceOnline          a device's state changed to Online    CMSetDeviceState()
      DeviceOffline         a device's state changed to Offline   CMSetDeviceState()
      DeviceState           a device's state changed              CMSetDeviceState()
      DefaultDevice         a class' default device changed       CMSetDefaultDevice()
      DeviceProfiles        a device's profiles changed           CMSetDeviceFactoryProfiles(), CMSetDeviceProfiles()
      DefaultDeviceProfile  a device's default profile ID changed CMSetDeviceDefaultProfileID()
      DisplayDeviceProfiles a display device's profiles changed   CMSetDeviceFactoryProfiles(), CMSetDeviceProfiles()
*/
#define kCMDeviceRegisteredNotification     CFSTR("CMDeviceRegisteredNotification")
#define kCMDeviceUnregisteredNotification   CFSTR("CMDeviceUnregisteredNotification")
#define kCMDeviceOnlineNotification         CFSTR("CMDeviceOnlineNotification")
#define kCMDeviceOfflineNotification        CFSTR("CMDeviceOfflineNotification")
#define kCMDeviceStateNotification          CFSTR("CMDeviceStateNotification")
#define kCMDefaultDeviceNotification        CFSTR("CMDefaultDeviceNotification")
#define kCMDeviceProfilesNotification       CFSTR("CMDeviceProfilesNotification")
#define kCMDefaultDeviceProfileNotification CFSTR("CMDefaultDeviceProfileNotification")
#define kCMDisplayDeviceProfilesNotification CFSTR("CMDisplayDeviceProfilesNotification")

/*
    Device state data.
*/
typedef UInt32                          CMDeviceState DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
    A CMDeviceID must be unique within a device's class.
*/

typedef UInt32                          CMDeviceID DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
    A CMDeviceProfileID must only be unique per device.
*/
typedef UInt32                          CMDeviceProfileID DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
/*
    DeviceClass type.
*/
enum {
  cmScannerDeviceClass          = 'scnr',
  cmCameraDeviceClass           = 'cmra',
  cmDisplayDeviceClass          = 'mntr',
  cmPrinterDeviceClass          = 'prtr',
  cmProofDeviceClass            = 'pruf'
};

typedef OSType                          CMDeviceClass;

/*
    CMDeviceScope
    Structure specifying a device's or a device setting's scope.
*/
typedef struct CMDeviceScope {
  CFStringRef         deviceUser;             /* kCFPreferencesCurrentUser | _AnyUser */
  CFStringRef         deviceHost;             /* kCFPreferencesCurrentHost | _AnyHost */
} CMDeviceScope, CMDeviceProfileScope DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
    CMDeviceInfo
    Structure containing information on a given device.
*/
typedef struct CMDeviceInfo {
  UInt32              dataVersion;            /* cmDeviceInfoVersion1 */
  CMDeviceClass       deviceClass;            /* device class */
  CMDeviceID          deviceID;               /* device ID */
  CMDeviceScope       deviceScope;            /* device's scope */
  CMDeviceState       deviceState;            /* Device State flags */
  CMDeviceProfileID   defaultProfileID;       /* Can change */
  CFDictionaryRef *   deviceName;             /* Ptr to storage for CFDictionary of */
                                              /* localized device names (could be nil) */
  UInt32              profileCount;           /* Count of registered profiles */
  UInt32              reserved;               /* Reserved for use by ColorSync */
} CMDeviceInfo, *CMDeviceInfoPtr DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/*
    CMDeviceProfileInfo
    Structure containing information on a device profile.
*/
typedef struct CMDeviceProfileInfo {
  UInt32              dataVersion;            /* cmDeviceProfileInfoVersion1 */
  CMDeviceProfileID   profileID;              /* The identifier for this profile */
  CMProfileLocation   profileLoc;             /* The profile's location */
  CFDictionaryRef     profileName;            /* CFDictionary of localized profile names */
  UInt32              reserved;               /* Reserved for use by ColorSync */
} CMDeviceProfileInfo DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

typedef struct NCMDeviceProfileInfo {
  UInt32              dataVersion;            /* cmDeviceProfileInfoVersion2 */
  CMDeviceProfileID   profileID;              /* The identifier for this profile */
  CMProfileLocation   profileLoc;             /* The profile's location */
  CFDictionaryRef     profileName;            /* CFDictionary of localized profile names */
  CMDeviceProfileScope  profileScope;         /* The scope this profile applies to */
  UInt32              reserved;               /* Reserved for use by ColorSync */
} NCMDeviceProfileInfo DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/*
    CMDeviceProfileArray
    Structure containing the profiles for a device.
*/

typedef struct CMDeviceProfileArray {
  UInt32              profileCount;           /* Count of profiles in array */
  CMDeviceProfileInfo  profiles[1];           /* The profile info records */
} CMDeviceProfileArray, *CMDeviceProfileArrayPtr DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/*
    Caller-supplied iterator functions
*/

typedef CALLBACK_API_C( OSErr , CMIterateDeviceInfoProcPtr )(const CMDeviceInfo *deviceInfo, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
typedef CALLBACK_API_C( OSErr , CMIterateDeviceProfileProcPtr )(const CMDeviceInfo *deviceInfo, const NCMDeviceProfileInfo *profileInfo, void *refCon) DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;


/*
    Device Registration
*/

/*!
    @function    CMRegisterColorDevice
    @abstract    Registers a device with ColorSync
    @discussion  For a device to be recognized by ColorSync it needs to register itself 
                    via this API.  After calling this API, the CMSetDeviceFactoryProfiles
                    API should be called to specify the initial modes and profiles for the
                    device. Registration need only happen once, when the device is installed.
    @param       deviceClass    (in) Device class to add
    @param       deviceID       (in) Device id to add
    @param       deviceName     (in) Dictionary containing localized names
    @param       deviceScope    (in) Scope where information should be stored
*/

#if !__LP64__ && !TARGET_OS_WIN32
#pragma options align=reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* TARGET_OS_OSX */
#endif /* __COLORSYNCDEPRECATED__ */

