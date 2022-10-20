#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextSelectionNavigation.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextSelectionNavigation.h
//  Text Kit
//
//  Copyright (c) 2019-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSObject.h>
#import <CoreGraphics/CGGeometry.h>
#import <AppKit/NSTextSelection.h>

NS_ASSUME_NONNULL_BEGIN

@class NSTextSelection;
@class NSTextRange;
@class NSTextLineFragment;
@protocol NSTextLocation;
@protocol NSTextSelectionDataSource;

// NSTextSelectionNavigation is an interface exposing methods for obtaining results from actions performed on text selections. It returns the essential information necessary for editing, selecting, and navigating operations.

typedef NS_ENUM(NSInteger, NSTextSelectionNavigationDirection) {
    // Logical directions
    // Works with just character information
    NSTextSelectionNavigationDirectionForward,
    NSTextSelectionNavigationDirectionBackward,

    // Visual directions
    // Requires layout information if the direction is along the line (i.e. right and left for a horizontal line). Otherwise, it gets resolved to the corresponding logical direction except for NSTextSelectionNavigationDestinationCharacter
    NSTextSelectionNavigationDirectionRight,
    NSTextSelectionNavigationDirectionLeft,
    NSTextSelectionNavigationDirectionUp,
    NSTextSelectionNavigationDirectionDown
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSInteger, NSTextSelectionNavigationDestination) {
    NSTextSelectionNavigationDestinationCharacter, // Moving to the next extended grapheme cluster boundary. Could result in a location inside a cluster depending on operations. When the movement direction is not along the line (i.e. up and down for a horizontal line), it moves to the adjacent line using the anchor point instead of resolving to the logical direction
    NSTextSelectionNavigationDestinationWord, // Moving to the next word boundary. Ignores punctuations, whitespaces, and format characters preceding the next word
    NSTextSelectionNavigationDestinationLine, // Moving to the next line boundary. The boundary can be logical based on the line separator characters as well as visual with soft line wrapping
    NSTextSelectionNavigationDestinationSentence, // Moving to the next sentence boundary. Ignores punctuations, whitespaces, and format characters preceding the next sentence
    NSTextSelectionNavigationDestinationParagraph, // Moving to the next paragraph boundary. Ignores the end of line elastic characters and paragraph separators
    NSTextSelectionNavigationDestinationContainer, // Moving to the next container/page boundary. Ignores the end of line elastic characters and container/page separators
    NSTextSelectionNavigationDestinationDocument, // Moving to the document boundary
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_OPTIONS(NSUInteger, NSTextSelectionNavigationModifier) {
    NSTextSelectionNavigationModifierExtend = (1 << 0), // Extends the selection by not moving the initial location while drag selection
    NSTextSelectionNavigationModifierVisual = (1 << 1), // Extends the selection visually inside the rectangular area defined by the anchor and dragged positions
    NSTextSelectionNavigationModifierMultiple = (1 << 2), // Extends the selection visually inside the rectangular area defined by the anchor and dragged positions and produces an NSTextSelection per line
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSInteger, NSTextSelectionNavigationWritingDirection) {
    NSTextSelectionNavigationWritingDirectionLeftToRight   = 0,    // Left to right writing direction
    NSTextSelectionNavigationWritingDirectionRightToLeft   = 1     // Right to left writing direction
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSInteger, NSTextSelectionNavigationLayoutOrientation) {
    NSTextSelectionNavigationLayoutOrientationHorizontal = 0, // Lines rendered horizontally, grow top to bottom
    NSTextSelectionNavigationLayoutOrientationVertical = 1, // Lines rendered vertically, grow right to left
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextSelectionNavigation : NSObject
#pragma mark Initialization
- (instancetype)initWithDataSource:(id <NSTextSelectionDataSource>)dataSource NS_DESIGNATED_INITIALIZER;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

#pragma mark Properties
// The data source object providing the layout and document content information.
@property (readonly, weak) id <NSTextSelectionDataSource> textSelectionDataSource;

// If YES, the object could produce selections with multiple disjoint ranges.
@property BOOL allowsNonContiguousRanges;

// If YES, rotates the coordinate system for arguments passed to the navigation methods such as -textSelectionsInteractingAtPoint:inContainerAtLocation:anchors:modifiers:selecting:bounds: based on the text container layout orientation. NO by default.
@property BOOL rotatesCoordinateSystemForLayoutOrientation;

#pragma mark Layout Information Cache
// Flushes cached layout information. Should be called whenever the document contents get changed.
- (void)flushLayoutCache;

#pragma mark Selection & Cursor Navigation
// Returns a text selection moved from textSelection by the navigating operation presented by direction, destination, and extending. Returns nil when the requested operation does not produce any logically valid result. If 'confined' is YES, confine any movement to the text element that the selection already lies within.
- (nullable NSTextSelection *)destinationSelectionForTextSelection:(NSTextSelection *)textSelection direction:(NSTextSelectionNavigationDirection)direction destination:(NSTextSelectionNavigationDestination)destination extending:(BOOL)extending confined:(BOOL)confined;

// Returns an array of text selections produced with tap/mouse down at point. anchors defines the last text selection state if selecting=YES or NSTextSelectionNavigationModifierExtend. selecting=YES indicates it's in a text selection dragging session. bounds defines the view area in the container coordinate system that can interact with events. It is typically the view's visible area.
- (NSArray<NSTextSelection *> *)textSelectionsInteractingAtPoint:(CGPoint)point inContainerAtLocation:(id <NSTextLocation>)containerLocation anchors:(NSArray <NSTextSelection *> *)anchors modifiers:(NSTextSelectionNavigationModifier)modifiers selecting:(BOOL)selecting bounds:(CGRect)bounds;

// Returns a text selection expanded to the nearest boundaries for selectionGranularity enclosing textSelection.textRanges. The returned selection will have granularity=selectionGranularity.
- (NSTextSelection *)textSelectionForSelectionGranularity:(NSTextSelectionGranularity)selectionGranularity enclosingTextSelection:(NSTextSelection *)textSelection;

// Returns a text selection expanded to the nearest boundaries for selectionGranularity that contains point in the coordinate system of the text container at containerLocation. The returned selection will have granularity=selectionGranularity.
- (nullable NSTextSelection *)textSelectionForSelectionGranularity:(NSTextSelectionGranularity)selectionGranularity enclosingPoint:(CGPoint)point inContainerAtLocation:(id <NSTextLocation>)location;

#pragma mark Editing
// Returns the resolved location for inserting the next input if textSelection is isLogical=NO && secondarySelectionLocation!=nil. If not, returns nil.
- (nullable id <NSTextLocation>)resolvedInsertionLocationForTextSelection:(NSTextSelection *)textSelection writingDirection:(NSTextSelectionNavigationWritingDirection)writingDirection;

// Returns the ranges that should be deleted based on textSelection and movement arguments. The selection after deletion should contain a 0-length range starting at the location of the first range returned. destination is ignored when textSelection has a non-empty selection. allowsDecomposition only applies to NSTextSelectionNavigationDirectionBackward and NSTextSelectionNavigationDestinationCharacter with 0-length selection.
- (NSArray<NSTextRange *> *)deletionRangesForTextSelection:(NSTextSelection *)textSelection direction:(NSTextSelectionNavigationDirection)direction destination:(NSTextSelectionNavigationDestination)destination allowsDecomposition:(BOOL)allowsDecomposition;
@end

API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextSelectionDataSource <NSObject>
@required
// Declares the starting and ending locations for the document.
@property (strong, readonly) NSTextRange *documentRange;

// Enumerating the textual segment boundaries starting at location. The segmentation logic should be based on the natural language semantics and rules such as Unicode Text Segmentation UAX 29. It is allowed to have custom text segmentation behavior for specialized document contents (i.e. text editors with structured document contents such as a programming language). NSStringEnumerationByLines should be aware of the visual line boundaries based on soft line wrapping.
- (void)enumerateSubstringsFromLocation:(id <NSTextLocation>)location options:(NSStringEnumerationOptions)options usingBlock:(void (NS_NOESCAPE ^)(NSString * _Nullable substring, NSTextRange *substringRange, NSTextRange * _Nullable enclosingRange, BOOL *stop))block;

// Returns a text range corresponding to selectionGranularity enclosing location. Returns nil when documentRange.isEmpty=YES.
- (nullable NSTextRange *)textRangeForSelectionGranularity:(NSTextSelectionGranularity)selectionGranularity enclosingLocation:(id <NSTextLocation>)location;

// Returns a new location from location with offset. The offset value could be positive or negative indicating the logical direction. Could return nil when the inputs don't produce any legal location (i.e. out of bounds index).
- (nullable id <NSTextLocation>)locationFromLocation:(id <NSTextLocation>)location withOffset:(NSInteger)offset;

// Returns the offset between from and to. The return value could be positive or negative. Could return NSNotFound when the offset cannot be represented in an integer value (i.e. locations are not in the same document).
- (NSInteger)offsetFromLocation:(id <NSTextLocation>)from toLocation:(id <NSTextLocation>)to;

// Returns the base writing direction at location. It is recommended to return the resolved base direction either NSTextSelectionNavigationWritingDirectionLeftToRight or NSTextSelectionNavigationWritingDirectionRightToLeft.
- (NSTextSelectionNavigationWritingDirection)baseWritingDirectionAtLocation:(id <NSTextLocation>)location;

// Layout
// Enumerates all the caret offset from left to right in visual order. caretOffset should be in the coordinate system for the text container. When leadingEdge=YES, it indicates caretOffset is at the edge logically preceding the character. For LTR characters, it is on the left, and on the right for RTL characters.
- (void)enumerateCaretOffsetsInLineFragmentAtLocation:(id<NSTextLocation>)location usingBlock:(void (NS_NOESCAPE ^)(CGFloat caretOffset, id <NSTextLocation> location, BOOL leadingEdge, BOOL * stop))block;

// Returns the range of the line fragment containing point inside the coordinate system containing location.
- (nullable NSTextRange *)lineFragmentRangeForPoint:(CGPoint)point inContainerAtLocation:(id <NSTextLocation>)location;

@optional
// This optional method should be implemented enumerating the text container/page boundary when the text selection data provider supports such a layout functionality.
- (void)enumerateContainerBoundariesFromLocation:(id <NSTextLocation>)location reverse:(BOOL)reverse usingBlock:(void (NS_NOESCAPE ^)(id <NSTextLocation> boundaryLocation, BOOL *stop))block;

// Returns the text layout orientation at location. If not implemented, NSTextSelectionNavigation assumes it is NSTextSelectionNavigationLayoutOrientationHorizontal.
- (NSTextSelectionNavigationLayoutOrientation)textLayoutOrientationAtLocation:(id <NSTextLocation>)location;
@end
NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextSelectionNavigation.h>
#endif
