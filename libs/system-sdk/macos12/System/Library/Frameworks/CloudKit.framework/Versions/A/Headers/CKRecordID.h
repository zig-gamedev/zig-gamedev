//
//  CKRecordID.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CKRecordZoneID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKRecordID : NSObject <NSSecureCoding, NSCopying>

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/*! Record names must be 255 characters or less. Most UTF-8 characters are valid. */

/*! Creates a record ID in the default zone */
- (instancetype)initWithRecordName:(NSString *)recordName;

- (instancetype)initWithRecordName:(NSString *)recordName zoneID:(CKRecordZoneID *)zoneID NS_DESIGNATED_INITIALIZER;

@property (nonatomic, readonly, copy) NSString *recordName;
@property (nonatomic, readonly, copy) CKRecordZoneID *zoneID;

@end

NS_ASSUME_NONNULL_END
