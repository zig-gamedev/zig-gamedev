//
//  CKDatabaseOperation.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CloudKit/CKOperation.h>

@class CKDatabase;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKDatabaseOperation : CKOperation

/*! @abstract The database on which to perform the operation.
 *
 * @discussion If no database is set, @code [self.container privateCloudDatabase] @endcode is used.
 * This will also set the container property of the operation's configuration to match the container of the passed-in database.
 */
@property (nonatomic, strong, nullable) CKDatabase *database;

@end

NS_ASSUME_NONNULL_END
