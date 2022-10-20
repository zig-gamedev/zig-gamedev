//
//  CKFetchRecordZoneChangesOperation.h
//  CloudKit
//
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>
#import <CloudKit/CKRecord.h>
#import <CloudKit/CKServerChangeToken.h>

@class CKFetchRecordZoneChangesConfiguration, CKFetchRecordZoneChangesOptions;

NS_ASSUME_NONNULL_BEGIN

/*! @abstract This operation will fetch records changes across the given record zones
 *
 *  @discussion For each @c previousServerChangeToken passed in with a @c CKFetchRecordZoneChangesConfiguration, only records that have changed since that anchor will be fetched.
 *  If this is your first fetch of a zone or if you wish to re-fetch all records within a zone, do not include a @c previousServerChangeToken.
 *  Change tokens are opaque tokens and clients should not infer any behavior based on their content.
 */
API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKFetchRecordZoneChangesOperation : CKDatabaseOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecordZoneIDs:(NSArray<CKRecordZoneID *> *)recordZoneIDs configurationsByRecordZoneID:(nullable NSDictionary<CKRecordZoneID *, CKFetchRecordZoneChangesConfiguration *> *)configurationsByRecordZoneID API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0), watchos(5.0));

@property (nonatomic, copy, nullable) NSArray<CKRecordZoneID *> *recordZoneIDs;
@property (nonatomic, copy, nullable) NSDictionary<CKRecordZoneID *, CKFetchRecordZoneChangesConfiguration *> *configurationsByRecordZoneID API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0), watchos(5.0));

/*! @abstract Determines if the opertaion should fetch all changes from the server before completing.
 *
 *  @discussion When set to YES, this operation will send repeated requests to the server until all record changes have been fetched. @c recordZoneChangeTokensUpdatedBlock will be invoked periodically, to give clients an updated change token so that already-fetched record changes don't need to be re-fetched on a subsequent operation. @c recordZoneFetchCompletionBlock will only be called once and @c moreComing will always be NO.
 *
 *  When set to NO, it is the responsibility of the caller to issue subsequent fetch-changes operations when @c moreComing is YES in a @c recordZoneFetchCompletionBlock invocation.
 *
 *  @c fetchAllChanges is YES by default
 */
@property (nonatomic, assign) BOOL fetchAllChanges;

/*! @discussion If the replacement callback @c recordWasChangedBlock is set, this callback block is ignored.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^recordChangedBlock)(CKRecord *record)

    API_DEPRECATED("Use recordWasChangedBlock instead, which surfaces per-record errors", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));



/*! @discussion If a record fails in post-processing (say, a network failure materializing a @c CKAsset record field), the per-record error will be passed here.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^recordWasChangedBlock)(CKRecordID *recordID, CKRecord * _Nullable record, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;

//! @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
@property (nonatomic, copy, nullable) void (^recordWithIDWasDeletedBlock)(CKRecordID *recordID, CKRecordType recordType);

/*! @discussion Clients are responsible for saving this per-recordZone @c serverChangeToken and passing it in to the next call to @c CKFetchRecordZoneChangesOperation.
 *  Note that a fetch can fail partway. If that happens, an updated change token may be returned in this block so that already fetched records don't need to be re-downloaded on a subsequent operation.
 *  @c recordZoneChangeTokensUpdatedBlock will not be called after the last batch of changes in a zone; the @c recordZoneFetchCompletionBlock block will be called instead.
 *  The @c clientChangeTokenData from the most recent @c CKModifyRecordsOperation issued on this zone is also returned, or nil if none was provided.
 *  If the server returns a @c CKErrorChangeTokenExpired error, the @c serverChangeToken used for this record zone when initting this operation was too old and the client should toss its local cache and re-fetch the changes in this record zone starting with a nil @c serverChangeToken.
 *  @c recordZoneChangeTokensUpdatedBlock will not be called if @c fetchAllChanges is NO.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^recordZoneChangeTokensUpdatedBlock)(CKRecordZoneID *recordZoneID, CKServerChangeToken * _Nullable serverChangeToken, NSData * _Nullable clientChangeTokenData);
@property (nonatomic, copy, nullable) void (^recordZoneFetchCompletionBlock)(CKRecordZoneID *recordZoneID, CKServerChangeToken * _Nullable serverChangeToken, NSData * _Nullable clientChangeTokenData, BOOL moreComing, NSError * _Nullable recordZoneError) CK_SWIFT_DEPRECATED("Use recordZoneFetchResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion @c serverChangeToken-s previously returned via a @c recordZoneChangeTokensUpdatedBlock or @c recordZoneFetchCompletionBlock invocation, along with the record changes that preceded it, are valid even if there is a subsequent @c operationError
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of recordIDs and zoneIDs to errors keyed off of @c CKPartialErrorsByItemIDKey.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchRecordZoneChangesCompletionBlock)(NSError * _Nullable operationError) CK_SWIFT_DEPRECATED("Use fetchRecordZoneChangesResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

@end

@interface CKFetchRecordZoneChangesOperation(Deprecated)

- (instancetype)initWithRecordZoneIDs:(NSArray<CKRecordZoneID *> *)recordZoneIDs optionsByRecordZoneID:(nullable NSDictionary<CKRecordZoneID *, CKFetchRecordZoneChangesOptions *> *)optionsByRecordZoneID
API_DEPRECATED_WITH_REPLACEMENT("initWithRecordZoneIDs:configurationsByRecordZoneID:", macos(10.12, 10.14), ios(10.0, 12.0), tvos(10.0, 12.0), watchos(3.0, 5.0));

@property (nonatomic, copy, nullable) NSDictionary<CKRecordZoneID *, CKFetchRecordZoneChangesOptions *> *optionsByRecordZoneID
API_DEPRECATED_WITH_REPLACEMENT("configurationsByRecordZoneID", macos(10.12, 10.14), ios(10.0, 12.0), tvos(10.0, 12.0), watchos(3.0, 5.0));
@end


API_AVAILABLE(macos(10.14), ios(12.0), tvos(12.0), watchos(5.0))
@interface CKFetchRecordZoneChangesConfiguration : NSObject <NSSecureCoding, NSCopying>

@property (nonatomic, copy, nullable) CKServerChangeToken *previousServerChangeToken;

@property (nonatomic, assign) NSUInteger resultsLimit;

/*! @abstract Declares which user-defined keys should be fetched and added to the resulting CKRecords.
 *
 *  @discussion If nil, declares the entire record should be downloaded. If set to an empty array, declares that no user fields should be downloaded.
 *  Defaults to @c nil.
 */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;
@end



API_DEPRECATED_WITH_REPLACEMENT("CKFetchRecordZoneChangesConfiguration", macos(10.12, 10.14), ios(10.0, 12.0), tvos(10.0, 12.0), watchos(3.0, 5.0))
@interface CKFetchRecordZoneChangesOptions : NSObject <NSSecureCoding, NSCopying>
@property (nonatomic, copy, nullable) CKServerChangeToken *previousServerChangeToken;
@property (nonatomic, assign) NSUInteger resultsLimit;
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;
@end

NS_ASSUME_NONNULL_END
