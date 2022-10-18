//
//  CKDatabase.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKSubscription.h>

@class CKDatabaseOperation, CKRecord, CKRecordID, CKRecordZone, CKRecordZoneID, CKQuery;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, CKDatabaseScope) {
    CKDatabaseScopePublic = 1,
    CKDatabaseScopePrivate,
    CKDatabaseScopeShared,
} API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKDatabase : NSObject
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (void)addOperation:(CKDatabaseOperation *)operation;
@property (nonatomic, readonly, assign) CKDatabaseScope databaseScope API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0));
@end

/*! @abstract Convenience APIs
 *
 *  @discussion These calls operate on a single item in the default zone and allow for simple operations.
 *  If you'd like to batch your requests, add dependencies between requests, set priorities, or schedule operations on your own queue, take a look at the corresponding @c CKOperation.
 *  This work is treated as having @c NSQualityOfServiceUserInitiated quality of service.
 */
@interface CKDatabase (ConvenienceMethods)

#pragma mark Record Convenience Methods
/*! @c CKFetchRecordsOperation and @c CKModifyRecordsOperation are the more configurable, @c CKOperation -based alternatives to these methods */
- (void)fetchRecordWithID:(CKRecordID *)recordID completionHandler:(void (^)(CKRecord * _Nullable record, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(record(for:));
- (void)saveRecord:(CKRecord *)record completionHandler:(void (^)(CKRecord * _Nullable record, NSError * _Nullable error))completionHandler;
- (void)deleteRecordWithID:(CKRecordID *)recordID completionHandler:(void (^)(CKRecordID * _Nullable recordID, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(deleteRecord(withID:));

#pragma mark Query Convenience Method
/*! @discussion @c CKQueryOperation is the more configurable, @c CKOperation -based alternative to this method
 *  Queries can potentially return a large number of records, and the server will return those records in batches. This convenience API will only fetch the first batch of results (equivalent to using @c CKQueryOperationMaximumResults).
 *  If you would like to fetch all results, use @c CKQueryOperation and its @c CKQueryCursor instead.
 *  Queries invoked within a @c sharedCloudDatabase must specify a @c zoneID.  Cross-zone queries are not supported in a @c sharedCloudDatabase
 *  Queries that do not specify a @c zoneID will perform a query across all zones in the database.
 */
- (void)performQuery:(CKQuery *)query inZoneWithID:(nullable CKRecordZoneID *)zoneID completionHandler:(void (^)(NSArray<CKRecord *> * _Nullable results, NSError * _Nullable error))completionHandler
CK_SWIFT_DEPRECATED("renamed to fetchRecords(matching:inZoneWith:desiredKeys:resultsLimit:completionHandler:)", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));

#pragma mark Record Zone Convenience Methods
/*! @c CKFetchRecordZonesOperation and @c CKModifyRecordZonesOperation are the more configurable, @c CKOperation -based alternatives to these methods */
- (void)fetchAllRecordZonesWithCompletionHandler:(void (^)(NSArray<CKRecordZone *> * _Nullable zones, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(allRecordZones());
- (void)fetchRecordZoneWithID:(CKRecordZoneID *)zoneID completionHandler:(void (^)(CKRecordZone * _Nullable zone, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(recordZone(for:));
- (void)saveRecordZone:(CKRecordZone *)zone completionHandler:(void (^)(CKRecordZone * _Nullable zone, NSError * _Nullable error))completionHandler;
- (void)deleteRecordZoneWithID:(CKRecordZoneID *)zoneID completionHandler:(void (^)(CKRecordZoneID * _Nullable zoneID, NSError * _Nullable error))completionHandler NS_SWIFT_ASYNC_NAME(deleteRecordZone(withID:));

#pragma mark Subscription Convenience Methods
/*! @c CKFetchSubscriptionsOperation and @c CKModifySubscriptionsOperation are the more configurable, @c CKOperation -based alternative to these methods */
- (void)fetchSubscriptionWithID:(CKSubscriptionID)subscriptionID completionHandler:(void (^)(CKSubscription * _Nullable subscription, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), tvos(9.0), watchos(6.0)) NS_REFINED_FOR_SWIFT_ASYNC(2);
- (void)fetchAllSubscriptionsWithCompletionHandler:(void (^)(NSArray<CKSubscription *> * _Nullable subscriptions, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), tvos(9.0), watchos(6.0)) NS_SWIFT_ASYNC_NAME(allSubscriptions());
- (void)saveSubscription:(CKSubscription *)subscription completionHandler:(void (^)(CKSubscription * _Nullable subscription, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), tvos(9.0), watchos(6.0));
- (void)deleteSubscriptionWithID:(CKSubscriptionID)subscriptionID completionHandler:(void (^)(CKSubscriptionID _Nullable subscriptionID, NSError * _Nullable error))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), tvos(9.0), watchos(6.0)) NS_REFINED_FOR_SWIFT_ASYNC(2);

@end

NS_ASSUME_NONNULL_END
