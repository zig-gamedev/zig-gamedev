/*	NSPortNameServer.h
	Copyright (c) 1993-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSString, NSPort;

NS_ASSUME_NONNULL_BEGIN

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSPortNameServer : NSObject

+ (NSPortNameServer *)systemDefaultPortNameServer;

- (nullable NSPort *)portForName:(NSString *)name;
- (nullable NSPort *)portForName:(NSString *)name host:(nullable NSString *)host;

- (BOOL)registerPort:(NSPort *)port name:(NSString *)name;

- (BOOL)removePortForName:(NSString *)name;

@end

#if TARGET_OS_OSX || TARGET_OS_IPHONE

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSMachBootstrapServer : NSPortNameServer
	// This port name server actually takes and
	// returns instances of NSMachPort

+ (id)sharedInstance;

- (nullable NSPort *)portForName:(NSString *)name;
- (nullable NSPort *)portForName:(NSString *)name host:(nullable NSString *)host;
	// the bootstrap server is a local-only server;
	// host parameter must be emptry string or nil

- (BOOL)registerPort:(NSPort *)port name:(NSString *)name;

// removePortForName: functionality is not supported in
// the bootstrap server; if you want to cancel a service,
// you have to destroy the port (invalidate the NSMachPort
// given to registerPort:name:).

- (nullable NSPort *)servicePortWithName:(NSString *)name API_AVAILABLE(macos(10.5), ios(2.0), watchos(2.0), tvos(9.0));

@end

#endif

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSMessagePortNameServer : NSPortNameServer
	// This port name server actually takes and
	// returns instances of NSMessagePort

+ (id)sharedInstance;

- (nullable NSPort *)portForName:(NSString *)name;
- (nullable NSPort *)portForName:(NSString *)name host:(nullable NSString *)host;
	// this name server is a local-only server;
	// host parameter must be emptry string or nil

// removePortForName: functionality is not supported in
// this name server; if you want to cancel a service,
// you have to destroy the port (invalidate the
// NSMessagePort given to registerPort:name:).

@end

#if TARGET_OS_OSX

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSSocketPortNameServer : NSPortNameServer
	// This port name server actually takes and
	// returns instances of NSSocketPort

+ (id)sharedInstance;

- (nullable NSPort *)portForName:(NSString *)name;
- (nullable NSPort *)portForName:(NSString *)name host:(nullable NSString *)host;
    // this name server supports remote lookup
- (BOOL)registerPort:(NSPort *)port name:(NSString *)name;

- (BOOL)removePortForName:(NSString *)name;
     // removePortForName: is supported, and should be used

// the following may be used in the future, but for now nameServerPortNumber arguments are ignored
- (nullable NSPort *)portForName:(NSString *)name host:(nullable NSString *)host nameServerPortNumber:(uint16_t)portNumber;
- (BOOL)registerPort:(NSPort *)port name:(NSString *)name nameServerPortNumber:(uint16_t)portNumber;
@property uint16_t defaultNameServerPortNumber;

@end

#endif

NS_ASSUME_NONNULL_END
