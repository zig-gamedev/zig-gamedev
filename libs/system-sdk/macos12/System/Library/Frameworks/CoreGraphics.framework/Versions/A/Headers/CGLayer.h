/* CoreGraphics - CGLayer.h
 * Copyright (c) 2004-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGLAYER_H_
#define CGLAYER_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGLayer *CGLayerRef;

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGContext.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Create a layer of size `size' relative to the context `context'. The
   value of `size' is specified in default user space (base space) units.
   The parameter `auxiliaryInfo' should be NULL; it is reserved for future
   expansion. */

CG_EXTERN CGLayerRef __nullable CGLayerCreateWithContext(
    CGContextRef cg_nullable context,
    CGSize size, CFDictionaryRef __nullable auxiliaryInfo)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Equivalent to `CFRetain(layer)', except it doesn't crash (as CFRetain
   does) if `layer' is NULL. */

CG_EXTERN CGLayerRef cg_nullable CGLayerRetain(CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Equivalent to `CFRelease(layer)', except it doesn't crash (as CFRelease
   does) if `layer' is NULL. */

CG_EXTERN void CGLayerRelease(CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the size of the layer `layer'. CGSizeZero if invalid `layer'. */

CG_EXTERN CGSize CGLayerGetSize(CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the context of `layer'. */

CG_EXTERN CGContextRef __nullable CGLayerGetContext(CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Draw the contents of `layer' into `rect' of `context'. The contents are
   scaled, if necessary, to fit into `rect'; the rectangle `rect' is in user
   space. */

CG_EXTERN void CGContextDrawLayerInRect(CGContextRef cg_nullable context,
    CGRect rect, CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Draw the contents of `layer' at `point' in `context'. This is equivalent
   to calling "CGContextDrawLayerInRect" with a rectangle having origin at
   `point' and size equal to the size of `layer'. */

CG_EXTERN void CGContextDrawLayerAtPoint(CGContextRef cg_nullable context,
    CGPoint point, CGLayerRef cg_nullable layer)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Return the CFTypeID for CGLayerRefs. */

CG_EXTERN CFTypeID CGLayerGetTypeID(void)
    CG_AVAILABLE_STARTING(10.4, 2.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif  /* CGLAYER_H_ */
