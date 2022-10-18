/*
 *  NSPointerArray.h
 *  Copyright (c) 2005-2019, Apple Inc. All rights reserved.
 *
 */
 

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSPointerFunctions.h>

NS_ASSUME_NONNULL_BEGIN

/*
   NSPointerArray.h

   A PointerArray acts like a traditional array that slides elements on insertion or deletion.
   Unlike traditional arrays, it holds NULLs, which can be inserted or extracted (and contribute to count).
   Also unlike traditional arrays, the 'count' of the array may be set directly.
   Using NSPointerFunctionsWeakMemory object references will turn to NULL on last release.

   The copying and archiving protocols are applicable only when NSPointerArray is configured for Object uses.
   The fast enumeration protocol (supporting the for..in statement) will yield NULLs if present.  It is defined for all types of pointers although the language syntax doesn't directly support this.
*/

API_AVAILABLE(macos(10.5), ios(6.0), watchos(2.0), tvos(9.0))
@interface NSPointerArray : NSObject <NSFastEnumeration, NSCopying, NSSecureCoding>
// construction
- (instancetype)initWithOptions:(NSPointerFunctionsOptions)options NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithPointerFunctions:(NSPointerFunctions *)functions NS_DESIGNATED_INITIALIZER;

+ (NSPointerArray *)pointerArrayWithOptions:(NSPointerFunctionsOptions)options;
+ (NSPointerArray *)pointerArrayWithPointerFunctions:(NSPointerFunctions *)functions;

/* return an NSPointerFunctions object reflecting the functions in use.  This is a new autoreleased object that can be subsequently modified and/or used directly in the creation of other pointer "collections". */
@property (readonly, copy) NSPointerFunctions *pointerFunctions;

- (nullable void *)pointerAtIndex:(NSUInteger)index;

// Array like operations that slide or grow contents, including NULLs
- (void)addPointer:(nullable void *)pointer;  // add pointer at index 'count'
- (void)removePointerAtIndex:(NSUInteger)index;    // everything above index, including holes, slide lower
- (void)insertPointer:(nullable void *)item atIndex:(NSUInteger)index;  // everything at & above index, including holes, slide higher

- (void)replacePointerAtIndex:(NSUInteger)index withPointer:(nullable void *)item;  // O(1); NULL item is okay; index must be < count

- (void)compact;   // eliminate NULLs

// Getter: the number of elements in the array, including NULLs
// Setter: sets desired number of elements, adding NULLs or removing items as necessary.
@property NSUInteger count;

@end


@interface NSPointerArray (NSPointerArrayConveniences)  

// construction
#if TARGET_OS_OSX
+ (id) pointerArrayWithStrongObjects API_DEPRECATED("GC no longer supported", macos(10.5,10.8)) API_UNAVAILABLE(ios, watchos, tvos); // strong objects
+ (id) pointerArrayWithWeakObjects API_DEPRECATED("GC no longer supported", macos(10.5,10.8)) API_UNAVAILABLE(ios, watchos, tvos); // weak objects
#endif

+ (NSPointerArray *)strongObjectsPointerArray API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));
+ (NSPointerArray *)weakObjectsPointerArray API_AVAILABLE(macos(10.8), ios(6.0), watchos(2.0), tvos(9.0));

@property (readonly, copy) NSArray *allObjects;

@end

NS_ASSUME_NONNULL_END
