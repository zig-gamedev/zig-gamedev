/* CoreAnimation - CAEDRMetadata.h

 Copyright (c) 2018-2021, Apple Inc.
 All rights reserved. */

#ifndef CAEDRMetadata_h
#define CAEDRMetadata_h


#include <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.15))
@interface CAEDRMetadata : NSObject
{
@private
  struct _CAEDRMetadataPrivate *_priv;
}

/* Use one of the class methods to instantiate CAEDRMetadata. */

- (instancetype)init NS_UNAVAILABLE;

/* The following two selectors are for static mastering display color volume and
 * content light level info - typically associated with "HDR10" content. The
 * data is treated as display referred with 1.0 mapping to diffuse white of 100
 * nits in a reference grading environment. */

/* Initialize with SEI MDCV and CLLI as defined by ISO/IEC 23008-2:2017
 *
 * `displayData'
 * The value is 24 bytes containing a big-endian structure as defined in D.2.28
 * Mastering display colour volume SEI message. If nil, uses system defaults.
 *
 * `contentData'
 * The value is 4 bytes containing a big-endian structure as defined in D.2.35
 * Content light level information SEI message. If nil, uses system defaults.
 *
 * `scale'
 * Scale factor relating (display-referred linear) extended range buffer values
 * (such as MTLPixelFormatRGBA16Float) to optical output of a reference display.
 * Values y in the buffer are assumed to be proportional to the optical output
 * C (in cd/m^2) of a reference display; denoting the opticalOutputScale as C1
 * (cd/m^2), the relationship is C = C1 * y. As an example, if C1 = 100 cd/m^2,
 * the optical output corresponding to y = 1 is C = C1 = 100 cd/m^2, and the
 * display-referred linear value corresponding to C = 4,000 cd/m^2 is y = 40.
 * If the content, y, is in a normalized pixel format then `scale' is
 * assumed to be 10,000. */


+ (CAEDRMetadata *)HDR10MetadataWithDisplayInfo:(nullable NSData *)displayData
                                    contentInfo:(nullable NSData *)contentData
                             opticalOutputScale:(float)scale NS_SWIFT_NAME(hdr10(displayInfo:contentInfo:opticalOutputScale:));

/* Simplified HDR10 initializer based on the minimum and maximum candelas per
 * meters squared ("nits") of the mastering display. Any content greater than
 * `maxNits' may be clamped when displayed.
 *
 * `minNits'
 * Minimum nits (cd/m^2) of the mastering display
 *
 * `maxNits'
 * Maximum nits (cd/m^2) of the mastering display
 *
 * `scale'
 * Scale factor relating (display-referred linear) extended range buffer values
 * (such as MTLPixelFormatRGBA16Float) to optical output of a reference display.
 * Values y in the buffer are assumed to be proportional to the optical output
 * C (in cd/m^2) of a reference display; denoting the opticalOutputScale as C1
 * (cd/m^2), the relationship is C = C1 * y. As an example, if C1 = 100 cd/m^2,
 * the optical output corresponding to y = 1 is C = C1 = 100 cd/m^2, and the
 * display-referred linear value corresponding to C = 4,000 cd/m^2 is y = 40.
 * If the content, y, is in a normalized pixel format then `scale' is
 * assumed to be 10,000.
 */

+ (CAEDRMetadata *)HDR10MetadataWithMinLuminance:(float)minNits
                                    maxLuminance:(float)maxNits
                              opticalOutputScale:(float)scale NS_SWIFT_NAME(hdr10(minLuminance:maxLuminance:opticalOutputScale:));

/* Content is scene referred and originally encoded with the ITU-R BT.2100-2
 * Hybrid Log Gamma (HLG) opto-electrical transfer function (OETF). The system
 * will apply the opto-optical transfer function (OOTF) based on peak display
 * brightness and ambient. If rendering to a CAMetalLayer with a linear
 * colorspace (for floating point extended dynamic range layers), the content
 * provider must have already applied the HLG inverse OETF. */

@property (class, readonly, retain) CAEDRMetadata *HLGMetadata;

@end

NS_ASSUME_NONNULL_END


#endif /* CAEDRMetadata_h */
