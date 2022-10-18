/*
    NSFetchedPropertyDescription.h
    Core Data
    Copyright (c) 2004-2021, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <CoreData/NSPropertyDescription.h>

NS_ASSUME_NONNULL_BEGIN

@class NSFetchRequest;

// Fetched properties allow to specify related objects through a "weakly" resolved property, so there is no actual join necessary.
API_AVAILABLE(macosx(10.4),ios(3.0))
@interface NSFetchedPropertyDescription : NSPropertyDescription {
}

// As part of the predicate for a fetched property, you can use the two variables $FETCH_SOURCE (which is the managed object fetching the property) and $FETCHED_PROPERTY (which is the NSFetchedPropertyDescription instance).
@property (nullable, strong) NSFetchRequest *fetchRequest;

@end

NS_ASSUME_NONNULL_END
