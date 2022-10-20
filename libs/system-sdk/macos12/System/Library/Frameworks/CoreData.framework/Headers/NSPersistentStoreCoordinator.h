/*
    NSPersistentStoreCoordinator.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSError.h>
#import <Foundation/NSLock.h>
#import <CoreData/CoreDataDefines.h>

NS_ASSUME_NONNULL_BEGIN

@class NSURL;
@class NSValue;
@class NSManagedObjectID;
@class NSManagedObjectModel;
@class NSManagedObjectContext;
@class NSPersistentStore;
@class NSPersistentStoreRequest;
@class NSPersistentStoreDescription;
@class NSPersistentHistoryToken;

// Persistent store types supported by Core Data:
COREDATA_EXTERN NSString * const NSSQLiteStoreType API_AVAILABLE(macosx(10.4),ios(3.0));
COREDATA_EXTERN NSString * const NSXMLStoreType API_AVAILABLE(macosx(10.4)) API_UNAVAILABLE(ios);
COREDATA_EXTERN NSString * const NSBinaryStoreType API_AVAILABLE(macosx(10.4),ios(3.0));
COREDATA_EXTERN NSString * const NSInMemoryStoreType API_AVAILABLE(macosx(10.4),ios(3.0));

// Persistent store metadata dictionary keys:

// key in the metadata dictionary to identify the store type
COREDATA_EXTERN NSString * const NSStoreTypeKey API_AVAILABLE(macosx(10.4),ios(3.0));    

// key in the metadata dictionary to identify the store UUID - the store UUID is useful to identify stores through URI representations, but it is NOT guaranteed to be unique (while the UUID generated for new stores is unique, users can freely copy files and thus the UUID stored inside, so developers that track/reference stores explicitly do need to be aware of duplicate UUIDs and potentially override the UUID when a new store is added to the list of known stores in their application)
COREDATA_EXTERN NSString * const NSStoreUUIDKey API_AVAILABLE(macosx(10.4),ios(3.0));    

/* A notification posted before the list of open persistent stores changes, similar to NSPersistentStoreCoordinatorStoresDidChangeNotification.  If the application is running, Core Data will post this before responding to iCloud account changes or "Delete All" from Documents & Data.
 */
COREDATA_EXTERN NSString * const NSPersistentStoreCoordinatorStoresWillChangeNotification API_AVAILABLE(macosx(10.9),ios(7.0));

// user info dictionary contains information about the stores that were added or removed
COREDATA_EXTERN NSString * const NSPersistentStoreCoordinatorStoresDidChangeNotification API_AVAILABLE(macosx(10.4),ios(3.0));    

// sent during the invocation of NSPersistentStore's willRemoveFromPersistentStoreCoordinator during store deallocation or removal
COREDATA_EXTERN NSString * const NSPersistentStoreCoordinatorWillRemoveStoreNotification API_AVAILABLE(macosx(10.5),ios(3.0));    

// User info keys for NSPersistentStoreCoordinatorStoresDidChangeNotification:

// The object values for NSAddedPersistentStoresKey and NSRemovedPersistentStoresKey will be arrays containing added/removed stores
COREDATA_EXTERN NSString * const NSAddedPersistentStoresKey API_AVAILABLE(macosx(10.4),ios(3.0));
COREDATA_EXTERN NSString * const NSRemovedPersistentStoresKey API_AVAILABLE(macosx(10.4),ios(3.0));

// The object value for NSUUIDChangedPersistentStoresKey will be an array where the object at index 0 will be the old store instance, and the object at index 1 the new
COREDATA_EXTERN NSString * const NSUUIDChangedPersistentStoresKey API_AVAILABLE(macosx(10.4),ios(3.0));

// Persistent store option keys:

// flag indicating whether a store is treated as read-only or not - default is NO
COREDATA_EXTERN NSString * const NSReadOnlyPersistentStoreOption API_AVAILABLE(macosx(10.4),ios(3.0));    

// flag indicating whether an XML file should be validated with the DTD while opening - default is NO
COREDATA_EXTERN NSString * const NSValidateXMLStoreOption API_AVAILABLE(macosx(10.4)) API_UNAVAILABLE(ios);    

// Migration keys:

/* Options key specifying the connection timeout for Core Data stores.  This value (an NSNumber) represents the duration, in seconds, Core Data will wait while attempting to create a connection to a persistent store.  If a connection is unable to be made within that timeframe, the operation is aborted and an error is returned.  
*/
COREDATA_EXTERN NSString * const NSPersistentStoreTimeoutOption API_AVAILABLE(macosx(10.5),ios(3.0));

/* Options key for a dictionary of sqlite pragma settings with pragma values indexed by pragma names as keys.  All pragma values must be specified as strings.  The fullfsync and synchronous pragmas control the tradeoff between write performance (write to disk speed & cache utilization) and durability (data loss/corruption sensitivity to power interruption).  For more information on pragma settings visit <http://sqlite.org/pragma.html>
*/
COREDATA_EXTERN NSString * const NSSQLitePragmasOption API_AVAILABLE(macosx(10.5),ios(3.0));

/* Option key to run an analysis of the store data to optimize indices based on statistical information when the store is added to the coordinator.  This invokes SQLite's ANALYZE command.  Ignored by other stores.
*/
COREDATA_EXTERN NSString * const NSSQLiteAnalyzeOption API_AVAILABLE(macosx(10.5),ios(3.0));

/* Option key to rebuild the store file, forcing a database wide defragmentation when the store is added to the coordinator.  This invokes SQLite's VACUUM command.  Ignored by other stores.
 */
COREDATA_EXTERN NSString * const NSSQLiteManualVacuumOption API_AVAILABLE(macosx(10.6),ios(3.0));

/* Options key to ignore the built-in versioning provided by Core Data.  If the value for this key (an NSNumber) evaluates to YES (using boolValue), Core Data will not compare the version hashes between the managed object model in the coordinator and the metadata for the loaded store.  (It will, however, continue to update the version hash information in the metadata.)  This key is specified by default for all applications linked on or before Mac OS X 10.4.
*/
COREDATA_EXTERN NSString * const NSIgnorePersistentStoreVersioningOption API_AVAILABLE(macosx(10.5),ios(3.0));

/* Options key to automatically attempt to migrate versioned stores.  If the value for this key (an NSNumber) evaluates to YES (using boolValue) Core Data will, if the version hash information for added store is determined to be incompatible with the model for the coordinator, attempt to locate the source and mapping models in the application bundles, and perform a migration.  
*/
COREDATA_EXTERN NSString * const NSMigratePersistentStoresAutomaticallyOption API_AVAILABLE(macosx(10.5),ios(3.0));

/* When combined with NSMigratePersistentStoresAutomaticallyOption, coordinator will attempt to infer a mapping model if none can be found */
COREDATA_EXTERN NSString * const NSInferMappingModelAutomaticallyOption API_AVAILABLE(macosx(10.6),ios(3.0));

/* Key to represent the version hash information (dictionary) for the model used to create a persistent store.  This key is used in the metadata for a persistent store.
*/
COREDATA_EXTERN NSString * const NSStoreModelVersionHashesKey API_AVAILABLE(macosx(10.5),ios(3.0));    

/* Key to represent the version identifier for the model used to create the store. This key is used in the metadata for a persistent store.
*/
COREDATA_EXTERN NSString * const NSStoreModelVersionIdentifiersKey API_AVAILABLE(macosx(10.5),ios(3.0));    

/* Key to represent the earliest version of MacOS X the persistent store should support.  Backward compatibility may preclude some features.  The numeric values are defined in AvailabilityMacros.h
*/
COREDATA_EXTERN NSString * const NSPersistentStoreOSCompatibility API_AVAILABLE(macosx(10.5),ios(3.0));    

/* User info key specifying the maximum connection pool size that should be used on a store that supports concurrent request handling, the value should be an NSNumber. The connection pool size determines the number of requests a store can handle concurrently, and should be a function of how many contexts are attempting to access store data at any time. Generally, application developers should not set this, and should use the default value. The default connection pool size is implementation dependent and may vary by store type and/or platform.
 */
COREDATA_EXTERN NSString * const NSPersistentStoreConnectionPoolMaxSizeKey API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0));

/* Spotlight indexing and external record support keys */

COREDATA_EXTERN NSString * const NSCoreDataCoreSpotlightExporter API_AVAILABLE(macosx(10.13),ios(11.0)) API_UNAVAILABLE(tvos,watchos);

/* Values to be passed with NSExternalRecordsFileFormatOption indicating the format used when writing external records. 
   The files are serialized dictionaries. 
*/
COREDATA_EXTERN NSString * const NSXMLExternalRecordType API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);
COREDATA_EXTERN NSString * const NSBinaryExternalRecordType API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* option indicating the file format used when writing external records.The default is NSXMLExternalRecordType if this options isn't specified. */
COREDATA_EXTERN NSString * const NSExternalRecordsFileFormatOption API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* option indicating the directory URL where external records are stored. External records are files that can be used by 
   Spotlight to index the contents of the store. They can also contain a serialized dictionary representation of the instances.
   The location specified with this option must be somewhere under the path ~/Library/Caches/Metadata/CoreData or ~/Library/CoreData
   This option must be set together with NSExternalRecordExtensionOption
*/
COREDATA_EXTERN NSString * const NSExternalRecordsDirectoryOption API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* option indicating the extension used in the external record files. This option must be set together with NSExternalRecordsDirectoryOption */
COREDATA_EXTERN NSString * const NSExternalRecordExtensionOption API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Dictionary key for the entity name extracted from an external record file URL */
COREDATA_EXTERN NSString * const NSEntityNameInPathKey API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Dictionary key for the store UUID extracted from an external record file URL */
COREDATA_EXTERN NSString * const NSStoreUUIDInPathKey API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Dictionary key for the store URL extracted from an external record file URL */
COREDATA_EXTERN NSString * const NSStorePathKey API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Dictionary key for the managed object model URL extracted from an external record file URL */
COREDATA_EXTERN NSString * const NSModelPathKey API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Dictionary key for the object URI extracted from an external record file URL */
COREDATA_EXTERN NSString * const NSObjectURIKey API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);


/* store option for the destroy... and replace... to indicate that the store file should be destroyed even if the operation might be unsafe (overriding locks
 */
COREDATA_EXTERN NSString * const NSPersistentStoreForceDestroyOption API_AVAILABLE(macosx(10.8),ios(6.0));

/* Key to represent the protection class for the persistent store.  Backward compatibility may preclude some features.  The acceptable values are those defined in Foundation for the NSFileProtectionKey.  The default value of NSPersistentStoreFileProtectionKey is NSFileProtectionCompleteUntilFirstUserAuthentication for all applications built on or after iOS5.  The default value for all older applications is NSFileProtectionNone. */
COREDATA_EXTERN NSString * const NSPersistentStoreFileProtectionKey API_AVAILABLE(ios(5.0)) API_UNAVAILABLE(macosx);

/* Dictionary key for enabling persistent history - default is NO */
COREDATA_EXTERN NSString * const NSPersistentHistoryTrackingKey API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

/*
 Allows developers to provide an additional set of classes (which must implement NSSecureCoding) that should be used while
 decoding a binary store.
 Using this option is preferable to using NSBinaryStoreInsecureDecodingCompatibilityOption.
 */
COREDATA_EXTERN NSString * const NSBinaryStoreSecureDecodingClasses API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

/*
 Indicate that the binary store should be decoded insecurely. This may be necessary if a store has metadata or transformable
 properties containing non-standard classes. If possible, developers should use the NSBinaryStoreSecureDecodingClasses option
 to specify the contained classes, allowing the binary store to to be securely decoded.
 Applications linked before the availability date will default to using this option.
 */
COREDATA_EXTERN NSString * const NSBinaryStoreInsecureDecodingCompatibilityOption API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

/* When NSPersistentStoreRemoteChangeNotificationPostOptionKey is set to YES, a NSPersistentStoreRemoteChangeNotification is posted for every
 write to the store, this includes writes that are done by other processes
 */
COREDATA_EXTERN NSString * const NSPersistentStoreRemoteChangeNotificationPostOptionKey API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));

/* NSPersistentStoreRemoteChangeNotification is posted for all cross process writes to the store
 The payload is the store UUID (NSStoreUUIDKey), store URL (NSPersistentStoreURLKey), and NSPersistentHistoryToken for the transaction (if NSPersistentHistoryTrackingKey was also set)
 */
COREDATA_EXTERN NSString * const NSPersistentStoreRemoteChangeNotification API_AVAILABLE(macosx(10.14),ios(12.0),tvos(12.0),watchos(5.0));

/* Keys found in the UserInfo for a NSPersistentStoreRemoteChangeNotification */
COREDATA_EXTERN NSString * const NSPersistentStoreURLKey API_AVAILABLE(macosx(10.14),ios(12.0),tvos(12.0),watchos(5.0));
COREDATA_EXTERN NSString * const NSPersistentHistoryTokenKey API_AVAILABLE(macosx(10.14),ios(12.0),tvos(12.0),watchos(5.0));

API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSPersistentStoreCoordinator : NSObject <NSLocking> {
}

- (instancetype)initWithManagedObjectModel:(NSManagedObjectModel *)model NS_DESIGNATED_INITIALIZER;

@property (readonly, strong) NSManagedObjectModel *managedObjectModel;

@property (readonly, strong) NSArray<__kindof NSPersistentStore *> *persistentStores;

/* custom name for a coordinator.  Coordinators will set the label on their queue */
@property (nullable, copy) NSString *name  API_AVAILABLE(macosx(10.10),ios(8.0));

- (nullable __kindof NSPersistentStore *)persistentStoreForURL:(NSURL *)URL;
- (NSURL *)URLForPersistentStore:(NSPersistentStore *)store;

/* Sets the URL for the specified store in the coordinator.  For atomic stores, this will alter the location to which the next save operation will persist the file;  for non-atomic stores, invoking this method will release the existing connection and create a new one at the specified URL.  (For non-atomic stores, a store must pre-exist at the destination URL; a new store will not be created.)
 */
- (BOOL)setURL:(NSURL*)url forPersistentStore:(NSPersistentStore *)store API_AVAILABLE(macosx(10.5),ios(3.0));

/* Adds the store at the specified URL (of the specified type) to the coordinator with the model configuration and options.  The configuration can be nil -- then it's the complete model; storeURL is usually the file location of the database
 */
- (nullable __kindof NSPersistentStore *)addPersistentStoreWithType:(NSString *)storeType configuration:(nullable NSString *)configuration URL:(nullable NSURL *)storeURL options:(nullable NSDictionary *)options error:(NSError **)error;

- (void)addPersistentStoreWithDescription:(NSPersistentStoreDescription *)storeDescription completionHandler:(void (^)(NSPersistentStoreDescription *, NSError * _Nullable))block API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0)) NS_SWIFT_DISABLE_ASYNC;

- (BOOL)removePersistentStore:(NSPersistentStore *)store error:(NSError **)error;

/* Sets the metadata stored in the persistent store during the next save operation executed on it; the store type and UUID (NSStoreTypeKey and NSStoreUUIDKey) are always added automatically (but NSStoreUUIDKey is only added if it is not set manually as part of the dictionary argument)
 */
- (void)setMetadata:(nullable NSDictionary<NSString *, id> *)metadata forPersistentStore:(NSPersistentStore *)store;

/* Returns the metadata currently stored or to-be-stored in the persistent store
 */
- (NSDictionary<NSString *, id> *)metadataForPersistentStore:(NSPersistentStore *)store;

/* Given a URI representation of an object ID, returns an object ID if a matching store is available or nil if a matching store cannot be found (the URI representation contains a UUID of the store the ID is coming from, and the coordinator can match it against the stores added to it)
 */
- (nullable NSManagedObjectID *)managedObjectIDForURIRepresentation:(NSURL *)url;

/* Sends a request to all of the stores associated with this coordinator.
 Returns an array if successful,  nil if not.
 The contents of the array will vary depending on the request type: NSFetchRequest results will be an array of managed objects, managed object IDs, or NSDictionaries;
 NSSaveChangesRequests will an empty array. User defined requests will return arrays of arrays, where the nested array is the result returned form a single store.
 */
- (nullable id)executeRequest:(NSPersistentStoreRequest *)request withContext:(NSManagedObjectContext *)context error:(NSError**)error API_AVAILABLE(macosx(10.7),ios(5.0));

/* Returns a dictionary of the registered store types:  the keys are the store type strings and the values are the NSPersistentStore subclasses wrapped in NSValues.
*/
@property(class, readonly, strong) NSDictionary<NSString *, NSValue *> *registeredStoreTypes API_AVAILABLE(macosx(10.5),ios(3.0));

/* Registers the specified NSPersistentStore subclass for the specified store type string.  This method must be invoked before a custom subclass of NSPersistentStore can be loaded into a persistent store coordinator.  Passing nil for the store class argument will unregister the specified store type.
*/
+ (void)registerStoreClass:(nullable Class)storeClass forStoreType:(NSString *)storeType API_AVAILABLE(macosx(10.5),ios(3.0));

/* Allows to access the metadata stored in a persistent store without warming up a CoreData stack; the guaranteed keys in this dictionary are NSStoreTypeKey and NSStoreUUIDKey. If storeType is nil, Core Data will guess which store class should be used to get/set the store file's metadata. 
 */
+ (nullable NSDictionary<NSString *, id> *)metadataForPersistentStoreOfType:(NSString*)storeType URL:(NSURL *)url options:(nullable NSDictionary *)options error:(NSError **)error API_AVAILABLE(macosx(10.9),ios(7.0));
+ (BOOL)setMetadata:(nullable NSDictionary<NSString *, id> *)metadata forPersistentStoreOfType:(NSString*)storeType URL:(NSURL*)url options:(nullable NSDictionary*)options error:(NSError**)error API_AVAILABLE(macosx(10.9),ios(7.0));

/* Takes a URL to an external record file file and returns a dictionary with the derived elements
  The keys in the dictionary are
        NSEntityNameInPathKey - the name of the entity for the managed object instance
        NSStoreUUIDInPathKey - UUID of the store containing the instance
        NSStorePathKey - path to the store file (this is resolved to the store-file path contained in the support directory)
        NSModelPathKey - path to the model file (this is resolved to the model.mom path contained in the support directory)
        NSObjectURIKey - URI of the object instance.
*/
    + (NSDictionary *)elementsDerivedFromExternalRecordURL:(NSURL *)fileURL API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);

/* Creates and populates a store with the external records found at externalRecordsURL. The store is written to destinationURL using
    options and with type storeType. If storeIdentifier is nil, the records for a single store at externalRecordsURL at imported.
    externalRecordsURL must not exist as the store will be created from scratch (no appending to an existing store is allowed).
*/
    - (nullable NSPersistentStore *)importStoreWithIdentifier:(nullable NSString *)storeIdentifier fromExternalRecordsDirectory:(NSURL *)externalRecordsURL toURL:(NSURL *)destinationURL options:(nullable NSDictionary *)options withType:(NSString *)storeType error:(NSError **)error API_DEPRECATED("Spotlight integration is deprecated. Use CoreSpotlight integration instead.", macosx(10.6,10.13)) API_UNAVAILABLE(ios);
    
/* Used for save as - performance may vary depending on the type of old and new store; the old store is usually removed from the coordinator by the migration operation, and therefore is no longer a useful reference after invoking this method 
*/
- (nullable NSPersistentStore *)migratePersistentStore:(NSPersistentStore *)store toURL:(NSURL *)URL options:(nullable NSDictionary *)options withType:(NSString *)storeType error:(NSError **)error;

/* delete or truncate the target persistent store in accordance with the store class's requirements.  It is important to pass similar options as addPersistentStoreWithType: ... SQLite stores will honor file locks, journal files, journaling modes, and other intricacies.  It is not possible to unlink a database file safely out from underneath another thread or process, so this API performs a truncation.  Other stores will default to using NSFileManager.
 */
- (BOOL)destroyPersistentStoreAtURL:(NSURL *)url withType:(NSString *)storeType options:(nullable NSDictionary *)options error:(NSError**)error API_AVAILABLE(macosx(10.11),ios(9.0));

/* copy or overwrite the target persistent store in accordance with the store class's requirements.  It is important to pass similar options as addPersistentStoreWithType: ... SQLite stores will honor file locks, journal files, journaling modes, and other intricacies.  Other stores will default to using NSFileManager.
 */
- (BOOL)replacePersistentStoreAtURL:(NSURL *)destinationURL destinationOptions:(nullable NSDictionary *)destinationOptions withPersistentStoreFromURL:(NSURL *)sourceURL sourceOptions:(nullable NSDictionary *)sourceOptions storeType:(NSString *)storeType error:(NSError**)error API_AVAILABLE(macosx(10.11),ios(9.0));

/* asynchronously performs the block on the coordinator's queue.  Encapsulates an autorelease pool. */
- (void)performBlock:(void (^)(void))block  API_AVAILABLE(macosx(10.10),ios(8.0));

/* synchronously performs the block on the coordinator's queue.  May safely be called reentrantly. Encapsulates an autorelease pool. */
- (void)performBlockAndWait:(void (NS_NOESCAPE ^)(void))block  API_AVAILABLE(macosx(10.10),ios(8.0));

/* Constructs a combined NSPersistentHistoryToken given an array of persistent stores. If stores is nil or an empty array, the NSPersistentHistoryToken will be constructed with all of the persistent stores in the coordinator. */
- (nullable NSPersistentHistoryToken *)currentPersistentHistoryTokenFromStores:(nullable NSArray*)stores API_AVAILABLE(macosx(10.14),ios(12.0),tvos(12.0),watchos(5.0));

 /*
  *   DEPRECATED
  */
    
+ (nullable NSDictionary *)metadataForPersistentStoreWithURL:(NSURL *)url error:(NSError **)error API_DEPRECATED("Use -metadataForPersistentStoreOfType:URL:options:error: and pass in an options dictionary matching addPersistentStoreWithType", macosx(10.4,10.5));
    
- (void)lock API_DEPRECATED( "Use -performBlockAndWait: instead", macosx(10.4,10.10), ios(3.0,8.0));
- (void)unlock API_DEPRECATED( "Use -performBlockAndWait: instead", macosx(10.4,10.10), ios(3.0,8.0));
- (BOOL)tryLock API_DEPRECATED( "Use -performBlock: instead", macosx(10.4,10.10), ios(3.0,8.0));
    
+ (nullable NSDictionary<NSString *, id> *)metadataForPersistentStoreOfType:(nullable NSString *)storeType URL:(NSURL *)url error:(NSError **)error API_DEPRECATED("Use -metadataForPersistentStoreOfType:URL:options:error: and pass in an options dictionary matching addPersistentStoreWithType", macosx(10.5,10.11), ios(3.0,9.0));

+ (BOOL)setMetadata:(nullable NSDictionary<NSString *, id> *)metadata forPersistentStoreOfType:(nullable NSString *)storeType URL:(NSURL*)url error:(NSError **)error API_DEPRECATED("Use  -setMetadata:forPersistentStoreOfType:URL:options:error: and pass in an options dictionary matching addPersistentStoreWithType", macosx(10.5,10.11), ios(3.0,9.0));

/*
 Delete all ubiquitous content for all peers for the persistent store at the given URL and also delete the local store file. storeOptions should contain the options normally passed to addPersistentStoreWithType:URL:options:error. Errors may be returned as a result of file I/O, iCloud network or iCloud account issues.
 */
+ (BOOL)removeUbiquitousContentAndPersistentStoreAtURL:(NSURL *)storeURL options:(nullable NSDictionary *)options error:(NSError**)error API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.7,10.12), ios(5.0,10.0));


@end

/*
 NSPersistentStoreUbiquitousTransitionTypeAccountAdded
 This value indicates that a new iCloud account is available, and the persistent store in use will / did transition to the new account.
 It is only possible to discern this state when the application is running, and therefore this transition type will only be posted if the account changes while the application is running or in the background.
 
 NSPersistentStoreUbiquitousTransitionTypeAccountRemoved
 This value indicates that no iCloud account is available, and the persistent store in use will / did transition to the “local” store.
 It is only possible to discern this state when the application is running, and therefore this transition type will only be posted if the account is removed while the application is running or in the background.
 
 NSPersistentStoreUbiquitousTransitionTypeContentRemoved
 This value indicates that the user has wiped the contents of the iCloud account, usually using Delete All from Documents & Data in Settings. The Core Data integration will transition to an empty store file as a result of this event.
 
 NSPersistentStoreUbiquitousTransitionTypeInitialImportCompleted
 This value indicates that the Core Data integration has finished building a store file that is consistent with the contents of the iCloud account, and is readyto replace the fallback store with that file.
 */
typedef NS_ENUM(NSUInteger, NSPersistentStoreUbiquitousTransitionType) {
    NSPersistentStoreUbiquitousTransitionTypeAccountAdded = 1,
    NSPersistentStoreUbiquitousTransitionTypeAccountRemoved,
    NSPersistentStoreUbiquitousTransitionTypeContentRemoved,
    NSPersistentStoreUbiquitousTransitionTypeInitialImportCompleted
} API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));

/* option indicating that a persistent store has a given name in ubiquity, this option is required for ubiquity to function  */
COREDATA_EXTERN NSString * const NSPersistentStoreUbiquitousContentNameKey API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.7,10.12), ios(5.0,10.0));

/* option indicating the log path to use for ubiquity logs, this option is optional for ubiquity, a default path will be generated for the store if none is provided */
COREDATA_EXTERN NSString * const NSPersistentStoreUbiquitousContentURLKey API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.7,10.12), ios(5.0,10.0));

/* Notification sent after records are imported from the ubiquity store. The notification is sent with the object set to the NSPersistentStoreCoordinator instance which registered the store. */
COREDATA_EXTERN NSString * const NSPersistentStoreDidImportUbiquitousContentChangesNotification API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.7,10.12), ios(5.0,10.0));

/*
 In the NSPersistentStoreCoordinatorStoresWillChangeNotification / NSPersistentStoreCoordinatorStoresDidChangeNotification userInfo dictionaries, this identifies the type of event. This could be one of the NSPersistentStoreUbiquitousTransitionType enum values as an NSNumber
 */
COREDATA_EXTERN NSString * const NSPersistentStoreUbiquitousTransitionTypeKey API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));

/*
 Optionally specified string which will be mixed in to Core Data’s identifier for each iCloud peer. The value must be an alphanumeric string without any special characters, whitespace or punctuation.  The primary use for this option is to allow multiple applications on the same peer (device) to share a Core Data store integrated with iCloud. Each application will require its own store file.
 */
COREDATA_EXTERN NSString * const NSPersistentStoreUbiquitousPeerTokenOption API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));

/* NSNumber boolean indicating that the receiver should remove all associated ubiquity metadata from a persistent store. This is mostly used during migration or copying to disassociate a persistent store file from an iCloud account
 */
COREDATA_EXTERN NSString * const NSPersistentStoreRemoveUbiquitousMetadataOption API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));

/* NSString specifying the iCloud container identifier Core Data should pass to  -URLForUbiquitousContainerIdentifier:
 */
COREDATA_EXTERN NSString * const NSPersistentStoreUbiquitousContainerIdentifierKey API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));

/* NSNumber boolean indicating that the receiver should erase the local store file and rebuild it from the iCloud data in Mobile Documents.
 */
COREDATA_EXTERN NSString * const NSPersistentStoreRebuildFromUbiquitousContentOption API_DEPRECATED("Please see the release notes and Core Data documentation.", macosx(10.9,10.12), ios(7.0,10.0));
    

NS_ASSUME_NONNULL_END

