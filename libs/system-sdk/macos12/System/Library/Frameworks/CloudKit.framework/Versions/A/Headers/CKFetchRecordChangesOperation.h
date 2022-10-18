//
//  CKFetchRecordChangesOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>
#import <CloudKit/CKRecord.h>
#import <CloudKit/CKServerChangeToken.h>

NS_ASSUME_NONNULL_BEGIN

/*! @class CKFetchRecordChangesOperation
 *
 *  @discussion Use CKFetchRecordZoneChangesOperation instead of this class.
 *
 *  Any serverChangeTokens saved from a CKFetchRecordChangesOperation are usable as a serverRecordZoneChangeToken in CKFetchRecordZoneChangesOperation
 *
 *  This operation will fetch records changes in the given record zone.
 *
 *  If a change token from a previous @c CKFetchRecordChangesOperation is passed in, only the records that have changed since that token will be fetched.
 *  If this is your first fetch or if you wish to re-fetch all records, pass nil for the change token.
 *  Change tokens are opaque tokens and clients should not infer any behavior based on their content
 */
API_DEPRECATED_WITH_REPLACEMENT("CKFetchRecordZoneChangesOperation", macos(10.10, 10.12), ios(8.0, 10.0), tvos(9.0, 10.0), watchos(3.0, 3.0))
@interface CKFetchRecordChangesOperation : CKDatabaseOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithRecordZoneID:(CKRecordZoneID *)recordZoneID previousServerChangeToken:(nullable CKServerChangeToken *)previousServerChangeToken;

@property (nonatomic, copy, nullable) CKRecordZoneID *recordZoneID;
@property (nonatomic, copy, nullable) CKServerChangeToken *previousServerChangeToken;

@property (nonatomic, assign) NSUInteger resultsLimit;

/*! @abstract Declares which user-defined keys should be fetched and added to the resulting CKRecords.
 *
 *  @discussion If nil, declares the entire record should be downloaded. If set to an empty array, declares that no user fields should be downloaded.
 *  Defaults to @c nil.
 */
@property (nonatomic, copy, nullable) NSArray<CKRecordFieldKey> *desiredKeys;

//! @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
@property (nonatomic, copy, nullable) void (^recordChangedBlock)(CKRecord *record);

//! @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
@property (nonatomic, copy, nullable) void (^recordWithIDWasDeletedBlock)(CKRecordID *recordID);

/*! @abstract If true, then the server wasn't able to return all the changes in this response.
 *
 *  @discussion Will be set before fetchRecordChangesCompletionBlock is called.
 *  Another CKFetchRecordChangesOperation operation should be run with the updated serverChangeToken token from this operation.
 */
@property (nonatomic, readonly, assign) BOOL moreComing;

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion Clients are responsible for saving the change token at the end of the operation and passing it in to the next call to @c CKFetchRecordChangesOperation.
 *  Note that a fetch can fail partway. If that happens, an updated change token may be returned in the completion block so that already fetched records don't need to be re-downloaded on a subsequent operation.
 *  The @c clientChangeTokenData from the most recent @c CKModifyRecordsOperation is also returned, or nil if none was provided.
 *  If the server returns a @c CKErrorChangeTokenExpired error, the @c previousServerChangeToken value was too old and the client should toss its local cache and re-fetch the changes in this record zone starting with a nil @c previousServerChangeToken.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchRecordChangesCompletionBlock)(CKServerChangeToken * _Nullable serverChangeToken, NSData * _Nullable clientChangeTokenData, NSError * _Nullable operationError);

@end

NS_ASSUME_NONNULL_END
