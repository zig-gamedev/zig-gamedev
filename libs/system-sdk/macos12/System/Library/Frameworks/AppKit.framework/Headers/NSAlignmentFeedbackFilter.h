/*
 NSAlignmentFeedbackFilter.h
 Application Kit
 Copyright (c) 2015-2021, Apple Inc.
 All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <AppKit/NSEvent.h>
#import <AppKit/NSHapticFeedback.h>
#import <AppKit/AppKitDefines.h>

@class NSPanGestureRecognizer;
@class NSView;

@protocol NSAlignmentFeedbackToken <NSObject>
@end

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

API_AVAILABLE(macos(10.11))
@interface NSAlignmentFeedbackFilter : NSObject

/*  The event mask indicating which event types the filter accepts. Suitable for use in tracking loops.
 */
@property (class, readonly) NSEventMask inputEventMask;

/*  Input a new event matching +inputEventMask.
 */
- (void)updateWithEvent:(NSEvent *)event;

/*  Input events from a pan gesture recognizer. Use this instead of -updateWithEvent: if your event handling uses gesture recognizers.
 */
- (void)updateWithPanRecognizer:(NSPanGestureRecognizer *)panRecognizer;

/*  Prepare haptic feedback for object movement due to alignment from one location to another. Returns a context object which may be performed later, or nil if no alignment should be performed. previousPoint must be the last location which the user saw the object at. alignedPoint must be the latest location if the alignment is allowed. defaultPoint must be the location which the item will be at if the alignment does not take effect. To prevent unnecessary haptic feedback to the user, it's important that the correct three locations are used.
 */
- (nullable id<NSAlignmentFeedbackToken>)alignmentFeedbackTokenForMovementInView:(nullable NSView *)view previousPoint:(NSPoint)previousPoint alignedPoint:(NSPoint)alignedPoint defaultPoint:(NSPoint)defaultPoint;

/*  Same as alignmentFeedbackTokenForMovementInView:previousPoint:alignedPoint:defaultPoint:, except that it ignores any vertical movement.
 */
- (nullable id<NSAlignmentFeedbackToken>)alignmentFeedbackTokenForHorizontalMovementInView:(nullable NSView *)view previousX:(CGFloat)previousX alignedX:(CGFloat)alignedX defaultX:(CGFloat)defaultX;

/*  Same as alignmentFeedbackTokenForMovementInView:previousPoint:alignedPoint:defaultPoint:, except that it ignores any horizontal movement.
 */
- (nullable id<NSAlignmentFeedbackToken>)alignmentFeedbackTokenForVerticalMovementInView:(nullable NSView *)view previousY:(CGFloat)previousY alignedY:(CGFloat)alignedY defaultY:(CGFloat)defaultY;

/*  Performs haptic feedback described by one or more alignment feedback tokens, typically at most one per dimension. This should be called immediately before the updated location will be presented to the user.
 */
- (void)performFeedback:(NSArray<id<NSAlignmentFeedbackToken>> *)alignmentFeedbackTokens performanceTime:(NSHapticFeedbackPerformanceTime)performanceTime;

@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
