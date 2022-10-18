/*
    CoreDataErrors.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>

/* NSError codes for Core Data added errors in NSCocoaErrorDomain. Foundation error codes can be found in <Foundation/FoundationErrors.h>. AppKit error codes can be found in <AppKit/AppKitErrors.h>.
*/

#import <CoreData/CoreDataDefines.h>

NS_ASSUME_NONNULL_BEGIN

// User info keys for errors created by Core Data:
COREDATA_EXTERN NSString * const NSDetailedErrorsKey API_AVAILABLE(macosx(10.4),ios(3.0));           // if multiple validation errors occur in one operation, they are collected in an array and added with this key to the "top-level error" of the operation

COREDATA_EXTERN NSString * const NSValidationObjectErrorKey API_AVAILABLE(macosx(10.4),ios(3.0));    // object that failed to validate for a validation error
COREDATA_EXTERN NSString * const NSValidationKeyErrorKey API_AVAILABLE(macosx(10.4),ios(3.0));       // key that failed to validate for a validation error
COREDATA_EXTERN NSString * const NSValidationPredicateErrorKey API_AVAILABLE(macosx(10.4),ios(3.0)); // for predicate-based validation, the predicate for the condition that failed to validate
COREDATA_EXTERN NSString * const NSValidationValueErrorKey API_AVAILABLE(macosx(10.4),ios(3.0));     // if non-nil, the value for the key that failed to validate for a validation error

COREDATA_EXTERN NSString * const NSAffectedStoresErrorKey API_AVAILABLE(macosx(10.4),ios(3.0));      // stores prompting an error
COREDATA_EXTERN NSString * const NSAffectedObjectsErrorKey API_AVAILABLE(macosx(10.4),ios(3.0));     // objects prompting an error

COREDATA_EXTERN NSString * const NSPersistentStoreSaveConflictsErrorKey API_AVAILABLE(macosx(10.7),ios(5.0));     // key in NSError's userInfo specifying the NSArray of NSMergeConflict

COREDATA_EXTERN NSString * const NSSQLiteErrorDomain API_AVAILABLE(macosx(10.5),ios(3.0));           // Predefined domain for SQLite errors, value of "code" will correspond to preexisting values in SQLite.

enum : NSInteger {
    NSManagedObjectValidationError                   = 1550,   // generic validation error
    NSManagedObjectConstraintValidationError         = 1551,   // one or more uniqueness constraints were violated
    NSValidationMultipleErrorsError                  = 1560,   // generic message for error containing multiple validation errors
    NSValidationMissingMandatoryPropertyError        = 1570,   // non-optional property with a nil value
    NSValidationRelationshipLacksMinimumCountError   = 1580,   // to-many relationship with too few destination objects
    NSValidationRelationshipExceedsMaximumCountError = 1590,   // bounded, to-many relationship with too many destination objects
    NSValidationRelationshipDeniedDeleteError        = 1600,   // some relationship with NSDeleteRuleDeny is non-empty
    NSValidationNumberTooLargeError                  = 1610,   // some numerical value is too large
    NSValidationNumberTooSmallError                  = 1620,   // some numerical value is too small
    NSValidationDateTooLateError                     = 1630,   // some date value is too late
    NSValidationDateTooSoonError                     = 1640,   // some date value is too soon
    NSValidationInvalidDateError                     = 1650,   // some date value fails to match date pattern
    NSValidationStringTooLongError                   = 1660,   // some string value is too long
    NSValidationStringTooShortError                  = 1670,   // some string value is too short
    NSValidationStringPatternMatchingError           = 1680,   // some string value fails to match some pattern
    NSValidationInvalidURIError                      = 1690,   // some URI value cannot be represented as a string
    
    NSManagedObjectContextLockingError               = 132000, // can't acquire a lock in a managed object context
    NSPersistentStoreCoordinatorLockingError         = 132010, // can't acquire a lock in a persistent store coordinator
    
    NSManagedObjectReferentialIntegrityError         = 133000, // attempt to fire a fault pointing to an object that does not exist (we can see the store, we can't see the object)
    NSManagedObjectExternalRelationshipError         = 133010, // an object being saved has a relationship containing an object from another store
    NSManagedObjectMergeError                        = 133020, // merge policy failed - unable to complete merging
    NSManagedObjectConstraintMergeError              = 133021, // merge policy failed - unable to complete merging due to multiple conflicting constraint violations
    
    NSPersistentStoreInvalidTypeError                = 134000, // unknown persistent store type/format/version
    NSPersistentStoreTypeMismatchError               = 134010, // returned by persistent store coordinator if a store is accessed that does not match the specified type
    NSPersistentStoreIncompatibleSchemaError         = 134020, // store returned an error for save operation (database level errors ie missing table, no permissions)
    NSPersistentStoreSaveError                       = 134030, // unclassified save error - something we depend on returned an error
    NSPersistentStoreIncompleteSaveError             = 134040, // one or more of the stores returned an error during save (stores/objects that failed will be in userInfo)
	NSPersistentStoreSaveConflictsError				 = 134050, // an unresolved merge conflict was encountered during a save.  userInfo has NSPersistentStoreSaveConflictsErrorKey

    NSCoreDataError                                  = 134060, // general Core Data error
    NSPersistentStoreOperationError                  = 134070, // the persistent store operation failed 
    NSPersistentStoreOpenError                       = 134080, // an error occurred while attempting to open the persistent store
    NSPersistentStoreTimeoutError                    = 134090, // failed to connect to the persistent store within the specified timeout (see NSPersistentStoreTimeoutOption)
	NSPersistentStoreUnsupportedRequestTypeError	 = 134091, // an NSPersistentStore subclass was passed an NSPersistentStoreRequest that it did not understand
    
    NSPersistentStoreIncompatibleVersionHashError    = 134100, // entity version hashes incompatible with data model
    NSMigrationError                                 = 134110, // general migration error
    NSMigrationConstraintViolationError              = 134111, // migration failed due to a violated uniqueness constraint
    NSMigrationCancelledError                        = 134120, // migration failed due to manual cancellation
    NSMigrationMissingSourceModelError               = 134130, // migration failed due to missing source data model
    NSMigrationMissingMappingModelError              = 134140, // migration failed due to missing mapping model
    NSMigrationManagerSourceStoreError               = 134150, // migration failed due to a problem with the source data store
    NSMigrationManagerDestinationStoreError          = 134160, // migration failed due to a problem with the destination data store
    NSEntityMigrationPolicyError                     = 134170, // migration failed during processing of the entity migration policy 

    NSSQLiteError                                    = 134180,  // general SQLite error 

    NSInferredMappingModelError                      = 134190, // inferred mapping model creation error
    NSExternalRecordImportError                      = 134200, // general error encountered while importing external records
    
    NSPersistentHistoryTokenExpiredError             = 134301  // The history token passed to NSPersistentChangeRequest was invalid
};

NS_ASSUME_NONNULL_END
