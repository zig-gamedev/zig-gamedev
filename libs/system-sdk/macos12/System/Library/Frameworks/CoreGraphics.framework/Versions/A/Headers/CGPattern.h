/* CoreGraphics - CGPattern.h
   Copyright (c) 2000-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPATTERN_H_
#define CGPATTERN_H_

#include <CoreFoundation/CFBase.h>

typedef struct CF_BRIDGED_TYPE(id) CGPattern *CGPatternRef;

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGContext.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* kCGPatternTilingNoDistortion: The pattern cell is not distorted when
   painted, however the spacing between pattern cells may vary by as much as
   1 device pixel.

   kCGPatternTilingConstantSpacingMinimalDistortion: Pattern cells are
   spaced consistently, however the pattern cell may be distorted by as much
   as 1 device pixel when the pattern is painted.

   kCGPatternTilingConstantSpacing: Pattern cells are spaced consistently as
   with kCGPatternTilingConstantSpacingMinimalDistortion, however the
   pattern cell may be distorted additionally to permit a more efficient
   implementation. */

typedef CF_ENUM (int32_t, CGPatternTiling) {
    kCGPatternTilingNoDistortion,
    kCGPatternTilingConstantSpacingMinimalDistortion,
    kCGPatternTilingConstantSpacing
};

/* The drawing of the pattern is delegated to the callbacks. The callbacks
   may be called one or many times to draw the pattern.
    `version' is the version number of the structure passed in as a
      parameter to the CGPattern creation functions. The structure defined
      below is version 0.
    `drawPattern' should draw the pattern in the context `c'. `info' is the
      parameter originally passed to the CGPattern creation functions.
    `releaseInfo' is called when the pattern is deallocated. */

typedef void (*CGPatternDrawPatternCallback)(void * __nullable info,
                                             CGContextRef cg_nullable context);
typedef void (*CGPatternReleaseInfoCallback)(void * __nullable info);

struct CGPatternCallbacks {
    unsigned int version;
    CGPatternDrawPatternCallback __nullable drawPattern;
    CGPatternReleaseInfoCallback __nullable releaseInfo;
};
typedef struct CGPatternCallbacks CGPatternCallbacks;

/* Return the CFTypeID for CGPatternRefs. */

CG_EXTERN CFTypeID CGPatternGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a pattern. */

CG_EXTERN CGPatternRef __nullable CGPatternCreate(void * __nullable info,
    CGRect bounds, CGAffineTransform matrix, CGFloat xStep, CGFloat yStep,
    CGPatternTiling tiling, bool isColored,
    const CGPatternCallbacks * cg_nullable callbacks)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRetain(pattern)', except it doesn't crash (as CF does)
   if `pattern' is NULL. */

CG_EXTERN CGPatternRef cg_nullable CGPatternRetain(CGPatternRef cg_nullable pattern)
    CG_AVAILABLE_STARTING(10.0, 2.0);

/* Equivalent to `CFRelease(pattern)', except it doesn't crash (as CF does)
   if `pattern' is NULL. */

CG_EXTERN void CGPatternRelease(CGPatternRef cg_nullable pattern)
    CG_AVAILABLE_STARTING(10.0, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPATTERN_H_ */
