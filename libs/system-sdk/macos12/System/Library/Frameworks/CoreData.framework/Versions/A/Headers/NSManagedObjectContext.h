/*
    NSManagedObjectContext.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSLock.h>
#import <CoreData/CoreDataDefines.h>

NS_ASSUME_NONNULL_BEGIN

@class NSError;
@class NSFetchRequest;
@class NSPersistentStoreRequest;
@class NSPersistentStoreResult;
@class NSManagedObject;
@class NSManagedObjectID;
@class NSPersistentStore;
@class NSPersistentStoreCoordinator;
@class NSPropertyDescription;
@class NSQueryGenerationToken;
@class NSUndoManager;
@class NSNotification;

// Notifications immediately before and immediately after the context saves.  The user info dictionary contains information about the objects that changed and what changed
COREDATA_EXTERN NSString * const NSManagedObjectContextWillSaveNotification API_AVAILABLE(macosx(10.5),ios(3.0));
COREDATA_EXTERN NSString * const NSManagedObjectContextDidSaveNotification API_AVAILABLE(macosx(10.4),ios(3.0));

// Notification when objects in a context changed:  the user info dictionary contains information about the objects that changed and what changed
COREDATA_EXTERN NSString * const NSManagedObjectContextObjectsDidChangeNotification API_AVAILABLE(macosx(10.4),ios(3.0));

// Notification when objects in a context changed:  the user info dictionary contains information about the objectIDs that changed
COREDATA_EXTERN NSString * const NSManagedObjectContextDidSaveObjectIDsNotification API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));
COREDATA_EXTERN NSString * const NSManagedObjectContextDidMergeChangesObjectIDsNotification API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));

// User info keys for NSManagedObjectContextObjectsDidChangeNotification:  the values for these keys are sets of managed objects
COREDATA_EXTERN NSString * const NSInsertedObjectsKey API_AVAILABLE(macosx(10.4),ios(3.0));
COREDATA_EXTERN NSString * const NSUpdatedObjectsKey API_AVAILABLE(macosx(10.4),ios(3.0));
COREDATA_EXTERN NSString * const NSDeletedObjectsKey API_AVAILABLE(macosx(10.4),ios(3.0));

COREDATA_EXTERN NSString * const NSRefreshedObjectsKey API_AVAILABLE(macosx(10.5),ios(3.0));
COREDATA_EXTERN NSString * const NSInvalidatedObjectsKey API_AVAILABLE(macosx(10.5),ios(3.0));

COREDATA_EXTERN NSString * const NSManagedObjectContextQueryGenerationKey API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

// User info keys for NSManagedObjectContextObjectsDidChangeNotification:  the values for these keys are arrays of objectIDs
COREDATA_EXTERN NSString * const NSInvalidatedAllObjectsKey API_AVAILABLE(macosx(10.5),ios(3.0)); // All objects in the context have been invalidated

// User info keys for NSManagedObjectContextDidSaveObjectIDsNotification:  the values for these keys are sets of objectIDs
COREDATA_EXTERN NSString * const NSInsertedObjectIDsKey API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));
COREDATA_EXTERN NSString * const NSUpdatedObjectIDsKey API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));
COREDATA_EXTERN NSString * const NSDeletedObjectIDsKey API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));

COREDATA_EXTERN NSString * const NSRefreshedObjectIDsKey API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));
COREDATA_EXTERN NSString * const NSInvalidatedObjectIDsKey API_AVAILABLE(macosx(10.12.4),ios(10.3),tvos(10.2),watchos(3.2));

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

typedef NS_ENUM(NSUInteger, NSManagedObjectContextConcurrencyType) {
    NSConfinementConcurrencyType API_DEPRECATED( "Use another NSManagedObjectContextConcurrencyType", macosx(10.4,10.11), ios(3.0,9.0)) = 0x00,
    NSPrivateQueueConcurrencyType		= 0x01,
    NSMainQueueConcurrencyType			= 0x02
} API_AVAILABLE(macosx(10.7), ios(5.0));

API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSManagedObjectContext : NSObject <NSCoding, NSLocking> {
}

+ (instancetype)new API_DEPRECATED( "Use -initWithConcurrencyType: instead", macosx(10.4,10.11), ios(3.0,9.0));
- (instancetype)init API_DEPRECATED( "Use -initWithConcurrencyType: instead", macosx(10.4,10.11), ios(3.0,9.0));
- (instancetype)initWithConcurrencyType:(NSManagedObjectContextConcurrencyType)ct NS_DESIGNATED_INITIALIZER  API_AVAILABLE(macosx(10.7),ios(5.0));

/* asynchronously performs the block on the context's queue.  Encapsulates an autorelease pool and a call to processPendingChanges */
- (void)performBlock:(void (^)(void))block API_AVAILABLE(macosx(10.7),ios(5.0));

/* synchronously performs the block on the context's queue.  May safely be called reentrantly.  */
- (void)performBlockAndWait:(void (NS_NOESCAPE ^)(void))block API_AVAILABLE(macosx(10.7),ios(5.0));

/* coordinator which provides model and handles persistency (multiple contexts can share a coordinator) */
@property (nullable, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;

@property (nullable, strong) NSManagedObjectContext *parentContext API_AVAILABLE(macosx(10.7),ios(5.0));

/* custom label for a context.  NSPrivateQueueConcurrencyType contexts will set the label on their queue */
@property (nullable, copy) NSString *name API_AVAILABLE(macosx(10.10),ios(8.0));

@property (nullable, nonatomic, strong) NSUndoManager *undoManager;

@property (nonatomic, readonly) BOOL hasChanges;
@property (nonatomic, readonly, strong) NSMutableDictionary *userInfo API_AVAILABLE(macosx(10.7),ios(5.0));
@property (readonly) NSManagedObjectContextConcurrencyType concurrencyType API_AVAILABLE(macosx(10.7),ios(5.0));

/* returns the object for the specified ID if it is registered in the context already or nil. It never performs I/O. */
- (nullable __kindof NSManagedObject *)objectRegisteredForID:(NSManagedObjectID *)objectID;

/* returns the object for the specified ID if it is already registered, otherwise it creates a fault corresponding to that objectID.  It never returns nil, and never performs I/O.  The object specified by objectID is assumed to exist, and if that assumption is wrong the fault may throw an exception when used. */
- (__kindof NSManagedObject *)objectWithID:(NSManagedObjectID *)objectID;

/* returns the object for the specified ID if it is already registered in the context, or faults the object into the context.  It might perform I/O if the data is uncached.  If the object cannot be fetched, or does not exist, or cannot be faulted, it returns nil.  Unlike -objectWithID: it never returns a fault.  */
- (nullable __kindof NSManagedObject*)existingObjectWithID:(NSManagedObjectID*)objectID error:(NSError**)error API_AVAILABLE(macosx(10.6),ios(3.0));

// method to fetch objects from the persistent stores into the context (fetch request defines the entity and predicate as well as a sort order for the objects); context will match the results from persistent stores with current changes in the context (so inserted objects are returned even if they are not persisted yet); to fetch a single object with an ID if it is not guaranteed to exist and thus -objectWithObjectID: cannot be used, one would create a predicate like [NSComparisonPredicate predicateWithLeftExpression:[NSExpression expressionForKeyPath:@"objectID"] rightExpression:[NSExpression expressionForConstantValue:<object id>] modifier:NSDirectPredicateModifier type:NSEqualToPredicateOperatorType options:0]
    - (nullable NSArray *)executeFetchRequest:(NSFetchRequest *)request error:(NSError **)error NS_REFINED_FOR_SWIFT;

// returns the number of objects a fetch request would have returned if it had been passed to -executeFetchRequest:error:.   If an error occurred during the processing of the request, this method will return NSNotFound. 
- (NSUInteger) countForFetchRequest: (NSFetchRequest *)request error: (NSError **)error API_AVAILABLE(macosx(10.5),ios(3.0)) __attribute__((swift_error(nonnull_error)));

// Method to pass a request to the store without affecting the contents of the managed object context.
// Will return an NSPersistentStoreResult which may contain additional information about the result of the action
// (ie a batch update result may contain the object IDs of the objects that were modified during the update).
// A request may succeed in some stores and fail in others. In this case, the error will contain information
// about each individual store failure.
// Will always reject NSSaveChangesRequests.
- (nullable __kindof NSPersistentStoreResult *)executeRequest:(NSPersistentStoreRequest*)request error:(NSError **)error API_AVAILABLE(macosx(10.10),ios(8.0));

- (void)insertObject:(NSManagedObject *)object;
- (void)deleteObject:(NSManagedObject *)object;

// if flag is YES, merges an object with the state of the object available in the persistent store coordinator; if flag is NO, simply refaults an object without merging (which also causes other related managed objects to be released, so you can use this method to trim the portion of your object graph you want to hold in memory) 
- (void)refreshObject:(NSManagedObject *)object mergeChanges:(BOOL)flag;    

// marks an object for conflict detection, which means that the save fails if the object has been altered in the persistent store by another application.  This allows optimistic locking for unchanged objects.  Conflict detection is always performed on changed or deleted objects.
- (void)detectConflictsForObject:(NSManagedObject *)object;    

// key-value observation
- (void)observeValueForKeyPath:(nullable NSString *)keyPath ofObject:(nullable id)object change:(nullable NSDictionary<NSString *, id> *)change context:(nullable void *)context;

// usually contexts process changes to the object graph coalesced at the end of the event - this method triggers it explicitly
- (void)processPendingChanges;    

// specifies the store a newly inserted object will be saved in.  Unnecessary unless there are multiple writable persistent stores added to the NSPersistentStoreCoordinator which support this object's entity.
- (void)assignObject:(id)object toPersistentStore:(NSPersistentStore *)store;    

@property (nonatomic, readonly, strong) NSSet<__kindof NSManagedObject *> *insertedObjects;
@property (nonatomic, readonly, strong) NSSet<__kindof NSManagedObject *> *updatedObjects;
@property (nonatomic, readonly, strong) NSSet<__kindof NSManagedObject *> *deletedObjects;
@property (nonatomic, readonly, strong) NSSet<__kindof NSManagedObject *> *registeredObjects;

- (void)undo;
- (void)redo;
- (void)reset;
- (void)rollback;
- (BOOL)save:(NSError **)error;

/* calls -refreshObject:mergeChanges: on all currently registered objects with this context.  It handles dirtied objects and clearing the context reference queue */
- (void)refreshAllObjects API_AVAILABLE(macosx(10.11),ios(8.3));

- (void)lock API_DEPRECATED( "Use a queue style context and -performBlockAndWait: instead", macosx(10.4,10.10), ios(3.0,8.0));
- (void)unlock API_DEPRECATED( "Use a queue style context and -performBlockAndWait: instead", macosx(10.4,10.10), ios(3.0,8.0));
- (BOOL)tryLock API_DEPRECATED( "Use a queue style context and -performBlock: instead", macosx(10.4,10.10), ios(3.0,8.0));
    
// whether or not the context propagates deletes to related objects at the end of the event, or only at save time
@property (nonatomic) BOOL propagatesDeletesAtEndOfEvent;   // The default is YES.

// whether or not the context holds a retain on all registered objects, or only upon objects necessary for a pending save (inserted, updated, deleted, or locked)
@property (nonatomic) BOOL retainsRegisteredObjects;   // The default is NO.

/*  set the rule to handle inaccessible faults.  If YES, then the managed object is marked deleted and all its properties, including scalars, nonnullable, and mandatory properties, will be nil or zero’d out.  If NO, the context will throw an exception. Managed objects that are inaccessible because their context is nil due to memory management issues will throw an exception regardless. */
@property BOOL shouldDeleteInaccessibleFaults API_AVAILABLE(macosx(10.11),ios(9.0));

/*  this method will not be called from APIs which return an NSError like -existingObjectWithID:error: nor for managed objects with a nil context (e.g. the fault is inaccessible because the object or its context have been released) this method will not be called if Core Data determines there is an unambiguously correct action to recover.  This might happen if a fault was tripped during delete propagation.  In that case, the fault will simply be deleted.  triggeringProperty may be nil when either all properties are involved, or Core Data is unable to determine the reason for firing the fault. the default implementation logs and then returns the value of -shouldDeleteInaccessibleFaults. */
- (BOOL)shouldHandleInaccessibleFault:(NSManagedObject*)fault forObjectID:(NSManagedObjectID*)oid triggeredByProperty:(nullable NSPropertyDescription*)property API_AVAILABLE(macosx(10.11),ios(9.0));

// Staleness interval is the relative time until cached data should be considered stale. The value is applied on a per object basis. For example, a value of 300.0 informs the context to utilize cached information for no more than 5 minutes after that object was originally fetched. This does not affect objects currently in use. Principly, this controls whether fulfilling a fault uses data previously fetched by the application, or issues a new fetch.  It is a hint which may not be supported by all persistent store types.
@property () NSTimeInterval stalenessInterval; // a negative value is considered infinite.  The default is infinite staleness.
 // acceptable merge policies are listed above as id constants
@property (strong) id mergePolicy;    // default: NSErrorMergePolicy

/* Converts the object IDs of the specified objects to permanent IDs.  This implementation will convert the object ID of each managed object in the specified array to a permanent ID.  Any object in the target array with a permanent ID will be ignored;  additionally, any managed object in the array not already assigned to a store will be assigned, based on the same rules Core Data uses for assignment during a save operation (first writable store supporting the entity, and appropriate for the instance and its related items.)  Although the object will have a permanent ID, it will still respond positively to -isInserted until it is saved.  If an error is encountered obtaining an identifier, the return value will be NO.
*/
- (BOOL)obtainPermanentIDsForObjects:(NSArray<NSManagedObject *> *)objects error:(NSError **)error API_AVAILABLE(macosx(10.5),ios(3.0));

/* Merges the changes specified in notification object received from another context's NSManagedObjectContextDidSaveNotification into the receiver.  This method will refresh any objects which have been updated in the other context, fault in any newly inserted objects, and invoke deleteObject: on those which have been deleted.  The developer is only responsible for the thread safety of the receiver.
*/
- (void)mergeChangesFromContextDidSaveNotification:(NSNotification *)notification API_AVAILABLE(macosx(10.5),ios(3.0));

/* Similar to mergeChangesFromContextDidSaveNotification, this method handles changes from potentially other processes or serialized state.  It more efficiently
 * merges changes into multiple contexts, as well as nested context. The keys in the dictionary should be one those from an
 *  NSManagedObjectContextObjectsDidChangeNotification: NSInsertedObjectsKey, NSUpdatedObjectsKey, etc.
 *  the values should be an NSArray of either NSManagedObjectID or NSURL objects conforming to valid results from -URIRepresentation
 */
+ (void)mergeChangesFromRemoteContextSave:(NSDictionary*)changeNotificationData intoContexts:(NSArray<NSManagedObjectContext*> *)contexts API_AVAILABLE(macosx(10.11),ios(9.0));

/* Return the query generation currently in use by this context. Will be one of the following:
 * - nil, default value => this context is not using generational querying
 * - an opaque token => specifies the generation of data this context is vending
 *
 * All child contexts will return nil.
 */
@property (nullable, nonatomic, strong, readonly) NSQueryGenerationToken *queryGenerationToken API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

/* Set the query generation this context should use. Must be one of the following values:
 * - nil => this context will not use generational querying
 * - the value returned by +[NSQueryGenerationToken currentQueryGenerationToken] => this context will pin itself to the generation of the database when it first loads data
 * - the result of calling -[NSManagedObjectContext queryGenerationToken] on another managed object context => this context will be set to whatever query generation the original context is currently using;
 *
 * Query generations are for fetched data only; they are not used during saving. If a pinned context saves,
 * its query generation will be updated after the save. Executing a NSBatchUpdateRequest or NSBatchDeleteRequest 
 * will not cause the query generation to advance, since these do not otherwise consider or affect the 
 * managed object context's content.
 *
 * All nested contexts will defer to their parent context’s data. It is a programmer error to try to set
 * the queryGenerationToken on a child context.
 *
 * Query generations are tracked across the union of stores. Additions to the persistent store coordinator's
 * persistent stores will be ignored until the context's query generation is updated to include them.
 *
 * May partially fail if one or more stores being tracked by the token are removed from the persistent
 * store coordinator.
 */
- (BOOL)setQueryGenerationFromToken:(nullable NSQueryGenerationToken *)generation error:(NSError **)error API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

/* Whether the context automatically merges changes saved to its coordinator or parent context. Setting this property to YES when the context is pinned to a non-current query generation is not supported.
 */
@property (nonatomic) BOOL automaticallyMergesChangesFromParent API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

/* Set the author for the context, this will be used as an identifier in the Persistent History Transactions (NSPersistentHistoryTransaction)
 */
@property (nullable, copy) NSString *transactionAuthor API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

@end

NS_ASSUME_NONNULL_END
