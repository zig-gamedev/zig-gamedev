/*
	NSScriptExecutionContext.h
	Copyright (c) 1997-2019, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>

@class NSConnection;

NS_ASSUME_NONNULL_BEGIN

@interface NSScriptExecutionContext : NSObject {
    @private
    id _topLevelObject;
    id _objectBeingTested;
    id _rangeContainerObject;
    id _moreVars;
}

+ (NSScriptExecutionContext *)sharedScriptExecutionContext;

@property (nullable, retain) id topLevelObject;
@property (nullable, retain) id objectBeingTested;
@property (nullable, retain) id rangeContainerObject;

@end

NS_ASSUME_NONNULL_END
