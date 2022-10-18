//
//  CKFetchRecordZonesOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>

@class CKRecordZone, CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKFetchRecordZonesOperation : CKDatabaseOperation

+ (instancetype)fetchAllRecordZonesOperation;

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecordZoneIDs:(NSArray<CKRecordZoneID *> *)zoneIDs;

@property (nonatomic, copy, nullable) NSArray<CKRecordZoneID *> *recordZoneIDs;

/*! @abstract Called on success or failure for each record zone.
 *
 * @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perRecordZoneCompletionBlock)(CKRecordZoneID *zoneID, CKRecordZone * _Nullable recordZone, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;


/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of zoneIDs to errors keyed off of @c CKPartialErrorsByItemIDKey.
 *  @c recordZonesByZoneID and any @c CKPartialErrorsByItemIDKey errors are repeats of the data sent back in previous @c perRecordZoneCompletionBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations
 */
@property (nonatomic, copy, nullable) void (^fetchRecordZonesCompletionBlock)(NSDictionary<CKRecordZoneID *, CKRecordZone *> * _Nullable recordZonesByZoneID, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use fetchRecordZonesResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
