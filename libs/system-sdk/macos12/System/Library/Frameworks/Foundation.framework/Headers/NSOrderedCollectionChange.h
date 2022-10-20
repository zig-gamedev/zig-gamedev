/*	NSOrderedCollectionChange.h
	Copyright (c) 2017-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, NSCollectionChangeType) {
    NSCollectionChangeInsert,
    NSCollectionChangeRemove
} API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0));

API_AVAILABLE(macosx(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSOrderedCollectionChange<ObjectType> : NSObject
#ifndef __OBJC2__
{
@private
    id _object;
    NSCollectionChangeType _changeType;
    NSUInteger _index;
    NSUInteger _associatedIndex;
}
#endif // !__OBJC2__

+ (NSOrderedCollectionChange<ObjectType> *)changeWithObject:(nullable ObjectType)anObject
                                                       type:(NSCollectionChangeType)type
                                                      index:(NSUInteger)index;

+ (NSOrderedCollectionChange<ObjectType> *)changeWithObject:(nullable ObjectType)anObject
                                                       type:(NSCollectionChangeType)type
                                                      index:(NSUInteger)index
                                            associatedIndex:(NSUInteger)associatedIndex;

// The object that was inserted or removed, if recorded
@property (readonly, strong, nullable) ObjectType object;

// The change type: insert or remove
@property (readonly) NSCollectionChangeType changeType;

// For removes, the index of the object in the original state.
// For inserts, the index of the object in the final state.
@property (readonly) NSUInteger index;

// When non-NSNotFound, indicates that this change is one half of a move, with
// this value referring to the index of the other change that completes it.
// For differences produced by identity comparison (instead of equality), each
// change representing a move operation may store different objects.
@property (readonly) NSUInteger associatedIndex;

- (id)init API_UNAVAILABLE(macos, ios, watchos, tvos);

- (instancetype)initWithObject:(nullable ObjectType)anObject
                          type:(NSCollectionChangeType)type
                         index:(NSUInteger)index;

- (instancetype)initWithObject:(nullable ObjectType)anObject
                          type:(NSCollectionChangeType)type
                         index:(NSUInteger)index
               associatedIndex:(NSUInteger)associatedIndex NS_DESIGNATED_INITIALIZER;
@end

NS_ASSUME_NONNULL_END
