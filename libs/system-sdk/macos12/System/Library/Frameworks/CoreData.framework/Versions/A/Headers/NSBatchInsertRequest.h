/*
    NSBatchInsertRequest.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <CoreData/NSPersistentStoreRequest.h>
#import <CoreData/NSPersistentStoreResult.h>

@class NSManagedObject;
@class NSEntityDescription;

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0))
@interface NSBatchInsertRequest : NSPersistentStoreRequest {
}

@property (copy, readonly) NSString *entityName;
@property (strong, nullable, readonly) NSEntityDescription *entity;

@property (copy, nullable) NSArray <NSDictionary<NSString *, id> *> *objectsToInsert;
@property (copy, nullable) BOOL (^dictionaryHandler)(NSMutableDictionary<NSString *, id> *obj) API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));
@property (copy, nullable) BOOL (^managedObjectHandler)(NSManagedObject *obj) API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

// The type of result that should be returned from this request. Defaults to NSBatchInsertRequestResultTypeStatusOnly
@property NSBatchInsertRequestResultType resultType;

+ (instancetype)batchInsertRequestWithEntityName:(NSString *)entityName objects:(NSArray <NSDictionary<NSString *, id> *>*)dictionaries;

+ (instancetype)batchInsertRequestWithEntityName:(NSString *)entityName dictionaryHandler:(BOOL (^)(NSMutableDictionary<NSString *, id> *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));
+ (instancetype)batchInsertRequestWithEntityName:(NSString *)entityName managedObjectHandler:(BOOL (^)(NSManagedObject *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

- (instancetype)init API_DEPRECATED_WITH_REPLACEMENT("initWithEntityName", macosx(10.15,11.0),ios(13.0,14.0),tvos(13.0,14.0),watchos(6.0,7.0));
- (instancetype)initWithEntityName:(NSString *)entityName objects:(NSArray <NSDictionary<NSString *, id> *>*)dictionaries NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithEntity:(NSEntityDescription *)entity objects:(NSArray <NSDictionary<NSString *, id> *>*)dictionaries NS_DESIGNATED_INITIALIZER;

- (instancetype)initWithEntity:(NSEntityDescription *)entity dictionaryHandler:(BOOL (^)(NSMutableDictionary<NSString *, id> *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));
- (instancetype)initWithEntity:(NSEntityDescription *)entity managedObjectHandler:(BOOL (^)(NSManagedObject *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

- (instancetype)initWithEntityName:(NSString *)entityName dictionaryHandler:(BOOL (^)(NSMutableDictionary<NSString *, id> *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));
- (instancetype)initWithEntityName:(NSString *)entityName managedObjectHandler:(BOOL (^)(NSManagedObject *obj))handler API_AVAILABLE(macosx(11.0),ios(14.0),tvos(14.0),watchos(7.0));

@end

NS_ASSUME_NONNULL_END
