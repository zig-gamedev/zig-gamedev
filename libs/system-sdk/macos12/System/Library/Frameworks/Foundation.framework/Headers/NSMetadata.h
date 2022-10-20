/*	NSMetadata.h
	Copyright (c) 2004-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSMetadataAttributes.h>

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSNotification.h>

@class NSString, NSURL, NSArray<ObjectType>, NSDictionary<KeyType, ObjectType>, NSPredicate, NSOperationQueue, NSSortDescriptor;
@class NSMetadataItem, NSMetadataQueryAttributeValueTuple, NSMetadataQueryResultGroup;
@protocol NSMetadataQueryDelegate;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSMetadataQuery : NSObject {
@private
    NSUInteger _flags;
    NSTimeInterval _interval;
    id _private[11];
    void *_reserved;
}

@property (nullable, assign) id<NSMetadataQueryDelegate> delegate;
@property (nullable, copy) NSPredicate *predicate;
@property (copy) NSArray<NSSortDescriptor *> *sortDescriptors;
@property (copy) NSArray<NSString *> *valueListAttributes;
@property (nullable, copy) NSArray<NSString *> *groupingAttributes;
@property NSTimeInterval notificationBatchingInterval;

@property (copy) NSArray *searchScopes;
// scopes is an NSArray of NSURL objects (file URLs only) and/or string
// paths and/or the special string constants below, which specifies the
// locations to which the search is limited; an empty array means no
// limits, which is the default state.

@property (nullable, copy) NSArray *searchItems API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
// items can be a mixture of NSMetadataItem, NSURL objects (file URLs only)
// and/or string paths; the getter returns the same mixture as was set

@property (nullable, retain) NSOperationQueue *operationQueue API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
// optional operation queue for notifications and delegate method calls

- (BOOL)startQuery;
- (void)stopQuery;

@property (readonly, getter=isStarted) BOOL started;
@property (readonly, getter=isGathering) BOOL gathering;
@property (readonly, getter=isStopped) BOOL stopped;

- (void)disableUpdates; // these nest
- (void)enableUpdates;


// Results are NSMetadataItems, or whatever the delegate replaces that with
@property (readonly) NSUInteger resultCount;
- (id)resultAtIndex:(NSUInteger)idx;

- (void)enumerateResultsUsingBlock:(void (NS_NOESCAPE ^)(id result, NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));
- (void)enumerateResultsWithOptions:(NSEnumerationOptions)opts usingBlock:(void (NS_NOESCAPE ^)(id result, NSUInteger idx, BOOL *stop))block API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@property (readonly, copy) NSArray *results;   // this is for K-V Bindings, and causes side-effects on the query

- (NSUInteger)indexOfResult:(id)result;

@property (readonly, copy) NSDictionary<NSString *, NSArray<NSMetadataQueryAttributeValueTuple *> *> *valueLists; // values are arrays of NSMetadataQueryAttributeValueTuple

@property (readonly, copy) NSArray<NSMetadataQueryResultGroup *> *groupedResults; // array of NSMetadataQueryResultGroups, for first grouping attribute

- (nullable id)valueOfAttribute:(NSString *)attrName forResultAtIndex:(NSUInteger)idx;

@end

@protocol NSMetadataQueryDelegate <NSObject>
@optional

- (id)metadataQuery:(NSMetadataQuery *)query replacementObjectForResultObject:(NSMetadataItem *)result;
- (id)metadataQuery:(NSMetadataQuery *)query replacementValueForAttribute:(NSString *)attrName value:(id)attrValue;

@end

// notifications
FOUNDATION_EXPORT NSNotificationName const NSMetadataQueryDidStartGatheringNotification API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSMetadataQueryGatheringProgressNotification API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSMetadataQueryDidFinishGatheringNotification API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSNotificationName const NSMetadataQueryDidUpdateNotification API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0));

// keys for use with notification info dictionary
FOUNDATION_EXPORT NSString * const NSMetadataQueryUpdateAddedItemsKey API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSMetadataQueryUpdateChangedItemsKey API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSString * const NSMetadataQueryUpdateRemovedItemsKey API_AVAILABLE(macos(10.9), ios(8.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSString * const NSMetadataQueryResultContentRelevanceAttribute API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0));

// Scope constants for defined search locations
FOUNDATION_EXPORT NSString * const NSMetadataQueryUserHomeScope API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos); // user home directory
FOUNDATION_EXPORT NSString * const NSMetadataQueryLocalComputerScope API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos); // all local mounted volumes + user home (even if remote)
FOUNDATION_EXPORT NSString * const NSMetadataQueryNetworkScope API_AVAILABLE(macos(10.4)) API_UNAVAILABLE(ios, watchos, tvos); // all user-mounted remote volumes

FOUNDATION_EXPORT NSString * const NSMetadataQueryIndexedLocalComputerScope API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // all indexed local mounted volumes + user home (even if remote)
FOUNDATION_EXPORT NSString * const NSMetadataQueryIndexedNetworkScope API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos); // all indexed user-mounted remote volumes

// -setSearchScopes: will throw an exception if the given array contains a mix of the scope constants below with constants above.
FOUNDATION_EXPORT NSString * const NSMetadataQueryUbiquitousDocumentsScope API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // "Documents" subdirectory in the application's Ubiquity container
FOUNDATION_EXPORT NSString * const NSMetadataQueryUbiquitousDataScope API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0)); // application's Ubiquity container, excluding the "Documents" subdirectory
FOUNDATION_EXPORT NSString * const NSMetadataQueryAccessibleUbiquitousExternalDocumentsScope API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0)); // documents from outside the application's container that are accessible without user interaction. NSMetadataItemURLKey attributes of results are security-scoped NSURLs.


API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSMetadataItem : NSObject {
@private
    id _item;
    void *_reserved;
}

- (nullable instancetype)initWithURL:(NSURL *)url NS_DESIGNATED_INITIALIZER API_AVAILABLE(macos(10.9)) API_UNAVAILABLE(ios, watchos, tvos);

- (nullable id)valueForAttribute:(NSString *)key;
- (nullable NSDictionary<NSString *, id> *)valuesForAttributes:(NSArray<NSString *> *)keys;

@property (readonly, copy) NSArray<NSString *> *attributes;

@end

API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSMetadataQueryAttributeValueTuple : NSObject {
@private
    id _attr;
    id _value;
    NSUInteger _count;
    void *_reserved;
}

@property (readonly, copy) NSString *attribute;
@property (nullable, readonly, retain) id value;
@property (readonly) NSUInteger count;

@end

API_AVAILABLE(macos(10.4), ios(5.0), watchos(2.0), tvos(9.0))
@interface NSMetadataQueryResultGroup : NSObject {
@private
    id _private[9];
    NSUInteger _private2[1];
    void *_reserved;
}

@property (readonly, copy) NSString *attribute;
@property (readonly, retain) id value;

@property (nullable, readonly, copy) NSArray<NSMetadataQueryResultGroup *> *subgroups; // nil if this is a leaf

@property (readonly) NSUInteger resultCount;
- (id)resultAtIndex:(NSUInteger)idx; // uncertain whether this will do anything useful for non-leaf groups

@property (readonly, copy) NSArray *results;   // this is for K-V Bindings, and causes side-effects on the query

@end

NS_ASSUME_NONNULL_END
