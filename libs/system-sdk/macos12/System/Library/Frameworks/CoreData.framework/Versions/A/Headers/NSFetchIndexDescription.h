/*
    NSFetchIndexDescription.h
    Core Data
    Copyright (c) 2017-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSPredicate.h>
#import <Foundation/NSString.h>

@class NSEntityDescription;
@class NSFetchIndexElementDescription;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0))
@interface NSFetchIndexDescription : NSObject <NSCoding,NSCopying> {
}

- (instancetype)initWithName:(NSString*)name elements:(nullable NSArray <NSFetchIndexElementDescription *>*)elements;

@property (copy) NSString *name;
/* Will throw if the new value is invalid (ie includes both rtree and non-rtree elements). */
@property (copy) NSArray <NSFetchIndexElementDescription *>*elements;
@property (readonly, nonatomic, nullable, assign) NSEntityDescription *entity;

/* If the index should be a partial index, specifies the predicate selecting rows for indexing */
@property (nullable, copy) NSPredicate *partialIndexPredicate;

@end

NS_ASSUME_NONNULL_END
