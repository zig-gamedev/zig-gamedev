/*
    NSPersistentContainer.h
    Core Data
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSError.h>
#import <CoreData/NSManagedObjectContext.h>
#import <CoreData/NSPersistentStoreCoordinator.h>
#import <CoreData/NSManagedObjectModel.h>
#import <CoreData/NSPersistentStoreDescription.h>

@class NSURL;

NS_ASSUME_NONNULL_BEGIN

// An instance of NSPersistentContainer includes all objects needed to represent a functioning Core Data stack, and provides convenience methods and properties for common patterns.
API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0))
@interface NSPersistentContainer : NSObject {
}

+ (instancetype)persistentContainerWithName:(NSString *)name;
+ (instancetype)persistentContainerWithName:(NSString *)name managedObjectModel:(NSManagedObjectModel *)model;

+ (NSURL *)defaultDirectoryURL;

@property (copy, readonly) NSString *name;
@property (strong, readonly) NSManagedObjectContext *viewContext;
@property (strong, readonly) NSManagedObjectModel *managedObjectModel;
@property (strong, readonly) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (copy) NSArray<NSPersistentStoreDescription *> *persistentStoreDescriptions;

// Creates a container using the model named `name` in the main bundle
- (instancetype)initWithName:(NSString *)name;

- (instancetype)initWithName:(NSString *)name managedObjectModel:(NSManagedObjectModel *)model NS_DESIGNATED_INITIALIZER;

// Load stores from the storeDescriptions property that have not already been successfully added to the container. The completion handler is called once for each store that succeeds or fails.
- (void)loadPersistentStoresWithCompletionHandler:(void (^)(NSPersistentStoreDescription *, NSError * _Nullable))block NS_SWIFT_DISABLE_ASYNC;

- (NSManagedObjectContext *)newBackgroundContext NS_RETURNS_RETAINED;
- (void)performBackgroundTask:(void (^)(NSManagedObjectContext *))block;

@end

NS_ASSUME_NONNULL_END
