/*
    NSManagedObject.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSKeyValueObserving.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;
@class NSError;
@class NSManagedObjectContext;
@class NSManagedObjectID;
@class NSFetchRequest;

typedef NS_OPTIONS(NSUInteger, NSSnapshotEventType) {
	NSSnapshotEventUndoInsertion = 1 << 1,
	NSSnapshotEventUndoDeletion = 1 << 2,
	NSSnapshotEventUndoUpdate = 1 << 3,
	NSSnapshotEventRollback = 1 << 4,
	NSSnapshotEventRefresh = 1 << 5,
	NSSnapshotEventMergePolicy = 1 << 6
};

API_AVAILABLE(macosx(10.4),ios(3.0)) NS_REQUIRES_PROPERTY_DEFINITIONS
@interface NSManagedObject : NSObject {
}

/*  Distinguish between changes that should and should not dirty the object for any key unknown to Core Data.  10.5 & earlier default to NO.  10.6 and later default to YES. */
/*    Similarly, transient attributes may be individually flagged as not dirtying the object by adding +(BOOL)contextShouldIgnoreChangesFor<key> where <key> is the property name. */
@property(class, readonly) BOOL contextShouldIgnoreUnmodeledPropertyChanges API_AVAILABLE(macosx(10.6),ios(3.0));

/* The Entity represented by this subclass. This method is only legal to call on subclasses of NSManagedObject that represent a single entity in the model.
 */
+ (NSEntityDescription*)entity API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

/* A new fetch request initialized with the Entity represented by this subclass. This property's getter is only legal to call on subclasses of NSManagedObject that represent a single entity in the model.
 */
#ifndef __swift__
+ (NSFetchRequest*)fetchRequest API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0)) NS_REFINED_FOR_SWIFT;
#endif

/* The designated initializer. */
- (__kindof NSManagedObject*)initWithEntity:(NSEntityDescription *)entity insertIntoManagedObjectContext:(nullable NSManagedObjectContext *)context NS_DESIGNATED_INITIALIZER;

/* Returns a new object, inserted into managedObjectContext. This method is only legal to call on subclasses of NSManagedObject that represent a single entity in the model. 
 */
-(instancetype)initWithContext:(NSManagedObjectContext*)moc API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

// identity
@property (nullable, nonatomic, readonly, assign) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, readonly, strong) NSEntityDescription *entity;
@property (nonatomic, readonly, strong) NSManagedObjectID *objectID;

// state - methods
@property (nonatomic, getter=isInserted, readonly) BOOL inserted;
@property (nonatomic, getter=isUpdated, readonly) BOOL updated;
@property (nonatomic, getter=isDeleted, readonly) BOOL deleted;

@property (nonatomic, readonly) BOOL hasChanges API_AVAILABLE(macosx(10.7),ios(5.0));

/* returns YES if any persistent properties do not compare isEqual to their last saved state.  Relationship faults will not be unnecessarily fired.  This differs from the existing -hasChanges method which is a simple dirty flag and also includes transient properties */
@property (nonatomic, readonly) BOOL hasPersistentChangedValues API_AVAILABLE(macosx(10.9),ios(7.0));

// this information is useful in many situations when computations are optional - this can be used to avoid growing the object graph unnecessarily (which allows to control performance as it can avoid time consuming fetches from databases)
@property (nonatomic, getter=isFault, readonly) BOOL fault;    

// returns a Boolean indicating if the relationship for the specified key is a fault.  If a value of NO is returned, the resulting relationship is a realized object;  otherwise the relationship is a fault.  If the specified relationship is a fault, calling this method does not result in the fault firing.
- (BOOL)hasFaultForRelationshipNamed:(NSString *)key API_AVAILABLE(macosx(10.5),ios(3.0)); 

/* returns an array of objectIDs for the contents of a relationship.  to-one relationships will return an NSArray with a single NSManagedObjectID.  Optional relationships may return an empty NSArray.  The objectIDs will be returned in an NSArray regardless of the type of the relationship.  */
- (NSArray<NSManagedObjectID *> *)objectIDsForRelationshipNamed:(NSString *)key API_AVAILABLE(macosx(10.11),ios(8.3));

/* Allow developers to determine if an object is in a transitional phase when receiving a KVO notification.  Returns 0 if the object is fully initialized as a managed object and not transitioning to or from another state */
@property (nonatomic, readonly) NSUInteger faultingState API_AVAILABLE(macosx(10.5),ios(3.0));

// lifecycle/change management (includes key-value observing methods)
- (void)willAccessValueForKey:(nullable NSString *)key; // read notification
- (void)didAccessValueForKey:(nullable NSString *)key;           // read notification (together with willAccessValueForKey used to maintain inverse relationships, to fire faults, etc.) - each read access has to be wrapped in this method pair (in the same way as each write access has to be wrapped in the KVO method pair)

// KVO change notification
- (void)willChangeValueForKey:(NSString *)key;
- (void)didChangeValueForKey:(NSString *)key;
- (void)willChangeValueForKey:(NSString *)inKey withSetMutation:(NSKeyValueSetMutationKind)inMutationKind usingObjects:(NSSet *)inObjects;
- (void)didChangeValueForKey:(NSString *)inKey withSetMutation:(NSKeyValueSetMutationKind)inMutationKind usingObjects:(NSSet *)inObjects;

// invoked after a fetch or after unfaulting (commonly used for computing derived values from the persisted properties)
- (void)awakeFromFetch;    

// invoked after an insert (commonly used for initializing special default/initial settings)
- (void)awakeFromInsert;

/* Callback for undo, redo, and other multi-property state resets */
- (void)awakeFromSnapshotEvents:(NSSnapshotEventType)flags API_AVAILABLE(macosx(10.6),ios(3.0));

/* Callback before delete propagation while the object is still alive.  Useful to perform custom propagation before the relationships are torn down or reconfigure KVO observers. */
- (void)prepareForDeletion API_AVAILABLE(macosx(10.6),ios(3.0));

// commonly used to compute persisted values from other transient/scratchpad values, to set timestamps, etc. - this method can have "side effects" on the persisted values
- (void)willSave;    

// commonly used to notify other objects after a save
- (void)didSave;    

// invoked automatically by the Core Data framework before receiver is converted (back) to a fault.  This method is the companion of the -didTurnIntoFault method, and may be used to (re)set state which requires access to property values (for example, observers across keypaths.)  The default implementation does nothing.  
- (void)willTurnIntoFault API_AVAILABLE(macosx(10.5),ios(3.0));

// commonly used to clear out additional transient values or caches
- (void)didTurnIntoFault;    

// value access (includes key-value coding methods)

// KVC - overridden to access generic dictionary storage unless subclasses explicitly provide accessors
- (nullable id)valueForKey:(NSString *)key;    

// KVC - overridden to access generic dictionary storage unless subclasses explicitly provide accessors
- (void)setValue:(nullable id)value forKey:(NSString *)key;    

// primitive methods give access to the generic dictionary storage from subclasses that implement explicit accessors like -setName/-name to add custom document logic
- (nullable id)primitiveValueForKey:(NSString *)key;    
- (void)setPrimitiveValue:(nullable id)value forKey:(NSString *)key;

// returns a dictionary of the last fetched or saved keys and values of this object.  Pass nil to get all persistent modeled properties.
- (NSDictionary<NSString *, id> *)committedValuesForKeys:(nullable NSArray<NSString *> *)keys;

// returns a dictionary with the keys and (new) values that have been changed since last fetching or saving the object (this is implemented efficiently without firing relationship faults)
- (NSDictionary<NSString *, id> *)changedValues;

- (NSDictionary<NSString *, id> *)changedValuesForCurrentEvent API_AVAILABLE(macosx(10.7),ios(5.0));

// validation - in addition to KVC validation managed objects have hooks to validate their lifecycle state; validation is a critical piece of functionality and the following methods are likely the most commonly overridden methods in custom subclasses
- (BOOL)validateValue:(id _Nullable * _Nonnull)value forKey:(NSString *)key error:(NSError **)error;    // KVC
- (BOOL)validateForDelete:(NSError **)error;
- (BOOL)validateForInsert:(NSError **)error;
- (BOOL)validateForUpdate:(NSError **)error;

- (void)setObservationInfo:(nullable void*)inObservationInfo;
- (nullable void*)observationInfo;
    
@end

NS_ASSUME_NONNULL_END
