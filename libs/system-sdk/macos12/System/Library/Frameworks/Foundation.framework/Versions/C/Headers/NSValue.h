/*	NSValue.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSDictionary;

NS_ASSUME_NONNULL_BEGIN

@interface NSValue : NSObject <NSCopying, NSSecureCoding>

- (void)getValue:(void *)value size:(NSUInteger)size API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

@property (readonly) const char *objCType NS_RETURNS_INNER_POINTER;

- (instancetype)initWithBytes:(const void *)value objCType:(const char *)type NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSValue (NSValueCreation)

+ (NSValue *)valueWithBytes:(const void *)value objCType:(const char *)type;
+ (NSValue *)value:(const void *)value withObjCType:(const char *)type;

@end

@interface NSValue (NSValueExtensionMethods)

+ (NSValue *)valueWithNonretainedObject:(nullable id)anObject;
@property (nullable, readonly) id nonretainedObjectValue;

+ (NSValue *)valueWithPointer:(nullable const void *)pointer;
@property (nullable, readonly) void *pointerValue;

- (BOOL)isEqualToValue:(NSValue *)value;

@end

@interface NSNumber : NSValue

- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithChar:(char)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedChar:(unsigned char)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithShort:(short)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedShort:(unsigned short)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithInt:(int)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedInt:(unsigned int)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithLong:(long)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedLong:(unsigned long)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithLongLong:(long long)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedLongLong:(unsigned long long)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithFloat:(float)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithDouble:(double)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithBool:(BOOL)value NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithInteger:(NSInteger)value API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;
- (NSNumber *)initWithUnsignedInteger:(NSUInteger)value API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;

@property (readonly) char charValue;
@property (readonly) unsigned char unsignedCharValue;
@property (readonly) short shortValue;
@property (readonly) unsigned short unsignedShortValue;
@property (readonly) int intValue;
@property (readonly) unsigned int unsignedIntValue;
@property (readonly) long longValue;
@property (readonly) unsigned long unsignedLongValue;
@property (readonly) long long longLongValue;
@property (readonly) unsigned long long unsignedLongLongValue;
@property (readonly) float floatValue;
@property (readonly) double doubleValue;
@property (readonly) BOOL boolValue;
@property (readonly) NSInteger integerValue API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
@property (readonly) NSUInteger unsignedIntegerValue API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@property (readonly, copy) NSString *stringValue;

- (NSComparisonResult)compare:(NSNumber *)otherNumber;

- (BOOL)isEqualToNumber:(NSNumber *)number;

- (NSString *)descriptionWithLocale:(nullable id)locale;

@end

@interface NSNumber (NSNumberCreation)

+ (NSNumber *)numberWithChar:(char)value;
+ (NSNumber *)numberWithUnsignedChar:(unsigned char)value;
+ (NSNumber *)numberWithShort:(short)value;
+ (NSNumber *)numberWithUnsignedShort:(unsigned short)value;
+ (NSNumber *)numberWithInt:(int)value;
+ (NSNumber *)numberWithUnsignedInt:(unsigned int)value;
+ (NSNumber *)numberWithLong:(long)value;
+ (NSNumber *)numberWithUnsignedLong:(unsigned long)value;
+ (NSNumber *)numberWithLongLong:(long long)value;
+ (NSNumber *)numberWithUnsignedLongLong:(unsigned long long)value;
+ (NSNumber *)numberWithFloat:(float)value;
+ (NSNumber *)numberWithDouble:(double)value;
+ (NSNumber *)numberWithBool:(BOOL)value;
+ (NSNumber *)numberWithInteger:(NSInteger)value API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
+ (NSNumber *)numberWithUnsignedInteger:(NSUInteger)value API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end

@interface NSValue (NSDeprecated)
/* This method is unsafe because it could potentially cause buffer overruns. You should use -getValue:size: instead.
 */
- (void)getValue:(void *)value API_DEPRECATED_WITH_REPLACEMENT("getValue:size:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
@end

NS_ASSUME_NONNULL_END
