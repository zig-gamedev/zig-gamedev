/*	NSPortMessage.h
	Copyright (c) 1994-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSPort, NSDate, NSArray, NSMutableArray;

NS_ASSUME_NONNULL_BEGIN

@interface NSPortMessage : NSObject {
    @private
    NSPort 		*localPort;
    NSPort 		*remotePort;
    NSMutableArray 	*components;
    uint32_t		msgid;
    void		*reserved2;
    void		*reserved;
}

- (instancetype)initWithSendPort:(nullable NSPort *)sendPort receivePort:(nullable NSPort *)replyPort components:(nullable NSArray *)components NS_DESIGNATED_INITIALIZER;

@property (nullable, readonly, copy) NSArray *components;
@property (nullable, readonly, retain) NSPort *receivePort;
@property (nullable, readonly, retain) NSPort *sendPort;
- (BOOL)sendBeforeDate:(NSDate *)date;

@property uint32_t msgid;

@end

NS_ASSUME_NONNULL_END
