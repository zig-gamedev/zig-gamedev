//
//  CKFetchDatabaseChangesOperation.h
//  CloudKit
//
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>

@class CKRecordZoneID, CKServerChangeToken;

NS_ASSUME_NONNULL_BEGIN

/*! @class CKFetchDatabaseChangesOperation
 *
 *  @abstract This operation will fetch changes to record zones within a database
 *
 *  @discussion If a change anchor from a previous @c CKFetchDatabaseChangesOperation is passed in, only the zones that have changed since that anchor will be returned.
 *  This per-database @c serverChangeToken is not to be confused with the per-recordZone @c serverChangeToken from @c CKFetchRecordZoneChangesOperation.
 *  If this is your first fetch or if you wish to re-fetch all zones, pass nil for the change token.
 *  Change token are opaque tokens and clients should not infer any behavior based on their content.
 *  @c CKFetchDatabaseChangesOperation is supported in a @c privateCloudDatabase and @c sharedCloudDatabase
 */
API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKFetchDatabaseChangesOperation : CKDatabaseOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithPreviousServerChangeToken:(nullable CKServerChangeToken *)previousServerChangeToken;

@property (nonatomic, copy, nullable) CKServerChangeToken *previousServerChangeToken;
@property (nonatomic, assign) NSUInteger resultsLimit;

/*! @discussion When set to YES, this operation will send repeated requests to the server until all record zone changes have been fetched.
 *
 *  @c changeTokenUpdatedBlock will be invoked periodically, to give clients an updated change token so that already-fetched record zone changes don't need to be re-fetched on a subsequent operation.
 *  When set to NO, it is the responsibility of the caller to issue subsequent fetch-changes operations when moreComing is YES in a @c fetchDatabaseChangesCompletionBlock invocation.
 *  @c fetchAllChanges is @c YES by default
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations
 */
@property (nonatomic, assign) BOOL fetchAllChanges;

@property (nonatomic, copy, nullable) void (^recordZoneWithIDChangedBlock)(CKRecordZoneID *zoneID);
@property (nonatomic, copy, nullable) void (^recordZoneWithIDWasDeletedBlock)(CKRecordZoneID *zoneID);

/*! @abstract If this block is set it will be called instead of @c recordZoneWithIDWasDeletedBlock if the user deleted this zone via the iCloud storage UI.
 *
 *  @discussion This is an indication that the user wanted all data deleted, so local cached data should be wiped and not re-uploaded to the server.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations
 */
@property (nonatomic, copy, nullable) void (^recordZoneWithIDWasPurgedBlock)(CKRecordZoneID *zoneID) API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0));

/*! @abstract If this block is set it will be called instead of @c recordZoneWithIDWasDeletedBlock if the user chose to reset all encrypted data for their account.
 *
 *  @discussion This is an indication that the user had to reset encrypted data during account recovery, so local cached data should be re-uploaded to the server to minimize data loss.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations
 */
@property (nonatomic, copy, nullable) void (^recordZoneWithIDWasDeletedDueToUserEncryptedDataResetBlock)(CKRecordZoneID *zoneID) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0));

//! @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
@property (nonatomic, copy, nullable) void (^changeTokenUpdatedBlock)(CKServerChangeToken * serverChangeToken);

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion Clients are responsible for saving the change token at the end of the operation and passing it in to the next call to @c CKFetchDatabaseChangesOperation.
 *  If the server returns a @c CKErrorChangeTokenExpired error, the @c previousServerChangeToken value was too old and the client should toss its local cache and re-fetch the changes in this record zone starting with a nil @c previousServerChangeToken.
 *  If @c moreComing is true then the server wasn't able to return all the changes in this response. Another @c CKFetchDatabaseChangesOperation operation should be run with the @c previousServerChangeToken token from this operation.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations
 */
@property (nonatomic, copy, nullable) void (^fetchDatabaseChangesCompletionBlock)(CKServerChangeToken * _Nullable serverChangeToken, BOOL moreComing, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use fetchDatabaseChangesResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
