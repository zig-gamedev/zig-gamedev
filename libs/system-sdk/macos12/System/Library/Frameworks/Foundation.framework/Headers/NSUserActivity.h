/*	NSUserActivity.h
	Copyright (c) 2014-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSObjCRuntime.h>

#if __OBJC2__

NS_ASSUME_NONNULL_BEGIN

@class NSArray, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSSet<ObjectType>, NSString, NSURL, NSInputStream, NSOutputStream, NSError;
@protocol NSUserActivityDelegate;

typedef NSString* NSUserActivityPersistentIdentifier NS_SWIFT_BRIDGED_TYPEDEF;

/* NSUserActivity encapsulates the state of a user activity in an application on a particular device, in a way that allows the same activity to be continued on another device in a corresponding application from the same developer. Examples of user user activities include editing a document, viewing a web page, or watching a video.
*/
API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0))
@interface NSUserActivity : NSObject

/* Initializes and returns a newly created NSUserActivity with the given activityType. A user activity may be continued only in an application that (1) has the same developer Team ID as the activity's source application and (2) supports the activity's type. Supported activity types are specified in the application's Info.plist under the NSUserActivityTypes key. When receiving a user activity for continuation, the system locates the appropriate application to launch by finding applications with the target Team ID, then filtering on the incoming activity's type identifier.
*/
- (instancetype)initWithActivityType:(NSString *)activityType NS_DESIGNATED_INITIALIZER;

/* Initializes and returns a newly created NSUserActivity with the first activityType from the NSUserActivityTypes key in the application’s Info.plist.
*/
- (instancetype)init API_DEPRECATED("Use initWithActivityType: with a specific activity type string", macosx(10.10, 10.12), ios(8.0,10.0), watchos(2.0,3.0), tvos(9.0,10.0));

/* The activityType the user activity was created with.
*/
@property (readonly, copy) NSString *activityType;

/* An optional, user-visible title for this activity, such as a document name or web page title.
*/
@property (nullable, copy) NSString *title;

/* The userInfo dictionary contains application-specific state needed to continue an activity on another device. Each key and value must be of the following types: NSArray, NSData, NSDate, NSDictionary, NSNull, NSNumber, NSSet, NSString, NSURL, or NSUUID. File scheme URLs which refer to iCloud documents may be translated to valid file URLs on a receiving device.
*/
@property (nullable, copy) NSDictionary *userInfo;

/* Adds to the userInfo dictionary the entries from otherDictionary.  The keys and values must be of the types allowed in the userInfo 
*/
- (void)addUserInfoEntriesFromDictionary:(NSDictionary *)otherDictionary;

/* The keys from the userInfo property which represent the minimal information about this user activity that should be stored for later restoration.  A nil value means all keys in .userInfo are required. */
@property (nullable, copy) NSSet<NSString *> *requiredUserInfoKeys API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* If set to YES, then the delegate for this user activity will receive a userActivityWillSave: callback before being sent for continuation on another device. 
*/
@property (assign) BOOL needsSave;

/* When no suitable application is installed on a resuming device and the webpageURL is set, the user activity will instead be continued in a web browser by loading this resource.
*/
@property (nullable, copy) NSURL *webpageURL;

/* The URL of the webpage that referred (linked to) webpageURL.
 */
@property (nullable, copy) NSURL *referrerURL API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/* If non-nil, then an absolute date after which this activity is no longer eligible to be indexed or handed off. */
@property (nullable, copy) NSDate *expirationDate API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* A set of NSString* keywords, representing words or phrases in the current user's language that might help the user to find this activity in the application history. */
@property (copy) NSSet<NSString *> *keywords API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* When used for continuation, the user activity can allow the continuing side to connect back for more information using streams. This value is set to NO by default. It can be dynamically set to YES to selectively support continuation streams based on the state of the user activity.
*/
@property BOOL supportsContinuationStreams;

/* The user activity delegate is informed when the activity is being saved or continued (see NSUserActivityDelegate, below)
*/
@property (nullable, weak) id<NSUserActivityDelegate> delegate;

/*  A string that identifies the content of this NSUserActivity, for matching against existing documents when re-opening to see if they are the same.
    Setting this property is optional and does not automatically set .needsSave to YES.
 */
@property (nullable, copy) NSString* targetContentIdentifier API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* Marks the receiver as the activity currently in use by the user, for example, the activity associated with the active window. A newly created activity is eligible for continuation on another device after the first time it becomes current.
*/
- (void)becomeCurrent;

/* If this activity is the current activity, it should stop being so and set the current activity to nothing. */
- (void)resignCurrent API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* Invalidate an activity when it's no longer eligible for continuation, for example, when the window associated with an activity is closed. An invalid activity cannot become current.
*/
- (void)invalidate;

/* When an app is launched for a continuation event it can request streams back to the originating side. Streams can only be successfully retrieved from the NSUserActivity in the NS/UIApplication delegate that is called for a continuation event. This functionality is optional and is not expected to be needed in most continuation cases. The streams returned in the completion handler will be in an unopened state. The streams should be opened immediately to start requesting information from the other side.
*/
- (void)getContinuationStreamsWithCompletionHandler:(void (^)(NSInputStream * _Nullable inputStream, NSOutputStream * _Nullable outputStream, NSError * _Nullable error))completionHandler;

/* Set to YES if this user activity should be eligible to be handed off to another device */
@property (getter=isEligibleForHandoff) BOOL eligibleForHandoff API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* Set to YES if this user activity should be indexed by App History */
@property (getter=isEligibleForSearch) BOOL eligibleForSearch API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

/* Set to YES if this user activity should be eligible for indexing for any user of this application, on any device, or NO if the activity contains private or sensitive information or which would not be useful to other users if indexed.  The activity must also have requiredUserActivityKeys or a webpageURL */
@property (getter=isEligibleForPublicIndexing) BOOL eligibleForPublicIndexing API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0), tvos(10.0));

@property (getter=isEligibleForPrediction) BOOL eligibleForPrediction API_AVAILABLE( ios(12.0), watchos(5.0) ) API_UNAVAILABLE( macos, tvos );

@property (copy, nullable) NSUserActivityPersistentIdentifier persistentIdentifier  API_AVAILABLE( macos(10.15), ios(12.0), watchos(5.0) ) API_UNAVAILABLE( tvos );

+(void) deleteSavedUserActivitiesWithPersistentIdentifiers:(NSArray<NSUserActivityPersistentIdentifier>*) persistentIdentifiers completionHandler:(void(^)(void))handler API_AVAILABLE( macos(10.15), ios(12.0), watchos(5.0) ) API_UNAVAILABLE( tvos );
+(void) deleteAllSavedUserActivitiesWithCompletionHandler:(void(^)(void))handler API_AVAILABLE( macos(10.15), ios(12.0), watchos(5.0) ) API_UNAVAILABLE( tvos );

@end

/* The activity type used when continuing from a web browsing session to either a web browser or a native app. Only activities of this type can be continued from a web browser to a native app.
*/
FOUNDATION_EXPORT NSString * const NSUserActivityTypeBrowsingWeb;

/* The user activity delegate is responsible for updating the state of an activity and is also notified when an activity has been continued on another device.
*/
API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0), tvos(9.0))
@protocol NSUserActivityDelegate <NSObject>
@optional

/* The user activity will be saved (to be continued or persisted). The receiver should update the activity with current activity state.
*/
- (void)userActivityWillSave:(NSUserActivity *)userActivity;

/* The user activity was continued on another device.
*/
- (void)userActivityWasContinued:(NSUserActivity *)userActivity;

/* If supportsContinuationStreams is set to YES the continuing side can request streams back to this user activity. This delegate callback will be received with the incoming streams from the other side. The streams will be in an unopened state. The streams should be opened immediately to start receiving requests from the continuing side.
*/
- (void)userActivity:(NSUserActivity *)userActivity didReceiveInputStream:(NSInputStream *) inputStream outputStream:(NSOutputStream *)outputStream;

@end

NS_ASSUME_NONNULL_END

#endif
