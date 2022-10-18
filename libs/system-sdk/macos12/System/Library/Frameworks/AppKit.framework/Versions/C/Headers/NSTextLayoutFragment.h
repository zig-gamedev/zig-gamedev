#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextLayoutFragment.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextLayoutFragment.h
//  Text Kit
//
//  Copyright (c) 2017-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSArray.h>
#import <CoreGraphics/CoreGraphics.h>

@class NSTextLayoutManager;
@class NSTextElement;
@class NSTextRange;
@class NSTextParagraph;
@class NSTextLineFragment;
@class NSOperationQueue;
@class NSTextAttachmentViewProvider;
@protocol NSTextLocation;

NS_ASSUME_NONNULL_BEGIN

// NSTextLayoutFragment represents the layout fragment typically corresponding to a rendering surface such as CALayer or UIView/NSView subclasses.

typedef NS_OPTIONS(NSUInteger, NSTextLayoutFragmentEnumerationOptions) {
  NSTextLayoutFragmentEnumerationOptionsNone = 0,
  NSTextLayoutFragmentEnumerationOptionsReverse = (1 << 0),
  NSTextLayoutFragmentEnumerationOptionsEstimatesSize = (1 << 1), // When enumerating, the layout fragments will be asked to estimate their size.
  NSTextLayoutFragmentEnumerationOptionsEnsuresLayout = (1 << 2), // When enumerating, the layout fragments will be asked to layout.
  NSTextLayoutFragmentEnumerationOptionsEnsuresExtraLineFragment = (1 << 3), // It synthesizes the extra line fragment when necessary.
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSUInteger, NSTextLayoutFragmentState) {
  NSTextLayoutFragmentStateNone = 0, // No layout information
  NSTextLayoutFragmentStateEstimatedUsageBounds = 1, // When associated with NSTextLayoutManager, filled with an estimated rect if no layout
  NSTextLayoutFragmentStateCalculatedUsageBounds = 2, // layout fragment measurements available without textLineFragments
  NSTextLayoutFragmentStateLayoutAvailable = 3 // textLineFragments and layout fragment measurements available
} API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos);

API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextLayoutFragment : NSObject <NSSecureCoding>
#pragma mark Initialization
- (instancetype)initWithTextElement:(NSTextElement *)textElement range:(nullable NSTextRange *)rangeInElement NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (instancetype)init NS_UNAVAILABLE;

#pragma mark Properties
@property (nullable, weak, readonly) NSTextLayoutManager *textLayoutManager;

#pragma mark Contents
// The parent text element
@property (weak, readonly) NSTextElement *textElement;

// range inside textElement relative to the document origin
@property (strong, readonly) NSTextRange *rangeInElement;

#pragma mark Layout
// An array of NSTextLineFragments. Valid when NSTextLayoutFragmentStateLayoutAvailable. KVO-compliant
@property (copy, readonly) NSArray<NSTextLineFragment *> *textLineFragments;

// When non-nil, the layout operation is dispatched to the queue asynchronously.
@property (nullable, strong) NSOperationQueue *layoutQueue;

// The layout information state. KVO-compliant
@property (readonly) NSTextLayoutFragmentState state;

// Invalidates any layout information associated with the receiver
- (void)invalidateLayout;

#pragma mark Layout information
// The rect for tiling the layout fragment inside the target layout coordinate system typically in an NSTextContainer.
@property (readonly) CGRect layoutFragmentFrame;

// The bounds defining the area required for rendering the contents. The coordinate system is vertically flipped from the layoutFragmentFrame origin ({0,0} is at the upper left corner). The size should be larger than layoutFragmentFrame.size. The origin could be in the negative coordinate since the rendering could be stretched out of layoutFragmentFrame. Only valid when state > NSTextLayoutFragmentStateEstimatedUsageBounds.
@property (readonly) CGRect renderingSurfaceBounds;

#pragma mark Custom spacing

// The amount of margin space reserved during paragraph layout between the leading edge of the text layout fragment (according to the primary writing direction of the paragraph) and the start of the lines in the paragraph.
@property (readonly) CGFloat leadingPadding;

// The amount of margin space reserved during paragraph layout between the end of the lines in the paragraph and the trailing edge of the text layout fragment (according to the primary writing direction of the paragraph).
@property (readonly) CGFloat trailingPadding;

// The amount of space reserved during paragraph layout between the top of the text layout fragment and the top of the first line in the paragraph.
@property (readonly) CGFloat topMargin;

// The amount of space reserved during paragraph layout between the bottom of the last line in the paragraph and the bottom of the text layout fragment.
@property (readonly) CGFloat bottomMargin;

#pragma mark Rendering
// Renders the visual representation of this element in the specified graphics context.
- (void)drawAtPoint:(CGPoint)point inContext:(CGContextRef)context;

#pragma mark Text attachments
// Returns NSTextAttachmentViewProvider associated with the receiver. The property contents are only valid with NSTextLayoutFragmentStateLayoutAvailable.
@property (copy, readonly) NSArray<NSTextAttachmentViewProvider *> *textAttachmentViewProviders;

// Returns the frame in the text layout fragment coordinate system for the attachment at location. It returns CGRectZero if location is not with any attachment or the state is not NSTextLayoutFragmentStateLayoutAvailable.
- (CGRect)frameForTextAttachmentAtLocation:(id <NSTextLocation>)location;
@end
NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextLayoutFragment.h>
#endif
