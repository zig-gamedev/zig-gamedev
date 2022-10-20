/*	NSIndexSet.h
	Copyright (c) 2002-2019, Apple Inc. All rights reserved.
*/

/* Class for managing set of indexes. The set of valid indexes are 0 .. NSNotFound - 1; trying to use indexes outside this range is an error.  NSIndexSet uses NSNotFound as a return value in cases where the queried index doesn't exist in the set; for instance, when you ask firstIndex and there are no indexes; or when you ask for indexGreaterThanIndex: on the last index, and so on.

The following code snippets can be used to enumerate over the indexes in an NSIndexSet:

    // Forward
    NSUInteger currentIndex = [set firstIndex];
    while (currentIndex != NSNotFound) {
        ...
        currentIndex = [set indexGreaterThanIndex:currentIndex];
    }
    
    // Backward
    NSUInteger currentIndex = [set lastIndex];
    while (currentIndex != NSNotFound) {
        ...
        currentIndex = [set indexLessThanIndex:currentIndex];
    }

To enumerate without doing a call per index, you can use the method getIndexes:maxCount:inIndexRange:.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRange.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSIndexSet : NSObject <NSCopying, NSMutableCopying, NSSecureCoding> {
    @protected   // all instance variables are private
    struct {
        NSUInteger _isEmpty:1;
        NSUInteger _hasSingleRange:1;
        NSUInteger _cacheValid:1;
        NSUInteger _reservedArrayBinderController:29;
    } _indexSetFlags;
    union {
        struct {
            NSRange _range;
        } _singleRange;
        struct {
            void * _data;
            void *_reserved;
        } _multipleRanges;
    } _internal;
}

+ (instancetype)indexSet;
+ (instancetype)indexSetWithIndex:(NSUInteger)value;
+ (instancetype)indexSetWithIndexesInRange:(NSRange)range;

- (instancetype)initWithIndexesInRange:(NSRange)range NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithIndexSet:(NSIndexSet *)indexSet NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithIndex:(NSUInteger)value;

- (BOOL)isEqualToIndexSet:(NSIndexSet *)indexSet;

@property (readonly) NSUInteger count;

/* The following six methods will return NSNotFound if there is no index in the set satisfying the query. 
*/
@property (readonly) NSUInteger firstIndex;
@property (readonly) NSUInteger lastIndex;
- (NSUInteger)indexGreaterThanIndex:(NSUInteger)value;
- (NSUInteger)indexLessThanIndex:(NSUInteger)value;
- (NSUInteger)indexGreaterThanOrEqualToIndex:(NSUInteger)value;
- (NSUInteger)indexLessThanOrEqualToIndex:(NSUInteger)value;

/* Fills up to bufferSize indexes in the specified range into the buffer and returns the number of indexes actually placed in the buffer; also modifies the optional range passed in by pointer to be "positioned" after the last index filled into the buffer.Example: if the index set contains the indexes 0, 2, 4, ..., 98, 100, for a buffer of size 10 and the range (20, 80) the buffer would contain 20, 22, ..., 38 and the range would be modified to (40, 60).
*/
- (NSUInteger)getIndexes:(NSUInteger *)indexBuffer maxCount:(NSUInteger)bufferSize inIndexRange:(nullable NSRangePointer)range;

- (NSUInteger)countOfIndexesInRange:(NSRange)range API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

- (BOOL)containsIndex:(NSUInteger)value;
- (BOOL)containsIndexesInRange:(NSRange)range;
- (BOOL)containsIndexes:(NSIndexSet *)indexSet;

- (BOOL)intersectsIndexesInRange:(NSRange)range;

- (void)enumerateIndexesUsingBlock:(void (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateIndexesWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (void)enumerateIndexesInRange:(NSRange)range options:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSUInteger)indexPassingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSUInteger)indexWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSUInteger)indexInRange:(NSRange)range options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (NSIndexSet *)indexesPassingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSIndexSet *)indexesWithOptions:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (NSIndexSet *)indexesInRange:(NSRange)range options:(NSEnumerationOptions)opts passingTest:(BOOL (NS_NOESCAPE ^)(NSUInteger idx, BOOL *stop))predicate API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

/*
 The following three convenience methods allow you to enumerate the indexes in the receiver by ranges of contiguous indexes. The performance of these methods is not guaranteed to be any better than if they were implemented with enumerateIndexesInRange:options:usingBlock:. However, depending on the receiver's implementation, they may perform better than that.
 
 If the specified range for enumeration intersects a range of contiguous indexes in the receiver, then the block will be invoked with the intersection of those two ranges.
*/
- (void)enumerateRangesUsingBlock:(void (NS_NOESCAPE ^)(NSRange range, BOOL *stop))block API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)enumerateRangesWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSRange range, BOOL *stop))block API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (void)enumerateRangesInRange:(NSRange)range options:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(NSRange range, BOOL *stop))block API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@end

@interface NSMutableIndexSet : NSIndexSet {
    @protected
    void *_reserved;
}

- (void)addIndexes:(NSIndexSet *)indexSet;
- (void)removeIndexes:(NSIndexSet *)indexSet;
- (void)removeAllIndexes;
- (void)addIndex:(NSUInteger)value;
- (void)removeIndex:(NSUInteger)value;
- (void)addIndexesInRange:(NSRange)range;
- (void)removeIndexesInRange:(NSRange)range;

/* For a positive delta, shifts the indexes in [index, INT_MAX] to the right, thereby inserting an "empty space" [index, delta], for a negative delta, shifts the indexes in [index, INT_MAX] to the left, thereby deleting the indexes in the range [index - delta, delta].
*/
- (void)shiftIndexesStartingAtIndex:(NSUInteger)index by:(NSInteger)delta;   

@end

NS_ASSUME_NONNULL_END
