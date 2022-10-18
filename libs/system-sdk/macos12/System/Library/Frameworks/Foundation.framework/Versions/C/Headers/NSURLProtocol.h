/*	
    NSURLProtocol.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSURLCache.h>

@class NSCachedURLResponse;
@class NSError;
@class NSMutableURLRequest;
@class NSURLAuthenticationChallenge;
@class NSURLConnection;
@class NSURLProtocol;
@class NSURLProtocolInternal;
@class NSURLRequest;
@class NSURLResponse;
@class NSURLSessionTask;

NS_ASSUME_NONNULL_BEGIN

/*!
    @header NSURLProtocol.h

    This header file describes the constructs used to represent URL
    protocols, and describes the extensible system by which specific
    classes can be made to handle the loading of particular URL types or
    schemes.
    
    <p>NSURLProtocol is an abstract class which provides the
    basic structure for performing protocol-specific loading of URL
    data.
    
    <p>The NSURLProtocolClient describes the integration points a
    protocol implemention can use to hook into the URL loading system.
    NSURLProtocolClient describes the methods a protocol implementation
    needs to drive the URL loading system from a NSURLProtocol subclass.
    
    <p>To support customization of protocol-specific requests,
    protocol implementors are encouraged to provide categories on
    NSURLRequest and NSMutableURLRequest. Protocol implementors who
    need to extend the capabilities of NSURLRequest and
    NSMutableURLRequest in this way can store and retrieve
    protocol-specific request data by using the
    <tt>+propertyForKey:inRequest:</tt> and
    <tt>+setProperty:forKey:inRequest:</tt> class methods on
    NSURLProtocol. See the NSHTTPURLRequest on NSURLRequest and
    NSMutableHTTPURLRequest on NSMutableURLRequest for examples of
    such extensions.
    
    <p>An essential responsibility for a protocol implementor is
    creating a NSURLResponse for each request it processes successfully.
    A protocol implementor may wish to create a custom, mutable 
    NSURLResponse class to aid in this work.
*/

/*!
@protocol NSURLProtocolClient
@discussion NSURLProtocolClient provides the interface to the URL
loading system that is intended for use by NSURLProtocol
implementors.
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@protocol NSURLProtocolClient <NSObject>

/*!
@method URLProtocol:wasRedirectedToRequest:
 @abstract Indicates to an NSURLProtocolClient that a redirect has
 occurred.
 @param protocol the NSURLProtocol object sending the message.
 @param request the NSURLRequest to which the protocol implementation
 has redirected.
 */
- (void)URLProtocol:(NSURLProtocol *)protocol wasRedirectedToRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponse;

    /*!
@method URLProtocol:cachedResponseIsValid:
     @abstract Indicates to an NSURLProtocolClient that the protocol
     implementation has examined a cached response and has
     determined that it is valid.
     @param protocol the NSURLProtocol object sending the message.
     @param cachedResponse the NSCachedURLResponse object that has
     examined and is valid.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol cachedResponseIsValid:(NSCachedURLResponse *)cachedResponse;

    /*!
@method URLProtocol:didReceiveResponse:
     @abstract Indicates to an NSURLProtocolClient that the protocol
     implementation has created an NSURLResponse for the current load.
     @param protocol the NSURLProtocol object sending the message.
     @param response the NSURLResponse object the protocol implementation
     has created.
     @param policy The NSURLCacheStoragePolicy the protocol
     has determined should be used for the given response if the
     response is to be stored in a cache.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol didReceiveResponse:(NSURLResponse *)response cacheStoragePolicy:(NSURLCacheStoragePolicy)policy;

    /*!
@method URLProtocol:didLoadData:
     @abstract Indicates to an NSURLProtocolClient that the protocol
     implementation has loaded URL data.
     @discussion The data object must contain only new data loaded since
     the previous call to this method (if any), not cumulative data for
     the entire load.
     @param protocol the NSURLProtocol object sending the message.
     @param data URL load data being made available.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol didLoadData:(NSData *)data;

    /*!
@method URLProtocolDidFinishLoading:
     @abstract Indicates to an NSURLProtocolClient that the protocol
     implementation has finished loading successfully.
     @param protocol the NSURLProtocol object sending the message.
     */
- (void)URLProtocolDidFinishLoading:(NSURLProtocol *)protocol;

    /*!
                @method URLProtocol:didFailWithError:
     @abstract Indicates to an NSURLProtocolClient that the protocol
     implementation has failed to load successfully.
     @param protocol the NSURLProtocol object sending the message.
     @param error The error that caused the load to fail.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol didFailWithError:(NSError *)error;

    /*!
@method URLProtocol:didReceiveAuthenticationChallenge:
     @abstract Start authentication for the specified request
     @param protocol The protocol object requesting authentication.
     @param challenge The authentication challenge.
     @discussion The protocol client guarantees that it will answer the
     request on the same thread that called this method. It may add a
     default credential to the challenge it issues to the connection delegate,
     if the protocol did not provide one.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

    /*!
@method URLProtocol:didCancelAuthenticationChallenge:
     @abstract Cancel authentication for the specified request
     @param protocol The protocol object cancelling authentication.
     @param challenge The authentication challenge.
     */
- (void)URLProtocol:(NSURLProtocol *)protocol didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

@end


/*!
    @class NSURLProtocol
    
    @abstract NSURLProtocol is an abstract class which provides the
    basic structure for performing protocol-specific loading of URL
    data. Concrete subclasses handle the specifics associated with one
    or more protocols or URL schemes.
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLProtocol : NSObject
{
    @private
    NSURLProtocolInternal *_internal;
}

/*! 
    @method initWithRequest:cachedResponse:client:
    @abstract Initializes an NSURLProtocol given request, 
    cached response, and client.
    @param request The request to load.
    @param cachedResponse A response that has been retrieved from the
    cache for the given request. The protocol implementation should
    apply protocol-specific validity checks if such tests are
    necessary.
    @param client The NSURLProtocolClient object that serves as the
    interface the protocol implementation can use to report results back
    to the URL loading system.
*/
- (instancetype)initWithRequest:(NSURLRequest *)request cachedResponse:(nullable NSCachedURLResponse *)cachedResponse client:(nullable id <NSURLProtocolClient>)client NS_DESIGNATED_INITIALIZER;

/*!
    @abstract Returns the NSURLProtocolClient of the receiver. 
    @result The NSURLProtocolClient of the receiver.  
*/
@property (nullable, readonly, retain) id <NSURLProtocolClient> client;

/*!
    @abstract Returns the NSURLRequest of the receiver. 
    @result The NSURLRequest of the receiver. 
*/
@property (readonly, copy) NSURLRequest *request;

/*!
    @abstract Returns the NSCachedURLResponse of the receiver.  
    @result The NSCachedURLResponse of the receiver. 
*/
@property (nullable, readonly, copy) NSCachedURLResponse *cachedResponse;

/*======================================================================
  Begin responsibilities for protocol implementors

  The methods between this set of begin-end markers must be
  implemented in order to create a working protocol.
  ======================================================================*/
  
/*! 
    @method canInitWithRequest:
    @abstract This method determines whether this protocol can handle
    the given request.
    @discussion A concrete subclass should inspect the given request and
    determine whether or not the implementation can perform a load with
    that request. This is an abstract method. Sublasses must provide an
    implementation.
    @param request A request to inspect.
    @result YES if the protocol can handle the given request, NO if not.
*/
+ (BOOL)canInitWithRequest:(NSURLRequest *)request;

/*! 
    @method canonicalRequestForRequest:
    @abstract This method returns a canonical version of the given
    request.
    @discussion It is up to each concrete protocol implementation to
    define what "canonical" means. However, a protocol should
    guarantee that the same input request always yields the same
    canonical form. Special consideration should be given when
    implementing this method since the canonical form of a request is
    used to look up objects in the URL cache, a process which performs
    equality checks between NSURLRequest objects.
    <p>
    This is an abstract method; sublasses must provide an
    implementation.
    @param request A request to make canonical.
    @result The canonical form of the given request. 
*/
+ (NSURLRequest *)canonicalRequestForRequest:(NSURLRequest *)request;

/*!
    @method requestIsCacheEquivalent:toRequest:
    @abstract Compares two requests for equivalence with regard to caching.
    @discussion Requests are considered euqivalent for cache purposes
    if and only if they would be handled by the same protocol AND that
    protocol declares them equivalent after performing 
    implementation-specific checks.
    @result YES if the two requests are cache-equivalent, NO otherwise.
*/
+ (BOOL)requestIsCacheEquivalent:(NSURLRequest *)a toRequest:(NSURLRequest *)b;

/*! 
    @method startLoading
    @abstract Starts protocol-specific loading of a request. 
    @discussion When this method is called, the protocol implementation
    should start loading a request.
*/
- (void)startLoading;

/*! 
    @method stopLoading
    @abstract Stops protocol-specific loading of a request. 
    @discussion When this method is called, the protocol implementation
    should end the work of loading a request. This could be in response
    to a cancel operation, so protocol implementations must be able to
    handle this call while a load is in progress.
*/
- (void)stopLoading;

/*======================================================================
  End responsibilities for protocol implementors
  ======================================================================*/

/*! 
    @method propertyForKey:inRequest:
    @abstract Returns the property in the given request previously
    stored with the given key.
    @discussion The purpose of this method is to provide an interface
    for protocol implementors to access protocol-specific information
    associated with NSURLRequest objects.
    @param key The string to use for the property lookup.
    @param request The request to use for the property lookup.
    @result The property stored with the given key, or nil if no property
    had previously been stored with the given key in the given request.
*/
+ (nullable id)propertyForKey:(NSString *)key inRequest:(NSURLRequest *)request;

/*! 
    @method setProperty:forKey:inRequest:
    @abstract Stores the given property in the given request using the
    given key.
    @discussion The purpose of this method is to provide an interface
    for protocol implementors to customize protocol-specific
    information associated with NSMutableURLRequest objects.
    @param value The property to store. 
    @param key The string to use for the property storage. 
    @param request The request in which to store the property. 
*/
+ (void)setProperty:(id)value forKey:(NSString *)key inRequest:(NSMutableURLRequest *)request;

/*!
    @method removePropertyForKey:inRequest:
    @abstract Remove any property stored under the given key
    @discussion Like setProperty:forKey:inRequest: above, the purpose of this
        method is to give protocol implementors the ability to store 
        protocol-specific information in an NSURLRequest
    @param key The key whose value should be removed
    @param request The request to be modified
*/
+ (void)removePropertyForKey:(NSString *)key inRequest:(NSMutableURLRequest *)request;


/*! 
    @method registerClass:
    @abstract This method registers a protocol class, making it visible
    to several other NSURLProtocol class methods.
    @discussion When the URL loading system begins to load a request,
    each protocol class that has been registered is consulted in turn to
    see if it can be initialized with a given request. The first
    protocol handler class to provide a YES answer to
    <tt>+canInitWithRequest:</tt> "wins" and that protocol
    implementation is used to perform the URL load. There is no
    guarantee that all registered protocol classes will be consulted.
    Hence, it should be noted that registering a class places it first
    on the list of classes that will be consulted in calls to
    <tt>+canInitWithRequest:</tt>, moving it in front of all classes
    that had been registered previously.
    <p>A similar design governs the process to create the canonical form
    of a request with the <tt>+canonicalRequestForRequest:</tt> class
    method.
    @param protocolClass the class to register.
    @result YES if the protocol was registered successfully, NO if not.
    The only way that failure can occur is if the given class is not a
    subclass of NSURLProtocol.
*/
+ (BOOL)registerClass:(Class)protocolClass;

/*! 
    @method unregisterClass:
    @abstract This method unregisters a protocol. 
    @discussion After unregistration, a protocol class is no longer
    consulted in calls to NSURLProtocol class methods.
    @param protocolClass The class to unregister.
*/
+ (void)unregisterClass:(Class)protocolClass;

@end


@interface NSURLProtocol (NSURLSessionTaskAdditions)
+ (BOOL)canInitWithTask:(NSURLSessionTask *)task API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
- (instancetype)initWithTask:(NSURLSessionTask *)task cachedResponse:(nullable NSCachedURLResponse *)cachedResponse client:(nullable id <NSURLProtocolClient>)client API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
@property (nullable, readonly, copy) NSURLSessionTask *task API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
@end

NS_ASSUME_NONNULL_END
