/*	NSDictionary.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>

@class NSArray<ObjectType>, NSSet<ObjectType>, NSString, NSURL;

/****************	Immutable Dictionary	****************/

NS_ASSUME_NONNULL_BEGIN

@interface NSDictionary<__covariant KeyType, __covariant ObjectType> : NSObject <NSCopying, NSMutableCopying, NSSecureCoding, NSFastEnumeration>

@property (readonly) NSUInteger count;
- (nullable ObjectType)objectForKey:(KeyType)aKey;
- (NSEnumerator<KeyType> *)keyEnumerator;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects forKeys:(const KeyType <NSCopying> _Nonnull [_Nullable])keys count:(NSUInteger)cnt NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSDictionary<KeyType, ObjectType> (NSExtendedDictionary)

@property (readonly, copy) NSArray<KeyType> *allKeys;
- (NSArray<KeyType> *)allKeysForObject:(ObjectType)anObject;
@property (readonly, copy) NSArray<ObjectType> *allValues;
@property (readonly, copy) NSString *description;
@property (readonly, copy) NSString *descriptionInStringsFileFormat;
- (NSString *)descriptionWithLocale:(nullable id)locale;
- (NSString *)descriptionWithLocale:(nullable id)locale indent:(NSUInteger)level;
- (BOOL)isEqualToDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (NSEnumerator<ObjectType> *)objectEnumerator;
- (NSArray<ObjectType> *)objectsForKeys:(NSArray<KeyType> *)keys notFoundMarker:(ObjectType)marker;

/* Serializes this instance to the specified URL in the NSPropertyList format (using NSPropertyListXMLFormat_v1_0). For other formats use NSPropertyListSerialization directly. */
- (BOOL)writeToURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


- (NSArray<KeyType> *)keysSortedByValueUsingSelector:(SEL)comparator;
// count refers to the number of elements in the dictionary
- (void)getObjects:(ObjectType _Nonnull __unsafe_unretained [_Nullable])objects andKeys:(KeyType _Nonnull __unsafe_unretained [_Nullable])keys count:(NSUInteger)count API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)) NS_SWIFT_UNAVAILABLE("Use 'allKeys' and/or 'allValues' instead");

- (nullable ObjectType)objectForKeyedSubscript:(KeyType)key API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

- (void)enumerateKeysAndObjectsUsingBlock:(void (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateKeysAndObjectsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSArray<KeyType> *)keysSortedByValueUsingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSArray<KeyType> *)keysSortedByValueWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSSet<KeyType> *)keysOfEntriesPassingTest:(BOOL (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSSet<KeyType> *)keysOfEntriesWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(KeyType key, ObjectType obj, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

@interface NSDictionary<KeyType, ObjectType> (NSDeprecated)
/// This method is unsafe because it could potentially cause buffer overruns. You should use -getObjects:andKeys:count:
- (void)getObjects:(ObjectType _Nonnull __unsafe_unretained [_Nullable])objects andKeys:(KeyType _Nonnull __unsafe_unretained [_Nullable])keys  NS_SWIFT_UNAVAILABLE("Use 'allKeys' and/or 'allValues' instead")  API_DEPRECATED("Use -getObjects:andKeys:count: instead", macos(10.0, 10.13), ios(2.0, 11.0), watchos(2.0, 4.0), tvos(9.0, 11.0));

/* These methods are deprecated, and will be marked with API_DEPRECATED in a subsequent release. Use the variants that use errors instead. */
+ (nullable NSDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfFile:(NSString *)path API_DEPRECATED_WITH_REPLACEMENT("dictionaryWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
+ (nullable NSDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfURL:(NSURL *)url API_DEPRECATED_WITH_REPLACEMENT("dictionaryWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSDictionary<KeyType, ObjectType> *)initWithContentsOfFile:(NSString *)path API_DEPRECATED_WITH_REPLACEMENT("initWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSDictionary<KeyType, ObjectType> *)initWithContentsOfURL:(NSURL *)url API_DEPRECATED_WITH_REPLACEMENT("initWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile API_DEPRECATED_WITH_REPLACEMENT("writeToURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically API_DEPRECATED_WITH_REPLACEMENT("writeToURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)); // the atomically flag is ignored if url of a type that cannot be written atomically.
@end

@interface NSDictionary<KeyType, ObjectType> (NSDictionaryCreation)

+ (instancetype)dictionary;
+ (instancetype)dictionaryWithObject:(ObjectType)object forKey:(KeyType <NSCopying>)key;
+ (instancetype)dictionaryWithObjects:(const ObjectType _Nonnull [_Nullable])objects forKeys:(const KeyType <NSCopying> _Nonnull [_Nullable])keys count:(NSUInteger)cnt;

+ (instancetype)dictionaryWithObjectsAndKeys:(id)firstObject, ... NS_REQUIRES_NIL_TERMINATION NS_SWIFT_UNAVAILABLE("Use dictionary literals instead");

+ (instancetype)dictionaryWithDictionary:(NSDictionary<KeyType, ObjectType> *)dict;
+ (instancetype)dictionaryWithObjects:(NSArray<ObjectType> *)objects forKeys:(NSArray<KeyType <NSCopying>> *)keys;

- (instancetype)initWithObjectsAndKeys:(id)firstObject, ... NS_REQUIRES_NIL_TERMINATION;
- (instancetype)initWithDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (instancetype)initWithDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary copyItems:(BOOL)flag;
- (instancetype)initWithObjects:(NSArray<ObjectType> *)objects forKeys:(NSArray<KeyType <NSCopying>> *)keys;

/* Reads dictionary stored in NSPropertyList format from the specified url. */
- (nullable NSDictionary<NSString *, ObjectType> *)initWithContentsOfURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/* Reads dictionary stored in NSPropertyList format from the specified url. */
+ (nullable NSDictionary<NSString *, ObjectType> *)dictionaryWithContentsOfURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0)) NS_SWIFT_UNAVAILABLE("Use initializer instead");

@end

/****************	Mutable Dictionary	****************/

@interface NSMutableDictionary<KeyType, ObjectType> : NSDictionary<KeyType, ObjectType>

- (void)removeObjectForKey:(KeyType)aKey;
- (void)setObject:(ObjectType)anObject forKey:(KeyType <NSCopying>)aKey;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSExtendedMutableDictionary)

- (void)addEntriesFromDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (void)removeAllObjects;
- (void)removeObjectsForKeys:(NSArray<KeyType> *)keyArray;
- (void)setDictionary:(NSDictionary<KeyType, ObjectType> *)otherDictionary;
- (void)setObject:(nullable ObjectType)obj forKeyedSubscript:(KeyType <NSCopying>)key API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSMutableDictionaryCreation)

+ (instancetype)dictionaryWithCapacity:(NSUInteger)numItems;

+ (nullable NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfFile:(NSString *)path;
+ (nullable NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithContentsOfURL:(NSURL *)url;
- (nullable NSMutableDictionary<KeyType, ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSMutableDictionary<KeyType, ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

@interface NSDictionary<KeyType, ObjectType> (NSSharedKeySetDictionary)

/*  Use this method to create a key set to pass to +dictionaryWithSharedKeySet:.
 The keys are copied from the array and must be copyable.
 If the array parameter is nil or not an NSArray, an exception is thrown.
 If the array of keys is empty, an empty key set is returned.
 The array of keys may contain duplicates, which are ignored (it is undefined which object of each duplicate pair is used).
 As for any usage of hashing, is recommended that the keys have a well-distributed implementation of -hash, and the hash codes must satisfy the hash/isEqual: invariant.
 Keys with duplicate hash codes are allowed, but will cause lower performance and increase memory usage.
 */
+ (id)sharedKeySetForKeys:(NSArray<KeyType <NSCopying>> *)keys API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@end

@interface NSMutableDictionary<KeyType, ObjectType> (NSSharedKeySetDictionary)

/*  Create a mutable dictionary which is optimized for dealing with a known set of keys.
 Keys that are not in the key set can still be set into the dictionary, but that usage is not optimal.
 As with any dictionary, the keys must be copyable.
 If keyset is nil, an exception is thrown.
 If keyset is not an object returned by +sharedKeySetForKeys:, an exception is thrown.
 */
+ (NSMutableDictionary<KeyType, ObjectType> *)dictionaryWithSharedKeySet:(id)keyset API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@end

@interface NSDictionary<K, V> (NSGenericFastEnumeraiton) <NSFastEnumeration>
- (NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(K __unsafe_unretained _Nullable [_Nonnull])buffer count:(NSUInteger)len;
@end

NS_ASSUME_NONNULL_END
