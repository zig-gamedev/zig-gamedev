/* CoreGraphics - CGPath.h
   Copyright (c) 2001-2011 Apple Inc.
   All rights reserved. */

#ifndef CGPATH_H_
#define CGPATH_H_

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFAvailability.h>
#include <stdint.h>

typedef struct CF_BRIDGED_TYPE(id) CGPath *CGMutablePathRef;
typedef const struct CF_BRIDGED_TYPE(id) CGPath *CGPathRef;

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGAffineTransform.h>
#include <CoreFoundation/CFBase.h>

CF_IMPLICIT_BRIDGING_ENABLED

CF_ASSUME_NONNULL_BEGIN

/* Line join styles. */

typedef CF_ENUM(int32_t, CGLineJoin) {
    kCGLineJoinMiter,
    kCGLineJoinRound,
    kCGLineJoinBevel
};

/* Line cap styles. */

typedef CF_ENUM(int32_t, CGLineCap) {
    kCGLineCapButt,
    kCGLineCapRound,
    kCGLineCapSquare
};

/* Return the CFTypeID for CGPathRefs. */

CG_EXTERN CFTypeID CGPathGetTypeID(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a mutable path. */

CG_EXTERN CGMutablePathRef  CGPathCreateMutable(void)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a copy of `path'. */

CG_EXTERN CGPathRef __nullable CGPathCreateCopy(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a copy of `path' transformed by `transform'. */

CG_EXTERN CGPathRef __nullable CGPathCreateCopyByTransformingPath(
    CGPathRef cg_nullable path, const CGAffineTransform * __nullable transform)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Create a mutable copy of `path'. */

CG_EXTERN CGMutablePathRef __nullable CGPathCreateMutableCopy(
    CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Create a mutable copy of `path' transformed by `transform'. */

CG_EXTERN CGMutablePathRef __nullable CGPathCreateMutableCopyByTransformingPath(
    CGPathRef cg_nullable path, const CGAffineTransform * __nullable transform)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Return a path representing a rectangle bounded by `rect'. The rectangle
   forms a complete subpath of the path --- that is, it begins with a "move
   to" and ends with a "close subpath" --- oriented in the clockwise
   direction. If `transform' is non-NULL, then the lines representing the
   rectangle will be transformed by `transform' before they are added to the
   path. */

CG_EXTERN CGPathRef  CGPathCreateWithRect(CGRect rect,
    const CGAffineTransform * __nullable transform)
    CG_AVAILABLE_STARTING(10.5, 4.0);

/* Return a path representing an ellipse bounded by `rect'. The ellipse is
   approximated by a sequence of Bézier curves. The center of the ellipse is
   the midpoint of `rect'. If `rect' is square, then the ellipse will be
   circular with radius equal to one-half the width (equivalently, one-half
   the height) of `rect'. If `rect' is rectangular, then the major- and
   minor-axes will be the `width' and `height' of rect. The ellipse forms a
   complete subpath of the path --- that is, it begins with a "move to" and
   ends with a "close subpath" --- oriented in the clockwise direction. If
   `transform' is non-NULL, then the constructed Bézier curves representing
   the ellipse will be transformed by `transform' before they are added to
   the path. */

CG_EXTERN CGPathRef  CGPathCreateWithEllipseInRect(CGRect rect,
    const CGAffineTransform * __nullable transform)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Return a path representing a rounded rectangle. The rounded rectangle
   coincides with the edges of `rect'. Each corner is consists of
   one-quarter of an ellipse with axes equal to `cornerWidth' and
   `cornerHeight'. The rounded rectangle forms a complete subpath of the
   path --- that is, it begins with a "move to" and ends with a "close
   subpath" --- oriented in the clockwise direction. If `transform' is
   non-NULL, then the path elements representing the rounded rectangle will
   be transformed by `transform' before they are added to the path. */

CG_EXTERN CGPathRef  CGPathCreateWithRoundedRect(CGRect rect,
    CGFloat cornerWidth, CGFloat cornerHeight,
    const CGAffineTransform * __nullable transform)
    CG_AVAILABLE_STARTING(10.9, 7.0);

/* Add a rounded rectangle to `path'. The rounded rectangle coincides with
   the edges of `rect'. Each corner is consists of one-quarter of an ellipse
   with axes equal to `cornerWidth' and `cornerHeight'. The rounded
   rectangle forms a complete subpath of the path --- that is, it begins
   with a "move to" and ends with a "close subpath" --- oriented in the
   clockwise direction. If `transform' is non-NULL, then the path elements
   representing the rounded rectangle will be transformed by `transform'
   before they are added to the path. */

CG_EXTERN void CGPathAddRoundedRect(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable transform, CGRect rect,
    CGFloat cornerWidth, CGFloat cornerHeight)
    CG_AVAILABLE_STARTING(10.9, 7.0);

/* Create a dashed path from `path'. The parameters `phase', `lengths', and
   `count' have the same meaning as the corresponding parameters for
   `CGContextSetLineDash'. If `transform' is non-NULL, then the elements of
   the constructed path will be transformed by `transform' before they are
   added to the path. */

CG_EXTERN CGPathRef __nullable CGPathCreateCopyByDashingPath(
    CGPathRef cg_nullable path, const CGAffineTransform * __nullable transform,
    CGFloat phase, const CGFloat * __nullable lengths, size_t count)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Create a stroked path from `path'. The parameters `lineWidth', `lineCap',
   `lineJoin', and `miterLimit' have the same meaning as the corresponding
   CGContext parameters. If `transform' is non-NULL, then the elements of
   the constructed path will be transformed by `transform' before they are
   added to the path. */

CG_EXTERN CGPathRef __nullable CGPathCreateCopyByStrokingPath(
    CGPathRef cg_nullable path, const CGAffineTransform * __nullable transform,
    CGFloat lineWidth, CGLineCap lineCap,
    CGLineJoin lineJoin, CGFloat miterLimit)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Equivalent to `CFRetain(path)', except it doesn't crash (as CFRetain
   does) if `path' is NULL. */

CG_EXTERN CGPathRef cg_nullable CGPathRetain(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Equivalent to `CFRelease(path)', except it doesn't crash (as CFRelease
   does) if `path' is NULL. */

CG_EXTERN void CGPathRelease(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return true if `path1' is equal to `path2'; false otherwise. */

CG_EXTERN bool CGPathEqualToPath(CGPathRef cg_nullable path1,
    CGPathRef cg_nullable path2)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/*** Path construction functions. ***/

/* Move the current point to `(x, y)' in `path' and begin a new subpath. If
   `m' is non-NULL, then transform `(x, y)' by `m' first. */

CG_EXTERN void CGPathMoveToPoint(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGFloat x, CGFloat y)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Append a straight line segment from the current point to `(x, y)' in
   `path' and move the current point to `(x, y)'. If `m' is non-NULL, then
   transform `(x, y)' by `m' first. */

CG_EXTERN void CGPathAddLineToPoint(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGFloat x, CGFloat y)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Append a quadratic curve from the current point to `(x, y)' with control
   point `(cpx, cpy)' in `path' and move the current point to `(x, y)'. If
   `m' is non-NULL, then transform all points by `m' first. */

CG_EXTERN void CGPathAddQuadCurveToPoint(CGMutablePathRef cg_nullable path,
    const CGAffineTransform *__nullable m, CGFloat cpx, CGFloat cpy,
    CGFloat x, CGFloat y)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Append a cubic Bézier curve from the current point to `(x,y)' with
   control points `(cp1x, cp1y)' and `(cp2x, cp2y)' in `path' and move the
   current point to `(x, y)'. If `m' is non-NULL, then transform all points
   by `m' first. */

CG_EXTERN void CGPathAddCurveToPoint(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGFloat cp1x, CGFloat cp1y,
    CGFloat cp2x, CGFloat cp2y, CGFloat x, CGFloat y)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Append a line from the current point to the starting point of the current
   subpath of `path' and end the subpath. */

CG_EXTERN void CGPathCloseSubpath(CGMutablePathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/*** Path construction convenience functions. ***/

/* Add `rect' to `path'. If `m' is non-NULL, then first transform `rect' by
   `m' before adding it to `path'. */

CG_EXTERN void CGPathAddRect(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGRect rect)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Add each rectangle specified by `rects', an array of `count' CGRects, to
   `path'. If `m' is non-NULL, then first transform each rectangle by `m'
   before adding it to `path'. */

CG_EXTERN void CGPathAddRects(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, const CGRect * __nullable rects,
    size_t count)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Move to the first element of `points', an array of `count' CGPoints, and
   append a line from each point to the next point in `points'. If `m' is
   non-NULL, then first transform each point by `m'. */

CG_EXTERN void CGPathAddLines(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, const CGPoint * __nullable points,
    size_t count)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Add an ellipse (an oval) inside `rect' to `path'. The ellipse is
   approximated by a sequence of Bézier curves. The center of the ellipse is
   the midpoint of `rect'. If `rect' is square, then the ellipse will be
   circular with radius equal to one-half the width (equivalently, one-half
   the height) of `rect'. If `rect' is rectangular, then the major- and
   minor-axes will be the `width' and `height' of rect. The ellipse forms a
   complete subpath of `path' --- that is, it begins with a "move to" and
   ends with a "close subpath" --- oriented in the clockwise direction. If
   `m' is non-NULL, then the constructed Bézier curves representing the
   ellipse will be transformed by `m' before they are added to `path'. */

CG_EXTERN void CGPathAddEllipseInRect(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGRect rect)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* Add an arc of a circle to `path', possibly preceded by a straight line
   segment. The arc is approximated by a sequence of Bézier curves. The
   center of the arc is `(x,y)'; `radius' is its radius. `startAngle' is the
   angle to the first endpoint of the arc, measured counter-clockwise from
   the positive x-axis. `startAngle + delta' is the angle to the second
   endpoint of the arc. If `delta' is positive, then the arc is drawn
   counter-clockwise; if negative, clockwise. `startAngle' and `delta' are
   measured in radians. If `matrix' is non-NULL, then the constructed Bézier
   curves representing the arc will be transformed by `matrix' before they
   are added to the path. */

CG_EXTERN void CGPathAddRelativeArc(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable matrix, CGFloat x, CGFloat y,
    CGFloat radius, CGFloat startAngle, CGFloat delta)
    CG_AVAILABLE_STARTING(10.7, 5.0);

/* Add an arc of a circle to `path', possibly preceded by a straight line
   segment. The arc is approximated by a sequence of Bézier curves. `(x, y)'
   is the center of the arc; `radius' is its radius; `startAngle' is the
   angle to the first endpoint of the arc; `endAngle' is the angle to the
   second endpoint of the arc; and `clockwise' is true if the arc is to be
   drawn clockwise, false otherwise. `startAngle' and `endAngle' are
   measured in radians. If `m' is non-NULL, then the constructed Bézier
   curves representing the arc will be transformed by `m' before they are
   added to `path'.

   Note that using values very near 2π can be problematic. For example,
   setting `startAngle' to 0, `endAngle' to 2π, and `clockwise' to true will
   draw nothing. (It's easy to see this by considering, instead of 0 and 2π,
   the values ε and 2π - ε, where ε is very small.) Due to round-off error,
   however, it's possible that passing the value `2 * M_PI' to approximate
   2π will numerically equal to 2π + δ, for some small δ; this will cause a
   full circle to be drawn.

   If you want a full circle to be drawn clockwise, you should set
   `startAngle' to 2π, `endAngle' to 0, and `clockwise' to true. This avoids
   the instability problems discussed above. */

CG_EXTERN void CGPathAddArc(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m,
    CGFloat x, CGFloat y, CGFloat radius, CGFloat startAngle, CGFloat endAngle,
    bool clockwise)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Add an arc of a circle to `path', possibly preceded by a straight line
   segment. The arc is approximated by a sequence of Bézier curves. `radius'
   is the radius of the arc. The resulting arc is tangent to the line from
   the current point of `path' to `(x1, y1)', and the line from `(x1, y1)'
   to `(x2, y2)'. If `m' is non-NULL, then the constructed Bézier curves
   representing the arc will be transformed by `m' before they are added to
   `path'. */

CG_EXTERN void CGPathAddArcToPoint(CGMutablePathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGFloat x1, CGFloat y1,
    CGFloat x2, CGFloat y2, CGFloat radius)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Add `path2' to `path1'. If `m' is non-NULL, then the points in `path2'
   will be transformed by `m' before they are added to `path1'. */

CG_EXTERN void CGPathAddPath(CGMutablePathRef cg_nullable path1,
    const CGAffineTransform * __nullable m, CGPathRef cg_nullable path2)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/*** Path information functions. ***/

/* Return true if `path' contains no elements, false otherwise. */

CG_EXTERN bool CGPathIsEmpty(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return true if `path' represents a rectangle, false otherwise. */

CG_EXTERN bool CGPathIsRect(CGPathRef cg_nullable path, CGRect * __nullable rect)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the current point of the current subpath of `path'. If there is no
   current point, then return CGPointZero. */

CG_EXTERN CGPoint CGPathGetCurrentPoint(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the bounding box of `path'. The bounding box is the smallest
   rectangle completely enclosing all points in the path, including control
   points for Bézier cubic and quadratic curves. If the path is empty, then
   return `CGRectNull'. */

CG_EXTERN CGRect CGPathGetBoundingBox(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.2, 2.0);

/* Return the path bounding box of `path'. The path bounding box is the
   smallest rectangle completely enclosing all points in the path, *not*
   including control points for Bézier cubic and quadratic curves. If the
   path is empty, then return `CGRectNull'. */

CG_EXTERN CGRect CGPathGetPathBoundingBox(CGPathRef cg_nullable path)
    CG_AVAILABLE_STARTING(10.6, 4.0);

/* Return true if `point' is contained in `path'; false otherwise. A point
   is contained in a path if it is inside the painted region when the path
   is filled; if `eoFill' is true, then the even-odd fill rule is used to
   evaluate the painted region of the path, otherwise, the winding-number
   fill rule is used. If `m' is non-NULL, then the point is transformed by
   `m' before determining whether the path contains it. */

CG_EXTERN bool CGPathContainsPoint(CGPathRef cg_nullable path,
    const CGAffineTransform * __nullable m, CGPoint point, bool eoFill)
    CG_AVAILABLE_STARTING(10.4, 2.0);

/* The types of path elements returned by `CGPathApply'. */

typedef CF_ENUM(int32_t, CGPathElementType) {
  kCGPathElementMoveToPoint,
  kCGPathElementAddLineToPoint,
  kCGPathElementAddQuadCurveToPoint,
  kCGPathElementAddCurveToPoint,
  kCGPathElementCloseSubpath
};

/* An element of a path returned by `CGPathApply'. */

struct CGPathElement {
  CGPathElementType type;
  CGPoint *  points;
};
typedef struct CGPathElement CGPathElement;

/* The prototype for the function called by `CGPathApplyFunction'. */

typedef void (*CGPathApplierFunction)(void * __nullable info,
  const CGPathElement *  element);

/* For element of `path', call `function', passing it the path element and
   `info'. */

CG_EXTERN void CGPathApply(CGPathRef cg_nullable path, void * __nullable info,
    CGPathApplierFunction cg_nullable function)
    CG_AVAILABLE_STARTING(10.2, 2.0);

typedef void (^CGPathApplyBlock)(const CGPathElement * element);

CG_EXTERN void CGPathApplyWithBlock(CGPathRef path, CGPathApplyBlock CF_NOESCAPE block)
    CG_AVAILABLE_STARTING(10.13, 11.0);

CF_ASSUME_NONNULL_END

CF_IMPLICIT_BRIDGING_DISABLED

#endif /* CGPATH_H_ */
