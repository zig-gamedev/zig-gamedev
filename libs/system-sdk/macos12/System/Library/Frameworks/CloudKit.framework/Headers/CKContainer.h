//
//  CKContainer.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKDatabase.h>
#import <CloudKit/CKDefines.h>
#import <CloudKit/CKOperation.h>

@class CKRecordID, CKUserIdentity, CKShareParticipant, CKShare, CKShareMetadata;

NS_ASSUME_NONNULL_BEGIN

/*! Stand-in for the current user's ID; most often used in RecordZoneID->ownerName */
CK_EXTERN NSString * const CKCurrentUserDefaultName API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

CK_EXTERN NSString * const CKOwnerDefaultName API_DEPRECATED_WITH_REPLACEMENT("CKCurrentUserDefaultName", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0), watchos(3.0, 3.0));

/*! @class CKContainer
 *
 *  @abstract A CKContainer, and its CKDatabases, are the main entry points into the CloudKit framework.
 *
 *  @discussion
 *  Several methods in CloudKit accept completion handlers to indicate when they're completed.
 *  All CKOperation subclasses include progress and completion blocks to report significant events in their lifecycles.
 *  Each of these handlers and blocks is invoked on a non-main serial queue.  The receiver is responsible for handling the message on a different queue or thread if it is required.
 */
API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKContainer : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! @abstract Convenience method that uses the calling process' "iCloud.\(application-identifier)" as the container identifier
 *
 *  @discussion
 *  application-identifier is the calling process' @c application-identifier entitlement on iOS / tvOS / watchOS.
 *  application-identifier is the calling process' @c com.apple.application-identifier entitlement on macOS.
 *  On all OSes, if an @c com.apple.developer.associated-application-identifier entitlement is present, its value will be preferred over the @c application-identifier variants.
 */
+ (CKContainer *)defaultContainer;

/*! @abstract Obtain a CKContainer for the given containerIdentifier
 *
 *  @discussion If the application is in production mode (aka, @c com.apple.developer.icloud-container-environment is set to Production in your entitlements plist, and you have no override in @c com.apple.developer.icloud-container-development-container-identifiers), then the production environment is used.
 */
+ (CKContainer *)containerWithIdentifier:(NSString *)containerIdentifier;

@property (nonatomic, readonly, copy, nullable) NSString *containerIdentifier;

- (void)addOperation:(CKOperation *)operation;

@end

/*! @discussion
 *  Database properties:
 *  Records in a public database
 *  - By default are world readable, owner writable.
 *  - Can be locked down by Roles, a process done in the Developer Portal, a web interface.  Roles are not present in the client API.
 *  - Are visible to the application developer via the Developer Portal.
 *  - Do not contribute to the owner's iCloud account storage quota.
 *  Records in a private database
 *  - By default are only owner readable and owner writable.
 *  - Are not visible to the application developer via the Developer Portal.
 *  - Are counted towards the owner's iCloud account storage quota.
 *  Records in a shared database
 *  - Are available to share participants based on the permissions of the enclosing CKShare
 *  - Are not visible to the application developer via the Developer Portal.
 *  - Are counted towards the originating owner's iCloud account storage quota.
 */
@interface CKContainer (Database)

@property (nonatomic, readonly) CKDatabase *privateCloudDatabase;
@property (nonatomic, readonly) CKDatabase *publicCloudDatabase;
@property (nonatomic, readonly) CKDatabase *sharedCloudDatabase API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

/*! @abstract Convenience methods
 *
 *  @return a database that's pointer-equal to one of the above properties@enum 
 */
- (CKDatabase *)databaseWithDatabaseScope:(CKDatabaseScope)databaseScope API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@end

/*! @enum CKAccountStatus
 *  @constant CKAccountStatusCouldNotDetermine An error occurred when getting the account status, consult the corresponding NSError.
 *  @constant CKAccountStatusAvailable The iCloud account credentials are available for this application
 *  @constant CKAccountStatusRestricted Parental Controls / Device Management has denied access to iCloud account credentials
 *  @constant CKAccountStatusNoAccount No iCloud account is logged in on this device
 *  @constant CKAccountStatusTemporarilyUnavailable An iCloud account is logged in but not ready. The user can be asked to verify their
 *  credentials in Settings app.
 */
typedef NS_ENUM(NSInteger, CKAccountStatus) {
    CKAccountStatusCouldNotDetermine                                                                      = 0,
    CKAccountStatusAvailable                                                                              = 1,
    CKAccountStatusRestricted                                                                             = 2,
    CKAccountStatusNoAccount                                                                              = 3,
    CKAccountStatusTemporarilyUnavailable API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) = 4
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/*! @abstract This local notification is posted when there has been any change to the logged in iCloud account.
 *
 *  @discussion On receipt, an updated account status should be obtained by calling @c accountStatusWithCompletionHandler:
 */
CK_EXTERN NSString * const CKAccountChangedNotification API_AVAILABLE(macos(10.11), ios(9.0), watchos(3.0));

@interface CKContainer (AccountStatus)

- (void)accountStatusWithCompletionHandler:(void (^)(CKAccountStatus accountStatus, NSError * _Nullable error))completionHandler;

@end

typedef NS_OPTIONS(NSUInteger, CKApplicationPermissions) {
    /*! Allows the user's record in CloudKit to be discoverable via the user's email address */
    CKApplicationPermissionUserDiscoverability         = 1 << 0,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

/*! @enum CKApplicationPermissionStatus
 *  @constant CKApplicationPermissionStatusInitialState The user has not made a decision for this application permission.
 *  @constant CKApplicationPermissionStatusCouldNotComplete An error occurred when getting or setting the application permission status, consult the corresponding NSError
 *  @constant CKApplicationPermissionStatusDenied The user has denied this application permission
 *  @constant CKApplicationPermissionStatusGranted The user has granted this application permission
 */
typedef NS_ENUM(NSInteger, CKApplicationPermissionStatus) {
    CKApplicationPermissionStatusInitialState          = 0,
    CKApplicationPermissionStatusCouldNotComplete      = 1,
    CKApplicationPermissionStatusDenied                = 2,
    CKApplicationPermissionStatusGranted               = 3,
} API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

typedef void(^CKApplicationPermissionBlock)(CKApplicationPermissionStatus applicationPermissionStatus, NSError * _Nullable error);

@interface CKContainer (ApplicationPermission)

- (void)statusForApplicationPermission:(CKApplicationPermissions)applicationPermission completionHandler:(CKApplicationPermissionBlock)completionHandler NS_SWIFT_ASYNC_NAME(applicationPermissionStatus(for:));
- (void)requestApplicationPermission:(CKApplicationPermissions)applicationPermission completionHandler:(CKApplicationPermissionBlock)completionHandler;

@end

@interface CKContainer (UserRecords)

/*! @discussion If there is no iCloud account configured, or if access is restricted, a @c CKErrorNotAuthenticated error will be returned.
 *
 *  This work is treated as having @c NSQualityOfServiceUserInitiated quality of service.
 */
- (void)fetchUserRecordIDWithCompletionHandler:(void (^)(CKRecordID * _Nullable recordID, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(userRecordID());

/*! @abstract Fetches all user identities that match an entry in the user's contacts database.
 *
 *  @discussion @c CKDiscoverAllUserIdentitiesOperation is the more configurable, @c CKOperation -based alternative to this methods
 */
- (void)discoverAllIdentitiesWithCompletionHandler:(void (^)(NSArray<CKUserIdentity *> * _Nullable userIdentities, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), watchos(3.0)) API_UNAVAILABLE(tvos) NS_SWIFT_ASYNC_NAME(allUserIdentitiesFromContacts());

/*! @abstract Fetches the user identity that corresponds to the given email address.
 *
 *  @discussion Only users who have opted-in to user discoverability will have their identities returned by this method.  If a user with the inputted email exists in iCloud, but has not opted-in to user discoverability, this method completes with a nil @c userInfo.  @c CKDiscoverUserIdentitiesOperation is the more configurable, @c CKOperation -based alternative to this method
 */
- (void)discoverUserIdentityWithEmailAddress:(NSString *)email completionHandler:(void (^)(CKUserIdentity * _Nullable_result userInfo, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(userIdentity(forEmailAddress:));

/*! @abstract Fetches the user identity that corresponds to the given phone number.
 *
 *  @discussion Only users who have opted-in to user discoverability will have their identities returned by this method.  If a user with the inputted phone number exists in iCloud, but has not opted-in to user discoverability, this method completes with a nil @c userInfo.  @c CKDiscoverUserIdentitiesOperation is the more configurable, @c CKOperation -based alternative to this method
 */
- (void)discoverUserIdentityWithPhoneNumber:(NSString *)phoneNumber completionHandler:(void (^)(CKUserIdentity * _Nullable_result userInfo, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(userIdentity(forPhoneNumber:));

/*! @abstract Fetches the user identity that corresponds to the given user record id.
 *
 *  @discussion Only users who have opted-in to user discoverability will have their identities returned by this method.  If a user has not opted-in to user discoverability, this method completes with a nil @c userInfo.  @c CKDiscoverUserIdentitiesOperation is the more configurable, @c CKOperation -based alternative to this method
 */
- (void)discoverUserIdentityWithUserRecordID:(CKRecordID *)userRecordID completionHandler:(void (^)(CKUserIdentity * _Nullable_result userInfo, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(userIdentity(forUserRecordID:));

@end

@interface CKContainer (Sharing)

/*! @abstract Fetches share participants matching the provided info.
 *
 *  @discussion @c CKFetchShareParticipantsOperation is the more configurable, @c CKOperation -based alternative to these methods.
 */
- (void)fetchShareParticipantWithEmailAddress:(NSString *)emailAddress completionHandler:(void (^)(CKShareParticipant * _Nullable shareParticipant, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(shareParticipant(forEmailAddress:));
- (void)fetchShareParticipantWithPhoneNumber:(NSString *)phoneNumber completionHandler:(void (^)(CKShareParticipant * _Nullable shareParticipant, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(shareParticipant(forPhoneNumber:));
- (void)fetchShareParticipantWithUserRecordID:(CKRecordID *)userRecordID completionHandler:(void (^)(CKShareParticipant *_Nullable shareParticipant, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(shareParticipant(forUserRecordID:));

- (void)fetchShareMetadataWithURL:(NSURL *)url completionHandler:(void (^)(CKShareMetadata *_Nullable metadata, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0)) NS_SWIFT_ASYNC_NAME(shareMetadata(for:));
- (void)acceptShareMetadata:(CKShareMetadata *)metadata completionHandler:(void (^)(CKShare *_Nullable acceptedShare, NSError *_Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

@end

@interface CKContainer (CKLongLivedOperations)
/*! @discussion Long lived CKOperations returned by this call must be started on an operation queue.
 *  Remember to set the callback blocks before starting the operation.
 *  If an operation has already completed against the server, and is subsequently resumed, that operation will replay all of its callbacks from the start of the operation, but the request will not be re-sent to the server.
 *  If a long lived operation is cancelled or finishes completely it is no longer returned by these calls.
 */
- (void)fetchAllLongLivedOperationIDsWithCompletionHandler:(void (^)(NSArray<CKOperationID> * _Nullable outstandingOperationIDs, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(9.3), tvos(9.2), watchos(3.0)) NS_REFINED_FOR_SWIFT_ASYNC(1);
- (void)fetchLongLivedOperationWithID:(CKOperationID)operationID completionHandler:(void (^)(CKOperation * _Nullable_result outstandingOperation, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.12), ios(9.3), tvos(9.2), watchos(3.0)) NS_REFINED_FOR_SWIFT_ASYNC(2);
@end

NS_ASSUME_NONNULL_END
