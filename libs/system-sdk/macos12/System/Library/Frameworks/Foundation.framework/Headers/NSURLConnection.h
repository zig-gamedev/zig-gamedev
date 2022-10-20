/*    
    NSURLConnection.h
    Copyright (c) 2003-2019, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSRunLoop.h>

@class NSArray;
@class NSURL;
@class NSCachedURLResponse;
@class NSData;
@class NSError;
@class NSURLAuthenticationChallenge;
@class NSURLConnectionInternal;
@class NSURLRequest;
@class NSURLResponse;
@class NSRunLoop;
@class NSInputStream;
@class NSURLProtectionSpace;
@class NSOperationQueue;

@protocol NSURLConnectionDelegate;

NS_ASSUME_NONNULL_BEGIN

/*** DEPRECATED: The NSURLConnection class should no longer be used.  NSURLSession is the replacement for NSURLConnection ***/

/*!
    @class NSURLConnection
    
    @abstract An NSURLConnection object provides support to perform
        asynchronous loads of a URL request, providing data to a
        client supplied delegate.
    
    @discussion The interface for NSURLConnection is very sparse, providing
        only the controls to start and cancel asynchronous loads of a
        URL request.<p>

        An NSURLConnection may be used for loading of resource data
        directly to memory, in which case an
        NSURLConnectionDataDelegate should be supplied, or for
        downloading of resource data directly to a file, in which case
        an NSURLConnectionDownloadDelegate is used.  The delegate is
        retained by the NSURLConnection until a terminal condition is
        encountered.  These two delegates are logically subclasses of
        the base protocol, NSURLConnectionDelegate.<p>

        A terminal condition produced by the loader will result in a
        connection:didFailWithError: in the case of an error, or
        connectiondidFinishLoading: or connectionDidFinishDownloading:
        delegate message.<p>

        The -cancel message hints to the loader that a resource load
        should be abandoned but does not guarantee that more delegate
        messages will not be delivered.  If -cancel does cause the
        load to be abandoned, the delegate will be released without
        further messages.  In general, a caller should be prepared for
        -cancel to have no effect, and internally ignore any delegate
        callbacks until the delegate is released.

        Scheduling of an NSURLConnection specifies the context in
        which delegate callbacks will be made, but the actual IO may
        occur on a separate thread and should be considered an
        implementation detail.<p>

        When created, an NSURLConnection performs a deep-copy of the
        NSURLRequest.  This copy is available through the
        -originalRequest method.  As the connection performs the load,
        this request may change as a result of protocol
        canonicalization or due to following redirects.
        -currentRequest can be used to retrieve this value.<p>

        An NSURLConnections created with the
        +connectionWithRequest:delegate: or -initWithRequest:delegate:
        methods are scheduled on the current runloop immediately, and
        it is not necessary to send the -start message to begin the
        resource load.<p>

        NSURLConnections created with
        -initWithRequest:delegate:startImmediately: are not
        automatically scheduled.  Use -scheduleWithRunLoop:forMode: or
        -setDelegateQueue: to specify the context for delegate
        callbacks, and -start to begin the load.  If you do not
        explicitly schedule the connection before -start, it will be
        scheduled on the current runloop and mode automatically.<p>

        The NSURLConnectionSynchronousLoading category adds
        +sendSynchronousRequest:returningResponse:error, which blocks
        the current thread until the resource data is available or an
        error occurs.  It should be noted that using this method on an
        applications main run loop may result in an unacceptably long
        delay in a user interface and its use is strongly
        discourage.<p>

        The NSURLConnectionQueuedLoading category implements
        +sendAsynchronousRequest:queue:completionHandler, providing
        similar simplicity but provides a mechanism where the current
        runloop is not blocked.<p>

        Both of the immediate loading categories do not provide for
        customization of resource load, and do not allow the caller to
        respond to, e.g., authentication challenges.<p>
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@interface NSURLConnection : NSObject
{
    @private
    NSURLConnectionInternal *_internal;
}

/* Designated initializer */
- (nullable instancetype)initWithRequest:(NSURLRequest *)request delegate:(nullable id)delegate startImmediately:(BOOL)startImmediately API_DEPRECATED("Use NSURLSession (see NSURLSession.h)", macos(10.5,10.11), ios(2.0,9.0), tvos(9.0,9.0)) API_UNAVAILABLE(watchos);

- (nullable instancetype)initWithRequest:(NSURLRequest *)request delegate:(nullable id)delegate API_DEPRECATED("Use NSURLSession (see NSURLSession.h)", macos(10.3,10.11), ios(2.0,9.0), tvos(9.0,9.0)) API_UNAVAILABLE(watchos);
+ (nullable NSURLConnection*)connectionWithRequest:(NSURLRequest *)request delegate:(nullable id)delegate API_DEPRECATED("Use NSURLSession (see NSURLSession.h)", macos(10.3,10.11), ios(2.0,9.0), tvos(9.0,9.0)) API_UNAVAILABLE(watchos);

@property (readonly, copy) NSURLRequest *originalRequest API_AVAILABLE(macos(10.8), ios(5.0), watchos(2.0), tvos(9.0));
@property (readonly, copy) NSURLRequest *currentRequest API_AVAILABLE(macos(10.8), ios(5.0), watchos(2.0), tvos(9.0));

- (void)start API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (void)cancel;

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (void)unscheduleFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (void)setDelegateQueue:(nullable NSOperationQueue*) queue API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));


/*! 
    @method         canHandleRequest:

    @abstract
                    Performs a "preflight" operation that performs
                    some speculative checks to see if a connection can
                    be initialized, and the associated I/O that is
                    started in the initializer methods can begin.

    @discussion
                    The result of this method is valid only as long as
                    no protocols are registered or unregistered, and
                    as long as the request is not mutated (if the
                    request is mutable). Hence, clients should be
                    prepared to handle failures even if they have
                    performed request preflighting by calling this
                    method.

    @param 
        request     The request to preflight.

    @result         YES if it is likely that the given request can be used to
                    initialize a connection and the associated I/O can be
                    started, NO otherwise.
 
*/
+ (BOOL)canHandleRequest:(NSURLRequest *)request;

@end

/*!
    @protocol       NSURLConnectionDelegate

    @abstract        
                    Delegate methods that are common to all forms of
                    NSURLConnection.  These are all optional.  This
                    protocol should be considered a base class for the
                    NSURLConnectionDataDelegate and
                    NSURLConnectionDownloadDelegate protocols.

    @discussion
                    connection:didFailWithError: will be called at
                    most once, if an error occurs during a resource
                    load.  No other callbacks will be made after.<p>

                    connectionShouldUseCredentialStorage: will be
                    called at most once, before a resource load begins
                    (which means it may be called during construction
                    of the connection.)  The delegate should return
                    TRUE if the connection should consult the shared
                    NSURLCredentialStorage in response to
                    authentication challenges.  Regardless of the
                    result, the authentication challenge methods may
                    still be called.

                    connection:willSendRequestForAuthenticationChallenge:
                    is the preferred (Mac OS X 10.7 and iOS 5.0 or
                    later) mechanism for responding to authentication
                    challenges.  See
                    <Foundation/NSURLAuthenticationChallenge.h> for
                    more information on dealing with the various types
                    of authentication challenges.

                    connection:canAuthenticateAgainstProtectionSpace:
                    connection:didReciveAuthenticationChallenge:
                    connection:didCancelAuthenticationChallenge: are
                    deprected and new code should adopt
                    connection:willSendRequestForAuthenticationChallenge.
                    The older delegates will still be called for
                    compatability, but incur more latency in dealing
                    with the authentication challenge.
*/
API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@protocol NSURLConnectionDelegate <NSObject>
@optional
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;
- (BOOL)connectionShouldUseCredentialStorage:(NSURLConnection *)connection;
- (void)connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;
- (BOOL)connection:(NSURLConnection *)connection canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace API_DEPRECATED("Use -connection:willSendRequestForAuthenticationChallenge: instead.", macos(10.6,10.10), ios(3.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge API_DEPRECATED("Use -connection:willSendRequestForAuthenticationChallenge: instead.", macos(10.2,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
- (void)connection:(NSURLConnection *)connection didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge API_DEPRECATED("Use -connection:willSendRequestForAuthenticationChallenge: instead.", macos(10.2,10.10), ios(2.0,8.0), watchos(2.0,2.0), tvos(9.0,9.0));
@end

/*!
    @protocol       NSURLConnectionDataDelegate

    @abstract        
                    Delegate methods used for loading data to memory.
                    These delegate methods are all optional.

    @discussion
                    connection:willSendRequest:redirectResponse: is
                    called whenever an connection determines that it
                    must change URLs in order to continue loading a
                    request.  This gives the delegate an opportunity
                    inspect and if necessary modify a request.  A
                    delegate can cause the request to abort by either
                    calling the connections -cancel method, or by
                    returning nil from this callback.<p>

                    There is one subtle difference which results from
                    this choice. If -cancel is called in the delegate
                    method, all processing for the connection stops,
                    and no further delegate callbacks will be sent. If
                    the delegate returns nil, the connection will
                    continue to process, and this has special
                    relevance in the case where the redirectResponse
                    argument is non-nil. In this case, any data that
                    is loaded for the connection will be sent to the
                    delegate, and the delegate will receive a finished
                    or failure delegate callback as appropriate.<p>

                    connection:didReceiveResponse: is called when
                    enough data has been read to construct an
                    NSURLResponse object. In the event of a protocol
                    which may return multiple responses (such as HTTP
                    multipart/x-mixed-replace) the delegate should be
                    prepared to inspect the new response and make
                    itself ready for data callbacks as appropriate.<p>

                    connection:didReceiveData: is called with a single
                    immutable NSData object to the delegate,
                    representing the next portion of the data loaded
                    from the connection.  This is the only guaranteed
                    for the delegate to receive the data from the
                    resource load.<p>

                    connection:needNewBodyStream: is called when the
                    loader must retransmit a requests payload, due to
                    connection errors or authentication challenges.
                    Delegates should construct a new unopened and
                    autoreleased NSInputStream.  If not implemented,
                    the loader will be required to spool the bytes to
                    be uploaded to disk, a potentially expensive
                    operation.  Returning nil will cancel the
                    connection.

                    connection:didSendBodyData:totalBytesWritten:totalBytesExpectedToWrite:
                    is called during an upload operation to provide
                    progress feedback.  Note that the values may
                    change in unexpected ways if the request needs to
                    be retransmitted.<p>

                    connection:willCacheResponse: gives the delegate
                    an opportunity to inspect and modify the
                    NSCachedURLResponse which will be cached by the
                    loader if caching is enabled for the original
                    NSURLRequest.  Returning nil from this delegate
                    will prevent the resource from being cached.  Note
                    that the -data method of the cached response may
                    return an autoreleased in-memory copy of the true
                    data, and should not be used as an alternative to
                    receiving and accumulating the data through
                    connection:didReceiveData:<p>

                    connectionDidFinishLoading: is called when all
                    connection processing has completed successfully,
                    before the delegate is released by the
                    connection.<p>
*/

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@protocol NSURLConnectionDataDelegate <NSURLConnectionDelegate>
@optional
- (nullable NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(nullable NSURLResponse *)response;
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;

- (nullable NSInputStream *)connection:(NSURLConnection *)connection needNewBodyStream:(NSURLRequest *)request;
- (void)connection:(NSURLConnection *)connection   didSendBodyData:(NSInteger)bytesWritten
                                                 totalBytesWritten:(NSInteger)totalBytesWritten
                                         totalBytesExpectedToWrite:(NSInteger)totalBytesExpectedToWrite;

- (nullable NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse;

- (void)connectionDidFinishLoading:(NSURLConnection *)connection;
@end

/*!
    @protocol       NSURLConnectionDownloadDelegate

    @abstract
                    Delegate methods used to perform resource
                    downloads directly to a disk file.  All the
                    methods are optional with the exception of
                    connectionDidFinishDownloading:destinationURL:
                    which must be implemented in order to inform the
                    delegate of the location of the finished download.
                    This delegate and download implementation is
                    currently only available on iOS 5.0 or later.

    @discussion
                    connection:didWriteData:totalBytesWritten:expectedTotalBytes:
                    provides progress information about the state of
                    the download, the number of bytes written since
                    the last delegate callback, the total number of
                    bytes written to disk and the total number of
                    bytes that are expected (or 0 if this is unknown.)

                    connectionDidResumeDownloading:totalBytesWritten:expectedTotalBytes:
                    is called when the connection is able to resume an
                    in progress download.  This may happen due to a
                    connection or network failure.

                    connectionDidFinishDownloading:destinationURL: is
                    a terminal event which indicates the completion of
                    a download and provides the location of the file.
                    The file will be located in the applications cache
                    directory and is guaranteed to exist for the
                    duration of the delegate callback.  The
                    implication is that the delegate should copy or
                    move the download to a more persistent location if
                    desired.
*/

API_AVAILABLE(macos(10.2), ios(2.0), watchos(2.0), tvos(9.0))
@protocol NSURLConnectionDownloadDelegate <NSURLConnectionDelegate>
@optional
- (void)connection:(NSURLConnection *)connection didWriteData:(long long)bytesWritten totalBytesWritten:(long long)totalBytesWritten expectedTotalBytes:(long long) expectedTotalBytes;
- (void)connectionDidResumeDownloading:(NSURLConnection *)connection totalBytesWritten:(long long)totalBytesWritten expectedTotalBytes:(long long) expectedTotalBytes;

@required
- (void)connectionDidFinishDownloading:(NSURLConnection *)connection destinationURL:(NSURL *) destinationURL;
@end

/*!
    @category    NSURLConnection(NSURLConnectionSynchronousLoading)

    @abstract
                 The NSURLConnectionSynchronousLoading category on
                 NSURLConnection provides the interface to perform
                 synchronous loading of URL requests.
*/
@interface NSURLConnection (NSURLConnectionSynchronousLoading)

/*! 
    @method      sendSynchronousRequest:returningResponse:error:

    @abstract 
                 Performs a synchronous load of the given request,
                 returning an NSURLResponse in the given out
                 parameter.

    @discussion
                 A synchronous load for the given request is built on
                 top of the asynchronous loading code made available
                 by the class.  The calling thread is blocked while
                 the asynchronous loading system performs the URL load
                 on a thread spawned specifically for this load
                 request. No special threading or run loop
                 configuration is necessary in the calling thread in
                 order to perform a synchronous load. For instance,
                 the calling thread need not be running its run loop.

    @param
       request   The request to load. Note that the request is
                 deep-copied as part of the initialization
                 process. Changes made to the request argument after
                 this method returns do not affect the request that is
                 used for the loading process.

    @param
       response  An out parameter which is filled in with the
                 response generated by performing the load.

    @param
       error     Out parameter (may be NULL) used if an error occurs
                 while processing the request. Will not be modified if the 
                 load succeeds.

    @result      The content of the URL resulting from performing the load,
                 or nil if the load failed.
*/
+ (nullable NSData *)sendSynchronousRequest:(NSURLRequest *)request returningResponse:(NSURLResponse * _Nullable * _Nullable)response error:(NSError **)error API_DEPRECATED("Use [NSURLSession dataTaskWithRequest:completionHandler:] (see NSURLSession.h", macos(10.3,10.11), ios(2.0,9.0), tvos(9.0,9.0)) API_UNAVAILABLE(watchos);

@end

/*!
    @category NSURLConnection(NSURLConnectionQueuedLoading)

    The NSURLConnectionQueuedLoading category on NSURLConnection
    provides the interface to perform asynchronous loading of URL
    requests where the results of the request are delivered to a
    block via an NSOperationQueue.

    Note that there is no guarantee of load ordering implied by this
    method.
 */
@interface NSURLConnection (NSURLConnectionQueuedLoading)

/*!
    @method       sendAsynchronousRequest:queue:completionHandler:

    @abstract 
                  Performs an asynchronous load of the given
                  request. When the request has completed or failed,
                  the block will be executed from the context of the
                  specified NSOperationQueue.

    @discussion
                  This is a convenience routine that allows for
                  asynchronous loading of a url-based resource.  If
                  the resource load is successful, the data parameter
                  to the callback will contain the resource data and
                  the error parameter will be nil.  If the resource
                  load fails, the data parameter will be nil and the
                  error will contain information about the failure.

    @param
         request   The request to load. Note that the request is
                   deep-copied as part of the initialization
                   process. Changes made to the request argument after
                   this method returns do not affect the request that
                   is used for the loading process.

    @param 
         queue     An NSOperationQueue upon which    the handler block will
                   be dispatched.

    @param
         handler   A block which receives the results of the resource load.
 */
+ (void)sendAsynchronousRequest:(NSURLRequest*) request
                          queue:(NSOperationQueue*) queue
              completionHandler:(void (^)(NSURLResponse* _Nullable response, NSData* _Nullable data, NSError* _Nullable connectionError)) handler API_DEPRECATED("Use [NSURLSession dataTaskWithRequest:completionHandler:] (see NSURLSession.h", macos(10.7,10.11), ios(5.0,9.0), tvos(9.0,9.0)) API_UNAVAILABLE(watchos);
           
@end

NS_ASSUME_NONNULL_END
