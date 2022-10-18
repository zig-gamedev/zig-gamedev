/*	NSConnection.h
	Copyright (c) 1989-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>

@class NSMutableData, NSDistantObject, NSException, NSData, NSString, NSNumber;
@class NSPort, NSRunLoop, NSPortNameServer, NSDictionary<KeyType, ObjectType>, NSArray<ObjectType>;
@class NSDistantObjectRequest;
@protocol NSConnectionDelegate;

NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE
NS_ASSUME_NONNULL_BEGIN

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSConnection : NSObject {
    @private
    id		receivePort;
    id		sendPort;
    id          delegate;
    int32_t	busy;
    int32_t	localProxyCount;
    int32_t	waitCount;
    id		delayedRL;
    id		statistics;
    unsigned char isDead;
    unsigned char isValid;
    unsigned char wantsInvalid;
    unsigned char authGen:1;
    unsigned char authCheck:1;
    unsigned char _reserved1:1;
    unsigned char _reserved2:1;
    unsigned char doRequest:1;
    unsigned char isQueueing:1;
    unsigned char isMulti:1;
    unsigned char invalidateRP:1;
    id          ___1;
    id          ___2;
    id          runLoops;
    id		requestModes;
    id          rootObject;
    void *	registerInfo;
    id		replMode;
    id          classInfoImported;
    id		releasedProxies;
    id		reserved;
}

@property (readonly, copy) NSDictionary<NSString *, NSNumber *> *statistics;

+ (NSArray<NSConnection *> *)allConnections;

+ (NSConnection *)defaultConnection API_DEPRECATED("", macos(10.0, 10.6)) API_UNAVAILABLE(ios, watchos, tvos);

+ (nullable instancetype)connectionWithRegisteredName:(NSString *)name host:(nullable NSString *)hostName;
+ (nullable instancetype)connectionWithRegisteredName:(NSString *)name host:(nullable NSString *)hostName usingNameServer:(NSPortNameServer *)server;
+ (nullable NSDistantObject *)rootProxyForConnectionWithRegisteredName:(NSString *)name host:(nullable NSString *)hostName;
+ (nullable NSDistantObject *)rootProxyForConnectionWithRegisteredName:(NSString *)name host:(nullable NSString *)hostName usingNameServer:(NSPortNameServer *)server;

+ (nullable instancetype)serviceConnectionWithName:(NSString *)name rootObject:(id)root usingNameServer:(NSPortNameServer *)server API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
+ (nullable instancetype)serviceConnectionWithName:(NSString *)name rootObject:(id)root API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@property NSTimeInterval requestTimeout;
@property NSTimeInterval replyTimeout;
@property (nullable, retain) id rootObject;
@property (nullable, assign) id<NSConnectionDelegate> delegate;
@property BOOL independentConversationQueueing;
@property (readonly, getter=isValid) BOOL valid;

@property (readonly, retain) NSDistantObject *rootProxy;

- (void)invalidate;

- (void)addRequestMode:(NSString *)rmode;
- (void)removeRequestMode:(NSString *)rmode;
@property (readonly, copy) NSArray<NSString *> *requestModes;

- (BOOL)registerName:(nullable NSString *) name;
- (BOOL)registerName:(nullable NSString *) name withNameServer:(NSPortNameServer *)server;

+ (nullable instancetype)connectionWithReceivePort:(nullable NSPort *)receivePort sendPort:(nullable NSPort *)sendPort;

+ (nullable id)currentConversation;

- (nullable instancetype)initWithReceivePort:(nullable NSPort *)receivePort sendPort:(nullable NSPort *)sendPort;

@property (readonly, retain) NSPort *sendPort;
@property (readonly, retain) NSPort *receivePort;

- (void)enableMultipleThreads;
@property (readonly) BOOL multipleThreadsEnabled;

- (void)addRunLoop:(NSRunLoop *)runloop;
- (void)removeRunLoop:(NSRunLoop *)runloop;

- (void)runInNewThread;

@property (readonly, copy) NSArray *remoteObjects;
@property (readonly, copy) NSArray *localObjects;

// NSPort subclasses should use this method to ask a connection object to dispatch Distributed Objects component data received over the wire. This will decode the data, authenticate, and send the message.
- (void)dispatchWithComponents:(NSArray *)components API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@end

FOUNDATION_EXPORT NSString * const NSConnectionReplyMode NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));

FOUNDATION_EXPORT NSString * const NSConnectionDidDieNotification NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));


API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@protocol NSConnectionDelegate <NSObject>
@optional

// Use the NSConnectionDidInitializeNotification notification instead
// of this delegate method if possible.
- (BOOL)makeNewConnection:(NSConnection *)conn sender:(NSConnection *)ancestor;

// Use the NSConnectionDidInitializeNotification notification instead
// of this delegate method if possible.
- (BOOL)connection:(NSConnection *)ancestor shouldMakeNewConnection:(NSConnection *)conn;

- (NSData *)authenticationDataForComponents:(NSArray *)components;
- (BOOL)authenticateComponents:(NSArray *)components withData:(NSData *)signature;

- (id)createConversationForConnection:(NSConnection *)conn;

- (BOOL)connection:(NSConnection *)connection handleRequest:(NSDistantObjectRequest *)doreq;

@end

FOUNDATION_EXPORT NSString * const NSFailedAuthenticationException NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));

FOUNDATION_EXPORT NSString * const NSConnectionDidInitializeNotification NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSDistantObjectRequest : NSObject

@property (readonly, retain) NSInvocation *invocation;
@property (readonly, retain) NSConnection *connection;
@property (readonly, retain) id conversation;
- (void)replyWithException:(nullable NSException *)exception;

@end

NS_ASSUME_NONNULL_END
