/*
 * ImageIO - CGImageDestination.h
 * Copyright (c) 2004-2016 Apple Inc. All rights reserved.
 *
 */

#ifndef CGIMAGEDESTINATION_H_
#define CGIMAGEDESTINATION_H_

#include <ImageIO/ImageIOBase.h>

typedef struct IIO_BRIDGED_TYPE(id) CGImageDestination * CGImageDestinationRef;

#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/CGImageSource.h>
#include <ImageIO/CGImageMetadata.h>

CF_IMPLICIT_BRIDGING_ENABLED

/** Properties which may be passed to "CGImageDestinationAddImage"
 ** or "CGImageDestinationAddImageFromSource" to effect the output.
 ** The values apply to a single image of an image destination. **/

CF_ASSUME_NONNULL_BEGIN

/* The desired compression quality to use when writing to an image 
 * destination. If present, the value of this key is a CFNumberRef 
 * in the range 0.0 to 1.0. A value of 1.0 implies lossless
 * compression is desired if destination format supports it. 
 * A value of 0.0 implies that that maximum compression is 
 * desired. */

IMAGEIO_EXTERN const CFStringRef kCGImageDestinationLossyCompressionQuality  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);


/* The desired background color to composite against when writing 
 * an image with alpha to a destination format that does not support 
 * alpha. If present, the value of this key is a CGColorRef without
 * any alpha component of its own.  If not present a white color
 * will be used if needed. */

IMAGEIO_EXTERN const CFStringRef kCGImageDestinationBackgroundColor  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Rescale the image to the maximum width and height in pixels.
 * If present, this value of this key must be a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImageDestinationImageMaxPixelSize  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);

/* Enable or disable thumbnail embedding for JPEG and HEIF.
 * The value should be kCFBooleanTrue or kCFBooleanFalse. Defaults to kCFBooleanFalse */

IMAGEIO_EXTERN const CFStringRef kCGImageDestinationEmbedThumbnail  IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);


/* Create an image using a colorspace, that has is compatible with older devices
 * The value should be kCFBooleanTrue or kCFBooleanFalse
 * Defaults to kCFBooleanFalse = don't do any color conversion
 */
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationOptimizeColorForSharing  IMAGEIO_AVAILABLE_STARTING(10.12, 9.3);

CF_ASSUME_NONNULL_END


/* Return the CFTypeID for CGImageDestinations. */

IMAGEIO_EXTERN CFTypeID CGImageDestinationGetTypeID(void)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return an array of supported type identifiers. */

IMAGEIO_EXTERN CFArrayRef _iio_Nonnull CGImageDestinationCopyTypeIdentifiers(void)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image destination writing to the data consumer `consumer'.
 * The parameter `type' specifies the type identifier of the resulting
 * image file.  Constants for `type' are found in the LaunchServices framework
 * header UTCoreTypes.h.  The parameter `count' specifies number of images
 * (not including thumbnails) that the image file will contain. The `options'
 * dictionary is reserved for future use; currently, you should pass NULL for
 * this parameter. */

IMAGEIO_EXTERN CGImageDestinationRef _iio_Nullable CGImageDestinationCreateWithDataConsumer(CGDataConsumerRef _iio_Nonnull consumer, CFStringRef _iio_Nonnull type, size_t count, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image destination writing to `data'. The parameter `type'
 * specifies the type identifier of the resulting image file.  Constants for
 * `type' are found in the LaunchServices framework header UTCoreTypes.h.  The
 * parameter `count' specifies number of images (not including thumbnails)
 * that the image file will contain. The `options' dictionary is reserved
 * for future use; currently, you should pass NULL for this parameter. */

IMAGEIO_EXTERN CGImageDestinationRef _iio_Nullable CGImageDestinationCreateWithData(CFMutableDataRef _iio_Nonnull data, CFStringRef _iio_Nonnull type, size_t count, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image destination writing to `url'. The parameter `type'
 * specifies the type identifier of the resulting image file.  Constants for
 * `type' are found in the LaunchServices framework header UTCoreTypes.h.  The
 * parameter `count' specifies number of images (not including thumbnails)
 * that the image file will contain. The `options' dictionary is reserved
 * for future use; currently, you should pass NULL for this parameter.
 * Note that if `url' already exists, it will be overwritten. */

IMAGEIO_EXTERN CGImageDestinationRef _iio_Nullable CGImageDestinationCreateWithURL(CFURLRef _iio_Nonnull url, CFStringRef _iio_Nonnull type, size_t count, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specify the dictionary `properties' of properties which apply to all
 * images in the image destination `idst'. */

IMAGEIO_EXTERN void CGImageDestinationSetProperties(CGImageDestinationRef _iio_Nonnull idst, CFDictionaryRef _iio_Nullable properties)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Set the next image in the image destination `idst' to be `image' with
 * optional properties specified in `properties'.  An error is logged if
 * more images are added than specified in the original count of the image
 * destination. */

IMAGEIO_EXTERN void CGImageDestinationAddImage(CGImageDestinationRef _iio_Nonnull idst, CGImageRef _iio_Nonnull image, CFDictionaryRef _iio_Nullable properties)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Set the next image in the image destination `idst' to be the image at
 * `index' in the image source `isrc'.  The index is zero-based. The
 * properties of the source image can be added to or overriden by supplying
 * additional keys/values in `properties'.  If a key in `properties' has
 * the value kCFNull, the corresponding property in the destination will be
 * removed. */

IMAGEIO_EXTERN void CGImageDestinationAddImageFromSource(CGImageDestinationRef _iio_Nonnull idst, CGImageSourceRef _iio_Nonnull isrc, size_t index, CFDictionaryRef _iio_Nullable properties)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Write everything to the destination data, url or consumer of the image
 * destination `idst'.  You must call this function or the image
 * destination will not be valid.  After this function is called, no
 * additional data will be written to the image destination.  Return true
 * if the image was successfully written; false otherwise. */

IMAGEIO_EXTERN bool CGImageDestinationFinalize(CGImageDestinationRef _iio_Nonnull idst)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);


/* Set the next image in the image destination `idst' to be `image' with
 * metadata properties specified in `metadata'. An error is logged if more
 * images are added than specified in the original count of the image
 * destination. */
IMAGEIO_EXTERN void CGImageDestinationAddImageAndMetadata(CGImageDestinationRef _iio_Nonnull idst, CGImageRef _iio_Nonnull image, CGImageMetadataRef _iio_Nullable metadata, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

CF_ASSUME_NONNULL_BEGIN
/* For CGImageDestinationAddImageFromSource: when set to kCFBooleanTrue, a HEIF-embedded GainMap will be preserved.
 * If the destination image is scaled (using kCGImageDestinationImageMaxPixelSize), the GainMap will be scaled accordingly.
 * The value should be kCFBooleanTrue or kCFBooleanFalse
 * Defaults to kCFBooleanFalse
 */
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationPreserveGainMap IMAGEIO_AVAILABLE_STARTING(11.0, 14.1);

/**
 ** Keys which may be used in the 'options' dictionary of
 ** "CGImageDestinationCopyImageSource" to effect the output.
 **/

/* Set the metadata tags for the image destination. If present, the value of
 * this key is a CGImageMetadataRef. By default, all EXIF, IPTC, and XMP tags
 * will be replaced. Use kCGImageDestinationMergeMetadata to merge the tags
 * with the existing tags in the image source.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationMetadata IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/* If true, The metadata will be copied from the source and merged with the tags
 * specified in kCGImageDestinationMetadata. If a tag does not exist in the 
 * source, it will be added. If the tag exists in the source, it will be 
 * updated. A metadata tag can be removed by setting the tag's value to 
 * kCFNull. If present, the value of this key is a CFBoooleanRef. The default
 * is kCFBooleanFalse.
 */ 
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationMergeMetadata IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/* XMP data will not be written to the destination. If used in conjunction with 
 * kCGImageDestinationMetadata, EXIF tags will be preserved, but
 * an XMP packet will not be written to the file. IPTC tags, that are stored in XMP, will not be preserved.
 * If present, the value for this key is a CFBooleanRef. The default is kCFBooleanFalse.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageMetadataShouldExcludeXMP IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/* If true, GPS metadata will not be written to EXIF data or the corresponding
 * EXIF tags in XMP. This flag cannot filter any proprietary location data that
 * could be stored in a manufacturer's EXIF MakerNote or custom XMP properties.
 * If present, the value for this key is a CFBooleanRef. The default is 
 * kCFBooleanFalse.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageMetadataShouldExcludeGPS IMAGEIO_AVAILABLE_STARTING(10.10, 8.0);

/* Updates the DateTime parameters of the image metadata. Only values
 * present in the original image will updated. If present, the value should
 * be a CFStringRef or a CFDateRef. If CFString, the value must be in 
 * Exif DateTime or ISO 8601 DateTime format. This option is mutually
 * exclusive with kCGImageDestinationMetadata.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationDateTime IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/* Updates the orientation in the image metadata. The image data itself will
 * not be rotated. If present, the value should be a CFNumberRef from 1 to 8. 
 * This option is mutually exclusive with kCGImageDestinationMetadata.
 */
IMAGEIO_EXTERN const CFStringRef kCGImageDestinationOrientation IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

CF_ASSUME_NONNULL_END

/* Losslessly copies the contents of the image source, 'isrc', to the 
 * destination, 'idst'. The image data will not be modified. The image's 
 * metadata can be modified by adding the keys and values defined above to 
 * 'options'. No other images should be added to the image destination. 
 * CGImageDestinationFinalize() should not be called afterward -
 * the result is saved to the destination when this function returns. 
 * The image type of the destination must match the image source. Returns true
 * if the operation was successful. If an error occurs, false will be returned 
 * and 'err' will be set to a CFErrorRef. Not all image formats are supported 
 * for this operation. */
IMAGEIO_EXTERN bool CGImageDestinationCopyImageSource(CGImageDestinationRef _iio_Nonnull idst, CGImageSourceRef _iio_Nonnull isrc, CFDictionaryRef _iio_Nullable options, _iio_Nullable CFErrorRef * _iio_Nullable err) IMAGEIO_AVAILABLE_STARTING(10.8, 7.0);

/* Auxiliary data support for JPEG, HEIF, and DNG images.
 * The auxiliaryDataInfoDictionary should contain:
 *   - the auxiliary data (CFDataRef) - (kCGImageAuxiliaryDataInfoData),
 *   - the auxiliary data description (CFDictionary) - (kCGImageAuxiliaryDataInfoDataDescription)
 *   - metadata (CGImageMetadataRef) - (kCGImageAuxiliaryDataInfoMetadata)
 * To add auxiliary data to an image, call CGImageDestinationAddAuxiliaryDataInfo() after adding the CGImage to the CGImageDestinationRef.
 */
IMAGEIO_EXTERN void CGImageDestinationAddAuxiliaryDataInfo(CGImageDestinationRef _iio_Nonnull idst, CFStringRef _iio_Nonnull auxiliaryImageDataType, CFDictionaryRef _iio_Nonnull auxiliaryDataInfoDictionary ) IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGIMAGEDESTINATION_H_ */
