//
//  CKFetchRecordsOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>
#import <CloudKit/CKRecord.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKFetchRecordsOperation : CKDatabaseOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecordIDs:(NSArray<CKRecordID *> *)recordIDs;

+ (instancetype)fetchCurrentUserRecordOperation;

@property (nonatomic, copy, nullable) NSArray<CKRecordID *> *recordIDs;

/*! @abstract Declares which user-defined keys should be fetched and added to the resulting CKRecords.
 *
 *  @discussion If nil, declares the entire record should be downloaded. If set to an empty array, declares that no user fields should be downloaded.
 *  Defaults to @c nil.
 */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;

/*! @abstract Indicates the progress for each record.
 *
 *  @discussion This method is called at least once with a progress of 1.0 for every record. Intermediate progress is only reported for records that contain assets.
 *  It is possible for progress to regress when a retry is automatically triggered.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perRecordProgressBlock)(CKRecordID *recordID, double progress);

/*! @abstract Called on success or failure for each record.
 *
 * @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perRecordCompletionBlock)(CKRecord * _Nullable record, CKRecordID * _Nullable recordID, NSError * _Nullable error)
CK_SWIFT_DEPRECATED("Use perRecordResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));


/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of recordIDs to errors keyed off of @c CKPartialErrorsByItemIDKey.
 *  @c recordsByRecordID and any @c CKPartialErrorsByItemIDKey errors are repeats of the data sent back in previous @c perRecordCompletionBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchRecordsCompletionBlock)(NSDictionary<CKRecordID * , CKRecord *> * _Nullable recordsByRecordID, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use fetchRecordsResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
