//
//  CKRecordZoneID.h
//  CloudKit
//
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.10), ios(8.0), watchos(3.0))
@interface CKRecordZoneID : NSObject <NSSecureCoding, NSCopying>

/*! Zone names must be 255 characters or less. Most UTF-8 characters are valid. */

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)initWithZoneName:(NSString *)zoneName ownerName:(NSString *)ownerName NS_DESIGNATED_INITIALIZER;

@property (nonatomic, readonly, copy) NSString *zoneName;
@property (nonatomic, readonly, copy) NSString *ownerName;

@end

NS_ASSUME_NONNULL_END
