/*
    NSPersistentCloudKitContainerEventRequest.h
    Core Data
    Copyright (c) 2020-2021, Apple Inc.
    All rights reserved.
*/


#import <CoreData/NSPersistentStoreRequest.h>
#import <CoreData/NSPersistentStoreResult.h>

NS_ASSUME_NONNULL_BEGIN

@class NSPersistentCloudKitContainerEvent;

API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0))
@interface NSPersistentCloudKitContainerEventRequest : NSPersistentStoreRequest
@property NSPersistentCloudKitContainerEventResultType resultType;

+ (nonnull instancetype)fetchEventsAfterDate:(NSDate *)date;
+ (nonnull instancetype)fetchEventsAfterEvent:(nullable NSPersistentCloudKitContainerEvent *)event;

/*
 Supports fetching instances of NSPersistentCloudKitContainerEvent matching a fetch request.
 */
+ (nonnull instancetype)fetchEventsMatchingFetchRequest:(NSFetchRequest *)fetchRequest;

/*
 Returns an instance of NSFetchRequest configured with the correct entity for fetching instances
 of NSPersistentCloudKitContainerEvent.
 */
+ (NSFetchRequest *)fetchRequestForEvents;

@end

NS_ASSUME_NONNULL_END

