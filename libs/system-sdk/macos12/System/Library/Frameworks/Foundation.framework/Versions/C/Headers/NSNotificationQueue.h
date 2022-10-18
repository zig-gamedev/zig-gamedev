/*	NSNotificationQueue.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRunLoop.h>

@class NSNotification, NSNotificationCenter, NSArray<ObjectType>, NSString;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NSPostingStyle) {
    NSPostWhenIdle = 1,
    NSPostASAP = 2,
    NSPostNow = 3
};

typedef NS_OPTIONS(NSUInteger, NSNotificationCoalescing) {
    NSNotificationNoCoalescing = 0,
    NSNotificationCoalescingOnName = 1,
    NSNotificationCoalescingOnSender = 2
};

@interface NSNotificationQueue : NSObject {
@private
    id		_notificationCenter;
    id		_asapQueue;
    id		_asapObs;
    id		_idleQueue;
    id		_idleObs;
}
@property (class, readonly, strong) NSNotificationQueue *defaultQueue;

- (instancetype)initWithNotificationCenter:(NSNotificationCenter *)notificationCenter NS_DESIGNATED_INITIALIZER;

- (void)enqueueNotification:(NSNotification *)notification postingStyle:(NSPostingStyle)postingStyle;
- (void)enqueueNotification:(NSNotification *)notification postingStyle:(NSPostingStyle)postingStyle coalesceMask:(NSNotificationCoalescing)coalesceMask forModes:(nullable NSArray<NSRunLoopMode> *)modes;

- (void)dequeueNotificationsMatching:(NSNotification *)notification coalesceMask:(NSUInteger)coalesceMask;

@end

NS_ASSUME_NONNULL_END
