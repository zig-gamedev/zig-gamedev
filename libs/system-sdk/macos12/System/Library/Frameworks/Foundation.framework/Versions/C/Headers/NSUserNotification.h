/*	
     NSUserNotification.h
     Copyright (c) 2011-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSDictionary<KeyType, ObjectType>, NSArray<ObjectType>, NSDateComponents, NSDate, NSTimeZone, NSImage, NSAttributedString, NSUserNotificationAction;
@protocol NSUserNotificationCenterDelegate;

NS_ASSUME_NONNULL_BEGIN

/* All NSUserNotifications API are deprecated. Please switch to the UserNotifications.framework for all notifications work. */
// Used to describe the method in which the user activated the user notification. Alerts can be activated by either clicking on the body of the alert or the action button.
typedef NS_ENUM(NSInteger, NSUserNotificationActivationType) {
    NSUserNotificationActivationTypeNone = 0,
    NSUserNotificationActivationTypeContentsClicked = 1,
    NSUserNotificationActivationTypeActionButtonClicked = 2,
    NSUserNotificationActivationTypeReplied API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos) = 3,
    NSUserNotificationActivationTypeAdditionalActionClicked API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos) = 4,
  } API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos);

API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserNotification : NSObject <NSCopying> {
@private
    id _internal;
}

- (instancetype)init NS_DESIGNATED_INITIALIZER;

// -----------------------
// These properties are used to configure the notification before it is scheduled.

// The title of the notification. Must be localized as it will be presented to the user. String will be truncated to a length appropriate for display.
@property (nullable, copy) NSString *title;

// The subtitle displayed in the notification. Must be localized as it will be presented to the user. String will be truncated to a length appropriate for display.
@property (nullable, copy) NSString *subtitle;

// The body of the notification. Must be localized as it will be presented to the user. String will be truncated to a length appropriate for display.
@property (nullable, copy) NSString *informativeText;

// The title of the button displayed in the notification. Must be localized as it will be presented to the user. String will be truncated to a length appropriate for display.
@property (copy) NSString *actionButtonTitle;

// Application-specific user info that may be retrieved later. All items must be property list types or an exception will be thrown. The userInfo should be of reasonable serialized size or an exception will be thrown.
@property (nullable, copy) NSDictionary<NSString *, id> *userInfo;

// Specifies when (in an absolute time) the notification should be delivered. After a notification is delivered, it may be presented to the user.
@property (nullable, copy) NSDate *deliveryDate;

// Set the time zone to interpret the delivery date in. If this value is nil and the user switches time zones, the notification center will adjust the time of presentation to account for the time zone change. If a notification should be delivered at a time in a specific time zone (regardless if the user switches time zones), set this value to that time zone. One common value may be the current time zone.
@property (nullable, copy) NSTimeZone *deliveryTimeZone;

// The date components that specify how a notification is to be repeated. This value may be nil if the notification should not repeat. The date component values are relative to the date the notification was delivered. If the calendar value of the deliveryRepeatInterval is nil, the current calendar will be used to calculate the repeat interval. For example, if a notification should repeat every hour, set the 'hour' property of the deliveryRepeatInterval to 1.
@property (nullable, copy) NSDateComponents *deliveryRepeatInterval;

// The date at which this notification was actually delivered. The notification center will set this value if a notification is put in the scheduled list and the delivery time arrives. If the notification is delivered directly using the 'deliverNotification:' method on NSUserNotificationCenter, this value will be set to the deliveryDate value (unless deliveryDate is nil, in which case this value is set to the current date). This value is used to sort the list of notifications in the user interface.
@property (nullable, readonly, copy) NSDate *actualDeliveryDate;

// In some cases, e.g. when your application is frontmost, the notification center may decide not to actually present a delivered notification. In that case, the value of this property will be NO. It will be set to YES if the notification was presented according to user preferences (note: this can mean no dialog, animation, or sound, if the user has turned off notifications completely for your application). 
@property (readonly, getter=isPresented) BOOL presented;

// This property will be YES if the user notification is from a remote (push) notification.
@property (readonly, getter=isRemote) BOOL remote;

// The name of the sound file in the resources of the application bundle to play when the notification is delivered. NSUserNotificationDefaultSoundName can be used to play the default Notification Center sound. A value of 'nil' means no sound.
@property (nullable, copy) NSString *soundName;

// Set to NO if the notification has no action button. This will be the case for notifications that are purely for informational purposes and have no user action. The default value is YES.
@property BOOL hasActionButton;

// This property describes how notifications sent to the didActivateNotification were activated.
@property (readonly) NSUserNotificationActivationType activationType;

// Set this property to a localized string to customize the title of the 'Close' button in an alert-style notification. An empty string will cause the default localized text to be used. A nil value is invalid.
@property (copy) NSString *otherButtonTitle;

// This identifier is used to uniquely identify a notification. A notification delivered with the same identifier as an existing notification will replace that notification, rather then display a new one.
@property (nullable, copy) NSString *identifier API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

// NSImage shown in the content of the notification.
@property (nullable, copy) NSImage *contentImage API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

// Set to YES if the notification has a reply button. The default value is NO. If both this and hasActionButton are YES, the reply button will be shown.
@property BOOL hasReplyButton API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

// Optional placeholder for inline reply field.
@property (nullable, copy) NSString *responsePlaceholder API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

// When a notification has been responded to, the NSUserNotificationCenter delegate didActivateNotification: will be called with the notification with the activationType set to NSUserNotificationActivationTypeReplied and the response set on the response property
@property (nullable, readonly, copy) NSAttributedString *response API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

// An array of NSUserNotificationAction objects that describe the different actions that can be taken on a notification in addition to the default action described by actionButtonTitle
@property (nullable, copy) NSArray<NSUserNotificationAction *> *additionalActions API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos);
// When a user selects an additional action that action will be set on the notification's additionalActivationAction property when passed into the delegate callback didActivateNotification
@property (nullable, readonly, copy) NSUserNotificationAction *additionalActivationAction API_AVAILABLE(macos(10.10)) API_UNAVAILABLE(ios, watchos, tvos);

@end

// An action shown to the user as part of a NSUserNotification in the additionalActions property.
API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.10, 11.0)) API_UNAVAILABLE(ios, watchos, tvos)

@interface NSUserNotificationAction : NSObject <NSCopying>

+ (instancetype)actionWithIdentifier:(nullable NSString *)identifier title:(nullable NSString *)title;

@property (nullable, readonly, copy) NSString *identifier;

// The localized title of the action.
@property (nullable, readonly, copy) NSString *title;

@end

FOUNDATION_EXPORT NSString * const NSUserNotificationDefaultSoundName API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos);

API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos)
@interface NSUserNotificationCenter : NSObject {
@private
    id _internal;
}

// Get a singleton user notification center that posts notifications for this process.
@property (class, readonly, strong) NSUserNotificationCenter *defaultUserNotificationCenter;

@property (nullable, assign) id <NSUserNotificationCenterDelegate> delegate;

/* 
    Notifications that the NSUserNotificationCenter is tracking will be in one of two states: scheduled or delivered. 
    A scheduled notification has a deliveryDate. On that delivery date, the notification will move from being scheduled to being delivered.
    A delivered notification has an actualDeliveryDate. That is the date when it moved from being scheduled to delivered, or when it was manually delivered (using the deliverNotification: method).
    The notification center reserves the right to decide if a delivered notification is presented on screen to the user. For example, it may supress the notification if the application is already frontmost. The application may check the result of this decision by examining the 'presented' property of a delivered notification.
    The application and the notification center are both ultimately subject to user preferences. If the user decides to hide all alerts from your application, the presented property will still behave as above, but the user will not see any animation or hear any sound.
*/

// Get a list of notifications that are scheduled but have not yet been presented. Newly scheduled notifications are added to the end of the array. You may also bulk-schedule notifications by setting this array.
@property (copy) NSArray<NSUserNotification *> *scheduledNotifications;

// Add a notification to the center for scheduling.
- (void)scheduleNotification:(NSUserNotification *)notification;

// Cancels a notification. If the deliveryDate occurs before the cancellation finishes, the notification may still be delivered. If the notification is not in the scheduled list, nothing happens.
- (void)removeScheduledNotification:(NSUserNotification *)notification;



// Get a list of notifications that have been delivered to the Notification Center. The number of notifications the user actually sees in the user interface may be less than the size of this array. Note that these may or may not have been actually presented to the user (see the presented property on the NSUserNotification).
@property (readonly, copy) NSArray<NSUserNotification *> *deliveredNotifications;

// Deliver a notification immediately, including animation or sound alerts. It will be presented to the user (subject to user preferences). The 'presented' property will always be set to YES if a notification is delivered using this method.
- (void)deliverNotification:(NSUserNotification *)notification;

// Clear a delivered notification from the notification center. If the notification is not in the delivered list, nothing happens.
- (void)removeDeliveredNotification:(NSUserNotification *)notification;

// Clear all delivered notifications for this application from the notification center.
- (void)removeAllDeliveredNotifications;

@end

/* All NSUserNotifications API are deprecated. Please switch to the UserNotifications.framework for all notifications work. */
@protocol NSUserNotificationCenterDelegate <NSObject>
@optional

// Sent to the delegate when a notification delivery date has arrived. At this time, the notification has either been presented to the user or the notification center has decided not to present it because your application was already frontmost.
- (void)userNotificationCenter:(NSUserNotificationCenter *)center didDeliverNotification:(NSUserNotification *)notification API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos);

// Sent to the delegate when a user clicks on a notification in the notification center. This would be a good time to take action in response to user interacting with a specific notification.
// Important: If want to take an action when your application is launched as a result of a user clicking on a notification, be sure to implement the applicationDidFinishLaunching: method on your NSApplicationDelegate. The notification parameter to that method has a userInfo dictionary, and that dictionary has the NSApplicationLaunchUserNotificationKey key. The value of that key is the NSUserNotification that caused the application to launch. The NSUserNotification is delivered to the NSApplication delegate because that message will be sent before your application has a chance to set a delegate for the NSUserNotificationCenter.
- (void)userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos);

// Sent to the delegate when the Notification Center has decided not to present your notification, for example when your application is front most. If you want the notification to be displayed anyway, return YES.
- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification API_DEPRECATED("All NSUserNotifications API should be replaced with UserNotifications.frameworks API", macos(10.8, 11.0)) API_UNAVAILABLE(ios, watchos, tvos);

@end

NS_ASSUME_NONNULL_END
