/*	NSGeometry.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <AvailabilityMacros.h>
#import <Foundation/NSValue.h>
#import <Foundation/NSCoder.h>

#import <CoreGraphics/CGBase.h>
#import <CoreGraphics/CGGeometry.h>

NS_ASSUME_NONNULL_BEGIN

#if __LP64__ || TARGET_OS_IPHONE || NS_BUILD_32_LIKE_64

typedef CGPoint NSPoint;

typedef NSPoint *NSPointPointer;
typedef NSPoint *NSPointArray;

typedef CGSize NSSize;

typedef NSSize *NSSizePointer;
typedef NSSize *NSSizeArray;

typedef CGRect NSRect;

typedef NSRect *NSRectPointer;
typedef NSRect *NSRectArray;

typedef NS_ENUM(NSUInteger, NSRectEdge) {
    NSRectEdgeMinX = CGRectMinXEdge,
    NSRectEdgeMinY = CGRectMinYEdge,
    NSRectEdgeMaxX = CGRectMaxXEdge,
    NSRectEdgeMaxY = CGRectMaxYEdge,
    
    NSMinXEdge NS_SWIFT_UNAVAILABLE("Use NSRectEdge.MinX instead") = NSRectEdgeMinX,
    NSMinYEdge NS_SWIFT_UNAVAILABLE("Use NSRectEdge.MinY instead") = NSRectEdgeMinY,
    NSMaxXEdge NS_SWIFT_UNAVAILABLE("Use NSRectEdge.MaxX instead") = NSRectEdgeMaxX,
    NSMaxYEdge NS_SWIFT_UNAVAILABLE("Use NSRectEdge.MaxY instead") = NSRectEdgeMaxY,
};

#define NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES 1

#else

typedef struct _NSPoint {
    CGFloat x;
    CGFloat y;
} NSPoint;

typedef NSPoint *NSPointPointer;
typedef NSPoint *NSPointArray;

typedef struct _NSSize {
    CGFloat width;		/* should never be negative */
    CGFloat height;		/* should never be negative */
} NSSize;

typedef NSSize *NSSizePointer;
typedef NSSize *NSSizeArray;

typedef struct _NSRect {
    NSPoint origin;
    NSSize size;
} NSRect;

typedef NSRect *NSRectPointer;
typedef NSRect *NSRectArray;

typedef enum {
    NSRectEdgeMinX = 0,
    NSRectEdgeMinY = 1,
    NSRectEdgeMaxX = 2,
    NSRectEdgeMaxY = 3,
    
    NSMinXEdge = NSRectEdgeMinX,
    NSMinYEdge = NSRectEdgeMinY,
    NSMaxXEdge = NSRectEdgeMaxX,
    NSMaxYEdge = NSRectEdgeMaxY,
} NSRectEdge;

#endif

#define NSEDGEINSETS_DEFINED 1
typedef struct NSEdgeInsets {
    CGFloat top;
    CGFloat left;
    CGFloat bottom;
    CGFloat right;
} NSEdgeInsets;

typedef NS_OPTIONS(unsigned long long, NSAlignmentOptions) {
    NSAlignMinXInward   = 1ULL << 0,
    NSAlignMinYInward   = 1ULL << 1,
    NSAlignMaxXInward   = 1ULL << 2,
    NSAlignMaxYInward   = 1ULL << 3,
    NSAlignWidthInward  = 1ULL << 4,
    NSAlignHeightInward = 1ULL << 5,
    
    NSAlignMinXOutward   = 1ULL << 8,
    NSAlignMinYOutward   = 1ULL << 9,
    NSAlignMaxXOutward   = 1ULL << 10,
    NSAlignMaxYOutward   = 1ULL << 11,
    NSAlignWidthOutward  = 1ULL << 12,
    NSAlignHeightOutward = 1ULL << 13,
    
    NSAlignMinXNearest   = 1ULL << 16,
    NSAlignMinYNearest   = 1ULL << 17,
    NSAlignMaxXNearest   = 1ULL << 18,
    NSAlignMaxYNearest   = 1ULL << 19,
    NSAlignWidthNearest  = 1ULL << 20,
    NSAlignHeightNearest = 1ULL << 21,
    
    NSAlignRectFlipped = 1ULL << 63, // pass this if the rect is in a flipped coordinate system. This allows 0.5 to be treated in a visually consistent way.

    // convenience combinations
    NSAlignAllEdgesInward = NSAlignMinXInward|NSAlignMaxXInward|NSAlignMinYInward|NSAlignMaxYInward,
    NSAlignAllEdgesOutward = NSAlignMinXOutward|NSAlignMaxXOutward|NSAlignMinYOutward|NSAlignMaxYOutward,
    NSAlignAllEdgesNearest = NSAlignMinXNearest|NSAlignMaxXNearest|NSAlignMinYNearest|NSAlignMaxYNearest,
};

@class NSString;

FOUNDATION_EXPORT const NSPoint NSZeroPoint;
FOUNDATION_EXPORT const NSSize NSZeroSize;
FOUNDATION_EXPORT const NSRect NSZeroRect;
FOUNDATION_EXPORT const NSEdgeInsets NSEdgeInsetsZero API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

NS_INLINE NSPoint NSMakePoint(CGFloat x, CGFloat y) {
    NSPoint p;
    p.x = x;
    p.y = y;
    return p;
}

NS_INLINE NSSize NSMakeSize(CGFloat w, CGFloat h) {
    NSSize s;
    s.width = w;
    s.height = h;
    return s;
}

NS_INLINE NSRect NSMakeRect(CGFloat x, CGFloat y, CGFloat w, CGFloat h) {
    NSRect r;
    r.origin.x = x;
    r.origin.y = y;
    r.size.width = w;
    r.size.height = h;
    return r;
}

NS_INLINE CGFloat NSMaxX(NSRect aRect) {
    return (aRect.origin.x + aRect.size.width);
}

NS_INLINE CGFloat NSMaxY(NSRect aRect) {
    return (aRect.origin.y + aRect.size.height);
}

NS_INLINE CGFloat NSMidX(NSRect aRect) {
    return (aRect.origin.x + aRect.size.width * (CGFloat)0.5);
}

NS_INLINE CGFloat NSMidY(NSRect aRect) {
    return (aRect.origin.y + aRect.size.height * (CGFloat)0.5);
}

NS_INLINE CGFloat NSMinX(NSRect aRect) {
    return (aRect.origin.x);
}

NS_INLINE CGFloat NSMinY(NSRect aRect) {
    return (aRect.origin.y);
}

NS_INLINE CGFloat NSWidth(NSRect aRect) {
    return (aRect.size.width);
}

NS_INLINE CGFloat NSHeight(NSRect aRect) {
    return (aRect.size.height);
}

NS_INLINE NSRect NSRectFromCGRect(CGRect cgrect) {
    union _ {NSRect ns; CGRect cg;};
    return ((union _ *)&cgrect)->ns;
}

NS_INLINE CGRect NSRectToCGRect(NSRect nsrect) {
    union _ {NSRect ns; CGRect cg;};
    return ((union _ *)&nsrect)->cg;
}

NS_INLINE NSPoint NSPointFromCGPoint(CGPoint cgpoint) {
    union _ {NSPoint ns; CGPoint cg;};
    return ((union _ *)&cgpoint)->ns;
}

NS_INLINE CGPoint NSPointToCGPoint(NSPoint nspoint) {
    union _ {NSPoint ns; CGPoint cg;};
    return ((union _ *)&nspoint)->cg;
}

NS_INLINE NSSize NSSizeFromCGSize(CGSize cgsize) {
    union _ {NSSize ns; CGSize cg;};
    return ((union _ *)&cgsize)->ns;
}

NS_INLINE CGSize NSSizeToCGSize(NSSize nssize) {
    union _ {NSSize ns; CGSize cg;};
    return ((union _ *)&nssize)->cg;
}

NS_INLINE NSEdgeInsets NSEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
    NSEdgeInsets e;
    e.top = top;
    e.left = left;
    e.bottom = bottom;
    e.right = right;
    return e;
}

FOUNDATION_EXPORT BOOL NSEqualPoints(NSPoint aPoint, NSPoint bPoint);
FOUNDATION_EXPORT BOOL NSEqualSizes(NSSize aSize, NSSize bSize);
FOUNDATION_EXPORT BOOL NSEqualRects(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT BOOL NSIsEmptyRect(NSRect aRect);
FOUNDATION_EXPORT BOOL NSEdgeInsetsEqual(NSEdgeInsets aInsets, NSEdgeInsets bInsets) API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSRect NSInsetRect(NSRect aRect, CGFloat dX, CGFloat dY);
FOUNDATION_EXPORT NSRect NSIntegralRect(NSRect aRect);
FOUNDATION_EXPORT NSRect NSIntegralRectWithOptions(NSRect aRect, NSAlignmentOptions opts) API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSRect NSUnionRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT NSRect NSIntersectionRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT NSRect NSOffsetRect(NSRect aRect, CGFloat dX, CGFloat dY);
FOUNDATION_EXPORT void NSDivideRect(NSRect inRect, NSRect *slice, NSRect *rem, CGFloat amount, NSRectEdge edge);
FOUNDATION_EXPORT BOOL NSPointInRect(NSPoint aPoint, NSRect aRect);
FOUNDATION_EXPORT BOOL NSMouseInRect(NSPoint aPoint, NSRect aRect, BOOL flipped);
FOUNDATION_EXPORT BOOL NSContainsRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT BOOL NSIntersectsRect(NSRect aRect, NSRect bRect);

FOUNDATION_EXPORT NSString *NSStringFromPoint(NSPoint aPoint);
FOUNDATION_EXPORT NSString *NSStringFromSize(NSSize aSize);
FOUNDATION_EXPORT NSString *NSStringFromRect(NSRect aRect);
FOUNDATION_EXPORT NSPoint NSPointFromString(NSString *aString);
FOUNDATION_EXPORT NSSize NSSizeFromString(NSString *aString);
FOUNDATION_EXPORT NSRect NSRectFromString(NSString *aString);

@interface NSValue (NSValueGeometryExtensions)

+ (NSValue *)valueWithPoint:(NSPoint)point;
+ (NSValue *)valueWithSize:(NSSize)size;
+ (NSValue *)valueWithRect:(NSRect)rect;
+ (NSValue *)valueWithEdgeInsets:(NSEdgeInsets)insets API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@property (readonly) NSPoint pointValue;
@property (readonly) NSSize sizeValue;
@property (readonly) NSRect rectValue;
@property (readonly) NSEdgeInsets edgeInsetsValue API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@end

@interface NSCoder (NSGeometryCoding)

- (void)encodePoint:(NSPoint)point;
- (NSPoint)decodePoint;

- (void)encodeSize:(NSSize)size;
- (NSSize)decodeSize;

- (void)encodeRect:(NSRect)rect;
- (NSRect)decodeRect;

@end

@interface NSCoder (NSGeometryKeyedCoding)

- (void)encodePoint:(NSPoint)point forKey:(NSString *)key;
- (void)encodeSize:(NSSize)size forKey:(NSString *)key;
- (void)encodeRect:(NSRect)rect forKey:(NSString *)key;

- (NSPoint)decodePointForKey:(NSString *)key;
- (NSSize)decodeSizeForKey:(NSString *)key;
- (NSRect)decodeRectForKey:(NSString *)key;

@end

NS_ASSUME_NONNULL_END
