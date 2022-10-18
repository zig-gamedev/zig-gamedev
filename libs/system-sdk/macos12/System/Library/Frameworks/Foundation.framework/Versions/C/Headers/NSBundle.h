/*	NSBundle.h
	Copyright (c) 1994-2019, Apple Inc.  All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSString.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSProgress.h>
#import <Foundation/NSNotification.h>

@class NSString, NSURL, NSError, NSUUID, NSLock, NSNumber;

NS_ASSUME_NONNULL_BEGIN

/* Because NSBundle caches allocated instances, subclasses should be prepared
   to receive an already initialized object back from [super initWithPath:] */
@interface NSBundle : NSObject

/* Methods for creating or retrieving bundle instances. */
@property (class, readonly, strong) NSBundle *mainBundle;

+ (nullable instancetype)bundleWithPath:(NSString *)path;
- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;

+ (nullable instancetype)bundleWithURL:(NSURL *)url API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable instancetype)initWithURL:(NSURL *)url API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (NSBundle *)bundleForClass:(Class)aClass;
+ (nullable NSBundle *)bundleWithIdentifier:(NSString *)identifier;

@property (class, readonly, copy) NSArray<NSBundle *> *allBundles;
@property (class, readonly, copy) NSArray<NSBundle *> *allFrameworks;

/* Methods for loading and unloading bundles. */
- (BOOL)load;
@property (readonly, getter=isLoaded) BOOL loaded;
- (BOOL)unload;

- (BOOL)preflightAndReturnError:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (BOOL)loadAndReturnError:(NSError **)error API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

/* Methods for locating various components of a bundle. */
@property (readonly, copy) NSURL *bundleURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *resourceURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *executableURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable NSURL *)URLForAuxiliaryExecutable:(NSString *)executableName API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property (nullable, readonly, copy) NSURL *privateFrameworksURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *sharedFrameworksURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *sharedSupportURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURL *builtInPlugInsURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

@property (nullable, readonly, copy) NSURL *appStoreReceiptURL API_AVAILABLE(macos(10.7), ios(7.0), watchos(2.0), tvos(9.0));

@property (readonly, copy) NSString *bundlePath;
@property (nullable, readonly, copy) NSString *resourcePath;
@property (nullable, readonly, copy) NSString *executablePath;
- (nullable NSString *)pathForAuxiliaryExecutable:(NSString *)executableName;

@property (nullable, readonly, copy) NSString *privateFrameworksPath;
@property (nullable, readonly, copy) NSString *sharedFrameworksPath;
@property (nullable, readonly, copy) NSString *sharedSupportPath;
@property (nullable, readonly, copy) NSString *builtInPlugInsPath;

/* Methods for locating bundle resources.  Instance methods locate resources in the bundle indicated by the receiver; class methods take an argument pointing to a bundle on disk.  In the class methods, bundleURL is a URL pointing to the location of a bundle on disk, and may not be nil; bundlePath is the path equivalent of bundleURL, an absolute path pointing to the location of a bundle on disk.  By contrast, subpath is a relative path to a subdirectory inside the relevant global or localized resource directory, and should be nil if the resource file in question is not in a subdirectory.  Where appropriate, localizationName is the name of a .lproj directory in the bundle, minus the .lproj extension; passing nil for localizationName retrieves only global resources, whereas using a method without this argument retrieves both global and localized resources (using the standard localization search algorithm).  */

+ (nullable NSURL *)URLForResource:(nullable NSString *)name withExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath inBundleWithURL:(NSURL *)bundleURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
+ (nullable NSArray<NSURL *> *)URLsForResourcesWithExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath inBundleWithURL:(NSURL *)bundleURL API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (nullable NSURL *)URLForResource:(nullable NSString *)name withExtension:(nullable NSString *)ext API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable NSURL *)URLForResource:(nullable NSString *)name withExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable NSURL *)URLForResource:(nullable NSString *)name withExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath localization:(nullable NSString *)localizationName API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

- (nullable NSArray<NSURL *> *)URLsForResourcesWithExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
- (nullable NSArray<NSURL *> *)URLsForResourcesWithExtension:(nullable NSString *)ext subdirectory:(nullable NSString *)subpath localization:(nullable NSString *)localizationName API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));

+ (nullable NSString *)pathForResource:(nullable NSString *)name ofType:(nullable NSString *)ext inDirectory:(NSString *)bundlePath;
+ (NSArray<NSString *> *)pathsForResourcesOfType:(nullable NSString *)ext inDirectory:(NSString *)bundlePath;

- (nullable NSString *)pathForResource:(nullable NSString *)name ofType:(nullable NSString *)ext;
- (nullable NSString *)pathForResource:(nullable NSString *)name ofType:(nullable NSString *)ext inDirectory:(nullable NSString *)subpath;
- (nullable NSString *)pathForResource:(nullable NSString *)name ofType:(nullable NSString *)ext inDirectory:(nullable NSString *)subpath forLocalization:(nullable NSString *)localizationName;

- (NSArray<NSString *> *)pathsForResourcesOfType:(nullable NSString *)ext inDirectory:(nullable NSString *)subpath;
- (NSArray<NSString *> *)pathsForResourcesOfType:(nullable NSString *)ext inDirectory:(nullable NSString *)subpath forLocalization:(nullable NSString *)localizationName;

/* Methods for retrieving localized strings. */
- (NSString *)localizedStringForKey:(NSString *)key value:(nullable NSString *)value table:(nullable NSString *)tableName NS_FORMAT_ARGUMENT(1);
- (NSAttributedString *)localizedAttributedStringForKey:(NSString *)key value:(nullable NSString *)value table:(nullable NSString *)tableName NS_FORMAT_ARGUMENT(1) NS_REFINED_FOR_SWIFT API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/* Methods for obtaining various information about a bundle. */
@property (nullable, readonly, copy) NSString *bundleIdentifier;
@property (nullable, readonly, copy) NSDictionary<NSString *, id> *infoDictionary;
@property (nullable, readonly, copy) NSDictionary<NSString *, id> *localizedInfoDictionary;
- (nullable id)objectForInfoDictionaryKey:(NSString *)key;
- (nullable Class)classNamed:(NSString *)className;
@property (nullable, readonly) Class principalClass;

/* Methods for dealing with localizations. */
@property (readonly, copy) NSArray<NSString *> *preferredLocalizations;	// a subset of this bundle's localizations, re-ordered into the preferred order for this process's current execution environment; the main bundle's preferred localizations indicate the language (of text) the user is most likely seeing in the UI

@property (readonly, copy) NSArray<NSString *> *localizations;		// list of language names this bundle appears to be localized to
@property (nullable, readonly, copy) NSString *developmentLocalization;

+ (NSArray<NSString *> *)preferredLocalizationsFromArray:(NSArray<NSString *> *)localizationsArray;
+ (NSArray<NSString *> *)preferredLocalizationsFromArray:(NSArray<NSString *> *)localizationsArray forPreferences:(nullable NSArray<NSString *> *)preferencesArray;

/* Method for determining executable architectures. */
enum {
    NSBundleExecutableArchitectureI386      = 0x00000007,
    NSBundleExecutableArchitecturePPC       = 0x00000012,
    NSBundleExecutableArchitectureX86_64    = 0x01000007,
    NSBundleExecutableArchitecturePPC64     = 0x01000012,
    NSBundleExecutableArchitectureARM64 API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0)) = 0x0100000c
};

@property (nullable, readonly, copy) NSArray<NSNumber *> *executableArchitectures API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end

#define NSLocalizedString(key, comment) \
	    [NSBundle.mainBundle localizedStringForKey:(key) value:@"" table:nil]
#define NSLocalizedStringFromTable(key, tbl, comment) \
	    [NSBundle.mainBundle localizedStringForKey:(key) value:@"" table:(tbl)]
#define NSLocalizedStringFromTableInBundle(key, tbl, bundle, comment) \
	    [bundle localizedStringForKey:(key) value:@"" table:(tbl)]
#define NSLocalizedStringWithDefaultValue(key, tbl, bundle, val, comment) \
	    [bundle localizedStringForKey:(key) value:(val) table:(tbl)]

#define NSLocalizedAttributedString(key, comment) \
	    [NSBundle.mainBundle localizedAttributedStringForKey:(key) value:@"" table:nil]
#define NSLocalizedAttributedStringFromTable(key, tbl, comment) \
	    [NSBundle.mainBundle localizedAttributedStringForKey:(key) value:@"" table:(tbl)]
#define NSLocalizedAttributedStringFromTableInBundle(key, tbl, bundle, comment) \
	    [bundle localizedAttributedStringForKey:(key) value:@"" table:(tbl)]
#define NSLocalizedAttributedStringWithDefaultValue(key, tbl, bundle, val, comment) \
	    [bundle localizedAttributedStringForKey:(key) value:(val) table:(tbl)]

@interface NSString (NSBundleExtensionMethods)

/* For strings with length variations, such as from a stringsdict file, this method returns the variant at the given width. If there is no variant at the given width, the one for the next smaller width is returned. And if there are none smaller, the smallest available is returned. For strings without variations, this method returns self. The unit that width is expressed in is decided by the application or framework. But it is intended to be some measurement indicative of the context a string would fit best to avoid truncation and wasted space.
*/ 
- (NSString *)variantFittingPresentationWidth:(NSInteger)width API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

@end

FOUNDATION_EXPORT NSNotificationName const NSBundleDidLoadNotification;
FOUNDATION_EXPORT NSString * const NSLoadedClasses;	// notification key


/*
 The NSBundleResourceRequest class is used to interact with the on demand resource loading system.
 
 The purpose of the system is to allow an application to download certain resources on demand, when they are required. This also means that the system can purge a resource from disk when it is no longer required, which will save disk space. This class describes which resources are required, makes the request and reports progress, allows the app to specify how long during its execution that they are required.
 
 Resources are downloaded into the application container, and are made available via the standard NSBundle resource lookup API.
 
 The request object itself is lightweight. You may create as many as you need, for example to request the same set of tags in different components of your application.
*/

API_AVAILABLE(ios(9.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos)
@interface NSBundleResourceRequest : NSObject <NSProgressReporting>

- (instancetype)init API_UNAVAILABLE(macos, ios, watchos, tvos);

/*
 A tag and bundle are required arguments to the init methods. The tag argument is required and it must exist in the manifest of the specified bundle. The bundle argument describes an existing bundle which was built with on demand resources support. Any resources downloaded can be found using the standard NSBundle resource lookup API once the request is completed. If no bundle is specified then the main bundle is used.
 */
- (instancetype)initWithTags:(NSSet<NSString *> *)tags;
- (instancetype)initWithTags:(NSSet<NSString *> *)tags bundle:(NSBundle *)bundle NS_DESIGNATED_INITIALIZER;

/*
 Provides a hint to the resource loading system as to the loading priority of this request. Values are limited to between 0 and 1, with 1 being the highest priority. The default priority is 0.5.
 
 The exact meaning of the value is up to your application. The system will prefer to act on requests that have a higher priority (from the same application). You may change the priority at any time, even after a request has started. The system will make a best attempt to take the new priority into account.
 */
@property double loadingPriority;

/*
 The tags this request will load.
 */
@property (readonly, copy) NSSet<NSString *> *tags;

/*
 The bundle object that will hold the requested resources. After the -beginAccessingResourcesWithCompletionHandler: callback is invoked, you may use the standard bundle lookup APIs on this bundle object to find your resources.
 */
@property (readonly, strong) NSBundle *bundle;

/*
 Ask the system to fetch the resources that were part of the tag set in this request. Resources will not be purged while in use by the application (as indicated by the application using this begin API paired with a call to -endAccessingResources). If an application has too many fetched resources and the system is unable to reserve enough space for newly requested tags, the request may return an error.
 
 When you are finished with the resources and they may be purged off the disk, invoke -endAccessingResources. If the request object is deallocated, it will also inform the system that the resources are no longer in use.
 
 The completion block will be invoked on a non-main serial queue when the resources are available or an error has occurred. An example of a possible error that may be reported is the lack of a network connection or a problem connecting to the on-demand servers.
 
 Fetch requests are reference counted across the application. So if you have two requests outstanding with the same set of tags, each may be used independently without having to know about any global state. However, each NSBundleResourceRequest object may only be used once.
 
 If you cancel an outstanding request (via the cancel method on the NSProgress object, or cancelling a parent progress object you have created) the completion handler argument to this method will be called back with an NSUserCancelledError in the NSCocoaErrorDomain.
 
 Be sure to always invoke the -endAccessingResources method to balance a call to the begin method, even in the case of an error in the completion handler.
 
 If you want to access the resources again, create a new NSBundleResourceRequest object.
 */
- (void)beginAccessingResourcesWithCompletionHandler:(void (^)(NSError * _Nullable error))completionHandler;

/*
 Inform the system that you wish to begin accessing the resources that are part of this request, but do not attempt to download any content over the network. The completion handler will be invoked with a YES argument if the resources are available.
 
 If the resources were available, then you must invoke the -endAccessingResources method once you are done accessing them. If the resources were not available, then you may invoke the -beginAccessingResourcesWithCompletionHandler: method to initiate a download of the resources.
*/
- (void)conditionallyBeginAccessingResourcesWithCompletionHandler:(void (^)(BOOL resourcesAvailable))completionHandler;

/*
 Informs the system that you are finished with the resources that were part of the tag set in this request. Call this after you no longer need the resources to be available on disk. It is important to invoke this method to make room for newly requested resources. This method may only be invoked if you have received a callback from -beginAccessingResourcesWithCompletionHandler:. To cancel an in-progress request, invoke cancel on the -progress property.
 */
- (void)endAccessingResources;

/*
 Progress for the request. The progress object will be valid at initialization and begin updating after the -beginAccessingResourcesWithCompletionHandler: method is called.
*/
@property (readonly, strong) NSProgress *progress;

@end

@interface NSBundle (NSBundleResourceRequestAdditions)

/* Set a preservation priority for tags that are included in this bundle for the On Demand Resources system. Preservation priorities may be between 0.0 and 1.0, with higher values being the last choice for purging by the system. The exact meaning of this value is up to your application as it only has meaning within the set of tags your application uses.
   
   The default value is 0.0.
 
   This method will throw an exception if the receiver bundle has no on demand resource tag information.
*/
- (void)setPreservationPriority:(double)priority forTags:(NSSet<NSString *> *)tags API_AVAILABLE(ios(9.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);
- (double)preservationPriorityForTag:(NSString *)tag API_AVAILABLE(ios(9.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

@end

/*
 This notification is posted to the default notification center when the resource request system detects a low disk space condition.
 
 If the application is in the background, the system needs more space, and the application does not free up enough in response to the notification then the application may be killed. The application can free up space by calling -endAccessingResources on any outstanding requests. This will inform the system that you are done with those resources and it may purge the content to make room for a new request.
 
 Note that this notification may not be the same as low disk space on the system, as applications can have a smaller quota.
 */
FOUNDATION_EXPORT NSNotificationName const NSBundleResourceRequestLowDiskSpaceNotification API_AVAILABLE(ios(9.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

/* Use this value for the loadingPriority property if the user is doing nothing but waiting on the result of this request. The system will dedicate the maximum amount of resources available to finishing this request as soon as possible.
 */
FOUNDATION_EXPORT double const NSBundleResourceRequestLoadingPriorityUrgent API_AVAILABLE(ios(9.0), watchos(2.0), tvos(9.0)) API_UNAVAILABLE(macos);

NS_ASSUME_NONNULL_END
