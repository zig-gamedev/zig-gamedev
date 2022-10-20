/*
    NSPersistentHistoryChange.h
    Core Data
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSDictionary.h>
#import <Foundation/NSData.h>
#import <Foundation/NSSet.h>

NS_ASSUME_NONNULL_BEGIN

@class NSPersistentHistoryTransaction;
@class NSManagedObjectID;
@class NSPropertyDescription;
@class NSEntityDescription;
@class NSFetchRequest;
@class NSManagedObjectContext;

typedef NS_ENUM (NSInteger, NSPersistentHistoryChangeType) {
    NSPersistentHistoryChangeTypeInsert,
    NSPersistentHistoryChangeTypeUpdate,
    NSPersistentHistoryChangeTypeDelete,
} API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0));

API_AVAILABLE(macosx(10.13),ios(11.0),tvos(11.0),watchos(4.0))
@interface NSPersistentHistoryChange : NSObject <NSCopying>

+ (nullable NSEntityDescription *)entityDescriptionWithContext:(NSManagedObjectContext *)context API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));

@property (class,nullable,readonly) NSEntityDescription *entityDescription API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));
@property (class,nullable,readonly) NSFetchRequest *fetchRequest API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));

@property (readonly) int64_t changeID;
@property (readonly,copy) NSManagedObjectID *changedObjectID;
@property (readonly) NSPersistentHistoryChangeType changeType;
@property (nullable,readonly,copy) NSDictionary *tombstone;
@property (nullable,readonly,strong) NSPersistentHistoryTransaction *transaction;
@property (nullable,readonly,copy) NSSet<NSPropertyDescription *> *updatedProperties;

@end

NS_ASSUME_NONNULL_END
