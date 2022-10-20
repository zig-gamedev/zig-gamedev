/*
    NSIncrementalStoreNode.h
    Core Data
    Copyright (c) 2010-2021, Apple Inc.
    All rights reserved.
*/

#import <CoreData/NSManagedObjectID.h>
#import <Foundation/NSDictionary.h>

NS_ASSUME_NONNULL_BEGIN

@class NSPropertyDescription;

// Provides the basic unit of external data that the Core Data stack interacts with.
API_AVAILABLE(macosx(10.7),ios(5.0))
@interface NSIncrementalStoreNode : NSObject {
}

// Returns an object initialized with the following values
// objectID -> The NSManagedObjectID corresponding to the object whose values are cached
//
// values -> A dictionary containing the values persisted in an external store with keys corresponding to the names of the NSPropertyDescriptions
//      in the NSEntityDescription described by the NSManagedObjectID.  Unknown or unmodeled keys will be stripped out.
//
//		For attributes: an immutable value (NSNumber, NSString, NSData etc).  Missing attribute keys will assume a nil value.
//
//		For to-one relationships: the NSManagedObjectID of the related object or NSNull for nil relationship values. A missing key will be resolved lazily through calling
//          -newValueForRelationship:forObjectWithID:withContext:error: on the NSPersistentStore.  Lazy resolution for to-ones is discouraged.
//
//      For to-many relationships: an NSArray or NSSet containing the NSManagedObjectIDs of the related objects.  Empty to-many relationships must
//          be represented by an empty non-nil collection.  A missing key will be resolved lazily through calling.  Lazy resolution for to-manys is encouraged.
//          -newValueForRelationship:forObjectWithID:withContext:error: on the NSPersistentStore
//
// version -> The revision number of this state; used for conflict detection and merging 
- (instancetype)initWithObjectID:(NSManagedObjectID*)objectID withValues:(NSDictionary<NSString *, id> *)values version:(uint64_t)version;

// Update the values and version to reflect new data being saved to or loaded from the external store.  
// The values dictionary is in the same format as the initializer
- (void)updateWithValues:(NSDictionary<NSString *, id> *)values version:(uint64_t)version;

// Return the object ID that identifies the data stored by this node
@property (nonatomic, readonly, strong) NSManagedObjectID *objectID;

// Return the version of data in this node.
@property (nonatomic, readonly) uint64_t version;

// May return NSNull for to-one relationships.  If a relationship is nil, clients should  invoke -newValueForRelationship:forObjectWithID:withContext:error: on the NSPersistentStore
- (nullable id)valueForPropertyDescription:(NSPropertyDescription*)prop;

@end

NS_ASSUME_NONNULL_END
