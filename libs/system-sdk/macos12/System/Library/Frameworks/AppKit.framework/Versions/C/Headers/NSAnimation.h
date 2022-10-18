/*
    NSAnimation.h
    Application Kit
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKitDefines.h>

NS_ASSUME_NONNULL_BEGIN
APPKIT_API_UNAVAILABLE_BEGIN_MACCATALYST

@class NSGraphicsContext, NSString;
@protocol NSAnimationDelegate;

typedef NS_ENUM(NSUInteger, NSAnimationCurve) {
    NSAnimationEaseInOut,       // default
    NSAnimationEaseIn,
    NSAnimationEaseOut,
    NSAnimationLinear
};

typedef NS_ENUM(NSUInteger, NSAnimationBlockingMode) {
    NSAnimationBlocking,
    NSAnimationNonblocking,
    NSAnimationNonblockingThreaded
};

typedef float NSAnimationProgress;

APPKIT_EXTERN NSNotificationName NSAnimationProgressMarkNotification; // has single entry in user info dictionary
APPKIT_EXTERN NSString * NSAnimationProgressMark; // NSNumber(float) with NSAnimationProgress

@interface NSAnimation : NSObject <NSCopying, NSCoding>

- (instancetype)initWithDuration:(NSTimeInterval)duration animationCurve:(NSAnimationCurve)animationCurve NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

- (void)startAnimation;
- (void)stopAnimation;
@property (getter=isAnimating, readonly) BOOL animating;

@property NSAnimationProgress currentProgress;

@property NSTimeInterval duration;

@property NSAnimationBlockingMode animationBlockingMode;

@property float frameRate;

@property NSAnimationCurve animationCurve;

@property (readonly) float currentValue;

@property (nullable, weak) id<NSAnimationDelegate> delegate;

@property (copy) NSArray<NSNumber *> *progressMarks;

- (void)addProgressMark:(NSAnimationProgress)progressMark;
- (void)removeProgressMark:(NSAnimationProgress)progressMark;

- (void)startWhenAnimation:(NSAnimation *)animation reachesProgress:(NSAnimationProgress)startProgress;
- (void)stopWhenAnimation:(NSAnimation *)animation reachesProgress:(NSAnimationProgress)stopProgress;

- (void)clearStartAnimation;
- (void)clearStopAnimation;

@property (nullable, readonly, copy) NSArray<NSRunLoopMode> *runLoopModesForAnimating;

@end

@protocol NSAnimationDelegate <NSObject>
@optional
- (BOOL)animationShouldStart:(NSAnimation *)animation;
- (void)animationDidStop:(NSAnimation *)animation;
- (void)animationDidEnd:(NSAnimation *)animation;
- (float)animation:(NSAnimation *)animation valueForProgress:(NSAnimationProgress)progress;
- (void)animation:(NSAnimation *)animation didReachProgressMark:(NSAnimationProgress)progress;
@end

/*-----------------------------------------------------------------------------------------*/

typedef NSString * NSViewAnimationKey NS_TYPED_ENUM;
APPKIT_EXTERN NSViewAnimationKey NSViewAnimationTargetKey;       // NSWindow* or NSView* (required)
APPKIT_EXTERN NSViewAnimationKey NSViewAnimationStartFrameKey;   // NSValue*(NSRect) (optional)
APPKIT_EXTERN NSViewAnimationKey NSViewAnimationEndFrameKey;     // NSValue*(NSRect) (optional)
APPKIT_EXTERN NSViewAnimationKey NSViewAnimationEffectKey;       // NSViewAnimationEffectName (optional)

typedef NSString * NSViewAnimationEffectName NS_TYPED_ENUM;
APPKIT_EXTERN NSViewAnimationEffectName NSViewAnimationFadeInEffect;
APPKIT_EXTERN NSViewAnimationEffectName NSViewAnimationFadeOutEffect;

@interface NSViewAnimation : NSAnimation

- (instancetype)initWithViewAnimations:(NSArray<NSDictionary<NSViewAnimationKey, id> *> *)viewAnimations;

@property (copy) NSArray<NSDictionary<NSViewAnimationKey, id> *> *viewAnimations;

@end

typedef NSString * NSAnimatablePropertyKey NS_SWIFT_BRIDGED_TYPEDEF;

API_AVAILABLE(macos(10.5), ios(10.13))
@protocol NSAnimatablePropertyContainer

/* Returns a proxy object for the receiver that can be used to initiate implied animation of property changes. An object's "animator" should be treated as if it was the object itself, and may be passed to any code that accepts the object as a parameter. Sending of KVC-compliant "set" messages to the proxy will trigger animation for automatically animated properties of its target object, if the active NSAnimationContext in the current thread has a duration value greater than zero, and an animation to use for the property key is found by the -animationForKey: search mechanism defined below. An object's automatically animated properties are those for which [theObject animationForKey:] finds and returns an CAAnimation instead of nil, often because [[theObject class] defaultAnimationForKey:] specifies a default animation for the key.

It's perfectly valid to set a new value for a property for which an animation is currently in progress; this simply sets a new target value for that property, with animation to the new target proceeding from whatever current value the property has reached. An in-flight property animation can be stopped by setting a new value for the property with 0.0 as the surrounding NSAnimationContext's duration value.
*/
- (instancetype)animator API_AVAILABLE(macos(10.5));

/* An animatable property container's optional "animations" dictionary maps NSString keys to CAAnimation values. When an occurrence matching the key fires for the view, -animationForKey: first looks in this dictionary for an animation to execute in response. Typically, the key will name a property of the object whose value has just changed, but it may instead specify a special event trigger (NSAnimationTriggerOrderIn or NSAnimationTriggerOrderOut).
*/
@property (readwrite, copy) NSDictionary<NSAnimatablePropertyKey, id> *animations API_AVAILABLE(macos(10.5));

/* When the occurrence specified by "key" fires for an object, this method is consulted to find the animation, if any, that should be performed in response. Like its counterpart, -[CALayer actionForKey:], this method is a funnel point that defines the order in which the search for an animation proceeds, and is not one that clients would typically need to override. It first checks the receiver's "animations" dictionary, then falls back to  +defaultAnimationForKey: for the receiver's class.
*/
- (nullable id)animationForKey:(NSAnimatablePropertyKey)key API_AVAILABLE(macos(10.5));

/* As described above, -animationForKey: consults this class method when its search of an instance's "animations" dictionary doesn't turn up an animation to use for a given property change.

An animatable property container should implement +defaultAnimationForKey: to return a default animation to be performed for each key that it wants to make auto-animatable, where "key" usually names a property of the receiver, but can also specify a special animation trigger (NSAnimationTriggerOrderIn or NSAnimationTriggerOrderOut).

A developer implementing a custom view subclass, for example, can enable automatic animation the subclass' added properties by overriding this method, and having it return the desired default CAAnimation to use for each of the property keys of interest. The override should defer to super for any keys it doesn't specifically handle, facilitating inheritance of default animation specifications.

The full set of available CAAnimation classes can be found in QuartzCore/CAAnimation.h.  Useful concrete animation classes are CABasicAnimation, CAKeyframeAnimation, CAAnimationGroup, and CATransition, all of which can be instantiated with default parameters by sending the class an +animation message.

@implementation MyView
+ (id)defaultAnimationForKey:(NSString *)key {
    if ([key isEqualToString:@"borderColor"]) {
        // By default, animate border color changes with simple linear interpolation to the new color value.
        return [CABasicAnimation animation];
    } else {
        // Defer to super's implementation for any keys we don't specifically handle.
        return [super defaultAnimationForKey:key];
    }
}
@end
*/
+ (nullable id)defaultAnimationForKey:(NSAnimatablePropertyKey)key API_AVAILABLE(macos(10.5));

@end /* @protocol NSAnimatablePropertyContainer */

APPKIT_EXTERN NSAnimatablePropertyKey NSAnimationTriggerOrderIn API_AVAILABLE(macos(10.5));
APPKIT_EXTERN NSAnimatablePropertyKey NSAnimationTriggerOrderOut API_AVAILABLE(macos(10.5));

API_UNAVAILABLE_END
NS_ASSUME_NONNULL_END


