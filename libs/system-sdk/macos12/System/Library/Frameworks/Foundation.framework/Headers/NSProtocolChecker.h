/*	NSProtocolChecker.h
	Copyright (c) 1995-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSProxy.h>
#import <Foundation/NSObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSProtocolChecker : NSProxy

@property (readonly) Protocol *protocol;
@property (nullable, readonly, retain) NSObject *target;

@end

@interface NSProtocolChecker (NSProtocolCheckerCreation)

+ (instancetype)protocolCheckerWithTarget:(NSObject *)anObject protocol:(Protocol *)aProtocol;
- (instancetype)initWithTarget:(NSObject *)anObject protocol:(Protocol *)aProtocol;

@end

NS_ASSUME_NONNULL_END
