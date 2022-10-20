//
//  CKFetchSubscriptionsOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKDatabaseOperation.h>
#import <CloudKit/CKDefines.h>
#import <CloudKit/CKSubscription.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(6.0))
@interface CKFetchSubscriptionsOperation : CKDatabaseOperation

+ (instancetype)fetchAllSubscriptionsOperation;

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithSubscriptionIDs:(NSArray<CKSubscriptionID> *)subscriptionIDs;

@property (nonatomic, copy, nullable) NSArray<CKSubscriptionID> *subscriptionIDs;

/*! @abstract Called on success or failure for each subscriptionID.
 *
 * @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perSubscriptionCompletionBlock)(CKSubscriptionID subscriptionID, CKSubscription * _Nullable subscription, NSError * _Nullable error) API_AVAILABLE(macos(12.0), ios(15.0), tvos(15.0), watchos(8.0)) NS_REFINED_FOR_SWIFT;

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of subscriptionID to errors keyed off of @c CKPartialErrorsByItemIDKey.
 *  @c subscriptionsBySubscriptionID and any @c CKPartialErrorsByItemIDKey errors are repeats of the data sent back in previous @c perSubscriptionCompletionBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchSubscriptionCompletionBlock)(NSDictionary<CKSubscriptionID, CKSubscription *> * _Nullable subscriptionsBySubscriptionID, NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use fetchSubscriptionsResultBlock instead", macos(10.10, 12.0), ios(8.0, 15.0), tvos(9.0, 15.0), watchos(6.0, 8.0));

@end

NS_ASSUME_NONNULL_END
