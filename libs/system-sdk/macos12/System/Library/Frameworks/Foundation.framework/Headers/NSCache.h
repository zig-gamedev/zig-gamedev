/*      NSCache.h
        Copyright (c) 2008-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString;
@protocol NSCacheDelegate;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0))
@interface NSCache <KeyType, ObjectType> : NSObject {
@private
    id _delegate;
    void *_private[5];
    void *_reserved;
}

@property (copy) NSString *name;

@property (nullable, assign) id<NSCacheDelegate> delegate;

- (nullable ObjectType)objectForKey:(KeyType)key;
- (void)setObject:(ObjectType)obj forKey:(KeyType)key; // 0 cost
- (void)setObject:(ObjectType)obj forKey:(KeyType)key cost:(NSUInteger)g;
- (void)removeObjectForKey:(KeyType)key;

- (void)removeAllObjects;

@property NSUInteger totalCostLimit;	// limits are imprecise/not strict
@property NSUInteger countLimit;	// limits are imprecise/not strict
@property BOOL evictsObjectsWithDiscardedContent;

@end

@protocol NSCacheDelegate <NSObject>
@optional
- (void)cache:(NSCache *)cache willEvictObject:(id)obj;
@end

NS_ASSUME_NONNULL_END
