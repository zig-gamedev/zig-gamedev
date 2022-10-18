/*	NSUUID.h
        Copyright (c) 2011-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#include <CoreFoundation/CFUUID.h>
#include <uuid/uuid.h>

/* Note: NSUUID is not toll-free bridged with CFUUID. Use UUID strings to convert between CFUUID and NSUUID, if needed. NSUUIDs are not guaranteed to be comparable by pointer value (as CFUUIDRef is); use isEqual: to compare two NSUUIDs. */

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0))
@interface NSUUID : NSObject <NSCopying, NSSecureCoding>

/* Create a new autoreleased NSUUID with RFC 4122 version 4 random bytes */
+ (instancetype)UUID;

/* Create a new NSUUID with RFC 4122 version 4 random bytes */
- (instancetype)init NS_DESIGNATED_INITIALIZER;

/* Create an NSUUID from a string such as "E621E1F8-C36C-495A-93FC-0C247A3E6E5F". Returns nil for invalid strings. */
- (nullable instancetype)initWithUUIDString:(NSString *)string;

/* Create an NSUUID with the given bytes */
- (instancetype)initWithUUIDBytes:(const uuid_t _Nullable)bytes;

/* Get the individual bytes of the receiver */
- (void)getUUIDBytes:(uuid_t _Nonnull)uuid;

/* Compare the receiver to another NSUUID in constant time */
- (NSComparisonResult)compare:(NSUUID *)otherUUID API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/* Return a string description of the UUID, such as "E621E1F8-C36C-495A-93FC-0C247A3E6E5F" */
@property (readonly, copy) NSString *UUIDString;

@end

NS_ASSUME_NONNULL_END
