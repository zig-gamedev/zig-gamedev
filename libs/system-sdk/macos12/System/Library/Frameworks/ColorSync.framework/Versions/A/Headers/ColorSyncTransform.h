/*
 * ColorSync - ColorSyncTransform.h
 * Copyright (c)  2008 Apple Inc.
 * All rights reserved.
 */

#ifndef __COLORSYNCTRANSFORM__
#define __COLORSYNCTRANSFORM__

#if !defined(__swift__)

#ifdef __cplusplus
extern "C" {
#endif

#include <ColorSync/ColorSyncBase.h>

typedef struct CF_BRIDGED_TYPE(id) ColorSyncTransform* ColorSyncTransformRef;

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

CSEXTERN CFTypeID ColorSyncTransformGetTypeID(void) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    * returns the CFTypeID for ColorSyncTransforms.
    */

CSEXTERN ColorSyncTransformRef __nullable ColorSyncTransformCreate (CFArrayRef __nullable profileSequence, CFDictionaryRef __nullable options) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   profileSequence  - array of dictionaries, each one containing a profile object and the
    *                      information on the usage of the profile in the transform.
    *               
    *               Required keys:
    *               ==============
    *                      kColorSyncProfile           : ColorSyncProfileRef
    *                      kColorSyncRenderingIntent   : CFStringRef defining rendering intent
    *                      kColorSyncTransformTag      : CFStringRef defining which tags to use 
    *               Optional key:
    *               =============
    *                      kColorSyncBlackPointCompensation : CFBooleanRef to enable/disable BPC
    *   
    *   options      - dictionary with additional public global options (e.g. preferred CMM, quality,
    *                       etc... It can also contain custom options that are CMM specific.
    *
    *   returns ColorSyncTransformRef or NULL in case of failure
    */


CSEXTERN CFTypeRef __nullable ColorSyncTransformCopyProperty(ColorSyncTransformRef transform, CFTypeRef key, CFDictionaryRef __nullable options) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   transform - transform from which to copy the property
    *   key       - CFTypeRef to be used as a key to identify the property
    */


CSEXTERN void ColorSyncTransformSetProperty(ColorSyncTransformRef transform, CFTypeRef key, __nullable CFTypeRef property) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   transform - transform in which to set the property
    *   key       - CFTypeRef to be used as a key to identify the property
    *   property  - CFTypeRef to be set as the property
    */

CSEXTERN CFArrayRef __nullable ColorSyncTransformGetProfileSequence(ColorSyncTransformRef transform) CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
    /*
     *   transform - transform from which to get the profile sequnce used to create the transform
     */

enum ColorSyncDataDepth {
    kColorSync1BitGamut             = 1,
    kColorSync8BitInteger           = 2,
    kColorSync16BitInteger          = 3,
    kColorSync16BitFloat            = 4,
    kColorSync32BitInteger          = 5,
    kColorSync32BitNamedColorIndex  = 6,
    kColorSync32BitFloat            = 7,
    kColorSync10BitInteger          = 8
};

typedef enum ColorSyncDataDepth ColorSyncDataDepth;

enum ColorSyncAlphaInfo {
    kColorSyncAlphaNone,               /* For example, RGB. */
    kColorSyncAlphaPremultipliedLast,  /* For example, premultiplied RGBA */
    kColorSyncAlphaPremultipliedFirst, /* For example, premultiplied ARGB */
    kColorSyncAlphaLast,               /* For example, non-premultiplied RGBA */
    kColorSyncAlphaFirst,              /* For example, non-premultiplied ARGB */
    kColorSyncAlphaNoneSkipLast,       /* For example, RBGX. */
    kColorSyncAlphaNoneSkipFirst,      /* For example, XRGB. */
};

typedef enum ColorSyncAlphaInfo ColorSyncAlphaInfo;

enum {
    kColorSyncAlphaInfoMask = 0x1F,
    
    kColorSyncByteOrderMask = 0x7000,
    kColorSyncByteOrderDefault = (0 << 12),
    kColorSyncByteOrder16Little = (1 << 12),
    kColorSyncByteOrder32Little = (2 << 12),
    kColorSyncByteOrder16Big = (3 << 12),
    kColorSyncByteOrder32Big = (4 << 12)
};


typedef uint32_t ColorSyncDataLayout;

CSEXTERN bool ColorSyncTransformConvert (ColorSyncTransformRef transform, size_t width, size_t height,
                                         void* dst, ColorSyncDataDepth dstDepth,
                                         ColorSyncDataLayout dstLayout, size_t dstBytesPerRow,
                                         const void* src, ColorSyncDataDepth srcDepth,
                                         ColorSyncDataLayout srcLayout, size_t srcBytesPerRow,
                                         __nullable CFDictionaryRef options) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
   /*
    *   transform         - transform to be used for converting color
    *   width             - width of the image in pixels
    *   height            - height of the image in pixels
    *   dst               - a pointer to the destination where the results will be written.
    *   dstDepth          - describes the bit depth and type of the destination color components
    *   dstFormat         - describes the format and byte packing of the destination pixels
    *   dstBytesPerRow    - number of bytes in the row of data
    *   src               - a pointer to the data to be converted.
    *   srcDepth          - describes the bit depth and type of the source color components
    *   srcFormat         - describes the format and byte packing of the source pixels
    *   srcBytesPerRow    - number of bytes in the row of data
    *
    *   returns true if conversion was successful or false otherwise 
    */

    /* This function is deprecated for performance reasons. Color conversions are computationally intensive
     * and the recommended way to perform these is by using the vImage Converter with ColorSync Code Fragment.
     * vImage employs vectorized code which is not only faster but also more battery efficient.
     * Please visit the following link to see a sample application of vImage used in conjunction with ColorSync.
     * <https://developer.apple.com/library/prerelease/content/samplecode/convertImage/Listings/convertImage_main_c.html>
     * More details regarding ColorSync Code Fragments are included below, as well.
     */
    
/* Keys and values for profile specific info and options */
CSEXTERN CFStringRef kColorSyncProfile CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
CSEXTERN CFStringRef kColorSyncRenderingIntent CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

    /* Legal values for kColorSyncRenderingIntent */
        CSEXTERN CFStringRef kColorSyncRenderingIntentPerceptual CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncRenderingIntentRelative CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncRenderingIntentSaturation CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncRenderingIntentAbsolute CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncRenderingIntentUseProfileHeader CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

CSEXTERN CFStringRef kColorSyncTransformTag CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

    /* Legal values for kColorSyncTransformTag */
        CSEXTERN CFStringRef kColorSyncTransformDeviceToPCS CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncTransformPCSToPCS CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncTransformPCSToDevice CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncTransformDeviceToDevice CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
        CSEXTERN CFStringRef kColorSyncTransformGamutCheck CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

CSEXTERN CFStringRef kColorSyncBlackPointCompensation CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
CSEXTERN CFStringRef kColorSyncExtendedRange CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);


/* Global transform options */
CSEXTERN CFStringRef kColorSyncPreferredCMM CS_AVAILABLE_STARTING(10.4) CS_UNAVAILABLE_EMBEDDED; /* ColorSyncCMMRef of the preferred CMM */
CSEXTERN CFStringRef kColorSyncConvertQuality CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

    /* Legal values for kColorSyncConvertQuality */
        CSEXTERN CFStringRef kColorSyncBestQuality CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);      /* do not coalesce profile transforms (default) */
        CSEXTERN CFStringRef kColorSyncNormalQuality CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);    /* coalesce all transforms */
        CSEXTERN CFStringRef kColorSyncDraftQuality CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);     /* coalesce all transforms, do not interpolate */

/* Conversion options */
CSEXTERN CFStringRef kColorSyncConvertUseExtendedRange CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED); /* allow float data to exceed [0.0 .. 1.0] range */

/* Public keys for copying transform properties */

CSEXTERN CFStringRef kColorSyncTransformInfo CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);        /* dictionary with the following keys */
    CSEXTERN CFStringRef kColorSyncTransformCreator CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED); /* name of the CMM that created the transform */
    CSEXTERN CFStringRef kColorSyncTransformSrcSpace CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
    CSEXTERN CFStringRef kColorSyncTransformDstSpace CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);

/*
 * =============================
 *
 *  Code Fragment Support
 *
 * =============================
 *
 * ColorSync can return parameters for standard components of color conversion,
 * i.e. tone rendering curves (TRCs), 3x4 matrices (3x3 + 3x1), multi-dimensional
 * interpolation tables and Black Point Compensation.
 * The parameters are wrapped in CFArray or CFData objects specific to the
 * component type and placed in a CFDictionary under a key that identifies the 
 * type of the component. The complete code fragment is a CFArray of component
 * dictionaries that are placed in the in the order they have to be executed.
 * 
 * A code fragment is created by calling ColorSyncTransformCopyProperty with the key 
 * specifying the type of the code fragment to be created. NULL pointer will be
 * returnde if the requested code fragment cannot be created.
 */

/*
 * Types of Code Fragments:
 *
 * 1. Full conversion: all non-NULL components based on all the tags from the
 *                     sequence of profiles passed to create the ColorSyncTransform with
 *                     an exception of adjacent matrices that can be collapsed to
 *                     one matrix.
 * 2. Parametric:      same as above, except that the returned code fragment consists
 *                     only of parametric curves, matrices and BPC components.
 * 3. Simplified:      Full conversion is collapsed to one
 *                     multi-dimensional table with N inputs and M outputs.
 */
 
    
CSEXTERN CFStringRef kColorSyncTransformCodeFragmentType CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);           /* CFSTR("com.apple.cmm.CodeFragmentType")  */
CSEXTERN CFStringRef kColorSyncTransformCodeFragmentMD5 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);            /* CFSTR("com.apple.cmm.CodeFragmentMD5")  */
CSEXTERN CFStringRef kColorSyncTransformFullConversionData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* CFSTR("com.apple.cmm.FullConversion") */
CSEXTERN CFStringRef kColorSyncTransformSimplifiedConversionData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.SimplifiedConversion") */
CSEXTERN CFStringRef kColorSyncTransformParametricConversionData CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParametricConversion") */
CSEXTERN CFStringRef kColorSyncTransformProfileSequnce CS_AVAILABLE_PUBLIC_STARTING(11.0, CS_UNAVAILABLE_PUBLIC_EMBEDDED);             /* CFSTR("com.apple.cmm.ProfileSequnce") */

/*
 * Matrix: represented as a CFArray of three CFArrays of four CFNumbers (Float32)
 *         each, performin the following matrix operation
 *         y[3] = 3x3 matrix *x[3] + 3x1 vector (last column)
 */
CSEXTERN CFStringRef kColorSyncConversionMatrix CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);        /* CFSTR("com.apple.cmm.Matrix") */

/*
 * Tone Rendering Curves:
 *
 * 1. Parametric curves: represented as a CFArray of seven CFNumbers (Float32)
 *                       [gamma a b c d e f]
 *  Curve Type 0  Y = X^gamma
 *  Curve Type 1  Y = (aX+b)^gamma     [X >= -b/a],  Y = 0  [X < -b/a]
 *  Curve Type 2  Y = (aX+b)^gamma + c [X >= -b/a],  Y = c  [X < -b/a]
 *  Curve Type 3  Y = (aX+b)^gamma     [X >= d],     Y = cX [X < d]
 *  Curve Type 4  Y = (aX+b)^gamma + e [X >= d],     Y = cX + f [X < d]
 *
 * 2. 1-dimensional lookup with interpolation:
 *                  represented as CFData containing a Float32 table[gridPoints]
 */
 
CSEXTERN CFStringRef kColorSyncConversionParamCurve0 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParamCurve0") */
CSEXTERN CFStringRef kColorSyncConversionParamCurve1 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParamCurve1") */
CSEXTERN CFStringRef kColorSyncConversionParamCurve2 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParamCurve2") */
CSEXTERN CFStringRef kColorSyncConversionParamCurve3 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParamCurve3") */
CSEXTERN CFStringRef kColorSyncConversionParamCurve4 CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.ParamCurve4") */
CSEXTERN CFStringRef kColorSyncConversion1DLut CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);         /* CFSTR("com.apple.cmm.1D-LUT") */
CSEXTERN CFStringRef kColorSyncConversionGridPoints CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);    /* CFSTR("com.apple.cmm.GridPointCount") */
CSEXTERN CFStringRef kColorSyncConversionChannelID CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);     /* CFSTR("com.apple.cmm.ChannelID") */

/*
 * Multi-dimensional lookup with interpolation:
 *
 *       represented as CFData containing a table for N inputs, M outputs 
 *       and P gridPoints in each direction. The dimension corresponding to the
 *       first input channel varies least rapidly, and the dimension corresponding
 *       to the last input channel varies most rapidly. Each grid point value contains
 *       M uint16_t numbers, one for each of output channels (M is the number of outputs).
 */
 

CSEXTERN CFStringRef kColorSyncConversion3DLut CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);      /* CFSTR("com.apple.cmm.3D-LUT") */
CSEXTERN CFStringRef kColorSyncConversionNDLut CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);      /* CFSTR("com.apple.cmm.ND-LUT") */
CSEXTERN CFStringRef kColorSyncConversionInpChan CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);    /* CFSTR("com.apple.cmm.InputChannels") */
CSEXTERN CFStringRef kColorSyncConversionOutChan CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);    /* CFSTR("com.apple.cmm.OutputChannels") */

/*
 * Black Point Compensation: represented as an CFArray of CFNumbers (Float32)
 *
 * 1. Scaling in Luminance: CFArray containing two numbers.
 * 2. Scaling in XYZ: CFArray containing six numbers.
 */
 
CSEXTERN CFStringRef kColorSyncConversionBPC CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);   /* CFSTR("com.apple.cmm.BPC") */

CSEXTERN CFStringRef kColorSyncFixedPointRange CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED); /* CFSTR("com.apple.cmm.FixedPointRange") */

CSEXTERN CFTypeRef ColorSyncCreateCodeFragment(CFArrayRef profileSequence, CFDictionaryRef options) CS_AVAILABLE_PUBLIC_STARTING(10.4, CS_UNAVAILABLE_PUBLIC_EMBEDDED);
/*
 *   profileSequence  - array of dictionaries, each one containing a profile object and the
 *                      information on the usage of the profile in the code fragment.
 *
 *               Required keys:
 *               ==============
 *                      kColorSyncProfile           : ColorSyncProfileRef
 *                      kColorSyncRenderingIntent   : CFStringRef defining rendering intent
 *                      kColorSyncTransformTag      : CFStringRef defining which tags to use
 *               Optional key:
 *               =============
 *                      kColorSyncBlackPointCompensation : CFBooleanRef to enable/disable BPC
 *                      kColorSyncExtendedRange          : CFBooleanRef to enable/disable extended range; disabling implies floating point conversions

 *
 *   options      - dictionary with additional options as in case of creating ColorSyncTransform
 *
 *   returns CFTypeRef representing ColorSyncCodeFragment or NULL in case of failure
 */
    
CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#ifdef __cplusplus
}
#endif

#else /* #if !defined(__swift__) */

#ifdef __cplusplus
extern "C" {
#endif
    
#include <ColorSync/ColorSyncBase.h>
    
    typedef struct CF_BRIDGED_TYPE(id) ColorSyncTransform* ColorSyncTransformRef;
    
    CSEXTERN CFTypeID ColorSyncTransformGetTypeID(void);
    /*
     * returns the CFTypeID for ColorSyncTransforms.
     */
    
    CSEXTERN ColorSyncTransformRef ColorSyncTransformCreate (CFArrayRef profileSequence, CFDictionaryRef options);
    /*
     *   profileSequence  - array of dictionaries, each one containing a profile object and the
     *                      information on the usage of the profile in the transform.
     *
     *               Required keys:
     *               ==============
     *                      kColorSyncProfile           : ColorSyncProfileRef
     *                      kColorSyncRenderingIntent   : CFStringRef defining rendering intent
     *                      kColorSyncTransformTag      : CFStringRef defining which tags to use
     *               Optional key:
     *               =============
     *                      kColorSyncBlackPointCompensation : CFBooleanRef to enable/disable BPC
     *                      kColorSyncExtendedRange          : CFBooleanRef to enable/disable extended range; disabling implies floating point conversions
     *
     *   options      - dictionary with additional public global options (e.g. preferred CMM, quality,
     *                       etc... It can also contain custom options that are CMM specific.
     *
     *   returns ColorSyncTransformRef or NULL in case of failure
     */
    
    
    CSEXTERN CFTypeRef ColorSyncTransformCopyProperty(ColorSyncTransformRef transform, CFTypeRef key, CFDictionaryRef options);
    /*
     *   transform - transform from which to copy the property
     *   key       - CFTypeRef to be used as a key to identify the property
     */
    
    
    CSEXTERN void ColorSyncTransformSetProperty(ColorSyncTransformRef transform, CFTypeRef key, CFTypeRef property);
    /*
     *   transform - transform in which to set the property
     *   key       - CFTypeRef to be used as a key to identify the property
     *   property  - CFTypeRef to be set as the property
     */

    CSEXTERN CFArrayRef ColorSyncTransformGetProfileSequence(ColorSyncTransformRef transform);
    /*
     *   transform - transform from which to get the profile sequnce used to create the transform
     */

    enum ColorSyncDataDepth {
        kColorSync1BitGamut             = 1,
        kColorSync8BitInteger           = 2,
        kColorSync16BitInteger          = 3,
        kColorSync16BitFloat            = 4,
        kColorSync32BitInteger          = 5,
        kColorSync32BitNamedColorIndex  = 6,
        kColorSync32BitFloat            = 7,
        kColorSync10BitInteger          = 8
    };
    
    typedef enum ColorSyncDataDepth ColorSyncDataDepth;
    
    enum ColorSyncAlphaInfo {
        kColorSyncAlphaNone,               /* For example, RGB. */
        kColorSyncAlphaPremultipliedLast,  /* For example, premultiplied RGBA */
        kColorSyncAlphaPremultipliedFirst, /* For example, premultiplied ARGB */
        kColorSyncAlphaLast,               /* For example, non-premultiplied RGBA */
        kColorSyncAlphaFirst,              /* For example, non-premultiplied ARGB */
        kColorSyncAlphaNoneSkipLast,       /* For example, RBGX. */
        kColorSyncAlphaNoneSkipFirst,      /* For example, XRGB. */
    };
    
    typedef enum ColorSyncAlphaInfo ColorSyncAlphaInfo;
    
    enum {
        kColorSyncAlphaInfoMask = 0x1F,
        
        kColorSyncByteOrderMask = 0x7000,
        kColorSyncByteOrderDefault = (0 << 12),
        kColorSyncByteOrder16Little = (1 << 12),
        kColorSyncByteOrder32Little = (2 << 12),
        kColorSyncByteOrder16Big = (3 << 12),
        kColorSyncByteOrder32Big = (4 << 12)
    };
    
    
    typedef uint32_t ColorSyncDataLayout;
    
    CSEXTERN bool ColorSyncTransformConvert (ColorSyncTransformRef transform, size_t width, size_t height,
                                             void* dst, ColorSyncDataDepth dstDepth,
                                             ColorSyncDataLayout dstLayout, size_t dstBytesPerRow,
                                             const void* src, ColorSyncDataDepth srcDepth,
                                             ColorSyncDataLayout srcLayout, size_t srcBytesPerRow,
                                             CFDictionaryRef options);
    /*
     *   transform         - transform to be used for converting color
     *   width             - width of the image in pixels
     *   height            - height of the image in pixels
     *   dst               - a pointer to the destination where the results will be written.
     *   dstDepth          - describes the bit depth and type of the destination color components
     *   dstFormat         - describes the format and byte packing of the destination pixels
     *   dstBytesPerRow    - number of bytes in the row of data
     *   src               - a pointer to the data to be converted.
     *   srcDepth          - describes the bit depth and type of the source color components
     *   srcFormat         - describes the format and byte packing of the source pixels
     *   srcBytesPerRow    - number of bytes in the row of data
     *
     *   returns true if conversion was successful or false otherwise
     */
    
    
    /* Keys and values for profile specific info and options */
    CSEXTERN CFStringRef kColorSyncProfile;
    CSEXTERN CFStringRef kColorSyncRenderingIntent;
    
    /* Legal values for kColorSyncRenderingIntent */
    CSEXTERN CFStringRef kColorSyncRenderingIntentPerceptual;
    CSEXTERN CFStringRef kColorSyncRenderingIntentRelative;
    CSEXTERN CFStringRef kColorSyncRenderingIntentSaturation;
    CSEXTERN CFStringRef kColorSyncRenderingIntentAbsolute;
    CSEXTERN CFStringRef kColorSyncRenderingIntentUseProfileHeader;
    
    CSEXTERN CFStringRef kColorSyncTransformTag;
    
    /* Legal values for kColorSyncTransformTag */
    CSEXTERN CFStringRef kColorSyncTransformDeviceToPCS;
    CSEXTERN CFStringRef kColorSyncTransformPCSToPCS;
    CSEXTERN CFStringRef kColorSyncTransformPCSToDevice;
    CSEXTERN CFStringRef kColorSyncTransformDeviceToDevice;
    CSEXTERN CFStringRef kColorSyncTransformGamutCheck;
    
    CSEXTERN CFStringRef kColorSyncBlackPointCompensation;
    CSEXTERN CFStringRef kColorSyncExtendedRange;

    /* Global transform options */
    CSEXTERN CFStringRef kColorSyncPreferredCMM;       /* ColorSyncCMMRef of the preferred CMM */
    CSEXTERN CFStringRef kColorSyncConvertQuality;
    
    /* Legal values for kColorSyncConvertQuality */
    CSEXTERN CFStringRef kColorSyncBestQuality;      /* do not coalesce profile transforms (default) */
    CSEXTERN CFStringRef kColorSyncNormalQuality;    /* coalesce all transforms */
    CSEXTERN CFStringRef kColorSyncDraftQuality;     /* coalesce all transforms, do not interpolate */
    
    /* Conversion options */
    CSEXTERN CFStringRef kColorSyncConvertUseExtendedRange; /* allow float data to exceed [0.0 .. 1.0] range */
    
    /* Public keys for copying transform properties */
    
    CSEXTERN CFStringRef kColorSyncTranformInfo;         /* dictionary with the following keys */
    CSEXTERN CFStringRef kColorSyncTransformCreator; /* name of the CMM that created the transform */
    CSEXTERN CFStringRef kColorSyncTransformSrcSpace;
    CSEXTERN CFStringRef kColorSyncTransformDstSpace;
    
    /*
     * =============================
     *
     *  Code Fragment Support
     *
     * =============================
     *
     * ColorSync can return parameters for standard components of color conversion,
     * i.e. tone rendering curves (TRCs), 3x4 matrices (3x3 + 3x1), multi-dimensional
     * interpolation tables and Black Point Compensation.
     * The parameters are wrapped in CFArray or CFData objects specific to the
     * component type and placed in a CFDictionary under a key that identifies the
     * type of the component. The complete code fragment is a CFArray of component
     * dictionaries that are placed in the in the order they have to be executed.
     *
     * A code fragment is created by calling ColorSyncTransformCopyProperty with the key
     * specifying the type of the code fragment to be created. NULL pointer will be
     * returnde if the requested code fragment cannot be created.
     */
    
    /*
     * Types of Code Fragments:
     *
     * 1. Full conversion: all non-NULL components based on all the tags from the
     *                     sequence of profiles passed to create the ColorSyncTransform with
     *                     an exception of adjacent matrices that can be collapsed to
     *                     one matrix.
     * 2. Parametric:      same as above, except that the returned code fragment consists
     *                     only of parametric curves, matrices and BPC components.
     * 3. Simplified:      Full conversion is collapsed to one
     *                     multi-dimensional table with N inputs and M outputs.
     */
    
    
    CSEXTERN CFStringRef kColorSyncTransformCodeFragmentType;           /* CFSTR("com.apple.cmm.CodeFragmentType")  */
    CSEXTERN CFStringRef kColorSyncTransformCodeFragmentMD5;            /* CFSTR("com.apple.cmm.CodeFragmentMD5")  */
    CSEXTERN CFStringRef kColorSyncTransformFullConversionData;         /* CFSTR("com.apple.cmm.FullConversion") */
    CSEXTERN CFStringRef kColorSyncTransformSimplifiedConversionData;   /* CFSTR("com.apple.cmm.SimplifiedConversion") */
    CSEXTERN CFStringRef kColorSyncTransformParametricConversionData;   /* CFSTR("com.apple.cmm.ParametricConversion") */
    CSEXTERN CFStringRef kColorSyncTransformProfileSequnce;             /* CFSTR("com.apple.cmm.ProfileSequnce") */

    /*
     * Matrix: represented as a CFArray of three CFArrays of four CFNumbers (Float32)
     *         each, performin the following matrix operation
     *         y[3] = 3x3 matrix *x[3] + 3x1 vector (last column)
     */
    CSEXTERN CFStringRef kColorSyncConversionMatrix;        /* CFSTR("com.apple.cmm.Matrix") */
    
    /*
     * Tone Rendering Curves:
     *
     * 1. Parametric curves: represented as a CFArray of seven CFNumbers (Float32)
     *                       [gamma a b c d e f]
     *  Curve Type 0  Y = X^gamma
     *  Curve Type 1  Y = (aX+b)^gamma     [X >= -b/a],  Y = 0  [X < -b/a]
     *  Curve Type 2  Y = (aX+b)^gamma + c [X >= -b/a],  Y = c  [X < -b/a]
     *  Curve Type 3  Y = (aX+b)^gamma     [X >= d],     Y = cX [X < d]
     *  Curve Type 4  Y = (aX+b)^gamma + e [X >= d],     Y = cX + f [X < d]
     *
     * 2. 1-dimensional lookup with interpolation:
     *                  represented as CFData containing a Float32 table[gridPoints]
     */
    
    CSEXTERN CFStringRef kColorSyncConversionParamCurve0;   /* CFSTR("com.apple.cmm.ParamCurve0") */
    CSEXTERN CFStringRef kColorSyncConversionParamCurve1;   /* CFSTR("com.apple.cmm.ParamCurve1") */
    CSEXTERN CFStringRef kColorSyncConversionParamCurve2;   /* CFSTR("com.apple.cmm.ParamCurve2") */
    CSEXTERN CFStringRef kColorSyncConversionParamCurve3;   /* CFSTR("com.apple.cmm.ParamCurve3") */
    CSEXTERN CFStringRef kColorSyncConversionParamCurve4;   /* CFSTR("com.apple.cmm.ParamCurve4") */
    CSEXTERN CFStringRef kColorSyncConversion1DLut;         /* CFSTR("com.apple.cmm.1D-LUT") */
    CSEXTERN CFStringRef kColorSyncConversionGridPoints;    /* CFSTR("com.apple.cmm.GridPointCount") */
    CSEXTERN CFStringRef kColorSyncConversionChannelID;     /* CFSTR("com.apple.cmm.ChannelID") */
    
    /*
     * Multi-dimensional lookup with interpolation:
     *
     *       represented as CFData containing a table for N inputs, M outputs
     *       and P gridPoints in each direction. The dimension corresponding to the
     *       first input channel varies least rapidly, and the dimension corresponding
     *       to the last input channel varies most rapidly. Each grid point value contains
     *       M uint16_t numbers, one for each of output channels (M is the number of outputs).
     */
    
    
    CSEXTERN CFStringRef kColorSyncConversion3DLut;      /* CFSTR("com.apple.cmm.3D-LUT") */
    CSEXTERN CFStringRef kColorSyncConversionNDLut;      /* CFSTR("com.apple.cmm.ND-LUT") */
    CSEXTERN CFStringRef kColorSyncConversionInpChan;    /* CFSTR("com.apple.cmm.InputChannels") */
    CSEXTERN CFStringRef kColorSyncConversionOutChan;    /* CFSTR("com.apple.cmm.OutputChannels") */
    
    /*
     * Black Point Compensation: represented as an CFArray of CFNumbers (Float32)
     *
     * 1. Scaling in Luminance: CFArray containing two numbers.
     * 2. Scaling in XYZ: CFArray containing six numbers.
     */
    
    CSEXTERN CFStringRef kColorSyncConversionBPC;   /* CFSTR("com.apple.cmm.BPC") */
    
    CSEXTERN CFStringRef kColorSyncFixedPointRange; /* CFSTR("com.apple.cmm.FixedPointRange") */
    
#ifdef __cplusplus
}
#endif

#endif /* #if !defined(__swift__) */

#endif /* __COLORSYNCTRANSFORM__ */
