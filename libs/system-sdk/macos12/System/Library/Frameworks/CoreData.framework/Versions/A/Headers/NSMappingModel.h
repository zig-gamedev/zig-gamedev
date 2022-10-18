/*
    NSMappingModel.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>

NS_ASSUME_NONNULL_BEGIN

@class NSBundle;
@class NSError;
@class NSManagedObjectModel;
@class NSEntityMapping;

API_AVAILABLE(macosx(10.5),ios(3.0))
@interface NSMappingModel: NSObject {
}

/* Returns the mapping model to translate data from the source to the destination model.  This method is a companion to the mergedModelFromBundles: methods;  in this case, the framework uses the version information from the models to locate the appropriate mapping model in the available bundles.  If the mapping model for the models cannot be found, this method returns nil. 
*/
+ (nullable NSMappingModel *)mappingModelFromBundles:(nullable NSArray<NSBundle *> *)bundles forSourceModel:(nullable NSManagedObjectModel *)sourceModel destinationModel:(nullable NSManagedObjectModel *)destinationModel;

/* Returns a newly created mapping model to translate data from the source to the destination model, if possible.  The differences between the source and destination model are compared and if all changes are simple enough to be able to reasonably infer a data migration mapping model, such a model is created and returned.  If the mapping model can not be created, nil is returned and an error code is returned to indicate why inferring a mapping model automatically failed.
 */
+ (nullable NSMappingModel *)inferredMappingModelForSourceModel:(NSManagedObjectModel *)sourceModel destinationModel:(NSManagedObjectModel *)destinationModel error:(NSError **)error API_AVAILABLE(macosx(10.6),ios(3.0));

/* Loads the mapping model from the specified URL.
*/
- (nullable instancetype)initWithContentsOfURL:(nullable NSURL *)url;


/* Returns/sets the collection of entity mappings for the model.  The order of the mappings dictates the order in which they will be processed during migration.
*/
@property (null_resettable, strong) NSArray<NSEntityMapping *> *entityMappings;


/* Returns a dictionary of the entity mappings for the model, keyed by their respective name.  (This API is provided for quick access to a mapping by name, rather than iterating the ordered entityMapping array.)
*/
@property (readonly, copy) NSDictionary<NSString *, NSEntityMapping *> *entityMappingsByName;

@end

NS_ASSUME_NONNULL_END
