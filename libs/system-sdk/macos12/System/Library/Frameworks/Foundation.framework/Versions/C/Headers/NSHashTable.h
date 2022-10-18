/*	NSHashTable.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSPointerFunctions.h>
#import <Foundation/NSString.h>
#import <Foundation/NSEnumerator.h>

#if !defined(__FOUNDATION_NSHASHTABLE__)
#define __FOUNDATION_NSHASHTABLE__ 1

@class NSArray<ObjectType>, NSSet<ObjectType>, NSHashTable;

NS_ASSUME_NONNULL_BEGIN

/* An NSHashTable is modeled after a set, although, because of its options, is not a set because it can behave differently (for example, if pointer equality is specified two isEqual strings will both be entered).  The major option is to provide for "weak" references that are removed automatically, but at some indefinite point in the future.
   An NSHashTable can also be configured to operate on arbitrary pointers and not just objects.  We recommend the C function API for "void *" access.  To configure for pointer use, consult and choose the appropriate NSPointerFunctionsOptions or configure or use an NSPointerFunctions object itself for initialization.
*/

static const NSPointerFunctionsOptions NSHashTableStrongMemory API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = NSPointerFunctionsStrongMemory;
static const NSPointerFunctionsOptions NSHashTableZeroingWeakMemory API_DEPRECATED("GC no longer supported", macos(10.5,10.8)) API_UNAVAILABLE(ios, watchos, tvos) = NSPointerFunctionsZeroingWeakMemory;
static const NSPointerFunctionsOptions NSHashTableCopyIn API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = NSPointerFunctionsCopyIn;
static const NSPointerFunctionsOptions NSHashTableObjectPointerPersonality API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0)) = NSPointerFunctionsObjectPointerPersonality;
static const NSPointerFunctionsOptions NSHashTableWeakMemory API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)) = NSPointerFunctionsWeakMemory;

typedef NSUInteger NSHashTableOptions;

API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0))
@interface NSHashTable<ObjectType> : NSObject <NSCopying, NSSecureCoding, NSFastEnumeration>

- (instancetype)initWithOptions:(NSPointerFunctionsOptions)options capacity:(NSUInteger)initialCapacity NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithPointerFunctions:(NSPointerFunctions *)functions capacity:(NSUInteger)initialCapacity NS_DESIGNATED_INITIALIZER;

// conveniences

+ (NSHashTable<ObjectType> *)hashTableWithOptions:(NSPointerFunctionsOptions)options;

+ (id)hashTableWithWeakObjects API_DEPRECATED("GC no longer supported", macos(10.5,10.8)) API_UNAVAILABLE(ios, watchos, tvos);  // GC zeroing, otherwise unsafe unretained

+ (NSHashTable<ObjectType> *)weakObjectsHashTable API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0)); // entries are not necessarily purged right away when the weak object is reclaimed


/* return an NSPointerFunctions object reflecting the functions in use.  This is a new autoreleased object that can be subsequently modified and/or used directly in the creation of other pointer "collections". */
@property (readonly, copy) NSPointerFunctions *pointerFunctions;

@property (readonly) NSUInteger count;
- (nullable ObjectType)member:(nullable ObjectType)object;
- (NSEnumerator<ObjectType> *)objectEnumerator;

- (void)addObject:(nullable ObjectType)object;
- (void)removeObject:(nullable ObjectType)object;

- (void)removeAllObjects;

@property (readonly, copy) NSArray<ObjectType> *allObjects;    // convenience

@property (nullable, nonatomic, readonly) ObjectType anyObject;
- (BOOL)containsObject:(nullable ObjectType)anObject;

- (BOOL)intersectsHashTable:(NSHashTable<ObjectType> *)other;
- (BOOL)isEqualToHashTable:(NSHashTable<ObjectType> *)other;
- (BOOL)isSubsetOfHashTable:(NSHashTable<ObjectType> *)other;

- (void)intersectHashTable:(NSHashTable<ObjectType> *)other;
- (void)unionHashTable:(NSHashTable<ObjectType> *)other;
- (void)minusHashTable:(NSHashTable<ObjectType> *)other;

@property (readonly, copy) NSSet<ObjectType> *setRepresentation;  // create a set of the contents

@end

/****************	(void *) Hash table operations	****************/

typedef struct {NSUInteger _pi; NSUInteger _si; void * _Nullable _bs;} NSHashEnumerator;

FOUNDATION_EXPORT void NSFreeHashTable(NSHashTable *table);
FOUNDATION_EXPORT void NSResetHashTable(NSHashTable *table);
FOUNDATION_EXPORT BOOL NSCompareHashTables(NSHashTable *table1, NSHashTable *table2);
FOUNDATION_EXPORT NSHashTable *NSCopyHashTableWithZone(NSHashTable *table, NSZone * _Nullable zone);
FOUNDATION_EXPORT void *NSHashGet(NSHashTable *table, const void * _Nullable pointer);
FOUNDATION_EXPORT void NSHashInsert(NSHashTable *table, const void * _Nullable pointer);
FOUNDATION_EXPORT void NSHashInsertKnownAbsent(NSHashTable *table, const void * _Nullable pointer);
FOUNDATION_EXPORT void * _Nullable NSHashInsertIfAbsent(NSHashTable *table, const void * _Nullable pointer);
FOUNDATION_EXPORT void NSHashRemove(NSHashTable *table, const void * _Nullable pointer);
FOUNDATION_EXPORT NSHashEnumerator NSEnumerateHashTable(NSHashTable *table);
FOUNDATION_EXPORT void * _Nullable NSNextHashEnumeratorItem(NSHashEnumerator *enumerator);
FOUNDATION_EXPORT void NSEndHashTableEnumeration(NSHashEnumerator *enumerator);
FOUNDATION_EXPORT NSUInteger NSCountHashTable(NSHashTable *table);
FOUNDATION_EXPORT NSString *NSStringFromHashTable(NSHashTable *table);
FOUNDATION_EXPORT NSArray *NSAllHashTableObjects(NSHashTable *table);


/****************	Legacy	****************/

typedef struct {
    NSUInteger	(* _Nullable hash)(NSHashTable *table, const void *);
    BOOL	(* _Nullable isEqual)(NSHashTable *table, const void *, const void *);
    void	(* _Nullable retain)(NSHashTable *table, const void *);
    void	(* _Nullable release)(NSHashTable *table, void *);
    NSString 	* _Nullable (* _Nullable describe)(NSHashTable *table, const void *);
} NSHashTableCallBacks;

FOUNDATION_EXPORT NSHashTable *NSCreateHashTableWithZone(NSHashTableCallBacks callBacks, NSUInteger capacity, NSZone * _Nullable zone);
FOUNDATION_EXPORT NSHashTable *NSCreateHashTable(NSHashTableCallBacks callBacks, NSUInteger capacity);


FOUNDATION_EXPORT const NSHashTableCallBacks NSIntegerHashCallBacks API_AVAILABLE(macos(10.5)) API_UNAVAILABLE(ios, watchos, tvos);
FOUNDATION_EXPORT const NSHashTableCallBacks NSNonOwnedPointerHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSNonRetainedObjectHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSObjectHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSOwnedObjectIdentityHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSOwnedPointerHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSPointerToStructHashCallBacks;
FOUNDATION_EXPORT const NSHashTableCallBacks NSIntHashCallBacks API_DEPRECATED("Not supported", macos(10.0,10.5)) API_UNAVAILABLE(ios, watchos, tvos);

NS_ASSUME_NONNULL_END
#endif // defined __FOUNDATION_NSHASHTABLE__
