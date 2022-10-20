/*
    NSBatchUpdateRequest.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSPredicate.h>
#import <Foundation/NSDictionary.h>

#import <CoreData/NSPersistentStoreRequest.h>
#import <CoreData/NSPersistentStoreResult.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;

//  Used to request that Core Data do a batch update of data in a persistent store without
//  loading any data into memory. May not be supported by all store types.
//  WARNING:
//  It is up to the developer creating the request to ensure that changes made by the request to
//  the underlying store do not violate any validation rules specified in the model.
API_AVAILABLE(macosx(10.10),ios(8.0))
@interface NSBatchUpdateRequest : NSPersistentStoreRequest {
}

+ (instancetype)batchUpdateRequestWithEntityName:(NSString*)entityName;

- (instancetype)initWithEntityName:(NSString *)entityName NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithEntity:(NSEntityDescription *)entity NS_DESIGNATED_INITIALIZER;

@property (copy, readonly) NSString* entityName;
@property (strong, readonly) NSEntityDescription *entity;
@property (nullable, strong) NSPredicate *predicate;

// Should the update include subentities? Defaults to YES.
@property BOOL includesSubentities;

// The type of result that should be returned from this request. Defaults to NSStatusOnlyResultType
@property NSBatchUpdateRequestResultType resultType;

// Dictionary of NSPropertyDescription|property name string -> constantValue/NSExpression pairs describing the desired updates.
// The expressions can be any NSExpression that evaluates to a scalar value.
@property (nullable, copy) NSDictionary *propertiesToUpdate;

@end

NS_ASSUME_NONNULL_END
