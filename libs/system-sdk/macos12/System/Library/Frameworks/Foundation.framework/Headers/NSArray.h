/*	NSArray.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>
#import <Foundation/NSRange.h>
#import <Foundation/NSObjCRuntime.h>
#import <Foundation/NSOrderedCollectionDifference.h>

@class NSData, NSIndexSet, NSString, NSURL;

/****************	Immutable Array		****************/

NS_ASSUME_NONNULL_BEGIN

@interface NSArray<__covariant ObjectType> : NSObject <NSCopying, NSMutableCopying, NSSecureCoding, NSFastEnumeration>

@property (readonly) NSUInteger count;
- (ObjectType)objectAtIndex:(NSUInteger)index;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects count:(NSUInteger)cnt NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSArray<ObjectType> (NSExtendedArray)

- (NSArray<ObjectType> *)arrayByAddingObject:(ObjectType)anObject;
- (NSArray<ObjectType> *)arrayByAddingObjectsFromArray:(NSArray<ObjectType> *)otherArray;
- (NSString *)componentsJoinedByString:(NSString *)separator;
- (BOOL)containsObject:(ObjectType)anObject;
@property (readonly, copy) NSString *description;
- (NSString *)descriptionWithLocale:(nullable id)locale;
- (NSString *)descriptionWithLocale:(nullable id)locale indent:(NSUInteger)level;
- (nullable ObjectType)firstObjectCommonWithArray:(NSArray<ObjectType> *)otherArray;
- (void)getObjects:(ObjectType _Nonnull __unsafe_unretained [_Nonnull])objects range:(NSRange)range NS_SWIFT_UNAVAILABLE("Use 'subarrayWithRange()' instead");
- (NSUInteger)indexOfObject:(ObjectType)anObject;
- (NSUInteger)indexOfObject:(ObjectType)anObject inRange:(NSRange)range;
- (NSUInteger)indexOfObjectIdenticalTo:(ObjectType)anObject;
- (NSUInteger)indexOfObjectIdenticalTo:(ObjectType)anObject inRange:(NSRange)range;
- (BOOL)isEqualToArray:(NSArray<ObjectType> *)otherArray;
@property (nullable, nonatomic, readonly) ObjectType firstObject API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, nonatomic, readonly) ObjectType lastObject;
- (NSEnumerator<ObjectType> *)objectEnumerator;
- (NSEnumerator<ObjectType> *)reverseObjectEnumerator;
@property (readonly, copy) NSData *sortedArrayHint;
- (NSArray<ObjectType> *)sortedArrayUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType, ObjectType, void * _Nullable))comparator context:(nullable void *)context;
- (NSArray<ObjectType> *)sortedArrayUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType, ObjectType, void * _Nullable))comparator context:(nullable void *)context hint:(nullable NSData *)hint;
- (NSArray<ObjectType> *)sortedArrayUsingSelector:(SEL)comparator;
- (NSArray<ObjectType> *)subarrayWithRange:(NSRange)range;
/* Serializes this instance to the specified URL in the NSPropertyList format (using NSPropertyListXMLFormat_v1_0). For other formats use NSPropertyListSerialization directly. */
- (BOOL)writeToURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


- (void)makeObjectsPerformSelector:(SEL)aSelector NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");
- (void)makeObjectsPerformSelector:(SEL)aSelector withObject:(nullable id)argument NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");

- (NSArray<ObjectType> *)objectsAtIndexes:(NSIndexSet *)indexes;

- (ObjectType)objectAtIndexedSubscript:(NSUInteger)idx API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

- (void)enumerateObjectsUsingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateObjectsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateObjectsAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSUInteger)indexOfObjectPassingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSUInteger)indexOfObjectWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSUInteger)indexOfObjectAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSIndexSet *)indexesOfObjectsPassingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSIndexSet *)indexesOfObjectsWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSIndexSet *)indexesOfObjectsAtIndexes:(NSIndexSet *)s options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSArray<ObjectType> *)sortedArrayUsingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSArray<ObjectType> *)sortedArrayWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

typedef NS_OPTIONS(NSUInteger, NSBinarySearchingOptions) {
	NSBinarySearchingFirstEqual = (1UL << 8),
	NSBinarySearchingLastEqual = (1UL << 9),
	NSBinarySearchingInsertionIndex = (1UL << 10),
};

- (NSUInteger)indexOfObject:(ObjectType)obj inSortedRange:(NSRange)r options:(NSBinarySearchingOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmp API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)); // binary search

@end

@interface NSArray<ObjectType> (NSArrayCreation)

+ (instancetype)array;
+ (instancetype)arrayWithObject:(ObjectType)anObject;
+ (instancetype)arrayWithObjects:(const ObjectType _Nonnull [_Nonnull])objects count:(NSUInteger)cnt;
+ (instancetype)arrayWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
+ (instancetype)arrayWithArray:(NSArray<ObjectType> *)array;

- (instancetype)initWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
- (instancetype)initWithArray:(NSArray<ObjectType> *)array;
- (instancetype)initWithArray:(NSArray<ObjectType> *)array copyItems:(BOOL)flag;

/* Reads array stored in NSPropertyList format from the specified url. */
- (nullable NSArray<ObjectType> *)initWithContentsOfURL:(NSURL *)url error:(NSError **)error  API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));
/* Reads array stored in NSPropertyList format from the specified url. */
+ (nullable NSArray<ObjectType> *)arrayWithContentsOfURL:(NSURL *)url error:(NSError **)error API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0)) NS_SWIFT_UNAVAILABLE("Use initializer instead");

@end

API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0))
NS_SWIFT_UNAVAILABLE("NSArray diffing methods are not available in Swift, use Collection.difference(from:) instead")
@interface NSArray<ObjectType> (NSArrayDiffing)

- (NSOrderedCollectionDifference<ObjectType> *)differenceFromArray:(NSArray<ObjectType> *)other withOptions:(NSOrderedCollectionDifferenceCalculationOptions)options usingEquivalenceTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj1, ObjectType obj2))block;

- (NSOrderedCollectionDifference<ObjectType> *)differenceFromArray:(NSArray<ObjectType> *)other withOptions:(NSOrderedCollectionDifferenceCalculationOptions)options;

// Uses isEqual: to determine the difference between the parameter and the receiver
- (NSOrderedCollectionDifference<ObjectType> *)differenceFromArray:(NSArray<ObjectType> *)other;

- (nullable NSArray<ObjectType> *)arrayByApplyingDifference:(NSOrderedCollectionDifference<ObjectType> *)difference;

@end

@interface NSArray<ObjectType> (NSDeprecated)

/* This method is unsafe because it could potentially cause buffer overruns. You should use -getObjects:range: instead.
 */

- (void)getObjects:(ObjectType _Nonnull __unsafe_unretained [_Nonnull])objects NS_SWIFT_UNAVAILABLE("Use 'as [AnyObject]' instead") API_DEPRECATED("Use -getObjects:range: instead", macos(10.0, 10.13), ios(2.0, 11.0), watchos(2.0, 4.0), tvos(9.0, 11.0));

/* These methods are deprecated, and will be marked with API_DEPRECATED in a subsequent release. Use the variants that use errors instead. */
+ (nullable NSArray<ObjectType> *)arrayWithContentsOfFile:(NSString *)path API_DEPRECATED_WITH_REPLACEMENT("arrayWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
+ (nullable NSArray<ObjectType> *)arrayWithContentsOfURL:(NSURL *)url API_DEPRECATED_WITH_REPLACEMENT("arrayWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSArray<ObjectType> *)initWithContentsOfFile:(NSString *)path API_DEPRECATED_WITH_REPLACEMENT("initWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (nullable NSArray<ObjectType> *)initWithContentsOfURL:(NSURL *)url API_DEPRECATED_WITH_REPLACEMENT("initWithContentsOfURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile API_DEPRECATED_WITH_REPLACEMENT("writeToURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));
- (BOOL)writeToURL:(NSURL *)url atomically:(BOOL)atomically API_DEPRECATED_WITH_REPLACEMENT("writeToURL:error:", macos(10.0, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

@end

/****************	Mutable Array		****************/

@interface NSMutableArray<ObjectType> : NSArray<ObjectType>

- (void)addObject:(ObjectType)anObject;
- (void)insertObject:(ObjectType)anObject atIndex:(NSUInteger)index;
- (void)removeLastObject;
- (void)removeObjectAtIndex:(NSUInteger)index;
- (void)replaceObjectAtIndex:(NSUInteger)index withObject:(ObjectType)anObject;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSMutableArray<ObjectType> (NSExtendedMutableArray)
    
- (void)addObjectsFromArray:(NSArray<ObjectType> *)otherArray;
- (void)exchangeObjectAtIndex:(NSUInteger)idx1 withObjectAtIndex:(NSUInteger)idx2;
- (void)removeAllObjects;
- (void)removeObject:(ObjectType)anObject inRange:(NSRange)range;
- (void)removeObject:(ObjectType)anObject;
- (void)removeObjectIdenticalTo:(ObjectType)anObject inRange:(NSRange)range;
- (void)removeObjectIdenticalTo:(ObjectType)anObject;
- (void)removeObjectsFromIndices:(NSUInteger *)indices numIndices:(NSUInteger)cnt API_DEPRECATED("Not supported", macos(10.0,10.6), ios(2.0,4.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)removeObjectsInArray:(NSArray<ObjectType> *)otherArray;
- (void)removeObjectsInRange:(NSRange)range;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray<ObjectType> *)otherArray range:(NSRange)otherRange;
- (void)replaceObjectsInRange:(NSRange)range withObjectsFromArray:(NSArray<ObjectType> *)otherArray;
- (void)setArray:(NSArray<ObjectType> *)otherArray;
- (void)sortUsingFunction:(NSInteger (NS_NOESCAPE *)(ObjectType,  ObjectType, void * _Nullable))compare context:(nullable void *)context;
- (void)sortUsingSelector:(SEL)comparator;

- (void)insertObjects:(NSArray<ObjectType> *)objects atIndexes:(NSIndexSet *)indexes;
- (void)removeObjectsAtIndexes:(NSIndexSet *)indexes;
- (void)replaceObjectsAtIndexes:(NSIndexSet *)indexes withObjects:(NSArray<ObjectType> *)objects;

- (void)setObject:(ObjectType)obj atIndexedSubscript:(NSUInteger)idx API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

- (void)sortUsingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)sortWithOptions:(NSSortOptions)opts usingComparator:(NSComparator NS_NOESCAPE)cmptr API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

@interface NSMutableArray<ObjectType> (NSMutableArrayCreation)

+ (instancetype)arrayWithCapacity:(NSUInteger)numItems;

+ (nullable NSMutableArray<ObjectType> *)arrayWithContentsOfFile:(NSString *)path;
+ (nullable NSMutableArray<ObjectType> *)arrayWithContentsOfURL:(NSURL *)url;
- (nullable NSMutableArray<ObjectType> *)initWithContentsOfFile:(NSString *)path;
- (nullable NSMutableArray<ObjectType> *)initWithContentsOfURL:(NSURL *)url;

@end

API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0))
NS_SWIFT_UNAVAILABLE("NSMutableArray diffing methods are not available in Swift")
@interface NSMutableArray<ObjectType> (NSMutableArrayDiffing)

- (void)applyDifference:(NSOrderedCollectionDifference<ObjectType> *)difference;

@end

NS_ASSUME_NONNULL_END
