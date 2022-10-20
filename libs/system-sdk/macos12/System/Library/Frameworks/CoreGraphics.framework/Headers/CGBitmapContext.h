/* CoreGraphics - CGBitmapContext.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGBITMAPCONTEXT_H_
#define CGBITMAPCONTEXT_H_

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGContext.h>
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* The callback for releasing the data supplied to
   `CGBitmapContextCreateWithData'. */

typedef void (*CGBitmapContextReleaseDataCallback)(void * __nullable releaseInfo,
    void * __nullable data);

/* Create a bitmap context. The context draws into a bitmap which is `width'
   pixels wide and `height' pixels high. The number of components for each
   pixel is specified by `space', which may also specify a destination color
   profile. Note that the only legal case when `space' can be NULL is when
   alpha is specified as kCGImageAlphaOnly. The number of bits for each
   component of a pixel is specified by `bitsPerComponent'. The number of bytes
   per pixel is equal to `(bitsPerComponent * number of components + 7)/8'.
   Each row of the bitmap consists of `bytesPerRow' bytes, which must be at
   least `width * bytes per pixel' bytes; in addition, `bytesPerRow' must be
   an integer multiple of the number of bytes per pixel. `data', if non-NULL,
   points to a block of memory at least `bytesPerRow * height' bytes. If `data'
   is NULL, the context will allocate the data itself; this data will be freed
   when the context is deallocated. `bitmapInfo' specifies whether the bitmap
   should contain an alpha channel and how it's to be generated, along with
   whether the components are floating-point or integer. If `releaseCallback' is
   non-NULL, it is called when the context is freed with `releaseInfo' and
   `data' as arguments. */

CG_EXTERN CGContextRef __nullable CGBitmapContextCreateWithData(
    void * __nullable data, size_t width, size_t height, size_t bitsPerComponent,
    size_t bytesPerRow, CGColorSpaceRef cg_nullable space, uint32_t bitmapInfo,
    CGBitmapContextReleaseDataCallback __nullable releaseCallback,
    void * __nullable releaseInfo)
    CG_AVAILABLE_STARTING(10.6, 4.0);

/* Create a bitmap context. The context draws into a bitmap which is `width'
   pixels wide and `height' pixels high. The number of components for each
   pixel is specified by `space', which may also specify a destination color
   profile.  Note that the only legal case when `space' can be NULL is when
   alpha is specified as kCGImageAlphaOnly.The number of bits for each component
   of a pixel is specified by `bitsPerComponent'. The number of bytes per pixel
   is equal to `(bitsPerComponent * number of components + 7)/8'. Each row of
   the bitmap consists of `bytesPerRow' bytes, which must be at least
   `width * bytes per pixel' bytes; in addition, `bytesPerRow' must be an
   integer multiple of the number of bytes per pixel. `data', if non-NULL,
   points to a block of memory at least `bytesPerRow * height' bytes.
   If `data' is NULL, the data for context is allocated automatically and freed
   when the context is deallocated. `bitmapInfo' specifies whether the bitmap
   should contain an alpha channel and how it's to be generated, along with
   whether the components are floating-point or integer. */

CG_EXTERN CGContextRef __nullable CGBitmapContextCreate(void * __nullable data,
    size_t width, size_t height, size_t bitsPerComponent, size_t bytesPerRow,
    CGColorSpaceRef cg_nullable space, uint32_t bitmapInfo)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Return the data associated with the bitmap context `context', or NULL if
   `context' is not a bitmap context. */

CG_EXTERN void * __nullable CGBitmapContextGetData(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the width of the bitmap context `context', or 0 if `context' is
   not a bitmap context. */

CG_EXTERN size_t CGBitmapContextGetWidth(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the height of the bitmap context `context', or 0 if `context' is
   not a bitmap context. */

CG_EXTERN size_t CGBitmapContextGetHeight(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the bits per component of the bitmap context `context', or 0 if
   `context' is not a bitmap context. */

CG_EXTERN size_t CGBitmapContextGetBitsPerComponent(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the bits per pixel of the bitmap context `context', or 0 if
   `context' is not a bitmap context. */

CG_EXTERN size_t CGBitmapContextGetBitsPerPixel(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the bytes per row of the bitmap context `context', or 0 if
   `context' is not a bitmap context. */

CG_EXTERN size_t CGBitmapContextGetBytesPerRow(CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the color space of the bitmap context `context', or NULL if
   `context' is not a bitmap context. */

CG_EXTERN CGColorSpaceRef __nullable CGBitmapContextGetColorSpace(
    CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the alpha info of the bitmap context `context', or
   "kCGImageAlphaNone" if `context' is not a bitmap context. */

CG_EXTERN CGImageAlphaInfo CGBitmapContextGetAlphaInfo(
    CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the bitmap info of the bitmap context `context', or 0 if `context'
   is not a bitmap context. */

CG_EXTERN CGBitmapInfo CGBitmapContextGetBitmapInfo(
    CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return an image containing a snapshot of the bitmap context `context'. If
   context is not a bitmap context, or if the image cannot be created for
   any reason, this function returns NULL. This is a "copy" operation ---
   subsequent changes to context will not affect the contents of the
   returned image.

   Note that in some cases the copy will actually follow "copy-on-write"
   semantics, so that the actual physical copy of the bits will only occur
   if the underlying data in the bitmap context is modified. As a
   consequence, you may wish to use the resulting image and release it
   before performing more drawing into the bitmap context; in this way, the
   actual physical copy of the data may be avoided. */

CG_EXTERN CGImageRef __nullable CGBitmapContextCreateImage(
    CGContextRef cg_nullable context)
    CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGBITMAPCONTEXT_H_ */
