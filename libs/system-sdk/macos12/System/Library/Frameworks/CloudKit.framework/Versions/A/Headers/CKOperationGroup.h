//
//  CKOperationGroup.h
//  CloudKit
//
//  Copyright (c) 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CloudKit/CKDefines.h>

@class CKOperationConfiguration;

NS_ASSUME_NONNULL_BEGIN

/*! @enum CKOperationGroupTransferSize
 *  @abstract Valid values for expectedSendSize and expectedReceiveSize
 *  @constant CKOperationGroupTransferSizeUnknown Default value when you're completely unsure of your working set size.
 *  @constant CKOperationGroupTransferSizeKilobytes Less than 1MB
 *  @constant CKOperationGroupTransferSizeMegabytes 1-10MB
 *  @constant CKOperationGroupTransferSizeTensOfMegabytes 10-100MB
 *  @constant CKOperationGroupTransferSizeHundredsOfMegabytes 100MB-1GB
 *  @constant CKOperationGroupTransferSizeGigabytes 1-10GB
 *  @constant CKOperationGroupTransferSizeTensOfGigabytes 10-100GB
 *  @constant CKOperationGroupTransferSizeHundredsOfGigabytes More than 100GB
 */
typedef NS_ENUM(NSInteger, CKOperationGroupTransferSize) {
    CKOperationGroupTransferSizeUnknown,
    CKOperationGroupTransferSizeKilobytes,
    CKOperationGroupTransferSizeMegabytes,
    CKOperationGroupTransferSizeTensOfMegabytes,
    CKOperationGroupTransferSizeHundredsOfMegabytes,
    CKOperationGroupTransferSizeGigabytes,
    CKOperationGroupTransferSizeTensOfGigabytes,
    CKOperationGroupTransferSizeHundredsOfGigabytes,
} API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0));

/*! @class CKOperationGroup
 *
 *  @abstract A mechanism for your app to group several operations at the granularity of a user action.
 *
 *  @discussion For example, when building a Calendar application, these things might warrant being their own operation groups:
 *  - an initial fetch of data from the server, consisting of many queries, fetchChanges, and fetch operations
 *  - doing an incremental fetch of data in response to a push notification
 *  - saving several records due to a user saving a calendar event
 *
 *  You associate @c CKOperationGroup s with@c  CKOperation s by setting the @c CKOperation.group property.  Create a new @c CKOperationGroup instance for each distinct user action.
 */
API_AVAILABLE(macos(10.13), ios(11.0), tvos(11.0), watchos(4.0))
@interface CKOperationGroup : NSObject <NSSecureCoding>

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)aDecoder NS_DESIGNATED_INITIALIZER;

/*! @abstract This is an identifier unique to this @c CKOperationGroup
 *
 *  @discussion This value is chosen by the system, and will be unique to this instance of a @c CKOperationGroup.  This identifier will be sent to Apple's servers, and can be used to identify any server-side logging associated with this operation group.
 */
@property (nonatomic, readonly, copy) NSString *operationGroupID;
 
/*! @abstract This is the default configuration applied to operations in this operation group.
 *
 *  @discussion If an operation associated with this operation group has its own configuration, then any explicitly-set properties in that operation's configuration will override these default configuration values.  See the example in CKOperation.h
 */
@property (atomic, copy, null_resettable) CKOperationConfiguration *defaultConfiguration;

/*! @abstract Describes the user action attributed to the operation group.
 *
 *  @discussion @c name should describe the type of work being done.  Some examples:
 *  "Initial Fetch"
 *  "Incremental Fetch"
 *  "Saving User-Entered Record"
 *  This string will be sent to Apple servers to provide aggregate reporting for @c CKOperationGroup s and therefore must not include personally identifying data.
 */
@property (atomic, copy, nullable) NSString *name;

/*! @abstract Describes an application-specific "number of elements" associated with the operation group.
 *
 *  @discussion @c quantity is intended to show the app-specific count of items contained within the operation group.  It is your job to assign meaning to this value.  For example, if an app created an operation group to save 3 calendar events the user had created, the app might want to set this to "3".  This value is not shown to your users, it's meant to aid your development and debugging.  This value will be reported in the CloudKit Dashboard's log entries for all operations associated with this operation group.
 */
@property (atomic, assign) NSUInteger quantity;

/*! @abstract Estimated size of traffic being uploaded to the CloudKit Server
 *
 *  @discussion Inform the system how much data you plan on transferring.  Obviously, these won't be exact.  Be as accurate as possible, but even an order-of-magnitude estimate is better than no value.  The system will consult these values when scheduling discretionary network requests (see the description of @c CKOperationConfiguration.qualityOfService).
 *  Overestimating your workload means that an operation group issuing discretionary network requests may be delayed until network conditions are good.
 *  Underestimating your workload may cause the system to oversaturate a constrained connection, leading to network failures.
 *  You may update after the @c CKOperationGroup is created.  If it is increased, then subsequent @c CKOperation s associated with this operation group may be delayed until network conditions are good.
 *  Defaults to @c CKOperationGroupTransferSizeUnknown
 */
@property (atomic, assign) CKOperationGroupTransferSize expectedSendSize;

/*! @abstract Estimated size of traffic being downloaded from the CloudKit Server
 *
 *  @discussion Inform the system how much data you plan on transferring.  Obviously, these won't be exact.  Be as accurate as possible, but even an order-of-magnitude estimate is better than no value.  The system will consult these values when scheduling discretionary network requests (see the description of @c CKOperationConfiguration.qualityOfService).
 *  Overestimating your workload means that an operation group issuing discretionary network requests may be delayed until network conditions are good.
 *  Underestimating your workload may cause the system to oversaturate a constrained connection, leading to network failures.
 *  You may update after the @c CKOperationGroup is created.  If it is increased, then subsequent @c CKOperation s associated with this operation group may be delayed until network conditions are good.
 *  Defaults to @c CKOperationGroupTransferSizeUnknown
 */
@property (atomic, assign) CKOperationGroupTransferSize expectedReceiveSize;

@end

NS_ASSUME_NONNULL_END
