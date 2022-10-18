/*
    NSAtomicStore.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSString.h>
#import <Foundation/NSError.h>
#import <Foundation/NSURL.h>
#import <CoreData/NSPersistentStore.h>
#import <CoreData/NSAtomicStoreCacheNode.h>
#import <CoreData/NSManagedObject.h>

NS_ASSUME_NONNULL_BEGIN

@class NSPersistentStoreCoordinator;
@class NSEntityDescription;
@class NSManagedObjectID;

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSAtomicStore : NSPersistentStore {
}

// API method that may be overriden by subclasses for custom initialization
- (instancetype)initWithPersistentStoreCoordinator:(nullable NSPersistentStoreCoordinator *)coordinator configurationName:(nullable NSString *)configurationName URL:(NSURL *)url options:(nullable NSDictionary *)options;

// API method that must be overriden by subclasses
- (BOOL)load:(NSError **)error;

// API method that must be overriden by subclasses
- (BOOL)save:(NSError **)error;

// API method that must be overriden by subclasses
- (NSAtomicStoreCacheNode *)newCacheNodeForManagedObject:(NSManagedObject *)managedObject;

// API method that must be overriden by subclasses
- (void)updateCacheNode:(NSAtomicStoreCacheNode *)node fromManagedObject:(NSManagedObject *)managedObject;

// Utility method with implementation provided by NSAtomicStore
- (NSSet<__kindof NSAtomicStoreCacheNode *> *)cacheNodes;

// Utility method with implementation provided by NSAtomicStore
- (void)addCacheNodes:(NSSet<__kindof NSAtomicStoreCacheNode *> *)cacheNodes;

// Called before cache nodes are removed
- (void)willRemoveCacheNodes:(NSSet<__kindof NSAtomicStoreCacheNode *> *)cacheNodes;

// Utility method with implementation provided by NSAtomicStore
- (nullable NSAtomicStoreCacheNode *)cacheNodeForObjectID:(NSManagedObjectID *)objectID;

// Utility method with implementation provided by NSAtomicStore
- (NSManagedObjectID *)objectIDForEntity:(NSEntityDescription *)entity referenceObject:(id)data;

// This method MUST return a new unique primary key reference data for an instance of entity. This 
// primary key value MUST be an id

// API method that must be overriden by subclasses
- (id)newReferenceObjectForManagedObject:(NSManagedObject *)managedObject;

// Utility method with implementation provided by NSAtomicStore
- (id)referenceObjectForObjectID:(NSManagedObjectID *)objectID;

@end

NS_ASSUME_NONNULL_END
