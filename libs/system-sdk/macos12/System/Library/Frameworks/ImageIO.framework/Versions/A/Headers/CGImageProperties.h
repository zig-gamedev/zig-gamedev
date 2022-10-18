/*
 * ImageIO - CGImageProperties.h
 * Copyright (c) 2004-2016 Apple Inc. All rights reserved.
 *
 */

#ifndef __CGIMAGEPROPERTIES__
#define __CGIMAGEPROPERTIES__

#include <CoreGraphics/CGBase.h>
#include <ImageIO/ImageIOBase.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Properties that, if returned by CGImageSourceCopyProperties or 
 * CGImageSourceCopyPropertiesAtIndex, contain a dictionary of file-format 
 * or metadata-format specific key-values. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSDictionary  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyRawDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyCIFFDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerCanonDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerNikonDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerMinoltaDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerFujiDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerOlympusDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerPentaxDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImageProperty8BIMDictionary  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxDictionary  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyOpenEXRDictionary  IMAGEIO_AVAILABLE_STARTING(10.9, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyMakerAppleDictionary  IMAGEIO_AVAILABLE_STARTING(10.10, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyFileContentsDictionary IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPDictionary  IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTGADictionary  IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);


/** Properties which may be returned by "CGImageSourceCopyProperties".  The
 ** values apply to the container in general but not necessarily to any
 ** individual image that it contains. **/

/* The size of the image file in bytes, if known. If present, the value of
 * this key is a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyFileSize  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);


/** Properties which may be returned by "CGImageSourceCopyPropertiesAtIndex".
 ** The values apply to a single image of an image source file. **/

/* The number of pixels in the x- and y-dimensions. The value of these keys 
 * is a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPixelHeight  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPixelWidth  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The DPI in the x- and y-dimensions, if known. If present, the value of
 * these keys is a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDPIHeight  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDPIWidth  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The number of bits in each color sample of each pixel. The value of this 
 * key is a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDepth  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The intended display orientation of the image. If present, the value 
 * of this key is a CFNumberRef with the same value as defined by the 
 * TIFF and Exif specifications.  That is:
 *   1  =  0th row is at the top, and 0th column is on the left.  
 *   2  =  0th row is at the top, and 0th column is on the right.  
 *   3  =  0th row is at the bottom, and 0th column is on the right.  
 *   4  =  0th row is at the bottom, and 0th column is on the left.  
 *   5  =  0th row is on the left, and 0th column is the top.  
 *   6  =  0th row is on the right, and 0th column is the top.  
 *   7  =  0th row is on the right, and 0th column is the bottom.  
 *   8  =  0th row is on the left, and 0th column is the bottom.  
 * If not present, a value of 1 is assumed. */ 
 
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyOrientation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The value of this key is kCFBooleanTrue if the image contains floating- 
 * point pixel samples */ 
 
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIsFloat  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The value of this key is kCFBooleanTrue if the image contains indexed 
 * (a.k.a. paletted) pixel samples */ 
 
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIsIndexed  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The value of this key is kCFBooleanTrue if the image contains an alpha 
 * (a.k.a. coverage) channel */ 
 
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHasAlpha  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The color model of the image such as "RGB", "CMYK", "Gray", or "Lab".
 * The value of this key is CFStringRef. */ 

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyColorModel  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* The name of the optional ICC profile embedded in the image, if known.  
 * If present, the value of this key is a CFStringRef. */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyProfileName  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* For HEIF images: the value of this key is kCFBooleanTrue if the image at the given index is the primary image.
 */
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPrimaryImage     IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);

/* Possible values for kCGImagePropertyColorModel property */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyColorModelRGB  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyColorModelGray  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyColorModelCMYK  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyColorModelLab  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);


/* Possible keys for kCGImagePropertyTIFFDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFCompression  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFPhotometricInterpretation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFDocumentName  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFImageDescription  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFMake  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFModel  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFOrientation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFXResolution  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFYResolution  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFResolutionUnit  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFSoftware  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFTransferFunction  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFDateTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFArtist  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFHostComputer  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFCopyright  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFWhitePoint  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFPrimaryChromaticities  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFTileWidth  IMAGEIO_AVAILABLE_STARTING(10.11, 9.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyTIFFTileLength  IMAGEIO_AVAILABLE_STARTING(10.11, 9.0);

/* Possible keys for kCGImagePropertyJFIFDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFVersion  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFXDensity  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFYDensity  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFDensityUnit  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyJFIFIsProgressive  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Possible keys for kCGImagePropertyHEICSDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSLoopCount  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSDelayTime  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSUnclampedDelayTime  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSCanvasPixelWidth IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSCanvasPixelHeight IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHEICSFrameInfoArray  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/* Possible keys for kCGImagePropertyExifDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifExposureTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFNumber  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifExposureProgram  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSpectralSensitivity  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifISOSpeedRatings  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifOECF  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSensitivityType  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifStandardOutputSensitivity  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifRecommendedExposureIndex  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifISOSpeed  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifISOSpeedLatitudeyyy  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifISOSpeedLatitudezzz  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifVersion  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifDateTimeOriginal  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifDateTimeDigitized  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifOffsetTime  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifOffsetTimeOriginal  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifOffsetTimeDigitized  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifComponentsConfiguration  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifCompressedBitsPerPixel  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifShutterSpeedValue  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifApertureValue  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifBrightnessValue  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifExposureBiasValue  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifMaxApertureValue  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubjectDistance  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifMeteringMode  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifLightSource  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFlash  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFocalLength  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubjectArea  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifMakerNote  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifUserComment  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubsecTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubsecTimeOriginal  IMAGEIO_AVAILABLE_STARTING(10.11, 10.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubsecTimeDigitized  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFlashPixVersion  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifColorSpace  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifPixelXDimension  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifPixelYDimension  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifRelatedSoundFile  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFlashEnergy  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSpatialFrequencyResponse  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFocalPlaneXResolution  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFocalPlaneYResolution  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFocalPlaneResolutionUnit  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubjectLocation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifExposureIndex  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSensingMethod  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFileSource  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSceneType  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifCFAPattern  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifCustomRendered  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifExposureMode  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifWhiteBalance  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifDigitalZoomRatio  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifFocalLenIn35mmFilm  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSceneCaptureType  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifGainControl  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifContrast  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSaturation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSharpness  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifDeviceSettingDescription  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubjectDistRange  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifImageUniqueID  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifCameraOwnerName  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifBodySerialNumber  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifLensSpecification  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifLensMake  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifLensModel  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifLensSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifGamma  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifCompositeImage  IMAGEIO_AVAILABLE_STARTING(10.15.1, 13.1);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSourceImageNumberOfCompositeImage  IMAGEIO_AVAILABLE_STARTING(10.15.1, 13.1);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSourceExposureTimesOfCompositeImage  IMAGEIO_AVAILABLE_STARTING(10.15.1, 13.1);

/* Deprecated. See kCGImagePropertyExifSubsecTimeOriginal */
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifSubsecTimeOrginal IMAGEIO_AVAILABLE_BUT_DEPRECATED(10.4, 10.11, 4.0, 10.0);

/* Possible keys for kCGImagePropertyExifAuxDictionary */
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxLensInfo  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxLensModel  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxLensID  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxLensSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxImageNumber IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxFlashCompensation  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxOwnerName  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyExifAuxFirmware  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);

/* Possible keys for kCGImagePropertyGIFDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFLoopCount  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFDelayTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFImageColorMap  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFHasGlobalColorMap  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFUnclampedDelayTime  IMAGEIO_AVAILABLE_STARTING(10.7, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFCanvasPixelWidth  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFCanvasPixelHeight  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGIFFrameInfoArray  IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/* Possible keys for kCGImagePropertyPNGDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGAuthor  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGChromaticities  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGComment  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGCopyright  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGCreationTime  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGDescription  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGDisclaimer  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGGamma  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGInterlaceType  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGModificationTime  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGSoftware  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGSource  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGsRGBIntent  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGTitle  IMAGEIO_AVAILABLE_STARTING(10.7, 5.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGWarning  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGXPixelsPerMeter  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGYPixelsPerMeter  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGPixelsAspectRatio  IMAGEIO_AVAILABLE_STARTING(12.0, 19.0);

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGLoopCount  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGDelayTime  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGUnclampedDelayTime  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGFrameInfoArray IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGCanvasPixelWidth IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAPNGCanvasPixelHeight IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);

/* Possible keys for kCGImagePropertyWebPDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPLoopCount  IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPDelayTime  IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPUnclampedDelayTime  IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPFrameInfoArray IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPCanvasPixelWidth IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWebPCanvasPixelHeight IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);

/* Possible keys for kCGImagePropertyGPSDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSVersion  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSLatitudeRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSLatitude  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSLongitudeRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSLongitude  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSAltitudeRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSAltitude  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSTimeStamp  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSSatellites  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSStatus  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSMeasureMode  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDOP  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSSpeedRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSSpeed  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSTrackRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSTrack  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSImgDirectionRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSImgDirection  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSMapDatum  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestLatitudeRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestLatitude  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestLongitudeRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestLongitude  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestBearingRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestBearing  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestDistanceRef  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDestDistance  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSProcessingMethod  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSAreaInformation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDateStamp  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSDifferental  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGPSHPositioningError  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);

/* Possible keys for kCGImagePropertyIPTCDictionary */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCObjectTypeReference  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCObjectAttributeReference  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCObjectName  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCEditStatus  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCEditorialUpdate  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCUrgency  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCSubjectReference  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCategory  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCSupplementalCategory  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCFixtureIdentifier  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCKeywords  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContentLocationCode  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContentLocationName  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCReleaseDate  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCReleaseTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExpirationDate  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExpirationTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCSpecialInstructions  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCActionAdvised  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCReferenceService  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCReferenceDate  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCReferenceNumber  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCDateCreated  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCTimeCreated  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCDigitalCreationDate  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCDigitalCreationTime  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCOriginatingProgram  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCProgramVersion  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCObjectCycle  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCByline  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCBylineTitle  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCity  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCSubLocation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCProvinceState  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCountryPrimaryLocationCode  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCountryPrimaryLocationName  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCOriginalTransmissionReference  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCHeadline  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCredit  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCSource  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCopyrightNotice  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContact  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCaptionAbstract  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCWriterEditor  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCImageType  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCImageOrientation  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCLanguageIdentifier  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCStarRating  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCCreatorContactInfo  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);  // IPTC Core
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCRightsUsageTerms  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);    // IPTC Core
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCScene  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);               // IPTC Core

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAboutCvTerm  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAboutCvTermCvId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAboutCvTermId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAboutCvTermName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAboutCvTermRefinedAbout  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAddlModelInfo  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkOrObject  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCircaDateCreated  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkContentDescription  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkContributionDescription  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCopyrightNotice  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCreator  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCreatorID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCopyrightOwnerID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkCopyrightOwnerName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkLicensorID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkLicensorName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkDateCreated  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkPhysicalDescription  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkSource  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkSourceInventoryNo  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkSourceInvURL  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkStylePeriod  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtArtworkTitle  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAudioBitrate  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAudioBitrateMode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtAudioChannelCount  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCircaDateCreated  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContainerFormat  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContainerFormatIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContainerFormatName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContributor  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContributorIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContributorName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtContributorRole  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCopyrightYear  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCreator  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCreatorIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCreatorName     IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtCreatorRole  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtControlledVocabularyTerm  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreen  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegion  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionD  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionH  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionText  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionUnit  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionW  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionX  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDataOnScreenRegionY  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDigitalImageGUID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDigitalSourceFileType  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDigitalSourceType  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDopesheet  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDopesheetLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDopesheetLinkLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtDopesheetLinkLinkQualifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEmbdEncRightsExpr  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEmbeddedEncodedRightsExpr  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEmbeddedEncodedRightsExprType  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEmbeddedEncodedRightsExprLangID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEpisode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEpisodeIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEpisodeName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEpisodeNumber  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtEvent  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtShownEvent  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtShownEventIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtShownEventName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtExternalMetadataLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtFeedIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtGenre  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtGenreCvId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtGenreCvTermId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtGenreCvTermName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtGenreCvTermRefinedAbout  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtHeadline  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtIPTCLastEdited  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLinkedEncRightsExpr  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLinkedEncodedRightsExpr  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLinkedEncodedRightsExprType  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLinkedEncodedRightsExprLangID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationCreated  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationCity  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationCountryCode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationCountryName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationGPSAltitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationGPSLatitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationGPSLongitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationLocationId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationLocationName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationProvinceState  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationSublocation  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationWorldRegion  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtLocationShown  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtMaxAvailHeight  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtMaxAvailWidth  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtModelAge  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtOrganisationInImageCode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtOrganisationInImageName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonHeard  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonHeardIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonHeardName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImage  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageWDetails  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageCharacteristic  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageCvTermCvId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageCvTermId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageCvTermName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageCvTermRefinedAbout  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageDescription  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPersonInImageName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtProductInImage  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtProductInImageDescription  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtProductInImageGTIN  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtProductInImageName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPublicationEvent  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPublicationEventDate  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPublicationEventIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtPublicationEventName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRating  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRatingRegion  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionCity  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionCountryCode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionCountryName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionGPSAltitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionGPSLatitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionGPSLongitude  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionLocationId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionLocationName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionProvinceState  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionSublocation  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingRegionWorldRegion  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingScaleMaxValue  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingScaleMinValue  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingSourceLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingValue  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRatingValueLogoLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRegistryID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRegistryEntryRole  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRegistryItemID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtRegistryOrganisationID  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtReleaseReady  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeason  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeasonIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeasonName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeasonNumber  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeries  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeriesIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSeriesName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtStorylineIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtStreamReady  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtStylePeriod  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSupplyChainSource  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSupplyChainSourceIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtSupplyChainSourceName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTemporalCoverage  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTemporalCoverageFrom  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTemporalCoverageTo  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTranscript  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTranscriptLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTranscriptLinkLink  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtTranscriptLinkLinkQualifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoBitrate  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoBitrateMode  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoDisplayAspectRatio  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoEncodingProfile  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoShotType  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoShotTypeIdentifier  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoShotTypeName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVideoStreamsCount  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtVisualColor  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtWorkflowTag  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtWorkflowTagCvId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtWorkflowTagCvTermId  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtWorkflowTagCvTermName  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCExtWorkflowTagCvTermRefinedAbout  IMAGEIO_AVAILABLE_STARTING(10.13.4, 11.3);


/* Possible keys for kCGImagePropertyIPTCCreatorContactInfo dictionary (part of IPTC Core - above) */

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoCity  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoCountry  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoAddress  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoPostalCode  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoStateProvince  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoEmails  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoPhones  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyIPTCContactInfoWebURLs  IMAGEIO_AVAILABLE_STARTING(10.6, 4.0);

/* Possible keys for kCGImageProperty8BIMDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImageProperty8BIMLayerNames  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImageProperty8BIMVersion  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);

/* Possible keys for kCGImagePropertyDNGDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGVersion  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGBackwardVersion  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGUniqueCameraModel  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGLocalizedCameraModel  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCameraSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGLensInfo  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGBlackLevel  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGWhiteLevel  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCalibrationIlluminant1  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCalibrationIlluminant2  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGColorMatrix1  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGColorMatrix2  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCameraCalibration1  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCameraCalibration2  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGAsShotNeutral  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGAsShotWhiteXY  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGBaselineExposure  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGBaselineNoise  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGBaselineSharpness  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGPrivateData  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGCameraCalibrationSignature  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGProfileCalibrationSignature  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGNoiseProfile  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGWarpRectilinear  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGWarpFisheye  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyDNGFixVignetteRadial  IMAGEIO_AVAILABLE_STARTING(10.12, 10.0);

IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGActiveArea  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGAnalogBalance  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGAntiAliasStrength  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGAsShotICCProfile  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGAsShotPreProfileMatrix  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGAsShotProfileName  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBaselineExposureOffset  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBayerGreenSplit  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBestQualityScale  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBlackLevelDeltaH  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBlackLevelDeltaV  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGBlackLevelRepeatDim  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGCFALayout  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGCFAPlaneColor  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGChromaBlurRadius  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGColorimetricReference  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGCurrentICCProfile  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGCurrentPreProfileMatrix  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDefaultBlackRender  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDefaultCropOrigin  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDefaultCropSize  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDefaultScale  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGDefaultUserCrop  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGExtraCameraProfiles  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGForwardMatrix1  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGForwardMatrix2  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGLinearizationTable  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGLinearResponseLimit  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGMakerNoteSafety  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGMaskedAreas  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGNewRawImageDigest  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGNoiseReductionApplied  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOpcodeList1  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOpcodeList2  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOpcodeList3  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalBestQualityFinalSize  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalDefaultCropSize  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalDefaultFinalSize  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalRawFileData  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalRawFileDigest  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGOriginalRawFileName  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewApplicationName  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewApplicationVersion  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewColorSpace  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewDateTime  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewSettingsDigest  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGPreviewSettingsName  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileCopyright  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileEmbedPolicy  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileHueSatMapData1  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileHueSatMapData2  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileHueSatMapDims  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileHueSatMapEncoding  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileLookTableData  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileLookTableDims  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileLookTableEncoding  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileName  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGProfileToneCurve  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGRawDataUniqueID  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGRawImageDigest  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGRawToPreviewGain  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGReductionMatrix1  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGReductionMatrix2  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGRowInterleaveFactor  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGShadowScale  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyDNGSubTileBlockSize  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);



/* Possible keys for kCGImagePropertyCIFFDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFDescription  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFFirmware  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFOwnerName  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFImageName  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFImageFileName  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFReleaseMethod  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFReleaseTiming  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFRecordID  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFSelfTimingTime  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFCameraSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFImageSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFContinuousDrive  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFFocusMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFMeteringMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFShootingMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFLensModel  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFLensMaxMM  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFLensMinMM  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFWhiteBalanceIndex  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFFlashExposureComp  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyCIFFMeasuredEV  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);


/* Possible keys for kCGImagePropertyMakerNikonDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonISOSetting  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonColorMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonQuality  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonWhiteBalanceMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonSharpenMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonFocusMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonFlashSetting  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonISOSelection  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonFlashExposureComp  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonImageAdjustment  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonLensAdapter  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonLensType  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonLensInfo  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonFocusDistance  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonDigitalZoom  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonShootingMode  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonCameraSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerNikonShutterCount  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);

/* Possible keys for kCGImagePropertyMakerCanonDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonOwnerName  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonCameraSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonImageSerialNumber  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonFlashExposureComp  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonContinuousDrive  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonLensModel  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonFirmware  IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyMakerCanonAspectRatioInfo IMAGEIO_AVAILABLE_STARTING(10.5, 4.0);

/* Possible keys for kCGImagePropertyOpenEXRDictionary */

IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyOpenEXRAspectRatio  IMAGEIO_AVAILABLE_STARTING(10.9, 11.3);


/* Possible int values for kCGImagePropertyTIFFOrientation */
typedef CF_CLOSED_ENUM(uint32_t, CGImagePropertyOrientation) {
    kCGImagePropertyOrientationUp = 1,        // 0th row at top,    0th column on left   - default orientation
    kCGImagePropertyOrientationUpMirrored,    // 0th row at top,    0th column on right  - horizontal flip
    kCGImagePropertyOrientationDown,          // 0th row at bottom, 0th column on right  - 180 deg rotation
    kCGImagePropertyOrientationDownMirrored,  // 0th row at bottom, 0th column on left   - vertical flip
    kCGImagePropertyOrientationLeftMirrored,  // 0th row on left,   0th column at top
    kCGImagePropertyOrientationRight,         // 0th row on right,  0th column at top    - 90 deg CW
    kCGImagePropertyOrientationRightMirrored, // 0th row on right,  0th column on bottom
    kCGImagePropertyOrientationLeft           // 0th row on left,   0th column at bottom - 90 deg CCW
};


/* Possible keys for kCGImagePropertyTGADictionary */
IMAGEIO_EXTERN const CFStringRef  kCGImagePropertyTGACompression IMAGEIO_AVAILABLE_STARTING(11.0, 14.0);

/* Possible int values for kCGImagePropertyTGACompression */
typedef CF_CLOSED_ENUM(uint32_t, CGImagePropertyTGACompression) {
    kCGImageTGACompressionNone = 0,
    kCGImageTGACompressionRLE,
};

/*
 * Allows client to choose the filters applied before PNG compression
 * http://www.libpng.org/pub/png/book/chapter09.html#png.ch09.div.1
 * The value should be a CFNumber, of type long, containing a bitwise OR of the desired filters
 * The filters are defined below, IMAGEIO_PNG_NO_FILTERS, IMAGEIO_PNG_FILTER_NONE, etc
 * This value has no effect when compressing to any format other than PNG
 */
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGCompressionFilter IMAGEIO_AVAILABLE_STARTING(10.11, 9.0);

#define IMAGEIO_PNG_NO_FILTERS     0x00
#define IMAGEIO_PNG_FILTER_NONE    0x08
#define IMAGEIO_PNG_FILTER_SUB     0x10
#define IMAGEIO_PNG_FILTER_UP      0x20
#define IMAGEIO_PNG_FILTER_AVG     0x40
#define IMAGEIO_PNG_FILTER_PAETH   0x80
#define IMAGEIO_PNG_ALL_FILTERS (IMAGEIO_PNG_FILTER_NONE | IMAGEIO_PNG_FILTER_SUB | IMAGEIO_PNG_FILTER_UP | IMAGEIO_PNG_FILTER_AVG | IMAGEIO_PNG_FILTER_PAETH)

/*
 * For PNG files:
 * When writing indexed color PNGs, the data for the transparity chunk 'tRNS' can be passed in with this key.
 * The value (CFDataRef) should contain one byte transparency info for each entry in the indexed color map.
 */
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPNGTransparency     IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);


/* For use with CGImageSourceCopyAuxiliaryDataInfoAtIndex and CGImageDestinationAddAuxiliaryDataInfo:
 * These strings specify the 'auxiliaryImageDataType':
 */
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeDepth IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeDisparity IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypePortraitEffectsMatte IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);

IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeSemanticSegmentationSkinMatte IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeSemanticSegmentationHairMatte IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeSemanticSegmentationTeethMatte IMAGEIO_AVAILABLE_STARTING(10.15, 13.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeSemanticSegmentationGlassesMatte IMAGEIO_AVAILABLE_STARTING(11.0, 14.1);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeSemanticSegmentationSkyMatte IMAGEIO_AVAILABLE_STARTING(11.0, 14.1);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataTypeHDRGainMap   IMAGEIO_AVAILABLE_STARTING(11.0, 14.1);


/* Depth/Disparity data support for JPEG, HEIF, and DNG images:
 * CGImageSourceCopyAuxiliaryDataInfoAtIndex and CGImageDestinationAddAuxiliaryDataInfo will use these keys in the dictionary:
 * kCGImageAuxiliaryDataInfoData - the depth data (CFDataRef)
 * kCGImageAuxiliaryDataInfoDataDescription - the depth data description (CFDictionary)
 * kCGImageAuxiliaryDataInfoMetadata - metadata (CGImageMetadataRef)
 */
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataInfoData IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataInfoDataDescription IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImageAuxiliaryDataInfoMetadata IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);


IMAGEIO_EXTERN const CFStringRef kCGImagePropertyImageCount IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyWidth IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyHeight IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyBytesPerRow IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyNamedColorSpace IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyPixelFormat IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyImages IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyThumbnailImages IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAuxiliaryData IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyAuxiliaryDataType IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyImageIndex IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroups IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupIndex IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupType IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupTypeStereoPair IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupTypeAlternate IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImagesAlternate IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImageIndexLeft IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImageIndexRight IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImageIsLeftImage IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImageIsRightImage IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);
IMAGEIO_EXTERN const CFStringRef kCGImagePropertyGroupImageIsAlternateImage IMAGEIO_AVAILABLE_STARTING(12.0, 15.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif  /* __CGIMAGEPROPERTIES__ */
