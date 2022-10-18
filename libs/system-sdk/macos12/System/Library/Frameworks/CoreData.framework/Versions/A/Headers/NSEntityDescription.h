/*
    NSEntityDescription.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSRange.h>

NS_ASSUME_NONNULL_BEGIN

@class NSManagedObjectModel;
@class NSManagedObjectContext;
@class NSManagedObject;
@class NSData;
@class NSPropertyDescription;
@class NSRelationshipDescription;
@class NSAttributeDescription;
@class NSFetchIndexDescription;
@class NSExpression;

// Entities describe the "types" of objects available.
API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSEntityDescription : NSObject <NSCoding, NSCopying, NSFastEnumeration> {
}

+ (nullable NSEntityDescription *)entityForName:(NSString *)entityName inManagedObjectContext:(NSManagedObjectContext *)context;
+ (__kindof NSManagedObject *)insertNewObjectForEntityForName:(NSString *)entityName inManagedObjectContext:(NSManagedObjectContext *)context;

@property (readonly, assign) NSManagedObjectModel *managedObjectModel;
@property (null_resettable, copy) NSString *managedObjectClassName;

@property (nullable, copy) NSString *name;

@property (getter=isAbstract) BOOL abstract;
@property (readonly, copy) NSDictionary<NSString *, NSEntityDescription *> *subentitiesByName;
@property (strong) NSArray<NSEntityDescription *> *subentities;
@property (nullable, readonly, assign) NSEntityDescription *superentity;

@property (readonly, copy) NSDictionary<NSString *, __kindof NSPropertyDescription *> *propertiesByName;
@property (strong) NSArray<__kindof NSPropertyDescription *> *properties;

@property (nullable, nonatomic, strong) NSDictionary *userInfo;

// convenience methods to get the most common (and most relevant) types of properties for an entity
@property (readonly, copy) NSDictionary<NSString *, NSAttributeDescription *> *attributesByName;
@property (readonly, copy) NSDictionary<NSString *, NSRelationshipDescription *> *relationshipsByName;
- (NSArray<NSRelationshipDescription *> *)relationshipsWithDestinationEntity:(NSEntityDescription *)entity;

/* Returns a boolean indicating if the receiver is a subentity of the specified entity.  (This method is the Core Data entity inheritance equivalent of -isKindOfClass:)
*/
- (BOOL)isKindOfEntity:(NSEntityDescription *)entity API_AVAILABLE(macosx(10.5),ios(3.0));

/* Returns the version hash for the entity.  The version hash is used to uniquely identify an entity based on the collection and configuration of properties for the entity.  The version hash uses only values which affect the persistence of data and the user-defined versionHashModifier value.  (The values which affect persistence are the name of the entity, the version hash of the superentity (if present), if the entity is abstract, and all of the version hashes for the properties.)  This value is stored as part of the version information in the metadata for stores which use this entity, as well as a definition of an entity involved in an NSEntityMapping.
*/
@property (readonly, copy) NSData *versionHash API_AVAILABLE(macosx(10.5),ios(3.0));

/* Returns/sets the version hash modifier for the entity.  This value is included in the version hash for the entity, allowing developers to mark/denote an entity as being a different "version" than another, even if all of the values which affect persistence are equal.  (Such a difference is important in cases where the structure of an entity is unchanged, but the format or content of data has changed.)
*/
@property (nullable, copy) NSString *versionHashModifier API_AVAILABLE(macosx(10.5),ios(3.0));

@property (nullable, copy) NSString *renamingIdentifier API_AVAILABLE(macosx(10.6),ios(3.0));

/* Returns/sets the set of indexes for the entity. Returns/takes an array of NSFetchIndexDescription instances. This value does not form part of the entity's version hash, and may be ignored by stores which do not natively support indexing.
    IMPORTANT: Indexes should be the last things set in a model. Changing an entity hierarchy in any way that could affect the validity of indexes (adding or removing super/subentities, adding or removing properties anywhere in the hierarchy) will cause all exisiting indexes for entities in that hierarchy to be dropped.
 */
@property (copy) NSArray <NSFetchIndexDescription *>*indexes API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

/* Returns/sets uniqueness constraints for the entity. A uniqueness constraint is a set of one or more attributes whose value must be unique over the set of instances of that entity.

    Sets an array of arrays, each of which contains one or more NSAttributeDescription or NSString instances (strings must be the names of attributes on the entity) on which the constraint is registered.
    Returns an array of arrays, each of which contains instances of NSString which identify the attributes on the entity that comprise the constraint.
    This value forms part of the entity's version hash. Stores which do not support uniqueness constraints should refuse to initialize when given a model containing such constraints.
    Discussion: uniqueness constraint violations can be computationally expensive to handle. It is highly suggested that there be only one uniqueness constraint per entity hierarchy,
    although subentites may extend a sueprentity's constraint.
*/
@property (strong)NSArray<NSArray<id> *> *uniquenessConstraints API_AVAILABLE(macosx(10.11),ios(9.0));

/*  Getter returns an array of arrays of NSPropertyDescription objects describing the components of the indexes.
    Setter takes an array of arrays of NSPropertyDescription objects and/or strings that are the names of properties of the entity on which the index is created. The special strings @"self" and @"entity" can be used to indicate that an index should contain a reference to the object's primary or entity key.
    This value does not form part of the entity's version hash, and may be ignored by stores which do not natively support compound indexes.
 */
@property (strong) NSArray<NSArray<id> *> *compoundIndexes API_DEPRECATED( "Use NSEntityDescription.indexes instead", macosx(10.5,10.13),ios(3.0,11.0),tvos(9.0, 11.0),watchos(2.0, 4.0));

/* Expression used to compute the CoreSpotlight display name for instance of this entity. */
@property (nonatomic, retain) NSExpression *coreSpotlightDisplayNameExpression API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

@end

NS_ASSUME_NONNULL_END
