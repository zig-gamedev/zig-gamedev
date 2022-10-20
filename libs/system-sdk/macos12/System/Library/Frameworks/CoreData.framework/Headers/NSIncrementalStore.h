/*
    NSIncrementalStore.h
    Core Data
    Copyright (c) 2009-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <CoreData/NSPersistentStore.h>
#import <CoreData/NSPersistentStoreRequest.h>

NS_ASSUME_NONNULL_BEGIN

@class NSManagedObjectContext;
@class NSManagedObjectID;
@class NSManagedObject;
@class NSRelationshipDescription;
@class NSIncrementalStoreNode;
@class NSEntityDescription;

// Abstract class defining the API through which Core Data communicates with a store. 
// This API is designed to allow users to create persistent stores which load and save 
// data incrementally, allowing for the management of large and/or shared datasets.
API_AVAILABLE(macosx(10.7),ios(5.0))
@interface NSIncrementalStore : NSPersistentStore {
}

// CoreData expects loadMetadata: to validate that the URL used to create the store is usable
// (location exists, is writable (if applicable), schema is compatible, etc) and return an 
// error if there is an issue. 
// Any subclass of NSIncrementalStore which is file-based must be able to handle being initialized
// with a URL pointing to a zero-length file. This serves as an indicator that a new store is to be
// constructed at the specified location and allows applications using the store to securly create
// reservation files in known locations.
-(BOOL)loadMetadata:(NSError **)error; 

// API methods that must be overriden by a subclass:

// Return a value as appropriate for the request, or nil if the request cannot be completed.
// If the request is a fetch request whose result type is set to one of NSManagedObjectResultType, 
// NSManagedObjectIDResultType, NSDictionaryResultType, return an array containing all objects
// in the store matching the request.
// If the request is a fetch request whose result type is set to NSCountResultType, return an 
// array containing an NSNumber of all objects in the store matching the request.
// If the request is a save request, the result should be an empty array. Note that 
// save requests may have nil inserted/updated/deleted/locked collections; this should be
// treated as a request to save the store metadata.
// Note that subclasses of NSIncrementalStore should implement this method conservatively, 
// and expect that unknown request types may at some point be passed to the 
// method. The correct behavior in these cases would be to return nil and an error.
- (nullable id)executeRequest:(NSPersistentStoreRequest *)request withContext:(nullable NSManagedObjectContext*)context error:(NSError **)error;

// Returns an NSIncrementalStoreNode encapsulating the persistent external values for the object for an objectID. 
// It should include all attributes values and may include to-one relationship values as NSManagedObjectIDs.
// Should return nil and set the error if the object cannot be found.
- (nullable NSIncrementalStoreNode *)newValuesForObjectWithID:(NSManagedObjectID*)objectID withContext:(NSManagedObjectContext*)context error:(NSError**)error;

// Returns the relationship for the given relationship on the object with ID objectID. If the relationship
// is a to-one it should return an NSManagedObjectID corresponding to the destination or NSNull if the relationship value is nil.
// If the relationship is a to-many, should return an NSSet or NSArray containing the NSManagedObjectIDs of the related objects.
// Should return nil and set the error if the source object cannot be found.
- (nullable id)newValueForRelationship:(NSRelationshipDescription*)relationship forObjectWithID:(NSManagedObjectID*)objectID withContext:(nullable NSManagedObjectContext *)context error:(NSError **)error;


// API methods that may be overriden:
+ (id)identifierForNewStoreAtURL:(NSURL*)storeURL;

// Called before executeRequest with a save request, to assign permanent IDs to newly inserted objects; 
// must return the objectIDs in the same order as the objects appear in array.
- (nullable NSArray<NSManagedObjectID *> *)obtainPermanentIDsForObjects:(NSArray<NSManagedObject *> *)array error:(NSError **)error;

// Inform the store that the objects with ids in objectIDs are in use in a client NSManagedObjectContext
- (void)managedObjectContextDidRegisterObjectsWithIDs:(NSArray<NSManagedObjectID *> *)objectIDs;

// Inform the store that the objects with ids in objectIDs are no longer in use in a client NSManagedObjectContext
- (void)managedObjectContextDidUnregisterObjectsWithIDs:(NSArray<NSManagedObjectID *> *)objectIDs;


// API utility methods that should not be overriden (implemented by NSIncrementalStore):

// Returns a new objectID with retain count 1 that uses data as the key.
- (NSManagedObjectID *)newObjectIDForEntity:(NSEntityDescription *)entity referenceObject:(id)data;
// Returns the reference data used to construct the objectID. Will raise an NSInvalidArgumentException if the objectID was not created
// by this store.
- (id)referenceObjectForObjectID:(NSManagedObjectID *)objectID;

@end

NS_ASSUME_NONNULL_END
