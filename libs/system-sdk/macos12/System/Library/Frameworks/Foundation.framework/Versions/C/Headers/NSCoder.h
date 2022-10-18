/*	NSCoder.h
	Copyright (c) 1993-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSData, NSSet<ObjectType>;

NS_ASSUME_NONNULL_BEGIN

/*!
 Describes the action an NSCoder should take when it encounters decode failures (e.g. corrupt data) for non-TopLevel decodes.
 */
typedef NS_ENUM(NSInteger, NSDecodingFailurePolicy) {
    // On decode failure, the NSCoder will raise an exception internally to propagate failure messages (and unwind the stack). This exception can be transformed into an NSError via any of the TopLevel decode APIs.
    NSDecodingFailurePolicyRaiseException,

    // On decode failure, the NSCoder will capture the failure as an NSError, and prevent further decodes (by returning 0 / nil equivalent as appropriate). Clients should consider using this policy if they know that all encoded objects behave correctly in the presence of decode failures (e.g. they use -failWithError: to communicate decode failures and don't raise exceptions for error propagation)
    NSDecodingFailurePolicySetErrorAndReturn,
} API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

@interface NSCoder : NSObject

- (void)encodeValueOfObjCType:(const char *)type at:(const void *)addr;
- (void)encodeDataObject:(NSData *)data;
- (nullable NSData *)decodeDataObject;
- (void)decodeValueOfObjCType:(const char *)type at:(void *)data size:(NSUInteger)size API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));
- (NSInteger)versionForClassName:(NSString *)className;

@end

@interface NSCoder (NSExtendedCoder)
    
- (void)encodeObject:(nullable id)object;
- (void)encodeRootObject:(id)rootObject;
- (void)encodeBycopyObject:(nullable id)anObject;
- (void)encodeByrefObject:(nullable id)anObject;
- (void)encodeConditionalObject:(nullable id)object;
- (void)encodeValuesOfObjCTypes:(const char *)types, ...;
- (void)encodeArrayOfObjCType:(const char *)type count:(NSUInteger)count at:(const void *)array;
- (void)encodeBytes:(nullable const void *)byteaddr length:(NSUInteger)length;

- (nullable id)decodeObject;
- (nullable id)decodeTopLevelObjectAndReturnError:(NSError **)error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) NS_SWIFT_UNAVAILABLE("Use 'decodeTopLevelObject() throws' instead");
- (void)decodeValuesOfObjCTypes:(const char *)types, ...;
- (void)decodeArrayOfObjCType:(const char *)itemType count:(NSUInteger)count at:(void *)array;
- (nullable void *)decodeBytesWithReturnedLength:(NSUInteger *)lengthp NS_RETURNS_INNER_POINTER;

#if TARGET_OS_OSX
- (void)encodePropertyList:(id)aPropertyList;
- (nullable id)decodePropertyList;
#endif

- (void)setObjectZone:(nullable NSZone *)zone NS_AUTOMATED_REFCOUNT_UNAVAILABLE;
- (nullable NSZone *)objectZone NS_AUTOMATED_REFCOUNT_UNAVAILABLE;

@property (readonly) unsigned int systemVersion;

@property (readonly) BOOL allowsKeyedCoding;

- (void)encodeObject:(nullable id)object forKey:(NSString *)key;
- (void)encodeConditionalObject:(nullable id)object forKey:(NSString *)key;
- (void)encodeBool:(BOOL)value forKey:(NSString *)key;
- (void)encodeInt:(int)value forKey:(NSString *)key;
- (void)encodeInt32:(int32_t)value forKey:(NSString *)key;
- (void)encodeInt64:(int64_t)value forKey:(NSString *)key;
- (void)encodeFloat:(float)value forKey:(NSString *)key;
- (void)encodeDouble:(double)value forKey:(NSString *)key;
- (void)encodeBytes:(nullable const uint8_t *)bytes length:(NSUInteger)length forKey:(NSString *)key;

- (BOOL)containsValueForKey:(NSString *)key;
- (nullable id)decodeObjectForKey:(NSString *)key;
- (nullable id)decodeTopLevelObjectForKey:(NSString *)key error:(NSError **)error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) NS_SWIFT_UNAVAILABLE("Use 'decodeObject(of:, forKey:)' instead");
- (BOOL)decodeBoolForKey:(NSString *)key;
- (int)decodeIntForKey:(NSString *)key;
- (int32_t)decodeInt32ForKey:(NSString *)key;
- (int64_t)decodeInt64ForKey:(NSString *)key;
- (float)decodeFloatForKey:(NSString *)key;
- (double)decodeDoubleForKey:(NSString *)key;
- (nullable const uint8_t *)decodeBytesForKey:(NSString *)key returnedLength:(nullable NSUInteger *)lengthp NS_RETURNS_INNER_POINTER;   // returned bytes immutable!

- (void)encodeInteger:(NSInteger)value forKey:(NSString *)key API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (NSInteger)decodeIntegerForKey:(NSString *)key API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

// Returns YES if this coder requires secure coding. Secure coders check a list of allowed classes before decoding objects, and all objects must implement NSSecureCoding.
@property (readonly) BOOL requiresSecureCoding API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

// Specify what the expected class of the allocated object is. If the coder responds YES to -requiresSecureCoding, then an exception will be thrown if the class to be decoded does not implement NSSecureCoding or is not isKindOfClass: of the argument. If the coder responds NO to -requiresSecureCoding, then the class argument is ignored and no check of the class of the decoded object is performed, exactly as if decodeObjectForKey: had been called.
- (nullable id)decodeObjectOfClass:(Class)aClass forKey:(NSString *)key API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));
- (nullable id)decodeTopLevelObjectOfClass:(Class)aClass forKey:(NSString *)key error:(NSError **)error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) NS_SWIFT_UNAVAILABLE("Use 'decodeObject(of:, forKey:)' instead");

/**
 Decodes the \c NSArray object for the given  \c key, which should be an \c NSArray<cls>, containing the given non-collection class (no nested arrays or arrays of dictionaries, etc) from the coder.

 Requires \c NSSecureCoding otherwise an exception is thrown and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the object for \c key is not of the expected types, or cannot be decoded, and sets the \c error on the decoder.
 */
- (nullable NSArray *)decodeArrayOfObjectsOfClass:(Class)cls forKey:(NSString *)key API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

/**
  Decodes the \c NSDictionary object for the given \c key, which should be an \c NSDictionary<keyCls,objectCls> , with keys of type given in \c keyCls and objects of the given non-collection class \c objectCls (no nested dictionaries or other dictionaries contained in the dictionary, etc) from the coder.

 Requires \c NSSecureCoding otherwise an exception is thrown and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the object for \c key is not of the expected types, or cannot be decoded, and sets the \c error on the decoder.
 */
- (nullable NSDictionary *)decodeDictionaryWithKeysOfClass:(Class)keyCls objectsOfClass:(Class)objectCls forKey:(NSString *)key API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

// The class of the object may be any class in the provided NSSet, or a subclass of any class in the set. Otherwise, the behavior is the same as -decodeObjectOfClass:forKey:.
- (nullable id)decodeObjectOfClasses:(nullable NSSet<Class> *)classes forKey:(NSString *)key API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) NS_REFINED_FOR_SWIFT;
- (nullable id)decodeTopLevelObjectOfClasses:(nullable NSSet<Class> *)classes forKey:(NSString *)key error:(NSError **)error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) NS_SWIFT_UNAVAILABLE("Use 'decodeObject(of:, forKey:)' instead");

/**
 Decodes the \c NSArray object for the given \c key, which should be an \c NSArray, containing the given non-collection classes (no nested arrays or arrays of dictionaries, etc) from the coder.

 Requires \c NSSecureCoding otherwise an exception is thrown and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the object for \c key is not of the expected types, or cannot be decoded, and sets the \c error on the decoder.
 */
- (nullable NSArray *)decodeArrayOfObjectsOfClasses:(NSSet<Class> *)classes forKey:(NSString *)key API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

/**
 Decodes the \c NSDictionary object for the given \c key, which should be an \c NSDictionary, with keys of the types given in \c keyClasses and objects of the given non-collection classes in \c objectClasses (no nested dictionaries or other dictionaries contained in the dictionary, etc) from the given coder.

 Requires \c NSSecureCoding otherwise an exception is thrown and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the object for \c key is not of the expected types, or cannot be decoded, and sets the \c error on the decoder.
 */
- (nullable NSDictionary *)decodeDictionaryWithKeysOfClasses:(NSSet<Class> *)keyClasses objectsOfClasses:(NSSet<Class> *)objectClasses forKey:(NSString *)key API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

// Calls -decodeObjectOfClasses:forKey: with a set allowing only property list types.
- (nullable id)decodePropertyListForKey:(NSString *)key API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

// Get the current set of allowed classes.
@property (nullable, readonly, copy) NSSet<Class> *allowedClasses API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

/*!
 @abstract Signals to this coder that the decode has failed.
 @parameter non-nil error that describes the reason why the decode failed
 @discussion
 Sets an error on this NSCoder once per TopLevel decode; calling it repeatedly will have no effect until the call stack unwinds to one of the TopLevel decode entry-points.

 This method is only meaningful to call for decodes.

 Typically, you would want to call this method in your -initWithCoder: implementation when you detect situations like:
 - lack of secure coding
 - corruption of your data
 - domain validation failures

 After calling -failWithError: within your -initWithCoder: implementation, you should clean up and return nil as early as possible.

 Once an error has been signaled to a decoder, it remains set until it has handed off to the first TopLevel decode invocation above it.  For example, consider the following call graph:
 A    -decodeTopLevelObjectForKey:error:
 B        -initWithCoder:
 C            -decodeObjectForKey:
 D                -initWithCoder:
 E                    -decodeObjectForKey:
 F                        -failWithError:

 In this case the error provided in stack-frame F will be returned via the outError in stack-frame A. Furthermore the result object from decodeTopLevelObjectForKey:error: will be nil, regardless of the result of stack-frame B.

 NSCoder implementations support two mechanisms for the stack-unwinding from F to A:
 - forced (NSException based)
 - particpatory (error based)

 The kind of unwinding you get is determined by the decodingFailurePolicy property of this NSCoder (which defaults to NSDecodingFailurePolicyRaiseException to match historical behavior).
 */
- (void)failWithError:(NSError *)error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/*!
 @abstract Defines the behavior this NSCoder should take on decode failure (i.e. corrupt archive, invalid data, etc.).
 @discussion
 The default result of this property is NSDecodingFailurePolicyRaiseException, subclasses can change this to an alternative policy.
 */
@property (readonly) NSDecodingFailurePolicy decodingFailurePolicy API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/*!
 @abstract The current error (if there is one) for the current TopLevel decode.
 @discussion
 The meaning of this property changes based on the result of the decodingFailurePolicy property:
 For NSDecodingFailurePolicyRaiseException, this property will always be nil.
 For NSDecodingFailurePolicySetErrorAndReturn, this property can be non-nil, and if so, indicates that there was a failure while decoding the archive (specifically its the very first error encountered).

 While .error is non-nil, all attempts to decode data from this coder will return a nil/zero-equivalent value.

 This error is consumed by a TopLevel decode API (which resets this coder back to a being able to potentially decode data).
 */
@property (nullable, readonly, copy) NSError *error API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

@end

#if TARGET_OS_OSX
FOUNDATION_EXPORT NSObject * _Nullable NXReadNSObjectFromCoder(NSCoder *decoder) API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
/* Given an NSCoder, returns an object previously written with
   NXWriteNSObject(). The returned object is autoreleased. */

@interface NSCoder (NSTypedstreamCompatibility)

- (void)encodeNXObject:(id)object API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
/* Writes old-style object onto the coder. No sharing is done across
   separate -encodeNXObject:. Callers must have implemented an
   -encodeWithCoder:, which parallels the -write: methods, on all of
   their classes which may be touched by this operation. Object's
   -replacementObjectForCoder: compatibility method will take care
   of calling -startArchiving:. */
    
- (nullable id)decodeNXObject API_DEPRECATED("Not supported", macos(10.0,10.5), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));
/* Reads an object previously written with -encodeNXObject:. No
   sharing is done across separate -decodeNXObject. Callers must
   have implemented an -initWithCoder:, which parallels the -read:
   methods, on all of their classes which may be touched by this
   operation. Object's -awakeAfterUsingCoder: compatibility method
   will take care of calling -awake and -finishUnarchiving. The
   returned object is autoreleased. */

@end
#endif

@interface NSCoder(NSDeprecated)
/* This method is unsafe because it could potentially cause buffer overruns. You should use -decodeValueOfObjCType:at:size: instead.
 */
- (void)decodeValueOfObjCType:(const char *)type at:(void *)data API_DEPRECATED_WITH_REPLACEMENT("decodeValueOfObjCType:at:size:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@end

NS_ASSUME_NONNULL_END
