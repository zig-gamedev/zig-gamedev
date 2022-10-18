/*
    NSMergePolicy.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSError.h>
#import <CoreData/CoreDataDefines.h>

NS_ASSUME_NONNULL_BEGIN

@class NSMergePolicy;
@class NSManagedObject;

// Default policy for all managed object contexts - save returns with an error that contains the object IDs of the objects that had conflicts(NSInsertedObjectsKey, NSUpdatedObjectsKey).
COREDATA_EXTERN id NSErrorMergePolicy API_AVAILABLE(macosx(10.4),ios(3.0));

// This singleton policy merges conflicts between the persistent store's version of the object and the current in memory version. The merge occurs by individual property. For properties which have been changed in both the external source and in memory, the external changes trump the in memory ones.
COREDATA_EXTERN id NSMergeByPropertyStoreTrumpMergePolicy API_AVAILABLE(macosx(10.4),ios(3.0));    

// This singleton policy merges conflicts between the persistent store's version of the object and the current in memory version. The merge occurs by individual property. For properties which have been changed in both the external source and in memory, the in memory changes trump the external ones.
COREDATA_EXTERN id NSMergeByPropertyObjectTrumpMergePolicy API_AVAILABLE(macosx(10.4),ios(3.0));    

// This singleton policy overwrites all state for the changed objects in conflict The current object's state is pushed upon the persistent store.
COREDATA_EXTERN id NSOverwriteMergePolicy API_AVAILABLE(macosx(10.4),ios(3.0));    

// This singleton policy discards all state for the changed objects in conflict. The persistent store's version of the object is used.
COREDATA_EXTERN id NSRollbackMergePolicy API_AVAILABLE(macosx(10.4),ios(3.0));    


typedef NS_ENUM(NSUInteger, NSMergePolicyType) {
    NSErrorMergePolicyType                      = 0x00,
    NSMergeByPropertyStoreTrumpMergePolicyType	= 0x01,
    NSMergeByPropertyObjectTrumpMergePolicyType = 0x02,
    NSOverwriteMergePolicyType                  = 0x03,
    NSRollbackMergePolicyType                   = 0x04
};

API_AVAILABLE(macosx(10.7),ios(5.0))
@interface NSMergeConflict : NSObject {
}

@property (readonly, retain) NSManagedObject* sourceObject;
@property (nullable, readonly, retain) NSDictionary<NSString *, id> * objectSnapshot;
@property (nullable, readonly, retain) NSDictionary<NSString *, id> * cachedSnapshot;
@property (nullable, readonly, retain) NSDictionary<NSString *, id> * persistedSnapshot;
@property (readonly) NSUInteger newVersionNumber;
@property (readonly) NSUInteger oldVersionNumber;

/*
 * There are two situations in which a conflict may occur:
 * 
 * 1. Between the NSManagedObjectContext and its in-memory cached state at the NSPersistentStoreCoordinator layer.
 *    In this case, the merge conflict has a source object and a cached snapshot but no persisted snapshot (persnap is nil).
 *
 * 2. Between the cached state at the NSPersistentStoreCoordinator and the external store (file, database, etc.).
 *    In this case, the merge conflict has a cached snapshot and a persisted snapshot.  The source object is also provided as a convenience,
 *    but it is not directly involved in the conflict.
 * 
 *  Snapshot dictionaries include values for all attributes and to-one relationships, but not to-many relationships.  
 *   Relationship values are NSManagedObjectID references. to-many relationships must be pulled from the persistent store as needed. 
 *
 *  A newVersion number of 0 means the object was deleted and the corresponding snapshot is nil.
 */
- (instancetype)initWithSource:(NSManagedObject*)srcObject newVersion:(NSUInteger)newvers oldVersion:(NSUInteger)oldvers cachedSnapshot:(nullable NSDictionary<NSString *, id> *)cachesnap persistedSnapshot:(nullable NSDictionary<NSString *, id>  *)persnap NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

@end

/* Used to report merge conflicts which include uniqueness constraint violations. Optimistic locking failures will be reported
 separately from uniquness conflicts and will be resolved first. Each constraint violated will result in a separate NSMergeConflict,
 although if an entity hierarchy has a constraint which is extended in subentities, all constraint violations for that constraint
 will be collapsed into a single report.
 */
API_AVAILABLE(macosx(10.11),ios(9.0))
@interface NSConstraintConflict : NSObject {
}

@property (readonly, copy) NSArray <NSString *> *constraint; // The constraint which has been violated.
@property (readonly, copy) NSDictionary <NSString *, id> *constraintValues; // The values which the conflictingObjects had when this conflict was created. May no longer match the values of any conflicted object if something else resolved the conflict.
@property (nullable, readonly, retain) NSManagedObject *databaseObject; // Object whose DB row is using constraint values. May be null if this is a context-level violation.
@property (nullable, readonly, retain) NSDictionary<NSString *, id> *databaseSnapshot; // DB row already using constraint values. May be null if this is a context-level violation.
@property (readonly, copy) NSArray <NSManagedObject *> *conflictingObjects; // The objects in violation of the constraint. May contain one (in the case of a db level conflict) or more objects.
@property (readonly, copy) NSArray <NSDictionary *> *conflictingSnapshots; // The original property values of objects in violation of the constraint.  Will contain as many objects as there are conflictingObjects. If an object was unchanged, its snapshot will instead be -[NSNull null].

/*
 * There are two situations in which a constraint conflict may occur:
 *
 * 1. Between multiple objects being saved in a single managed object context. In this case, the conflict
 *      will have a nil database object/snapshot, and multiple conflicting objects/snapshots representing
 *      the state of the objects when they were first faulted or inserted into the context.
 *
 * 2. Between a single object being saved in a managed object context and the external store. In this case, the
 *      constraint conflict will have a database object, the current row snapshot for the database object, plus a
 *      a single conflicting object and its snapshot from when it was first faulted or inserted.
 *
 *  Snapshot dictionaries include values for all attributes and to-one relationships, but not to-many relationships.
 *   Relationship values are NSManagedObjectID references. to-many relationships must be pulled from the persistent store as needed.
 */
- (instancetype)initWithConstraint:(NSArray<NSString *> *)contraint databaseObject:(nullable NSManagedObject*)databaseObject databaseSnapshot:(nullable NSDictionary *)databaseSnapshot conflictingObjects:(NSArray<NSManagedObject *> *)conflictingObjects conflictingSnapshots:(NSArray  *)conflictingSnapshots NS_DESIGNATED_INITIALIZER;

@end

API_AVAILABLE(macosx(10.7),ios(5.0))
@interface NSMergePolicy : NSObject {
}

@property (class, readonly, strong) NSMergePolicy *errorMergePolicy API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));
@property (class, readonly, strong) NSMergePolicy *rollbackMergePolicy API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));
@property (class, readonly, strong) NSMergePolicy *overwriteMergePolicy API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));
@property (class, readonly, strong) NSMergePolicy *mergeByPropertyObjectTrumpMergePolicy API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));
@property (class, readonly, strong) NSMergePolicy *mergeByPropertyStoreTrumpMergePolicy API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

@property (readonly) NSMergePolicyType mergeType;

/*
 * In a subclass implementation of initWithMergeType:, you should invoke super with the NSMergePolicyType that is closest to the behavior you want.
 * This will make it easier to use the superclass's implementation of -resolveConflicts:error:, and then customize the results.  You are strongly encouraged to do so.
 * Due to the complexity of merging to-many relationships, this class is designed with the expectation that you call super as the base implemenation.
 */
- (id)initWithMergeType:(NSMergePolicyType)ty NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

/*
 * In a subclass, you are strongly encouraged to override initWithMergeType: and customize the results from calling super instead of performing your own actions from scratch.
 * Correctly merging to-many relationships is very challenging and any mistakes will cause permanent data corruption in the form of dangling foreign keys.
 * Calls -resolveOptimisticLockingVersionConflicts:error: and then -resolveConstraintConflicts:error:
 */
- (BOOL)resolveConflicts:(NSArray *)list error:(NSError **)error;

/* Resolve optimistic locking failures for the list of failures. In a subclass, you are strongly encouraged to override initWithMergeType: and customize 
 *  the results from calling super instead of performing your own actions from scratch. Correctly merging to-many relationships is very challenging and
 *  any mistakes will cause permanent data corruption in the form of dangling foreign keys.
 * Will be called before -resolveConstraintConflicts:error:
 */
- (BOOL)resolveOptimisticLockingVersionConflicts:(NSArray<NSMergeConflict *> *)list error:(NSError **)error API_AVAILABLE(macosx(10.11),ios(9.0));

/* Resolve uniqueness constraint violations for the list of failures.
 *  Will be called after -resolveOptimisticLockingVersionConflicts:error:
 */
- (BOOL)resolveConstraintConflicts:(NSArray<NSConstraintConflict *> *)list error:(NSError **)error API_AVAILABLE(macosx(10.11),ios(9.0));

@end

NS_ASSUME_NONNULL_END
