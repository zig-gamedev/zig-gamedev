/*
    NSBatchDeleteRequest.h
    Core Data
    Copyright (c) 2015-2021, Apple Inc.
    All rights reserved.
*/


#import <Foundation/NSArray.h>

#import <CoreData/NSPersistentStoreRequest.h>
#import <CoreData/NSPersistentStoreResult.h>

@class NSFetchRequest;
@class NSManagedObjectID;

NS_ASSUME_NONNULL_BEGIN

//  Used to request that Core Data do a batch deletion of objects without faulting in
//  objects to be deleted.
//  May not be supported by all store types.
//  WARNING:
//  It is up to the developer creating the request to ensure that changes made by the request to
//  the underlying store do not violate any validation rules specified in the model beyond basic
//  delete rules (for example, minimum relationship counts).
API_AVAILABLE(macosx(10.11),ios(9.0))
@interface NSBatchDeleteRequest : NSPersistentStoreRequest {
}

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithFetchRequest:(NSFetchRequest *)fetch NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithObjectIDs:(NSArray<NSManagedObjectID *> *)objects;

// The type of result that should be returned from this request. Defaults to NSBatchDeleteResultTypeStatusOnly
@property NSBatchDeleteRequestResultType resultType;

@property (readonly, copy) NSFetchRequest *fetchRequest;


@end

NS_ASSUME_NONNULL_END
