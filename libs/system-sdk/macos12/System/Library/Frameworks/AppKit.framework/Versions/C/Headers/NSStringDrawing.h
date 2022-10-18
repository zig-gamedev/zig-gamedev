#if !__has_include(<UIFoundation/NSStringDrawing.h>)
/*
        NSStringDrawing.h
        Copyright (c) 1994-2021, Apple Inc.  All rights reserved.
*/

#import <Foundation/NSString.h>
#import <AppKit/NSAttributedString.h>

@class NSStringDrawingContext;

NS_ASSUME_NONNULL_BEGIN
#if !TARGET_OS_IPHONE

// When attributes=nil, the methods declared here uses the default behavior for each attribute described in <AppKit/NSAttributedString.h>. When stringDrawingContext=nil, it's equivalent of passing the default instance initialized with [[NSStringDrawingContext alloc] init].

API_AVAILABLE(macos(10.11), ios(6.0), watchos(2.0), tvos(9.0)) @interface NSStringDrawingContext : NSObject

// Minimum scale factor for drawWithRect:options:context: and boundingRectWithSize:options:context: methods. If this property is set, the extended string drawing methods will attempt to draw the attributed string in the given bounds by proportionally scaling the font(s) in the attributed string
@property CGFloat minimumScaleFactor;

// actual scale factor used by the last drawing call where minimum scale factor was specified
@property (readonly) CGFloat actualScaleFactor;

// bounds of the string drawn by the previous invocation of drawWithRect:options:context:
@property (readonly) NSRect totalBounds;

@end

@interface NSString(NSStringDrawing)
- (NSSize)sizeWithAttributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0));
- (void)drawAtPoint:(NSPoint)point withAttributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0));
- (void)drawInRect:(NSRect)rect withAttributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attrs API_AVAILABLE(macos(10.0), ios(7.0), watchos(2.0), tvos(9.0));
@end

@interface NSAttributedString(NSStringDrawing)
- (NSSize)size API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));
- (void)drawAtPoint:(NSPoint)point API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));
- (void)drawInRect:(NSRect)rect API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));
@end

typedef NS_OPTIONS(NSInteger, NSStringDrawingOptions) {
    NSStringDrawingUsesLineFragmentOrigin = 1 << 0, // The specified origin is the line fragment origin, not the base line origin
    NSStringDrawingUsesFontLeading = 1 << 1, // Uses the font leading for calculating line heights
    NSStringDrawingUsesDeviceMetrics = 1 << 3, // Uses image glyph bounds instead of typographic bounds
    NSStringDrawingTruncatesLastVisibleLine API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = 1 << 5, // Truncates and adds the ellipsis character to the last visible line if the text doesn't fit into the bounds specified. Ignored if NSStringDrawingUsesLineFragmentOrigin is not also set.

    NSStringDrawingDisableScreenFontSubstitution API_DEPRECATED("", macos(10.0,10.11)) = (1 << 2),
    NSStringDrawingOneShot API_DEPRECATED("", macos(10.0,10.11)) = (1 << 4)
} API_AVAILABLE(macos(10.0), ios(6.0), watchos(2.0), tvos(9.0));


// NOTE: All of the following methods will default to drawing on a baseline, limiting drawing to a single line.
// To correctly draw and size multi-line text, pass NSStringDrawingUsesLineFragmentOrigin in the options parameter.
@interface NSString (NSExtendedStringDrawing)
- (void)drawWithRect:(NSRect)rect options:(NSStringDrawingOptions)options attributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attributes context:(nullable NSStringDrawingContext *)context API_AVAILABLE(macos(10.11), ios(7.0), watchos(2.0), tvos(9.0));
- (NSRect)boundingRectWithSize:(NSSize)size options:(NSStringDrawingOptions)options attributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attributes context:(nullable NSStringDrawingContext *)context API_AVAILABLE(macos(10.11), ios(7.0), watchos(2.0), tvos(9.0));
@end

@interface NSAttributedString (NSExtendedStringDrawing)
- (void)drawWithRect:(NSRect)rect options:(NSStringDrawingOptions)options context:(nullable NSStringDrawingContext *)context API_AVAILABLE(macos(10.11), ios(6.0), watchos(2.0), tvos(9.0));
- (NSRect)boundingRectWithSize:(NSSize)size options:(NSStringDrawingOptions)options context:(nullable NSStringDrawingContext *)context API_AVAILABLE(macos(10.11), ios(6.0), watchos(2.0), tvos(9.0));
@end

/************************ Deprecated ************************/
// Following NSStringDrawing methods are soft deprecated starting with OS X 10.11. It will be officially deprecated in a future release. Use corresponding API with NSStringDrawingContext instead
@interface NSString (NSStringDrawingDeprecated)
- (void)drawWithRect:(NSRect)rect options:(NSStringDrawingOptions)options attributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attributes; // Use -drawWithRect:options:attributes:context: instead

- (NSRect)boundingRectWithSize:(NSSize)size options:(NSStringDrawingOptions)options attributes:(nullable NSDictionary<NSAttributedStringKey, id> *)attributes; // Use -boundingRectWithSize:options:attributes:context: instead
@end

@interface NSAttributedString (NSStringDrawingDeprecated)
- (void)drawWithRect:(NSRect)rect options:(NSStringDrawingOptions)options; // Use -drawWithRect:options:context: instead
- (NSRect)boundingRectWithSize:(NSSize)size options:(NSStringDrawingOptions)options; // Use -boundingRectWithSize:options:context: instead
@end

#endif // !TARGET_OS_IPHONE
NS_ASSUME_NONNULL_END
#else
#import <UIFoundation/NSStringDrawing.h>
#endif

