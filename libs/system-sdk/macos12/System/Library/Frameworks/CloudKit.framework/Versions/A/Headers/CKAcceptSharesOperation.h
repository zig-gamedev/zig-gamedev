//
//  CKAcceptSharesOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKDatabaseOperation.h>

@class CKShare, CKShareMetadata;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKAcceptSharesOperation : CKOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithShareMetadatas:(NSArray<CKShareMetadata *> *)shareMetadatas;

@property (nonatomic, copy, nullable) NSArray<CKShareMetadata *> *shareMetadatas;

/*! @abstract Called once for each share metadata that the server processed
 *
 *  @discussion If error is nil then the share was successfully accepted.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^perShareCompletionBlock)(CKShareMetadata *shareMetadata, CKShare * _Nullable acceptedShare, NSError * _Nullable error)
CK_SWIFT_DEPRECATED("Use perShareResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

/*! @abstract This block is called when the operation completes.
 *
 *  @discussion The @code -[NSOperation completionBlock] @endcode will also be called if both are set.
 *  If the error is @c CKErrorPartialFailure, the error's userInfo dictionary contains a dictionary of shareURLs to errors keyed off of @c CKPartialErrorsByItemIDKey.  These errors are repeats of those sent back in previous @c perShareCompletionBlock invocations
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^acceptSharesCompletionBlock)(NSError * _Nullable operationError)
CK_SWIFT_DEPRECATED("Use acceptSharesResultBlock instead", macos(10.12, 12.0), ios(10.0, 15.0), tvos(10.0, 15.0), watchos(3.0, 8.0));

@end

NS_ASSUME_NONNULL_END
