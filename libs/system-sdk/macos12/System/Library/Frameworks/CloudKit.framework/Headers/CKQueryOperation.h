//
//  CKQueryOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDefines.h>
#import <CloudKit/CKDatabaseOperation.h>
#import <CloudKit/CKRecord.h>

@class CKQuery, CKRecord, CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKQueryCursor : NSObject <NSCopying, NSSecureCoding>
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
@end

/*! @discussion Query operations have a dynamically defined maximum number of results.  If the results of a query exceed this max, your completion block will invoked with a cursor.
 *  Issue a new query with that cursor to fetch the next batch of results.
 */
CK_EXTERN const NSUInteger CKQueryOperationMaximumResults API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0));

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKQueryOperation : CKDatabaseOperation

/*! Queries invoked within a sharedCloudDatabase must specify a zoneID.  Cross-zone queries are not supported in a sharedCloudDatabase */
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithQuery:(CKQuery *)query;
- (instancetype)initWithCursor:(CKQueryCursor *)cursor;

@property (nonatomic, copy, nullable) CKQuery *query;
@property (nonatomic, copy, nullable) CKQueryCursor *cursor;

/*! @abstract Indicates which record zone to query.
 *
 *  @discussion For query operations constructed using a cursor, this property is ignored and instead will be evaluated in the record zone in which the cursor was originally created.
 *  Queries that do not specify a @c zoneID will perform a query across all zones in the database.
 */
@property (nonatomic, copy, nullable) CKRecordZoneID *zoneID;

/*! @discussion Defaults to @c CKQueryOperationMaximumResults.
 *  Queries may return fewer than @c resultsLimit in some scenarios:
 *  - There are legitimately fewer than @c resultsLimit number of records matching the query (and visible to the current user).
 *  - During the process of querying and fetching the results, some records were deleted, or became un-readable by the current user.
 *  When determining if there are more records to fetch, always check for the presence of a cursor in @c queryCompletionBlock.
 */
@property (nonatomic, assign) NSUInteger resultsLimit;

/*! @abstract Declares which user-defined keys should be fetched and added to the resulting CKRecords.
 *
 *  @discussion If nil, declares the entire record should be downloaded. If set to an empty array, declares that no user fields should be downloaded.
 *  Defaults to @c nil.
 */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;

/*! @abstract This block will be called once for every record that is returned as a result of the query.
 *
 *  @discussion The callbacks will happen in the order that the results were sorted in.
 *  If the replacement callback @c recordMatchedBlock is set, this callback block is ignored.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^recordFetchedBlock)(CKRecord *record)

    API_DEPRECATED("Use recordMatchedBlock instead, which surfaces per-record errors", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));



/*! @abstract This block will be called once for every record that is returned as a result of the query.
 *
 *  @discussion The callbacks will happen in the order that the results were sorted in.  If a record fails in post-processing (say, a network failure materializing a @c CKAsset record field), the per-record error will be passed here.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^recordMatchedBlock)(CKRecordID *recordID, CKRecord * _Nullable record, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;


/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of recordIDs to errors keyed off of @c CKPartialErrorsByItemIDKey.  These errors are repeats of those sent back in previous @c recordMatchedBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^queryCompletionBlock)(CKQueryCursor * _Nullable cursor, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use queryResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
