/*	NSProcessInfo.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
 */

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSNotification.h>

NS_ASSUME_NONNULL_BEGIN

enum {	/* Constants returned by -operatingSystem */
    NSWindowsNTOperatingSystem = 1,
    NSWindows95OperatingSystem,
    NSSolarisOperatingSystem,
    NSHPUXOperatingSystem,
    NSMACHOperatingSystem,
    NSSunOSOperatingSystem,
    NSOSF1OperatingSystem
} API_DEPRECATED("Not supported", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

typedef struct {
    NSInteger majorVersion;
    NSInteger minorVersion;
    NSInteger patchVersion;
} NSOperatingSystemVersion;

@class NSArray<ObjectType>, NSString, NSDictionary<KeyType, ObjectType>;

@interface NSProcessInfo : NSObject {
@private
    NSDictionary	*environment;
    NSArray		*arguments;
    NSString		*hostName;
    NSString		*name;
    NSInteger		automaticTerminationOptOutCounter;
}

@property (class, readonly, strong) NSProcessInfo *processInfo;

@property (readonly, copy) NSDictionary<NSString *, NSString *> *environment;
@property (readonly, copy) NSArray<NSString *> *arguments;
@property (readonly, copy) NSString *hostName;
@property (copy) NSString *processName;
@property (readonly) int processIdentifier;

@property (readonly, copy) NSString *globallyUniqueString;

- (NSUInteger)operatingSystem API_DEPRECATED("-operatingSystem always returns NSMACHOperatingSystem, use -operatingSystemVersion or -isOperatingSystemAtLeastVersion: instead", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (NSString *)operatingSystemName API_DEPRECATED("-operatingSystemName always returns NSMACHOperatingSystem, use -operatingSystemVersionString instead", macos(10.0,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));

	/* Human readable, localized; appropriate for displaying to user or using in bug emails and such; NOT appropriate for parsing */
@property (readonly, copy) NSString *operatingSystemVersionString;

@property (readonly) NSOperatingSystemVersion operatingSystemVersion API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

@property (readonly) NSUInteger processorCount API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (readonly) NSUInteger activeProcessorCount API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (readonly) unsigned long long physicalMemory API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

- (BOOL) isOperatingSystemAtLeastVersion:(NSOperatingSystemVersion)version API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
@property (readonly) NSTimeInterval systemUptime API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/* Disable or reenable the ability to be quickly killed. The default implementations of these methods increment or decrement, respectively, a counter whose value is 1 when the process is first created. When the counter's value is 0 the application is considered to be safely killable and may be killed by the operating system without any notification or event being sent to the process first. If an application's Info.plist has an NSSupportsSuddenTermination entry whose value is true then NSApplication invokes -enableSuddenTermination automatically during application launch, which typically renders the process killable right away. You can also manually invoke -enableSuddenTermination right away in, for example, agents or daemons that don't depend on AppKit. After that, you can invoke these methods whenever the process has work it must do before it terminates. For example:
 - NSUserDefaults uses these to prevent process killing between the time at which a default has been set and the time at which the preferences file including that default has been written to disk.
 - NSDocument uses these to prevent process killing between the time at which the user has made a change to a document and the time at which the user's change has been written to disk.
 - You can use these whenever your application defers work that must be done before the application terminates. If for example your application ever defers writing something to disk, and it has an NSSupportsSuddenTermination entry in its Info.plist so as not to contribute to user-visible delays at logout or shutdown time, it must invoke -disableSuddenTermination when the writing is first deferred and -enableSuddenTermination after the writing is actually done.
 */
- (void)disableSuddenTermination API_AVAILABLE(macos(10.6)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)enableSuddenTermination API_AVAILABLE(macos(10.6)) API_UNAVAILABLE(ios, watchos, tvos);

/*
 * Increment or decrement the counter tracking the number of automatic quit opt-out requests. When this counter is greater than zero, the app will be considered 'active' and ineligible for automatic termination.
 * An example of using this would be disabling autoquitting when the user of an instant messaging application signs on, due to it requiring a background connection to be maintained even if the app is otherwise inactive.
 * Each pair of calls should have a matching "reason" argument, which can be used to easily track why an application is or is not automatically terminable.
 * A given reason can be used more than once at the same time (for example: two files are transferring over the network, each one disables automatic termination with the reason @"file transfer in progress")
 */
- (void)disableAutomaticTermination:(NSString *)reason API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);
- (void)enableAutomaticTermination:(NSString *)reason API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

/*
 * Marks the calling app as supporting automatic termination. Without calling this or setting the equivalent Info.plist key (NSSupportsAutomaticTermination), the above methods (disableAutomaticTermination:/enableAutomaticTermination:) have no effect,
 * although the counter tracking automatic termination opt-outs is still kept up to date to ensure correctness if this is called later. Currently, passing NO has no effect.
 * This should be called during -applicationDidFinishLaunching or earlier.
 */
@property BOOL automaticTerminationSupportEnabled API_AVAILABLE(macos(10.7)) API_UNAVAILABLE(ios, watchos, tvos);

@end

/*
 The system has heuristics to improve battery life, performance, and responsiveness of applications for the benefit of the user. This API can be used to give hints to the system that your application has special requirements. In response to creating one of these activities, the system will disable some or all of the heuristics so your application can finish quickly while still providing responsive behavior if the user needs it.
 
 These activities can be used when your application is performing a long-running operation. If the activity can take different amounts of time (for example, calculating the next move in a chess game), it should use this API. This will ensure correct behavior when the amount of data or the capabilities of the user's computer varies. You should put your activity into one of two major categories:
 
 User initiated: These are finite length activities that the user has explicitly started. Examples include exporting or downloading a user specified file.
 
 Background: These are finite length activities that are part of the normal operation of your application but are not explicitly started by the user. Examples include autosaving, indexing, and automatic downloading of files.
 
 In addition, if your application requires high priority IO, you can include the 'NSActivityLatencyCritical' flag (using a bitwise or). This should be reserved for activities like audio or video recording.
 
 If your activity takes place synchronously inside an event callback on the main thread, you do not need to use this API.
 
 Be aware that failing to end these activities for an extended period of time can have significant negative impacts to the performance of your user's computer, so be sure to use only the minimum amount of time required. User preferences may override your application’s request.
 
 This API can also be used to control auto termination or sudden termination. 
 
    id activity = [NSProcessInfo.processInfo beginActivityWithOptions:NSActivityAutomaticTerminationDisabled reason:@"Good Reason"];
    // work
    [NSProcessInfo.processInfo endActivity:activity];
 
 is equivalent to:
 
    [NSProcessInfo.processInfo disableAutomaticTermination:@"Good Reason"];
    // work
    [NSProcessInfo.processInfo enableAutomaticTermination:@"Good Reason"]
 
 Since this API returns an object, it may be easier to pair begins and ends. If the object is deallocated before the -endActivity: call, the activity will be automatically ended.
 
 This API also provides a mechanism to disable system-wide idle sleep and display idle sleep. These can have a large impact on the user experience, so be sure not to forget to end activities that disable sleep (including NSActivityUserInitiated).
 
 */

typedef NS_OPTIONS(uint64_t, NSActivityOptions) {
    // To include one of these individual flags in one of the sets, use bitwise or:
    //   NSActivityUserInitiated | NSActivityIdleDisplaySleepDisabled
    // (this may be used during a presentation, for example)
    
    // To exclude from one of the sets, use bitwise and with not:
    //   NSActivityUserInitiated & ~NSActivitySuddenTerminationDisabled
    // (this may be used during a user intiated action that may be safely terminated with no application interaction in case of logout)
    
    // Used for activities that require the screen to stay powered on.
    NSActivityIdleDisplaySleepDisabled = (1ULL << 40),
    
    // Used for activities that require the computer to not idle sleep. This is included in NSActivityUserInitiated.
    NSActivityIdleSystemSleepDisabled = (1ULL << 20),
    
    // Prevents sudden termination. This is included in NSActivityUserInitiated.
    NSActivitySuddenTerminationDisabled = (1ULL << 14),
    
    // Prevents automatic termination. This is included in NSActivityUserInitiated.
    NSActivityAutomaticTerminationDisabled = (1ULL << 15),
    
    // ----
    // Sets of options.
    
    // App is performing a user-requested action.
    NSActivityUserInitiated = (0x00FFFFFFULL | NSActivityIdleSystemSleepDisabled),
    NSActivityUserInitiatedAllowingIdleSystemSleep = (NSActivityUserInitiated & ~NSActivityIdleSystemSleepDisabled),
    
    // App has initiated some kind of work, but not as the direct result of user request.
    NSActivityBackground = 0x000000FFULL,
    
    // Used for activities that require the highest amount of timer and I/O precision available. Very few applications should need to use this constant.
    NSActivityLatencyCritical = 0xFF00000000ULL,
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@interface NSProcessInfo (NSProcessInfoActivity)
/*
 * Pass in an activity to this API, and a non-NULL, non-empty reason string. Indicate completion of the activity by calling the corresponding endActivity: method with the result of the beginActivityWithOptions:reason: method. The reason string is used for debugging.
 */
- (id <NSObject>)beginActivityWithOptions:(NSActivityOptions)options reason:(NSString *)reason API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*
 * The argument to this method is the result of beginActivityWithOptions:reason:.
 */
- (void)endActivity:(id <NSObject>)activity API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*
 * Synchronously perform an activity. The activity will be automatically ended after your block argument returns. The reason string is used for debugging.
 */
- (void)performActivityWithOptions:(NSActivityOptions)options reason:(NSString *)reason usingBlock:(void (^)(void))block API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/* 
 * Perform an expiring background task, which obtains an expiring task assertion on iOS. The block contains any work which needs to be completed as a background-priority task. The block will be scheduled on a system-provided concurrent queue. After a system-specified time, the block will be called with the `expired` parameter set to YES. The `expired` parameter will also be YES if the system decides to prematurely terminate a previous non-expiration invocation of the block.
 */
- (void)performExpiringActivityWithReason:(NSString *)reason usingBlock:(void(^)(BOOL expired))block API_AVAILABLE(ios(8.2), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

@end

@interface NSProcessInfo (NSUserInformation)

@property (readonly, copy) NSString *userName API_AVAILABLE(macosx(10.12)) API_UNAVAILABLE(ios, watchos, tvos);
@property (readonly, copy) NSString *fullUserName API_AVAILABLE(macosx(10.12)) API_UNAVAILABLE(ios, watchos, tvos);

@end

// Describes the current thermal state of the system.
typedef NS_ENUM(NSInteger, NSProcessInfoThermalState) {
    // No corrective action is needed.
    NSProcessInfoThermalStateNominal,

    // The system has reached a state where fans may become audible (on systems which have fans). Recommendation: Defer non-user-visible activity.
    NSProcessInfoThermalStateFair,

    // Fans are running at maximum speed (on systems which have fans), system performance may be impacted. Recommendation: reduce application's usage of CPU, GPU and I/O, if possible. Switch to lower quality visual effects, reduce frame rates.
    NSProcessInfoThermalStateSerious,
    
    // System performance is significantly impacted and the system needs to cool down. Recommendation: reduce application's usage of CPU, GPU, and I/O to the minimum level needed to respond to user actions. Consider stopping use of camera and other peripherals if your application is using them.
    NSProcessInfoThermalStateCritical
} API_AVAILABLE(macosx(10.10.3), ios(11.0), watchos(4.0), tvos(11.0));

@interface NSProcessInfo (NSProcessInfoThermalState)

// Retrieve the current thermal state of the system. On systems where thermal state is unknown or unsupported, the value returned from the thermalState property is always NSProcessInfoThermalStateNominal.
@property (readonly) NSProcessInfoThermalState thermalState API_AVAILABLE(macosx(10.10.3), ios(11.0), watchos(4.0), tvos(11.0));

@end

@interface NSProcessInfo (NSProcessInfoPowerState)

// Retrieve the current setting of the system for the low power mode setting. On systems where the low power mode is unknown or unsupported, the value returned from the lowPowerModeEnabled property is always NO
@property (readonly, getter=isLowPowerModeEnabled) BOOL lowPowerModeEnabled API_AVAILABLE(macos(12.0), ios(9.0), watchos(2.0), tvos(9.0));

@end

/*
 NSProcessInfoThermalStateDidChangeNotification is posted once the thermal state of the system has changed. Once the notification is posted, use the thermalState property to retrieve the current thermal state of the system.
 
 You can use this opportunity to take corrective action in your application to help cool the system down. Work that could be done in the background or at opportunistic times should be using the Quality of Service levels in NSOperation or the NSBackgroundActivityScheduler API.
 
 This notification is posted on the global dispatch queue. Register for it using the default notification center. The object associated with the notification is NSProcessInfo.processInfo.
*/
FOUNDATION_EXTERN NSNotificationName const NSProcessInfoThermalStateDidChangeNotification API_AVAILABLE(macosx(10.10.3), ios(11.0), watchos(4.0), tvos(11.0));

/*
 NSProcessInfoPowerStateDidChangeNotification is posted once any power usage mode of the system has changed. Once the notification is posted, use the isLowPowerModeEnabled property to retrieve the current state of the low power mode setting of the system.
 
 When this notification is posted your application should attempt to reduce power usage by reducing potentially costly computation and other power using activities like network activity or keeping the screen on if the low power mode setting is enabled.
 
 This notification is posted on the global dispatch queue. Register for it using the default notification center. The object associated with the notification is NSProcessInfo.processInfo.
 */
FOUNDATION_EXTERN NSNotificationName const NSProcessInfoPowerStateDidChangeNotification API_AVAILABLE(macos(12.0), ios(9.0), watchos(2.0), tvos(9.0));

@interface NSProcessInfo (NSProcessInfoPlatform)

@property (readonly, getter=isMacCatalystApp) BOOL macCatalystApp API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));
@property (readonly, getter=isiOSAppOnMac) BOOL iOSAppOnMac API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

@end

NS_ASSUME_NONNULL_END
