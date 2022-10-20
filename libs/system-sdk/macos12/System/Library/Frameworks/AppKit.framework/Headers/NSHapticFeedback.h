/*
   NSHapticFeedback.h
   Application Kit
   Copyright (c) 2015-2021, Apple Inc.
   All rights reserved.
*/

#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSObject.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

typedef NS_ENUM(NSInteger, NSHapticFeedbackPattern) {
    NSHapticFeedbackPatternGeneric = 0, // when none of the other options apply
    NSHapticFeedbackPatternAlignment,  // Alignment of any type: guides, best fit, etc...
    NSHapticFeedbackPatternLevelChange // Changes in discrete pressure zones. Used by NSMultiLevelAcceleratorButtons.
} API_AVAILABLE(macos(10.11));

typedef NS_ENUM(NSUInteger, NSHapticFeedbackPerformanceTime) {
    NSHapticFeedbackPerformanceTimeDefault = 0,     // Currently, NSHapticFeedbackPerformanceTimeDrawCompleted
    NSHapticFeedbackPerformanceTimeNow,             // Perform immediately. Do not synchronize
    NSHapticFeedbackPerformanceTimeDrawCompleted,   // Synchronize when the next cocoa screen drawing and layer rendering pass complete
} API_AVAILABLE(macos(10.11));

@protocol NSHapticFeedbackPerformer <NSObject>
@required
/* Provide user feedback synchronized with a drawing style. Always use the feedback pattern that describes the user action. In most cases, haptic feedback should occur with something on screen such as the appearance of an alignment guide. The haptic feedback performer waits until the performanceTime to ensure that the user simultaneously experiences both. Note: The system reserves the right to suppress this request. For example, Force Touch trackpads will not perform the feedback if the user isn't currently touching the trackpad.
 */
- (void)performFeedbackPattern:(NSHapticFeedbackPattern)pattern performanceTime:(NSHapticFeedbackPerformanceTime)performanceTime;
@end

API_AVAILABLE(macos(10.11))
@interface NSHapticFeedbackManager : NSObject
/* The most appropriate feedback performer for the current input device, accessibility settings and user preferences. Note: This device may change during the life of your application. Always request the defaultPerformer when you need perform feedback.
 */
@property (class, readonly, strong) id<NSHapticFeedbackPerformer> defaultPerformer;
@end

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END
