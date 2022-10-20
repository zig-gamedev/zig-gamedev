/*
        NSBezierPath.h
        Application Kit
        Copyright (c) 1997-2021, Apple Inc.
        All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>
#import <AppKit/NSFont.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSAffineTransform;

typedef NS_ENUM(NSUInteger, NSLineCapStyle) {
    NSLineCapStyleButt = 0,
    NSLineCapStyleRound = 1,
    NSLineCapStyleSquare = 2
};

typedef NS_ENUM(NSUInteger, NSLineJoinStyle) {
    NSLineJoinStyleMiter = 0,
    NSLineJoinStyleRound = 1,
    NSLineJoinStyleBevel = 2
};

typedef NS_ENUM(NSUInteger, NSWindingRule) {
    NSWindingRuleNonZero = 0,
    NSWindingRuleEvenOdd = 1
};

typedef NS_ENUM(NSUInteger, NSBezierPathElement) {
    NSBezierPathElementMoveTo,
    NSBezierPathElementLineTo,
    NSBezierPathElementCurveTo,
    NSBezierPathElementClosePath
};

@interface NSBezierPath : NSObject <NSCopying, NSSecureCoding>

// Creating common paths.

+ (NSBezierPath *)bezierPath;
+ (NSBezierPath *)bezierPathWithRect:(NSRect)rect;
+ (NSBezierPath *)bezierPathWithOvalInRect:(NSRect)rect;
+ (NSBezierPath *)bezierPathWithRoundedRect:(NSRect)rect xRadius:(CGFloat)xRadius yRadius:(CGFloat)yRadius API_AVAILABLE(macos(10.5));

// Immediate mode drawing of common paths.

+ (void)fillRect:(NSRect)rect;
+ (void)strokeRect:(NSRect)rect;
+ (void)clipRect:(NSRect)rect;
+ (void)strokeLineFromPoint:(NSPoint)point1 toPoint:(NSPoint)point2;
+ (void)drawPackedGlyphs:(const char *)packedGlyphs atPoint:(NSPoint)point;

// Default path rendering parameters.

@property (class) CGFloat defaultMiterLimit;
@property (class) CGFloat defaultFlatness;

@property (class) NSWindingRule defaultWindingRule;
@property (class) NSLineCapStyle defaultLineCapStyle;
@property (class) NSLineJoinStyle defaultLineJoinStyle;
@property (class) CGFloat defaultLineWidth;

// Path construction.

- (void)moveToPoint:(NSPoint)point;
- (void)lineToPoint:(NSPoint)point;
- (void)curveToPoint:(NSPoint)endPoint
       controlPoint1:(NSPoint)controlPoint1
       controlPoint2:(NSPoint)controlPoint2;
- (void)closePath;

- (void)removeAllPoints;

// Relative path construction.

- (void)relativeMoveToPoint:(NSPoint)point;
- (void)relativeLineToPoint:(NSPoint)point;
- (void)relativeCurveToPoint:(NSPoint)endPoint
	       controlPoint1:(NSPoint)controlPoint1
	       controlPoint2:(NSPoint)controlPoint2;

// Path rendering parameters.

@property CGFloat lineWidth;
@property NSLineCapStyle lineCapStyle;
@property NSLineJoinStyle lineJoinStyle;
@property NSWindingRule windingRule;
@property CGFloat miterLimit;
@property CGFloat flatness;
- (void)getLineDash:(nullable CGFloat *)pattern count:(nullable NSInteger *)count phase:(nullable CGFloat *)phase;
- (void)setLineDash:(nullable const CGFloat *)pattern count:(NSInteger)count phase:(CGFloat)phase;

// Path operations.

- (void)stroke;
- (void)fill;
- (void)addClip;
- (void)setClip;

// Path modifications.

@property (readonly, copy) NSBezierPath *bezierPathByFlatteningPath;
@property (readonly, copy) NSBezierPath *bezierPathByReversingPath;

// Applying transformations.

- (void)transformUsingAffineTransform:(NSAffineTransform *)transform;

// Path info

@property (getter=isEmpty, readonly) BOOL empty;
@property (readonly) NSPoint currentPoint;
@property (readonly) NSRect controlPointBounds;
@property (readonly) NSRect bounds;

// Elements.

@property (readonly) NSInteger elementCount;

// `points' should contain space for at least three points.  `points'
// may be NULL.  In the case of NSBezierPathElementCurveTo, the order
// of the points is controlPoint1 (points[0]), controlPoint2 (points[1]),
// endPoint (points[2]).
- (NSBezierPathElement)elementAtIndex:(NSInteger)index
		     associatedPoints:(nullable NSPointArray)points;
// As above with points == NULL.
- (NSBezierPathElement)elementAtIndex:(NSInteger)index;
- (void)setAssociatedPoints:(nullable NSPointArray)points atIndex:(NSInteger)index;

// Appending common paths

- (void)appendBezierPath:(NSBezierPath *)path;
- (void)appendBezierPathWithRect:(NSRect)rect;
- (void)appendBezierPathWithPoints:(NSPointArray)points count:(NSInteger)count;
- (void)appendBezierPathWithOvalInRect:(NSRect)rect;
- (void)appendBezierPathWithArcWithCenter:(NSPoint)center radius:(CGFloat)radius
			       startAngle:(CGFloat)startAngle
				 endAngle:(CGFloat)endAngle
				clockwise:(BOOL)clockwise;
// As above with clockwise == NO. */	
- (void)appendBezierPathWithArcWithCenter:(NSPoint)center radius:(CGFloat)radius
			       startAngle:(CGFloat)startAngle
				 endAngle:(CGFloat)endAngle;
- (void)appendBezierPathWithArcFromPoint:(NSPoint)point1
				 toPoint:(NSPoint)point2
				  radius:(CGFloat)radius;

- (void)appendBezierPathWithCGGlyph:(CGGlyph)glyph inFont:(NSFont *)font API_AVAILABLE(macos(10.13));
- (void)appendBezierPathWithCGGlyphs:(const CGGlyph *)glyphs count:(NSInteger)count inFont:(NSFont *)font API_AVAILABLE(macos(10.13));

// Appends paths for a rounded rectangle.
- (void)appendBezierPathWithRoundedRect:(NSRect)rect xRadius:(CGFloat)xRadius yRadius:(CGFloat)yRadius API_AVAILABLE(macos(10.5));

// Hit detection.
- (BOOL)containsPoint:(NSPoint)point;
@end

@interface NSBezierPath (NSBezierPathDeprecated)
- (BOOL)cachesBezierPath API_DEPRECATED("", macos(10.0,10.0));
- (void)setCachesBezierPath:(BOOL)flag API_DEPRECATED("", macos(10.0,10.0));

// The following NSGlyph-related methods are deprecated. Use corresponding CGGlyph-based methods instead
- (void)appendBezierPathWithGlyph:(NSGlyph)glyph inFont:(NSFont *)font API_DEPRECATED("Use -appendBezierPathWithCGGlyph:inFont: instead", macos(10.0,10.14));
- (void)appendBezierPathWithGlyphs:(NSGlyph *)glyphs count:(NSInteger)count inFont:(NSFont *)font API_DEPRECATED("Use -appendBezierPathWithCGGlyphs:count:inFont: instead", macos(10.0,10.14));
- (void)appendBezierPathWithPackedGlyphs:(const char *)packedGlyphs API_DEPRECATED("Use -appendBezierPathWithCGGlyphs:count:inFont: instead", macos(10.0,10.14));
@end

/* Deprecated legacy line cap style constants. Prefer to use NSLineCapStyle values instead.
*/
static const NSLineCapStyle NSButtLineCapStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineCapStyleButt", macos(10.0, 11.0)) = NSLineCapStyleButt;
static const NSLineCapStyle NSRoundLineCapStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineCapStyleRound", macos(10.0, 11.0)) = NSLineCapStyleRound;
static const NSLineCapStyle NSSquareLineCapStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineCapStyleSquare", macos(10.0, 11.0)) = NSLineCapStyleSquare;

/* Deprecated legacy line join style constants. Prefer to use NSLineJoinStyle values instead.
*/
static const NSLineJoinStyle NSMiterLineJoinStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineJoinStyleMiter", macos(10.0, 11.0)) = NSLineJoinStyleMiter;
static const NSLineJoinStyle NSRoundLineJoinStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineJoinStyleRound", macos(10.0, 11.0)) = NSLineJoinStyleRound;
static const NSLineJoinStyle NSBevelLineJoinStyle API_DEPRECATED_WITH_REPLACEMENT("NSLineJoinStyleBevel", macos(10.0, 11.0)) = NSLineJoinStyleBevel;

/* Deprecated legacy winding rule constants. Prefer to use NSWindingRule values instead.
*/
static const NSWindingRule NSNonZeroWindingRule API_DEPRECATED_WITH_REPLACEMENT("NSWindingRuleNonZero", macos(10.0, 11.0)) = NSWindingRuleNonZero;
static const NSWindingRule NSEvenOddWindingRule API_DEPRECATED_WITH_REPLACEMENT("NSWindingRuleEvenOdd", macos(10.0, 11.0)) = NSWindingRuleEvenOdd;

/* Deprecated legacy Bezier path element constants. Prefer to use NSBezierPathElement values instead.
*/
static const NSBezierPathElement NSMoveToBezierPathElement API_DEPRECATED_WITH_REPLACEMENT("NSBezierPathElementMoveTo", macos(10.0, 11.0)) = NSBezierPathElementMoveTo;
static const NSBezierPathElement NSLineToBezierPathElement API_DEPRECATED_WITH_REPLACEMENT("NSBezierPathElementLineTo", macos(10.0, 11.0)) = NSBezierPathElementLineTo;
static const NSBezierPathElement NSCurveToBezierPathElement API_DEPRECATED_WITH_REPLACEMENT("NSBezierPathElementCurveTo", macos(10.0, 11.0)) = NSBezierPathElementCurveTo;
static const NSBezierPathElement NSClosePathBezierPathElement API_DEPRECATED_WITH_REPLACEMENT("NSBezierPathElementClosePath", macos(10.0, 11.0)) = NSBezierPathElementClosePath;

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
