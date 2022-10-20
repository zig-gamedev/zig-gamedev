/*	NSURLSession.h
	Copyright (c) 2013-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSURLRequest.h>
#import <Foundation/NSHTTPCookieStorage.h>
#import <Foundation/NSProgress.h>

#include <Security/SecureTransport.h>

@class NSString;
@class NSURL;
@class NSError;
@class NSArray<ObjectType>;
@class NSDictionary<KeyType, ObjectType>;
@class NSInputStream;
@class NSOutputStream;
@class NSData;
@class NSOperationQueue;

@class NSURLCache;
@class NSURLResponse;
@class NSHTTPURLResponse;
@class NSHTTPCookie;
@class NSCachedURLResponse;
@class NSURLAuthenticationChallenge;
@class NSURLProtectionSpace;
@class NSURLCredential;
@class NSURLCredentialStorage;
@class NSURLSessionDataTask;
@class NSURLSessionUploadTask;
@class NSURLSessionDownloadTask;
@class NSNetService;

/*

 NSURLSession is a replacement API for NSURLConnection.  It provides
 options that affect the policy of, and various aspects of the
 mechanism by which NSURLRequest objects are retrieved from the
 network.

 An NSURLSession may be bound to a delegate object.  The delegate is
 invoked for certain events during the lifetime of a session, such as
 server authentication or determining whether a resource to be loaded
 should be converted into a download.
 
 NSURLSession instances are threadsafe.

 The default NSURLSession uses a system provided delegate and is
 appropriate to use in place of existing code that uses
 +[NSURLConnection sendAsynchronousRequest:queue:completionHandler:]

 An NSURLSession creates NSURLSessionTask objects which represent the
 action of a resource being loaded.  These are analogous to
 NSURLConnection objects but provide for more control and a unified
 delegate model.
 
 NSURLSessionTask objects are always created in a suspended state and
 must be sent the -resume message before they will execute.

 Subclasses of NSURLSessionTask are used to syntactically
 differentiate between data and file downloads.

 An NSURLSessionDataTask receives the resource as a series of calls to
 the URLSession:dataTask:didReceiveData: delegate method.  This is type of
 task most commonly associated with retrieving objects for immediate parsing
 by the consumer.

 An NSURLSessionUploadTask differs from an NSURLSessionDataTask
 in how its instance is constructed.  Upload tasks are explicitly created
 by referencing a file or data object to upload, or by utilizing the
 -URLSession:task:needNewBodyStream: delegate message to supply an upload
 body.

 An NSURLSessionDownloadTask will directly write the response data to
 a temporary file.  When completed, the delegate is sent
 URLSession:downloadTask:didFinishDownloadingToURL: and given an opportunity 
 to move this file to a permanent location in its sandboxed container, or to
 otherwise read the file. If canceled, an NSURLSessionDownloadTask can
 produce a data blob that can be used to resume a download at a later
 time.

 Beginning with iOS 9 and Mac OS X 10.11, NSURLSessionStream is
 available as a task type.  This allows for direct TCP/IP connection
 to a given host and port with optional secure handshaking and
 navigation of proxies.  Data tasks may also be upgraded to a
 NSURLSessionStream task via the HTTP Upgrade: header and appropriate
 use of the pipelining option of NSURLSessionConfiguration.  See RFC
 2817 and RFC 6455 for information about the Upgrade: header, and
 comments below on turning data tasks into stream tasks.

 An NSURLSessionWebSocketTask is a task that allows clients to connect to servers supporting
 WebSocket. The task will perform the HTTP handshake to upgrade the connection
 and once the WebSocket handshake is successful, the client can read and write
 messages that will be framed using the WebSocket protocol by the framework.
 */

@class NSURLSession;
@class NSURLSessionDataTask;                /* DataTask objects receive the payload through zero or more delegate messages */
@class NSURLSessionUploadTask;              /* UploadTask objects receive periodic progress updates but do not return a body */
@class NSURLSessionDownloadTask;            /* DownloadTask objects represent an active download to disk.  They can provide resume data when canceled. */
@class NSURLSessionStreamTask;              /* StreamTask objects may be used to create NSInput and NSOutputStreams, or used directly in reading and writing. */
@class NSURLSessionWebSocketTask;           /* WebSocket objects perform a WebSocket handshake with the server and can be used to send and receive WebSocket messages */

@class NSURLSessionConfiguration;
@protocol NSURLSessionDelegate;
@protocol NSURLSessionTaskDelegate;

@class NSURLSessionTaskMetrics;
@class NSDateInterval;

NS_ASSUME_NONNULL_BEGIN

#define NSURLSESSION_AVAILABLE	10_9

FOUNDATION_EXPORT const int64_t NSURLSessionTransferSizeUnknown API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));    /* -1LL */

API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSession : NSObject

/*
 * The shared session uses the currently set global NSURLCache,
 * NSHTTPCookieStorage and NSURLCredentialStorage objects.
 */
@property (class, readonly, strong) NSURLSession *sharedSession;

/*
 * Customization of NSURLSession occurs during creation of a new session.
 * If you only need to use the convenience routines with custom
 * configuration options it is not necessary to specify a delegate.
 * If you do specify a delegate, the delegate will be retained until after
 * the delegate has been sent the URLSession:didBecomeInvalidWithError: message.
 */
+ (NSURLSession *)sessionWithConfiguration:(NSURLSessionConfiguration *)configuration;
+ (NSURLSession *)sessionWithConfiguration:(NSURLSessionConfiguration *)configuration delegate:(nullable id <NSURLSessionDelegate>)delegate delegateQueue:(nullable NSOperationQueue *)queue;

@property (readonly, retain) NSOperationQueue *delegateQueue;
@property (nullable, readonly, retain) id <NSURLSessionDelegate> delegate;
@property (readonly, copy) NSURLSessionConfiguration *configuration;

/*
 * The sessionDescription property is available for the developer to
 * provide a descriptive label for the session.
 */
@property (nullable, copy) NSString *sessionDescription;

/* -finishTasksAndInvalidate returns immediately and existing tasks will be allowed
 * to run to completion.  New tasks may not be created.  The session
 * will continue to make delegate callbacks until URLSession:didBecomeInvalidWithError:
 * has been issued. 
 *
 * -finishTasksAndInvalidate and -invalidateAndCancel do not
 * have any effect on the shared session singleton.
 *
 * When invalidating a background session, it is not safe to create another background
 * session with the same identifier until URLSession:didBecomeInvalidWithError: has
 * been issued.
 */
- (void)finishTasksAndInvalidate;

/* -invalidateAndCancel acts as -finishTasksAndInvalidate, but issues
 * -cancel to all outstanding tasks for this session.  Note task 
 * cancellation is subject to the state of the task, and some tasks may
 * have already have completed at the time they are sent -cancel. 
 */
- (void)invalidateAndCancel;

- (void)resetWithCompletionHandler:(void (^)(void))completionHandler;    /* empty all cookies, cache and credential stores, removes disk files, issues -flushWithCompletionHandler:. Invokes completionHandler() on the delegate queue if not nil. */
- (void)flushWithCompletionHandler:(void (^)(void))completionHandler;    /* flush storage to disk and clear transient network caches.  Invokes completionHandler() on the delegate queue if not nil. */

- (void)getTasksWithCompletionHandler:(void (^)(NSArray<NSURLSessionDataTask *> *dataTasks, NSArray<NSURLSessionUploadTask *> *uploadTasks, NSArray<NSURLSessionDownloadTask *> *downloadTasks))completionHandler NS_SWIFT_ASYNC_NAME(getter:tasks()); /* invokes completionHandler with outstanding data, upload and download tasks. */

- (void)getAllTasksWithCompletionHandler:(void (^)(NSArray<__kindof NSURLSessionTask *> *tasks))completionHandler NS_SWIFT_ASYNC_NAME(getter:allTasks()) API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)); /* invokes completionHandler with all outstanding tasks. */

/* 
 * NSURLSessionTask objects are always created in a suspended state and
 * must be sent the -resume message before they will execute.
 */

/* Creates a data task with the given request.  The request may have a body stream. */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request;

/* Creates a data task to retrieve the contents of the given URL. */
- (NSURLSessionDataTask *)dataTaskWithURL:(NSURL *)url;

/* Creates an upload task with the given request.  The body of the request will be created from the file referenced by fileURL */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromFile:(NSURL *)fileURL;

/* Creates an upload task with the given request.  The body of the request is provided from the bodyData. */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromData:(NSData *)bodyData;

/* Creates an upload task with the given request.  The previously set body stream of the request (if any) is ignored and the URLSession:task:needNewBodyStream: delegate will be called when the body payload is required. */
- (NSURLSessionUploadTask *)uploadTaskWithStreamedRequest:(NSURLRequest *)request;

/* Creates a download task with the given request. */
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request;

/* Creates a download task to download the contents of the given URL. */
- (NSURLSessionDownloadTask *)downloadTaskWithURL:(NSURL *)url;

/* Creates a download task with the resume data.  If the download cannot be successfully resumed, URLSession:task:didCompleteWithError: will be called. */
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData;

/* Creates a bidirectional stream task to a given host and port.
 */
- (NSURLSessionStreamTask *)streamTaskWithHostName:(NSString *)hostname port:(NSInteger)port API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/* Creates a bidirectional stream task with an NSNetService to identify the endpoint.
 * The NSNetService will be resolved before any IO completes.
 */
- (NSURLSessionStreamTask *)streamTaskWithNetService:(NSNetService *)service API_DEPRECATED("Use nw_connection_t in Network framework instead", macos(10.11, API_TO_BE_DEPRECATED), ios(9.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(watchos);

/* Creates a WebSocket task given the url. The given url must have a ws or wss scheme.
 */
- (NSURLSessionWebSocketTask *)webSocketTaskWithURL:(NSURL *)url API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* Creates a WebSocket task given the url and an array of protocols. The protocols will be used in the WebSocket handshake to
 * negotiate a prefered protocol with the server
 * Note - The protocol will not affect the WebSocket framing. More details on the protocol can be found by reading the WebSocket RFC
 */
- (NSURLSessionWebSocketTask *)webSocketTaskWithURL:(NSURL *)url protocols:(NSArray<NSString *>*)protocols API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* Creates a WebSocket task given the request. The request properties can be modified and will be used by the task during the HTTP handshake phase.
 * Clients who want to add custom protocols can do so by directly adding headers with the key Sec-WebSocket-Protocol
 * and a comma separated list of protocols they wish to negotiate with the server. The custom HTTP headers provided by the client will remain unchanged for the handshake with the server.
 */
- (NSURLSessionWebSocketTask *)webSocketTaskWithRequest:(NSURLRequest *)request API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));


- (instancetype)init API_DEPRECATED("Please use +[NSURLSession sessionWithConfiguration:] or other class methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use +[NSURLSession sessionWithConfiguration:] or other class methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

/*
 * NSURLSession convenience routines deliver results to 
 * a completion handler block.  These convenience routines
 * are not available to NSURLSessions that are configured
 * as background sessions.
 *
 * Task objects are always created in a suspended state and 
 * must be sent the -resume message before they will execute.
 */
@interface NSURLSession (NSURLSessionAsynchronousConvenience)
/*
 * data task convenience methods.  These methods create tasks that
 * bypass the normal delegate calls for response and data delivery,
 * and provide a simple cancelable asynchronous interface to receiving
 * data.  Errors will be returned in the NSURLErrorDomain, 
 * see <Foundation/NSURLError.h>.  The delegate, if any, will still be
 * called for authentication challenges.
 */
- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;
- (NSURLSessionDataTask *)dataTaskWithURL:(NSURL *)url completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/*
 * upload convenience method.
 */
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromFile:(NSURL *)fileURL completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request fromData:(nullable NSData *)bodyData completionHandler:(void (^)(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

/*
 * download task convenience methods.  When a download successfully
 * completes, the NSURL will point to a file that must be read or
 * copied during the invocation of the completion routine.  The file
 * will be removed automatically.
 */
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;
- (NSURLSessionDownloadTask *)downloadTaskWithURL:(NSURL *)url completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData completionHandler:(void (^)(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error))completionHandler;

@end

typedef NS_ENUM(NSInteger, NSURLSessionTaskState) {
    NSURLSessionTaskStateRunning = 0,                     /* The task is currently being serviced by the session */
    NSURLSessionTaskStateSuspended = 1,
    NSURLSessionTaskStateCanceling = 2,                   /* The task has been told to cancel.  The session will receive a URLSession:task:didCompleteWithError: message. */
    NSURLSessionTaskStateCompleted = 3,                   /* The task has completed and the session will receive no more delegate notifications */
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*
 * NSURLSessionTask - a cancelable object that refers to the lifetime
 * of processing a given request.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionTask : NSObject <NSCopying, NSProgressReporting>

@property (readonly)                 NSUInteger    taskIdentifier;    /* an identifier for this task, assigned by and unique to the owning session */
@property (nullable, readonly, copy) NSURLRequest  *originalRequest;  /* may be nil if this is a stream task */
@property (nullable, readonly, copy) NSURLRequest  *currentRequest;   /* may differ from originalRequest due to http server redirection */
@property (nullable, readonly, copy) NSURLResponse *response;         /* may be nil if no response has been received */

/* Sets a task-specific delegate. Methods not implemented on this delegate will
 * still be forwarded to the session delegate.
 *
 * Cannot be modified after task resumes. Not supported on background session.
 *
 * Delegate is strongly referenced until the task completes, after which it is
 * reset to `nil`.
 */
@property (nullable, retain) id <NSURLSessionTaskDelegate> delegate API_AVAILABLE(macos(12.0), ios(15.0), watchos(8.0), tvos(15.0));

/*
 * NSProgress object which represents the task progress.
 * It can be used for task progress tracking.
 */
@property (readonly, strong) NSProgress *progress API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/*
 * Start the network load for this task no earlier than the specified date. If
 * not specified, no start delay is used.
 *
 * Only applies to tasks created from background NSURLSession instances; has no
 * effect for tasks created from other session types.
 */
@property (nullable, copy) NSDate *earliestBeginDate API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/*
 * The number of bytes that the client expects (a best-guess upper-bound) will
 * be sent and received by this task. These values are used by system scheduling
 * policy. If unspecified, NSURLSessionTransferSizeUnknown is used.
 */
@property int64_t countOfBytesClientExpectsToSend API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));
@property int64_t countOfBytesClientExpectsToReceive API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));


/* Byte count properties may be zero if no body is expected, 
 * or NSURLSessionTransferSizeUnknown if it is not possible 
 * to know how many bytes will be transferred.
 */

/* number of body bytes already sent */
@property (readonly) int64_t countOfBytesSent;

/* number of body bytes already received */
@property (readonly) int64_t countOfBytesReceived;

/* number of body bytes we expect to send, derived from the Content-Length of the HTTP request */
@property (readonly) int64_t countOfBytesExpectedToSend;

/* number of byte bytes we expect to receive, usually derived from the Content-Length header of an HTTP response. */
@property (readonly) int64_t countOfBytesExpectedToReceive;

/*
 * The taskDescription property is available for the developer to
 * provide a descriptive label for the task.
 */
@property (nullable, copy) NSString *taskDescription;

/* -cancel returns immediately, but marks a task as being canceled.
 * The task will signal -URLSession:task:didCompleteWithError: with an
 * error value of { NSURLErrorDomain, NSURLErrorCancelled }.  In some 
 * cases, the task may signal other work before it acknowledges the 
 * cancelation.  -cancel may be sent to a task that has been suspended.
 */
- (void)cancel;

/*
 * The current state of the task within the session.
 */
@property (readonly) NSURLSessionTaskState state;

/*
 * The error, if any, delivered via -URLSession:task:didCompleteWithError:
 * This property will be nil in the event that no error occured.
 */
@property (nullable, readonly, copy) NSError *error;

/*
 * Suspending a task will prevent the NSURLSession from continuing to
 * load data.  There may still be delegate calls made on behalf of
 * this task (for instance, to report data received while suspending)
 * but no further transmissions will be made on behalf of the task
 * until -resume is sent.  The timeout timer associated with the task
 * will be disabled while a task is suspended. -suspend and -resume are
 * nestable. 
 */
- (void)suspend;
- (void)resume;

/*
 * Sets a scaling factor for the priority of the task. The scaling factor is a
 * value between 0.0 and 1.0 (inclusive), where 0.0 is considered the lowest
 * priority and 1.0 is considered the highest.
 *
 * The priority is a hint and not a hard requirement of task performance. The
 * priority of a task may be changed using this API at any time, but not all
 * protocols support this; in these cases, the last priority that took effect
 * will be used.
 *
 * If no priority is specified, the task will operate with the default priority
 * as defined by the constant NSURLSessionTaskPriorityDefault. Two additional
 * priority levels are provided: NSURLSessionTaskPriorityLow and
 * NSURLSessionTaskPriorityHigh, but use is not restricted to these.
 */
@property float priority API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* Provides a hint indicating if incremental delivery of a partial response body
 * would be useful for the application, or if it cannot process the response
 * until it is complete. Indicating that incremental delivery is not desired may
 * improve task performance. For example, if a response cannot be decoded until
 * the entire content is received, set this property to false.
 *
 * Defaults to true unless this task is created with completion-handler based
 * convenience methods, or if it is a download task.
 */
@property BOOL prefersIncrementalDelivery API_AVAILABLE(macos(11.3), ios(14.5), watchos(7.4), tvos(14.5));


- (instancetype)init API_DEPRECATED("Not supported", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Not supported", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

FOUNDATION_EXPORT const float NSURLSessionTaskPriorityDefault API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT const float NSURLSessionTaskPriorityLow API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT const float NSURLSessionTaskPriorityHigh API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/*
 * An NSURLSessionDataTask does not provide any additional
 * functionality over an NSURLSessionTask and its presence is merely
 * to provide lexical differentiation from download and upload tasks.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionDataTask : NSURLSessionTask

- (instancetype)init API_DEPRECATED("Please use -[NSURLSession dataTaskWithRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use -[NSURLSession dataTaskWithRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

/*
 * An NSURLSessionUploadTask does not currently provide any additional
 * functionality over an NSURLSessionDataTask.  All delegate messages
 * that may be sent referencing an NSURLSessionDataTask equally apply
 * to NSURLSessionUploadTasks.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionUploadTask : NSURLSessionDataTask

- (instancetype)init API_DEPRECATED("Please use -[NSURLSession uploadTaskWithStreamedRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use -[NSURLSession uploadTaskWithStreamedRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

/*
 * NSURLSessionDownloadTask is a task that represents a download to
 * local storage.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionDownloadTask : NSURLSessionTask

/* Cancel the download (and calls the superclass -cancel).  If
 * conditions will allow for resuming the download in the future, the
 * callback will be called with an opaque data blob, which may be used
 * with -downloadTaskWithResumeData: to attempt to resume the download.
 * If resume data cannot be created, the completion handler will be
 * called with nil resumeData.
 */
- (void)cancelByProducingResumeData:(void (^)(NSData * _Nullable resumeData))completionHandler;


- (instancetype)init API_DEPRECATED("Please use -[NSURLSession downloadTaskWithRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use -[NSURLSession downloadTaskWithRequest:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

/*
 * An NSURLSessionStreamTask provides an interface to perform reads
 * and writes to a TCP/IP stream created via NSURLSession.  This task
 * may be explicitly created from an NSURLSession, or created as a
 * result of the appropriate disposition response to a
 * -URLSession:dataTask:didReceiveResponse: delegate message.
 * 
 * NSURLSessionStreamTask can be used to perform asynchronous reads
 * and writes.  Reads and writes are enquened and executed serially,
 * with the completion handler being invoked on the sessions delegate
 * queuee.  If an error occurs, or the task is canceled, all
 * outstanding read and write calls will have their completion
 * handlers invoked with an appropriate error.
 *
 * It is also possible to create NSInputStream and NSOutputStream
 * instances from an NSURLSessionTask by sending
 * -captureStreams to the task.  All outstanding read and writess are
 * completed before the streams are created.  Once the streams are
 * delivered to the session delegate, the task is considered complete
 * and will receive no more messsages.  These streams are
 * disassociated from the underlying session.
 */

API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionStreamTask : NSURLSessionTask

/* Read minBytes, or at most maxBytes bytes and invoke the completion
 * handler on the sessions delegate queue with the data or an error.
 * If an error occurs, any outstanding reads will also fail, and new
 * read requests will error out immediately.
 */
- (void)readDataOfMinLength:(NSUInteger)minBytes maxLength:(NSUInteger)maxBytes timeout:(NSTimeInterval)timeout completionHandler:(void (^) (NSData * _Nullable_result data, BOOL atEOF, NSError * _Nullable error))completionHandler;

/* Write the data completely to the underlying socket.  If all the
 * bytes have not been written by the timeout, a timeout error will
 * occur.  Note that invocation of the completion handler does not
 * guarantee that the remote side has received all the bytes, only
 * that they have been written to the kernel. */
- (void)writeData:(NSData *)data timeout:(NSTimeInterval)timeout completionHandler:(void (^) (NSError * _Nullable error))completionHandler;

/* -captureStreams completes any already enqueued reads
 * and writes, and then invokes the
 * URLSession:streamTask:didBecomeInputStream:outputStream: delegate
 * message. When that message is received, the task object is
 * considered completed and will not receive any more delegate
 * messages. */
- (void)captureStreams;

/* Enqueue a request to close the write end of the underlying socket.
 * All outstanding IO will complete before the write side of the
 * socket is closed.  The server, however, may continue to write bytes
 * back to the client, so best practice is to continue reading from
 * the server until you receive EOF.
 */
- (void)closeWrite;

/* Enqueue a request to close the read side of the underlying socket.
 * All outstanding IO will complete before the read side is closed.
 * You may continue writing to the server.
 */
- (void)closeRead;

/*
 * Begin encrypted handshake.  The hanshake begins after all pending 
 * IO has completed.  TLS authentication callbacks are sent to the 
 * session's -URLSession:task:didReceiveChallenge:completionHandler:
 */
- (void)startSecureConnection;

/*
 * Cleanly close a secure connection after all pending secure IO has 
 * completed.
 *
 * @warning This API is non-functional.
 */
- (void)stopSecureConnection API_DEPRECATED("TLS cannot be disabled once it is enabled", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));


- (instancetype)init API_DEPRECATED("Please use -[NSURLSession streamTaskWithHostName:port:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use -[NSURLSession streamTaskWithHostName:port:] or other NSURLSession methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

typedef NS_ENUM(NSInteger, NSURLSessionWebSocketMessageType) {
    NSURLSessionWebSocketMessageTypeData = 0,
    NSURLSessionWebSocketMessageTypeString = 1,
} API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* The client can create a WebSocket message object that will be passed to the send calls
 * and will be delivered from the receive calls. The message can be initialized with data or string.
 * If initialized with data, the string property will be nil and vice versa.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSURLSessionWebSocketMessage : NSObject

/* Create a message with data type
 */
- (instancetype)initWithData:(NSData *)data NS_DESIGNATED_INITIALIZER;

/* Create a message with string type
 */
- (instancetype)initWithString:(NSString *)string NS_DESIGNATED_INITIALIZER;

@property (readonly) NSURLSessionWebSocketMessageType type;
@property (nullable, readonly, copy) NSData *data;
@property (nullable, readonly, copy) NSString *string;


- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

/* The WebSocket close codes follow the close codes given in the RFC
 */
typedef NS_ENUM(NSInteger, NSURLSessionWebSocketCloseCode)
{
    NSURLSessionWebSocketCloseCodeInvalid =                             0,
    NSURLSessionWebSocketCloseCodeNormalClosure =                    1000,
    NSURLSessionWebSocketCloseCodeGoingAway =                        1001,
    NSURLSessionWebSocketCloseCodeProtocolError =                    1002,
    NSURLSessionWebSocketCloseCodeUnsupportedData =                  1003,
    NSURLSessionWebSocketCloseCodeNoStatusReceived =                 1005,
    NSURLSessionWebSocketCloseCodeAbnormalClosure =                  1006,
    NSURLSessionWebSocketCloseCodeInvalidFramePayloadData =          1007,
    NSURLSessionWebSocketCloseCodePolicyViolation =                  1008,
    NSURLSessionWebSocketCloseCodeMessageTooBig =                    1009,
    NSURLSessionWebSocketCloseCodeMandatoryExtensionMissing =        1010,
    NSURLSessionWebSocketCloseCodeInternalServerError =              1011,
    NSURLSessionWebSocketCloseCodeTLSHandshakeFailure =              1015,
} API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * A WebSocket task can be created with a ws or wss url. A client can also provide
 * a list of protocols it wishes to advertise during the WebSocket handshake phase.
 * Once the handshake is successfully completed the client will be notified through an optional delegate.
 * All reads and writes enqueued before the completion of the handshake will be queued up and
 * executed once the hanshake succeeds. Before the handshake completes, the client can be called to handle
 * redirection or authentication using the same delegates as NSURLSessionTask. WebSocket task will also provide
 * support for cookies and will store cookies to the cookie storage on the session and will attach cookies to
 * outgoing HTTP handshake requests.
 */
API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@interface NSURLSessionWebSocketTask : NSURLSessionTask

/* Sends a WebSocket message. If an error occurs, any outstanding work will also fail.
 * Note that invocation of the completion handler does not
 * guarantee that the remote side has received all the bytes, only
 * that they have been written to the kernel.
 */
- (void)sendMessage:(NSURLSessionWebSocketMessage *)message completionHandler:(void (^)(NSError * _Nullable error))completionHandler;

/* Reads a WebSocket message once all the frames of the message are available.
 * If the maximumMessage size is hit while buffering the frames, the receiveMessage call will error out
 * and all outstanding work will also fail resulting in the end of the task.
 */
- (void)receiveMessageWithCompletionHandler:(void (^)(NSURLSessionWebSocketMessage * _Nullable message, NSError * _Nullable error))completionHandler;

/* Sends a ping frame from the client side. The pongReceiveHandler is invoked when the client
 * receives a pong from the server endpoint. If a connection is lost or an error occurs before receiving
 * the pong from the endpoint, the pongReceiveHandler block will be invoked with an error.
 * Note - the pongReceiveHandler will always be called in the order in which the pings were sent.
 */
- (void)sendPingWithPongReceiveHandler:(void (^)(NSError * _Nullable error))pongReceiveHandler;

/* Sends a close frame with the given closeCode. An optional reason can be provided while sending the close frame.
 * Simply calling cancel on the task will result in a cancellation frame being sent without any reason.
 */
- (void)cancelWithCloseCode:(NSURLSessionWebSocketCloseCode)closeCode reason:(nullable NSData *)reason;

@property NSInteger maximumMessageSize; /* The maximum number of bytes to be buffered before erroring out. This includes the sum of all bytes from continuation frames. Recieve calls will error out if this value is reached */
@property (readonly) NSURLSessionWebSocketCloseCode closeCode; /* A task can be queried for it's close code at any point. When the task is not closed, it will be set to NSURLSessionWebSocketCloseCodeInvalid */
@property (nullable, readonly, copy) NSData *closeReason; /* A task can be queried for it's close reason at any point. A nil value indicates no closeReason or that the task is still running */


- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

@end

/*!
 @enum NSURLSessionMultipathServiceType
 
 @discussion The NSURLSessionMultipathServiceType enum defines constants that
 can be used to specify the multipath service type to associate an NSURLSession.  The
 multipath service type determines whether multipath TCP should be attempted and the conditions
 for creating and switching between subflows.  Using these service types requires the appropriate entitlement.  Any connection attempt will fail if the process does not have the required entitlement.
 A primary interface is a generally less expensive interface in terms of both cost and power (such as WiFi or ethernet).  A secondary interface is more expensive (such as 3G or LTE).
 
 @constant NSURLSessionMultipathServiceTypeNone Specifies that multipath tcp should not be used.  Connections will use a single flow.
 This is the default value.  No entitlement is required to set this value.
 
 @constant NSURLSessionMultipathServiceTypeHandover Specifies that a secondary subflow should only be used
 when the primary subflow is not performing adequately.   Requires the com.apple.developer.networking.multipath entilement.
 
 @constant NSURLSessionMultipathServiceTypeInteractive Specifies that a secodary subflow should be used if the
 primary subflow is not performing adequately (packet loss, high round trip times, bandwidth issues).  The secondary
 subflow will be created more aggressively than with NSURLSessionMultipathServiceTypeHandover.  Requires the com.apple.developer.networking.multipath entitlement.
 
 @constant NSURLSessionMultipathServiceTypeAggregate Specifies that multiple subflows across multiple interfaces should be
 used for better bandwidth.  This mode is only available for experimentation on devices configured for development use.
 It can be enabled in the Developer section of the Settings app.
 
 */
typedef NS_ENUM(NSInteger, NSURLSessionMultipathServiceType)
{
    NSURLSessionMultipathServiceTypeNone = 0,      	/* None - no multipath (default) */
    NSURLSessionMultipathServiceTypeHandover = 1,   	/* Handover - secondary flows brought up when primary flow is not performing adequately. */
    NSURLSessionMultipathServiceTypeInteractive = 2, /* Interactive - secondary flows created more aggressively. */
    NSURLSessionMultipathServiceTypeAggregate = 3    /* Aggregate - multiple subflows used for greater bandwitdh. */
} API_AVAILABLE(ios(11.0)) API_UNAVAILABLE(macos, watchos, tvos) NS_SWIFT_NAME(URLSessionConfiguration.MultipathServiceType);

/*
 * Configuration options for an NSURLSession.  When a session is
 * created, a copy of the configuration object is made - you cannot
 * modify the configuration of a session after it has been created.
 *
 * The shared session uses the global singleton credential, cache
 * and cookie storage objects.
 *
 * An ephemeral session has no persistent disk storage for cookies,
 * cache or credentials.
 *
 * A background session can be used to perform networking operations
 * on behalf of a suspended application, within certain constraints.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@interface NSURLSessionConfiguration : NSObject <NSCopying>

@property (class, readonly, strong) NSURLSessionConfiguration *defaultSessionConfiguration;
@property (class, readonly, strong) NSURLSessionConfiguration *ephemeralSessionConfiguration;

+ (NSURLSessionConfiguration *)backgroundSessionConfigurationWithIdentifier:(NSString *)identifier API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* identifier for the background session configuration */
@property (nullable, readonly, copy) NSString *identifier;

/* default cache policy for requests */
@property NSURLRequestCachePolicy requestCachePolicy;

/* default timeout for requests.  This will cause a timeout if no data is transmitted for the given timeout value, and is reset whenever data is transmitted. */
@property NSTimeInterval timeoutIntervalForRequest;

/* default timeout for requests.  This will cause a timeout if a resource is not able to be retrieved within a given timeout. */
@property NSTimeInterval timeoutIntervalForResource;

/* type of service for requests. */
@property NSURLRequestNetworkServiceType networkServiceType;

/* allow request to route over cellular. */
@property BOOL allowsCellularAccess;

/* allow request to route over expensive networks.  Defaults to YES. */
@property BOOL allowsExpensiveNetworkAccess API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* allow request to route over networks in constrained mode. Defaults to YES. */
@property BOOL allowsConstrainedNetworkAccess API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * Causes tasks to wait for network connectivity to become available, rather
 * than immediately failing with an error (such as NSURLErrorNotConnectedToInternet)
 * when it is not. When waiting for connectivity, the timeoutIntervalForRequest
 * property does not apply, but the timeoutIntervalForResource property does.
 *
 * Unsatisfactory connectivity (that requires waiting) includes cases where the
 * device has limited or insufficient connectivity for a task (e.g., only has a
 * cellular connection but the allowsCellularAccess property is NO, or requires
 * a VPN connection in order to reach the desired host).
 *
 * Default value is NO. Ignored by background sessions, as background sessions
 * always wait for connectivity.
 */
@property BOOL waitsForConnectivity API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/* allows background tasks to be scheduled at the discretion of the system for optimal performance. */
@property (getter=isDiscretionary) BOOL discretionary API_AVAILABLE(macos(10.10), ios(7.0), watchos(2.0), tvos(9.0));

/* The identifier of the shared data container into which files in background sessions should be downloaded.
 * App extensions wishing to use background sessions *must* set this property to a valid container identifier, or
 * all transfers in that session will fail with NSURLErrorBackgroundSessionRequiresSharedContainer.
 */
@property (nullable, copy) NSString *sharedContainerIdentifier API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));

/* 
 * Allows the app to be resumed or launched in the background when tasks in background sessions complete
 * or when auth is required. This only applies to configurations created with +backgroundSessionConfigurationWithIdentifier:
 * and the default value is YES.
 *
 * NOTE: macOS apps based on AppKit do not support background launch.
 */
@property BOOL sessionSendsLaunchEvents API_AVAILABLE(macos(11.0), ios(7.0), watchos(2.0), tvos(9.0));

/* The proxy dictionary, as described by <CFNetwork/CFHTTPStream.h> */
@property (nullable, copy) NSDictionary *connectionProxyDictionary;

/* The minimum allowable versions of the TLS protocol, from <Security/SecureTransport.h> */
@property SSLProtocol TLSMinimumSupportedProtocol API_DEPRECATED_WITH_REPLACEMENT("TLSMinimumSupportedProtocolVersion", macos(10.9, API_TO_BE_DEPRECATED), ios(7.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* The maximum allowable versions of the TLS protocol, from <Security/SecureTransport.h> */
@property SSLProtocol TLSMaximumSupportedProtocol API_DEPRECATED_WITH_REPLACEMENT("TLSMaximumSupportedProtocolVersion", macos(10.9, API_TO_BE_DEPRECATED), ios(7.0, API_TO_BE_DEPRECATED), watchos(2.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED));

/* The minimum allowable versions of the TLS protocol, from <Security/SecProtocolTypes.h> */
@property tls_protocol_version_t TLSMinimumSupportedProtocolVersion API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* The maximum allowable versions of the TLS protocol, from <Security/SecProtocolTypes.h> */
@property tls_protocol_version_t TLSMaximumSupportedProtocolVersion API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/* Allow the use of HTTP pipelining */
@property BOOL HTTPShouldUsePipelining;

/* Allow the session to set cookies on requests */
@property BOOL HTTPShouldSetCookies;

/* Policy for accepting cookies.  This overrides the policy otherwise specified by the cookie storage. */
@property NSHTTPCookieAcceptPolicy HTTPCookieAcceptPolicy;

/* Specifies additional headers which will be set on outgoing requests.
   Note that these headers are added to the request only if not already present. */
@property (nullable, copy) NSDictionary *HTTPAdditionalHeaders;

/* The maximum number of simultanous persistent connections per host */
@property NSInteger HTTPMaximumConnectionsPerHost;

/* The cookie storage object to use, or nil to indicate that no cookies should be handled */
@property (nullable, retain) NSHTTPCookieStorage *HTTPCookieStorage;

/* The credential storage object, or nil to indicate that no credential storage is to be used */
@property (nullable, retain) NSURLCredentialStorage *URLCredentialStorage;

/* The URL resource cache, or nil to indicate that no caching is to be performed */
@property (nullable, retain) NSURLCache *URLCache;

/* Enable extended background idle mode for any tcp sockets created.    Enabling this mode asks the system to keep the socket open
 *  and delay reclaiming it when the process moves to the background (see https://developer.apple.com/library/ios/technotes/tn2277/_index.html) 
 */
@property BOOL shouldUseExtendedBackgroundIdleMode API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/* An optional array of Class objects which subclass NSURLProtocol.
   The Class will be sent +canInitWithRequest: when determining if
   an instance of the class can be used for a given URL scheme.
   You should not use +[NSURLProtocol registerClass:], as that
   method will register your class with the default session rather
   than with an instance of NSURLSession. 
   Custom NSURLProtocol subclasses are not available to background
   sessions.
 */
@property (nullable, copy) NSArray<Class> *protocolClasses;

/* multipath service type to use for connections.  The default is NSURLSessionMultipathServiceTypeNone */
@property NSURLSessionMultipathServiceType multipathServiceType API_AVAILABLE(ios(11.0)) API_UNAVAILABLE(macos, watchos, tvos);


- (instancetype)init API_DEPRECATED("Please use NSURLSessionConfiguration.defaultSessionConfiguration or other class methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));
+ (instancetype)new API_DEPRECATED("Please use NSURLSessionConfiguration.defaultSessionConfiguration or other class methods to create instances", macos(10.9,10.15), ios(7.0,13.0), watchos(2.0,6.0), tvos(9.0,13.0));

@end

/*
 * Disposition options for various delegate messages
 */
typedef NS_ENUM(NSInteger, NSURLSessionDelayedRequestDisposition) {
    NSURLSessionDelayedRequestContinueLoading = 0,  /* Use the original request provided when the task was created; the request parameter is ignored. */
    NSURLSessionDelayedRequestUseNewRequest = 1,    /* Use the specified request, which may not be nil. */
    NSURLSessionDelayedRequestCancel = 2,           /* Cancel the task; the request parameter is ignored. */
} NS_SWIFT_NAME(URLSession.DelayedRequestDisposition) API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

typedef NS_ENUM(NSInteger, NSURLSessionAuthChallengeDisposition) {
    NSURLSessionAuthChallengeUseCredential = 0,                                       /* Use the specified credential, which may be nil */
    NSURLSessionAuthChallengePerformDefaultHandling = 1,                              /* Default handling for the challenge - as if this delegate were not implemented; the credential parameter is ignored. */
    NSURLSessionAuthChallengeCancelAuthenticationChallenge = 2,                       /* The entire request will be canceled; the credential parameter is ignored. */
    NSURLSessionAuthChallengeRejectProtectionSpace = 3,                               /* This challenge is rejected and the next authentication protection space should be tried; the credential parameter is ignored. */
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));


typedef NS_ENUM(NSInteger, NSURLSessionResponseDisposition) {
    NSURLSessionResponseCancel = 0,                                      /* Cancel the load, this is the same as -[task cancel] */
    NSURLSessionResponseAllow = 1,                                       /* Allow the load to continue */
    NSURLSessionResponseBecomeDownload = 2,                              /* Turn this request into a download */
    NSURLSessionResponseBecomeStream API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0)) = 3,  /* Turn this task into a stream task */
} API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

/*
 * NSURLSessionDelegate specifies the methods that a session delegate
 * may respond to.  There are both session specific messages (for
 * example, connection based auth) as well as task based messages.
 */

/*
 * Messages related to the URL session as a whole
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@protocol NSURLSessionDelegate <NSObject>
@optional

/* The last message a session receives.  A session will only become
 * invalid because of a systemic error or when it has been
 * explicitly invalidated, in which case the error parameter will be nil.
 */
- (void)URLSession:(NSURLSession *)session didBecomeInvalidWithError:(nullable NSError *)error;

/* If implemented, when a connection level authentication challenge
 * has occurred, this delegate will be given the opportunity to
 * provide authentication credentials to the underlying
 * connection. Some types of authentication will apply to more than
 * one request on a given connection to a server (SSL Server Trust
 * challenges).  If this delegate message is not implemented, the 
 * behavior will be to use the default handling, which may involve user
 * interaction. 
 */
- (void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge
                                             completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;

/* If an application has received an
 * -application:handleEventsForBackgroundURLSession:completionHandler:
 * message, the session delegate will receive this message to indicate
 * that all messages previously enqueued for this session have been
 * delivered.  At this time it is safe to invoke the previously stored
 * completion handler, or to begin any internal updates that will
 * result in invoking the completion handler.
 */
- (void)URLSessionDidFinishEventsForBackgroundURLSession:(NSURLSession *)session API_AVAILABLE(macos(11.0), ios(7.0), watchos(2.0), tvos(9.0));

@end

/*
 * Messages related to the operation of a specific task.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@protocol NSURLSessionTaskDelegate <NSURLSessionDelegate>
@optional

/*
 * Sent when the system is ready to begin work for a task with a delayed start
 * time set (using the earliestBeginDate property). The completionHandler must
 * be invoked in order for loading to proceed. The disposition provided to the
 * completion handler continues the load with the original request provided to
 * the task, replaces the request with the specified task, or cancels the task.
 * If this delegate is not implemented, loading will proceed with the original
 * request.
 *
 * Recommendation: only implement this delegate if tasks that have the
 * earliestBeginDate property set may become stale and require alteration prior
 * to starting the network load.
 *
 * If a new request is specified, the allowsExpensiveNetworkAccess,
 * allowsContrainedNetworkAccess, and allowsCellularAccess properties
 * from the new request will not be used; the properties from the
 * original request will continue to be used.
 *
 * Canceling the task is equivalent to calling the task's cancel method; the
 * URLSession:task:didCompleteWithError: task delegate will be called with error
 * NSURLErrorCancelled.
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                        willBeginDelayedRequest:(NSURLRequest *)request
                              completionHandler:(void (^)(NSURLSessionDelayedRequestDisposition disposition, NSURLRequest * _Nullable newRequest))completionHandler
    API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/*
 * Sent when a task cannot start the network loading process because the current
 * network connectivity is not available or sufficient for the task's request.
 *
 * This delegate will be called at most one time per task, and is only called if
 * the waitsForConnectivity property in the NSURLSessionConfiguration has been
 * set to YES.
 *
 * This delegate callback will never be called for background sessions, because
 * the waitForConnectivity property is ignored by those sessions.
 */
- (void)URLSession:(NSURLSession *)session taskIsWaitingForConnectivity:(NSURLSessionTask *)task
    API_AVAILABLE(macos(10.13), ios(11.0), watchos(4.0), tvos(11.0));

/* An HTTP request is attempting to perform a redirection to a different
 * URL. You must invoke the completion routine to allow the
 * redirection, allow the redirection with a modified request, or
 * pass nil to the completionHandler to cause the body of the redirection 
 * response to be delivered as the payload of this request. The default
 * is to follow redirections. 
 *
 * For tasks in background sessions, redirections will always be followed and this method will not be called.
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                     willPerformHTTPRedirection:(NSHTTPURLResponse *)response
                                     newRequest:(NSURLRequest *)request
                              completionHandler:(void (^)(NSURLRequest * _Nullable))completionHandler;

/* The task has received a request specific authentication challenge.
 * If this delegate is not implemented, the session specific authentication challenge
 * will *NOT* be called and the behavior will be the same as using the default handling
 * disposition. 
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                            didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge 
                              completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * _Nullable credential))completionHandler;

/* Sent if a task requires a new, unopened body stream.  This may be
 * necessary when authentication has failed for any request that
 * involves a body stream. 
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                              needNewBodyStream:(void (^)(NSInputStream * _Nullable bodyStream))completionHandler NS_SWIFT_ASYNC_NAME(urlSession(_:needNewBodyStreamForTask:));

/* Sent periodically to notify the delegate of upload progress.  This
 * information is also available as properties of the task.
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                                didSendBodyData:(int64_t)bytesSent
                                 totalBytesSent:(int64_t)totalBytesSent
                       totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend;

/*
 * Sent when complete statistics information has been collected for the task.
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didFinishCollectingMetrics:(NSURLSessionTaskMetrics *)metrics API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/* Sent as the last message related to a specific task.  Error may be
 * nil, which implies that no error occurred and this task is complete. 
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
                           didCompleteWithError:(nullable NSError *)error;

@end

/*
 * Messages related to the operation of a task that delivers data
 * directly to the delegate.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@protocol NSURLSessionDataDelegate <NSURLSessionTaskDelegate>
@optional
/* The task has received a response and no further messages will be
 * received until the completion block is called. The disposition
 * allows you to cancel a request or to turn a data task into a
 * download task. This delegate message is optional - if you do not
 * implement it, you can get the response as a property of the task.
 *
 * This method will not be called for background upload tasks (which cannot be converted to download tasks).
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                 didReceiveResponse:(NSURLResponse *)response
                                  completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler;

/* Notification that a data task has become a download task.  No
 * future messages will be sent to the data task.
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                              didBecomeDownloadTask:(NSURLSessionDownloadTask *)downloadTask;

/*
 * Notification that a data task has become a bidirectional stream
 * task.  No future messages will be sent to the data task.  The newly
 * created streamTask will carry the original request and response as
 * properties.
 *
 * For requests that were pipelined, the stream object will only allow
 * reading, and the object will immediately issue a
 * -URLSession:writeClosedForStream:.  Pipelining can be disabled for
 * all requests in a session, or by the NSURLRequest
 * HTTPShouldUsePipelining property.
 *
 * The underlying connection is no longer considered part of the HTTP
 * connection cache and won't count against the total number of
 * connections per host.
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                didBecomeStreamTask:(NSURLSessionStreamTask *)streamTask
    API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0));

/* Sent when data is available for the delegate to consume.  It is
 * assumed that the delegate will retain and not copy the data.  As
 * the data may be discontiguous, you should use 
 * [NSData enumerateByteRangesUsingBlock:] to access it.
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                     didReceiveData:(NSData *)data;

/* Invoke the completion routine with a valid NSCachedURLResponse to
 * allow the resulting data to be cached, or pass nil to prevent
 * caching. Note that there is no guarantee that caching will be
 * attempted for a given resource, and you should not rely on this
 * message to receive the resource data.
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
                                  willCacheResponse:(NSCachedURLResponse *)proposedResponse 
                                  completionHandler:(void (^)(NSCachedURLResponse * _Nullable cachedResponse))completionHandler;

@end

/*
 * Messages related to the operation of a task that writes data to a
 * file and notifies the delegate upon completion.
 */
API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0))
@protocol NSURLSessionDownloadDelegate <NSURLSessionTaskDelegate>

/* Sent when a download task that has completed a download.  The delegate should 
 * copy or move the file at the given location to a new location as it will be 
 * removed when the delegate message returns. URLSession:task:didCompleteWithError: will
 * still be called.
 */
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                              didFinishDownloadingToURL:(NSURL *)location;

@optional
/* Sent periodically to notify the delegate of download progress. */
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                                           didWriteData:(int64_t)bytesWritten
                                      totalBytesWritten:(int64_t)totalBytesWritten
                              totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite;

/* Sent when a download has been resumed. If a download failed with an
 * error, the -userInfo dictionary of the error will contain an
 * NSURLSessionDownloadTaskResumeData key, whose value is the resume
 * data. 
 */
- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
                                      didResumeAtOffset:(int64_t)fileOffset
                                     expectedTotalBytes:(int64_t)expectedTotalBytes;

@end

API_AVAILABLE(macos(10.11), ios(9.0), watchos(2.0), tvos(9.0))
@protocol NSURLSessionStreamDelegate <NSURLSessionTaskDelegate>
@optional

/* Indiciates that the read side of a connection has been closed.  Any
 * outstanding reads complete, but future reads will immediately fail.
 * This may be sent even when no reads are in progress. However, when
 * this delegate message is received, there may still be bytes
 * available.  You only know that no more bytes are available when you
 * are able to read until EOF. */
- (void)URLSession:(NSURLSession *)session readClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;

/* Indiciates that the write side of a connection has been closed.
 * Any outstanding writes complete, but future writes will immediately
 * fail.
 */
- (void)URLSession:(NSURLSession *)session writeClosedForStreamTask:(NSURLSessionStreamTask *)streamTask;

/* A notification that the system has determined that a better route
 * to the host has been detected (eg, a wi-fi interface becoming
 * available.)  This is a hint to the delegate that it may be
 * desirable to create a new task for subsequent work.  Note that
 * there is no guarantee that the future task will be able to connect
 * to the host, so callers should should be prepared for failure of
 * reads and writes over any new interface. */
- (void)URLSession:(NSURLSession *)session betterRouteDiscoveredForStreamTask:(NSURLSessionStreamTask *)streamTask;

/* The given task has been completed, and unopened NSInputStream and
 * NSOutputStream objects are created from the underlying network
 * connection.  This will only be invoked after all enqueued IO has
 * completed (including any necessary handshakes.)  The streamTask
 * will not receive any further delegate messages.
 */
- (void)URLSession:(NSURLSession *)session streamTask:(NSURLSessionStreamTask *)streamTask
                                 didBecomeInputStream:(NSInputStream *)inputStream
                                         outputStream:(NSOutputStream *)outputStream;

@end

API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0))
@protocol NSURLSessionWebSocketDelegate <NSURLSessionTaskDelegate>
@optional

/* Indicates that the WebSocket handshake was successful and the connection has been upgraded to webSockets.
 * It will also provide the protocol that is picked in the handshake. If the handshake fails, this delegate will not be invoked.
 */
- (void)URLSession:(NSURLSession *)session webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask didOpenWithProtocol:(nullable NSString *) protocol;

/* Indicates that the WebSocket has received a close frame from the server endpoint.
 * The close code and the close reason may be provided by the delegate if the server elects to send
 * this information in the close frame
 */
- (void)URLSession:(NSURLSession *)session webSocketTask:(NSURLSessionWebSocketTask *)webSocketTask didCloseWithCode:(NSURLSessionWebSocketCloseCode)closeCode reason:(nullable NSData *)reason;

@end

/* Key in the userInfo dictionary of an NSError received during a failed download. */
FOUNDATION_EXPORT NSString * const NSURLSessionDownloadTaskResumeData API_AVAILABLE(macos(10.9), ios(7.0), watchos(2.0), tvos(9.0));

@interface NSURLSessionConfiguration (NSURLSessionDeprecated)
+ (NSURLSessionConfiguration *)backgroundSessionConfiguration:(NSString *)identifier API_DEPRECATED_WITH_REPLACEMENT("-backgroundSessionConfigurationWithIdentifier:", macos(10.9, 10.10), ios(7.0, 8.0), watchos(2.0, 2.0), tvos(9.0, 9.0));
@end

/*
 * The resource fetch type.
 */
typedef NS_ENUM(NSInteger, NSURLSessionTaskMetricsResourceFetchType) {
    NSURLSessionTaskMetricsResourceFetchTypeUnknown,
    NSURLSessionTaskMetricsResourceFetchTypeNetworkLoad,   /* The resource was loaded over the network. */
    NSURLSessionTaskMetricsResourceFetchTypeServerPush,    /* The resource was pushed by the server to the client. */
    NSURLSessionTaskMetricsResourceFetchTypeLocalCache,    /* The resource was retrieved from the local storage. */
} API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

/*
 * DNS protocol used for domain resolution.
 */
typedef NS_ENUM(NSInteger, NSURLSessionTaskMetricsDomainResolutionProtocol) {
    NSURLSessionTaskMetricsDomainResolutionProtocolUnknown,
    NSURLSessionTaskMetricsDomainResolutionProtocolUDP,     /* Resolution used DNS over UDP. */
    NSURLSessionTaskMetricsDomainResolutionProtocolTCP,     /* Resolution used DNS over TCP. */
    NSURLSessionTaskMetricsDomainResolutionProtocolTLS,     /* Resolution used DNS over TLS. */
    NSURLSessionTaskMetricsDomainResolutionProtocolHTTPS,   /* Resolution used DNS over HTTPS. */
} API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));

/*
 * This class defines the performance metrics collected for a request/response transaction during the task execution.
 */
API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSURLSessionTaskTransactionMetrics : NSObject

/*
 * Represents the transaction request.
 */
@property (copy, readonly) NSURLRequest *request;

/*
 * Represents the transaction response. Can be nil if error occurred and no response was generated.
 */
@property (nullable, copy, readonly) NSURLResponse *response;

/*
 * For all NSDate metrics below, if that aspect of the task could not be completed, then the corresponding “EndDate” metric will be nil.
 * For example, if a name lookup was started but the name lookup timed out, failed, or the client canceled the task before the name could be resolved -- then while domainLookupStartDate may be set, domainLookupEndDate will be nil along with all later metrics.
 */

/*
 * fetchStartDate returns the time when the user agent started fetching the resource, whether or not the resource was retrieved from the server or local resources.
 *
 * The following metrics will be set to nil, if a persistent connection was used or the resource was retrieved from local resources:
 *
 *   domainLookupStartDate
 *   domainLookupEndDate
 *   connectStartDate
 *   connectEndDate
 *   secureConnectionStartDate
 *   secureConnectionEndDate
 */
@property (nullable, copy, readonly) NSDate *fetchStartDate;

/*
 * domainLookupStartDate returns the time immediately before the user agent started the name lookup for the resource.
 */
@property (nullable, copy, readonly) NSDate *domainLookupStartDate;

/*
 * domainLookupEndDate returns the time after the name lookup was completed.
 */
@property (nullable, copy, readonly) NSDate *domainLookupEndDate;

/*
 * connectStartDate is the time immediately before the user agent started establishing the connection to the server.
 *
 * For example, this would correspond to the time immediately before the user agent started trying to establish the TCP connection.
 */
@property (nullable, copy, readonly) NSDate *connectStartDate;

/*
 * If an encrypted connection was used, secureConnectionStartDate is the time immediately before the user agent started the security handshake to secure the current connection.
 *
 * For example, this would correspond to the time immediately before the user agent started the TLS handshake.
 *
 * If an encrypted connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSDate *secureConnectionStartDate;

/*
 * If an encrypted connection was used, secureConnectionEndDate is the time immediately after the security handshake completed.
 *
 * If an encrypted connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSDate *secureConnectionEndDate;

/*
 * connectEndDate is the time immediately after the user agent finished establishing the connection to the server, including completion of security-related and other handshakes.
 */
@property (nullable, copy, readonly) NSDate *connectEndDate;

/*
 * requestStartDate is the time immediately before the user agent started requesting the source, regardless of whether the resource was retrieved from the server or local resources.
 *
 * For example, this would correspond to the time immediately before the user agent sent an HTTP GET request.
 */
@property (nullable, copy, readonly) NSDate *requestStartDate;

/*
 * requestEndDate is the time immediately after the user agent finished requesting the source, regardless of whether the resource was retrieved from the server or local resources.
 *
 * For example, this would correspond to the time immediately after the user agent finished sending the last byte of the request.
 */
@property (nullable, copy, readonly) NSDate *requestEndDate;

/*
 * responseStartDate is the time immediately after the user agent received the first byte of the response from the server or from local resources.
 *
 * For example, this would correspond to the time immediately after the user agent received the first byte of an HTTP response.
 */
@property (nullable, copy, readonly) NSDate *responseStartDate;

/*
 * responseEndDate is the time immediately after the user agent received the last byte of the resource.
 */
@property (nullable, copy, readonly) NSDate *responseEndDate;

/*
 * The network protocol used to fetch the resource, as identified by the ALPN Protocol ID Identification Sequence [RFC7301].
 * E.g., h2, http/1.1, spdy/3.1.
 *
 * When a proxy is configured AND a tunnel connection is established, then this attribute returns the value for the tunneled protocol.
 *
 * For example:
 * If no proxy were used, and HTTP/2 was negotiated, then h2 would be returned.
 * If HTTP/1.1 were used to the proxy, and the tunneled connection was HTTP/2, then h2 would be returned.
 * If HTTP/1.1 were used to the proxy, and there were no tunnel, then http/1.1 would be returned.
 *
 */
@property (nullable, copy, readonly) NSString *networkProtocolName;

/*
 * This property is set to YES if a proxy connection was used to fetch the resource.
 */
@property (assign, readonly, getter=isProxyConnection) BOOL proxyConnection;

/*
 * This property is set to YES if a persistent connection was used to fetch the resource.
 */
@property (assign, readonly, getter=isReusedConnection) BOOL reusedConnection;

/*
 * Indicates whether the resource was loaded, pushed or retrieved from the local cache.
 */
@property (assign, readonly) NSURLSessionTaskMetricsResourceFetchType resourceFetchType;

/*
 * countOfRequestHeaderBytesSent is the number of bytes transferred for request header.
 */
@property (readonly) int64_t countOfRequestHeaderBytesSent API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * countOfRequestBodyBytesSent is the number of bytes transferred for request body.
 * It includes protocol-specific framing, transfer encoding, and content encoding.
 */
@property (readonly) int64_t countOfRequestBodyBytesSent API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * countOfRequestBodyBytesBeforeEncoding is the size of upload body data, file, or stream.
 */
@property (readonly) int64_t countOfRequestBodyBytesBeforeEncoding API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * countOfResponseHeaderBytesReceived is the number of bytes transferred for response header.
 */
@property (readonly) int64_t countOfResponseHeaderBytesReceived API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * countOfResponseBodyBytesReceived is the number of bytes transferred for response header.
 * It includes protocol-specific framing, transfer encoding, and content encoding.
 */
@property (readonly) int64_t countOfResponseBodyBytesReceived API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * countOfResponseBodyBytesAfterDecoding is the size of data delivered to your delegate or completion handler.
 */
@property (readonly) int64_t countOfResponseBodyBytesAfterDecoding API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * localAddress is the IP address string of the local interface for the connection.
 *
 * For multipath protocols, this is the local address of the initial flow.
 *
 * If a connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSString *localAddress API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * localPort is the port number of the local interface for the connection.
 *
 * For multipath protocols, this is the local port of the initial flow.
 *
 * If a connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSNumber *localPort API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * remoteAddress is the IP address string of the remote interface for the connection.
 *
 * For multipath protocols, this is the remote address of the initial flow.
 *
 * If a connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSString *remoteAddress API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * remotePort is the port number of the remote interface for the connection.
 *
 * For multipath protocols, this is the remote port of the initial flow.
 *
 * If a connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSNumber *remotePort API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * negotiatedTLSProtocolVersion is the TLS protocol version negotiated for the connection.
 * It is a 2-byte sequence in host byte order.
 *
 * Please refer to tls_protocol_version_t enum in Security/SecProtocolTypes.h
 *
 * If an encrypted connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSNumber *negotiatedTLSProtocolVersion API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * negotiatedTLSCipherSuite is the TLS cipher suite negotiated for the connection.
 * It is a 2-byte sequence in host byte order.
 *
 * Please refer to tls_ciphersuite_t enum in Security/SecProtocolTypes.h
 *
 * If an encrypted connection was not used, this attribute is set to nil.
 */
@property (nullable, copy, readonly) NSNumber *negotiatedTLSCipherSuite API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * Whether the connection is established over a cellular interface.
 */
@property (readonly, getter=isCellular) BOOL cellular API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * Whether the connection is established over an expensive interface.
 */
@property (readonly, getter=isExpensive) BOOL expensive API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * Whether the connection is established over a constrained interface.
 */
@property (readonly, getter=isConstrained) BOOL constrained API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * Whether a multipath protocol is successfully negotiated for the connection.
 */
@property (readonly, getter=isMultipath) BOOL multipath API_AVAILABLE(macos(10.15), ios(13.0), watchos(6.0), tvos(13.0));

/*
 * DNS protocol used for domain resolution.
 */
@property (readonly) NSURLSessionTaskMetricsDomainResolutionProtocol domainResolutionProtocol API_AVAILABLE(macos(11.0), ios(14.0), watchos(7.0), tvos(14.0));


- (instancetype)init API_DEPRECATED("Not supported", macos(10.12,10.15), ios(10.0,13.0), watchos(3.0,6.0), tvos(10.0,13.0));
+ (instancetype)new API_DEPRECATED("Not supported", macos(10.12,10.15), ios(10.0,13.0), watchos(3.0,6.0), tvos(10.0,13.0));

@end


API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0))
@interface NSURLSessionTaskMetrics : NSObject

/*
 * transactionMetrics array contains the metrics collected for every request/response transaction created during the task execution.
 */
@property (copy, readonly) NSArray<NSURLSessionTaskTransactionMetrics *> *transactionMetrics;

/*
 * Interval from the task creation time to the task completion time.
 * Task creation time is the time when the task was instantiated.
 * Task completion time is the time when the task is about to change its internal state to completed.
 */
@property (copy, readonly) NSDateInterval *taskInterval;

/*
 * redirectCount is the number of redirects that were recorded.
 */
@property (assign, readonly) NSUInteger redirectCount;


- (instancetype)init API_DEPRECATED("Not supported", macos(10.12,10.15), ios(10.0,13.0), watchos(3.0,6.0), tvos(10.0,13.0));
+ (instancetype)new API_DEPRECATED("Not supported", macos(10.12,10.15), ios(10.0,13.0), watchos(3.0,6.0), tvos(10.0,13.0));

@end

NS_ASSUME_NONNULL_END
