#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextSelection.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextSelection.h
//  Text Kit
//
//  Copyright (c) 2018-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSAttributedString.h>
#import <CoreGraphics/CGGeometry.h>

@class NSTextRange;
@protocol NSTextLocation;

NS_ASSUME_NONNULL_BEGIN

// NSTextSelection represents a single logical selection context corresponding to an insertion point.
#pragma mark NSTextSelection

typedef NS_ENUM(NSInteger, NSTextSelectionGranularity) {
  NSTextSelectionGranularityCharacter,
  NSTextSelectionGranularityWord,
  NSTextSelectionGranularityParagraph,
  NSTextSelectionGranularityLine,
  NSTextSelectionGranularitySentence
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSInteger, NSTextSelectionAffinity) {
  NSTextSelectionAffinityUpstream = 0,
  NSTextSelectionAffinityDownstream = 1
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextSelection : NSObject <NSSecureCoding>
#pragma mark Initialization
// textRanges should be ordered and not overlapping. Otherwise, textRanges would get normalized by reordered and merging overlapping ranges.
- (instancetype)initWithRanges:(NSArray<NSTextRange *> *)textRanges affinity:(NSTextSelectionAffinity)affinity granularity:(NSTextSelectionGranularity)granularity NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithRange:(NSTextRange *)range affinity:(NSTextSelectionAffinity)affinity granularity:(NSTextSelectionGranularity)granularity;

- (instancetype)initWithLocation:(id <NSTextLocation>)location affinity:(NSTextSelectionAffinity)affinity;

- (instancetype)init NS_UNAVAILABLE;

#pragma mark Basic properties
// Represents an array of disjoint logical ranges in the selection. The array must be logically ordered. When editing, all ranges in a text selection constitute a single insertion point.
@property (copy, readonly) NSArray<NSTextRange *> *textRanges;

// The granularity of the selection. NSTextSelectionGranularityByCharacter by default. Extending operations should modify the selection by the granularity.
@property (readonly) NSTextSelectionGranularity granularity;

// Either upstream or downstream selection. NSTextSelectionAffinityDownstream by default. For a 0-length selection, it describes the visual location of the text cursor between the head of line containing the selection location (downstream) or tail of the previous line (upstream). For a selection with contents, it describes the logical direction of non-anchored edge of the selection.
@property (readonly) NSTextSelectionAffinity affinity;

// Transient text selection during drag handling
@property (readonly, getter=isTransient) BOOL transient;

#pragma mark Navigation properties
// Represents the anchor position offset from the beginning of a line fragment in the visual order for the initial tap or mouse down. That is from the left for a horizontal line fragment and from the top for a vertical. Navigating between lines uses this point when the current line fragment associated with the selection is shorter than the next line visited. 0.0 by default.
@property CGFloat anchorPositionOffset;

#pragma mark Visual selection
// Indicates whether the selection should be interpreted as logical or visual.
@property (getter=isLogical) BOOL logical;

// Specifies the secondary character location when user taps/clicks at a directional boundary. Setting non-nil location has a side effect of making -logical=NO.
@property (strong, nullable) id <NSTextLocation> secondarySelectionLocation;

#pragma mark Typing attributes
// The template attributes used for characters replacing the contents of this selection.
@property (copy) NSDictionary<NSAttributedStringKey, id> *typingAttributes;

#pragma mark Selection manipulation
// Returns a copy of this selection, replacing this instance's textRanges property with textRanges but keeping all other attributes the same.
- (NSTextSelection *)textSelectionWithTextRanges:(NSArray<NSTextRange *> *)textRanges;

@end

NS_ASSUME_NONNULL_END

#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextSelection.h>
#endif
