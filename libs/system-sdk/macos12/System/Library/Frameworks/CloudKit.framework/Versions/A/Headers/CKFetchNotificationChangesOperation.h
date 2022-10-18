//
//  CKFetchNotificationChangesOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>

@class CKNotification, CKServerChangeToken;

NS_ASSUME_NONNULL_BEGIN

/*! @class CKFetchNotificationChangesOperation
 *
 *  @abstract An operation that fetches all notification changes.
 *
 *  @discussion If a change token from a previous @c CKFetchNotificationChangesOperation is passed in, only the notifications that have changed since that token will be fetched.
 *  If this is your first fetch, pass nil for the change token.
 *  Change tokens are opaque tokens and clients should not infer any behavior based on their content.
 */
API_DEPRECATED("Instead of iterating notifications to enumerate changed record zones, use CKDatabaseSubscription, CKFetchDatabaseChangesOperation, and CKFetchRecordZoneChangesOperation", macos(10.10, 10.13), ios(8.0, 11.0), tvos(9.0, 11.0), watchos(3.0, 4.0))
@interface CKFetchNotificationChangesOperation : CKOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithPreviousServerChangeToken:(nullable CKServerChangeToken *)previousServerChangeToken;

@property (nonatomic, copy, nullable) CKServerChangeToken *previousServerChangeToken;

@property (nonatomic, assign) NSUInteger resultsLimit;

/*! @abstract If true, then the server wasn't able to return all the changes in this response.
 *
 *  @discussion Will be set before @c fetchNotificationChangesCompletionBlock is called.
 *  Another @c CKFetchNotificationChangesOperation operation should be run with the updated @c serverChangeToken token from this operation.
 */
@property (nonatomic, readonly, assign) BOOL moreComing;

/*! @abstract Called once for each updated notification fetch from the server
 *
 *  @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^notificationChangedBlock)(CKNotification *notification);

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion Clients are responsible for saving the change token at the end of the operation and passing it in to the next call to @c CKFetchNotificationChangesOperation.
 *  Note that a fetch can fail partway. If that happens, an updated change token may be returned in the completion block so that already fetched notifications don't need to be re-downloaded on a subsequent operation.
 *  If the server returns a @c CKErrorChangeTokenExpired error, the @c previousServerChangeToken value was too old and the client should toss its local cache and re-fetch notification changes starting with a nil @c previousServerChangeToken.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchNotificationChangesCompletionBlock)(CKServerChangeToken * _Nullable serverChangeToken, NSError * _Nullable operationError);

@end

NS_ASSUME_NONNULL_END
