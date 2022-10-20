/*	NSStream.h
        Copyright (c) 2003-2019, Apple Inc. All rights reserved
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSError.h>

@class NSData, NSDictionary, NSError, NSHost, NSInputStream, NSOutputStream, NSRunLoop, NSString, NSURL;
@protocol NSStreamDelegate;

typedef NSString * NSStreamPropertyKey NS_TYPED_EXTENSIBLE_ENUM;

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NSStreamStatus) {
    NSStreamStatusNotOpen = 0,
    NSStreamStatusOpening = 1,
    NSStreamStatusOpen = 2,
    NSStreamStatusReading = 3,
    NSStreamStatusWriting = 4,
    NSStreamStatusAtEnd = 5,
    NSStreamStatusClosed = 6,
    NSStreamStatusError = 7
};

typedef NS_OPTIONS(NSUInteger, NSStreamEvent) {
    NSStreamEventNone = 0,
    NSStreamEventOpenCompleted = 1UL << 0,
    NSStreamEventHasBytesAvailable = 1UL << 1,
    NSStreamEventHasSpaceAvailable = 1UL << 2,
    NSStreamEventErrorOccurred = 1UL << 3,
    NSStreamEventEndEncountered = 1UL << 4
};

// NSStream is an abstract class encapsulating the common API to NSInputStream and NSOutputStream.
// Subclassers of NSInputStream and NSOutputStream must also implement these methods.
@interface NSStream : NSObject
- (void)open;
- (void)close;

@property (nullable, assign) id <NSStreamDelegate> delegate;
    // By default, a stream is its own delegate, and subclassers of NSInputStream and NSOutputStream must maintain this contract. [someStream setDelegate:nil] must restore this behavior. As usual, delegates are not retained.

- (nullable id)propertyForKey:(NSStreamPropertyKey)key;
- (BOOL)setProperty:(nullable id)property forKey:(NSStreamPropertyKey)key;

- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSRunLoopMode)mode;

@property (readonly) NSStreamStatus streamStatus;
@property (nullable, readonly, copy) NSError *streamError;
@end

// NSInputStream is an abstract class representing the base functionality of a read stream.
// Subclassers are required to implement these methods.
@interface NSInputStream : NSStream
- (NSInteger)read:(uint8_t *)buffer maxLength:(NSUInteger)len;
    // reads up to length bytes into the supplied buffer, which must be at least of size len. Returns the actual number of bytes read.

- (BOOL)getBuffer:(uint8_t * _Nullable * _Nonnull)buffer length:(NSUInteger *)len;
    // returns in O(1) a pointer to the buffer in 'buffer' and by reference in 'len' how many bytes are available. This buffer is only valid until the next stream operation. Subclassers may return NO for this if it is not appropriate for the stream type. This may return NO if the buffer is not available.

@property (readonly) BOOL hasBytesAvailable;
    // returns YES if the stream has bytes available or if it impossible to tell without actually doing the read.

- (instancetype)initWithData:(NSData *)data NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithURL:(NSURL *)url API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;

@end

// NSOutputStream is an abstract class representing the base functionality of a write stream.
// Subclassers are required to implement these methods.
@interface NSOutputStream : NSStream
- (NSInteger)write:(const uint8_t *)buffer maxLength:(NSUInteger)len;
    // writes the bytes from the specified buffer to the stream up to len bytes. Returns the number of bytes actually written.

@property (readonly) BOOL hasSpaceAvailable;
    // returns YES if the stream can be written to or if it is impossible to tell without actually doing the write.

- (instancetype)initToMemory NS_DESIGNATED_INITIALIZER;
- (instancetype)initToBuffer:(uint8_t *)buffer capacity:(NSUInteger)capacity NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithURL:(NSURL *)url append:(BOOL)shouldAppend API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;

@end


@interface NSStream (NSSocketStreamCreationExtensions)

+ (void)getStreamsToHostWithName:(NSString *)hostname port:(NSInteger)port inputStream:(NSInputStream * _Nullable * _Nullable)inputStream outputStream:(NSOutputStream * _Nullable * _Nullable)outputStream API_DEPRECATED("Use nw_connection_t in Network framework instead", macos(10.10, API_TO_BE_DEPRECATED), ios(8.0, API_TO_BE_DEPRECATED), tvos(9.0, API_TO_BE_DEPRECATED)) API_UNAVAILABLE(watchos);

#if TARGET_OS_OSX
+ (void)getStreamsToHost:(NSHost *)host port:(NSInteger)port inputStream:(NSInputStream * _Nullable * _Nullable)inputStream outputStream:(NSOutputStream * _Nullable * _Nullable)outputStream API_DEPRECATED("Use nw_connection_t in Network framework instead", macos(10.3,10.10)) API_UNAVAILABLE(ios, watchos, tvos);
#endif

@end

@interface NSStream (NSStreamBoundPairCreationExtensions)
+ (void)getBoundStreamsWithBufferSize:(NSUInteger)bufferSize inputStream:(NSInputStream * _Nullable * _Nullable)inputStream outputStream:(NSOutputStream * _Nullable * _Nullable)outputStream API_AVAILABLE(macos(10.10), ios(8.0), watchos(2.0), tvos(9.0));
@end

// The NSInputStreamExtensions category contains additional initializers and convenience routines for dealing with NSInputStreams.
@interface NSInputStream (NSInputStreamExtensions)
- (nullable instancetype)initWithFileAtPath:(NSString *)path;

+ (nullable instancetype)inputStreamWithData:(NSData *)data;
+ (nullable instancetype)inputStreamWithFileAtPath:(NSString *)path;
+ (nullable instancetype)inputStreamWithURL:(NSURL *)url API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@end

// The NSOutputStreamExtensions category contains additional initializers and convenience routines for dealing with NSOutputStreams.
@interface NSOutputStream (NSOutputStreamExtensions)
- (nullable instancetype)initToFileAtPath:(NSString *)path append:(BOOL)shouldAppend;

+ (instancetype)outputStreamToMemory;
+ (instancetype)outputStreamToBuffer:(uint8_t *)buffer capacity:(NSUInteger)capacity;
+ (instancetype)outputStreamToFileAtPath:(NSString *)path append:(BOOL)shouldAppend;
+ (nullable instancetype)outputStreamWithURL:(NSURL *)url append:(BOOL)shouldAppend API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0));
@end

@protocol NSStreamDelegate <NSObject>
@optional
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
@end

// NSString constants for the propertyForKey/setProperty:forKey: API
// String constants for the setting of the socket security level.
FOUNDATION_EXPORT NSStreamPropertyKey const NSStreamSocketSecurityLevelKey		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));	// use this as the key for setting one of the following values for the security level of the target stream.

typedef NSString * NSStreamSocketSecurityLevel NS_TYPED_ENUM;

FOUNDATION_EXPORT NSStreamSocketSecurityLevel const NSStreamSocketSecurityLevelNone		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamSocketSecurityLevel const NSStreamSocketSecurityLevelSSLv2		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamSocketSecurityLevel const NSStreamSocketSecurityLevelSSLv3		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamSocketSecurityLevel const NSStreamSocketSecurityLevelTLSv1		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamSocketSecurityLevel const NSStreamSocketSecurityLevelNegotiatedSSL	API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));

FOUNDATION_EXPORT NSStreamPropertyKey const NSStreamSOCKSProxyConfigurationKey		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));	// Value is an NSDictionary containing the key/value pairs below. The dictionary returned from SystemConfiguration for SOCKS proxies will work without alteration.

typedef NSString * NSStreamSOCKSProxyConfiguration NS_TYPED_ENUM;

FOUNDATION_EXPORT NSStreamSOCKSProxyConfiguration const NSStreamSOCKSProxyHostKey			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is an NSString
FOUNDATION_EXPORT NSStreamSOCKSProxyConfiguration const NSStreamSOCKSProxyPortKey			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is an NSNumber
FOUNDATION_EXPORT NSStreamSOCKSProxyConfiguration const NSStreamSOCKSProxyVersionKey		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is one of NSStreamSOCKSProxyVersion4 or NSStreamSOCKSProxyVersion5
FOUNDATION_EXPORT NSStreamSOCKSProxyConfiguration const NSStreamSOCKSProxyUserKey			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is an NSString
FOUNDATION_EXPORT NSStreamSOCKSProxyConfiguration const NSStreamSOCKSProxyPasswordKey		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is an NSString

typedef NSString * NSStreamSOCKSProxyVersion NS_TYPED_ENUM;

FOUNDATION_EXPORT NSStreamSOCKSProxyVersion const NSStreamSOCKSProxyVersion4			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value for NSStreamSOCKProxyVersionKey
FOUNDATION_EXPORT NSStreamSOCKSProxyVersion const NSStreamSOCKSProxyVersion5			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value for NSStreamSOCKProxyVersionKey

FOUNDATION_EXPORT NSStreamPropertyKey const NSStreamDataWrittenToMemoryStreamKey	API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Key for obtaining the data written to a memory stream.

FOUNDATION_EXPORT NSStreamPropertyKey const NSStreamFileCurrentOffsetKey		API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // Value is an NSNumber representing the current absolute offset of the stream.

// NSString constants for error domains.
FOUNDATION_EXPORT NSErrorDomain const NSStreamSocketSSLErrorDomain			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));
    // SSL errors are to be interpreted via <Security/SecureTransport.h>
FOUNDATION_EXPORT NSErrorDomain const NSStreamSOCKSErrorDomain			API_AVAILABLE(macos(10.3), ios(2.0), watchos(2.0), tvos(9.0));

// Property key to specify the type of service for the stream.  This
// allows the system to properly handle the request with respect to
// routing, suspension behavior and other networking related attributes
// appropriate for the given service type.  The service types supported
// are documented below.
FOUNDATION_EXPORT NSStreamPropertyKey const NSStreamNetworkServiceType		    API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));

typedef NSString * NSStreamNetworkServiceTypeValue NS_TYPED_ENUM;

// Supported network service types:
FOUNDATION_EXPORT NSStreamNetworkServiceTypeValue const NSStreamNetworkServiceTypeVoIP	    API_AVAILABLE(macos(10.7), ios(4.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamNetworkServiceTypeValue const NSStreamNetworkServiceTypeVideo	    API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamNetworkServiceTypeValue const NSStreamNetworkServiceTypeBackground	    API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamNetworkServiceTypeValue const NSStreamNetworkServiceTypeVoice	    API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
FOUNDATION_EXPORT NSStreamNetworkServiceTypeValue const NSStreamNetworkServiceTypeCallSignaling    API_AVAILABLE(macosx(10.12), ios(10.0), watchos(3.0), tvos(10.0));

NS_ASSUME_NONNULL_END
