/*
 * ImageIO - CGImageSource.h
 * Copyright (c) 2004-2016 Apple Inc. All rights reserved.
 *
 */

#ifndef CGIMAGESOURCE_H_
#define CGIMAGESOURCE_H_

#include <ImageIO/ImageIOBase.h>

typedef struct IIO_BRIDGED_TYPE(id) CGImageSource * CGImageSourceRef;

#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/CGImageMetadata.h>

CF_IMPLICIT_BRIDGING_ENABLED

typedef CF_ENUM(int32_t, CGImageSourceStatus) {
    kCGImageStatusUnexpectedEOF = -5,
    kCGImageStatusInvalidData = -4,
    kCGImageStatusUnknownType = -3,
    kCGImageStatusReadingHeader = -2,
    kCGImageStatusIncomplete = -1,
    kCGImageStatusComplete = 0
};

CF_ASSUME_NONNULL_BEGIN

/** Keys for the options dictionary when creating a CGImageSourceRef. **/

/* Specifies the "best guess" of the type identifier for the format of the
 * image source file. If specified, the value of this key must be a
 * CFStringRef. For more information about type identifiers, see "UTType.h"
 * in the Application Services framework. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceTypeIdentifierHint  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/** Keys for the options dictionary of "CGImageSourceCopyPropertiesAtIndex"
 ** and "CGImageSourceCreateImageAtIndex". **/

/* Specifies whether the image should be cached in a decoded form. The
 * value of this key must be a CFBooleanRef.
 * kCFBooleanFalse indicates no caching, kCFBooleanTrue indicates caching.
 * For 64-bit architectures, the default is kCFBooleanTrue, for 32-bit the default is kCFBooleanFalse.
 */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceShouldCache  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specifies whether image decoding and caching should happen at image creation time.
 * The value of this key must be a CFBooleanRef. The default value is kCFBooleanFalse (image decoding will
 * happen at rendering time).
 */
IMAGEIO_EXTERN const CFStringRef kCGImageSourceShouldCacheImmediately  IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);

/* Specifies whether the image should be returned as a floating
 * point CGImageRef if supported by the file format. Extended
 * range floating point CGImageRef may require additional
 * processing  to render pleasingly.  The value of this key must
 * be a CFBooleanRef; the default value is kCFBooleanFalse. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceShouldAllowFloat  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);


/** Keys for the options dictionary of
 ** "CGImageSourceCreateThumbnailAtIndex". **/

/* Specifies whether a thumbnail should be automatically created for an
 * image if a thumbnail isn't present in the image source file.  The
 * thumbnail will be created from the full image, subject to the limit
 * specified by kCGImageSourceThumbnailMaxPixelSize---if a maximum pixel
 * size isn't specified, then the thumbnail will be the size of the full
 * image, which probably isn't what you want. The value of this key must be
 * a CFBooleanRef; the default value of this key is kCFBooleanFalse. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceCreateThumbnailFromImageIfAbsent  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specifies whether a thumbnail should be created from the full image even
 * if a thumbnail is present in the image source file. The thumbnail will
 * be created from the full image, subject to the limit specified by
 * kCGImageSourceThumbnailMaxPixelSize---if a maximum pixel size isn't
 * specified, then the thumbnail will be the size of the full image, which
 * probably isn't what you want. The value of this key must be a
 * CFBooleanRef; the default value of this key is kCFBooleanFalse. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceCreateThumbnailFromImageAlways  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specifies the maximum width and height in pixels of a thumbnail.  If
 * this this key is not specified, the width and height of a thumbnail is
 * not limited and thumbnails may be as big as the image itself.  If
 * present, this value of this key must be a CFNumberRef. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceThumbnailMaxPixelSize  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specifies whether the thumbnail should be rotated and scaled according
 * to the orientation and pixel aspect ratio of the full image. The value
 * of this key must be a CFBooleanRef; the default value of this key is 
 * kCFBooleanFalse. */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceCreateThumbnailWithTransform  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Specifies that, if possible, an image should be returned as scaled down (in height and width) by a specified factor.
 * The resulting image will be smaller and have reduced spatial quality but will otherwise have the same characteristics
 * as the full size normal image.
 * If the specified scaling factor is not supported, a larger or full size normal image will be returned.
 * Supported file formats are JPEG, HEIF, TIFF, and PNG.
 * The value of this key must be an integer CFNumberRef (allowed values: 2, 4, and 8).
 */

IMAGEIO_EXTERN const CFStringRef kCGImageSourceSubsampleFactor  IMAGEIO_AVAILABLE_STARTING(10.11, 9.0);

CF_ASSUME_NONNULL_END


/* Return the CFTypeID for CGImageSources. */

IMAGEIO_EXTERN CFTypeID CGImageSourceGetTypeID (void)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return an array of supported type identifiers. */

IMAGEIO_EXTERN CFArrayRef _iio_Nonnull CGImageSourceCopyTypeIdentifiers(void)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image source reading from the data provider `provider'. The
 * `options' dictionary may be used to request additional creation options;
 * see the list of keys above for more information. */

IMAGEIO_EXTERN CGImageSourceRef _iio_Nullable CGImageSourceCreateWithDataProvider(CGDataProviderRef _iio_Nonnull provider, CFDictionaryRef _iio_Nullable options) IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image source reading from `data'.  The `options' dictionary
 * may be used to request additional creation options; see the list of keys
 * above for more information. */

IMAGEIO_EXTERN CGImageSourceRef _iio_Nullable CGImageSourceCreateWithData(CFDataRef _iio_Nonnull data, CFDictionaryRef _iio_Nullable options) IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an image source reading from `url'. The `options' dictionary may
 * be used to request additional creation options; see the list of keys
 * above for more information. */

IMAGEIO_EXTERN CGImageSourceRef _iio_Nullable CGImageSourceCreateWithURL(CFURLRef _iio_Nonnull url, CFDictionaryRef _iio_Nullable options) IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the type identifier of the image source `isrc'.  This type is the
 * type of the source "container", which is not necessarily the type of the
 * image(s) in the container.  For example, the .icns format supports
 * embedded JPEG2000 but the source type will be "com.apple.icns". */

IMAGEIO_EXTERN CFStringRef _iio_Nullable CGImageSourceGetType(CGImageSourceRef _iio_Nonnull isrc)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the number of images (not including thumbnails) in the image
 * source `isrc'. */

IMAGEIO_EXTERN size_t CGImageSourceGetCount(CGImageSourceRef _iio_Nonnull isrc)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the properties of the image source `isrc'.  These properties
 * apply to the container in general but not necessarily to any individual
 * image that it contains. */

IMAGEIO_EXTERN CFDictionaryRef _iio_Nullable CGImageSourceCopyProperties(CGImageSourceRef _iio_Nonnull isrc, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the properties of the image at `index' in the image source
 * `isrc'.  The index is zero-based. The `options' dictionary may be used
 * to request additional options; see the list of keys above for more
 * information. */

IMAGEIO_EXTERN CFDictionaryRef _iio_Nullable CGImageSourceCopyPropertiesAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the metadata of the image at `index' in the image source
 * `isrc'. The index is zero-based. The `options' dictionary may be used
 * to request additional options; see the list of keys above for more
 * information. Please refer to CGImageMetadata.h for usage of metadata. */
IMAGEIO_EXTERN CGImageMetadataRef _iio_Nullable CGImageSourceCopyMetadataAtIndex (CGImageSourceRef _iio_Nonnull isrc, size_t index, CFDictionaryRef _iio_Nullable options) IMAGEIO_AVAILABLE_STARTING(10.8,7.0);

/* Return the image at `index' in the image source `isrc'.  The index is
 * zero-based. The `options' dictionary may be used to request additional
 * creation options; see the list of keys above for more information. */

IMAGEIO_EXTERN CGImageRef _iio_Nullable CGImageSourceCreateImageAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Remove the cached decoded image data for the image at `index' in the image source `isrc'.
 * The index is zero-based.
 */
IMAGEIO_EXTERN void CGImageSourceRemoveCacheAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index) IMAGEIO_AVAILABLE_STARTING(10.9, 7.0);

/* Return the thumbnail of the image at `index' in the image source `isrc'.
 * The index is zero-based. The `options' dictionary may be used to request
 * additional thumbnail creation options; see the list of keys above for
 * more information. */

IMAGEIO_EXTERN CGImageRef _iio_Nullable CGImageSourceCreateThumbnailAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index, CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Create an incremental image source. No data is provided at creation
 * time; it is assumed that data will eventually be provided using
 * "CGImageSourceUpdateDataProvider" or "CGImageSourceUpdateData".  The
 * `options' dictionary may be used to request additional creation options;
 * see the list of keys above for more information. */

IMAGEIO_EXTERN CGImageSourceRef _iio_Nonnull CGImageSourceCreateIncremental(CFDictionaryRef _iio_Nullable options)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Update the incremental image source `isrc' with new data.  The new data
 * must include all the previous data plus any additional new data. The
 * `final' parameter should be true when the final set of data is provided;
 * false otherwise. */

IMAGEIO_EXTERN void CGImageSourceUpdateData(CGImageSourceRef _iio_Nonnull isrc, CFDataRef _iio_Nonnull data, bool final)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Update the incremental image source `isrc' with a new data provider.
 * The new data provider must provide all the previous data plus any
 * additional new data. The `final' parameter should be true when the final
 * set of data is provided; false otherwise. */

IMAGEIO_EXTERN void CGImageSourceUpdateDataProvider(CGImageSourceRef _iio_Nonnull isrc, CGDataProviderRef _iio_Nonnull provider, bool final)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the overall status of the image source `isrc'.  The status is
 * particularly informative for incremental image sources, but may be used
 * by clients providing non-incremental data as well. */

IMAGEIO_EXTERN CGImageSourceStatus CGImageSourceGetStatus(CGImageSourceRef _iio_Nonnull isrc)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the current status of the image at `index' in the image source
 * `isrc'. The index is zero-based. The returned status is particularly
 * informative for incremental image sources but may used by clients
 * providing non-incremental data as well. */

IMAGEIO_EXTERN CGImageSourceStatus CGImageSourceGetStatusAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index)  IMAGEIO_AVAILABLE_STARTING(10.4, 4.0);

/* Return the primary image index for HEIF images.
 * Zero for all other formats.
 */
IMAGEIO_EXTERN size_t CGImageSourceGetPrimaryImageIndex(CGImageSourceRef _iio_Nonnull isrc)  IMAGEIO_AVAILABLE_STARTING(10.14, 12.0);

/* Depth data support for JPEG, HEIF, and DNG images.
 * The returned CFDictionary contains:
 *   - the depth data (CFDataRef) - (kCGImageAuxiliaryDataInfoData),
 *   - the depth data description (CFDictionary) - (kCGImageAuxiliaryDataInfoDataDescription)
 *   - metadata (CGImageMetadataRef) - (kCGImageAuxiliaryDataInfoMetadata)
 * CGImageSourceCopyAuxiliaryDataInfoAtIndex returns nil if the image did not contain ‘auxiliaryImageDataType’ data.
 */
IMAGEIO_EXTERN CFDictionaryRef _iio_Nullable CGImageSourceCopyAuxiliaryDataInfoAtIndex(CGImageSourceRef _iio_Nonnull isrc, size_t index, CFStringRef _iio_Nonnull auxiliaryImageDataType ) IMAGEIO_AVAILABLE_STARTING(10.13, 11.0);

CF_IMPLICIT_BRIDGING_DISABLED


#endif  /* CGIMAGESOURCE_H_ */
