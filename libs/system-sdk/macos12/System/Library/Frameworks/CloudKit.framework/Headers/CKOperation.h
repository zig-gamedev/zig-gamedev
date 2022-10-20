//
//  CKOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKDefines.h>

@class CKContainer, CKOperationConfiguration, CKOperationGroup;

NS_ASSUME_NONNULL_BEGIN

typedef NSString *CKOperationID;

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKOperation : NSOperation

- (instancetype)init NS_DESIGNATED_INITIALIZER;

/*! @abstract This defines per-operation configuration settings.
 *
 *  @discussion See the CKOperationConfiguration class description for info on how this configuration composes with CKOperationGroup.defaultConfiguration
 */
@property (nonatomic, copy, null_resettable) CKOperationConfiguration *configuration API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0));

/*! @abstract The group this operation is associated with
 */
@property (nonatomic, strong, nullable) CKOperationGroup *group API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0));

/*! @abstract This is an identifier unique to this CKOperation.
 *
 *  @discussion This value is chosen by the system, and will be unique to this instance of a CKOperation.  This identifier will be sent to Apple's servers, and can be used to identify any server-side logging associated with this operation.
 */
@property (nonatomic, readonly, copy) CKOperationID operationID API_AVAILABLE(macos(10.12), ios(9.3), tvos(9.2), watchos(3.0));

/*! @abstract This callback is called after a long lived operation has begun running and is persisted.
 *
 *  @discussion Once this callback is called the operation will continue running even if the current process exits.
 *  Each @c CKOperation instance has a private serial queue. This queue is used for all callback block invocations.
 */
@property (nonatomic, copy, nullable) void (^longLivedOperationWasPersistedBlock)(void) API_AVAILABLE(macos(10.12), ios(9.3), tvos(9.2), watchos(3.0));

@end

/*! @class CKOperationConfiguration
 *
 *  @discussion An operation configuration is a set of properties that describes how your operation should behave.  All properties have a default value.  When determining what properties to apply to an operation, we consult the operation's configuration property, as well as the operation->group->defaultConfiguration property.  We combine them following these rules:
 *  @code
 *   Group Default Configuration Value | Operation Configuration Value |        Value Applied To Operation
 *  -----------------------------------+-------------------------------+-----------------------------------------
 *             default value           |         default value         |                  default value
 *             default value           |         explicit value        |       operation.configuration explicit value
 *             explicit value          |         default value         | operation.group.defaultConfiguration explicit value
 *             explicit value          |         explicit value        |       operation.configuration explicit value
 *  @endcode
 *  For example:
 *  CKOperationGroup -> defaultConfiguration -> allowsCellularAccess explicitly set to NO
 *  + CKOperation -> configuration -> allowsCellularAccess has default value of YES
 *  = disallow cellular access
 *
 *  CKOperationGroup -> defaultConfiguration -> allowsCellularAccess explicitly set to NO
 *  + CKOperation -> configuration -> allowsCellularAccess explicitly set to YES
 *  = allow cellular access
 */
API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0))
@interface CKOperationConfiguration : NSObject

/*! If no container is set, [CKContainer defaultContainer] is used */
@property (nonatomic, strong, nullable) CKContainer *container;

/*! @discussion CKOperations behave differently depending on how you set qualityOfService.
 *
 *  @code
 *  Quality of Service | timeoutIntervalForResource | Network Error Behavior | Discretionary Behavior
 *  -------------------+----------------------------+------------------------+-----------------------
 *  UserInteractive    | -1 (no enforcement)        | fail                   | nonDiscretionary
 *  UserInitiated      | -1 (no enforcement)        | fail                   | nonDiscretionary
 *  Default            | 1 week                     | fail                   | discretionary when app backgrounded
 *  Utility            | 1 week                     | internally retried     | discretionary when app backgrounded
 *  Background         | 1 week                     | internally retried     | discretionary
 *  @endcode
 * timeoutIntervalForResource
 * - the timeout interval for any network resources retrieved by this operation
 * - this can be overridden via CKOperationConfiguration's timeoutIntervalForResource property
 *
 * Network Error Behavior
 * - when a network request in service of a CKOperation fails due to a networking error, the operation may fail with that error, or internally retry the network request.  Only a subset of networking errors are retried, and limiting factors such as timeoutIntervalForResource are still applicable.
 *
 * Discretionary Behavior
 * - network requests in service of a CKOperation may be marked as discretionary
 * - discretionary network requests are scheduled at the description of the system for optimal performance
 *
 * CKOperations have a default qualityOfService of Default.
 */
@property (nonatomic, assign) NSQualityOfService qualityOfService;


/*! Defaults to @c YES */
@property (nonatomic, assign) BOOL allowsCellularAccess;

/*! @discussion Long lived operations will continue running even if your process exits. If your process remains alive for the lifetime of the long lived operation its behavior is the same as a regular operation.
 *
 *  Long lived operations can be fetched and replayed from the container via the @c fetchAllLongLivedOperations: and @c fetchLongLivedOperationsWithIDs: APIs.
 *
 *  Long lived operations persist until their -[NSOperation completionBlock] returns or until the operation is cancelled.
 *  Long lived operations may be garbage collected 24 hours after they finish running if no client has replayed them.
 *
 *  The default value for longLived is NO. Changing the value of longLived on an already started operation or on an outstanding long lived operation fetched from CKContainer has no effect.
 */
@property (nonatomic, assign, getter=isLongLived) BOOL longLived;

/*! @discussion If non-zero, overrides the timeout interval for any network requests issued by this operation.
 *  The default value is 60.
 *
 * @see NSURLSessionConfiguration.timeoutIntervalForRequest
 */
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForRequest;

/*! @discussion If set, overrides the timeout interval for any network resources retrieved by this operation.
 *  If not explicitly set, defaults to a value based on the operation's @c qualityOfService
 *
 * @see NSURLSessionConfiguration.timeoutIntervalForResource
 */
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForResource;


@end

#pragma mark - Deprecated CKOperation

/*! These deprecated properties now read and write from the CKOperation's configuration */
@interface CKOperation (CKOperationDeprecated)
@property (nonatomic, strong, nullable) CKContainer *container          API_DEPRECATED("Use CKOperationConfiguration", macos(10.10, 10.13), ios(8.0, 11.0), tvos(9.0, 11.0), watchos(3.0, 4.0));
@property (nonatomic, assign) BOOL allowsCellularAccess                 API_DEPRECATED("Use CKOperationConfiguration", macos(10.10, 10.13), ios(8.0, 11.0), tvos(9.0, 11.0), watchos(3.0, 4.0));
@property (nonatomic, assign, getter=isLongLived) BOOL longLived        API_DEPRECATED("Use CKOperationConfiguration", macos(10.12, 10.13), ios(9.3, 11.0), tvos(9.2, 11.0), watchos(3.0, 4.0));
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForRequest  API_DEPRECATED("Use CKOperationConfiguration", macos(10.12, 10.13), ios(10.0, 11.0), tvos(10.0, 11.0), watchos(3.0, 4.0));
@property (nonatomic, assign) NSTimeInterval timeoutIntervalForResource API_DEPRECATED("Use CKOperationConfiguration", macos(10.12, 10.13), ios(10.0, 11.0), tvos(10.0, 11.0), watchos(3.0, 4.0));
@end

NS_ASSUME_NONNULL_END

