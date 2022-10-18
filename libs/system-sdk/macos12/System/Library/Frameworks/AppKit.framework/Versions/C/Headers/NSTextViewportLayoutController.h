#if (defined(USE_APPKIT_PUBLIC_HEADERS) && USE_APPKIT_PUBLIC_HEADERS) || !__has_include(<UIFoundation/NSTextViewportLayoutController.h>)
#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
//
//  NSTextViewportLayoutController.h
//  Text Kit
//
//  Copyright (c) 2017-2021, Apple Inc. All rights reserved.
//

#import <Foundation/NSObject.h>
#import <CoreGraphics/CGGeometry.h>

@class NSTextRange;
@class NSTextLayoutManager;
@class NSTextLayoutFragment;
@class NSTextViewportLayoutController;
@protocol NSTextLocation;

NS_ASSUME_NONNULL_BEGIN

// A viewport is a rectangular area within a flipped coordinate system expanding along the y-axis. With text contents, lines advance along the expanding direction typically. It defines the active area where text layout fragments laid out. The area corresponds to the user visible area with additional over-scroll region in most cases. NSTextViewportLayoutController manages the layout process inside the viewport interacting with its delegate.

#pragma mark NSTextViewportLayoutControllerDelegate
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@protocol NSTextViewportLayoutControllerDelegate <NSObject>
#pragma mark Properties
// The current viewport, typically the view visible bounds with overdraw.
- (CGRect)viewportBoundsForTextViewportLayoutController:(NSTextViewportLayoutController *)textViewportLayoutController;

#pragma mark Viewport layout
// Called when textViewportLayoutController lays out a given textLayoutFragment.
// The delegate should arrange to present the text layout fragment in the UI, e.g. a sublayer/subview.
// Layout information such as viewportBounds on textViewportLayoutController is not up-to-date at the point of this call.
- (void)textViewportLayoutController:(NSTextViewportLayoutController *)textViewportLayoutController configureRenderingSurfaceForTextLayoutFragment:(NSTextLayoutFragment *)textLayoutFragment;

@optional

#pragma mark Layout Process Observation
// Called when textViewportLayoutController is about to layout.
// Layout information on textViewportLayoutController is up-to-date at the point of this call.
- (void)textViewportLayoutControllerWillLayout:(NSTextViewportLayoutController *)textViewportLayoutController;

// Called when textViewportLayoutController is about to layout.
// Layout information on textViewportLayoutController is up-to-date at the point of this call.
- (void)textViewportLayoutControllerDidLayout:(NSTextViewportLayoutController *)textViewportLayoutController;
@end

#pragma mark NSTextViewportLayoutController
API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0)) API_UNAVAILABLE(watchos)
@interface NSTextViewportLayoutController : NSObject
#pragma mark Initialization
- (instancetype)initWithTextLayoutManager:(NSTextLayoutManager *)textLayoutManager NS_DESIGNATED_INITIALIZER;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

#pragma mark Properties
// The viewport layout delegate.
@property (nullable, weak) id <NSTextViewportLayoutControllerDelegate> delegate;

// The provider of viewport text layout fragments.
@property (weak, readonly) NSTextLayoutManager *textLayoutManager;

#pragma mark Viewport layout state
// The viewport of the current layout.
// Not KVO-compliant.
@property (readonly) CGRect viewportBounds;

// The text range of the current viewport layout.
// Not KVO-compliant.
@property (nullable, readonly) NSTextRange *viewportRange;

#pragma mark Viewport layout
// Performs layout in the viewport.
- (void)layoutViewport;

// Relocates the viewport to start with the specified *location*.
// This is used to programmatically scroll to the specified *location*.
// Returns the suggested viewport anchor point.
- (CGFloat)relocateViewportToTextLocation:(id <NSTextLocation>)textLocation;

// Adjusts the viewport rect by the specified offset if needed.
// This is used to artificially move the viewport without affecting viewport content, can be both positive/negative offset.
- (void)adjustViewportByVerticalOffset:(CGFloat)verticalOffset;
@end
NS_ASSUME_NONNULL_END
#endif // !TARGET_OS_IPHONE
#else
#import <UIFoundation/NSTextViewportLayoutController.h>
#endif
