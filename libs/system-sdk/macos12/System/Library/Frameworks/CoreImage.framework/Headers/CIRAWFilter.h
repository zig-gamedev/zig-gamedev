/* CoreImage - CIRAWFilter.h
 
 Copyright (c) 2006 Apple, Inc.
 All rights reserved. */


#import <CoreImage/CIFilter.h>
#import <CoreImage/CoreImageDefines.h>
#import <CoreVideo/CoreVideo.h>
#import <ImageIO/CGImageProperties.h>

@class NSURL;
@class NSDictionary;
@class NSData;

NS_ASSUME_NONNULL_BEGIN

NS_CLASS_AVAILABLE(12_0, 15_0)
@interface CIRAWFilter : CIFilter

typedef NSString* CIRAWDecoderVersion NS_TYPED_ENUM;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersionNone;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion8;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion8DNG;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion7;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion7DNG;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion6;
CORE_IMAGE_EXPORT CIRAWDecoderVersion const CIRAWDecoderVersion6DNG;

/* Returns a NSArray containing the names of all supported camera models. */
@property(class, readonly) NSArray<NSString*>* supportedCameraModels;

/* Array of all supported decoder versions for the given image type, sorted in increasingly newer order. All entries would represent a valid version identifier set to 'decoderVersion'.*/
@property(readonly) NSArray<CIRAWDecoderVersion>* supportedDecoderVersions;

/* The full native size of the unscaled image. This is not affected by changing orientation */
@property(readonly) CGSize nativeSize;

/* Metadata properties CIRAWFilter instance
   This is a dictionary with the same contents as CGImageSourceCopyProperties. */
@property(readonly) NSDictionary* properties;

/* Changing this value allows the user to change the orientation of the image.
   The valid values are in range 1...8 and follow the EXIF specification. */
@property(readwrite) CGImagePropertyOrientation orientation;

/* Setting Draft Mode to true can improve image decoding speed with minimal loss of quality. */
@property(readwrite, getter= isDraftModeEnabled) BOOL draftModeEnabled;

/* Version string representing the decoder version to be used. A newly initialized object defaults to the newest available decoder version for the given image type. User can request an alternative, older version in order to maintain compatibility with older releases. Must be one of supportedDecoderVersions (above), otherwise a nil output image will be generated. */
@property(readwrite, retain) CIRAWDecoderVersion decoderVersion;

/* The desired scale factor at which the image will be eventually drawn. Setting this value can greatly improve the drawing performance.
   A value of 1.0 results in a full size output image, values smaller than 1 will result in a smaller output image.
   The value should be the range of 0...1. Default value is 1 */
@property(readwrite) float scaleFactor;

/* A value controlling the amount of exposure to apply to the image. Default value is 0. */
@property(readwrite) float exposure;

/* A value for baseline exposure applied to the image. Default value can vary based on the image. */
@property(readwrite) float baselineExposure;

/* A value controlling an amount to subtract from shadows the image. Default value can vary based on the image. */
@property(readwrite) float shadowBias;

/* A value to control the amount of global tone curve to apply to the image.
   A value of 0 indicates no global tone curve, i.e. linear response.
   A value of 1 indicates full global tone curve.
   The value should be the range of 0...1.  The default value is 1. */
@property(readwrite) float boostAmount;

/* The amount to boost the shadow areas of the image. Can be used to lighten details in shadows. This has no effect if the boostAmount is 0.
   A value less than 1 will darken the shadows.  A value greater than 1 will lighten the shadows.
   The value should be the range of 0...2.  The default value is 1. */
@property(readwrite) float boostShadowAmount;

/* A boolean value to control if gamut mapping is enabled or not.
   The default value is true. */
@property(readwrite, getter= isGamutMappingEnabled) BOOL gamutMappingEnabled;

/* A boolean value to control if gamut mapping is enabled or not.
   The default value can vary based on the image.
   The 'isLensCorrectionSupported' property is false if the current image doesn't support this setting. */
@property(readonly, getter= isLensCorrectionSupported) BOOL lensCorrectionSupported;
@property(readwrite, getter= isLensCorrectionEnabled) BOOL lensCorrectionEnabled;

/* A value to control the amount of luminance noise reduction to apply to the image.
   A value of 0 indicates no luminance noise reduction.
   A value of 1 indicates maximum luminance noise reduction.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isLuminanceNoiseReductionSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isLuminanceNoiseReductionSupported) BOOL luminanceNoiseReductionSupported;
@property(readwrite) float luminanceNoiseReductionAmount;

/* A value to control the amount of chroma noise reduction to apply to the image.
   A value of 0 indicates no chroma noise reduction.
   A value of 1 indicates maximum chroma noise reduction.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isColorNoiseReductionSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isColorNoiseReductionSupported) BOOL colorNoiseReductionSupported;
@property(readwrite) float colorNoiseReductionAmount;

/* A value to control the amount of sharpness to apply to the edges of the image.
   A value of 0 indicates no sharpness applied.
   A value of 1 indicates maximum sharpness applied.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isSharpnessSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isSharpnessSupported) BOOL sharpnessSupported;
@property(readwrite) float sharpnessAmount;

/* A value to control the amount of local contrast to apply to the edges of the image.
   A value of 0 indicates no contrast applied.
   A value of 1 indicates maximum contrast applied.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isContrastSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isContrastSupported) BOOL contrastSupported;
@property(readwrite) float contrastAmount;

/* A value to control the amount of detail enhancement to apply to the edges of the image.
   A value of 0 indicates no detail enhancement.
   A value of 3 indicates maximum detail enhancement.
   The value should be the range of 0...3.  The default value will vary per image.
   The 'isDetailSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isDetailSupported) BOOL detailSupported;
@property(readwrite) float detailAmount;

/* A value to control the amount of moire artifact reduction to apply to high frequency areas of the image.
   A value of 0 indicates no moire reduction.
   A value of 3 indicates maximum moire reduction.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isMoireReductionSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isMoireReductionSupported) BOOL moireReductionSupported;
@property(readwrite) float moireReductionAmount;

/* A value to control the amount of local tone curve to apply to the image.
   A value of 0 indicates no local tone curve, i.e. linear response.
   A value of 1 indicates full global tone curve.
   The value should be the range of 0...1.  The default value will vary per image.
   The 'isLocalToneMapSupported' property is false if the current image doesn't support this adjustment. */
@property(readonly, getter= isLocalToneMapSupported) BOOL localToneMapSupported;
@property(readwrite) float localToneMapAmount;

/* Allows the output to have an Extended Dynamic Range with values greater than 1 possible.
   A value of 0 indicates no extended dynamic range output.
   A value of 1 indicates default extended dynamic range output.
   A value of 2 indicates maximum extended dynamic range output.
   The value should be the range of 0...2.  The default value is 0. */
@property(readwrite) float extendedDynamicRangeAmount;

/* These properties provide differnet was to query or modify the image white balance
   Use 'neutralChromaticity' to query or change white balance based on x,y chromaticity values in the range (0..1)
   Use 'neutralTemperature' and 'neutralTemperature' to query or change white balance based temp/tint values in the ranges (2000K..50000K, -150..150)
   Use 'neutralLocation' to change the white balance based on an x,y pixel coordiniates in the image.
 */
@property(readwrite) CGPoint neutralChromaticity;
@property(readwrite) CGPoint neutralLocation;
@property(readwrite) float neutralTemperature;
@property(readwrite) float neutralTint;

/* An optional CIFilter to be applied to the RAW image while it is in linear space. */
@property(readwrite, retain, nullable) CIFilter* linearSpaceFilter;

/* Properties for auxiliary images that may be present in the file */
@property(readonly, nullable) CIImage* previewImage;
@property(readonly, nullable) CIImage* portraitEffectsMatte;
@property(readonly, nullable) CIImage* semanticSegmentationSkinMatte;
@property(readonly, nullable) CIImage* semanticSegmentationHairMatte;
@property(readonly, nullable) CIImage* semanticSegmentationGlassesMatte;
@property(readonly, nullable) CIImage* semanticSegmentationSkyMatte;
@property(readonly, nullable) CIImage* semanticSegmentationTeethMatte;

+ (nullable instancetype)filterWithImageURL:(NSURL *)url;

+ (nullable instancetype)filterWithImageData:(NSData *)data
							  identifierHint:(nullable NSString *)identifierHint;

+ (nullable instancetype)filterWithCVPixelBuffer:(CVPixelBufferRef)buffer
									  properties:(NSDictionary *)properties;

@end

NS_ASSUME_NONNULL_END

API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
typedef NSString * CIRAWFilterOption NS_TYPED_ENUM;

/** CIRAWFilter is a category of CIFilter which returns a CIFilter initialized with a RAW image. It allows the user to influence key aspects of the raw processing, such as white balance, exposure, sharpening or boost. */

@interface CIFilter (CIRAWFilter)

/** Returns a CIFilter that will in turn return a properly processed CIImage as "outputImage". */
+ (CIFilter *)filterWithImageURL:(NSURL *)url options:(NSDictionary<CIRAWFilterOption, id> *)options API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED));

/** Returns a CIFilter that will in turn return a properly processed CIImage as "outputImage".

 Note that when using this initializer, you should pass in a source type identifier hint (kCGImageSourceTypeIdentifierHint) key/value pair in order to help the decoder determine the file type, as otherwise confusion and incorrect results are possible. */
+ (CIFilter *)filterWithImageData:(NSData *)data options:(NSDictionary<CIRAWFilterOption, id> *)options API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED));

/** Returns a CIFilter that will in turn return a properly processed CIImage as "outputImage".
 
 Note that when using this initializer, you should pass in a CVPixelBufferRef with one of the following Raw pixel format types
    kCVPixelFormatType_14Bayer_GRBG, kCVPixelFormatType_14Bayer_RGGB, kCVPixelFormatType_14Bayer_BGGR, kCVPixelFormatType_14Bayer_GBRG
 as well as the root properties attachment from the CMSampleBufferRef. */
+ (CIFilter *) filterWithCVPixelBuffer:(CVPixelBufferRef)pixelBuffer properties:(NSDictionary *)properties options:(NSDictionary<CIRAWFilterOption, id> *)options API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.12, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED));

/** Returns a NSArray containing the names of all supported RAW cameras. */
+ (NSArray<NSString*> *) supportedRawCameraModels API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.15, API_TO_BE_DEPRECATED), ios(13.0, API_TO_BE_DEPRECATED), tvos(13.0, API_TO_BE_DEPRECATED));

/** NSNumber (BOOL) : Setting Draft Mode to YES can improve image decoding speed without minimal loss of quality.
    The default value is NO. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputAllowDraftModeKey;

/** NSString: Version string representing the decoder version to be used. A newly initialized object defaults to the newest available decoder version for the given image type. User can request an alternative, older version in order to maintain compatibility with older releases. Must be one of kCISupportedDecoderVersions (below), otherwise a nil output image will be generated. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputDecoderVersionKey;

/** NSArray : Array of all supported decoder versions for the given image type, sorted in increasingly newer order. Each entry is a NSString. All entries would represent a valid version identifier to be passed in for kCIInputDecoderVersion. This value can be only read; setting this value will raise an exception.*/
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCISupportedDecoderVersionsKey;



/** @const      kCIInputEVKey
 NSNumber (float) : Exposure adjustment, default = 0.0. Declared in CIFilter.h */

/** @const      kCIInputBiasKey
 NSNumber (float) : Exposure bias adjustment, default = 0.0. Declared in CIFilter.h */

/** NSNumber (float) : A value controlling the amount of baseline exposure applied to the image.
 A value of 0 indicates no baseline exposure, i.e. linear response. Default varies from with camera settings. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.12, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputBaselineExposureKey;

/** NSNumber (float) : A value in the range of 0...1, controlling the amount of boost applied to the image.
    A value of 0 indicates no boost, i.e. linear response. Default is 1, full boost. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputBoostKey;

/** NSNumber (float) : The amount to boost the shadow areas of the image. Can be used to lighten details in shadows. 
    Has no effect if the image used for initialization was not RAW. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputBoostShadowAmountKey;

/** NSNumber (BOOL) : Setting DisableGamutMap to YES disables gamut mapping.
    The default value is NO. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.12, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputDisableGamutMapKey;

/** NSNumber (float): The X value of the chromaticity. You can always query this value and you'll get the current X value for neutral X,Y. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNeutralChromaticityXKey;

/** NSNumber (float): The Y value of the chromaticity. You can always query this value and you'll get the current Y value for neutral X,Y. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNeutralChromaticityYKey;

/** NSNumber (float) : The color temperature to be considered neutral. You can always query this value and you'll get the current value for temperature. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNeutralTemperatureKey;

/** NSNumber (float) : The tint to be considered neutral. You can always query this value and you'll get the current value for tint. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNeutralTintKey;

/** CIVector : (x, y) location in geometric coordinates of the unrotated output image that should be used as neutral. 
    You can't query this value - it's undefined for reading. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNeutralLocationKey;

/** NSNumber (float) : The desired scale factor at which the image will be eventually drawn. Setting this value can greatly improve the drawing performance. A value of 1 would mean identity, values smaller than 1 will result in a smaller output image. Changing the Scale Factor with enabled Draft Mode may also improve performance. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputScaleFactorKey;




/** NSNumber (BOOL) : Normally, an image is loaded in its proper orientation, given the associated metadata gives an indication about the orientation. For special purposes it may be useful to load the image in its physical orientation. The exact meaning of this is dependent on the image in question. The default value is NO. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputIgnoreImageOrientationKey;


/** NSNumber (int) : Overriding this value allows the user to change the orientation of the image. The valid values are in range 1...8 and follow the EXIF specification. Changing this value makes for instance rotation in 90-degree increments easy. The value is disregarded when the kCIInputIgnoreImageOrientationKey flag is set.
 */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputImageOrientationKey;



/** NSNumber (BOOL) : Determines if the default sharpening should be on. default = YES. Has no effect if the image used for initialization was not RAW. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputEnableSharpeningKey;

/** NSNumber (BOOL) : Determines if progressive chromatic noise tracking (based on ISO and exposure time) should be used. default = YES. Has no effect if the image used for initialization was not RAW. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputEnableChromaticNoiseTrackingKey;

/** NSNumber (double) : The amount of noise reduction applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.7, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNoiseReductionAmountKey;

/** NSNumber (double) : The amount of moire reduction applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.13, API_TO_BE_DEPRECATED), ios(11.0, API_TO_BE_DEPRECATED), tvos(11.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputMoireAmountKey;

/** NSNumber (BOOL) : Determines if the default vendor lens correction be on. default = YES if raw image used for initialization contains lens distortion parameters. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputEnableVendorLensCorrectionKey;

/** NSNumber (double) : The amount of luminance noise reduction applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputLuminanceNoiseReductionAmountKey;

/** NSNumber (double) : The amount of color noise reduction applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputColorNoiseReductionAmountKey;

/** NSNumber (double) : The amount of noise reduction sharpness applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNoiseReductionSharpnessAmountKey;

/** NSNumber (double) : The amount of noise reduction contrast applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNoiseReductionContrastAmountKey;

/** NSNumber (double) : The amount of noise reduction detail applied. Range is 0 to 1. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.10, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputNoiseReductionDetailAmountKey;


/** NSNumber (BOOL) : Allows the output to have an Extended Dynamic Range with values greater than 1 possible */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.14, API_TO_BE_DEPRECATED), ios(12.0, API_TO_BE_DEPRECATED), tvos(12.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputEnableEDRModeKey;


/** NSNumber (double): The amount of the local tone map. Range is 0 to 1. Default is 1  */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(11.1, API_TO_BE_DEPRECATED), ios(14.3, API_TO_BE_DEPRECATED), tvos(14.3, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputLocalToneMapAmountKey;


/** CIFilter (id) : CIFilter to be applied to the RAW image while it is in linear space. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.7, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIInputLinearSpaceFilter;

/** CIVector containing the full native size of the unscaled image. The vector's X value is the width, Y is the height. 
    This is not affected by changing either kCIInputIgnoreImageOrientationKey or kCIInputImageOrientationKey. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.5, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIOutputNativeSizeKey;

/** Read-only NSSet containing a list of keys that affect the output image. 
    Depending on the RAW decoder version (kCIInputDecoderVersionKey) and the input image type, 
    some input keys might have no effect. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(10.7, API_TO_BE_DEPRECATED), ios(10.0, API_TO_BE_DEPRECATED), tvos(10.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIActiveKeys;

/** Read-only NSDictionary containing properties for the output image. */
API_DEPRECATED("Use new CIRAWFilter class instead.", macos(12.0, API_TO_BE_DEPRECATED), ios(15.0, API_TO_BE_DEPRECATED), tvos(15.0, API_TO_BE_DEPRECATED))
CORE_IMAGE_EXPORT CIRAWFilterOption const kCIPropertiesKey;

@end

