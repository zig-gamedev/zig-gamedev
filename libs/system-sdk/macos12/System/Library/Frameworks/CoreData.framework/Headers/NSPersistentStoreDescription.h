/*
    NSPersistentStoreDescription.h
    Core Data
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSDate.h>

@class NSURL;

NS_ASSUME_NONNULL_BEGIN

// An instance of NSPersistentStoreDescription encapsulates all information needed to describe a persistent store.
API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0))
@interface NSPersistentStoreDescription : NSObject <NSCopying> {
}

+ (instancetype)persistentStoreDescriptionWithURL:(NSURL *)URL;

@property (copy) NSString *type;
@property (copy, nullable) NSString *configuration;
@property (copy, nullable) NSURL *URL;
@property (nonatomic, copy, readonly) NSDictionary<NSString *, NSObject *> *options;

- (void)setOption:(nullable NSObject *)option forKey:(NSString *)key;

// Store options
@property (getter = isReadOnly) BOOL readOnly;
@property NSTimeInterval timeout;
@property (nonatomic, copy, readonly) NSDictionary<NSString *, NSObject *> *sqlitePragmas;

- (void)setValue:(nullable NSObject *)value forPragmaNamed:(NSString *)name;

// addPersistentStore-time behaviours
@property BOOL shouldAddStoreAsynchronously;
@property BOOL shouldMigrateStoreAutomatically;
@property BOOL shouldInferMappingModelAutomatically;

// Returns a store description instance with default values for the store located at `URL` that can be used immediately with `addPersistentStoreWithDescription:completionHandler:`.
- (instancetype)initWithURL:(NSURL *)url NS_DESIGNATED_INITIALIZER;

@end

@class NSPersistentCloudKitContainerOptions;
@interface NSPersistentStoreDescription (NSPersistentCloudKitContainerAdditions)
/**
 Use this property to apply customized instances of NSPersistentCloudKitContainerOptions to
 a store description you wish to use with CloudKit.
 */
@property(strong, nullable) NSPersistentCloudKitContainerOptions *cloudKitContainerOptions API_AVAILABLE(macosx(10.15),ios(13.0),tvos(13.0),watchos(6.0));
@end

NS_ASSUME_NONNULL_END
