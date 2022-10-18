/*
    NSCoreDataCoreSpotlightDelegate.h
    Core Data
    Copyright (c) 2017-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>


@class NSManagedObjectContext;
@class NSPersistentStoreDescription;
@class NSManagedObjectModel;
@class NSManagedObject;
@class CSSearchableIndex;
@class CSSearchableItemAttributeSet;

NS_ASSUME_NONNULL_BEGIN

/*
 * When a change occurs to an entity or entities that are indexed in Spotlight
 * using NSCoreDataCoreSpotlightDelegate, that index is updated asynchronously.
 * NSCoreDataCoreSpotlightDelegate will post NSCoreDataCoreSpotlightDelegateIndexDidUpdateNotification
 * when the index is updated.
 *
 * This notification will have a .userInfo that contains two key-value pairs:

 * NSStoreUUIDKey: A NSString UUID of the store that contains the
 *  NSCoreDataCoreSpotlightDelegate that updated its index.
 * NSPersistentHistoryTokenKey: The NSPersistentHistoryToken of the store that
 *  contains the NSCoreDataCoreSpotlightDelegate that updated its index.
 */
COREDATA_EXTERN NSNotificationName const NSCoreDataCoreSpotlightDelegateIndexDidUpdateNotification API_AVAILABLE(macosx(11.0),ios(14.0),tvos(NA),watchos(NA));

/* NSCoreDataSpotlightDelegate implements the CSSearchableIndexDelegate API, but can't
 publicly declare it due to linkage requirements.
 */
API_AVAILABLE(macosx(10.13),ios(11.0)) API_UNAVAILABLE(tvos,watchos)
@interface NSCoreDataCoreSpotlightDelegate : NSObject {
}

/*
 * Returns if indexing is enabled or not.
 */
@property (getter=isIndexingEnabled, readonly) BOOL indexingEnabled API_AVAILABLE(macosx(12.0),ios(15.0),tvos(NA),watchos(NA));

/* CoreSpotlight domain identifer; default is the store's identifier */
- (NSString *)domainIdentifier;

/* CoreSpotlight index name; default nil */
- (nullable NSString *)indexName;

- (instancetype)init NS_UNAVAILABLE;

/*
 * Designated initializer for NSCoreDataCoreSpotlightDelegate. If this method is used to create the object,
 * -[NSCoreDataCoreSpotlightDelegate startSpotlightIndexing] must be called for the delegate to begin indexing.
 *
 * NSCoreDataSpotlightDelegate requires that
 *  - the store type is NSSQLiteStoreType.
 *  - the store has persistent history tracking enabled.
 */
- (instancetype)initForStoreWithDescription:(NSPersistentStoreDescription *)description
                                coordinator:(NSPersistentStoreCoordinator *)psc NS_DESIGNATED_INITIALIZER API_AVAILABLE(macosx(10.15),ios(13.0),tvos(NA),watchos(NA));

- (instancetype)initForStoreWithDescription:(NSPersistentStoreDescription *)description
                                      model:(NSManagedObjectModel *)model API_DEPRECATED_WITH_REPLACEMENT("initForStoreWithDescription:coordinator:", macosx(10.13,12.0),ios(11.0,15.0));

/*
 * Starts Spotlight indexing.
 */
- (void)startSpotlightIndexing API_AVAILABLE(macosx(10.15),ios(13.0),tvos(NA),watchos(NA));

/*
 * Stops Spotlight indexing.
 */
- (void)stopSpotlightIndexing API_AVAILABLE(macosx(10.15),ios(13.0),tvos(NA),watchos(NA));

/*
 * Deletes all searchable items for the configured Spotlight index. Calling this method may return
 * errors from lower layer dependencies, such as Core Data and Core Spotlight.
 */
- (void)deleteSpotlightIndexWithCompletionHandler:(void (^)(NSError * _Nullable error))completionHandler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(NA),watchos(NA));

/* Create the searchable attributes for the managed object. Override to return nil if you do not want the object included in the index.
 */
- (nullable CSSearchableItemAttributeSet *)attributeSetForObject:(NSManagedObject*)object;

/* CSSearchableIndexDelegate conformance */
- (void)searchableIndex:(CSSearchableIndex *)searchableIndex reindexAllSearchableItemsWithAcknowledgementHandler:(void (^)(void))acknowledgementHandler;

- (void)searchableIndex:(CSSearchableIndex *)searchableIndex reindexSearchableItemsWithIdentifiers:(NSArray <NSString *> *)identifiers
 acknowledgementHandler:(void (^)(void))acknowledgementHandler;

@end

NS_ASSUME_NONNULL_END

