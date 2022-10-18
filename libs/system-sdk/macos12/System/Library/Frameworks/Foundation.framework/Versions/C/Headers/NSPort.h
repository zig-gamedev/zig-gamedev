/*	NSPort.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSRunLoop.h>

typedef int NSSocketNativeHandle;

@class NSRunLoop, NSMutableArray, NSDate;
@class NSConnection, NSPortMessage;
@class NSData;
@protocol NSPortDelegate, NSMachPortDelegate;

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSNotificationName const NSPortDidBecomeInvalidNotification;

@interface NSPort : NSObject <NSCopying, NSCoding>

// For backwards compatibility on Mach, +allocWithZone: returns
// an instance of the NSMachPort class when sent to the NSPort
// class.  Otherwise, it returns an instance of a concrete
// subclass which can be used for messaging between threads
// or processes on the local machine.

+ (NSPort *)port;

- (void)invalidate;
@property (readonly, getter=isValid) BOOL valid;

- (void)setDelegate:(nullable id <NSPortDelegate>)anObject;
- (nullable id <NSPortDelegate>)delegate;

// These two methods should be implemented by subclasses
// to setup monitoring of the port when added to a run loop,
// and stop monitoring if needed when removed;
// These methods should not be called directly!
- (void)scheduleInRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode;
- (void)removeFromRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode;

// DO Transport API; subclassers should implement these methods
// default is 0
@property (readonly) NSUInteger reservedSpaceLength;	
- (BOOL)sendBeforeDate:(NSDate *)limitDate components:(nullable NSMutableArray *)components from:(nullable NSPort *) receivePort reserved:(NSUInteger)headerSpaceReserved;
- (BOOL)sendBeforeDate:(NSDate *)limitDate msgid:(NSUInteger)msgID components:(nullable NSMutableArray *)components from:(nullable NSPort *)receivePort reserved:(NSUInteger)headerSpaceReserved;
	// The components array consists of a series of instances
	// of some subclass of NSData, and instances of some
	// subclass of NSPort; since one subclass of NSPort does
	// not necessarily know how to transport an instance of
	// another subclass of NSPort (or could do it even if it
	// knew about the other subclass), all of the instances
	// of NSPort in the components array and the 'receivePort'
	// argument MUST be of the same subclass of NSPort that
	// receives this message.  If multiple DO transports are
	// being used in the same program, this requires some care.

#if TARGET_OS_OSX || TARGET_OS_MACCATALYST
- (void)addConnection:(NSConnection *)conn toRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));
- (void)removeConnection:(NSConnection *)conn fromRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead") API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0));
	// The default implementation of these two methods is to
	// simply add the receiving port to the run loop in the
	// given mode.  Subclassers need not override these methods,
	// but can if they need to do extra work.
#endif

@end

@protocol NSPortDelegate <NSObject>
@optional

- (void)handlePortMessage:(NSPortMessage *)message;
	// This is the delegate method that subclasses should send
	// to their delegates, unless the subclass has something
	// more specific that it wants to try to send first
@end

#if TARGET_OS_OSX || TARGET_OS_IPHONE

NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE 
@interface NSMachPort : NSPort {
    @private
    id _delegate;
    NSUInteger _flags;
    uint32_t _machPort;
    NSUInteger _reserved;
}

+ (NSPort *)portWithMachPort:(uint32_t)machPort;
- (instancetype)initWithMachPort:(uint32_t)machPort NS_DESIGNATED_INITIALIZER;

- (void)setDelegate:(nullable id <NSMachPortDelegate>)anObject;
- (nullable id <NSMachPortDelegate>)delegate;

typedef NS_OPTIONS(NSUInteger, NSMachPortOptions) {
    NSMachPortDeallocateNone = 0,
    NSMachPortDeallocateSendRight = (1UL << 0),
    NSMachPortDeallocateReceiveRight = (1UL << 1)
} API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

+ (NSPort *)portWithMachPort:(uint32_t)machPort options:(NSMachPortOptions)f API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));
- (instancetype)initWithMachPort:(uint32_t)machPort options:(NSMachPortOptions)f API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;

@property (readonly) uint32_t machPort;

- (void)scheduleInRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode;
- (void)removeFromRunLoop:(NSRunLoop *)runLoop forMode:(NSRunLoopMode)mode;
	// If you subclass NSMachPort, you have to override these 2
	// methods from NSPort; since this is complicated, subclassing
	// NSMachPort is not recommended

@end

@protocol NSMachPortDelegate <NSPortDelegate>
@optional

// Delegates are sent this if they respond, otherwise they
// are sent handlePortMessage:; argument is the raw Mach message
- (void)handleMachMessage:(void *)msg;

@end

#endif

// A subclass of NSPort which can be used for local
// message sending on all platforms.
NS_AUTOMATED_REFCOUNT_WEAK_UNAVAILABLE 
@interface NSMessagePort : NSPort {
    @private
    void *_port;
    id _delegate;
}

@end

#if TARGET_OS_OSX || TARGET_IPHONE_SIMULATOR

// A subclass of NSPort which can be used for remote
// message sending on all platforms.

@interface NSSocketPort : NSPort {
    @private
    void *_receiver;
    id _connectors;
    void *_loops;
    void *_data;
    id _signature;
    id _delegate;
    id _lock;
    NSUInteger _maxSize;
    NSUInteger _useCount;
    NSUInteger _reserved;
}

- (instancetype)init;
- (nullable instancetype)initWithTCPPort:(unsigned short)port;
- (nullable instancetype)initWithProtocolFamily:(int)family socketType:(int)type protocol:(int)protocol address:(NSData *)address NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithProtocolFamily:(int)family socketType:(int)type protocol:(int)protocol socket:(NSSocketNativeHandle)sock NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initRemoteWithTCPPort:(unsigned short)port host:(nullable NSString *)hostName;
- (instancetype)initRemoteWithProtocolFamily:(int)family socketType:(int)type protocol:(int)protocol address:(NSData *)address NS_DESIGNATED_INITIALIZER;


@property (readonly) int protocolFamily;
@property (readonly) int socketType;
@property (readonly) int protocol;
@property (readonly, copy) NSData *address;
@property (readonly) NSSocketNativeHandle socket;

@end

#endif

NS_ASSUME_NONNULL_END
