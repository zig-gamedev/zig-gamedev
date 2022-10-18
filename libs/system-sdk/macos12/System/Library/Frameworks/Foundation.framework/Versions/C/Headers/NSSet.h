/*	NSSet.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSEnumerator.h>

@class NSArray, NSDictionary, NSString;

/****************	Immutable Set	****************/

NS_ASSUME_NONNULL_BEGIN

@interface NSSet<__covariant ObjectType> : NSObject <NSCopying, NSMutableCopying, NSSecureCoding, NSFastEnumeration>

@property (readonly) NSUInteger count;
- (nullable ObjectType)member:(ObjectType)object;
- (NSEnumerator<ObjectType> *)objectEnumerator;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithObjects:(const ObjectType _Nonnull [_Nullable])objects count:(NSUInteger)cnt NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end

@interface NSSet<ObjectType> (NSExtendedSet)

@property (readonly, copy) NSArray<ObjectType> *allObjects;
- (nullable ObjectType)anyObject;
- (BOOL)containsObject:(ObjectType)anObject;
@property (readonly, copy) NSString *description;
- (NSString *)descriptionWithLocale:(nullable id)locale;
- (BOOL)intersectsSet:(NSSet<ObjectType> *)otherSet;
- (BOOL)isEqualToSet:(NSSet<ObjectType> *)otherSet;
- (BOOL)isSubsetOfSet:(NSSet<ObjectType> *)otherSet;

- (void)makeObjectsPerformSelector:(SEL)aSelector NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");
- (void)makeObjectsPerformSelector:(SEL)aSelector withObject:(nullable id)argument NS_SWIFT_UNAVAILABLE("Use enumerateObjectsUsingBlock: or a for loop instead");

- (NSSet<ObjectType> *)setByAddingObject:(ObjectType)anObject API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (NSSet<ObjectType> *)setByAddingObjectsFromSet:(NSSet<ObjectType> *)other API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (NSSet<ObjectType> *)setByAddingObjectsFromArray:(NSArray<ObjectType> *)other API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

- (void)enumerateObjectsUsingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateObjectsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(ObjectType obj, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSSet<ObjectType> *)objectsPassingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSSet<ObjectType> *)objectsWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(ObjectType obj, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@end

@interface NSSet<ObjectType> (NSSetCreation)

+ (instancetype)set;
+ (instancetype)setWithObject:(ObjectType)object;
+ (instancetype)setWithObjects:(const ObjectType _Nonnull [_Nonnull])objects count:(NSUInteger)cnt;
+ (instancetype)setWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
+ (instancetype)setWithSet:(NSSet<ObjectType> *)set;
+ (instancetype)setWithArray:(NSArray<ObjectType> *)array;

- (instancetype)initWithObjects:(ObjectType)firstObj, ... NS_REQUIRES_NIL_TERMINATION;
- (instancetype)initWithSet:(NSSet<ObjectType> *)set;
- (instancetype)initWithSet:(NSSet<ObjectType> *)set copyItems:(BOOL)flag;
- (instancetype)initWithArray:(NSArray<ObjectType> *)array;

@end

/****************	Mutable Set	****************/

@interface NSMutableSet<ObjectType> : NSSet<ObjectType>

- (void)addObject:(ObjectType)object;
- (void)removeObject:(ObjectType)object;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;

@end

@interface NSMutableSet<ObjectType> (NSExtendedMutableSet)

- (void)addObjectsFromArray:(NSArray<ObjectType> *)array;
- (void)intersectSet:(NSSet<ObjectType> *)otherSet;
- (void)minusSet:(NSSet<ObjectType> *)otherSet;
- (void)removeAllObjects;
- (void)unionSet:(NSSet<ObjectType> *)otherSet;

- (void)setSet:(NSSet<ObjectType> *)otherSet;

@end

@interface NSMutableSet<ObjectType> (NSMutableSetCreation)

+ (instancetype)setWithCapacity:(NSUInteger)numItems;

@end

/****************	Counted Set	****************/

@interface NSCountedSet<ObjectType> : NSMutableSet<ObjectType> {
    @private
    id _table;
    void *_reserved;
}

- (instancetype)initWithCapacity:(NSUInteger)numItems NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithArray:(NSArray<ObjectType> *)array;
- (instancetype)initWithSet:(NSSet<ObjectType> *)set;

- (NSUInteger)countForObject:(ObjectType)object;

- (NSEnumerator<ObjectType> *)objectEnumerator;
- (void)addObject:(ObjectType)object;
- (void)removeObject:(ObjectType)object;

@end

NS_ASSUME_NONNULL_END
