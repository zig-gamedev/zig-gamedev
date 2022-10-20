/*	NSDistantObject.h
	Copyright (c) 1989-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSProxy.h>

@class Protocol, NSConnection, NSCoder;

NS_ASSUME_NONNULL_BEGIN

API_DEPRECATED("Use NSXPCConnection instead", macosx(10.0, 10.13), ios(2.0,11.0), watchos(2.0,4.0), tvos(9.0,11.0))
NS_SWIFT_UNAVAILABLE("Use NSXPCConnection instead")
@interface NSDistantObject : NSProxy <NSCoding> {
    @private
    id         _knownSelectors;
    NSUInteger _wireCount;
    NSUInteger _refCount;
    id         _proto;
    uint16_t   ___2;
    uint8_t    ___1;
    uint8_t    _wireType;
    id         _remoteClass;
}

// Do not attempt to subclass NSDistantObject -- it is futile
// Do not use these next two methods
+ (nullable id)proxyWithTarget:(id)target connection:(NSConnection *)connection;
- (nullable instancetype)initWithTarget:(id)target connection:(NSConnection *)connection;

+ (id)proxyWithLocal:(id)target connection:(NSConnection *)connection;
- (instancetype)initWithLocal:(id)target connection:(NSConnection *)connection;

- (nullable instancetype)initWithCoder:(NSCoder *)inCoder NS_DESIGNATED_INITIALIZER;

- (void)setProtocolForProxy:(nullable Protocol *)proto;

@property (readonly, retain) NSConnection *connectionForProxy;

@end

NS_ASSUME_NONNULL_END
