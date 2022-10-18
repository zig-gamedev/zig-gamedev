/*
    NSPersistentCloudKitContainer_Sharing.h
    Core Data
    Copyright (c) 2021-2021, Apple Inc.
    All rights reserved.
*/


#import <CoreData/NSPersistentCloudKitContainer.h>

NS_ASSUME_NONNULL_BEGIN

@class CKContainer;
@class CKRecordZoneID;
@class CKShare;
@class CKRecordZone;
@class CKShareMetadata;
@class CKShareParticipant;
@class CKUserIdentityLookupInfo;
@interface NSPersistentCloudKitContainer (Sharing)

#pragma mark - Sharing
/*
 Accept a sharing invitation with the given share metadata.
 
 The provided persistent store must be configured to use the Shared database scope. If provided the completion block will be invoked
 once all operations required to accept the share invitations have finished with the resulting CKShareMetadata objects or with an
 error if one occurs.
 */
- (void)acceptShareInvitationsFromMetadata:(NSArray<CKShareMetadata *> *)metadata
                       intoPersistentStore:(NSPersistentStore *)persistentStore
                                completion:(void(^_Nullable)(NSArray<CKShareMetadata *> * _Nullable acceptedShareMetadatas, NSError * _Nullable acceptOperationError))completion NS_SWIFT_NAME(acceptShareInvitations(from:into:completion:)) API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Purges a zone from the CKDatabase associated with the given persistent store (or all active CKDatabases in the persistent cloud container's stores).
 
 If provided the completion block will be invoked once per store with the result of the purge operation.
 */
- (void)purgeObjectsAndRecordsInZoneWithID:(CKRecordZoneID *)zoneID
                         inPersistentStore:(nullable NSPersistentStore *)persistentStore
                                completion:(void(^_Nullable)(CKRecordZoneID * _Nullable purgedZoneID, NSError * _Nullable purgeError))completion API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Saves the given CKShare object to the persistent cloud container's metadata in the provided persistent store.
 
 If provided the completion block will be invoked once the share has been saved to the corresponding CKDatabase with the
 result of the operation.
 */
- (void)persistUpdatedShare:(CKShare *)share
          inPersistentStore:(NSPersistentStore *)persistentStore
                 completion:(void(^ _Nullable)(CKShare * _Nullable persistedShare, NSError * _Nullable persistedShareError))completion API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Fetches a set of CKShareParticipant objects from the CKDatabase associated with the given persistent store.
 
 The completion block will be invoked with the result of the CKFetchShareParticipantsOperation and the retrieved participants (if any).
 */
- (void)fetchParticipantsMatchingLookupInfos:(NSArray<CKUserIdentityLookupInfo *> *)lookupInfos
                         intoPersistentStore:(NSPersistentStore *)persistentStore
                                  completion:(void(^)(NSArray<CKShareParticipant *> * _Nullable fetchedParticipants, NSError * _Nullable fetchError))completion API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Returns the associated CKShare object for a given managed object. If the object's backing CKRecord resides in a CKRecordZone that
 is currently shared a CKShare will be returned.
 
 If the object is not associated with a shared record zone, or the object has not yet been exported to CloudKit (and it's zone is not known),
 it will not have an entry in the resulting dictionary.
 */
- (nullable NSDictionary<NSManagedObjectID *, CKShare *> *)fetchSharesMatchingObjectIDs:(NSArray<NSManagedObjectID *> *)objectIDs
                                                                                  error:(NSError **)error API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Fetches all known CKShare records in the given persistent store (or all persistent stores in the persistent cloud container if none is set).
 
 This method does not to any network work to attempt to discover additional zones or shares in the CKDatabase that is associated with the persistent store.
 */
- (nullable NSArray<CKShare *> *)fetchSharesInPersistentStore:(nullable NSPersistentStore *)persistentStore
                                                        error:(NSError **)error API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));

/*
 Use this method to share a set of managed objects to either a new CKShare or an existing one if specified as the 'share' parameter.
 
 - managedObjects - the set of objects to be shared. A deep traversal will be performed among the objects and any related objects will also be shared.
 - share - The existing share in which to share the objects, or nil if a new one should be created for them
 - completion - A completion block when the share is created and the objects are assigned to it (not necessarily when the objects have been exported to the share)
 
 This method will fail if:
 
 1. Any of the objects in 'managedObjects' or those discovered by traversal are already shared
 2. Any of the objects in 'managedObjects' are from persistent stores which do not support sharing (for example those using CKDatabaseScopePublic)
 3. The current device conditions do not support sharing, for example if there is no CloudKit account on the device, or if NSPersistentCloudKitContainer has failed to initialize.
 
 The completion block is meant to be used directly with UICloudSharingController's preparation block completion handler.
 */
- (void)shareManagedObjects:(NSArray<NSManagedObject *> *)managedObjects
                    toShare:(nullable CKShare *)share
                 completion:(void(^)(NSSet<NSManagedObjectID *> * _Nullable sharedObjectIDs, CKShare * _Nullable share, CKContainer * _Nullable cloudKitContainer, NSError * _Nullable sharingError))completion API_AVAILABLE(macosx(12.0),ios(15.0),tvos(15.0),watchos(8.0));
@end

NS_ASSUME_NONNULL_END

