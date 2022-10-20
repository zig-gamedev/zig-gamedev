/*
    NSManagedObjectModel.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>

NS_ASSUME_NONNULL_BEGIN

@class NSEntityDescription;
@class NSFetchRequest;
@class NSData;
@class NSBundle;

// Models describe object graphs to be managed. Models (and their entities/properties/fetch request templates) are editable until they are used by a persistent store coordinator, allowing developers to create/modify them dynamically. However, once a model is being used, it MUST NOT be changed. When the persistent store coordinator first fetches data using a model, it will become uneditable. Any attempt to mutate a model or any of its subobjects after that point will cause an exception to be thrown. If you need to modify a model that is in use, create a copy, modify the copy, and then discard the objects with the old model.
API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSManagedObjectModel : NSObject <NSCoding, NSCopying, NSFastEnumeration> {
}

+ (nullable NSManagedObjectModel *)mergedModelFromBundles:(nullable NSArray<NSBundle *> *)bundles;  // looks up all models in the specified bundles and merges them; if nil is specified as argument, uses the main bundle

+ (nullable NSManagedObjectModel *)modelByMergingModels:(nullable NSArray<NSManagedObjectModel *> *)models;    // combines multiple models (typically from different frameworks) into one

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url;

@property (readonly, copy) NSDictionary<NSString *, NSEntityDescription *> *entitiesByName;
@property (strong) NSArray<NSEntityDescription *> *entities;

@property (readonly, strong) NSArray<NSString *> *configurations;    // returns all available configuration names
- (nullable NSArray<NSEntityDescription *> *)entitiesForConfiguration:(nullable NSString *)configuration;
- (void)setEntities:(NSArray<NSEntityDescription *> *)entities forConfiguration:(NSString *)configuration;

// fetch request templates allow to pre-define queries and their parameters in the model (with the tool) - typically they contain variables that need to be substituted at runtime.
- (void)setFetchRequestTemplate:(nullable NSFetchRequest *)fetchRequestTemplate forName:(NSString *)name;
- (nullable NSFetchRequest *)fetchRequestTemplateForName:(NSString *)name;
- (nullable NSFetchRequest *)fetchRequestFromTemplateWithName:(NSString *)name substitutionVariables:(NSDictionary<NSString *, id> *)variables;    // returns a copy of the fetch request template with the variable bindings substituted - this is the usual way to bind an "abstractly" defined fetch request template to a concrete fetch

/*
 NSDictionary containing localized string values for entities, properties, and error strings related to this model. The key and value pattern follows:
 key = "Entity/NonLocalizedEntityName"
 value = "LocalizedEntityName"
 
 // for properties of the same non-localized name in differenct entities, which should have different localized names
 key = "Property/NonLocalizedPropertyName/Entity/EntityName" 
 value = "LocalizedPropertyName"
 
 key = "Property/NonLocalizedPropertyName"
 value = "LocalizedPropertyName"
 
 key = "ErrorString/NonLocalizedErrorString"
 value = "LocalizedErrorString"
 */
@property (nullable, strong) NSDictionary<NSString *, NSString *> *localizationDictionary;

/* Returns the managed object model used to create the store for the specified metadata.  This method is a companion to the mergedModelFromBundles: method;  in this case, the framework uses the version information stored in the metadata for a store to locate the models/entities used to create the store in the available bundles, and return the model.  If the model for the store cannot be found, this method will return nil.
*/
+ (nullable NSManagedObjectModel *)mergedModelFromBundles:(nullable NSArray<NSBundle *> *)bundles forStoreMetadata:(NSDictionary<NSString *, id> *)metadata API_AVAILABLE(macosx(10.5),ios(3.0));


/* Returns a merged model from the specified array for the version information in the provided metadata.  (This is the companion method to mergedModelFromBundles:forStoreMetadata:)  If a model cannot be created to match the version information in the specified metadata, this method will return nil.  
*/
+ (nullable NSManagedObjectModel *)modelByMergingModels:(NSArray<NSManagedObjectModel *> *)models forStoreMetadata:(NSDictionary<NSString *, id> *)metadata API_AVAILABLE(macosx(10.5),ios(3.0));


/* Returns the dictionary of fetch request templates, keyed by name, for the model.  If the template contains a predicate with substitution variables, you should instead use fetchRequestFromTemplateWithName:substitutionVariables: to create a new fetch request.
*/
@property (readonly, copy) NSDictionary<NSString *, NSFetchRequest *> *fetchRequestTemplatesByName API_AVAILABLE(macosx(10.5),ios(3.0));

/* Returns the collection of developer-defined version identifiers for the model.  For models created in Xcode, this value is set by the developer in the model inspector. Merged models return the combined  collection of identifiers. This value is meant to be used as a debugging hint to help developers determine the models that were combined to create a merged model. The framework does not give models a default identifier, nor does it depend this value at runtime.
*/
@property (copy) NSSet *versionIdentifiers API_AVAILABLE(macosx(10.5),ios(3.0));


/* Compares the version information in the store metadata with the entity version of a given configuration. Returns NO if there are differences between the version information.  (For information on specific differences, developers should utilize the entityVersionHashesByName method, and perform a comparison.)
*/
- (BOOL)isConfiguration:(nullable NSString *)configuration compatibleWithStoreMetadata:(NSDictionary<NSString *, id> *)metadata API_AVAILABLE(macosx(10.5),ios(3.0));


/* Returns a dictionary of the version hashes for the entities in the model, keyed by entity name.  (The dictionary of version hash information is used by Core Data to determine schema compatibility.)
*/
@property (readonly, copy) NSDictionary<NSString *, NSData *> *entityVersionHashesByName API_AVAILABLE(macosx(10.5),ios(3.0));

@end

NS_ASSUME_NONNULL_END
