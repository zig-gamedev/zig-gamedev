#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextLayoutManager.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextLayoutManager.h
//  Text Kit
//
//  Copyright (c) 2017-2021, Apple Inc. All rights reserved.
//


#import <Foundation/NSAttributedString.h>
#import <CoreGraphics/CGGeometry.h>
#import <AppKit/NSTextLayoutFragment.h>

@class NSTextRange;
@class NSTextContentManager;
@class NSTextElement;
@class NSTextContainer;
@class NSTextSelection;
@class NSTextSelectionNavigation;
@class NSTextViewportLayoutController;
@protocol NSTextLocation;
@protocol NSTextLayoutManagerDelegate;
@protocol NSTextSelectionDataSource;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSTextLayoutManagerSegmentType) {
    NSTextLayoutManagerSegmentTypeStandard = 0, // The standard segment matching the typographic bounds of range.
    NSTextLayoutManagerSegmentTypeSelection = 1, // The segment behavior suitable for the selection rendering. Extends the last segment in a line fragment to the trailing edge if continuing to the next line.
    NSTextLayoutManagerSegmentTypeHighlight = 2, // The segment behavior suitable for highlighting.
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_OPTIONS(NSUInteger, NSTextLayoutManagerSegmentOptions) {
    NSTextLayoutManagerSegmentOptionsNone = 0,
    NSTextLayoutManagerSegmentOptionsRangeNotRequired = (1 << 0), // Does not allocate and pass to the enumerator
    NSTextLayoutManagerSegmentOptionsMiddleFragmentsExcluded = (1 << 1), // Only enumerates segments in the first and last line fragments.
    NSTextLayoutManagerSegmentOptionsHeadSegmentExtended = (1 << 2), // When specified, the segment is extended to the head edge
    NSTextLayoutManagerSegmentOptionsTailSegmentExtended = (1 << 3), // When specified, the segment is extended to the tail edge.
    NSTextLayoutManagerSegmentOptionsUpstreamAffinity = (1 << 4), // When specified, the segment is placed based on the upstream affinity for an empty range.
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

// NSTextLayoutManager is the center piece of the TextKit object network maintaining the layout geometry via an array of NSTextContainer and layout results with NSTextLayoutFragment associated with NSTextElement vended from the owner NSTextContentManager.
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextLayoutManager : NSObject <NSSecureCoding, NSTextSelectionDataSource>
#pragma mark Initialization
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

#pragma mark Basic properties
@property (nullable, weak) id <NSTextLayoutManagerDelegate> delegate;

// If YES, uses the leading as specified by the font. However, this is not appropriate for most UI text. YES by default.
@property BOOL usesFontLeading;

// When YES, enables internal security analysis for malicious inputs and activates defensive behaviors. By enabling this functionality, it's possible certain text such as a very long paragraph might result in unexpected layout. NO by default.
@property BOOL limitsLayoutForSuspiciousContents;

// When YES, NSTextLayoutManager will attempt to hyphenate when wrapping lines. May be overridden on a per-paragraph basis by the NSParagraphStyle's usesDefaultHyphenation. The receiver makes the best effort to decide the exact logic including the hyphenation factor based on the context. The default value is NO. Can be overridden by the preference key @"NSUsesDefaultHyphenation".
@property BOOL usesHyphenation;

#pragma mark NSTextContentManager interface
// Pointing to the NSTextContentManager object strongly referencing the text layout manager.
@property (nullable, weak, readonly) NSTextContentManager *textContentManager;

// This method should be used in order to replace an NSTextContentManager with a new one, leaving all related objects intact.  This method makes sure the NSTextLayoutManager doesn't get deallocated while migrating to the new manager.
- (void)replaceTextContentManager:(NSTextContentManager *)textContentManager;

#pragma mark NSTextContainer interface
// The text container object providing geometric information for the layout destination. If isSimpleRectangularTextContainer=NO, NSTextLayoutManager always fill from the top instead of allowing non-contiguous layout support.
@property (nullable, strong, readwrite) NSTextContainer *textContainer;

// The usage bounds for textContainer. KVO-compliant. Views can observe this property in order to trigger a resize operation. For example, UIView/NSView should call setNeedsUpdateConstraints() when the usage bounds changes.
@property (readonly) CGRect usageBoundsForTextContainer;

#pragma mark Layout
// Returns NSTextViewportLayoutController associated with textContainer.
@property (readonly, strong) NSTextViewportLayoutController *textViewportLayoutController;

// If non-nil, it performs layout in the specified queue until estimatedUsageBounds=NO.
@property (nullable, strong) NSOperationQueue *layoutQueue;

// Performs the layout for textRange.
- (void)ensureLayoutForRange:(NSTextRange *)range;

// Performs the layout for filling bounds inside the last text container.
- (void)ensureLayoutForBounds:(CGRect)bounds;

// Invalidates layout information for textRange
- (void)invalidateLayoutForRange:(NSTextRange *)range;

// Returns the text layout fragment containing position inside the coordinate system for textContainer.
- (nullable NSTextLayoutFragment *)textLayoutFragmentForPosition:(CGPoint)position;

// Returns the text layout fragment containing the document location, location.
- (nullable NSTextLayoutFragment *)textLayoutFragmentForLocation:(id <NSTextLocation>)location;

// Enumerates the text layout fragments starting at textLocation. If textLocation=nil, it assumes self.textContentManager.documentRange.location; uses self.documentRange.endLocation for reverse enumeration. When enumerating backward, it starts with the fragment preceding the one containing textLocation.  If enumerated at least one fragment, it returns the edge of the enumerated range. Note that the enumerated range might not match the range of the last element returned. It enumerates the elements in the sequence, but it can skip a range (it can limit the maximum number of text elements enumerated for a single invocation or hide some elements from the layout). Returning NO from block breaks out of the enumeration.
- (nullable id <NSTextLocation>)enumerateTextLayoutFragmentsFromLocation:(nullable id <NSTextLocation>)location options:(NSTextLayoutFragmentEnumerationOptions)options usingBlock:(BOOL (NS_NOESCAPE ^)(NSTextLayoutFragment *layoutFragment))block;

#pragma mark Text Selection
// An array of NSTextSelections associated by the text layout manager. Each NSTextSelection represents an insertion point. The selection state is shared among all view ports connected to the text layout manager via text containers.
@property (strong) NSArray<NSTextSelection *> *textSelections;

// Returns an NSTextSelectionNavigation configured to have the text layout manager as NSTextSelectionDataSource
@property (strong) NSTextSelectionNavigation *textSelectionNavigation;

#pragma mark Rendering Attributes
// Rendering attributes overrides the document text attributes stored in NSTextParagraphs supplied by NSTextContentManager. NSTextLayoutFragment associated with a text paragraph applies the overriding attributes before executing layout. Specifying NSNull to an attribute indicates removing it from the document attributes. Rendering attributes are invalidated upon re-layout of the text layout fragment that they belong to.

// Enumerates the rendering attributes from location. It enumerates only ranges with rendering attributes specified. Returning NO from block breaks out of the enumeration.
- (void)enumerateRenderingAttributesFromLocation:(id <NSTextLocation> )location reverse:(BOOL)reverse usingBlock:(BOOL (NS_NOESCAPE ^)(NSTextLayoutManager *textLayoutManager, NSDictionary<NSAttributedStringKey, id> *attributes, NSTextRange *textRange))block;

// Sets the rendering attributes.
- (void)setRenderingAttributes:(NSDictionary<NSAttributedStringKey, id> *)renderingAttributes forTextRange:(NSTextRange *)textRange;

// Sets the rendering attribute. Passing nil value indicates overriding the specified attribute by removing from the final attributes passed to the layout and rendering engine. This is a convenience over -setRenderingAttributes:forTextRange:.
- (void)addRenderingAttribute:(NSAttributedStringKey)renderingAttribute value:(nullable id)value forTextRange:(NSTextRange *)textRange;

// Removes attribute from textRange.  This is a convenience over -setRenderingAttributes:forTextRange:.
- (void)removeRenderingAttribute:(NSAttributedStringKey)renderingAttribute forTextRange:(NSTextRange *)textRange;

// Invalidates the rendering attributes in textRange. Enumerating rendering attributes will skip the invalidated range.
- (void)invalidateRenderingAttributesForTextRange:(NSTextRange *)textRange;

// A callback block invoked whenever NSTextLayoutManager needs to validate the rendering attributes for the range. The validator should use -setRenderingAttributes:forTextRange: to fill the rendering attributes appropriate for the range inside textLayoutFragment.
@property (nullable, copy) void (^renderingAttributesValidator)(NSTextLayoutManager *textLayoutManager, NSTextLayoutFragment *textLayoutFragment);

#pragma mark Link Attributes
// The default set of attributes for rendering NSLinkAttributeName. The base NSTextLayoutManager class returns with NSUnderlineStyleSingle for NSUnderlineStyleAttributeName and the platform link color for NSForegroundColorAttributeName. The platform color for macOS is +linkColor. Other platforms uses +blueColor.
@property (class, readonly) NSDictionary<NSAttributedStringKey, id> *linkRenderingAttributes;

// Returns a dictionary of rendering attributes for rendering NSLinkAttributeName. Just as other rendering attributes, specifying NSNull removes the attribute from the final attributes used for rendering. It has priority over the general rendering attributes.
- (NSDictionary<NSAttributedStringKey, id> *)renderingAttributesForLink:(id)link atLocation:(id <NSTextLocation>)location;

#pragma mark Text Segments
// Enumerates text segments of type in textRange. A text segment is both logically and visually contiguous portion of the text content inside a line fragment. It can be defined with a single text range. The segments are enumerated visually from left to right. Returning NO from block breaks out of the enumeration.
- (void)enumerateTextSegmentsInRange:(NSTextRange *)textRange type:(NSTextLayoutManagerSegmentType)type options:(NSTextLayoutManagerSegmentOptions)options usingBlock:(BOOL (NS_NOESCAPE ^)(NSTextRange * _Nullable textSegmentRange, CGRect textSegmentFrame, CGFloat baselinePosition, NSTextContainer *textContainer))block;

#pragma mark Editing
- (void)replaceContentsInRange:(NSTextRange *)range withTextElements:(NSArray<NSTextElement *> *)textElements;
- (void)replaceContentsInRange:(NSTextRange *)range withAttributedString:(NSAttributedString *)attributedString;
@end

#pragma mark NSTextLayoutManagerDelegate
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextLayoutManagerDelegate <NSObject>
@optional
// NSTextLayoutFragment management
// Returns NSTextLayoutFragment for location in textElement. NSTextLayoutManagerDelegate could provide an NSTextLayoutFragment specialized for an NSTextElement subclass targeted for the rendering surface.
- (NSTextLayoutFragment *)textLayoutManager:(NSTextLayoutManager *)textLayoutManager textLayoutFragmentForLocation:(id <NSTextLocation>)location inTextElement:(NSTextElement *)textElement;

// Layout behavior
// Invoked while determining the soft line break point.  When hyphenating=NO, NSTextLayoutManager tries to find the next line break opportunity before location. When hyphenating=YES, it is an auto-hyphenation point.
- (BOOL)textLayoutManager:(NSTextLayoutManager *)textLayoutManager shouldBreakLineBeforeLocation:(id <NSTextLocation>)location hyphenating:(BOOL)hyphenating;

// Rendering attributes
// Returns a dictionary of rendering attributes for rendering NSLinkAttributeName. Just as other rendering attributes, specifying NSNull removes the attribute from the final attributes used for rendering. It has priority over the general rendering attributes.
- (nullable NSDictionary<NSAttributedStringKey, id> *)textLayoutManager:(NSTextLayoutManager *)textLayoutManager renderingAttributesForLink:(id)link atLocation:(id <NSTextLocation>)location defaultAttributes:(NSDictionary<NSAttributedStringKey, id> *)renderingAttributes;

@end

NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextLayoutManager.h>
#endif
