/*	
    NSURLCache.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

/*!
    @enum NSURLCacheStoragePolicy
    
    @discussion The NSURLCacheStoragePolicy enum defines constants that
    can be used to specify the type of storage that is allowable for an
    NSCachedURLResponse object that is to be stored in an NSURLCache.
    
    @constant NSURLCacheStorageAllowed Specifies that storage in an
    NSURLCache is allowed without restriction.

    @constant NSURLCacheStorageAllowedInMemoryOnly Specifies that
    storage in an NSURLCache is allowed; however storage should be
    done in memory only, no disk storage should be done.

    @constant NSURLCacheStorageNotAllowed Specifies that storage in an
    NSURLCache is not allowed in any fashion, either in memory or on
    disk.
*/
typedef NS_ENUM(NSUInteger, NSURLCacheStoragePolicy)
{
    NSURLCacheStorageAllowed,
    NSURLCacheStorageAllowedInMemoryOnly,
    NSURLCacheStorageNotAllowed,
};


@class NSCachedURLResponseInternal;
@class NSData;
@class NSDictionary;
@class NSURLRequest;
@class NSURLResponse;
@class NSDate;
@class NSURLSessionDataTask;

/*!
    @class NSCachedURLResponse
    NSCachedURLResponse is a class whose objects functions as a wrapper for
    objects that are stored in the framework's caching system. 
    It is used to maintain characteristics and attributes of a cached 
    object. 
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSCachedURLResponse : NSObject <NSSecureCoding, NSCopying>
{
    @private
    NSCachedURLResponseInternal *_internal;
}

/*! 
    @method initWithResponse:data
    @abstract Initializes an NSCachedURLResponse with the given
    response and data.
    @discussion A default NSURLCacheStoragePolicy is used for
    NSCachedURLResponse objects initialized with this method:
    NSURLCacheStorageAllowed.
    @param response a NSURLResponse object.
    @param data an NSData object representing the URL content
    corresponding to the given response.
    @result an initialized NSCachedURLResponse.
*/
- (instancetype)initWithResponse:(NSURLResponse *)response data:(NSData *)data;

/*! 
    @method initWithResponse:data:userInfo:storagePolicy:
    @abstract Initializes an NSCachedURLResponse with the given
    response, data, user-info dictionary, and storage policy.
    @param response a NSURLResponse object.
    @param data an NSData object representing the URL content
    corresponding to the given response.
    @param userInfo a dictionary user-specified information to be
    stored with the NSCachedURLResponse.
    @param storagePolicy an NSURLCacheStoragePolicy constant.
    @result an initialized NSCachedURLResponse.
*/
- (instancetype)initWithResponse:(NSURLResponse *)response data:(NSData *)data userInfo:(nullable NSDictionary *)userInfo storagePolicy:(NSURLCacheStoragePolicy)storagePolicy;

/*!
    @abstract Returns the response wrapped by this instance. 
    @result The response wrapped by this instance. 
*/
@property (readonly, copy) NSURLResponse *response;

/*!
    @abstract Returns the data of the receiver. 
    @result The data of the receiver. 
*/
@property (readonly, copy) NSData *data;

/*!
    @abstract Returns the userInfo dictionary of the receiver. 
    @result The userInfo dictionary of the receiver. 
*/
@property (nullable, readonly, copy) NSDictionary *userInfo;

/*!
    @abstract Returns the NSURLCacheStoragePolicy constant of the receiver. 
    @result The NSURLCacheStoragePolicy constant of the receiver. 
*/
@property (readonly) NSURLCacheStoragePolicy storagePolicy;

@end


@class NSURLRequest;
@class NSURLCacheInternal;

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLCache : NSObject
{
    @private
    NSURLCacheInternal *_internal;
}

/*! 
    @property sharedURLCache
    @abstract Returns the shared NSURLCache instance or
    sets the NSURLCache instance shared by all clients of
    the current process. This will be the new object returned when
    calls to the <tt>sharedURLCache</tt> method are made.
    @discussion Unless set explicitly through a call to
    <tt>+setSharedURLCache:</tt>, this method returns an NSURLCache
    instance created with the following default values:
    <ul>
    <li>Memory capacity: 4 megabytes (4 * 1024 * 1024 bytes)
    <li>Disk capacity: 20 megabytes (20 * 1024 * 1024 bytes)
    <li>Disk path: <nobr>(user home directory)/Library/Caches/(application bundle id)</nobr> 
    </ul>
    <p>Users who do not have special caching requirements or
    constraints should find the default shared cache instance
    acceptable. If this default shared cache instance is not
    acceptable, <tt>+setSharedURLCache:</tt> can be called to set a
    different NSURLCache instance to be returned from this method. 
    Callers should take care to ensure that the setter is called
    at a time when no other caller has a reference to the previously-set 
    shared URL cache. This is to prevent storing cache data from 
    becoming unexpectedly unretrievable.
    @result the shared NSURLCache instance.
*/
@property (class, strong) NSURLCache *sharedURLCache;

/*! 
    @method initWithMemoryCapacity:diskCapacity:diskPath:
    @abstract Initializes an NSURLCache with the given capacity and
    path.
    @discussion The returned NSURLCache is backed by disk, so
    developers can be more liberal with space when choosing the
    capacity for this kind of cache. A disk cache measured in the tens
    of megabytes should be acceptable in most cases.
    @param memoryCapacity the capacity, measured in bytes, for the cache in memory.
    @param diskCapacity the capacity, measured in bytes, for the cache on disk.
    @param path the path on disk where the cache data is stored.
    @result an initialized NSURLCache, with the given capacity, backed
    by disk.
*/
- (instancetype)initWithMemoryCapacity:(NSUInteger)memoryCapacity diskCapacity:(NSUInteger)diskCapacity diskPath:(nullable NSString *)path API_DEPRECATED_WITH_REPLACEMENT("initWithMemoryCapacity:diskCapacity:directoryURL:", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/*!
    @method initWithMemoryCapacity:diskCapacity:directoryURL:
    @abstract Initializes an NSURLCache with the given capacity and directory.
    @param memoryCapacity the capacity, measured in bytes, for the cache in memory. Or 0 to disable memory cache.
    @param diskCapacity the capacity, measured in bytes, for the cache on disk. Or 0 to disable disk cache.
    @param directoryURL the path to a directory on disk where the cache data is stored. Or nil for default directory.
    @result an initialized NSURLCache, with the given capacity, optionally backed by disk.
 */
- (instancetype)initWithMemoryCapacity:(NSUInteger)memoryCapacity diskCapacity:(NSUInteger)diskCapacity directoryURL:(nullable NSURL *)directoryURL API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*! 
    @method cachedResponseForRequest:
    @abstract Returns the NSCachedURLResponse stored in the cache with
    the given request.
    @discussion The method returns nil if there is no
    NSCachedURLResponse stored using the given request.
    @param request the NSURLRequest to use as a key for the lookup.
    @result The NSCachedURLResponse stored in the cache with the given
    request, or nil if there is no NSCachedURLResponse stored with the
    given request.
*/
- (nullable NSCachedURLResponse *)cachedResponseForRequest:(NSURLRequest *)request;

/*! 
    @method storeCachedResponse:forRequest:
    @abstract Stores the given NSCachedURLResponse in the cache using
    the given request.
    @param cachedResponse The cached response to store.
    @param request the NSURLRequest to use as a key for the storage.
*/
- (void)storeCachedResponse:(NSCachedURLResponse *)cachedResponse forRequest:(NSURLRequest *)request;

/*! 
    @method removeCachedResponseForRequest:
    @abstract Removes the NSCachedURLResponse from the cache that is
    stored using the given request. 
    @discussion No action is taken if there is no NSCachedURLResponse
    stored with the given request.
    @param request the NSURLRequest to use as a key for the lookup.
*/
- (void)removeCachedResponseForRequest:(NSURLRequest *)request;

/*! 
    @method removeAllCachedResponses
    @abstract Clears the given cache, removing all NSCachedURLResponse
    objects that it stores.
*/
- (void)removeAllCachedResponses;

/*!
 @method removeCachedResponsesSince:
 @abstract Clears the given cache of any cached responses since the provided date.
 */
- (void)removeCachedResponsesSinceDate:(NSDate *)date API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*!
    @abstract In-memory capacity of the receiver. 
    @discussion At the time this call is made, the in-memory cache will truncate its contents to the size given, if necessary.
    @result The in-memory capacity, measured in bytes, for the receiver. 
*/
@property NSUInteger memoryCapacity;

/*!
    @abstract The on-disk capacity of the receiver. 
    @discussion The on-disk capacity, measured in bytes, for the receiver. On mutation the on-disk cache will truncate its contents to the size given, if necessary.
*/
@property NSUInteger diskCapacity;

/*!
    @abstract Returns the current amount of space consumed by the
    in-memory cache of the receiver.
    @discussion This size, measured in bytes, indicates the current
    usage of the in-memory cache. 
    @result the current usage of the in-memory cache of the receiver.
*/
@property (readonly) NSUInteger currentMemoryUsage;

/*!
    @abstract Returns the current amount of space consumed by the
    on-disk cache of the receiver.
    @discussion This size, measured in bytes, indicates the current
    usage of the on-disk cache. 
    @result the current usage of the on-disk cache of the receiver.
*/
@property (readonly) NSUInteger currentDiskUsage;

@end

@interface NSURLCache (NSURLSessionTaskAdditions)
- (void)storeCachedResponse:(NSCachedURLResponse *)cachedResponse forDataTask:(NSURLSessionDataTask *)dataTask API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
- (void)getCachedResponseForDataTask:(NSURLSessionDataTask *)dataTask completionHandler:(void (^) (NSCachedURLResponse * _Nullable cachedResponse))completionHandler API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
- (void)removeCachedResponseForDataTask:(NSURLSessionDataTask *)dataTask API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
@end

NS_ASSUME_NONNULL_END
