/*	NSNetServices.h
        Copyright (c) 2002-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <Foundation/NSError.h>
#import <Foundation/NSRunLoop.h>

@class NSArray<ObjectType>, NSData, NSDictionary<KeyType, ObjectType>, NSInputStream, NSNumber, NSOutputStream, NSRunLoop, NSString;
@protocol NSNetServiceDelegate, NSNetServiceBrowserDelegate;

NS_ASSUME_NONNULL_BEGIN

#pragma mark Error constants

FOUNDATION_EXPORT NSString * const NSNetServicesErrorCode API_AVAILABLE(macos(10.2), ios(2.0), tvos(9.0)) API_UNAVAILABLE(watchos);
FOUNDATION_EXPORT NSErrorDomain const NSNetServicesErrorDomain API_AVAILABLE(macos(10.2), ios(2.0), tvos(9.0)) API_UNAVAILABLE(watchos);

typedef NS_ENUM(NSInteger, NSNetServicesError) {
    
/* An unknown error occured during resolution or publication.
*/
    NSNetServicesUnknownError = -72000L,
    
/* An NSNetService with the same domain, type and name was already present when the publication request was made.
*/
    NSNetServicesCollisionError = -72001L,
    
/* The NSNetService was not found when a resolution request was made.
*/
    NSNetServicesNotFoundError	= -72002L,
    
/* A publication or resolution request was sent to an NSNetService instance which was already published or a search request was made of an NSNetServiceBrowser instance which was already searching.
*/
    NSNetServicesActivityInProgress = -72003L,
    
/* An required argument was not provided when initializing the NSNetService instance.
*/
    NSNetServicesBadArgumentError = -72004L,
    
/* The operation being performed by the NSNetService or NSNetServiceBrowser instance was cancelled.
*/
    NSNetServicesCancelledError = -72005L,
    
/* An invalid argument was provided when initializing the NSNetService instance or starting a search with an NSNetServiceBrowser instance.
*/
    NSNetServicesInvalidError = -72006L,
        
/* Resolution of an NSNetService instance failed because the timeout was reached.
*/
    NSNetServicesTimeoutError = -72007L,
    
/* Missing required configuration for local network access.
 *
 * NSBonjourServices and NSLocalNetworkUsageDescription are required in Info.plist
 */
    NSNetServicesMissingRequiredConfigurationError API_AVAILABLE(macos(11.0), ios(14.0), tvos(14.0)) API_UNAVAILABLE(watchos) = -72008L,
};


typedef NS_OPTIONS(NSUInteger, NSNetServiceOptions) {
    /* When passed to -publishWithOptions:, this suppresses the auto-renaming of an
     * NSNetService in the event of a name collision. The collision is reported to the
     * instance's delegate on the -netService:didNotPublish: method.
     */
    NSNetServiceNoAutoRename = 1UL << 0,


    /* When passed to -publishWithOptions:, in addition to publishing the service, a
     * TCP listener is started for both IPv4 and IPv6 on the port specified by the
     * NSNetService. If the listening port can't be opened, an error is reported using
     * -netService:didNotPublish:. Specify a port number of zero to use a random port.
     * When -netServiceDidPublish: is called, -port will return the actual listening
     * port number. Since the listener only supports TCP, the publish will fail with
     * NSNetServicesBadArgumentError if the NSNetService type does not end with "_tcp".
     * New incoming connections will be delivered in the form of NSStreams via the
     * -netService:didAcceptConnectionWithInputStream:outputStream: delegate method.
     */
    NSNetServiceListenForConnections API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0)) = 1UL << 1
};



#pragma mark -

API_DEPRECATED("Use nw_connection_t or nw_listener_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(watchos)
@interface NSNetService : NSObject {
@private
    id _netService;
    id _delegate;
    id _reserved;
}

/* This is the initializer for publishing. You should use this initializer if you are going to announce the availability of a service on the network. To publish a service in all available domains, pass the empty string as the domain.
*/
- (instancetype)initWithDomain:(NSString *)domain type:(NSString *)type name:(NSString *)name port:(int)port NS_DESIGNATED_INITIALIZER;

/* This is the initializer for resolution. If you know the domain, type and name of the service for which you wish to discover addresses, you should initialize an NSNetService instance using this method and call resolve: on the result. If you wish to connect to this service immediately, you should call getInputStream:getOutputStream: on the result and forego the resolution step entirely.

If publish: is called on an NSNetService instance initialized with this method, an NSNetServicesBadArgumentError will be sent in the error dictionary to the delegate's netService:didNotPublish: method.
*/
- (instancetype)initWithDomain:(NSString *)domain type:(NSString *)type name:(NSString *)name;

/* NSNetService instances may be scheduled on NSRunLoops to operate in different modes, or in other threads. It is generally not necessary to schedule NSNetServices in other threads. NSNetServices are scheduled in the current thread's NSRunLoop in the NSDefaultRunLoopMode when they are created.
*/
- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;

/* Set a delegate to receive publish, resolve, or monitor events.
 */
@property (nullable, assign) id <NSNetServiceDelegate> delegate;

/* Initially set to NO. Set to YES to also publish, resolve, or monitor this service over peer to peer Wi-Fi (if available). Must be set before operation starts.
*/
@property BOOL includesPeerToPeer API_AVAILABLE(macos(10.10), ios(7.0), watchos(2.0), tvos(9.0));

/* Returns the name of the discovered or published service.
*/
@property (readonly, copy) NSString *name;

/* Returns the type of the discovered or published service.
*/
@property (readonly, copy) NSString *type;

/* Returns the domain of the discovered or published service.
*/
@property (readonly, copy) NSString *domain;

/* Returns the DNS host name of the computer hosting the discovered or published service. If a successful resolve has not yet occurred, this method will return nil.
*/
@property (nullable, readonly, copy) NSString *hostName;

/* The addresses of the service. This is an NSArray of NSData instances, each of which contains a single struct sockaddr suitable for use with connect(2). In the event that no addresses are resolved for the service or the service has not yet been resolved, an empty NSArray is returned.
*/
@property (nullable, readonly, copy) NSArray<NSData *> *addresses;

/* The port of a resolved service. This returns -1 if the service has not been resolved.
*/
@property (readonly) NSInteger port API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
 
/* Advertises a given service on the network. This method returns immediately. Success or failure is indicated by callbacks to the NSNetService instance's delegate.
 
    If the name of the service is the default name (@""), then the service will be renamed automatically. If the name of the service has been specified, then the service will not be renamed automatically. If more control over renaming is required, then -[NSNetService publishWithOptions:] is available.
*/
- (void)publish;

/* Advertises a given service on the network. This method returns immediately. Success or failure is indicated by callbacks to the NSNetService instance's delegate.
 
    See the notes above for NSNetServiceNoAutoRename for information about controlling the auto-renaming behavior using this method.
*/
- (void)publishWithOptions:(NSNetServiceOptions)options API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));


/* Attempts to determine at least one address for the NSNetService instance. For applications linked on or after Mac OS X 10.4 "Tiger", this method calls -resolveWithTimeout: with a value of 5.0. Applications linked prior to Mac OS X 10.4 "Tiger" must call -stop on the instance after an appropriate (short) amount of time to avoid causing unnecessary network traffic.
*/
- (void)resolve API_DEPRECATED("Not supported", macos(10.2,10.4), ios(2.0,2.0), watchos(2.0,2.0), tvos(9.0,9.0));

/* Halts a service which is either publishing or resolving.
*/
- (void)stop;

/* Returns an NSDictionary created from the provided NSData. The keys will be UTF8-encoded NSStrings. The values are NSDatas. The caller is responsible for interpreting these as types appropriate to the keys. If the NSData cannot be converted into an appropriate NSDictionary, this method will return nil. For applications linked on or after Mac OS X 10.5, this method will throw an NSInvalidException if it is passed nil as the argument.
*/
+ (NSDictionary<NSString *, NSData *> *)dictionaryFromTXTRecordData:(NSData *)txtData;

/* Returns an NSData created from the provided dictionary. The keys in the provided dictionary must be NSStrings, and the values must be NSDatas. If the dictionary cannot be converted into an NSData suitable for a TXT record, this method will return nil. For applications linked on or after Mac OS X 10.5, this method will throw an NSInvalidArgumentException if it is passed nil as the argument.
*/
+ (NSData *)dataFromTXTRecordDictionary:(NSDictionary<NSString *, NSData *> *)txtDictionary;

/* Starts a resolve for the NSNetService instance of the specified duration. If the delegate's -netServiceDidResolveAddress: method is called before the timeout expires, the resolve is successful. If the timeout is reached, the delegate's -netService:didNotResolve: method will be called. The value of the NSNetServicesErrorCode key in the error dictionary will be NSNetServicesTimeoutError.
*/
- (void)resolveWithTimeout:(NSTimeInterval)timeout;

/* Retrieves streams from the NSNetService instance. The instance's delegate methods are not called. Returns YES if the streams requested are created successfully. Returns NO if or any reason the stream could not be created. If only one stream is desired, pass NULL for the address of the other stream. The streams that are created are not open, and are not scheduled in any run loop for any mode.
*/
- (BOOL)getInputStream:(out __strong NSInputStream * _Nullable * _Nullable)inputStream outputStream:(out __strong NSOutputStream * _Nullable * _Nullable)outputStream;

/* Sets the TXT record of the NSNetService instance that has been or will be published. Pass nil to remove the TXT record from the instance.
*/
- (BOOL)setTXTRecordData:(nullable NSData *)recordData;

/* Returns the raw TXT record of the NSNetService instance. If the instance has not been resolved, or the delegate's -netService:didUpdateTXTRecordData: has not been called, this will return nil. It is permitted to have a zero-length TXT record.
*/
- (nullable NSData *)TXTRecordData;

/* Starts monitoring the NSNetService instance for events. In Mac OS X 10.4 Tiger, monitored NSNetService instances inform their delegates of changes to the instance's TXT record by calling the delegate's -netService:didUpdateTXTRecordData: method.
*/
- (void)startMonitoring;

/* Stops monitoring the NSNetService instance for events.
*/
- (void)stopMonitoring;


@end

#pragma mark -

API_DEPRECATED("Use nw_browser_t in Network framework instead", macos(10.2, API_TO_BE_DEPRECATED), ios(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(watchos)
@interface NSNetServiceBrowser : NSObject {
@private
    id _netServiceBrowser;
    id _delegate;
    void * _reserved;
}

- (instancetype)init;

/* Set a delegate to receive discovery events.
*/
@property (nullable, assign) id <NSNetServiceBrowserDelegate> delegate;

/* Initially set to NO. Set to YES to also browse over peer to peer Wi-Fi (if available). Must be set before starting to search.
*/
@property BOOL includesPeerToPeer API_AVAILABLE(macos(10.10), ios(7.0), watchos(2.0), tvos(9.0));

/* NSNetServiceBrowser instances may be scheduled on NSRunLoops to operate in different modes, or in other threads. It is generally not necessary to schedule NSNetServiceBrowsers in other threads. NSNetServiceBrowsers are scheduled in the current thread's NSRunLoop in the NSDefaultRunLoopMode when they are created.
*/
- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;

/* Starts a search for domains that are browsable via Bonjour and the computer's network configuration. Discovered domains are reported to the delegate's -netServiceBrowser:didFindDomain:moreComing: method. There may be more than one browsable domain.
*/
- (void)searchForBrowsableDomains;

/* Starts a search for domains in which the network configuration allows registration (i.e. publishing). Most NSNetServiceBrowser clients do not need to use this API, as it is sufficient to publish an NSNetService instance with the empty string (see -[NSNetService initWithDomain:type:name:port:]). Discovered domains are reported to the delegate's -netServiceBrowser:didFindDomain:moreComing: method. There may be more than one registration domain.
*/
- (void)searchForRegistrationDomains;

/* Starts a search for services of the specified type in the domain indicated by domainString. For each service discovered, a -netServiceBrowser:foundService:moreComing: message is sent to the NSNetServiceBrowser instance's delegate.
*/
- (void)searchForServicesOfType:(NSString *)type inDomain:(NSString *)domainString;

/* Stops the currently running search.
*/
- (void)stop;

@end

#pragma mark -

API_AVAILABLE(macos(10.2), ios(2.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@protocol NSNetServiceDelegate <NSObject>
@optional

/* Sent to the NSNetService instance's delegate prior to advertising the service on the network. If for some reason the service cannot be published, the delegate will not receive this message, and an error will be delivered to the delegate via the delegate's -netService:didNotPublish: method.
*/
- (void)netServiceWillPublish:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when the publication of the instance is complete and successful.
*/
- (void)netServiceDidPublish:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when an error in publishing the instance occurs. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants). It is possible for an error to occur after a successful publication.
*/
- (void)netService:(NSNetService *)sender didNotPublish:(NSDictionary<NSString *, NSNumber *> *)errorDict;

/* Sent to the NSNetService instance's delegate prior to resolving a service on the network. If for some reason the resolution cannot occur, the delegate will not receive this message, and an error will be delivered to the delegate via the delegate's -netService:didNotResolve: method.
*/
- (void)netServiceWillResolve:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when one or more addresses have been resolved for an NSNetService instance. Some NSNetService methods will return different results before and after a successful resolution. An NSNetService instance may get resolved more than once; truly robust clients may wish to resolve again after an error, or to resolve more than once.
*/
- (void)netServiceDidResolveAddress:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when an error in resolving the instance occurs. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants).
*/
- (void)netService:(NSNetService *)sender didNotResolve:(NSDictionary<NSString *, NSNumber *> *)errorDict;

/* Sent to the NSNetService instance's delegate when the instance's previously running publication or resolution request has stopped.
*/
- (void)netServiceDidStop:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when the instance is being monitored and the instance's TXT record has been updated. The new record is contained in the data parameter.
*/
- (void)netService:(NSNetService *)sender didUpdateTXTRecordData:(NSData *)data;
    
    
/* Sent to a published NSNetService instance's delegate when a new connection is
 * received. Before you can communicate with the connecting client, you must -open
 * and schedule the streams. To reject a connection, just -open both streams and
 * then immediately -close them.
 
 * To enable TLS on the stream, set the various TLS settings using
 * kCFStreamPropertySSLSettings before calling -open. You must also specify
 * kCFBooleanTrue for kCFStreamSSLIsServer in the settings dictionary along with
 * a valid SecIdentityRef as the first entry of kCFStreamSSLCertificates.
 */
- (void)netService:(NSNetService *)sender didAcceptConnectionWithInputStream:(NSInputStream *)inputStream outputStream:(NSOutputStream *)outputStream API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@end

#pragma mark -

API_AVAILABLE(macos(10.2), ios(2.0), tvos(9.0)) API_UNAVAILABLE(watchos)
@protocol NSNetServiceBrowserDelegate <NSObject>
@optional

/* Sent to the NSNetServiceBrowser instance's delegate before the instance begins a search. The delegate will not receive this message if the instance is unable to begin a search. Instead, the delegate will receive the -netServiceBrowser:didNotSearch: message.
*/
- (void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)browser;

/* Sent to the NSNetServiceBrowser instance's delegate when the instance's previous running search request has stopped.
*/
- (void)netServiceBrowserDidStopSearch:(NSNetServiceBrowser *)browser;

/* Sent to the NSNetServiceBrowser instance's delegate when an error in searching for domains or services has occurred. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants). It is possible for an error to occur after a search has been started successfully.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didNotSearch:(NSDictionary<NSString *, NSNumber *> *)errorDict;

/* Sent to the NSNetServiceBrowser instance's delegate for each domain discovered. If there are more domains, moreComing will be YES. If for some reason handling discovered domains requires significant processing, accumulating domains until moreComing is NO and then doing the processing in bulk fashion may be desirable.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindDomain:(NSString *)domainString moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate for each service discovered. If there are more services, moreComing will be YES. If for some reason handling discovered services requires significant processing, accumulating services until moreComing is NO and then doing the processing in bulk fashion may be desirable.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didFindService:(NSNetService *)service moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate when a previously discovered domain is no longer available.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveDomain:(NSString *)domainString moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate when a previously discovered service is no longer published.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)browser didRemoveService:(NSNetService *)service moreComing:(BOOL)moreComing;

@end

NS_ASSUME_NONNULL_END
