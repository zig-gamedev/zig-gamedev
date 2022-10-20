/* CoreGraphics - CGImage.h
 * Copyright (c) 2000-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGIMAGE_H_
#define CGIMAGE_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGImage *CGImageRef;

#include <CoreGraphics/CGColorSpace.h>
#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGGeometry.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

typedef CF_ENUM(uint32_t, CGImageAlphaInfo) {
    kCGImageAlphaNone,               /* For example, RGB. */
    kCGImageAlphaPremultipliedLast,  /* For example, premultiplied RGBA */
    kCGImageAlphaPremultipliedFirst, /* For example, premultiplied ARGB */
    kCGImageAlphaLast,               /* For example, non-premultiplied RGBA */
    kCGImageAlphaFirst,              /* For example, non-premultiplied ARGB */
    kCGImageAlphaNoneSkipLast,       /* For example, RBGX. */
    kCGImageAlphaNoneSkipFirst,      /* For example, XRGB. */
    kCGImageAlphaOnly                /* No color data, alpha data only */
};

typedef CF_ENUM(uint32_t, CGImageByteOrderInfo) {
    kCGImageByteOrderMask     = 0x7000,
    kCGImageByteOrderDefault  = (0 << 12),
    kCGImageByteOrder16Little = (1 << 12),
    kCGImageByteOrder32Little = (2 << 12),
    kCGImageByteOrder16Big    = (3 << 12),
    kCGImageByteOrder32Big    = (4 << 12)
} CG_AVAILABLE_STARTING(10.0, 2.0);

typedef CF_ENUM(uint32_t, CGImagePixelFormatInfo) {
    kCGImagePixelFormatMask      = 0xF0000,
    kCGImagePixelFormatPacked    = (0 << 16),
    kCGImagePixelFormatRGB555    = (1 << 16), /* Only for RGB 16 bits per pixel */
    kCGImagePixelFormatRGB565    = (2 << 16), /* Only for RGB 16 bits per pixel */
    kCGImagePixelFormatRGB101010 = (3 << 16), /* Only for RGB 32 bits per pixel */
    kCGImagePixelFormatRGBCIF10  = (4 << 16), /* Only for RGB 32 bits per pixel */
} CG_AVAILABLE_STARTING(10.14, 12.0);

typedef CF_OPTIONS(uint32_t, CGBitmapInfo) {
    kCGBitmapAlphaInfoMask = 0x1F,

    kCGBitmapFloatInfoMask = 0xF00,
    kCGBitmapFloatComponents = (1 << 8),

    kCGBitmapByteOrderMask     = kCGImageByteOrderMask,
    kCGBitmapByteOrderDefault  = kCGImageByteOrderDefault,
    kCGBitmapByteOrder16Little = kCGImageByteOrder16Little,
    kCGBitmapByteOrder32Little = kCGImageByteOrder32Little,
    kCGBitmapByteOrder16Big    = kCGImageByteOrder16Big,
    kCGBitmapByteOrder32Big    = kCGImageByteOrder32Big
} CG_AVAILABLE_STARTING(10.0, 2.0);

#ifdef __BIG_ENDIAN__
# define kCGBitmapByteOrder16Host kCGBitmapByteOrder16Big
# define kCGBitmapByteOrder32Host kCGBitmapByteOrder32Big
#else    /* Little endian. */
# define kCGBitmapByteOrder16Host kCGBitmapByteOrder16Little
# define kCGBitmapByteOrder32Host kCGBitmapByteOrder32Little
#endif

/* Return the CFTypeID for CGImageRefs. */

CG_EXTERN CFTypeID CGImageGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create an image. */

CG_EXTERN CGImageRef __nullable CGImageCreate(size_t width, size_t height,
    size_t bitsPerComponent, size_t bitsPerPixel, size_t bytesPerRow,
    CGColorSpaceRef cg_nullable space, CGBitmapInfo bitmapInfo,
    CGDataProviderRef cg_nullable provider,
    const CGFloat * __nullable decode, bool shouldInterpolate,
    CGColorRenderingIntent intent)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Create an image mask. Legal values for bits per component are 1, 2, 4 and 8.
 * Bits per pixel must be either the same as bits per component or 8, with
 * exception of 8 bits per component where bits per pixel can be also 16 or 32.
 */

CG_EXTERN CGImageRef __nullable CGImageMaskCreate(size_t width, size_t height,
    size_t bitsPerComponent, size_t bitsPerPixel, size_t bytesPerRow,
    CGDataProviderRef cg_nullable provider, const CGFloat * __nullable decode,
    bool shouldInterpolate)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return a copy of `image'. Only the image structure itself is copied; the
   underlying data is not. */

CG_EXTERN CGImageRef __nullable CGImageCreateCopy(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create an image from `source', a data provider of JPEG-encoded data. */

CG_EXTERN CGImageRef __nullable CGImageCreateWithJPEGDataProvider(
    CGDataProviderRef cg_nullable source, const CGFloat * __nullable decode,
    bool shouldInterpolate,
    CGColorRenderingIntent intent)
    CG_AVAILABLE_STARTING(10.1, 2.0);

/* Create an image using `source', a data provider for PNG-encoded data. */

CG_EXTERN CGImageRef __nullable CGImageCreateWithPNGDataProvider(
    CGDataProviderRef cg_nullable source, const CGFloat * __nullable decode,
    bool shouldInterpolate,
    CGColorRenderingIntent intent)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create an image using the data contained within the subrectangle `rect'
   of `image'.

   The new image is created by
     1) adjusting `rect' to integral bounds by calling "CGRectIntegral";
     2) intersecting the result with a rectangle with origin (0, 0) and size
        equal to the size of `image';
     3) referencing the pixels within the resulting rectangle, treating the
        first pixel of the image data as the origin of the image.
   If the resulting rectangle is the null rectangle, this function returns
   NULL.

   If W and H are the width and height of image, respectively, then the
   point (0,0) corresponds to the first pixel of the image data; the point
   (W-1, 0) is the last pixel of the first row of the image data; (0, H-1)
   is the first pixel of the last row of the image data; and (W-1, H-1) is
   the last pixel of the last row of the image data.

   The resulting image retains a reference to the original image, so you may
   release the original image after calling this function. */

CG_EXTERN CGImageRef __nullable CGImageCreateWithImageInRect(
    CGImageRef cg_nullable image, CGRect rect)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a new image from `image' masked by `mask', which may be an image
   mask or an image.

   If `mask' is an image mask, then it indicates which parts of the context
   are to be painted with the image when drawn in a context, and which are
   to be masked out (left unchanged). The source samples of the image mask
   determine which areas are painted, acting as an "inverse alpha": if the
   value of a source sample in the image mask is S, then the corresponding
   region in `image' is blended with the destination using an alpha of
   (1-S). (For example, if S is 1, then the region is not painted, while if
   S is 0, the region is fully painted.)

   If `mask' is an image, then it serves as alpha mask for blending the
   image onto the destination. The source samples of `mask' determine which
   areas are painted: if the value of the source sample in mask is S, then
   the corresponding region in image is blended with the destination with an
   alpha of S. (For example, if S is 0, then the region is not painted,
   while if S is 1, the region is fully painted.)

   The parameter `image' may not be an image mask and may not have an image
   mask or masking color associated with it.
  
   If `mask' is an image, then it must be in a monochrome color space
   (e.g. DeviceGray, GenericGray, etc...), may not have alpha, and may not
   itself be masked by an image mask or a masking color. */

CG_EXTERN CGImageRef __nullable CGImageCreateWithMask(
    CGImageRef cg_nullable image, CGImageRef cg_nullable mask)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a new image from `image' masked by `components', an array of 2N
   values { min[1], max[1], ... min[N], max[N] } where N is the number of
   components in color space of `image'. Any image sample with color value
   {c[1], ... c[N]} where min[i] <= c[i] <= max[i] for 1 <= i <= N is masked
   out (that is, not painted).

   Each value in `components' must be a valid image sample value: if `image'
   has integral pixel components, then each value of must be in the range
   [0..2**bitsPerComponent - 1] (where `bitsPerComponent' is the number of
   bits/component of `image'); if `image' has floating-point pixel
   components, then each value may be any floating-point number which is a
   valid color component.

   The parameter `image' may not be an image mask, and may not already have
   an image mask or masking color associated with it. */

CG_EXTERN CGImageRef __nullable CGImageCreateWithMaskingColors(
    CGImageRef cg_nullable image, const CGFloat * cg_nullable components)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Create a copy of `image', replacing the image's color space with `space'.
   Returns NULL if `image' is an image mask, or if the number of components
   of `space' isn't the same as the number of components of the color space
   of `image'. */

CG_EXTERN CGImageRef __nullable CGImageCreateCopyWithColorSpace(
    CGImageRef cg_nullable image, CGColorSpaceRef cg_nullable space)
    CG_AVAILABLE_STARTING(10.3, 2.0);

/* Equivalent to `CFRetain(image)'. */

CG_EXTERN CGImageRef cg_nullable CGImageRetain(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRelease(image)'. */

CG_EXTERN void CGImageRelease(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return true if `image' is an image mask, false otherwise. */

CG_EXTERN bool CGImageIsMask(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the width of `image'. */

CG_EXTERN size_t CGImageGetWidth(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the height of `image'. */

CG_EXTERN size_t CGImageGetHeight(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the number of bits/component of `image'. */

CG_EXTERN size_t CGImageGetBitsPerComponent(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the number of bits/pixel of `image'. */

CG_EXTERN size_t CGImageGetBitsPerPixel(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the number of bytes/row of `image'. */

CG_EXTERN size_t CGImageGetBytesPerRow(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the color space of `image', or NULL if `image' is an image
   mask. */

CG_EXTERN CGColorSpaceRef __nullable CGImageGetColorSpace(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the alpha info of `image'. */

CG_EXTERN CGImageAlphaInfo CGImageGetAlphaInfo(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the data provider of `image'. */

CG_EXTERN CGDataProviderRef __nullable CGImageGetDataProvider(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the decode array of `image'. */

CG_EXTERN const CGFloat * __nullable CGImageGetDecode(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the interpolation parameter of `image'. */

CG_EXTERN bool CGImageGetShouldInterpolate(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the rendering intent of `image'. */

CG_EXTERN CGColorRenderingIntent CGImageGetRenderingIntent(cg_nullable CGImageRef image)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the bitmap info of `image'. */

CG_EXTERN CGBitmapInfo CGImageGetBitmapInfo(CGImageRef cg_nullable image)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the byte order info of `image'. */

CG_EXTERN CGImageByteOrderInfo CGImageGetByteOrderInfo(CGImageRef cg_nullable image) CG_AVAILABLE_STARTING(10.14, 12.0);

/* Return the pixel format of `image'. */

CG_EXTERN CGImagePixelFormatInfo CGImageGetPixelFormatInfo(CGImageRef cg_nullable image) CG_AVAILABLE_STARTING(10.14, 12.0);

/* Return the UTType of `image'. */

CG_EXTERN CFStringRef  __nullable CGImageGetUTType(cg_nullable CGImageRef image)
    CG_AVAILABLE_STARTING(10.11, 9.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif  /* CGIMAGE_H_ */
