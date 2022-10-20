//
//  CKFetchShareParticipantsOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKOperation.h>

@class CKUserIdentityLookupInfo, CKShareParticipant, CKRecordID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKFetchShareParticipantsOperation : CKOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithUserIdentityLookupInfos:(NSArray<CKUserIdentityLookupInfo *> *)userIdentityLookupInfos;

@property (nonatomic, copy, nullable) NSArray<CKUserIdentityLookupInfo *> *userIdentityLookupInfos;

/*! @abstract Called once for each share participant created from a submitted user identity lookup info.
 *
 *  @discussion If the replacement callback @c perShareParticipantCompletionBlock is set, this callback block is ignored.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^shareParticipantFetchedBlock)(CKShareParticipant *participant)

    API_DEPRECATED("Use perShareParticipantCompletionBlock instead, which surfaces per-share-participant errors", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));


/*! @abstract Called once for each lookup info.
 *
 *  @discussion Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perShareParticipantCompletionBlock)(CKUserIdentityLookupInfo *lookupInfo, CKShareParticipant * _Nullable participant, NSError * _Nullable error) NS_REFINED_FOR_SWIFT;

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of lookup infos to errors keyed off of @c CKPartialErrorsByItemIDKey.  These errors are repeats of those sent back in previous @c perShareParticipantCompletionBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^fetchShareParticipantsCompletionBlock)(NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use fetchShareParticipantsResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
