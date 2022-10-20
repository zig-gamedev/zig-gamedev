/*
    NSPersistentStoreRequest.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN

@class NSPersistentStore;

typedef NS_ENUM(NSUInteger, NSPersistentStoreRequestType) {
    NSFetchRequestType = 1,
    NSSaveRequestType,
    NSBatchInsertRequestType API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0)) = 5,
    NSBatchUpdateRequestType API_AVAILABLE(macosx(10.10), ios(8.0)) = 6,
    NSBatchDeleteRequestType API_AVAILABLE(macosx(10.11), ios(9.0)) = 7
};

API_AVAILABLE(macosx(10.7),ios(5.0))
@interface NSPersistentStoreRequest : NSObject <NSCopying> {
}

// Stores this request should be sent to.
@property (nullable, nonatomic, strong) NSArray<NSPersistentStore *> *affectedStores;

// The type of the request.
@property (readonly) NSPersistentStoreRequestType requestType;

@end

NS_ASSUME_NONNULL_END
