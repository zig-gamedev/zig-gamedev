/*
    NSSliderAccessory.h
    Application Kit
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
 */

#import <AppKit/NSAccessibility.h>
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
@class UIImage;
#endif

NS_ASSUME_NONNULL_BEGIN

@class NSSlider, NSSliderAccessoryBehavior, NSImage;

API_AVAILABLE(macos(10.12), ios(13.0)) NS_SWIFT_UI_ACTOR
@interface NSSliderAccessory : NSObject <NSCoding>

/// Creates an image-based accessory
#if !TARGET_OS_IPHONE
+ (NSSliderAccessory *)accessoryWithImage:(NSImage *)image;
#else
+ (NSSliderAccessory *)accessoryWithImage:(UIImage *)image API_AVAILABLE(ios(13.0));
#endif

/// The effect on interaction with the accessory. Defaults to `automaticBehavior`
@property (copy) NSSliderAccessoryBehavior *behavior;

/// Whether or not the accessory is interactive and draws with an enabled appearance. Defaults to YES.
@property (getter=isEnabled) BOOL enabled;

@end

#if !TARGET_OS_IPHONE
@interface NSSliderAccessory () <NSAccessibility, NSAccessibilityElement>
@end
#endif

API_AVAILABLE(macos(10.12), ios(13.0)) NS_SWIFT_UI_ACTOR
@interface NSSliderAccessoryBehavior : NSObject <NSCoding, NSCopying>

/// The behavior is automatically picked to be the system standard for the slider's current context, e.g. NSTouchBarItems have `.valueStep` behavior.
@property (class, readonly, copy) NSSliderAccessoryBehavior *automaticBehavior;

/// The value of the slider moves towards the associated value for the accessory with by a delta of the slider's `altIncrementValue`.
@property (class, readonly, copy) NSSliderAccessoryBehavior *valueStepBehavior;

/// The value of the slider is reset to the associated value for the accessory.
@property (class, readonly, copy) NSSliderAccessoryBehavior *valueResetBehavior;

/// The action is sent to the target on interaction. The optional first parameter is an NSSliderAccessory.
+ (NSSliderAccessoryBehavior *)behaviorWithTarget:(nullable id)target action:(SEL)action;

/// The handler block is invoked on interaction. This variant is not codable and will assert in `-encodeWithCoder:`.
+ (NSSliderAccessoryBehavior *)behaviorWithHandler:(void(^)(NSSliderAccessory *))handler;

/// Override point for custom subclasses to handle interaction.
- (void)handleAction:(NSSliderAccessory *)sender;

@end

NS_ASSUME_NONNULL_END
