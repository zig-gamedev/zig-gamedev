/*	NSOrderedCollectionDifference.h
	Copyright (c) 2017-2019, Apple Inc. All rights reserved.
*/

@class NSArray<ObjectType>;

#import <Foundation/NSOrderedCollectionChange.h>
#import <Foundation/NSIndexSet.h>
#import <Foundation/NSEnumerator.h>

NS_ASSUME_NONNULL_BEGIN

/// Options supported by methods that produce difference objects.
typedef NS_OPTIONS(NSUInteger, NSOrderedCollectionDifferenceCalculationOptions) {
    /// Insertion changes do not store a reference to the inserted object.
    NSOrderedCollectionDifferenceCalculationOmitInsertedObjects = (1 << 0UL),
    /// Insertion changes do not store a reference to the removed object.
    NSOrderedCollectionDifferenceCalculationOmitRemovedObjects = (1 << 1UL),
    /// Assume objects that were uniquely removed and inserted were moved.
    /// This is useful when diffing based on identity instead of equality.
    NSOrderedCollectionDifferenceCalculationInferMoves = (1 << 2UL)
} API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSOrderedCollectionDifference<ObjectType> : NSObject <NSFastEnumeration>
#ifndef __OBJC2__
{
@private
    id _removeIndexes;
    id _removeObjects;
    id _insertIndexes;
    id _insertObjects;
    id _moves;
}
#endif // !__OBJC2__

/// Creates a new difference representing the changes in the parameter.
///
/// For clients interested in the difference between two collections, the
/// collection's differenceFrom method should be used instead.
///
/// To guarantee that instances are unambiguous and safe for compatible base
/// states, this method requires that its parameter conform to the following
/// requirements:
///
/// 1) All insertion offsets are unique
/// 2) All removal offsets are unique
/// 3) All associated indexes match a change with the opposite parity.
- (instancetype)initWithChanges:(NSArray<NSOrderedCollectionChange<ObjectType> *> *)changes;

- (instancetype)initWithInsertIndexes:(NSIndexSet *)inserts
                      insertedObjects:(nullable NSArray<ObjectType> *)insertedObjects
                        removeIndexes:(NSIndexSet *)removes
                       removedObjects:(nullable NSArray<ObjectType> *)removedObjects
                    additionalChanges:(NSArray<NSOrderedCollectionChange<ObjectType> *> *)changes NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithInsertIndexes:(NSIndexSet *)inserts
                      insertedObjects:(nullable NSArray<ObjectType> *)insertedObjects
                        removeIndexes:(NSIndexSet *)removes
                       removedObjects:(nullable NSArray<ObjectType> *)removedObjects;

@property (strong, readonly) NSArray<NSOrderedCollectionChange<ObjectType> *> *insertions API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));
@property (strong, readonly) NSArray<NSOrderedCollectionChange<ObjectType> *> *removals API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@property (assign, readonly) BOOL hasChanges;

// Create a new difference by mapping over this difference's members
- (NSOrderedCollectionDifference<id> *)differenceByTransformingChangesWithBlock:(NSOrderedCollectionChange<id> *(NS_NOESCAPE ^)(NSOrderedCollectionChange<ObjectType> *))block;

// Returns a difference that is the inverse of the receiver.
//
// In other words, given a valid difference `diff` the array `a` is equal to
// [[a arrayByApplyingDifference:diff] arrayByApplyingDifference:diff.inverseDifference]
//
// To revert a chronological sequence of diffs, apply their inverses in reverse order.
- (instancetype)inverseDifference API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

@end

NS_ASSUME_NONNULL_END
