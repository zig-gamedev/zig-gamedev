/*	NSBackgroundActivityScheduler.h
	Copyright (c) 2013-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSDate.h>

NS_ASSUME_NONNULL_BEGIN

/* These values are arguments to the completion handler block for the scheduler. */
typedef NS_ENUM(NSInteger, NSBackgroundActivityResult) {
    // Indicate that the work for this activity has finished and the next invocation can be scheduled.
    NSBackgroundActivityResultFinished = 1,
    
    // Indicate that the system should defer the remainder of this work for now and reschedule the completion soon. This should be used in response to checking the value of -shouldDefer.
    NSBackgroundActivityResultDeferred = 2,
} API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos);

typedef void (^NSBackgroundActivityCompletionHandler)(NSBackgroundActivityResult result) API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos);

/*
 This class provides a Cocoa-level interface to the XPC Activity API (see xpc/activity.h).
 
 It is used to schedule maintenance or background kinds of tasks. These activities are run by the OS at a time that best accommodates system-wide factors like energy, thermal conditions, and CPU usage. If you have activities that run at an interval measured in 10s of minutes or more, then use this class to schedule those activities.
 
 */
API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSBackgroundActivityScheduler : NSObject {
    @private
    id _private1;
    id _private2;
    id _private3;
    int64_t _flags;
}

/* Initialize a new instance of the class. The identifier argument should be a string that remains constant across launches of your application. The system uses the value to keep track of the number of times this activity has run and improve the heuristics for deciding when to run the activity in the future.
   
   The string should be a reverse DNS style string, such as "com.mycompany.myapplication.myactivity". nil and zero-length strings are not allowed.
*/
- (instancetype)initWithIdentifier:(NSString *)identifier NS_DESIGNATED_INITIALIZER;

/* The identifier this object was initialized with.
 */
@property (readonly, copy) NSString *identifier;

/* Default value is NSQualityOfServiceBackground. If you upgrade the quality of service above this level, the system will more aggressively schedule this activity. The default value is the recommended value for most activities.
 */
@property NSQualityOfService qualityOfService;

/* The default value is NO. 
   If set to YES, then the activity will be rescheduled after finishing at the specified interval.
 */
@property BOOL repeats;

/* If this activity is repeating, then this property describes the average interval of seconds between invocations of this activity.
   If the activity is not repeating, then this property is the suggested interval of time between scheduling the activity and the invocation of the activity.
*/
@property NSTimeInterval interval;

/* Specifies the number of seconds before or after the nominal fire date when the activity should be invoked. The nominal fire date is calculated by using the interval combined with the previous fire date or the time when the activity is started. These two properties create a window in time during which the activity may be scheduled.
 
   The system will more aggresively schedule the activity as it nears the end of the grace period after the nominal fire date.
 
   The default value is 1/2 the interval.
 */
@property NSTimeInterval tolerance;

/* Begin scheduling the activity. All properties should be setup by this point.
 
   The block will be called on a serial background queue appropriate for the level of quality of service specified. The system will automatically use NSProcessInfo's -beginActivityWithOptions:reason: API while invoking the block, choosing options appropriate for the desired quality of service.
  
   When you are done with the work, be sure to invoke the completionHandler block given to you as an argument to your block. You can pass in a kind of NSBackgroundActivityResult to indicate if the work has been finished or if you are deferring the rest of the work (see -shouldDefer). If you fail to invoke the completionHandler then your activity will never be rescheduled.
 
   When invoking the completionHandler, the system will set the schedule the next invocation. If you want to adjust the properties of your scheduler (e.g., the interval or tolerance), do this before invoking the completion block.
 */
- (void)scheduleWithBlock:(void (^)(NSBackgroundActivityCompletionHandler completionHandler))block NS_SWIFT_DISABLE_ASYNC;

/* Stop scheduling the activity. Any currently executing block will still complete.
 */
- (void)invalidate;

/* You can call this method occasionally during execution of your activity to determine if system conditions have changed such that it is no longer a good time to run this activity.
 
   If the result of this method is YES, then you should wrap up current state. Once you are done saving your state, invoke the completionHandler block (the argument to your scheduled block) with the NSBackgroundActivityResultDeferred constant. The system will invoke your activity block again at a later, more appropriate time. At that point you can read your state and continue the work.
 */
@property (readonly) BOOL shouldDefer;

@end

NS_ASSUME_NONNULL_END
