/*	NSKeyedArchiver.h
	Copyright (c) 2001-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSCoder.h>
#import <Foundation/NSPropertyList.h>
#import <Foundation/NSException.h>
#if TARGET_OS_OSX
#import <Foundation/NSGeometry.h>
#endif


@class NSArray<ObjectType>, NSMutableData, NSData, NSString;
@protocol NSKeyedArchiverDelegate, NSKeyedUnarchiverDelegate;

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSExceptionName const NSInvalidArchiveOperationException;
FOUNDATION_EXPORT NSExceptionName const NSInvalidUnarchiveOperationException;
// Archives created using the class method archivedDataWithRootObject used this key for the root object in the hierarchy of encoded objects. The NSKeyedUnarchiver class method unarchiveObjectWithData: will look for this root key as well. You can also use it as the key for the root object in your own archives.
FOUNDATION_EXPORT NSString * const NSKeyedArchiveRootObjectKey API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@interface NSKeyedArchiver : NSCoder

/**
 Initializes the receiver for encoding an archive, optionally disabling secure coding.

 If \c NSSecureCoding cannot be used, \c requiresSecureCoding may be turned off here; for improved security, however, \c requiresSecureCoding should be left enabled whenever possible. \c requiresSecureCoding ensures that all encoded objects conform to \c NSSecureCoding, preventing the possibility of encoding objects which cannot be decoded later.

 To produce archives whose structure matches those previously encoded using \c +archivedDataWithRootObject, encode the top-level object in your archive for the \c NSKeyedArchiveRootObjectKey.
 */
- (instancetype)initRequiringSecureCoding:(BOOL)requiresSecureCoding API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/**
 Returns an \c NSData object containing the encoded form of the object graph whose root object is given, optionally disabling secure coding.

 If \c NSSecureCoding cannot be used, \c requiresSecureCoding may be turned off here; for improved security, however, \c requiresSecureCoding should be left enabled whenever possible. \c requiresSecureCoding ensures that all encoded objects conform to \c NSSecureCoding, preventing the possibility of encoding objects which cannot be decoded later.

 If the object graph cannot be encoded, returns \c nil and sets the \c error out parameter.
 */
+ (nullable NSData *)archivedDataWithRootObject:(id)object requiringSecureCoding:(BOOL)requiresSecureCoding error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/// Initialize the archiver with empty data, ready for writing.
- (instancetype)init API_DEPRECATED("Use -initRequiringSecureCoding: instead", macosx(10.12,10.14), ios(10.0,12.0), watchos(3.0,5.0), tvos(10.0,12.0));
- (instancetype)initForWritingWithMutableData:(NSMutableData *)data API_DEPRECATED("Use -initRequiringSecureCoding: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));

+ (NSData *)archivedDataWithRootObject:(id)rootObject API_DEPRECATED("Use +archivedDataWithRootObject:requiringSecureCoding:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));
+ (BOOL)archiveRootObject:(id)rootObject toFile:(NSString *)path API_DEPRECATED("Use +archivedDataWithRootObject:requiringSecureCoding:error: and -writeToURL:options:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));

@property (nullable, assign) id <NSKeyedArchiverDelegate> delegate;

@property NSPropertyListFormat outputFormat;

/// If encoding has not yet finished, then invoking this property will call finishEncoding and return the data. If you initialized the keyed archiver with a specific mutable data instance, then it will be returned from this property after finishEncoding is called.
@property (readonly, strong) NSData *encodedData API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

- (void)finishEncoding;

+ (void)setClassName:(nullable NSString *)codedName forClass:(Class)cls;
- (void)setClassName:(nullable NSString *)codedName forClass:(Class)cls;
	// During encoding, the coder first checks with the coder's
	// own table, then if there was no mapping there, the class's.

+ (nullable NSString *)classNameForClass:(Class)cls;
- (nullable NSString *)classNameForClass:(Class)cls;

- (void)encodeObject:(nullable id)object forKey:(NSString *)key;
- (void)encodeConditionalObject:(nullable id)object forKey:(NSString *)key;
- (void)encodeBool:(BOOL)value forKey:(NSString *)key;
- (void)encodeInt:(int)value forKey:(NSString *)key;	// native int
- (void)encodeInt32:(int32_t)value forKey:(NSString *)key;
- (void)encodeInt64:(int64_t)value forKey:(NSString *)key;
- (void)encodeFloat:(float)value forKey:(NSString *)key;
- (void)encodeDouble:(double)value forKey:(NSString *)key;
- (void)encodeBytes:(nullable const uint8_t *)bytes length:(NSUInteger)length forKey:(NSString *)key;

// Enables secure coding support on this keyed archiver. You do not need to enable secure coding on the archiver to enable secure coding on the unarchiver. Enabling secure coding on the archiver is a way for you to be sure that all classes that are encoded conform with NSSecureCoding (it will throw an exception if a class which does not NSSecureCoding is archived). Note that the getter is on the superclass, NSCoder. See NSCoder for more information about secure coding.
@property (readwrite) BOOL requiresSecureCoding API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@end

@interface NSKeyedUnarchiver : NSCoder

/**
 Initializes the receiver for decoding an archive previously encoded by \c NSKeyedUnarchiver.

 Enables \c requiresSecureCoding by default. If \c NSSecureCoding cannot be used, \c requiresSecureCoding may be turned off manually; for improved security, \c requiresSecureCoding should be left enabled whenever possible.

 Sets the unarchiver's \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the given data is not valid, and sets the \c error out parameter.
 */
- (nullable instancetype)initForReadingFromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/**
 Decodes the root object of the given class from the given archive, previously encoded by \c NSKeyedArchiver.

 Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
 */
+ (nullable id)unarchivedObjectOfClass:(Class)cls fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0)) NS_REFINED_FOR_SWIFT;

/**
 Decodes the \c NSArray root object from \c data which should be an \c NSArray<cls> containing the given non-collection class (no nested arrays or arrays of dictionaries, etc) from the given archive, previously encoded by \c NSKeyedArchiver.
 
 Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
 */
+ (nullable NSArray *)unarchivedArrayOfObjectsOfClass:(Class)cls fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

/**
Decodes the \c NSDictionary root object from \c data which should be an \c NSDictionary<keyCls,objectCls>  with keys of type given in \c keyCls and objects of the given non-collection class \c objectCls (no nested dictionaries or other dictionaries contained in the dictionary, etc) from the given archive, previously encoded by \c NSKeyedArchiver.

Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
*/
+ (nullable NSDictionary *)unarchivedDictionaryWithKeysOfClass:(Class)keyCls objectsOfClass:(Class)valueCls fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

/**
 Decodes the root object of one of the given classes from the given archive, previously encoded by \c NSKeyedArchiver.

 Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

 Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
 */
+ (nullable id)unarchivedObjectOfClasses:(NSSet<Class> *)classes fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/**
Decodes the \c NSArray root object from \c data which should be an \c NSArray, containing the given non-collection classes in \c classes  (no nested arrays or arrays of dictionaries, etc) from the given archive, previously encoded by \c NSKeyedArchiver.

Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
*/
+ (nullable NSArray *)unarchivedArrayOfObjectsOfClasses:(NSSet<Class> *)classes fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

/**
Decodes the \c NSDictionary root object from \c data which should be an \c NSDictionary, with keys of the types given in \c keyClasses and objects of the given non-collection classes in \c objectClasses (no nested dictionaries or other dictionaries contained in the dictionary, etc) from the given archive, previously encoded by \c NSKeyedArchiver.

Enables \c requiresSecureCoding and sets the \c decodingFailurePolicy to \c NSDecodingFailurePolicySetErrorAndReturn.

Returns \c nil if the given data is not valid or cannot be decoded, and sets the \c error out parameter.
*/
+ (nullable NSDictionary *)unarchivedDictionaryWithKeysOfClasses:(NSSet<Class> *)keyClasses objectsOfClasses:(NSSet<Class> *)valueClasses fromData:(NSData *)data error:(NSError **)error API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) NS_REFINED_FOR_SWIFT;

- (instancetype)init API_DEPRECATED("Use -initForReadingFromData:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));
- (instancetype)initForReadingWithData:(NSData *)data API_DEPRECATED("Use -initForReadingFromData:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));

+ (nullable id)unarchiveObjectWithData:(NSData *)data API_DEPRECATED("Use +unarchivedObjectOfClass:fromData:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));
+ (nullable id)unarchiveTopLevelObjectWithData:(NSData *)data error:(NSError **)error API_DEPRECATED("Use +unarchivedObjectOfClass:fromData:error: instead", macosx(10.11,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0)) NS_SWIFT_UNAVAILABLE("Use 'unarchiveTopLevelObjectWithData(_:) throws' instead");
+ (nullable id)unarchiveObjectWithFile:(NSString *)path API_DEPRECATED("Use +unarchivedObjectOfClass:fromData:error: instead", macosx(10.2,10.14), ios(2.0,12.0), watchos(2.0,5.0), tvos(9.0,12.0));

@property (nullable, assign) id <NSKeyedUnarchiverDelegate> delegate;

- (void)finishDecoding;

+ (void)setClass:(nullable Class)cls forClassName:(NSString *)codedName;
- (void)setClass:(nullable Class)cls forClassName:(NSString *)codedName;
	// During decoding, the coder first checks with the coder's
	// own table, then if there was no mapping there, the class's.

+ (nullable Class)classForClassName:(NSString *)codedName;
- (nullable Class)classForClassName:(NSString *)codedName;

- (BOOL)containsValueForKey:(NSString *)key;

- (nullable id)decodeObjectForKey:(NSString *)key;
- (BOOL)decodeBoolForKey:(NSString *)key;
- (int)decodeIntForKey:(NSString *)key;		// may raise a range exception
- (int32_t)decodeInt32ForKey:(NSString *)key;
- (int64_t)decodeInt64ForKey:(NSString *)key;
- (float)decodeFloatForKey:(NSString *)key;
- (double)decodeDoubleForKey:(NSString *)key;
- (nullable const uint8_t *)decodeBytesForKey:(NSString *)key returnedLength:(nullable NSUInteger *)lengthp NS_RETURNS_INNER_POINTER;	// returned bytes immutable, and they go away with the unarchiver, not the containing autorelease pool

// Enables secure coding support on this keyed unarchiver. When enabled, unarchiving a disallowed class throws an exception. Once enabled, attempting to set requiresSecureCoding to NO will throw an exception. This is to prevent classes from selectively turning secure coding off. This is designed to be set once at the top level and remain on. Note that the getter is on the superclass, NSCoder. See NSCoder for more information about secure coding.
@property (readwrite) BOOL requiresSecureCoding API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@property (readwrite) NSDecodingFailurePolicy decodingFailurePolicy API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

@end

@protocol NSKeyedArchiverDelegate <NSObject>
@optional

// substitution
- (nullable id)archiver:(NSKeyedArchiver *)archiver willEncodeObject:(id)object;
	// Informs the delegate that the object is about to be encoded.  The delegate
	// either returns this object or can return a different object to be encoded
	// instead.  The delegate can also fiddle with the coder state.  If the delegate
	// returns nil, nil is encoded.  This method is called after the original object
        // may have replaced itself with replacementObjectForKeyedArchiver:.
        // This method is not called for an object once a replacement mapping has been
        // setup for that object (either explicitly, or because the object has previously
        // been encoded).  This is also not called when nil is about to be encoded.
        // This method is called whether or not the object is being encoded conditionally.

- (void)archiver:(NSKeyedArchiver *)archiver didEncodeObject:(nullable id)object;
	// Informs the delegate that the given object has been encoded.  The delegate
	// might restore some state it had fiddled previously, or use this to keep
	// track of the objects which are encoded.  The object may be nil.  Not called
	// for conditional objects until they are really encoded (if ever).

// notification
- (void)archiver:(NSKeyedArchiver *)archiver willReplaceObject:(nullable id)object withObject:(nullable id)newObject;
	// Informs the delegate that the newObject is being substituted for the
	// object. This is also called when the delegate itself is doing/has done
	// the substitution. The delegate may use this method if it is keeping track
	// of the encoded or decoded objects.

- (void)archiverWillFinish:(NSKeyedArchiver *)archiver;
	// Notifies the delegate that encoding is about to finish.

- (void)archiverDidFinish:(NSKeyedArchiver *)archiver;
	// Notifies the delegate that encoding has finished.

@end

@protocol NSKeyedUnarchiverDelegate <NSObject>
@optional

// error handling
- (nullable Class)unarchiver:(NSKeyedUnarchiver *)unarchiver cannotDecodeObjectOfClassName:(NSString *)name originalClasses:(NSArray<NSString *> *)classNames;
	// Informs the delegate that the named class is not available during decoding.
	// The delegate may, for example, load some code to introduce the class to the
	// runtime and return it, or substitute a different class object.  If the
	// delegate returns nil, unarchiving aborts with an exception.  The first class
        // name string in the array is the class of the encoded object, the second is
        // the immediate superclass, and so on.

// substitution
- (nullable id)unarchiver:(NSKeyedUnarchiver *)unarchiver didDecodeObject:(nullable id) NS_RELEASES_ARGUMENT object NS_RETURNS_RETAINED;
	// Informs the delegate that the object has been decoded.  The delegate
	// either returns this object or can return a different object to replace
	// the decoded one.  The object may be nil.  If the delegate returns nil,
        // the decoded value will be unchanged (that is, the original object will be
        // decoded). The delegate may use this to keep track of the decoded objects.

// notification
- (void)unarchiver:(NSKeyedUnarchiver *)unarchiver willReplaceObject:(id)object withObject:(id)newObject;
	// Informs the delegate that the newObject is being substituted for the
	// object. This is also called when the delegate itself is doing/has done
	// the substitution. The delegate may use this method if it is keeping track
	// of the encoded or decoded objects.

- (void)unarchiverWillFinish:(NSKeyedUnarchiver *)unarchiver;
	// Notifies the delegate that decoding is about to finish.

- (void)unarchiverDidFinish:(NSKeyedUnarchiver *)unarchiver;
	// Notifies the delegate that decoding has finished.

@end

@interface NSObject (NSKeyedArchiverObjectSubstitution)

@property (nullable, readonly) Class classForKeyedArchiver;
	// Implemented by classes to substitute a new class for instances during
	// encoding.  The object will be encoded as if it were a member of the
	// returned class.  The results of this method are overridden by the archiver
        // class and instance name<->class encoding tables.  If nil is returned,
        // the result of this method is ignored.  This method returns the result of
	// [self classForArchiver] by default, NOT -classForCoder as might be
	// expected.  This is a concession to source compatibility.

- (nullable id)replacementObjectForKeyedArchiver:(NSKeyedArchiver *)archiver;
	// Implemented by classes to substitute new instances for the receiving
	// instance during encoding.  The returned object will be encoded instead
	// of the receiver (if different).  This method is called only if no
	// replacement mapping for the object has been set up in the archiver yet
        // (for example, due to a previous call of replacementObjectForKeyedArchiver:
        // to that object).  This method returns the result of
	// [self replacementObjectForArchiver:nil] by default, NOT
	// -replacementObjectForCoder: as might be expected.  This is a concession
	// to source compatibility.


+ (NSArray<NSString *> *)classFallbacksForKeyedArchiver;


@end

@interface NSObject (NSKeyedUnarchiverObjectSubstitution)

+ (Class)classForKeyedUnarchiver;
	// Implemented by classes to substitute a new class during decoding.
        // Objects of the class will be decoded as members of the returned
        // class.  This method overrides the results of the unarchiver's class and
        // instance name<->class encoding tables.  Returns self by default.

@end

NS_ASSUME_NONNULL_END
