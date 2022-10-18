//
//  CKUserIdentityLookupInfo.h
//  CloudKit
//
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CKRecordID;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.12), ios(10.0), tvos(10.0), watchos(3.0))
@interface CKUserIdentityLookupInfo : NSObject <NSSecureCoding, NSCopying>
- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

- (instancetype)initWithEmailAddress:(NSString *)emailAddress;
- (instancetype)initWithPhoneNumber:(NSString *)phoneNumber;
- (instancetype)initWithUserRecordID:(CKRecordID *)userRecordID;

+ (NSArray<CKUserIdentityLookupInfo *> *)lookupInfosWithEmails:(NSArray<NSString *> *)emails;
+ (NSArray<CKUserIdentityLookupInfo *> *)lookupInfosWithPhoneNumbers:(NSArray<NSString *> *)phoneNumbers;
+ (NSArray<CKUserIdentityLookupInfo *> *)lookupInfosWithRecordIDs:(NSArray<CKRecordID *> *)recordIDs;

@property (nonatomic, readonly, copy, nullable) NSString *emailAddress;
@property (nonatomic, readonly, copy, nullable) NSString *phoneNumber;
@property (nonatomic, readonly, copy, nullable) CKRecordID *userRecordID;
@end

NS_ASSUME_NONNULL_END
