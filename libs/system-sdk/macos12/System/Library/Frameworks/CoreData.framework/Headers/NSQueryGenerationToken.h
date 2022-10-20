/*
    NSQueryGenerationToken.h
    Core Data
    Copyright (c) 2016-2021, Apple Inc.
    All rights reserved.
*/
#import <Foundation/NSArray.h>

NS_ASSUME_NONNULL_BEGIN

API_AVAILABLE(macosx(10.12),ios(10.0),tvos(10.0),watchos(3.0))
// Class used to track database generations for generational querying.
// See NSManagedObjectContext for details on how it is used.
@interface NSQueryGenerationToken : NSObject <NSCopying, NSSecureCoding>

@property (class, readonly, strong) NSQueryGenerationToken *currentQueryGenerationToken; // Used to inform a context that it should use the current generation

@end

NS_ASSUME_NONNULL_END

