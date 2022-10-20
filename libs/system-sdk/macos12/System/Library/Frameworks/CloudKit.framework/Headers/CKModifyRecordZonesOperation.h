//
//  CKModifyRecordZonesOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>

@class CKRecordZone, CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKModifyRecordZonesOperation : CKDatabaseOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecordZonesToSave:(nullable NSArray<CKRecordZone *> *)recordZonesToSave recordZoneIDsToDelete:(nullable NSArray<CKRecordZoneID *> *)recordZoneIDsToDelete;

@property (nonatomic, copy, nullable) NSArray<CKRecordZone *> *recordZonesToSave;
@property (nonatomic, copy, nullable) NSArray<CKRecordZoneID *> *recordZoneIDsToDelete;

/*! @abstract Called on success or failure of a record zone save
 *
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perRecordZoneSaveBlock)(CKRecordZoneID *recordZoneID, CKRecordZone * _Nullable recordZone, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;

/*! @abstract Called on success or failure of a record zone deletion
 *
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perRecordZoneDeleteBlock)(CKRecordZoneID *recordZoneID, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of recordZoneIDs to errors keyed off of @c CKPartialErrorsByItemIDKey.
 *  @c savedRecordZones, @c deletedRecordZoneIDs and any @c CKPartialErrorsByItemIDKey errors are repeats of the data sent back in previous @c perRecordZoneSaveBlock and @c perRecordZoneDeleteBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^modifyRecordZonesCompletionBlock)(NSArray<CKRecordZone *> * _Nullable savedRecordZones, NSArray<CKRecordZoneID *> * _Nullable deletedRecordZoneIDs, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use modifyRecordZonesResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
