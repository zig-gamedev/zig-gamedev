/* CoreGraphics - CGGradient.h
   Copyright (c) 2006-2011 Apple Inc.
   All rights reserved. */

#ifndef CGGRADIENT_H_
#define CGGRADIENT_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGGradient *CGGradientRef;

typedef CF_OPTIONS (uint32_t, CGGradientDrawingOptions) {
  kCGGradientDrawsBeforeStartLocation = (1 << 0),
  kCGGradientDrawsAfterEndLocation = (1 << 1)
};

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGColorSpace.h>
#include <CoreGraphics/CGContext.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* A CGGradient defines a transition between colors. The transition is
   defined over a range from 0 to 1 inclusive. A gradient specifies a color
   at location 0, one at location 1, and possibly additional colors assigned
   to locations between 0 and 1.

   A CGGradient has a color space. When a gradient is created, all colors
   specified are converted to that color space, and interpolation of colors
   occurs using the components of that color space. See the documentation of
   each creation function for more details. */

/* Return the CFTypeID for CGGradients. */

CG_EXTERN CFTypeID CGGradientGetTypeID(void)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* Creates a gradient by pairing the color components provided in
   `components' with locations provided in `locations'. If `locations' is
   NULL, the first color in `colors' will be at location 0, the last color
   in `colors' will be at location 1, and intervening colors will be at
   equal intervals in between. Otherwise, each location in `locations'
   should be a CGFloat between 0 and 1. Each set of color components should
   specify a color in the color space `space' (which may not be a pattern or
   indexed color space). The number of locations is specified by `count';
   the number of color components is the product of `count' and the number
   of color components of `space'. If no color is provided for 0 or 1, the
   gradient will use the color provided at the locations closest to 0 and 1
   for those values. */

CG_EXTERN CGGradientRef __nullable CGGradientCreateWithColorComponents(
    CGColorSpaceRef cg_nullable space, const CGFloat * cg_nullable components,
    const CGFloat * __nullable locations, size_t count)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* Creates a gradient by pairing the colors provided in `colors' with the
   locations provided in `locations'. `colors' should be a non-empty array
   of CGColor objects. The colors may be in any color space other than a
   pattern or indexed color space. If `space' is non-NULL, each color will
   be converted to that color space and the gradient will drawn in that
   space; otherwise, each color will be converted to and drawn in the
   "Generic" RGB color space. If `space' is specified, it may not be a
   pattern or indexed color space. If `locations' is NULL, the first color
   in `colors' will be at location 0, the last color in `colors' will be at
   location 1, and intervening colors will be at equal intervals in between.
   Otherwise, each location in `locations' should be a CGFloat between 0 and
   1; the array of locations should should contain the same number of items
   as `colors'. If no color is provided for 0 or 1, the gradient will use
   the color provided at the locations closest to 0 and 1 for those
   values. */

CG_EXTERN CGGradientRef __nullable CGGradientCreateWithColors(
    CGColorSpaceRef __nullable space, CFArrayRef cg_nullable colors,
    const CGFloat * __nullable locations)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* Equivalent to `CFRetain' except that it doesn't crash (as `CFRetain'
   does) if `gradient' is NULL. */

CG_EXTERN CGGradientRef cg_nullable CGGradientRetain(
    CGGradientRef cg_nullable gradient)
    CG_AVAILABLE_STARTING(10.5, 2.0);

/* Equivalent to `CFRelease' except that it doesn't crash (as `CFRelease'
   does) if `gradient' is NULL. */

CG_EXTERN void CGGradientRelease(CGGradientRef cg_nullable gradient)
    CG_AVAILABLE_STARTING(10.5, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGGRADIENT_H_ */
