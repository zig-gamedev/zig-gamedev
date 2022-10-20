#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextLineFragment.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextLineFragment.h
//  Text Kit
//
//  Copyright (c) 2017-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSArray.h>
#import <Foundation/NSAttributedString.h>
#import <CoreGraphics/CoreGraphics.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark NSTextLineFragment
// NSTextLineFragment represents a single textual layout and rendering unit inside NSTextLayoutFragment.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextLineFragment : NSObject <NSSecureCoding>
#pragma mark Initialization
- (instancetype)initWithAttributedString:(NSAttributedString *)attributedString range:(NSRange)range NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithString:(NSString *)string attributes:(NSDictionary<NSAttributedStringKey, id> *)attributes range:(NSRange)range;

- (instancetype)init NS_UNAVAILABLE;

#pragma mark Basic properties
// The source attributed string
@property (strong, readonly) NSAttributedString *attributedString;

// The string range for the source attributed string corresponding to this line fragment
@property (readonly) NSRange characterRange;

#pragma mark Typographic bounds
// The typographic bounds specifying the dimensions of the line fragment for laying out line fragments to each other. The origin value is offset from the beginning of the line fragment group belonging to the parent layout fragment.
@property (readonly) CGRect typographicBounds;

#pragma mark Rendering
// Rendering origin for the left most glyph in the line fragment coordinate system
@property (readonly) CGPoint glyphOrigin;

// Renders the line fragment contents at the rendering origin. The origin can be specified as (NSMinX(typographicBounds) + glyphOrigin.x, NSMinY(typographicBounds) + glyphOrigin.y) relative to the line fragment group coordinate system
- (void)drawAtPoint:(CGPoint)point inContext:(CGContextRef)context;

#pragma mark Character and point mappings
// The location of the character at the specified index. It is on the upstream edge of the glyph. It is in the coordinate system relative to the line fragment origin
- (CGPoint)locationForCharacterAtIndex:(NSInteger)index;

// The character index for point inside the line fragment coordinate system. The fraction of distance is from the upstream edge
- (NSInteger)characterIndexForPoint:(CGPoint)point;
- (CGFloat)fractionOfDistanceThroughGlyphForPoint:(CGPoint)point;

@end
NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextLineFragment.h>
#endif
