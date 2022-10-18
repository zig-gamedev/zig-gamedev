/*
    NSMigrationManager.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSError.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;
@class NSEntityMapping;
@class NSManagedObjectContext;
@class NSManagedObject;
@class NSManagedObjectModel;
@class NSMappingModel;
@class NSMigrationContext;

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSMigrationManager : NSObject {
}

/* Creates a migration manager instance with the corresponding source and destination models.  (All validation of the arguments is performed during migrateStoreFromURL:toURL:)  As with the NSPersistentStoreCoordinator, once models are added to the migration manager they are immutable and cannot be altered.*/

- (instancetype)initWithSourceModel:(NSManagedObjectModel *)sourceModel destinationModel:(NSManagedObjectModel *)destinationModel;

/* Migrates of the store at the specified source URL to the store at the destination URL, performing all of the mappings in the mapping model.  A store must exist at the source URL;  if a store does not exist at the destination URL, one will be created (otherwise the migration will append to the existing store.)  Invoking this method will perform compatibility checks on the source and destination models (and the mapping model.)  If an error occurs during the validation or migration, this method will return NO.*/
- (BOOL)migrateStoreFromURL:(NSURL *)sourceURL type:(NSString *)sStoreType options:(nullable NSDictionary *)sOptions withMappingModel:(nullable NSMappingModel *)mappings toDestinationURL:(NSURL *)dURL destinationType:(NSString *)dStoreType destinationOptions:(nullable NSDictionary *)dOptions error:(NSError **)error;

/* Tries to use a store specific migration manager to perform the store migration, note that a store specific migration manager class is not guaranteed to perform any of the migration manager delegate callbacks or update values for the observable properties.  
 Defaults to YES */
@property () BOOL usesStoreSpecificMigrationManager API_AVAILABLE(macosx(10.7),ios(5.0));

/* Resets the association tables for the migration.  (Note this does NOT reset the source or destination contexts).*/
- (void)reset;

/* Accessors for the mapping model, source model, and destination model*/
@property (readonly, strong) NSMappingModel *mappingModel;
@property (readonly, strong) NSManagedObjectModel *sourceModel;
@property (readonly, strong) NSManagedObjectModel *destinationModel;

/* Accessors for the managed object contexts used for reading the source and destination stores.  These contexts are created lazily, as part of the initialization of two Core Data stacks (one for reading, the other for writing data.)  */
@property (readonly, strong) NSManagedObjectContext *sourceContext;
@property (readonly, strong) NSManagedObjectContext *destinationContext;

/* Returns the NSEntityDescription for the source and destination entities, respectively, of the entity mapping.  (Entity mappings do not store the actual description objects, but rather the name and version information of the entity.)*/
- (nullable NSEntityDescription *)sourceEntityForEntityMapping:(NSEntityMapping *)mEntity;
- (nullable NSEntityDescription *)destinationEntityForEntityMapping:(NSEntityMapping *)mEntity;

/* Associates the source instance with the specified destination instance for the given entity mapping.  Since the migration is performed as a three-step process (first create the data, then relate the data, then validate the data) it is necessary to be able to associate data between the source and destination stores, in order to allow for relationship creation/fixup after the creation pass.  This method is called in the default
implementation of NSEntityMigrationPolicy's createDestinationInstancesForSourceInstance:entityMapping:manager:error: method.*/
- (void)associateSourceInstance:(NSManagedObject *)sourceInstance withDestinationInstance:(NSManagedObject *)destinationInstance forEntityMapping:(NSEntityMapping *)entityMapping;

/* Returns the managed object instances created in the destination store for the given entity mapping for the specified source instances.*/
- (NSArray<__kindof NSManagedObject *> *)destinationInstancesForEntityMappingNamed:(NSString *)mappingName sourceInstances:(nullable NSArray<__kindof NSManagedObject *> *)sourceInstances;
 
/* Returns the managed object instances in the source store used to create the specified destination instances for the given entity mapping.*/
- (NSArray<__kindof NSManagedObject *> *)sourceInstancesForEntityMappingNamed:(NSString *)mappingName destinationInstances:(nullable NSArray<__kindof NSManagedObject *> *)destinationInstances;

/* Observable property that can be used to determine progress of the migration process.  Returns the current entity mapping being processed. Each entity is processed a total of three times (instance creation, relationship creation, validation)*/
@property (readonly, strong) NSEntityMapping *currentEntityMapping;

/* Observable property that can be used to determine progress of the migration process.  Returns the percentage complete of the migration process.  The progress value is a number from 0 to 1 indicating percent complete.*/
@property (readonly) float migrationProgress;

/* Returns/sets the user info for the migration manager*/
@property (nullable, nonatomic, strong) NSDictionary *userInfo;

/* Cancels the migration with the specified error. Calling this method causes migrateStoreFromURL:type:options:withMappingModel:toDestinationURL:destinationType:destinationOptions:error: to abort the migration and return the specified error.  */
- (void)cancelMigrationWithError:(NSError *)error;

@end

NS_ASSUME_NONNULL_END
