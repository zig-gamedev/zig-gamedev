/*
    NSEntityMapping.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSExpression.h>
#import <Foundation/NSData.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;
@class NSPropertyMapping;

/*  mapping types */

typedef NS_ENUM(NSUInteger, NSEntityMappingType) {
    NSUndefinedEntityMappingType    = 0x00,
    NSCustomEntityMappingType       = 0x01,         /* developer handles destination instance creation */
    NSAddEntityMappingType          = 0x02,         /* new entity in destination */
    NSRemoveEntityMappingType	    = 0x03,         /* source instances aren't mapped to destination */
    NSCopyEntityMappingType         = 0x04,         /* migrate instances as-is */
    NSTransformEntityMappingType    = 0x05,         /* entity exists in source and destination and is mapped */
};

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSEntityMapping : NSObject {
}

/* Returns/sets the name of the mapping. The name is used only as a means of distinguishing mappings in a model.  If not specified, defaults to the string composed by the source entity name followed by the destination entity name (ex. SourceName->DestinationName)
*/
@property (null_resettable, copy) NSString *name;

/* Returns/sets the mapping type.  (If a custom entity mapping type is utilized, the developer must specify a migrationPolicyClassName as well.) 
*/
@property () NSEntityMappingType mappingType;

/* Returns/sets the source entity name for the mapping.  (Mappings are not directly bound to NSEntityDescriptions;  developers can use the sourceEntityForEntityMapping: API on the NSMigrationManager to retrieve the entity description for this name.)
*/
@property (nullable, copy) NSString *sourceEntityName;

/* Returns/sets the version hash for the source entity for the mapping.  VersionHashes are calculated by the Core Data framework (see NSEntityDescrition's versionHash method). The sourceEntityVersionHash must equal the version hash of the source entity represented by the mapping.
*/
@property (nullable, copy) NSData *sourceEntityVersionHash;

/* Returns/sets the destination entity name for the mapping.  (Mappings are not directly bound to NSEntityDescriptions;  developers can use the destinationEntityForEntityMapping: API on the NSMigrationManager to retrieve the entity description for this name.)
*/
@property (nullable, copy) NSString *destinationEntityName;

/* Returns/sets the version hash for the destination entity for the mapping.  VersionHashes are calculated by the Core Data framework (see NSEntityDescrition's versionHash method). The destinationEntityVersionHash must equal the version hash of the destination entity represented by the mapping.
*/
@property (nullable, copy) NSData *destinationEntityVersionHash;

/* Returns/sets the array of attribute mappings for the entity mapping.  The order of mappings in this collection dictates the order in which the mappings will be processed during a migration.
*/
@property (nullable, strong) NSArray<NSPropertyMapping *> *attributeMappings;

/* Returns/sets the array of relationship mappings for the entity mapping.  The order of mappings in this collection dictates the order in which the mappings will be processed during a migration.
*/
@property (nullable, strong) NSArray<NSPropertyMapping *> *relationshipMappings;

/* Returns/sets the source expression for the mapping.  The source expression is used to obtain the collection of managed object instances to process through the mapping.  The expression can be a fetch request expression, or any other expression which evaluates to a collection.  
*/
@property (nullable, strong) NSExpression *sourceExpression;

/* Returns/sets the user info dictionary for the mapping
*/
@property (nullable, nonatomic, strong) NSDictionary *userInfo;

/* Returns/sets the class name of the migration policy for the class.  If not specified, the default migration class name is NSEntityMigrationPolicy, though developers can specify a subclass for specific behavior.
*/
@property (nullable, copy) NSString *entityMigrationPolicyClassName;

@end

NS_ASSUME_NONNULL_END
