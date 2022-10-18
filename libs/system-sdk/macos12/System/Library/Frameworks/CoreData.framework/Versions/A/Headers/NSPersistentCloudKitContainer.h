/*
    NSPersistentCloudKitContainer.h
    Core Data
    Copyright (c) 2018-2021, Apple Inc.
    All rights reserved.
*/



#import <CoreData/NSPersistentContainer.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, NSPersistentCloudKitContainerSchemaInitializationOptions) {
    NSPersistentCloudKitContainerSchemaInitializationOptionsNone = 0,
    
    /*
     Validate the model, and generate the records, but don't actually upload them to
     CloudKit. This option is useful for unit testing to ensure your managed object model
     is valid for use with CloudKit.
     */
    NSPersistentCloudKitContainerSchemaInitializationOptionsDryRun = 1 << 1,
    
    /*
     Causes the generated records to be logged to console.
     */
    NSPersistentCloudKitContainerSchemaInitializationOptionsPrintSchema = 1 << 2
};

/*
 NSPersistentCloudKitContainer managed one or more persistent stores that are backed by a CloudKit private database.
 
 By default, NSPersistentContainer contains a single store description which, if not customized otherwise, is assigned
 to the first CloudKit container identifier in an application's entitlements.
 
 Instances of NSPersistentCloudKitContainerOptions can be used to customize this behavior or create additional instances of
 NSPersistentStoreDescription backed by different containers.
 
 As NSPersistentCloudKitContainer is a subclass of NSPersistentContainer, it can manage both CloudKit backed and non-cloud stores.
 */
@class CKRecord;
@class CKRecordID;

API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0))
@interface NSPersistentCloudKitContainer : NSPersistentContainer

/*
 This method creates a set of representative CKRecord instances for all stores in the container
 that use Core Data with CloudKit and uploads them to CloudKit. These records are "fully saturated"
 in that they have a representative value set for every field Core Data might serialize for the given
 managed object model.
 
 After records are successfully uploaded the schema will be visible in the CloudKit dashboard and
 the representative records will be deleted.
 
 This method returns YES if these operations succeed, or NO and the underlying error if they fail.
 
 Note: This method also validates the managed object model in use for a store, so a validation error
 may be returned if the model is not valid for use with CloudKit.
 */
- (BOOL)initializeCloudKitSchemaWithOptions:(NSPersistentCloudKitContainerSchemaInitializationOptions)options
                                      error:(NSError **)error;

/**
 These methods provide access to the underlying CKRecord, or CKRecordID, backing a given NSManagedObjectID.
 */
- (nullable CKRecord *)recordForManagedObjectID:(NSManagedObjectID *)managedObjectID;
- (NSDictionary<NSManagedObjectID *, CKRecord *> *)recordsForManagedObjectIDs:(NSArray<NSManagedObjectID *> *)managedObjectIDs;
- (nullable CKRecordID *)recordIDForManagedObjectID:(NSManagedObjectID *)managedObjectID;
- (NSDictionary<NSManagedObjectID *, CKRecordID *> *)recordIDsForManagedObjectIDs:(NSArray<NSManagedObjectID *> *)managedObjectIDs;

/*
 canUpdateRecordForManagedObjectWithID / canDeleteRecordForManagedObjectWithID indicate whether or not a given object assigned the provided NSManagedObjectID is mutable with respect to the instance of CKRecord that backs it with CloudKit.
 
 In order for canUpdateRecordForManagedObjectWithID / canDeleteRecordForManagedObjectWithID to return YES, -[NSPersistentCloudKitContainer canModifyManagedObjectsInStore] must also be YES.
 
 Returns YES if any of the following conditions are true:
 - The provided objectID is a temporary objectID
 - The provided objectID is assigned to a store not backed by a CKDatabase
 - The provided objectID is assigned to a store backed by the Private CKDatabase
 - The provided objectID is assigned to a store backed by the Public CKDatabase AND one of the following conditions is met:
     - The object has yet to be uploaded to CloudKit (it will be assigned to the current user)
     - The object has already been uploaded to CloudKit and is owned (indicated by CKRecord.creatorUserRecordID) by the current user
 */
- (BOOL)canUpdateRecordForManagedObjectWithID:(NSManagedObjectID *)objectID NS_SWIFT_NAME(canUpdateRecord(forManagedObjectWith:)) API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));
- (BOOL)canDeleteRecordForManagedObjectWithID:(NSManagedObjectID *)objectID NS_SWIFT_NAME(canDeleteRecord(forManagedObjectWith:)) API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

/*
 canModifyManagedObjectsInStore indicates whether or not a given store is mutable when used with CloudKit.
 
 This method return YES if the current user has write permissions to the CKDatabase that backs the store.
 
 For example:
 - When using the Public database, devices without an iCloud account can read data but not write any.
 - When using the Private database, this method always returns YES, even if no iCloud account is present on the device.
 */
- (BOOL)canModifyManagedObjectsInStore:(NSPersistentStore *)store API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

@end

NS_ASSUME_NONNULL_END

